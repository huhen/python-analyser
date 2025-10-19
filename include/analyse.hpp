#include <unistd.h>

#include <algorithm>
#include <print>
#include <ranges>
#include <string>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

// clang-format off

inline auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    return files |
        vs::transform([](const std::string &file_path) {
            auto file = file::File{file_path};
            auto functions = function::FunctionExtractor{}.Get(file);
            return functions;
        }) |
        vs::join |
        vs::transform([&metric_extractor](const function::Function &function) {
            return std::make_pair( function,  metric_extractor.Get(function));
        }) |
        rs::to<std::vector>();
}

auto SplitByClasses(const auto &analysis) {
    return analysis |
        vs::filter([](const auto &item) {
            return item.first.class_name.has_value();
        }) |
        vs::chunk_by([](const auto &l_item, const auto &r_item) {
            return l_item.first.class_name.value() == r_item.first.class_name.value();
        });
}

auto SplitByFiles(const auto &analysis) {
    return analysis |
        vs::chunk_by([](const auto &l_item, const auto &r_item) {
            return l_item.first.filename == r_item.first.filename;
        });
}

void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    rs::for_each(analysis, [&accumulator](const auto &item) {
        accumulator.AccumulateNextFunctionResults(item.second);
    });
}

// clang-format on

}  // namespace analyser
