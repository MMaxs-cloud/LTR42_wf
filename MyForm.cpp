#include "MyForm.h"
#include "CycleData.h"
#include "Device.h"
#include "header.h"
#include <Windows.h>
#include <cmath>
#include <mutex>
#include "utils/enlog_to_file.h"

using namespace LTR42wf;
using namespace System::Globalization;

namespace {
    String^ ResolveDataDirectory()
    {
        auto localData = IO::Path::GetFullPath(L"data");
        if (IO::Directory::Exists(localData)) return localData;

        auto executableDirectory = gcnew IO::DirectoryInfo(Application::StartupPath);
        auto adjacentData = IO::Path::Combine(executableDirectory->FullName, L"data");
        if (IO::Directory::Exists(adjacentData)) return adjacentData;

        for (auto parent = executableDirectory->Parent; parent != nullptr; parent = parent->Parent) {
            if (IO::File::Exists(IO::Path::Combine(parent->FullName, L"LTR42_wf.vcxproj"))) {
                return IO::Path::Combine(parent->FullName, L"data");
            }
        }

        return localData;
    }

    void WriteLog(const std::string& message, rt::LogLevel level)
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        try {
            static RammusLib::Log::CAppFileLog log;
            log.SendMessageToLog(level, message);
        }
        catch (const std::exception&) {
            OutputDebugStringA(message.c_str());
        }
    }
}

rt::Logger::Logger()
{
    init(WriteLog);
}

void rt::RTEngineLogger::write_to_log(const std::string& message)
{
    static rt::Logger logger;
    logger.info(message);
}

[STAThreadAttribute]
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm());
    return 0;
}

MyForm::MyForm()
{
    dataDirectory = ResolveDataDirectory();
    InitializeComponent();

    elapsed = gcnew Diagnostics::Stopwatch();
    motionTimer = gcnew Timer();
    motionTimer->Interval = 15;
    motionTimer->Tick += gcnew EventHandler(this, &MyForm::MotionTick);
    scheduleTimer = gcnew Timer();
    scheduleTimer->Interval = 1000;
    scheduleTimer->Tick += gcnew EventHandler(this, &MyForm::ScheduleTick);
    cycleTimer = gcnew Timer();
    cycleTimer->Interval = 300;
    cycleTimer->Tick += gcnew EventHandler(this, &MyForm::CycleTick);

    data = new RunData();
    UpdateControls();
    ENLOG("Form loaded");
}

MyForm::~MyForm()
{
    delete cycleTimer;
    delete scheduleTimer;
    delete motionTimer;
    delete data;
    data = nullptr;
    ltr42::Close();
}

void MyForm::OnFormClosing(FormClosingEventArgs^ e)
{
    Form::OnFormClosing(e);
    if (e->Cancel) return;
    StopTimers();
    SavePosition();
    ltr42::Close();
    ENLOG("Form closed");
}

void MyForm::UpdateControls()
{
    bool idle = operation == Operation::Idle;
    bool manualEnabled = connected && idle;
    connectButton->Enabled = !connected && idle;
    moveButton->Enabled = manualEnabled;
    zeroButton->Enabled = manualEnabled;
    heaterButtons->Enabled = manualEnabled;
    const bool canCancel = operation == Operation::Aligning || operation == Operation::Waiting;
    startButton->Enabled = manualEnabled || canCancel;
    startButton->Text = canCancel ? L"Отменить запуск" : L"Запланировать";
    calibrationCard->Enabled = idle;
    for each (Control^ control in cycleCard->Controls) {
        if (control != startButton && dynamic_cast<Label^>(control) == nullptr) control->Enabled = idle;
    }
    connectionLabel->Text = connected ? L"Подключён" : L"Нет подключения";
    connectionLabel->ForeColor = connected ? Color::FromArgb(167, 243, 208) : Color::FromArgb(203, 213, 225);
}

void MyForm::ShowStatus(String^ message, bool error)
{
    statusLabel->Text = message;
    statusLabel->ForeColor = error ? Color::FromArgb(153, 27, 27) : Color::FromArgb(30, 64, 88);
}

void MyForm::UpdatePosition()
{
    positionValue->Text = position.ToString() + L" тиков";
    cyclePositionValue->Text = position.ToString();
}

bool MyForm::AcceptDeviceResult(int code)
{
    if (code == 0) return true;

    StopTimers();
    ltr42::Close();
    connected = false;
    operation = Operation::Idle;
    UpdateControls();
    ShowStatus(String::Format(L"Ошибка LTR42, код {0}. Проверьте подключение и положение привода.", code), true);
    ENLOG("LTR42 error: ")(code);
    return false;
}

void MyForm::Connect(Object^, EventArgs^)
{
    if (!AcceptDeviceResult(ltr42::Connect())) return;
    connected = true;
    UpdateControls();
    ShowStatus(L"LTR42 подключён. Канал «Бета» включён.", false);
    ENLOG("LTR42 connected; Beta output ON");
}

void MyForm::Move(Object^, EventArgs^)
{
    int target;
    int seconds;
    if (!Int32::TryParse(targetPositionInput->Text, target) || target < 0 ||
        target > ltr42::manualPositionLimit || target % ltr42::positionStep != 0) {
        ShowStatus(L"Позиция должна быть от 0 до 1425 тиков с шагом 25.", true);
        return;
    }
    if (!Int32::TryParse(durationInput->Text, seconds) || seconds < 0) {
        ShowStatus(L"Укажите длительность в целых неотрицательных секундах.", true);
        return;
    }

    ShowStatus(L"Перемещение привода…", false);
    BeginMotion(target, seconds * 1000.0, Operation::Manual);
}

void MyForm::MoveToZero(Object^, EventArgs^)
{
    if (!AcceptDeviceResult(ltr42::MoveToZero())) return;
    position = 0;
    positionChanged = true;
    UpdatePosition();
    ShowStatus(L"Команда возврата в ноль выполнена.", false);
    ENLOG("Position reset to zero");
}

void MyForm::SetHeaterOutput(Object^ sender, EventArgs^)
{
    auto button = safe_cast<Button^>(sender);
    int output = safe_cast<int>(button->Tag);
    if (!AcceptDeviceResult(ltr42::Write(static_cast<unsigned short>(output)))) return;
    ShowStatus(button->Text + L". Команда выполнена.", false);
    ENLOG("Manual heater output: ")(output);
}

void MyForm::StartCycle(Object^, EventArgs^)
{
    if (operation == Operation::Aligning || operation == Operation::Waiting) {
        StopTimers();
        operation = Operation::Idle;
        UpdateControls();
        ShowStatus(L"Запуск циклограммы отменён.", false);
        return;
    }

    if (data->calibration.x.empty()) {
        ShowStatus(L"Сначала загрузите калибровку.", true);
        return;
    }

    DateTime requestedTime;
    auto formats = gcnew array<String^>{ L"H:mm", L"H:mm:ss" };
    if (!DateTime::TryParseExact(startTimeInput->Text->Trim(), formats, CultureInfo::InvariantCulture,
                                DateTimeStyles::None, requestedTime)) {
        ShowStatus(L"Введите время запуска в формате ЧЧ:ММ или ЧЧ:ММ:СС.", true);
        return;
    }

    try {
        ReadPowerInputs();
    }
    catch (FormatException^ ex) {
        ShowStatus(ex->Message, true);
        return;
    }

    DateTime now = DateTime::Now;
    scheduledStart = now.Date + requestedTime.TimeOfDay;
    if (scheduledStart < now) scheduledStart = scheduledStart.AddDays(1);

    // Keep the one-step offset used for the original initial approach.
    double initialPosition = data->PositionAtPower(data->cycle.power.y.front());
    if (!std::isfinite(initialPosition)) {
        ShowStatus(L"По калибровке не удалось рассчитать начальную позицию.", true);
        return;
    }
    initialPosition = Math::Max(0.0, Math::Min(initialPosition, ltr42::cyclePositionLimit + 25.0));
    int initialTarget = (static_cast<int>(initialPosition) + 24) / ltr42::positionStep * ltr42::positionStep;
    initialTarget = Math::Max(0, initialTarget - ltr42::positionStep);

    measuredPowerValue->Text = L"—";
    targetPowerValue->Text = L"—";
    elapsedValue->Text = L"—";
    ShowStatus(L"Перемещение к начальной позиции циклограммы…", false);
    BeginMotion(initialTarget, 0, Operation::Aligning);
}

void MyForm::BeginMotion(int target, double durationMs, Operation nextOperation)
{
    operation = nextOperation;
    motionStart = position;
    motionTarget = target;
    motionDurationMs = durationMs;
    elapsed->Restart();
    UpdateControls();
    motionTimer->Start();
}

void MyForm::MotionTick(Object^, EventArgs^)
{
    if (position != motionTarget) {
        int nextPosition = position + (motionTarget > position ? ltr42::positionStep : -ltr42::positionStep);
        if (motionDurationMs > 0) {
            double progress = std::abs(nextPosition - motionStart) / static_cast<double>(std::abs(motionTarget - motionStart));
            if (elapsed->Elapsed.TotalMilliseconds < motionDurationMs * progress) return;
        }

        unsigned short output = ltr42::Enable | ltr42::Beta;
        if (nextPosition > position) output |= ltr42::Forward;
        if (!AcceptDeviceResult(ltr42::Step(output, 10))) return;
        position = nextPosition;
        positionChanged = true;
        UpdatePosition();
    }

    if (position != motionTarget) return;
    motionTimer->Stop();
    if (operation == Operation::Aligning) {
        QueueCycle();
        return;
    }

    operation = Operation::Idle;
    UpdateControls();
    ShowStatus(L"Перемещение завершено.", false);
}

void MyForm::QueueCycle()
{
    operation = Operation::Waiting;
    ShowStatus(L"Запуск запланирован на " + scheduledStart.ToString(L"dd.MM.yyyy HH:mm:ss") + L".", false);
    scheduleTimer->Start();
    ScheduleTick(nullptr, EventArgs::Empty);
}

void MyForm::ScheduleTick(Object^, EventArgs^)
{
    if (DateTime::Now < scheduledStart) return;
    scheduleTimer->Stop();
    operation = Operation::Running;
    elapsed->Restart();
    cycleTimer->Start();
    ShowStatus(L"Циклограмма выполняется.", false);
    ENLOG("Cyclogram started");
}

void MyForm::CycleTick(Object^, EventArgs^)
{
    double elapsedMs = elapsed->Elapsed.TotalMilliseconds;
    if (elapsedMs >= data->cycle.Duration()) {
        StopTimers();
        SavePosition();
        operation = Operation::Idle;
        UpdateControls();
        ShowStatus(L"Циклограмма завершена.", false);
        ENLOG("Cyclogram finished");
        return;
    }

    double targetPower = data->cycle.PowerAt(elapsedMs);
    double target = data->PositionAtPower(targetPower);
    if (!std::isfinite(target)) {
        StopTimers();
        operation = Operation::Idle;
        UpdateControls();
        ShowStatus(L"Циклограмма остановлена. Не удалось рассчитать позицию.", true);
        return;
    }
    target = Math::Max(0.0, Math::Min(target, static_cast<double>(ltr42::cyclePositionLimit)));
    int targetPosition = static_cast<int>(std::round(target / ltr42::positionStep)) * ltr42::positionStep;

    unsigned short output = 0;
    if (CycleData::HeaterAt(data->cycle.alpha, elapsedMs)) output |= ltr42::Alpha;
    if (CycleData::HeaterAt(data->cycle.beta, elapsedMs)) output |= ltr42::Beta;

    if (targetPosition == position) {
        if (!AcceptDeviceResult(ltr42::Write(output))) return;
    }
    else {
        output |= ltr42::Enable;
        int delta = targetPosition > position ? ltr42::positionStep : -ltr42::positionStep;
        if (delta > 0) output |= ltr42::Forward;
        if (!AcceptDeviceResult(ltr42::Step(output, 5))) return;
        position += delta;
        positionChanged = true;
        UpdatePosition();
    }

    double power = data->PowerAtPosition(position);
    measuredPowerValue->Text = power.ToString(L"F2");
    targetPowerValue->Text = targetPower.ToString(L"F2");
    elapsedValue->Text = elapsedMs.ToString(L"F0");
    ENLOG("Elapsed ms: ")(elapsedMs)(" target: ")(targetPosition)(" position: ")(static_cast<int>(position))
        (" power: ")(power)(" target power: ")(targetPower)(" output: ")(output);
}

void MyForm::StopTimers()
{
    motionTimer->Stop();
    scheduleTimer->Stop();
    cycleTimer->Stop();
}

void MyForm::SavePosition()
{
    if (!positionChanged) return;
    try {
        auto path = IO::Path::Combine(IO::Path::GetDirectoryName(dataDirectory), L"position.txt");
        IO::File::WriteAllText(path, position.ToString(CultureInfo::InvariantCulture) + Environment::NewLine);
        positionChanged = false;
    }
    catch (IO::IOException^) {
        ENLOG("Could not save position.txt");
    }
    catch (UnauthorizedAccessException^) {
        ENLOG("Could not write position.txt");
    }
}
