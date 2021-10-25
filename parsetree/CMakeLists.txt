add_library (parsetree parsetree.cpp )
add_library (constructor constructor.cpp)

find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(constructor PRIVATE nlohmann_json::nlohmann_json PRIVATE parsetree)