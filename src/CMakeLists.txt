add_library(pgw_lib
    bearer.cpp
    bearer.h
    control_plane.cpp
    control_plane.h
    data_plane.cpp
    data_plane.h
    pdn_connection.cpp
    pdn_connection.h
)

# Линкуем зависимости (например, Boost)
target_link_libraries(pgw_lib PRIVATE Boost::boost)

# Собираем исполняемый файл (если есть main.cpp)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(pgw_main main.cpp)
    target_link_libraries(pgw_main PRIVATE pgw_lib)
endif()