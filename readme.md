# MongoDB Simple Query

MongoDB Simple Query (msq) is a header-only C++ library which provides simple way for building MongoDB queries structure
at compile time using operators overloading

Note: query structure is built at compile-time but query itself is built during runtime due to use of bsoncxx library
for making actual queries

## Dependencies

* libbson-1.0

## Examples

```c++
#include <iostream>
#include <bsoncxx/json.hpp>

#include "query.hpp"

int main()
{
    using msq::operator ""_k;
    
    constexpr auto q = "foo"_k == 13;
    
    std::cout << bsoncxx::to_json(bsoncxx::document::value{ q }) << std::endl;
    
    return 0;
}
```

prints

```json
{
  "foo": {"$eq": 13}
}
```

```c++
#include <iostream>
#include <bsoncxx/json.hpp>

#include "query.hpp"

int main()
{
    using msq::operator ""_k;
    
    constexpr auto q = "foo"_k.exists();
    
    std::cout << bsoncxx::to_json(bsoncxx::document::value{ q }) << std::endl;
    
    return 0;
}
```

prints

```json
{
  "foo": {"$exists": true}
}
```

```c++
#include <iostream>
#include <bsoncxx/json.hpp>

#include "query.hpp"

int main()
{
    using msq::operator ""_k;
    
    constexpr auto q = "foo"_k != "bar" and "foo"_k <= 5 or "bar"_k.not_exists() and "foo"_k.exists();
    
    std::cout << bsoncxx::to_json(bsoncxx::document::value{ q }) << std::endl;
    
    return 0;
}
```

prints

```json
{
  "$or": [
    {
      "$and": [
        {"foo": {"$ne": "bar"}},
        {"foo": {"$lte": 5}}
      ]
    },
    {
      "$and": [
        {"bar": {"$exists": false}},
        {"foo": {"$exists": true}}
      ]
    }
  ]
}

```
