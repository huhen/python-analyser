#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(AverageAccumulatorTest, EmptyTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 0);
}

TEST(AverageAccumulatorTest, ValidTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 1.5);
}

TEST(AverageAccumulatorTest, MustBeFinalizedTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(AverageAccumulatorTest, AccumulateAfterFinalizeTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 3}), std::runtime_error);
}

TEST(AverageAccumulatorTest, ComputeAfterResetTest) {
    AverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 1.5);
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 3});
    accumulator.Accumulate(metric::MetricResult{"", 5});
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(accumulator.Get(), 4);
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
