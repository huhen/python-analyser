#pragma once

#include <algorithm>
#include <charconv>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    int result{};
    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size()) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}

template <bool ShouldContain = true>
class boyer_moore_search_view {
public:
    constexpr explicit boyer_moore_search_view(std::string_view pattern) : searcher_(pattern.begin(), pattern.end()) {}

    bool operator()(const auto &range) const {
        auto result = std::search(range.cbegin(), range.cend(), searcher_);
        if constexpr (ShouldContain) {
            return result != range.cend();
        } else {
            return result == range.cend();
        }
    }

private:
    const std::boyer_moore_searcher<std::string_view::const_iterator> searcher_;
};

constexpr auto boyer_moore_contains(std::string_view pattern) { return boyer_moore_search_view<true>{pattern}; }

constexpr auto boyer_moore_not_contains(std::string_view pattern) { return boyer_moore_search_view<false>{pattern}; }