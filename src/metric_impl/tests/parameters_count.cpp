#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::pair<std::string, int>;

class CountParametersMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CountParametersMetricTest, CheckAllFiles) {
    const auto &[path, expected] = GetParam();
    auto testFilesPath = std::filesystem::path(__FILE__).parent_path() / "files" / path;
    auto file = file::File{testFilesPath};
    auto function = function::FunctionExtractor{}.Get(file).front();
    CountParametersMetric metric;
    auto result = metric.Calculate(function);

    int actual;

    ASSERT_NO_THROW(actual = std::get<int>(result.value));
    EXPECT_EQ(actual, expected);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(CheckAllFiles, CountParametersMetricTest, ::testing::Values(
                            Parameters{"comments.py", 3}, 
                            Parameters{"exceptions.py", 0},
                            Parameters{"if.py", 1},
                            Parameters{"loops.py", 1},
                            Parameters{"many_lines.py", 0},
                            Parameters{"many_parameters.py", 5},
                            Parameters{"match_case.py", 1},
                            Parameters{"nested_if.py", 2},
                            Parameters{"simple.py", 0}, 
                            Parameters{"ternary.py", 1}
                        ));
// clang-format on

}  // namespace analyser::metric::metric_impl
