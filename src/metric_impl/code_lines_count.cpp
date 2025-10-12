#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include "utils.hpp"
#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <unordered_set>

namespace analyser::metric::metric_impl {

std::string CodeLinesCountMetric::Name() const { return "code_lines_count"; }

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    static constexpr std::string_view comment_pattern = "(comment";

    auto res = f.ast | vs::split('\n') | vs::transform([](const auto &range) -> std::optional<int> {
                   const auto str_view = std::string_view{range};

                   static const std::boyer_moore_searcher searcher(comment_pattern.begin(), comment_pattern.end());

                   if (std::search(str_view.begin(), str_view.end(), searcher) == str_view.end()) {
                       if (auto start_pos = str_view.find('['); start_pos != std::string_view::npos) {
                           if (auto comma = str_view.find(',', start_pos); comma != std::string_view::npos) {
                               return ToInt(str_view.substr(start_pos + 1, comma - start_pos - 1));
                           }
                       }
                   }

                   return std::nullopt;
               }) |
               vs::filter([](const auto &opt) { return opt.has_value(); }) |
               vs::transform([](const auto &opt) { return opt.value(); }) | rs::to<std::unordered_set>();

    return static_cast<int>(res.size());
}

}  // namespace analyser::metric::metric_impl
