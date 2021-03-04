#include <iostream>

#include <bsoncxx/json.hpp>

#include "key.hpp"

int main()
{
    using msq::operator""_k;

    const bsoncxx::document::value query = "lol"_k == 5 && "kek"_k == 6 && not "foo"_k;

    std::cout << bsoncxx::to_json(query) << std::endl;

    return 0;
}
