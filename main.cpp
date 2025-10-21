#include <unistd.h>

#include <algorithm>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

namespace metric_impl = analyser::metric::metric_impl;
namespace metric_accumulator_impl = analyser::metric_accumulator::metric_accumulator_impl;

int main(int argc, char *argv[]) {
    analyser::cmd::ProgramOptions options;
    if (!options.Parse(argc, argv)) {
        return 1;
    }

    analyser::metric::MetricExtractor metric_extractor;
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
    metric_extractor.RegisterMetric(std::make_unique<metric_impl::NamingStyleMetric>());

    auto metric_results = analyser::AnalyseFunctions(options.GetFiles(), metric_extractor);

    auto print_values = [](const auto &metrics) {
        rs::for_each(metrics, [](const auto &metric) {
            std::print("    {}: ", metric.first);
            std::visit([](auto &&val) { std::println("{}", val); }, metric.second);
        });
    };

    rs::for_each(metric_results, [&print_values](const auto &item) {
        const auto &func = item.first;
        std::println("{}{}::{}:", func.filename, func.class_name.has_value() ? "::" + func.class_name.value() : "",
                     func.name);

        print_values(item.second | vs::transform([](const auto &metric) {
                         return std::make_pair(metric.metric_name, metric.value);
                     }));
    });

    analyser::metric_accumulator::MetricsAccumulator accumulator;
    accumulator.RegisterAccumulator("count_parameters",
                                    std::make_unique<metric_accumulator_impl::AverageAccumulator>());
    accumulator.RegisterAccumulator("code_lines_count",
                                    std::make_unique<metric_accumulator_impl::SumAverageAccumulator>());
    accumulator.RegisterAccumulator("cyclomatic_complexity",
                                    std::make_unique<metric_accumulator_impl::SumAverageAccumulator>());
    accumulator.RegisterAccumulator("naming_style",
                                    std::make_unique<metric_accumulator_impl::CategoricalAccumulator>());

    auto get_accumulators = [&accumulator](const auto &group) {
        accumulator.ResetAccumulators();
        analyser::AccumulateFunctionAnalysis(group, accumulator);

        const auto &avg_params =
            accumulator.GetFinalizedAccumulator<metric_accumulator_impl::AverageAccumulator>("count_parameters").Get();
        const auto &sum_avg_lines =
            accumulator.GetFinalizedAccumulator<metric_accumulator_impl::SumAverageAccumulator>("code_lines_count")
                .Get();
        const auto &sum_avg_complexity =
            accumulator.GetFinalizedAccumulator<metric_accumulator_impl::SumAverageAccumulator>("cyclomatic_complexity")
                .Get();
        const auto &naming_style_count =
            accumulator.GetFinalizedAccumulator<metric_accumulator_impl::CategoricalAccumulator>("naming_style").Get();

        using pair_type = std::pair<std::string, std::variant<int, double>>;

        std::vector<pair_type> result;
        result.reserve(5 + naming_style_count.size());

        result.emplace_back("count_parameters", avg_params);
        result.emplace_back("code_lines_count_average", sum_avg_lines.average);
        result.emplace_back("code_lines_count_sum", sum_avg_lines.sum);
        result.emplace_back("cyclomatic_complexity_average", sum_avg_complexity.average);
        result.emplace_back("cyclomatic_complexity_sum", sum_avg_complexity.sum);

        std::ranges::for_each(naming_style_count, [&result](const auto &item) {
            result.emplace_back(std::format("{}_count", item.first), item.second);
        });

        return result;
    };

    auto print_accumulators = [&print_values](auto &&groups) {
        rs::for_each(groups, [&print_values](const auto &metric) {
            std::println("Accumulated Analysis for {}:", metric.first);
            print_values(metric.second);
        });
    };

    auto by_files =
        analyser::SplitByFiles(metric_results) | vs::filter([](const auto &group) { return !group.empty(); }) |
        vs::transform([&](const auto &group) {
            return std::make_pair(std::format("file {}", group.front().first.filename), get_accumulators(group));
        });

    print_accumulators(by_files);

    auto by_classes =
        analyser::SplitByClasses(metric_results) |
        vs::filter([](const auto &group) { return !group.empty() || group.front().first.class_name.has_value(); }) |
        vs::transform([&](const auto &group) {
            return std::make_pair(std::format("class {}", group.front().first.class_name.value()),
                                  get_accumulators(group));
        });

    print_accumulators(by_classes);

    print_accumulators(std::views::single(std::make_pair("all", get_accumulators(metric_results))));

    return 0;
}
