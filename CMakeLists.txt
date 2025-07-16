cmake_minimum_required(VERSION 3.12)
project(PGW LANGUAGES CXX)

# Настройка стандарта C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение Boost (если используется, например, для ASIO)
find_package(Boost REQUIRED COMPONENTS system)

# Добавление поддиректорий
add_subdirectory(src)
add_subdirectory(test)  # Если есть тесты