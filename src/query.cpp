//
// Created by Fiodar Navumenka on 8/24/18.
//

#include "query.hpp"

document query::compile()
{
    if (not and_builder.view().empty()) {
        or_builder.emplace_back(and_builder.extract());
    }

    if (or_builder.size() == 1) {
        return or_builder[0];
    }

    auto fields_builder = bson_builder::array{};

    for (const auto& v : or_builder) {
        fields_builder.append(v);
    }

    return bson_builder::make_document(bson_builder::kvp("$or", fields_builder.extract()));
}

query::operator document_view_or_value()
{
    return compile();
}

query::operator document()
{
    return compile();
}