cmake_minimum_required(VERSION 3.10)
project(pgw)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск необходимых зависимостей
find_package(Boost REQUIRED COMPONENTS system)

# Включение директории с заголовочными файлами
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Основная библиотека PGW
add_library(pgw_lib
    src/bearer.cpp
    src/control_plane.cpp
    src/data_plane.cpp
    src/pdn_connection.cpp
)

# Связывание библиотеки с зависимостями
target_link_libraries(pgw_lib
    PRIVATE Boost::boost
)

# Исполняемый файл (если нужен)
add_executable(pgw_main
    src/main.cpp
)
target_link_libraries(pgw_main
    PRIVATE pgw_lib
)

# Тесты
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    enable_testing()
    
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    add_executable(pgw_tests
        test/data_plane_test.cpp
        src/bearer.cpp
        src/control_plane.cpp
        src/data_plane.cpp
        src/pdn_connection.cpp
    )
    
    target_link_libraries(pgw_tests
        PRIVATE pgw_lib
        ${GTEST_BOTH_LIBRARIES}
        pthread
    )
    
    add_test(NAME pgw_tests COMMAND pgw_tests)
endif()

# Установка clang-format (опционально)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format)
    find_program(CLANG_FORMAT "clang-format")
    if(CLANG_FORMAT)
        add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i -style=file
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
endif()