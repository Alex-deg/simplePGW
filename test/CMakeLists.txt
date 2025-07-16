# Включаем поддержку тестов (если используем Google Test или Catch2)
enable_testing()

# Пример для Google Test
find_package(GTest REQUIRED)

# Собираем тестовый модуль
add_executable(pgw_tests
    data_plane_test.cpp
)

# Линкуем тесты с основной библиотекой и GTest
target_link_libraries(pgw_tests PRIVATE pgw_lib GTest::GTest GTest::Main)

# Добавляем тест в CTest
add_test(NAME pgw_tests COMMAND pgw_tests)