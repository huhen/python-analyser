#pragma once
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "metric.hpp"

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string &metric_name, std::unique_ptr<Accumulator> acc) {
        auto [_, ok] = accumulators.try_emplace(metric_name, std::move(acc));
        if (!ok) {
            throw std::runtime_error(std::format("Accumulator with name '{}' already exists", metric_name));
        }
    }

    template <typename Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        const auto &it = accumulators.find(metric_name);
        if (it == accumulators.end()) {
            throw std::runtime_error(std::format("Accumulator with name '{}' does not exist", metric_name));
        }

        auto *acc_ptr = dynamic_cast<Accumulator *>(it->second);
        if (!acc_ptr) {
            throw std::runtime_error(std::format("Accumulator with name '{}' incorrect type", metric_name));
        }

        if (!acc_ptr->is_finalized) {
            throw std::runtime_error(std::format("Accumulator with name '{}' not finalized", metric_name));
        }
        return *acc_ptr;
    }

    void AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const;

    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> accumulators;
};

}  // namespace analyser::metric_accumulator
