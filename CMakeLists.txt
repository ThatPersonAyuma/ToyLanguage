cmake_minimum_required(VERSION 3.15)

# Nama project
project(ToyLanguage LANGUAGES CXX)

# Standar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tambahkan executable dari lexer.cpp
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE 
        src/ast.h
        src/lexer.h
        src/parser.h
)
