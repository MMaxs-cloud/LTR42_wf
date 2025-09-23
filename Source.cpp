#include "ltr11api.h"
#include "ltrapi.h"
#include "ltr42api.h"
#include "Logger.h"
#include <fstream>
#include <string>

using namespace std;
static TLTR42 ltr42;

// Глобальная переменная для хранения текущего положения
static int current_position = 0;

static int Startltr42() {
    int res;
    const CHAR crate = 0;
    LogEvent("Calling Startltr42: Initializing LTR42 device");
    res = LTR42_Init(&ltr42);
    if (res == 0) {
        LogEvent("LTR42_Init succeeded. Opening device...");
        res = LTR42_Open(&ltr42, (int)0, (WORD)0, &crate, (INT)5);
        if (res == 0) {
            ltr42.Marks.StartMark_Mode = 0;
            LogEvent("Configuring LTR42 device...");
            res = LTR42_Config(&ltr42);
            if (res == 0)
                LogEvent("LTR42 configuration succeeded.");
            else
                LogEvent("LTR42_Config failed with error code: " + std::to_string(res));
        }
        else {
            LogEvent("LTR42_Open failed with error code: " + std::to_string(res));
        }
    }
    else {
        LogEvent("LTR42_Init failed with error code: " + std::to_string(res));
    }

    return res;
}

static void startandwrite(WORD Output) {
    int res;
    std::string logMsg = "Writing to port: 0x" + std::to_string(Output);
   // LogEvent(logMsg);
    res = LTR42_WritePort(&ltr42, Output);
    if (res != 0)
        LogEvent("LTR42_WritePort failed with error code: " + std::to_string(res));
}


// Универсальная функция вращения на 1 шаг
static void rotate(int minus) {
    startandwrite(0 + 4096 + minus + 16384);
    startandwrite(0 + 4096);

    /*if (minus == 8192) {
        current_position += 25;
    }
    else if (minus == 0) {
        current_position -= 25;
    }*/
    LogEvent("Rotation completed. Current position: " + std::to_string(current_position));
}


static void stop_rotate() {
    LogEvent("Stopping rotation...");
    startandwrite(32768);
}

// Возврат в ноль
// Возврат в ноль
static void rotate_to_zero() {
    LogEvent("Starting rotation to zero...");

    // Цикл для перехода в 0
    for (int i = 0; i < 60; i++) {
        startandwrite(0 + 4096 + 16384);
        startandwrite(0 + 4096);
    }

    // Сбрасываем глобальное положение
    current_position = 0;

    // Записываем новое значение в файл
    std::ofstream out("position.txt", std::ios::app);
    if (out.is_open()) {
        LogEvent("Writing to file: position.txt");
        out << current_position << "\n"; // Теперь пишем глобальное значение
    }

    LogEvent("Position reset to 0 completed");
}
