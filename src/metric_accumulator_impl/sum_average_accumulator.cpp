#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <unistd.h>

#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void SumAverageAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized) {
        throw std::runtime_error("SumAverageAccumulator is finalized");
    }

    if (const auto int_value = std::get_if<int>(&metric_result.value); int_value) {
        sum += *int_value;
        ++count;
    }
}

void SumAverageAccumulator::Finalize() {
    if (is_finalized) {
        return;
    }

    if (count > 0) {
        average = static_cast<double>(sum) / count;
    }
    is_finalized = true;
}

void SumAverageAccumulator::Reset() {
    average = sum = count = 0;
    is_finalized = false;
}

SumAverageAccumulator::SumAverage SumAverageAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("SumAverageAccumulator not finalized");
    }
    return {sum, average};
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
