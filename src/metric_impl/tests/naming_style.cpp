#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl {

using Parameters = std::pair<std::string, std::string>;

class NamingStyleMetricTest : public ::testing::TestWithParam<Parameters> {};

TEST_P(NamingStyleMetricTest, CheckAllFiles) {
    const auto &[path, expected] = GetParam();
    auto testFilesPath = std::filesystem::path(__FILE__).parent_path() / "files" / path;
    auto file = file::File{testFilesPath};
    auto function = function::FunctionExtractor{}.Get(file).front();
    NamingStyleMetric metric;
    auto result = metric.Calculate(function);

    std::string actual;

    ASSERT_NO_THROW(actual = std::get<std::string>(result.value));
    EXPECT_EQ(actual, expected);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(CheckAllFiles, NamingStyleMetricTest, ::testing::Values(
                            Parameters{"comments.py", "unknown"}, 
                            Parameters{"exceptions.py", "unknown"},
                            Parameters{"if.py", "camelCase"},
                            Parameters{"loops.py", "PascalCase"},
                            Parameters{"many_lines.py", "lowercase"},
                            Parameters{"many_parameters.py", "unknown"},
                            Parameters{"match_case.py", "unknown"},
                            Parameters{"nested_if.py", "PascalCase"},
                            Parameters{"simple.py", "snake_case"}, 
                            Parameters{"ternary.py", "unknown"}
                        ));
// clang-format on

}  // namespace analyser::metric::metric_impl
