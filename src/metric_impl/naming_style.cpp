#include "metric_impl/naming_style.hpp"

#include <cctype>
#include <unistd.h>

#include <algorithm>
#include <string>

namespace analyser::metric::metric_impl {

std::string NamingStyleMetric::Name() const { return "naming_style"; }

MetricResult::ValueType NamingStyleMetric::CalculateImpl(const function::Function &f) const {
    const auto &name = f.name;

    if (!name.empty()) {

        auto is_underscore = [](const char &c) { return c == '_'; };

        if (std::isupper(name[0])) {
            if (rs::any_of(name, ::islower) && !rs::any_of(name, is_underscore))
                return "PascalCase";
        } else if (std::islower(name[0])) {
            if (rs::any_of(name, ::isupper)) {
                if (!rs::any_of(name, is_underscore))
                    return "camelCase";
            } else {
                return rs::any_of(name, is_underscore) ? "snake_case" : "lowercase";
            }
        }
    }

    return "unknown";
}

}  // namespace analyser::metric::metric_impl
