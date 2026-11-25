#pragma once

#include <utility>
#include <vector>
#include "LinearInterpolation.h"

struct Samples {
    std::vector<double> x;
    std::vector<double> y;

    void Add(double first, double second)
    {
        x.push_back(first);
        y.push_back(second);
    }
};

struct CycleData {
    Samples power;
    std::vector<std::pair<double, bool>> alpha;
    std::vector<std::pair<double, bool>> beta;

    double Duration() const;
    double PowerAt(double elapsedMs) const;
    static bool HeaterAt(const std::vector<std::pair<double, bool>>& events, double elapsedMs);
};

struct RunData {
    Samples calibration;
    CycleData cycle;

    double PositionAtPower(double power) const
    {
        return InterpolateLinearly(calibration.y, calibration.x, power);
    }

    double PowerAtPosition(double position) const
    {
        return InterpolateLinearly(calibration.x, calibration.y, position);
    }
};
