#pragma once

struct RunData;

namespace LTR42wf {
    using namespace System;
    using namespace System::Drawing;
    using namespace System::Windows::Forms;

    public ref class MyForm : public Form {
    public:
        MyForm();

    protected:
        ~MyForm();
        virtual void OnFormClosing(FormClosingEventArgs^ e) override;

    private:
        enum class Operation { Idle, Manual, Aligning, Waiting, Running };

        RunData* data;
        String^ dataDirectory;
        Operation operation;
        bool connected;
        bool positionChanged;
        int position;
        int motionTarget;
        int motionStart;
        double motionDurationMs;
        DateTime scheduledStart;
        Diagnostics::Stopwatch^ elapsed;
        Timer^ motionTimer;
        Timer^ scheduleTimer;
        Timer^ cycleTimer;

        Label^ connectionLabel;
        Label^ statusLabel;
        Label^ positionValue;
        Label^ cyclePositionValue;
        Label^ measuredPowerValue;
        Label^ targetPowerValue;
        Label^ elapsedValue;
        Label^ calibrationSummary;
        Label^ cycleSummary;
        Button^ connectButton;
        Button^ moveButton;
        Button^ zeroButton;
        Button^ startButton;
        Panel^ heaterButtons;
        TableLayoutPanel^ calibrationCard;
        TableLayoutPanel^ cycleCard;
        TextBox^ targetPositionInput;
        TextBox^ durationInput;
        TextBox^ calibrationName;
        TextBox^ cycleName;
        TextBox^ cycleTimes;
        TextBox^ cyclePowers;
        TextBox^ startTimeInput;

        void InitializeComponent();
        void UpdateControls();
        void ShowStatus(String^ message, bool error);
        void UpdatePosition();
        bool AcceptDeviceResult(int code);
        void SavePosition();

        void Connect(Object^ sender, EventArgs^ e);
        void Move(Object^ sender, EventArgs^ e);
        void MoveToZero(Object^ sender, EventArgs^ e);
        void SetHeaterOutput(Object^ sender, EventArgs^ e);
        void StartCycle(Object^ sender, EventArgs^ e);
        void BeginMotion(int target, double durationMs, Operation nextOperation);
        void MotionTick(Object^ sender, EventArgs^ e);
        void QueueCycle();
        void ScheduleTick(Object^ sender, EventArgs^ e);
        void CycleTick(Object^ sender, EventArgs^ e);
        void StopTimers();

        void LoadCalibration(Object^ sender, EventArgs^ e);
        void ExportCalibration(Object^ sender, EventArgs^ e);
        void LoadCycle(Object^ sender, EventArgs^ e);
        void ExportCycle(Object^ sender, EventArgs^ e);
        void ReadPowerInputs();
    };
}
