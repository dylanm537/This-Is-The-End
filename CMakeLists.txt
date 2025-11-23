cmake_minimum_required(VERSION 3.11)
project(thisistheend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")

add_executable(thisistheend
    src/main.cpp
    src/game_system.cpp
    src/Menu.cpp
    src/Player.cpp
    src/Zombie.cpp      
)

target_include_directories(thisistheend PRIVATE include ${SFML_INCS})

target_link_libraries(thisistheend sfml-graphics)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(MSVC)
    add_compile_options(/W4)
endif()

