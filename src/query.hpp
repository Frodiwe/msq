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

using document_builder = bson_builder::document;
using array_builder = bson_builder::array;

using document = bsoncxx::document::value;
using document_view_or_value = bsoncxx::document::view_or_value;
using string_view = bsoncxx::stdx::string_view;

class key;

class query {
private:
    document_builder and_builder;

    array_builder or_builder;

    document compile()
    {
        if (not and_builder.view().empty()) {
            or_builder.append(and_builder.extract());
        }

        if (std::distance(or_builder.view().begin(), or_builder.view().end()) == 1) {
            return document{or_builder.view()[0].get_document().view()};
        }

        return bson_builder::make_document(bson_builder::kvp("$or", or_builder.extract()));
    }

public:
    template<typename ValueT>
    query(string_view name, ValueT&& value)
        : and_builder{},
          or_builder{}
    {
        and_builder.append(bson_builder::kvp(name, std::forward<ValueT>(value)));
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>, query>, query&> operator&&(T&& rhs)
    {
        and_builder.append(rhs.compile());

        return *this;
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>, key>, query&> operator&&(T&& rhs)
    {
        and_builder.append(query{std::forward<T>(rhs)});

        return *this;
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<std::decay_t<T>, query>, query&> operator||(T&& rhs)
    {
        or_builder.append(rhs.compile());

        return *this;
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
