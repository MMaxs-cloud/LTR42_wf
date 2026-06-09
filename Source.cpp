#include "ltr11api.h"
#include "ltrapi.h"
#include "ltr42api.h"
#include "Logger.h"
#include <fstream>
#include <string>
#include "header.h"
using namespace std;
static TLTR42 ltr42;

// Глобальная переменная для хранения текущего положения
static int current_position = 0;

static int Startltr42() {
    int res;
    const CHAR crate = 0;
    ENLOG("Calling Startltr42: Initializing LTR42 device");
    res = LTR42_Init(&ltr42);
    if (res == 0) {
        ENLOG("LTR42_Init succeeded. Opening device...");
        res = LTR42_Open(&ltr42, (int)0, (WORD)0, &crate, (INT)4);
        if (res == 0) {
            ltr42.Marks.StartMark_Mode = 0;
            ENLOG("Configuring LTR42 device...");
            res = LTR42_Config(&ltr42);
            if (res == 0)
                ENLOG("LTR42 configuration succeeded.");
            else
                ENLOG("LTR42_Config failed with error code: ")(res);
        }
        else {
            ENLOG("LTR42_Open failed with error code: ")(res);
        }
    }
    else {
        ENLOG("LTR42_Init failed with error code: ")(res);
    }

    return res;
}

static void startandwrite(WORD Output) {
    int res;
    //std::string logMsg = "Writing to port: 0x" + std::to_string(Output);
   // ENLOG(logMsg);
    res = LTR42_WritePort(&ltr42, Output);
   // ENLOG("Output")(Output);
   /* if (res != 0)
        ENLOG("LTR42_WritePort failed with error code: ")(res);*/
}


// Универсальная функция вращения на 1 шаг
static void rotate(int minus) {
    startandwrite(0 + 4096 + minus + 16384 + 64);
    Sleep(10);
    startandwrite(0 + 4096 + minus + 64);

    /*if (minus == 8192) {
        current_position += 25;
    }
    else if (minus == 0) {
        current_position -= 25;
    }*/
    //ENLOG("Rotation completed. Current position: ")(current_position);
}


static void stop_rotate() {
    ENLOG("Stopping rotation...");
    startandwrite(32768);
}

// Возврат в ноль
// Возврат в ноль
static void rotate_to_zero() {
    ENLOG("Starting rotation to zero...");

    // Цикл для перехода в 0
    for (int i = 0; i < 100; i++) {
        startandwrite(0 + 4096 + 16384);
        startandwrite(0 + 4096);
    }
    //stop_rotate;
    startandwrite(32768);

    // Сбрасываем глобальное положение
    current_position = 0;

    // Записываем новое значение в файл
    /*std::ofstream out("position.txt", std::ios::app);
    if (out.is_open()) {
        ENLOG("Writing to file: position.txt");
        out << current_position << "\n"; // Теперь пишем глобальное значение
    }*/

    ENLOG("Position reset to 0 completed");
}
