//
// Created by Fiodar Navumenka on 8/24/18.
//

#include "key.hpp"

key::key(const std::string& name)
    : name { name }
{ }

key::key(std::string_view name)
    : name { name }
{ }

key::key(std::string&& name)
    : name { std::move(name) }
{ }

query key::operator!()
{
    return expr("$exists", false);
}

key::operator query()
{
    return expr("$exists", true);
}

key operator"" _k(const char* str, size_t len)
{
    return key { std::string { str, len } };
}