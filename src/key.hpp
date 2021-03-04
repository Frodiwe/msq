//
// Created by Fiodar Navumenka on 8/23/18.
//

#ifndef KEY_HPP
#define KEY_HPP

#include <string>
#include <string_view>

#include "query.hpp"

namespace msq {

using string_view = bsoncxx::stdx::string_view;

constexpr string_view operator "" _sv(const char* str, size_t len)
{
    return string_view{str, len};
}

class key {
private:
    string_view name;

public:
    constexpr key(string_view name)
        : name{name}
    { }

    template<typename ValueT>
    constexpr auto operator==(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$eq"_sv);
    }

    template<typename ValueT>
    constexpr auto operator!=(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$ne"_sv);
    }

    template<typename ValueT>
    constexpr auto operator>=(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$gte"_sv);
    }

    template<typename ValueT>
    constexpr auto operator>(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$gt"_sv);
    }

    template<typename ValueT>
    constexpr auto operator<=(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$lte"_sv);
    }

    template<typename ValueT>
    constexpr auto operator<(ValueT&& value)
    {
        return query<string_view, ValueT>(std::move(name), std::forward<ValueT>(value), "$lt"_sv);
    }

    constexpr auto operator!()
    {
        return query<string_view, bool>(std::move(name), false, "$exists"_sv);
    }

    constexpr operator query<string_view, bool>()
    {
        return query<string_view, bool>(std::move(name), true, "$exists"_sv);
    }
};

constexpr key operator "" _k(const char* str, size_t len)
{
    return key{string_view{str, len}};
}

}

#endif //KEY_HPP
