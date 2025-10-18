#include "metric_accumulator_impl/average_accumulator.hpp"

#include <unistd.h>

#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void AverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized) {
        throw std::runtime_error("AverageAccumulator is finalized");
    }

    if (const auto int_value = std::get_if<int>(&metric_result.value); int_value) {
        sum += *int_value;
        ++count;
    }
}

void AverageAccumulator::Finalize() {
    if (count != 0) {
        average = static_cast<double>(sum) / count;
    }

    is_finalized = true;
}

void AverageAccumulator::Reset() {
    average = sum = count = 0;
    is_finalized = false;
}

double AverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("AverageAccumulator not finalized");
    }
    return average;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
