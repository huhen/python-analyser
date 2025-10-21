#include <string>

#include "metric.hpp"

namespace analyser::metric::metric_impl {

struct CountParametersMetric final : public IMetric {
protected:
    MetricResult::ValueType CalculateImpl(const function::Function &f) const override;
    std::string Name() const override;
};

}  // namespace analyser::metric::metric_impl
