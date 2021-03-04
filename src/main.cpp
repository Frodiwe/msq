#include <iostream>

#include <bsoncxx/json.hpp>

#include "key.hpp"

int main()
{
    using msq::operator""_k;

    constexpr auto q = "lol"_k == 5 && "kek"_k == 6 || not "foo"_k;

    std::cout << bsoncxx::to_json(bsoncxx::document::value{q}) << std::endl;

    return 0;
}
