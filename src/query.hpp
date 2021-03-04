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

namespace bson_builder = bsoncxx::builder::basic;

class key;

template<typename, typename>
constexpr bool is_same_template{false};

template<
    template<typename...> typename T,
    typename... A,
    typename... B
>
constexpr bool is_same_template<T<A...>, T<B...>>{true};

template<typename ValueL, typename ValueR>
class query {
    using document = bsoncxx::document::value;
    using document_view_or_value = bsoncxx::document::view_or_value;
    using string_view = bsoncxx::stdx::string_view;

private:
    ValueL lhs;

    ValueR rhs;

    string_view op;

    template<typename T = ValueL, std::enable_if_t<is_same_template<T, query>, bool> = true>
    document compile()
    {
        return bson_builder::make_document(
            bson_builder::kvp(op, bson_builder::make_array(bson_builder::concatenate(lhs), bson_builder::concatenate(rhs)))
        );
    }

    template<typename T = ValueL, std::enable_if_t<std::is_same_v<T, string_view>, bool> = true>
    document compile()
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

    template<typename T>
    constexpr auto operator&&(T&& rhs)
    {
        return query<query, T>{std::move(*this), std::forward<T>(rhs), "$and"};
    }

    template<typename T>
    constexpr auto operator||(T&& rhs)
    {
        return query<query, T>{std::move(*this), std::forward<T>(rhs), "$or"};
    }

    operator document()
    {
        return compile();
    }

    operator document_view_or_value()
    {
        return compile();
    }
};

}

#endif //QUERY_HPP
