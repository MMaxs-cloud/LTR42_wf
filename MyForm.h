#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace LTR42wf {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	static int cont;
	static int current_position = 0;
	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^ groupBox1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox_res;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Button^ button_take_time;

	private: System::Windows::Forms::TextBox^ textBox_input_Time;


	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button_start_ltr;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::TextBox^ textBox_angle_ex;

	private: System::Windows::Forms::TextBox^ textBox_time_ex;

	private: System::Windows::Forms::Label^ label_angle_ex;

	private: System::Windows::Forms::Label^ label_time_exp;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ TextBox_current_position;

	private: System::Windows::Forms::Label^ label4;



	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_start_ltr = (gcnew System::Windows::Forms::Button());
			this->textBox_res = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_angle_ex = (gcnew System::Windows::Forms::TextBox());
			this->textBox_time_ex = (gcnew System::Windows::Forms::TextBox());
			this->label_angle_ex = (gcnew System::Windows::Forms::Label());
			this->label_time_exp = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button_take_time = (gcnew System::Windows::Forms::Button());
			this->textBox_input_Time = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->TextBox_current_position = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
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
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->textBox_angle_ex);
			this->groupBox2->Controls->Add(this->textBox_time_ex);
			this->groupBox2->Controls->Add(this->label_angle_ex);
			this->groupBox2->Controls->Add(this->label_time_exp);
			this->groupBox2->Controls->Add(this->button4);
			this->groupBox2->Controls->Add(this->button3);
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Controls->Add(this->button_take_time);
			this->groupBox2->Controls->Add(this->textBox_input_Time);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->TextBox_current_position);
			this->groupBox2->Location = System::Drawing::Point(84, 120);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(734, 232);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Панель управления";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &MyForm::groupBox2_Enter);
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
			this->label3->Text = L"5600 соответствует 60 ";
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
			// button3
			// 
			this->button3->Location = System::Drawing::Point(83, 135);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 23);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Против часовой";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(83, 105);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(100, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"По часовой";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(83, 75);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(100, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Вращать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button_take_time
			// 
			this->button_take_time->Location = System::Drawing::Point(83, 46);
			this->button_take_time->Name = L"button_take_time";
			this->button_take_time->Size = System::Drawing::Size(100, 23);
			this->button_take_time->TabIndex = 2;
			this->button_take_time->Text = L"Ок";
			this->button_take_time->UseVisualStyleBackColor = true;
			this->button_take_time->Click += gcnew System::EventHandler(this, &MyForm::button_take_time_Click);
			// 
			// textBox_input_Time
			// 
			this->textBox_input_Time->Location = System::Drawing::Point(83, 19);
			this->textBox_input_Time->Name = L"textBox_input_Time";
			this->textBox_input_Time->Size = System::Drawing::Size(100, 20);
			this->textBox_input_Time->TabIndex = 1;
			this->textBox_input_Time->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox_input_Time_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(30, 19);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"dt(msek)";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// TextBox_current_position
			// 
			this->TextBox_current_position->Location = System::Drawing::Point(555, 20);
			this->TextBox_current_position->Name = L"TextBox_current_position";
			this->TextBox_current_position->Size = System::Drawing::Size(100, 20);
			this->TextBox_current_position->TabIndex = 13;
			this->TextBox_current_position->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(955, 399);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
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
private: System::Void button_take_time_Click(System::Object^ sender, System::EventArgs^ e) {
	cont = Convert::ToInt32(this->textBox_input_Time->Text);
	startandwrite(cont);
	
}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	
}
private: System::Void button_start_ltr_Click(System::Object^ sender, System::EventArgs^ e) {
	this->textBox_res->Text = Convert::ToString(Startltr42());

	// Открываем файл для чтения
	std::ifstream inFile("position.txt");
	std::string line;
	std::vector<std::string> lines;

	if (inFile.is_open()) {
		while (std::getline(inFile, line)) {
			lines.push_back(line); // Сохраняем каждую строку в вектор
		}
		inFile.close();

		if (!lines.empty()) {
			current_position = std::stoi(lines.back()); // Берем последнюю строку
			
		}
	}
	

	this->TextBox_current_position->Text = Convert::ToString(current_position); // на этом месте будет считывание позиции из файла

}
private: System::Void button_ok_Click(System::Object^ sender, System::EventArgs^ e) {
	
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096;
	startandwrite(cont);
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096;
	startandwrite(cont);
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
	cont = Convert::ToInt32(this->textBox_input_Time->Text) + 4096 + 8192;
	startandwrite(cont);
}
	   int zero_count = 0;
	   int minus = 0;
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	int tick; // это нужное положение в пространстве
	//double angle;
	int time;
	double timer;
	int tick_counter;
	tick = Convert::ToInt32(this->textBox_angle_ex->Text);
	
	if (current_position > tick) { tick_counter = current_position - tick; minus = 8192; }
	if (tick > current_position) { tick_counter = tick - current_position; minus = 0; }


	time = Convert::ToInt32(this->textBox_time_ex->Text) * 1000;
	if (tick_counter != 0) { timer = (time / tick_counter)*2*0.745; }//умножаю на два потому что за один тик на ПК ардуино крутитсчя два раза
	else { timer = 0; }
	
	
	std::ofstream out("position.txt", std::ios::app);
		for (int i = 0; i < (tick_counter/2); i++)
		{

			if (minus == 0)
			{
				current_position+=2;
			}
			else 
			{
				current_position-=2;
			}



			cont = Convert::ToInt32(this->textBox_input_Time->Text); // 
			startandwrite(/*cont*/ 0 + 4096 + minus + 16384);
			Sleep(1);

			startandwrite(/*cont*/ 0 + 4096 + minus);
			Sleep(1);
			Sleep(timer-2);
			
			

		//}
	}
		if (out.is_open()) {
			// Записываем значение current_position в файл с новой строки
			out << current_position << "\n";

			// Закрываем файл

		}
		startandwrite(0);
	out.close();
	this->TextBox_current_position->Text = Convert::ToString(current_position);
			
		
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
};
}
