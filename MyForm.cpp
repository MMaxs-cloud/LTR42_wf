#include "MyForm.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <msclr/marshal_cppstd.h>
#include "LinearInterpolation.h"

#include <mutex>
#include "header.h"
#include "enlog.h"
#include <functional>


using namespace System;
using namespace System::Windows::Forms;
using namespace msclr::interop;

void toLog(std::string const& msg, rt::LogLevel level) {
    ENLOG(msg);
}


// глобальные переменные
std::mutex g_LogMutex;
rt::Logger g_Logger;

void handle_message(std::string const& s) {
    rt::RTEngineLogger::write_to_log(s);
    throw std::runtime_error(s);
}

void rt::RTEngineLogger::write_to_log(std::string const& str) {
    std::lock_guard<std::mutex> lock(g_LogMutex);
    g_Logger.info(str);
}

rt::Logger::Logger() {
    init(toLog);
}



[STAThreadAttribute] // обязательно для OpenFileDialog
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    LTR42wf::MyForm^ form = gcnew LTR42wf::MyForm();
    Application::Run(form);

    return 0;
}

// ------------------- Конструктор и деструктор -------------------
LTR42wf::MyForm::MyForm(void)
{
    InitializeComponent();

    // Подписываем кнопку на обработчик
    this->buttonLoadCSV->Click += gcnew EventHandler(this, &MyForm::buttonLoadCSV_Click);

    powerToPosition = new LinearCalibration();
    positionToPower = new LinearCalibration();
    Ticks = new std::vector<double>();
    Qvtp = new std::vector<double>();
    MinutesVec = new std::vector<double>();
    PowerVec = new std::vector<double>();

    cycleCurve = new LinearCalibration();
}

LTR42wf::MyForm::~MyForm()
{
    if (components) delete components;
    if (powerToPosition) delete powerToPosition;
}

// ------------------- Обработчик кнопки -------------------
/*void LTR42wf::MyForm::buttonLoadCSV_Click(System::Object^ sender, System::EventArgs^ e)
{
    try
    {
        String^ fileNameInput = textBox1->Text;
        if (String::IsNullOrWhiteSpace(fileNameInput)) {
            MessageBox::Show("Введите имя файла!");
            return;
        }

        String^ folder = "C:\\Users\\maxim\\source\\repos\\LTR42_wf";
        String^ fullPath = System::IO::Path::Combine(folder, fileNameInput + ".csv");

        if (!System::IO::File::Exists(fullPath)) {
            MessageBox::Show("Файл не найден: " + fullPath);
            return;
        }

        std::string path = marshal_as<std::string>(fullPath);

        std::ifstream file(path);
        if (!file.is_open()) {
            MessageBox::Show("Не удалось открыть файл: " + fullPath);
            return;
        }

        std::vector<double> Ticks, Qvtp;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            double tick, qvtp;
            char sep;
            if (ss >> tick >> sep >> qvtp && sep == ';') {
                Ticks.push_back(tick);
                Qvtp.push_back(qvtp);
            }
        }
        file.close();

        if (Ticks.empty() || Qvtp.empty()) {
            MessageBox::Show("Файл пустой или некорректный!");
            return;
        }

        // Очистка TextBox
        textBox_Ticks->Clear();
        textBox_Qvtp->Clear();

        // Заполнение TextBox значениями через ;
        for (size_t i = 0; i < Ticks.size(); ++i) {
            textBox_Ticks->AppendText(Ticks[i].ToString());
            textBox_Qvtp->AppendText(Qvtp[i].ToString());
            if (i != Ticks.size() - 1) {
                textBox_Ticks->AppendText(";");
                textBox_Qvtp->AppendText(";");
            }
        }

        // Построение кривой
        powerToPosition->SetPoints(Ticks, Qvtp);

        MessageBox::Show("Файл успешно загружен! Количество строк: " + Ticks.size());
    }
    catch (Exception^ ex) {
        MessageBox::Show("Ошибка: " + ex->Message);
    }
}*/
