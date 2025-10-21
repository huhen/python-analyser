#pragma once
#include <unistd.h>

#include <string>

#include "metric_accumulator.hpp"

namespace analyser::metric_accumulator::metric_accumulator_impl {

struct CategoricalAccumulator : public IAccumulator {
    void Accumulate(const metric::MetricResult &metric_result) override;

    virtual void Finalize() override;

    virtual void Reset() override;

    const std::unordered_map<std::string, int> &Get() const;

private:
    std::unordered_map<std::string, int> categories_freq;
};

}  // namespace analyser::metric_accumulator::metric_accumulator_impl
