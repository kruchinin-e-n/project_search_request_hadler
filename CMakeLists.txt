cmake_minimum_required(VERSION 3.11)
project(project_search_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(search_server)

target_sources(
	search_server PRIVATE
    src/main.cpp
    src/document.cpp
    src/process_queries.cpp
    src/read_input_functions.cpp
    src/request_queue.cpp
    src/search_server.cpp
    src/string_processing.cpp
    src/test_example_functions.cpp
)


target_sources(
	search_server PRIVATE
    inc/document.h
    inc/concurrent_map.h
    inc/paginator.h
    inc/process_queries.h
    inc/read_input_functions.h
    inc/request_queue.h
    inc/search_server.h
    inc/string_processing.h
    inc/test_example_functions.h
)

target_include_directories(search_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(MINGW)
    target_link_libraries(search_server PRIVATE stdc++)
endif()