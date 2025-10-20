#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::pair<std::string, int>;

class CyclomaticComplexityMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CyclomaticComplexityMetricTest, CheckAllFiles) {
    const auto &[path, expected] = GetParam();
    auto testFilesPath = std::filesystem::path(__FILE__).parent_path() / "files" / path;
    auto file = file::File{testFilesPath};
    auto function = function::FunctionExtractor{}.Get(file).front();
    CyclomaticComplexityMetric metric;
    auto result = metric.Calculate(function);

    int actual;

    ASSERT_NO_THROW(actual = std::get<int>(result.value));
    EXPECT_EQ(actual, expected);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(CheckAllFiles, CyclomaticComplexityMetricTest, ::testing::Values(
                            Parameters{"comments.py", 1}, 
                            Parameters{"exceptions.py", 5},
                            Parameters{"if.py", 2},
                            Parameters{"loops.py", 4},
                            Parameters{"many_lines.py", 2},
                            Parameters{"many_parameters.py", 2},
                            Parameters{"match_case.py", 5},
                            Parameters{"nested_if.py", 6},
                            Parameters{"simple.py", 2}, 
                            Parameters{"ternary.py", 3}
                        ));
// clang-format on

}  // namespace analyser::metric::metric_impl
