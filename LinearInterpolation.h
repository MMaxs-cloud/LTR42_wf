#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

// Inputs and outputs are paired, with at least two strictly increasing inputs.
inline double InterpolateLinearly(const std::vector<double>& inputs,
                                  const std::vector<double>& outputs,
                                  double input)
{
    const auto match = std::lower_bound(inputs.begin(), inputs.end(), input);
    const auto index = static_cast<std::size_t>(match - inputs.begin());
    if (match != inputs.end() && *match == input) return outputs[index];

    // Use the nearest end segment for extrapolation.
    const auto right = std::clamp(index, std::size_t{1}, inputs.size() - 1);
    const auto left = right - 1;
    const double slope = (outputs[right] - outputs[left]) / (inputs[right] - inputs[left]);
    const auto anchor = match == inputs.end() ? right : left;
    return outputs[anchor] + (input - inputs[anchor]) * slope;
}

// Store calibration points for the historical form's bidirectional lookup.
class LinearCalibration {
public:
    void SetPoints(const std::vector<double>& inputs, const std::vector<double>& outputs)
    {
        inputs_ = inputs;
        outputs_ = outputs;
    }

    double operator()(double input) const
    {
        if (inputs_.size() < 2) return 0.0;
        return InterpolateLinearly(inputs_, outputs_, input);
    }

private:
    std::vector<double> inputs_;
    std::vector<double> outputs_;
};
