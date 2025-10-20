

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(CategoricalAccumulatorTest, EmptyTest) {
    CategoricalAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    auto actual = accumulator.Get();
    EXPECT_TRUE(actual.empty());
}

TEST(CategoricalAccumulatorTest, ValidTest) {
    CategoricalAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Finalize();
    auto actual = accumulator.Get();
    EXPECT_EQ(actual["1"], 1);
    EXPECT_EQ(actual["2"], 2);
}

TEST(CategoricalAccumulatorTest, MustBeFinalizedTest) {
    CategoricalAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, AccumulateAfterFinalizeTest) {
    CategoricalAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Finalize();
    EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", "3"}), std::runtime_error);
}

TEST(CategoricalAccumulatorTest, ComputeAfterResetTest) {
    CategoricalAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Finalize();
    auto actual = accumulator.Get();
    EXPECT_EQ(actual["1"], 1);
    EXPECT_EQ(actual["2"], 2);
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "1"});
    accumulator.Accumulate(metric::MetricResult{"", "2"});
    accumulator.Finalize();
    actual = accumulator.Get();
    EXPECT_EQ(actual["1"], 2);
    EXPECT_EQ(actual["2"], 1);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
