#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::pair<std::string, int>;

class CodeLinesCountMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(CodeLinesCountMetricTest, CheckAllFiles) {
    const auto &[path, expected] = GetParam();
    auto testFilesPath = std::filesystem::path(__FILE__).parent_path() / "files" / path;
    auto file = file::File{testFilesPath};
    auto function = function::FunctionExtractor{}.Get(file).front();
    CodeLinesCountMetric metric;
    auto result = metric.Calculate(function);

    int actual;

    ASSERT_NO_THROW(actual = std::get<int>(result.value));
    EXPECT_EQ(actual, expected);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(CheckAllFiles, CodeLinesCountMetricTest, ::testing::Values(
                            Parameters{"comments.py", 4}, 
                            Parameters{"exceptions.py", 8},
                            Parameters{"if.py", 4},
                            Parameters{"loops.py", 7},
                            Parameters{"many_lines.py", 12},
                            Parameters{"many_parameters.py", 2},
                            Parameters{"match_case.py", 8},
                            Parameters{"nested_if.py", 9},
                            Parameters{"simple.py", 6}, 
                            Parameters{"ternary.py", 2}
                        ));
// clang-format on

}  // namespace analyser::metric::metric_impl
