#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test {

TEST(SumAverageAccumulatorTest, EmptyTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(0, 0));
}

TEST(SumAverageAccumulatorTest, ValidTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(3, 1.5));
}

TEST(SumAverageAccumulatorTest, MustBeFinalizedTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, AccumulateAfterFinalizeTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 3}), std::runtime_error);
}

TEST(SumAverageAccumulatorTest, ComputeAfterResetTest) {
    SumAverageAccumulator accumulator;
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 1});
    accumulator.Accumulate(metric::MetricResult{"", 2});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(3, 1.5));
    accumulator.Reset();
    accumulator.Accumulate(metric::MetricResult{"", 3});
    accumulator.Accumulate(metric::MetricResult{"", 5});
    accumulator.Finalize();
    EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(8, 4));
}

}  // namespace analyser::metric_accumulator::metric_accumulator_impl::test
