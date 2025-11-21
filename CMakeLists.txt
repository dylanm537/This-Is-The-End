project(setup)
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")

include_directories(${CMAKE_SOURCE_DIR})

add_executable(thisistheend main.cpp game_system.cpp)

target_include_directories(thisistheend PRIVATE ${SFML_INCS})
target_link_libraries(thisistheend sfml-graphics)