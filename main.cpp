#include "MyForm.h"
#include <Windows.h>
#include <objbase.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace LTR42wf;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED); // STA для OpenFileDialog

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    MyForm^ form = gcnew MyForm();
    Application::Run(form);

    CoUninitialize();
    return 0;
}
