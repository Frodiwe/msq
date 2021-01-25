#include <iostream>
#include "key.hpp"

#include <bsoncxx/json.hpp>

int main()
{
    const bsoncxx::document::value doc = "lol"_k == "kek";

    std::cout << bsoncxx::to_json(doc.view()) << std::endl;

    return 0;
}
