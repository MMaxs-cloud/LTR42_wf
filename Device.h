#pragma once

namespace ltr42 {
    constexpr int positionStep = 25;
    constexpr int manualPositionLimit = 1425;
    constexpr int cyclePositionLimit = 1450;

    enum Output : unsigned short {
        Beta = 64,
        Alpha = 128,
        Enable = 4096,
        Forward = 8192,
        Pulse = 16384,
        ZeroComplete = 32768
    };

    int Connect();
    void Close();
    int Write(unsigned short output);
    int Step(unsigned short output, unsigned long pulseMs);
    int MoveToZero();
}
