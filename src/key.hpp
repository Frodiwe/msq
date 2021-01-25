//
// Created by Fiodar Navumenka on 8/23/18.
//

#ifndef KEY_HPP
#define KEY_HPP

#include <string>
#include <string_view>

#include "query.hpp"

namespace msq {

class key {
private:
    std::string_view name;

    template<typename ValueT>
    query expr(const std::string& op, const ValueT& value)
    {
        return query{std::string{name}, bson_builder::make_document(bson_builder::kvp(op, value))};
    }

public:
    constexpr key(std::string_view name)
        : name{name}
    { }

    template<typename ValueT>
    query operator==(const ValueT& value)
    {
        return expr("$eq", value);
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

    template<typename ValueT>
    std::enable_if_t<std::is_same_v<std::decay_t<ValueT>, query>, query> operator==(ValueT&& rhs)
    {
        return query{name, rhs.compile()};
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
    return key{std::string_view{str, len}};
}

}

#endif //KEY_HPP
