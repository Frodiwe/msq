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

class key {
private:
    string_view name;

    template<typename ValueT>
    query expr(string_view op, const ValueT& value)
    {
        return query{name, bson_builder::make_document(bson_builder::kvp(op, value))};
    }

public:
    constexpr key(string_view name)
        : name{name}
    { }

    template<typename ValueT>
    query operator==(const ValueT& value)
    {
        return expr("$eq", value);
    }

    template<typename ValueT>
    std::enable_if_t<std::is_same_v<std::decay_t<ValueT>, query>, query> operator==(ValueT&& rhs)
    {
        return query{name, rhs.compile()};
    }

    template<typename ValueT>
    query operator!=(const ValueT& value)
    {
        return expr("$ne", value);
    }

    template<typename ValueT>
    query operator>=(const ValueT& value)
    {
        return expr("$gte", value);
    }

    template<typename ValueT>
    query operator>(const ValueT& value)
    {
        return expr("$gt", value);
    }

    template<typename ValueT>
    query operator<=(const ValueT& value)
    {
        return expr("$lte", value);
    }

    template<typename ValueT>
    query operator<(const ValueT& value)
    {
        return expr("$lt", value);
    }

    query operator!()
    {
        return expr("$exists", false);
    }

    operator query()
    {
        return expr("$exists", true);
    }
};

constexpr key operator "" _k(const char* str, size_t len)
{
    return key{string_view{str, len}};
}

}

#endif //KEY_HPP
