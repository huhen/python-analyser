#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <ranges>
#include <vector>

namespace analyser::metric_accumulator {

void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const {
    rs::for_each(metric_results, [this](const auto &res) {
        if (const auto &it = accumulators.find(res.metric_name); it != accumulators.end()) {
            it->second->Accumulate(res);
        }
    });
}

void MetricsAccumulator::ResetAccumulators() {
    rs::for_each(accumulators | vs::values, [](const auto &acc) { acc->Reset(); });
}

}  // namespace analyser::metric_accumulator
