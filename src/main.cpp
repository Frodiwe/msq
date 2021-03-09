#include <iostream>

#include <bsoncxx/json.hpp>

#include "query.hpp"

int main()
{
    using msq::operator""_k;

    constexpr auto q = "lol"_k == 7 and "kek"_k != "bar" or "bar"_k.not_exists() and "foo"_k.exists();

    std::cout << bsoncxx::to_json(bsoncxx::document::value{q}) << std::endl;

    return 0;
}
