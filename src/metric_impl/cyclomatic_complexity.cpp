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
    static constexpr std::array<std::string_view, 14> keywords = {
        "if_statement",
        "if_clause", 
        "else_clause", 
        "elif_clause", 
        "while_statement", 
        "for_statement", 
        "for_in_clause", 
        "try_statement", 
        "except_clause", 
        "finally_clause", 
        "match_statement", 
        "case_clause", 
        "assert_statement", 
        "conditional_expression"
    };

    return static_cast<int>(
        rs::count_if(
            f.ast |
            vs::split('\n') |
            vs::transform([](const auto &line) {
                auto trimmed = std::string_view{
                    vs::drop_while(line, ::isspace) | vs::drop_while([](const auto &c) {
                        return c=='(';
                    })};
                constexpr std::string_view alternative_prefix = "alternative: (";
                return trimmed.starts_with(alternative_prefix) ?
                    trimmed.substr(alternative_prefix.length()) : trimmed;
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
