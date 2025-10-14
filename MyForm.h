#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "Logger.h"
#include "LinearInterpolation.h"
#include "Source.cpp"
#include <msclr/marshal_cppstd.h>
#include <sstream>
#include <chrono>
#include "header.h"

inline void Log1Event(const std::string& event) {
	static std::ofstream logFile;
	static bool initialized = false;

	if (!initialized) {
		// Создаём папку logs при необходимости
		if (!System::IO::Directory::Exists("logs")) {
			System::IO::Directory::CreateDirectory("logs");
		}

		// Имя файла
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		tm ltm;
		localtime_s(&ltm, &t);

		char timeBuffer[80];
		strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d-%H%M%S", &ltm);
		std::string logFileName = "logs\\log_" + std::string(timeBuffer) + ".txt";

		logFile.open(logFileName, std::ios::app);
		initialized = true;
	}

	if (!logFile.is_open()) return;

	// Мгновенная запись (без сложного форматирования времени)
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	logFile << "[" << ms << "] " << event << "\n";
	// ⚠️ flush можно убрать для скорости, если не нужно мгновенное сохранение:
	// logFile.flush();
}





namespace LTR42wf {
	using namespace System::Threading;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	using namespace msclr::interop;

	
	void LoadPositionFromFile();
	void SavePositionToFile();
	static int cont;
	static int current_position = 0;
	static int time_interval;
	static int steps=0;
	static int ticker = 25; // переменная для того чтобы задавать к-во тиков в пакете
	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void);

	protected:
		virtual void OnFormClosed(FormClosedEventArgs^ e) override {
			ENLOG("Form closed");

			__super::OnFormClosed(e);
		}
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm();
		
	private: System::Windows::Forms::GroupBox^ groupBox1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox_res;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	//Thread^ thisThread, ^ thisThread2;



	private: System::Void Form1_FormClosing(System::Object^ sender, FormClosingEventArgs^ e) {
		SavePositionToFile();
	}

	private: System::Windows::Forms::Button^ button_start_ltr;

	

	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::TextBox^ textBox_angle_ex;

	private: System::Windows::Forms::TextBox^ textBox_time_ex;

	private: System::Windows::Forms::Label^ label_angle_ex;

	private: System::Windows::Forms::Label^ label_time_exp;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ TextBox_current_position;

	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Timer^ timer1;
	private:
		System::Windows::Forms::Timer^ cycleTimer;
		DateTime startTime;

	private: System::Windows::Forms::GroupBox^ groupBox3;
	private: System::Windows::Forms::Button^ buttonLoadCSV;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ textBox1;


	private: System::Windows::Forms::Button^ buttonSaveCSV;
	private: System::Windows::Forms::TextBox^ textBox_Qvtp;
	private: System::Windows::Forms::TextBox^ textBox_Ticks;
	private: System::Windows::Forms::Button^ buttonSaveCycle;

	private: System::Windows::Forms::TextBox^ textBox_CycleValues;

	private: System::Windows::Forms::TextBox^ textBox_CycleTicks;

	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ textBoxCycleName;

	private: System::Windows::Forms::Button^ buttonLoadCycle;
	private: System::Windows::Forms::Button^ buttonStartCycleChart;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label13;




	private: System::ComponentModel::IContainer^ components;





	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		/// 
		/// 
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_start_ltr = (gcnew System::Windows::Forms::Button());
			this->textBox_res = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_angle_ex = (gcnew System::Windows::Forms::TextBox());
			this->textBox_time_ex = (gcnew System::Windows::Forms::TextBox());
			this->label_angle_ex = (gcnew System::Windows::Forms::Label());
			this->label_time_exp = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->TextBox_current_position = (gcnew System::Windows::Forms::TextBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->buttonStartCycleChart = (gcnew System::Windows::Forms::Button());
			this->buttonSaveCycle = (gcnew System::Windows::Forms::Button());
			this->textBox_CycleValues = (gcnew System::Windows::Forms::TextBox());
			this->textBox_CycleTicks = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBoxCycleName = (gcnew System::Windows::Forms::TextBox());
			this->buttonLoadCycle = (gcnew System::Windows::Forms::Button());
			this->buttonSaveCSV = (gcnew System::Windows::Forms::Button());
			this->textBox_Qvtp = (gcnew System::Windows::Forms::TextBox());
			this->textBox_Ticks = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->buttonLoadCSV = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button_start_ltr);
			this->groupBox1->Controls->Add(this->textBox_res);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(84, 14);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(734, 100);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Панель состояния LTR42";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &MyForm::groupBox1_Enter);
			// 
			// button_start_ltr
			// 
			this->button_start_ltr->Location = System::Drawing::Point(21, 61);
			this->button_start_ltr->Name = L"button_start_ltr";
			this->button_start_ltr->Size = System::Drawing::Size(75, 23);
			this->button_start_ltr->TabIndex = 2;
			this->button_start_ltr->Text = L"Start LTR42";
			this->button_start_ltr->UseVisualStyleBackColor = true;
			this->button_start_ltr->Click += gcnew System::EventHandler(this, &MyForm::button_start_ltr_Click);
			// 
			// textBox_res
			// 
			this->textBox_res->Location = System::Drawing::Point(147, 23);
			this->textBox_res->Name = L"textBox_res";
			this->textBox_res->Size = System::Drawing::Size(116, 20);
			this->textBox_res->TabIndex = 1;
			this->textBox_res->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox_res_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(106, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Код запуска LTR42";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->textBox_angle_ex);
			this->groupBox2->Controls->Add(this->textBox_time_ex);
			this->groupBox2->Controls->Add(this->label_angle_ex);
			this->groupBox2->Controls->Add(this->label_time_exp);
			this->groupBox2->Controls->Add(this->button4);
			this->groupBox2->Controls->Add(this->TextBox_current_position);
			this->groupBox2->Location = System::Drawing::Point(84, 120);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(734, 232);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Панель управления";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &MyForm::groupBox2_Enter);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(316, 104);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(100, 23);
			this->button1->TabIndex = 14;
			this->button1->Text = L"0";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_2);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(438, 26);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(111, 13);
			this->label4->TabIndex = 12;
			this->label4->Text = L"Текущее положение";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(185, 80);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(125, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"1450 соответствует 60 ";
			this->label3->Click += gcnew System::EventHandler(this, &MyForm::label3_Click);
			// 
			// textBox_angle_ex
			// 
			this->textBox_angle_ex->Location = System::Drawing::Point(316, 46);
			this->textBox_angle_ex->Name = L"textBox_angle_ex";
			this->textBox_angle_ex->Size = System::Drawing::Size(100, 20);
			this->textBox_angle_ex->TabIndex = 10;
			this->textBox_angle_ex->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox_angle_ex_TextChanged);
			// 
			// textBox_time_ex
			// 
			this->textBox_time_ex->Location = System::Drawing::Point(316, 20);
			this->textBox_time_ex->Name = L"textBox_time_ex";
			this->textBox_time_ex->Size = System::Drawing::Size(100, 20);
			this->textBox_time_ex->TabIndex = 9;
			this->textBox_time_ex->Text = L"0";
			this->textBox_time_ex->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox_time_ex_TextChanged);
			// 
			// label_angle_ex
			// 
			this->label_angle_ex->AutoSize = true;
			this->label_angle_ex->Location = System::Drawing::Point(205, 51);
			this->label_angle_ex->Name = L"label_angle_ex";
			this->label_angle_ex->Size = System::Drawing::Size(101, 13);
			this->label_angle_ex->TabIndex = 8;
			this->label_angle_ex->Text = L"Количество тиков:";
			this->label_angle_ex->Click += gcnew System::EventHandler(this, &MyForm::label_angle_ex_Click);
			// 
			// label_time_exp
			// 
			this->label_time_exp->AutoSize = true;
			this->label_time_exp->Location = System::Drawing::Point(205, 26);
			this->label_time_exp->Name = L"label_time_exp";
			this->label_time_exp->Size = System::Drawing::Size(105, 13);
			this->label_time_exp->TabIndex = 7;
			this->label_time_exp->Text = L"Время поворота(c):";
			this->label_time_exp->Click += gcnew System::EventHandler(this, &MyForm::label_time_exp_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(316, 75);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(100, 23);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Эксперимент";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// TextBox_current_position
			// 
			this->TextBox_current_position->Location = System::Drawing::Point(555, 20);
			this->TextBox_current_position->Name = L"TextBox_current_position";
			this->TextBox_current_position->Size = System::Drawing::Size(100, 20);
			this->TextBox_current_position->TabIndex = 13;
			this->TextBox_current_position->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// timer1
			// 
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label13);
			this->groupBox3->Controls->Add(this->label12);
			this->groupBox3->Controls->Add(this->label11);
			this->groupBox3->Controls->Add(this->label10);
			this->groupBox3->Controls->Add(this->label9);
			this->groupBox3->Controls->Add(this->label8);
			this->groupBox3->Controls->Add(this->label7);
			this->groupBox3->Controls->Add(this->label6);
			this->groupBox3->Controls->Add(this->buttonStartCycleChart);
			this->groupBox3->Controls->Add(this->buttonSaveCycle);
			this->groupBox3->Controls->Add(this->textBox_CycleValues);
			this->groupBox3->Controls->Add(this->textBox_CycleTicks);
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->textBoxCycleName);
			this->groupBox3->Controls->Add(this->buttonLoadCycle);
			this->groupBox3->Controls->Add(this->buttonSaveCSV);
			this->groupBox3->Controls->Add(this->textBox_Qvtp);
			this->groupBox3->Controls->Add(this->textBox_Ticks);
			this->groupBox3->Controls->Add(this->label2);
			this->groupBox3->Controls->Add(this->textBox1);
			this->groupBox3->Controls->Add(this->buttonLoadCSV);
			this->groupBox3->Location = System::Drawing::Point(84, 378);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(734, 338);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Панель управления циклограммами";
			this->groupBox3->Enter += gcnew System::EventHandler(this, &MyForm::groupBox3_Enter);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(383, 277);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(41, 13);
			this->label13->TabIndex = 20;
			this->label13->Text = L"label13";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(323, 277);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(36, 13);
			this->label12->TabIndex = 19;
			this->label12->Text = L"milsec";
			this->label12->Click += gcnew System::EventHandler(this, &MyForm::label12_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(230, 277);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(41, 13);
			this->label11->TabIndex = 18;
			this->label11->Text = L"label11";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(180, 277);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(41, 13);
			this->label10->TabIndex = 17;
			this->label10->Text = L"Pcount";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(380, 260);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(35, 13);
			this->label9->TabIndex = 16;
			this->label9->Text = L"label9";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(320, 260);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(54, 13);
			this->label8->TabIndex = 15;
			this->label8->Text = L"Tick_now";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(227, 260);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(35, 13);
			this->label7->TabIndex = 14;
			this->label7->Text = L"label7";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(187, 260);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(34, 13);
			this->label6->TabIndex = 13;
			this->label6->Text = L"Pnow";
			// 
			// buttonStartCycleChart
			// 
			this->buttonStartCycleChart->Location = System::Drawing::Point(18, 255);
			this->buttonStartCycleChart->Name = L"buttonStartCycleChart";
			this->buttonStartCycleChart->Size = System::Drawing::Size(125, 23);
			this->buttonStartCycleChart->TabIndex = 12;
			this->buttonStartCycleChart->Text = L"Запуск циклограммы";
			this->buttonStartCycleChart->UseVisualStyleBackColor = true;
			this->buttonStartCycleChart->Click += gcnew System::EventHandler(this, &MyForm::buttonStartCycleChart_Click);
			// 
			// buttonSaveCycle
			// 
			this->buttonSaveCycle->Location = System::Drawing::Point(497, 134);
			this->buttonSaveCycle->Name = L"buttonSaveCycle";
			this->buttonSaveCycle->Size = System::Drawing::Size(75, 23);
			this->buttonSaveCycle->TabIndex = 11;
			this->buttonSaveCycle->Text = L"Выгрузить";
			this->buttonSaveCycle->UseVisualStyleBackColor = true;
			this->buttonSaveCycle->Click += gcnew System::EventHandler(this, &MyForm::button2_Click_2);
			// 
			// textBox_CycleValues
			// 
			this->textBox_CycleValues->Location = System::Drawing::Point(58, 201);
			this->textBox_CycleValues->Name = L"textBox_CycleValues";
			this->textBox_CycleValues->Size = System::Drawing::Size(584, 20);
			this->textBox_CycleValues->TabIndex = 10;
			this->textBox_CycleValues->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox2_TextChanged);
			// 
			// textBox_CycleTicks
			// 
			this->textBox_CycleTicks->Location = System::Drawing::Point(58, 175);
			this->textBox_CycleTicks->Name = L"textBox_CycleTicks";
			this->textBox_CycleTicks->Size = System::Drawing::Size(584, 20);
			this->textBox_CycleTicks->TabIndex = 9;
			this->textBox_CycleTicks->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox3_TextChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(228, 139);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(266, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Введите название файла с циклограммой без .csv";
			this->label5->Click += gcnew System::EventHandler(this, &MyForm::label5_Click);
			// 
			// textBoxCycleName
			// 
			this->textBoxCycleName->Location = System::Drawing::Point(121, 136);
			this->textBoxCycleName->Name = L"textBoxCycleName";
			this->textBoxCycleName->Size = System::Drawing::Size(100, 20);
			this->textBoxCycleName->TabIndex = 7;
			this->textBoxCycleName->Text = L"short_cycle";
			this->textBoxCycleName->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox4_TextChanged);
			// 
			// buttonLoadCycle
			// 
			this->buttonLoadCycle->Location = System::Drawing::Point(18, 134);
			this->buttonLoadCycle->Name = L"buttonLoadCycle";
			this->buttonLoadCycle->Size = System::Drawing::Size(98, 23);
			this->buttonLoadCycle->TabIndex = 6;
			this->buttonLoadCycle->Text = L"Загрузить CSV";
			this->buttonLoadCycle->UseVisualStyleBackColor = true;
			this->buttonLoadCycle->Click += gcnew System::EventHandler(this, &MyForm::button3_Click_1);
			// 
			// buttonSaveCSV
			// 
			this->buttonSaveCSV->Location = System::Drawing::Point(497, 19);
			this->buttonSaveCSV->Name = L"buttonSaveCSV";
			this->buttonSaveCSV->Size = System::Drawing::Size(75, 23);
			this->buttonSaveCSV->TabIndex = 5;
			this->buttonSaveCSV->Text = L"Выгрузить";
			this->buttonSaveCSV->UseVisualStyleBackColor = true;
			this->buttonSaveCSV->Click += gcnew System::EventHandler(this, &MyForm::buttonSaveCSV_Click);
			// 
			// textBox_Qvtp
			// 
			this->textBox_Qvtp->Location = System::Drawing::Point(58, 86);
			this->textBox_Qvtp->Name = L"textBox_Qvtp";
			this->textBox_Qvtp->Size = System::Drawing::Size(584, 20);
			this->textBox_Qvtp->TabIndex = 4;
			// 
			// textBox_Ticks
			// 
			this->textBox_Ticks->Location = System::Drawing::Point(58, 60);
			this->textBox_Ticks->Name = L"textBox_Ticks";
			this->textBox_Ticks->Size = System::Drawing::Size(584, 20);
			this->textBox_Ticks->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(228, 24);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(242, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Введите название файла калибровки без .csv";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click_1);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(122, 21);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = L"forpowerToPosition";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged_1);
			// 
			// buttonLoadCSV
			// 
			this->buttonLoadCSV->Location = System::Drawing::Point(18, 19);
			this->buttonLoadCSV->Name = L"buttonLoadCSV";
			this->buttonLoadCSV->Size = System::Drawing::Size(98, 23);
			this->buttonLoadCSV->TabIndex = 0;
			this->buttonLoadCSV->Text = L"Загрузить CSV";
			this->buttonLoadCSV->UseVisualStyleBackColor = true;
			this->buttonLoadCSV->Click += gcnew System::EventHandler(this, &MyForm::buttonLoadCSV_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(955, 770);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void groupBox1_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox_res_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void groupBox2_Enter(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox_input_Time_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	ENLOG("Form loaded");
}

private: System::Void button_start_ltr_Click(System::Object^ sender, System::EventArgs^ e) {
	this->textBox_res->Text = Convert::ToString(Startltr42());
	ENLOG("Button clicked: Start LTR42");

	// Открываем файл для чтения
	std::ifstream inFile("position.txt");
	std::string line;
	std::vector<std::string> lines;

	if (inFile.is_open()) {
		ENLOG("Reading from file: position.txt");
		while (std::getline(inFile, line)) {
			lines.push_back(line); // Сохраняем каждую строку в вектор
		}
		inFile.close();

		/*if (!lines.empty()) {
			current_position = std::stoi(lines.back()); // Берем последнюю строку
			
		}*/
	}
	

	this->TextBox_current_position->Text = Convert::ToString(current_position); 

}
private: System::Void button_ok_Click(System::Object^ sender, System::EventArgs^ e) {
	
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	//cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096;
	startandwrite(cont);
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	//cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096;
	startandwrite(cont);
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	//cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096 + 8192;
	startandwrite(cont);
}
	   int zero_count = 0;
	   int minus = 0;
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	ENLOG("Button clicked: Experiment started");
	int tick; // это нужное положение жалюзи
	int time;
	int tick_counter;
	int cur;
	int interval; // промежуток между сериями тиков
	// Открываем файл для чтения
	

	cur = current_position;
	
	

	this->TextBox_current_position->Text = Convert::ToString(cur);
	
	tick = Convert::ToInt32(this->textBox_angle_ex->Text);

	if (cur > tick) {
		tick_counter = cur - tick;
		minus = 0;
	}
	else {
		tick_counter = tick - cur;
		
		minus = 8192;
	}
	double timer; // Объявляем переменную timer здесь
	time = Convert::ToInt32(this->textBox_time_ex->Text) * 1000; // время в миллисекундах

	
		if (tick_counter != 0) {
			timer = (double)time / tick_counter;
		}
		else {
			timer = 1;
		}
	
	
	

	

	int total_steps = tick_counter / ticker; // общее количество шагов
	int completed_steps = 0; // количество завершенных шагов
	//steps = total_steps;

	
	for (int i = 0; i < total_steps; i++) {


		rotate(minus);

		if (minus == 0) {
			cur -= 25;
		}
		else {
			cur += 25;
		}
		completed_steps++;
	}
	current_position = cur;
	ENLOG(cur);
	this->TextBox_current_position->Text = Convert::ToString(cur);
	//System::Diagnostics::Stopwatch^ stopwatch = gcnew System::Diagnostics::Stopwatch();
	//stopwatch->Start();
	/*int wait_time = 0;
	if (time == 0) {
		interval = 0;
	}
	else {

		interval = time / (total_steps);
		time_interval = interval;
	}

	if (interval != 0) {
		timer1->Start();
	} 
	if (interval == 0) {

		zero_timer();
	}

	*/
	



	/*auto start_time = std::chrono::steady_clock::now();
		
	for (int i = 0; i<total_steps; i++) {
		auto next_step_time = start_time + std::chrono::milliseconds(interval * (i));
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		rotate(minus);
		while (std::chrono::steady_clock::now() < next_step_time) {
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		
		if (minus == 0) {
			cur -= 25;
		}
		else {
			cur += 25;
		}
		completed_steps++;
		if (out.is_open()) {
			out << cur << "\n"; // Записываем значение current_position в файл с новой строки
		}
	}
	stop_rotate;

		// Останавливаем таймер и вычисляем время выполнения цикла
		
		/*int elapsed_time = (int)stopwatch->ElapsedMilliseconds;

		// Рассчитываем время, которое нужно подождать, чтобы уложиться в time_per_cycle
		//int wait_time = (time_per_cycle*26) - elapsed_time;
		if (total_steps != completed_steps) {
			wait_time = (time - elapsed_time) / (total_steps - completed_steps);
		}
		else
		{
			wait_time = 0;
		}
		
		if (wait_time > 0) {
			System::Threading::Thread::Sleep(wait_time);
		}*/
	
	//stopwatch->Stop();
	
	//startandwrite(0);
	//out.close();
	//this->TextBox_current_position->Text = Convert::ToString(cur);
}




private: System::Void textBox_time_ex_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox_angle_ex_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label_time_exp_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label_angle_ex_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
	startandwrite(8192);
	//startandwrite(65535);
}
private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) {
	startandwrite(0);
}
private: System::Void button1_Click_2(System::Object^ sender, System::EventArgs^ e) {
	rotate_to_zero();
	int cur = current_position;
	this->TextBox_current_position->Text = Convert::ToString(cur);
}

private: void DoWork(int i) {
	button1->Text = Convert::ToString(i); // функция того что нужно сделать
}
private: System::Void Thread_Start() {
for (int i = 1; i <= 50; i++) {
	 Invoke(gcnew Action<int>(this, &MyForm::DoWork)); // описывает работу на потоке
	 }
}


	   void zero_timer() {
		   ENLOG("Zero timer started");
		   // запускает поток

		   if (steps <= 0) {
			   return;
		   }
		  // std::ofstream out("position.txt", std::ios::app);
		   int cur = current_position;
		   int step = steps;
		   int completed_steps = 0;
		   while (steps > 0) {
			   if (steps > 0) {
				   steps--;
				   rotate(minus);
				   if (minus == 0) {
					   cur -= ticker;
				   }
				   else {
					   cur += ticker;
				   }
				   completed_steps++;
				  /* if (out.is_open()) {
					   this->TextBox_current_position->Text = Convert::ToString(cur);
					   out << cur << "\n"; // Записываем значение current_position в файл с новой строки
				   }*/

			   }
			   else
			   {
				   if (minus == 0) {
					   cur -= ticker;
				   }
				   else {
					   cur += ticker;
				   }
				  
				   //ENLOG("Experiment completed");
				   stop_rotate();
				   //out.close();

			   }
		   }
		   
		   if (steps == 0) {
			   ENLOG("Timer stopped");
		   }
		   /*if (!thisThread || !thisThread->IsAlive) {
			   thisThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Thread_Start));
			   thisThread->Start();
		   }*/
		   current_position = 0;
		   ENLOG("Cur_pos: ")(cur);
}

private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) { 
	ENLOG("Timer tick triggered");
	timer1->Interval = time_interval; // запускает поток

	std::ofstream out("position.txt", std::ios::app);
	int cur = current_position;
	int step = steps;
	int completed_steps = 0;
	if (steps > 0) {
		steps--;
		rotate(minus);
		if (minus == 0) {
			cur -= 5;
		}
		else {
			cur += 5;
		}
		completed_steps++;
		if (out.is_open()) {
			this->TextBox_current_position->Text = Convert::ToString(cur);
			out << cur << "\n"; // Записываем значение current_position в файл с новой строки
		}
		
	}
	else
	{
		if (minus == 0) {
			cur -= 5;
		}
		else {
			cur += 5;
		}
		timer1->Stop();
		ENLOG("Experiment completed");
		stop_rotate();
		out.close();
		
	}
	if (steps == 0) {
		ENLOG("Timer stopped after experiment completion");
	}
	/*if (!thisThread || !thisThread->IsAlive) {
		thisThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::Thread_Start));
		thisThread->Start();
	}*/
	current_position = cur;
	
}
private: System::Void groupBox3_Enter(System::Object^ sender, System::EventArgs^ e) {
}
private:
    LinearCalibration* powerToPosition; // указатель на кривую
    LinearCalibration* positionToPower; // указатель на кривую
	std::vector<double>* Ticks;
	std::vector<double>* Qvtp;
	std::vector<double>* MinutesVec;
	std::vector<double>* PowerVec;

	LinearCalibration* cycleCurve;

private: System::Void buttonLoadCSV_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		Ticks->clear();
		Qvtp->clear();

		// Получаем имя файла из TextBox
		String^ fileNameInput = textBox1->Text;
		if (String::IsNullOrWhiteSpace(fileNameInput)) {
			MessageBox::Show("Введите имя файла!");
			return;
		}

		// Папка data рядом с программой
		std::string fileNameStr = msclr::interop::marshal_as<std::string>(fileNameInput);
		std::string folder = "data";  // папка внутри директории программы
		std::string fullPath = folder + "\\" + fileNameStr + ".csv";

		// Читаем исходный CSV
		std::ifstream file(fullPath);
		if (!file.is_open()) {
			MessageBox::Show("Файл не найден: " + textBox1->Text + " в папке data");
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) continue;
			std::stringstream ss(line);
			double tick, qvtp;
			char sep;
			if (ss >> tick >> sep >> qvtp && sep == ';') {
				Ticks->push_back(tick);
				Qvtp->push_back(qvtp);
			}
		}
		file.close();

		if (Ticks->empty() || Qvtp->empty()) {
			MessageBox::Show("Файл пустой или некорректный!");
			return;
		}

		// Очищаем TextBox перед выводом
		textBox_Ticks->Clear();
		textBox_Qvtp->Clear();

		for (size_t i = 0; i < Ticks->size(); ++i) {
			textBox_Ticks->AppendText(System::Convert::ToString((*Ticks)[i]));
			textBox_Qvtp->AppendText(System::Convert::ToString((*Qvtp)[i]));

			if (i != Ticks->size() - 1) {
				textBox_Ticks->AppendText(";");
				textBox_Qvtp->AppendText(";");
			}
		}

		MessageBox::Show("Данные из CSV успешно загружены из папки data!");
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка: " + ex->Message);
	}
}







  

private: System::Void label2_Click_1(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox1_TextChanged_1(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void buttonSaveCSV_Click(System::Object^ sender, System::EventArgs^ e) {
	try {
		if (!Ticks->empty() && !Qvtp->empty()) {

			// Создаем кривую
			powerToPosition->SetPoints(*Qvtp, *Ticks);
			positionToPower->SetPoints(*Ticks, *Qvtp);

			// Папка data
			String^ folder = "data";
			// Файл в папке data
			String^ outFileName = System::IO::Path::Combine(folder, "calibration_curve.csv");

			// Проверим, существует ли папка "data"
			if (!System::IO::Directory::Exists(folder)) {
				System::IO::Directory::CreateDirectory(folder);
			}

			// Запись в файл
			std::ofstream outFile(marshal_as<std::string>(outFileName));
			if (!outFile.is_open()) {
				MessageBox::Show("Не удалось создать файл: " + outFileName);
				return;
			}

			// Шаг 25 тиков
			double step = 25.0;
			double maxTick = *std::max_element(Ticks->begin(), Ticks->end());

			for (double x = 0; x <= maxTick; x += step) {
				double y = (*positionToPower)(x);
				outFile << x << ";" << y << "\n";
			}

			outFile.close();
			MessageBox::Show("CSV с данными кривой создан:\n" + outFileName);
		}
		else {
			MessageBox::Show("Векторы Ticks или Qvtp пусты!");
		}
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка: " + ex->Message);
	}
}


private: System::Void buttonLoadCycle_Click(System::Object^ sender, System::EventArgs^ e) {
	
}
private: System::Void buttonSaveCycle_Click(System::Object^ sender, System::EventArgs^ e) {
	
}


private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click_2(System::Object^ sender, System::EventArgs^ e) {
	try {
		// Имя файла берем из textBox1
		String^ fileNameInput = textBox1->Text;
		if (String::IsNullOrWhiteSpace(fileNameInput)) {
			MessageBox::Show("Введите имя файла для сохранения циклограммы!");
			return;
		}

		// Папка data
		std::string folder = "data";
		std::string fileNameStr = msclr::interop::marshal_as<std::string>(fileNameInput);
		std::string fullPath = folder + "\\" + fileNameStr + "_cycle.csv";

		// Проверим, существует ли папка data
		if (!System::IO::Directory::Exists("data")) {
			System::IO::Directory::CreateDirectory("data");
		}

		// Открываем файл на запись
		std::ofstream outFile(fullPath);
		if (!outFile.is_open()) {
			MessageBox::Show("Не удалось создать файл: " + textBox1->Text + "_cycle.csv (папка data)");
			return;
		}

		// Читаем данные из текстбоксов
		String^ minutesStr = textBox_CycleTicks->Text;
		String^ PowerStr = textBox_CycleValues->Text;

		// Разбиваем строки по ";"
		cli::array<String^>^ minutesArr = minutesStr->Split(';');
		cli::array<String^>^ PowerArr = PowerStr->Split(';');

		if (minutesArr->Length != PowerArr->Length) {
			MessageBox::Show("Количество значений минут и мощности не совпадает!");
			outFile.close();
			return;
		}

		// Записываем в CSV и заполняем векторы
		for (int i = 0; i < minutesArr->Length; i++) {
			if (String::IsNullOrWhiteSpace(minutesArr[i]) || String::IsNullOrWhiteSpace(PowerArr[i]))
				continue;

			double minute = Double::Parse(minutesArr[i]);
			double pow = Double::Parse(PowerArr[i]);

			MinutesVec->push_back(minute);
			PowerVec->push_back(pow);

			outFile << minute << ";" << pow << std::endl;
		}

		outFile.close();
		MessageBox::Show("Циклограмма успешно сохранена в файл (папка data): " + textBox1->Text + "_cycle.csv");

		// --- Построение кривой ---
		if (!MinutesVec->empty() && !PowerVec->empty()) {
			//cycleCurve->SetPoints(*MinutesVec, *PowerVec);
		}
		if (!MinutesVec->empty()) {
			maxTimeMs = MinutesVec->back(); // последняя минута в миллисекундах
		}
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка: " + ex->Message);
	}
}


private: System::Void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void textBox4_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button3_Click_1(System::Object^ sender, System::EventArgs^ e) {
	try {
		// Получаем имя файла циклограммы из TextBox
		String^ fileNameInput = textBoxCycleName->Text;
		if (String::IsNullOrWhiteSpace(fileNameInput)) {
			MessageBox::Show("Введите имя файла циклограммы!");
			return;
		}

		// Папка data и полный путь
		std::string folder = "data";
		std::string fileNameStr = msclr::interop::marshal_as<std::string>(fileNameInput);
		std::string fullPath = folder + "\\" + fileNameStr + ".csv";

		// Проверим, есть ли папка data
		if (!System::IO::Directory::Exists("data")) {
			System::IO::Directory::CreateDirectory("data");
		}

		// Читаем CSV
		std::ifstream file(fullPath);
		if (!file.is_open()) {
			MessageBox::Show("Файл циклограммы не найден: " + textBoxCycleName->Text + " (папка data)");
			return;
		}

		std::vector<double> CycleTicks, CycleValues;
		CycleTicks.clear();
		CycleValues.clear();
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) continue;
			std::stringstream ss(line);
			double tick, value;
			char sep;
			if (ss >> tick >> sep >> value && sep == ';') {
				CycleTicks.push_back(tick);
				CycleValues.push_back(value);
			}
		}
		file.close();

		if (CycleTicks.empty() || CycleValues.empty()) {
			MessageBox::Show("Файл циклограммы пустой или некорректный!");
			return;
		}

		// Очищаем текстбоксы
		textBox_CycleTicks->Clear();
		textBox_CycleValues->Clear();

		// Выводим данные в текстбоксы
		for (size_t i = 0; i < CycleTicks.size(); ++i) {
			textBox_CycleTicks->AppendText(CycleTicks[i].ToString());
			textBox_CycleValues->AppendText(CycleValues[i].ToString());

			if (i != CycleTicks.size() - 1) {
				textBox_CycleTicks->AppendText(";");
				textBox_CycleValues->AppendText(";");
			}
		}

		MessageBox::Show("Циклограмма успешно загружена из папки data!");
	}
	catch (Exception^ ex) {
		MessageBox::Show("Ошибка: " + ex->Message);
	}
}

private: System::Void buttonStartCycleChart_Click(System::Object^ sender, System::EventArgs^ e) {
	// --- Сохраняем время запуска ---
	if (MinutesVec->empty() || PowerVec->empty()) {
		MessageBox::Show("Ошибка: таблица мощности не загружена!");
		return;
	}
	if ((*MinutesVec)[0] != 0.0) {
		double shift = (*MinutesVec)[0];
		for (size_t i = 0; i < MinutesVec->size(); i++) {
			(*MinutesVec)[i] -= shift;
		}
	}
	maxTimeMs = (*MinutesVec).back();
	// --- Проверяем текущую позицию ---
	//int current_position = current_position;
	try {
		std::ifstream in("position.txt");
		if (in.is_open()) {
			std::string line;
			while (std::getline(in, line)) {
				if (!line.empty()) {
					try { current_position = std::stoi(line); }
					catch (...) {}
				}
			}
		}
	}
	catch (...) { current_position = 0; }

	// --- Вычисляем целевую позицию из кривой в момент 0 ---
	double countPower0 = (*PowerVec)[0];
	int targetTick0 = (int)((*powerToPosition)(countPower0));
	targetTick0 = (((int)(targetTick0 + 24) / 25)) * 25;

	// --- Крутим двигатель до начальной позиции ---
	int minus = 8192;
	while (current_position != targetTick0) {
		// читаем актуальное положение из файла
		/*try {
			std::ifstream in("position.txt");
			if (in.is_open()) {
				std::string line;
				while (std::getline(in, line)) {
					if (!line.empty()) {
						try { current_position = std::stoi(line); }
						catch (...) {}
					}
				}
			}
		}
		catch (...) { current_position = 0; }*/
		this->label9->Text = current_position.ToString();
		if (current_position > targetTick0) {
			rotate(0);      // шаг назад
		}
		else if (current_position < targetTick0) {
			rotate(minus);  // шаг вперёд
		}

		Sleep(2); // даём драйверу обработать шаг
	}

	// --- Логируем старт ---
	fix_power = (*positionToPower)(current_position); // мощность от времени
	ENLOG("Cycle start detected (position aligned)");

	// --- Создаём таймер (если ещё нет) ---
	if (this->cycleTimer == nullptr) {
		this->cycleTimer = gcnew System::Windows::Forms::Timer();
		this->cycleTimer->Interval = 300; // 50 мс
		this->cycleTimer->Tick += gcnew EventHandler(this, &MyForm::OnCycleTimerTick);
	}

	// --- Запускаем таймер ---
	this->startTime = DateTime::Now;
	this->cycleTimer->Start();
}






	   // === Реализации функций ===
	   /**/void LoadPositionFromFile() {
		   try {
			   std::ifstream in("position.txt");
			   if (in.is_open()) {
				   std::string line;
				   while (std::getline(in, line)) {
					   if (!line.empty()) {
						   try { current_position = std::stoi(line); }
						   catch (...) {}
					   }
				   }
			   }
		   }
		   catch (...) { current_position = 0; }
	   }

	   void SavePositionToFile() {
		   try {
			   std::ofstream out("position.txt", std::ios::trunc);
			   if (out.is_open()) {
				   out << current_position << "\n";
			   }
		   }
		   catch (...) {}
	   }



private: System::Void OnCycleTimerTick(System::Object^ sender, System::EventArgs^ e) {
	TimeSpan elapsed = DateTime::Now - this->startTime;
	double elapsedMillisec = elapsed.TotalMilliseconds;

	if (elapsedMillisec >= maxTimeMs) {
		this->cycleTimer->Stop();

		// финальное сохранение
		SavePositionToFile();

		ENLOG("Cycle finished. Timer stopped.");
		return;
	}

	int minus = 8192;

	// --- вычисление мощности ---
	double countPower = 0.0;
	if (MinutesVec->size() >= 2) {
		if (elapsedMillisec < (*MinutesVec)[0]) {
			countPower = (*PowerVec)[0];
		}
		else {
			for (size_t i = 0; i < MinutesVec->size() - 1; i++) {
				double t1 = (*MinutesVec)[i];
				double t2 = (*MinutesVec)[i + 1];
				if (elapsedMillisec >= t1 && elapsedMillisec < t2) {
					double p1 = (*PowerVec)[i];
					double p2 = (*PowerVec)[i + 1];
					double alpha = (elapsedMillisec - t1) / (t2 - t1);
					countPower = p1 + alpha * (p2 - p1);
					break;
				}
			}
			if (elapsedMillisec >= (*MinutesVec).back()) {
				countPower = (*PowerVec).back();
			}
		}
	}

	// --- пересчёт мощности в тики ---
	double countTick = 0.0;
	if (powerToPosition != nullptr && PowerVec->size() > 0) {
		countTick = (*powerToPosition)(countPower);

		countTick = round(countTick / 25.0) * 25.0;
		fix_power = (*positionToPower)(countTick);

		if (countTick < 0) countTick = 0;
		if (countTick > 1450) countTick = 1450;
	}

	// --- управление шаговиком ---
	if (current_position > countTick) {
		rotate(0);    // шаг назад
		Sleep(1);
		 current_position -= 25;
	}
	else if (current_position < countTick) {
		rotate(minus); // шаг вперёд
		Sleep(1);
		current_position += 25;
	}
	int cur_pos;
	cur_pos = current_position;

	// --- обновление интерфейса ---
	this->label9->Text = current_position.ToString();
	this->label7->Text = fix_power.ToString();
	this->label11->Text = countPower.ToString("F2");
	this->label13->Text = elapsedMillisec.ToString();

	// --- лог ---
	System::String^ msg = "Elapsed: " + elapsedMillisec.ToString() +
		" ms, Target pos: " + countTick.ToString() +
		", Current pos: " + current_position.ToString() +
		", Current P = " + fix_power.ToString() +
		", Interpolated P = " + countPower.ToString("F2");
	ENLOG(msclr::interop::marshal_as<std::string>(msg));
}







private:
	TimeSpan elapsed = DateTime::Now - this->startTime;
	double maxTimeMs = 0.0;   // конец цикла по времени
	double fix_power = 0.0;   // для отображения
	bool aligningToStart = false;
	int targetTick0 = 0;

	//System::Windows::Forms::Timer^ cycleTimer = nullptr;
	//DateTime startTime;

private: System::Void label12_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
