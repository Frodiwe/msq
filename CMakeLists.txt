cmake_minimum_required(VERSION 3.10)
project(mongodb_simple_query)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pg")
set(MONGOCXX_INCLUDE_DIR /usr/local/include)

find_package(bsoncxx REQUIRED)

include_directories(
        ${MONGOCXX_INCLUDE_DIR}/bsoncxx/v_noabi
        ${MONGOCXX_INCLUDE_DIR}/libbson-1.0
)

add_executable(mongodb_simple_query src/main.cpp)

target_link_libraries(mongodb_simple_query PUBLIC mongo::bsoncxx_shared)