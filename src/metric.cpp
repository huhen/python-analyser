#include "metric.hpp"

#include <unistd.h>

#include <ranges>
#include <vector>

#include "function.hpp"

namespace analyser::metric {

void MetricExtractor::RegisterMetric(std::unique_ptr<IMetric> metric) {
    if (metric) {
        metrics.emplace_back(std::move(metric));
    } else {
        throw std::runtime_error("MetricExtractor::RegisterMetric metric is nullptr");
    }
}

MetricResults MetricExtractor::Get(const function::Function &func) const {
    return metrics | vs::transform([&func](const auto &metric) { return metric->Calculate(func); }) |
           rs::to<std::vector>();
}

}  // namespace analyser::metric
