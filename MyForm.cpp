//#pragma once
#include <Windows.h>
#include "Source.cpp"
#include "MyForm.h"


using namespace LTR42wf;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm);
    
    
        
    return 0;

}