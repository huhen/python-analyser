#include "metric_impl/parameters_count.hpp"

#include <ranges>
#include <string>

namespace analyser::metric::metric_impl {

std::string CountParametersMetric::Name() const { return "count_parameters"; }

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const {
    static constexpr std::string_view parameters_pattern = "parameters:";
    static const std::boyer_moore_searcher parameters_searcher(parameters_pattern.begin(), parameters_pattern.end());

    auto p_begin = std::search(f.ast.cbegin(), f.ast.cend(), parameters_searcher);

    if (p_begin == f.ast.cend()) {
        return 0;
    }

    const auto char_range = std::string_view(p_begin + parameters_pattern.length(), f.ast.cend());

    using State = std::pair<int, int>;  // count, level

    bool need_more = true;

    // clang-format off
    auto [count, _] = std::ranges::fold_left(
        char_range | vs::take_while([&](char) {
            return need_more;
        }), 
        State{0, 0},
        [&](const State &acc, const char &c) {
            auto [count, level] = acc;

            if (c == '(') {
                if (level++ == 1) {
                    ++count;
                }
            } else if (c == ')') {
                if (--level == 0) {
                    need_more = false;
                }
            }

            return State{count, level};
        }
    );
    // clang-format on

    return count;
}

}  // namespace analyser::metric::metric_impl
