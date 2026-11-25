#include "Device.h"
#include <ltr42api.h>

namespace {
    TLTR42 device{};
    bool connected = false;
}

int ltr42::Connect()
{
    if (connected) return 0;

    int result = LTR42_Init(&device);
    if (result != 0) return result;

    result = LTR42_Open(&device, 0, 0, "", 4);
    if (result != 0) return result;

    device.Marks.StartMark_Mode = 0;
    result = LTR42_Config(&device);
    if (result != 0) {
        LTR42_Close(&device);
        return result;
    }

    connected = true;
    result = Write(Beta);
    if (result != 0) Close();
    return result;
}

void ltr42::Close()
{
    if (!connected) return;
    LTR42_Close(&device);
    connected = false;
}

int ltr42::Write(unsigned short output)
{
    return LTR42_WritePort(&device, output);
}

int ltr42::Step(unsigned short output, unsigned long pulseMs)
{
    int pulseResult = Write(output | Pulse);
    if (pulseResult == 0 && pulseMs != 0) Sleep(pulseMs);
    int resetResult = Write(output);
    return pulseResult != 0 ? pulseResult : resetResult;
}

int ltr42::MoveToZero()
{
    int result = 0;
    for (int i = 0; i < 100 && result == 0; ++i) {
        result = Step(Enable, 0);
    }

    int finishResult = Write(ZeroComplete);
    return result != 0 ? result : finishResult;
}
