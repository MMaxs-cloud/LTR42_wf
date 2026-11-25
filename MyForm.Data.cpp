#include "MyForm.h"
#include "CycleData.h"
#include "Device.h"
#include <algorithm>

using namespace LTR42wf;
using namespace System::Globalization;
using namespace System::IO;

double CycleData::Duration() const
{
    return power.x.back() - power.x.front();
}

double CycleData::PowerAt(double elapsedMs) const
{
    const double time = power.x.front() + elapsedMs;
    const auto next = std::upper_bound(power.x.begin(), power.x.end(), time);
    if (next == power.x.begin()) return power.y.front();
    if (next == power.x.end()) return power.y.back();

    const auto index = next - power.x.begin();
    const double fraction = (time - power.x[index - 1]) / (power.x[index] - power.x[index - 1]);
    return power.y[index - 1] + fraction * (power.y[index] - power.y[index - 1]);
}

bool CycleData::HeaterAt(const std::vector<std::pair<double, bool>>& events, double elapsedMs)
{
    bool state = false;
    for (const auto& event : events) {
        if (elapsedMs < event.first) break;
        state = event.second;
    }
    return state;
}

namespace {
    String^ CsvPath(String^ folder, String^ name, String^ suffix)
    {
        name = name->Trim();
        if (name->EndsWith(L".csv", StringComparison::OrdinalIgnoreCase)) {
            name = name->Substring(0, name->Length - 4);
        }
        if (String::IsNullOrWhiteSpace(name) || name->IndexOfAny(Path::GetInvalidFileNameChars()) >= 0) {
            throw gcnew FormatException(L"Укажите имя CSV-файла из папки data.");
        }
        return Path::Combine(folder, name + suffix + L".csv");
    }

    double Number(String^ text)
    {
        double value;
        auto invariant = CultureInfo::InvariantCulture;
        if ((!Double::TryParse(text, NumberStyles::Float, invariant, value) &&
             !Double::TryParse(text->Replace(L',', L'.'), NumberStyles::Float, invariant, value)) ||
            Double::IsNaN(value) || Double::IsInfinity(value)) {
            throw gcnew FormatException(L"Ожидалось число: «" + text + L"».");
        }
        return value;
    }

    String^ CsvNumber(double value)
    {
        return value.ToString(L"R", CultureInfo::InvariantCulture);
    }

    String^ DisplayNumbers(const std::vector<double>& values)
    {
        auto result = gcnew System::Text::StringBuilder();
        for (double value : values) {
            if (result->Length > 0) result->Append(L"; ");
            result->Append(value.ToString(L"R", CultureInfo::InvariantCulture));
        }
        return result->ToString();
    }

    void AddPair(Samples& samples, String^ x, String^ y, bool calibration)
    {
        double first = Number(x);
        double second = Number(y);
        if (first < 0 || (!samples.x.empty() && first <= samples.x.back())) {
            throw gcnew FormatException(calibration
                ? L"Позиции должны быть неотрицательными и строго возрастать."
                : L"Время должно быть неотрицательным и строго возрастать.");
        }
        if (calibration && (first > ltr42::cyclePositionLimit ||
                            (!samples.y.empty() && second <= samples.y.back()))) {
            throw gcnew FormatException(L"Калибровка требует уникальных возрастающих значений мощности и позиций до 1450 тиков.");
        }
        samples.Add(first, second);
    }

    void AddHeater(std::vector<std::pair<double, bool>>& events,
                   array<String^>^ columns, int start)
    {
        if (columns->Length <= start) return;
        bool hasTime = !String::IsNullOrWhiteSpace(columns[start]);
        bool hasState = columns->Length > start + 1 && !String::IsNullOrWhiteSpace(columns[start + 1]);
        if (!hasState) return;

        int state;
        if (!hasTime || !Int32::TryParse(columns[start + 1], state) || (state != 0 && state != 1)) {
            throw gcnew FormatException(L"Для нагревателя укажите время и состояние 0 или 1.");
        }
        double time = Number(columns[start]);
        if (time < 0) throw gcnew FormatException(L"Время нагревателя должно быть неотрицательным.");
        events.emplace_back(time, state == 1);
    }

    void ReadCsv(String^ path, Samples& samples, CycleData* cycle)
    {
        int row = 0;
        for each (String^ line in File::ReadLines(path)) {
            ++row;
            if (String::IsNullOrWhiteSpace(line)) continue;
            try {
                auto columns = line->Split(L';');
                bool hasX = !String::IsNullOrWhiteSpace(columns[0]);
                bool hasY = columns->Length > 1 && !String::IsNullOrWhiteSpace(columns[1]);
                if (hasX || hasY) {
                    if (!hasX || !hasY) throw gcnew FormatException(L"Ожидались два значения.");
                    AddPair(samples, columns[0], columns[1], cycle == nullptr);
                }
                if (cycle != nullptr) {
                    AddHeater(cycle->alpha, columns, 2);
                    AddHeater(cycle->beta, columns, 4);
                }
            }
            catch (FormatException^ ex) {
                throw gcnew FormatException(String::Format(L"{0}, строка {1}. {2}", path, row, ex->Message));
            }
        }
    }

    void WriteCsv(String^ path, System::Collections::Generic::List<String^>^ lines)
    {
        Directory::CreateDirectory(Path::GetDirectoryName(path));
        String^ temporaryPath = path + L"." + Guid::NewGuid().ToString(L"N") + L".tmp";
        try {
            File::WriteAllLines(temporaryPath, lines);
            if (File::Exists(path)) File::Replace(temporaryPath, path, nullptr);
            else File::Move(temporaryPath, path);
        }
        finally {
            File::Delete(temporaryPath);
        }
    }

    void SortHeaterEvents(std::vector<std::pair<double, bool>>& events)
    {
        std::stable_sort(events.begin(), events.end(), [](const auto& left, const auto& right) {
            return left.first < right.first;
        });
    }

    Samples ReadPowerSeries(String^ times, String^ powers)
    {
        auto x = times->Split(L';');
        auto y = powers->Split(L';');
        int count = x->Length;
        int powerCount = y->Length;
        while (count > 0 && String::IsNullOrWhiteSpace(x[count - 1])) --count;
        while (powerCount > 0 && String::IsNullOrWhiteSpace(y[powerCount - 1])) --powerCount;
        if (count != powerCount || count < 2) {
            throw gcnew FormatException(L"Укажите не менее двух пар времени и мощности. Разделяйте значения точкой с запятой.");
        }

        Samples result;
        for (int i = 0; i < count; ++i) AddPair(result, x[i], y[i], false);
        return result;
    }
}

void MyForm::LoadCalibration(Object^, EventArgs^)
{
    try {
        Samples next;
        ReadCsv(CsvPath(dataDirectory, calibrationName->Text, L""), next, nullptr);
        if (next.x.size() < 3) {
            throw gcnew FormatException(L"В калибровке должно быть не менее трёх точек.");
        }
        data->calibration = std::move(next);

        calibrationSummary->Text = String::Format(L"Загружено точек: {0}    •    {1}–{2} тиков",
            static_cast<int>(data->calibration.x.size()), data->calibration.x.front(), data->calibration.x.back());
        ShowStatus(L"Калибровка загружена.", false);
    }
    catch (Exception^ ex) {
        ShowStatus(ex->Message, true);
    }
}

void MyForm::ExportCalibration(Object^, EventArgs^)
{
    if (data->calibration.x.empty()) {
        ShowStatus(L"Сначала загрузите калибровку.", true);
        return;
    }
    try {
        auto path = CsvPath(dataDirectory, L"calibration_curve", L"");
        auto lines = gcnew System::Collections::Generic::List<String^>();
        for (int tick = 0; tick <= data->calibration.x.back(); tick += ltr42::positionStep) {
            lines->Add(CsvNumber(tick) + L";" + CsvNumber(data->PowerAtPosition(tick)));
        }
        WriteCsv(path, lines);
        ShowStatus(L"Сохранено в папке data: " + Path::GetFileName(path), false);
    }
    catch (Exception^ ex) {
        ShowStatus(ex->Message, true);
    }
}

void MyForm::LoadCycle(Object^, EventArgs^)
{
    try {
        CycleData next;
        ReadCsv(CsvPath(dataDirectory, cycleName->Text, L""), next.power, &next);
        if (next.power.x.size() < 2) throw gcnew FormatException(L"В циклограмме должно быть не менее двух точек мощности.");
        SortHeaterEvents(next.alpha);
        SortHeaterEvents(next.beta);
        cycleTimes->Text = DisplayNumbers(next.power.x);
        cyclePowers->Text = DisplayNumbers(next.power.y);
        data->cycle = std::move(next);
        cycleSummary->Text = String::Format(L"Точек: {0}    •    Событий «Альфа»: {1}    •    Событий «Бета»: {2}",
            static_cast<int>(data->cycle.power.x.size()), static_cast<int>(data->cycle.alpha.size()),
            static_cast<int>(data->cycle.beta.size()));
        ShowStatus(L"Циклограмма загружена.", false);
    }
    catch (Exception^ ex) {
        ShowStatus(ex->Message, true);
    }
}

void MyForm::ExportCycle(Object^, EventArgs^)
{
    try {
        auto power = ReadPowerSeries(cycleTimes->Text, cyclePowers->Text);
        auto path = CsvPath(dataDirectory, calibrationName->Text, L"_cycle");
        auto lines = gcnew System::Collections::Generic::List<String^>();
        for (size_t i = 0; i < power.x.size(); ++i) {
            lines->Add(CsvNumber(power.x[i]) + L";" + CsvNumber(power.y[i]));
        }
        WriteCsv(path, lines);
        ShowStatus(L"Сохранено в папке data: " + Path::GetFileName(path), false);
    }
    catch (Exception^ ex) {
        ShowStatus(ex->Message, true);
    }
}

void MyForm::ReadPowerInputs()
{
    auto power = ReadPowerSeries(cycleTimes->Text, cyclePowers->Text);
    data->cycle.power = std::move(power);
}
