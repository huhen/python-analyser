#include "metric_impl/cyclomatic_complexity.hpp"

#include <cctype>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <ranges>
#include <string>
#include <string_view>

namespace analyser::metric::metric_impl {

std::string CyclomaticComplexityMetric::Name() const { return "cyclomatic_complexity"; }

MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const {
    // clang-format off
    static constexpr std::array<std::string_view, 12> keywords = {
        "(if_", 
        "(else_", 
        "(elif_", 
        "(while_", 
        "(for_", 
        "(try_", 
        "(except_", 
        "(finally_", 
        "(match_", 
        "(case_", 
        "(assert_", 
        "(conditional_"
    };

    return static_cast<int>(
        rs::count_if(
            f.ast |
            vs::split('\n') |
            vs::transform([](const auto &line) {
                return std::string_view{vs::drop_while(line, ::isspace)};
            }),
            [](const auto &line) {
                return rs::any_of(keywords, [&line](const auto &keyword) {
                    return line.starts_with(keyword);
                });
            })
        + 1);
    // clang-format on
}

}  // namespace analyser::metric::metric_impl
