#include "MyForm.h"
#include "Device.h"

using namespace LTR42wf;

namespace {
    Color Ink() { return Color::FromArgb(20, 35, 52); }
    Color Muted() { return Color::FromArgb(73, 93, 113); }
    Color Accent() { return Color::FromArgb(0, 99, 110); }

    TableLayoutPanel^ Grid(int columns)
    {
        auto grid = gcnew TableLayoutPanel();
        grid->ColumnCount = columns;
        grid->Dock = DockStyle::Fill;
        grid->Margin = System::Windows::Forms::Padding(0);
        return grid;
    }

    void Row(TableLayoutPanel^ grid, int height)
    {
        ++grid->RowCount;
        grid->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, static_cast<float>(height)));
    }

    Label^ Copy(String^ text, float size, Color color, FontStyle style)
    {
        auto label = gcnew Label();
        label->Text = text;
        label->Font = gcnew Drawing::Font(L"Segoe UI", size, style);
        label->ForeColor = color;
        label->Dock = DockStyle::Fill;
        label->TextAlign = ContentAlignment::MiddleLeft;
        label->Margin = System::Windows::Forms::Padding(0);
        label->AutoEllipsis = true;
        return label;
    }

    Label^ Caption(String^ text)
    {
        return Copy(text, 9.5f, Muted(), FontStyle::Regular);
    }

    TableLayoutPanel^ Card(int columns)
    {
        auto card = Grid(columns);
        card->Padding = System::Windows::Forms::Padding(16, 8, 16, 10);
        card->BackColor = Color::White;
        card->CellBorderStyle = TableLayoutPanelCellBorderStyle::None;
        return card;
    }

    void Heading(TableLayoutPanel^ card, String^ title)
    {
        auto heading = Copy(title, 13.0f, Ink(), FontStyle::Bold);
        card->Controls->Add(heading, 0, 0);
        card->SetColumnSpan(heading, card->ColumnCount);
    }

    TextBox^ Input(String^ text, String^ name)
    {
        auto box = gcnew TextBox();
        box->Text = text;
        box->AccessibleName = name;
        box->Dock = DockStyle::Fill;
        box->Margin = System::Windows::Forms::Padding(0, 2, 8, 2);
        box->BorderStyle = BorderStyle::FixedSingle;
        box->BackColor = Color::FromArgb(246, 249, 252);
        box->ForeColor = Ink();
        box->Font = gcnew Drawing::Font(L"Segoe UI", 11.0f);
        return box;
    }

    void StylePrimaryButton(Object^ sender, EventArgs^)
    {
        auto button = safe_cast<Button^>(sender);
        button->BackColor = button->Enabled ? Accent() : Color::FromArgb(232, 239, 245);
        button->ForeColor = button->Enabled ? Color::White : Ink();
        button->FlatAppearance->BorderColor = button->Enabled ? Accent() : Color::FromArgb(204, 217, 227);
    }

    Button^ MakeButton(String^ text, bool primary)
    {
        auto button = gcnew Button();
        button->Text = text;
        button->Dock = DockStyle::Fill;
        button->Margin = System::Windows::Forms::Padding(0, 2, 8, 2);
        button->FlatStyle = FlatStyle::Flat;
        button->FlatAppearance->BorderSize = 1;
        button->FlatAppearance->BorderColor = Color::FromArgb(204, 217, 227);
        button->FlatAppearance->MouseOverBackColor = primary
            ? Color::FromArgb(0, 79, 88) : Color::FromArgb(227, 239, 245);
        button->BackColor = Color::FromArgb(240, 246, 250);
        button->ForeColor = Ink();
        button->UseVisualStyleBackColor = false;
        button->Font = gcnew Drawing::Font(L"Segoe UI", 9.5f, FontStyle::Bold);
        if (primary) {
            StylePrimaryButton(button, EventArgs::Empty);
            button->EnabledChanged += gcnew EventHandler(StylePrimaryButton);
        }
        return button;
    }

    void Metric(TableLayoutPanel^ grid, int column, String^ title, Label^% value)
    {
        auto tile = Grid(1);
        tile->BackColor = Color::FromArgb(217, 232, 239);
        tile->Padding = System::Windows::Forms::Padding(12, 5, 8, 5);
        tile->Margin = System::Windows::Forms::Padding(0, 0, 8, 0);
        Row(tile, 23);
        Row(tile, 28);
        tile->Controls->Add(Caption(title), 0, 0);
        value = Copy(L"—", 13.0f, Ink(), FontStyle::Bold);
        tile->Controls->Add(value, 0, 1);
        grid->Controls->Add(tile, column, 0);
    }
}

void MyForm::InitializeComponent()
{
    SuspendLayout();
    Text = L"LTR42 — управление стендом";
    AccessibleName = Text;
    Font = gcnew Drawing::Font(L"Segoe UI", 10.0f);
    AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
    AutoScaleDimensions = SizeF(96.0f, 96.0f);
    ClientSize = Drawing::Size(1100, 748);
    MinimumSize = Drawing::Size(790, 540);
    StartPosition = FormStartPosition::CenterScreen;
    BackColor = Color::FromArgb(234, 240, 246);
    ForeColor = Ink();

    auto shell = Grid(1);
    Row(shell, 80);
    shell->RowCount = 3;
    shell->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
    shell->RowStyles->Add(gcnew RowStyle(SizeType::Absolute, 40));
    Controls->Add(shell);

    auto header = Grid(3);
    header->Padding = System::Windows::Forms::Padding(20, 8, 12, 8);
    header->BackColor = Color::FromArgb(17, 35, 53);
    header->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));
    header->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 162));
    header->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 142));
    Row(header, 36);
    Row(header, 26);
    header->Controls->Add(Copy(L"Управление стендом LTR42", 18.0f, Color::White, FontStyle::Bold), 0, 0);
    header->Controls->Add(Copy(L"Привод, нагреватели и циклограммы", 10.0f,
        Color::FromArgb(192, 211, 226), FontStyle::Regular), 0, 1);
    connectionLabel = Copy(L"", 10.0f, Color::White, FontStyle::Bold);
    connectionLabel->TextAlign = ContentAlignment::MiddleCenter;
    header->Controls->Add(connectionLabel, 1, 0);
    header->SetRowSpan(connectionLabel, 2);

    connectButton = MakeButton(L"Подключить", true);
    connectButton->Margin = System::Windows::Forms::Padding(0, 12, 8, 12);
    connectButton->Click += gcnew EventHandler(this, &MyForm::Connect);
    header->Controls->Add(connectButton, 2, 0);
    header->SetRowSpan(connectButton, 2);
    shell->Controls->Add(header, 0, 0);

    auto scroll = gcnew Panel();
    scroll->AutoScroll = true;
    scroll->Dock = DockStyle::Fill;
    scroll->Margin = System::Windows::Forms::Padding(0);
    shell->Controls->Add(scroll, 0, 1);

    auto body = Grid(2);
    body->Dock = DockStyle::Top;
    body->AutoSize = true;
    body->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
    body->Padding = System::Windows::Forms::Padding(16, 12, 16, 8);
    body->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 62));
    body->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 38));
    Row(body, 150);
    Row(body, 120);
    Row(body, 226);
    Row(body, 78);
    scroll->Controls->Add(body);

    auto drive = Card(3);
    drive->Margin = System::Windows::Forms::Padding(0, 0, 10, 8);
    for (int i = 0; i < 3; ++i) drive->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100.0f / 3));
    Row(drive, 29);
    Row(drive, 22);
    Row(drive, 30);
    Row(drive, 35);
    Heading(drive, L"Привод");
    drive->Controls->Add(Caption(L"Цель, тики (0–1425)"), 0, 1);
    drive->Controls->Add(Caption(L"Длительность, с"), 1, 1);
    drive->Controls->Add(Caption(L"Текущая позиция"), 2, 1);
    targetPositionInput = Input(L"0", L"Целевая позиция, тики");
    durationInput = Input(L"0", L"Длительность перемещения, секунды");
    positionValue = Copy(L"0 тиков", 12.0f, Ink(), FontStyle::Bold);
    drive->Controls->Add(targetPositionInput, 0, 2);
    drive->Controls->Add(durationInput, 1, 2);
    drive->Controls->Add(positionValue, 2, 2);
    moveButton = MakeButton(L"Переместить", true);
    zeroButton = MakeButton(L"Возврат в ноль", false);
    moveButton->Click += gcnew EventHandler(this, &MyForm::Move);
    zeroButton->Click += gcnew EventHandler(this, &MyForm::MoveToZero);
    drive->Controls->Add(moveButton, 0, 3);
    drive->Controls->Add(zeroButton, 1, 3);
    body->Controls->Add(drive, 0, 0);

    auto heaters = Card(1);
    heaters->Margin = System::Windows::Forms::Padding(0, 0, 0, 8);
    Row(heaters, 29);
    Row(heaters, 76);
    Heading(heaters, L"Нагреватели");
    auto heaterGrid = Grid(2);
    heaterGrid->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50));
    heaterGrid->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50));
    Row(heaterGrid, 37);
    Row(heaterGrid, 37);
    heaterGrid->RowCount = 3;
    heaterGrid->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100));
    auto heaterNames = gcnew array<String^>{ L"Альфа · Вкл.", L"Альфа · Выкл.", L"Бета · Вкл.", L"Бета · Выкл." };
    auto outputs = gcnew array<int>{ ltr42::Alpha | ltr42::Beta, 0, ltr42::Beta, 0 };
    for (int i = 0; i < heaterNames->Length; ++i) {
        auto button = MakeButton(heaterNames[i], false);
        button->Tag = outputs[i];
        button->Click += gcnew EventHandler(this, &MyForm::SetHeaterOutput);
        heaterGrid->Controls->Add(button, i % 2, i / 2);
    }
    heaterButtons = heaterGrid;
    heaters->Controls->Add(heaterGrid, 0, 1);
    body->Controls->Add(heaters, 1, 0);

    calibrationCard = Card(3);
    calibrationCard->Margin = System::Windows::Forms::Padding(0, 0, 0, 8);
    calibrationCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));
    calibrationCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 142));
    calibrationCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 150));
    Row(calibrationCard, 28);
    Row(calibrationCard, 34);
    Row(calibrationCard, 26);
    Heading(calibrationCard, L"Калибровка привода");
    auto defaultCalibration = IO::File::Exists(IO::Path::Combine(dataDirectory, L"20260616_ticks-power.csv"))
        ? L"20260616_ticks-power" : L"20260316_ticks-power";
    calibrationName = Input(gcnew String(defaultCalibration), L"Имя файла калибровки в папке data");
    calibrationName->Margin = System::Windows::Forms::Padding(0, 4, 8, 2);
    auto loadCalibration = MakeButton(L"Загрузить CSV", false);
    auto exportCalibration = MakeButton(L"Выгрузить кривую", false);
    loadCalibration->Click += gcnew EventHandler(this, &MyForm::LoadCalibration);
    exportCalibration->Click += gcnew EventHandler(this, &MyForm::ExportCalibration);
    calibrationCard->Controls->Add(calibrationName, 0, 1);
    calibrationCard->Controls->Add(loadCalibration, 1, 1);
    calibrationCard->Controls->Add(exportCalibration, 2, 1);
    calibrationSummary = Caption(L"Папка data · калибровка ещё не загружена");
    calibrationCard->Controls->Add(calibrationSummary, 0, 2);
    calibrationCard->SetColumnSpan(calibrationSummary, 3);
    body->Controls->Add(calibrationCard, 0, 1);
    body->SetColumnSpan(calibrationCard, 2);

    cycleCard = Card(4);
    cycleCard->Margin = System::Windows::Forms::Padding(0, 0, 0, 8);
    cycleCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 162));
    cycleCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100));
    cycleCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 142));
    cycleCard->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Absolute, 150));
    Row(cycleCard, 29);
    Row(cycleCard, 34);
    Row(cycleCard, 31);
    Row(cycleCard, 31);
    Row(cycleCard, 37);
    Row(cycleCard, 24);
    Heading(cycleCard, L"Циклограмма");
    cycleName = Input(L"CD2E2R3H2", L"Имя файла циклограммы в папке data");
    cycleName->Margin = System::Windows::Forms::Padding(0, 4, 8, 2);
    auto loadCycle = MakeButton(L"Загрузить CSV", false);
    auto exportCycle = MakeButton(L"Выгрузить CSV", false);
    loadCycle->Click += gcnew EventHandler(this, &MyForm::LoadCycle);
    exportCycle->Click += gcnew EventHandler(this, &MyForm::ExportCycle);
    cycleCard->Controls->Add(Caption(L"CSV в папке data"), 0, 1);
    cycleCard->Controls->Add(cycleName, 1, 1);
    cycleCard->Controls->Add(loadCycle, 2, 1);
    cycleCard->Controls->Add(exportCycle, 3, 1);
    cycleTimes = Input(L"", L"Время в миллисекундах, через точку с запятой");
    cyclePowers = Input(L"", L"Мощность, через точку с запятой");
    cycleCard->Controls->Add(Caption(L"Время, мс"), 0, 2);
    cycleCard->Controls->Add(cycleTimes, 1, 2);
    cycleCard->SetColumnSpan(cycleTimes, 3);
    cycleCard->Controls->Add(Caption(L"Мощность"), 0, 3);
    cycleCard->Controls->Add(cyclePowers, 1, 3);
    cycleCard->SetColumnSpan(cyclePowers, 3);
    startTimeInput = Input(L"", L"Время запуска, часы и минуты");
    startTimeInput->Margin = System::Windows::Forms::Padding(0, 6, 8, 2);
    startButton = MakeButton(L"Запланировать", true);
    startButton->Click += gcnew EventHandler(this, &MyForm::StartCycle);
    cycleCard->Controls->Add(Caption(L"Запуск, ЧЧ:ММ[:СС]"), 0, 4);
    cycleCard->Controls->Add(startTimeInput, 1, 4);
    cycleCard->Controls->Add(startButton, 2, 4);
    cycleCard->SetColumnSpan(startButton, 2);
    cycleSummary = Caption(L"Разделяйте значения точкой с запятой");
    cycleCard->Controls->Add(cycleSummary, 0, 5);
    cycleCard->SetColumnSpan(cycleSummary, 4);
    body->Controls->Add(cycleCard, 0, 2);
    body->SetColumnSpan(cycleCard, 2);

    auto telemetry = Grid(4);
    telemetry->Padding = System::Windows::Forms::Padding(0, 3, 0, 6);
    Row(telemetry, 65);
    for (int i = 0; i < 4; ++i) telemetry->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 25));
    Metric(telemetry, 0, L"Позиция, тики", cyclePositionValue);
    Metric(telemetry, 1, L"Расчётная мощность", measuredPowerValue);
    Metric(telemetry, 2, L"Заданная мощность", targetPowerValue);
    Metric(telemetry, 3, L"Прошло, мс", elapsedValue);
    body->Controls->Add(telemetry, 0, 3);
    body->SetColumnSpan(telemetry, 2);

    statusLabel = Copy(L"Подключите LTR42 для управления стендом.", 9.5f, Muted(), FontStyle::Regular);
    statusLabel->BackColor = Color::FromArgb(220, 232, 241);
    statusLabel->Padding = System::Windows::Forms::Padding(20, 0, 20, 0);
    statusLabel->AccessibleName = L"Состояние приложения";
    shell->Controls->Add(statusLabel, 0, 2);
    ResumeLayout(true);
}
