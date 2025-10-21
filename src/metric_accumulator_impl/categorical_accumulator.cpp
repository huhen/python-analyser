#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <unistd.h>

#include <string>
#include <variant>

namespace analyser::metric_accumulator::metric_accumulator_impl {

void CategoricalAccumulator::Accumulate(const metric::MetricResult &metric_result) {
    if (is_finalized) {
        throw std::runtime_error("CategoricalAccumulator is finalized");
    }

    if (const auto string_value = std::get_if<std::string>(&metric_result.value); string_value) {
        categories_freq[*string_value]++;
    }
}

void CategoricalAccumulator::Finalize() {
    if (is_finalized) {
        return;
    }

    is_finalized = true;
}

void CategoricalAccumulator::Reset() {
    categories_freq.clear();
    is_finalized = false;
}

const std::unordered_map<std::string, int> &CategoricalAccumulator::Get() const {
    if (!is_finalized) {
        throw std::runtime_error("CategoricalAccumulator not finalized");
    }
    return categories_freq;
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
