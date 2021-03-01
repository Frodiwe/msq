#include <iostream>

#include <bsoncxx/json.hpp>

#include "key.hpp"

int main()
{
    using msq::operator""_k;

    const bsoncxx::document::value doc = "lol"_k == "kek" or "foo"_k != "bar";

    std::cout << bsoncxx::to_json(doc.view()) << std::endl;

    return 0;
}
