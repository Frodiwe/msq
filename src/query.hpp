//
// Created by Fiodar Navumenka on 8/23/18.
//

#ifndef QUERY_HPP
#define QUERY_HPP

#include <string>
#include <string_view>
#include <vector>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/builder/basic/array.hpp>

namespace msq {

template<typename, typename>
constexpr bool is_same_template{false};

template<template<typename...> typename T, typename... A,typename... B>
constexpr bool is_same_template<T<A...>, T<B...>>{true};

namespace bson_builder = bsoncxx::builder::basic;

using string_view = bsoncxx::stdx::string_view;

class key;

constexpr string_view operator "" _sv(const char* str, size_t len)
{
    return string_view{str, len};
}

template<typename ValueL, typename ValueR>
class query {
    using document = bsoncxx::document::value;
    using document_view_or_value = bsoncxx::document::view_or_value;
    using string_view = bsoncxx::stdx::string_view;

private:
    ValueL lhs;

    ValueR rhs;

    string_view op;

//    template<typename T>
//    std::enable_if_t<std::is_same_v<T, key>, query<string_view, bool>> convert(T&& value) const
//    {
//        return value.operator query<string_view, bool>();
//    }
//
//    template<typename T>
//    std::enable_if_t<is_same_template<std::decay_t<T>, query>, T> convert(T&& value) const
//    {
//        return std::forward<T>(value);
//    }

    template<typename T = ValueL, std::enable_if_t<is_same_template<T, query> || std::is_same_v<T, key>, bool> = true>
    document compile() const
    {
        return bson_builder::make_document(
            bson_builder::kvp(
                op,
                bson_builder::make_array(bson_builder::concatenate(lhs), bson_builder::concatenate(rhs))
            )
        );
    }

    template<typename T = ValueL, std::enable_if_t<std::is_same_v<T, string_view>, bool> = true>
    document compile() const
    {
        return bson_builder::make_document(
            bson_builder::kvp(lhs, bson_builder::make_document(bson_builder::kvp(op, rhs)))
        );
    }

public:
    constexpr query(ValueL&& lhs, ValueR&& rhs, string_view op)
        : lhs{std::forward<ValueL>(lhs)},
          rhs{std::forward<ValueR>(rhs)},
          op{op}
    { }

    template<typename T1, typename T2>
    constexpr auto operator and(query<T1, T2>&& rhs)
    {
        return query<query, query<T1, T2>>{std::move(*this), std::move(rhs), "$and"_sv};
    }

    template<typename T>
    constexpr auto operator or(T&& rhs)
    {
        return query<query, T>{std::move(*this), std::forward<T>(rhs), "$or"_sv};
    }

    operator document() const
    {
        return compile();
    }

    operator document_view_or_value() const
    {
        return compile();
    }
};

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

#endif //QUERY_HPP
