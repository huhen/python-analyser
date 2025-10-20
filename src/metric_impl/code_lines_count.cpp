#include "metric_impl/code_lines_count.hpp"

#include <unistd.h>

#include "utils.hpp"
#include <ranges>
#include <string>
#include <unordered_set>

namespace analyser::metric::metric_impl {

std::string CodeLinesCountMetric::Name() const { return "code_lines_count"; }

MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const {
    static auto not_comment = boyer_moore_not_contains("(comment");

    auto res = f.ast | vs::split('\n') | vs::filter(not_comment) |
               vs::transform([](const auto &line) { return line | vs::split('-'); }) | vs::join |
               vs::transform([](const auto &range) -> std::optional<int> {
                   auto str_view = std::string_view{range};
                   if (auto start_pos = str_view.find('['); start_pos != std::string_view::npos) {
                       if (auto comma_pos = str_view.find(',', start_pos); comma_pos != std::string_view::npos) {
                           return ToInt(str_view.substr(start_pos + 1, comma_pos - start_pos - 1));
                       }
                   }

                   return std::nullopt;
               }) |
               vs::filter([](const auto &opt) { return opt.has_value(); }) |
               vs::transform([](const auto &opt) { return opt.value(); }) | rs::to<std::unordered_set>();

    return static_cast<int>(res.size());
}

}  // namespace analyser::metric::metric_impl
