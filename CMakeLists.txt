cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project(MyProject1)

add_subdirectory(D:/1/libpqxx-7.9.0 libpqxx-build) 

add_executable(MyProject1 main.cpp)

target_compile_features(MyProject1 PRIVATE cxx_std_17) 

target_link_libraries(MyProject1 pqxx)