cmake_minimum_required(VERSION 3.11)

# Your project name
project(ThisIsTheEnd)

# Modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Output Directories ####
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add SFML from your lib folder ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")

#### Build the game ####
# If your .cpp files are inside src/, use this:
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ThisIsTheEnd ${SOURCES})

target_include_directories(ThisIsTheEnd PRIVATE ${SFML_INCS})

# Link SFML (graphics + window + system required)
target_link_libraries(ThisIsTheEnd 
    sfml-graphics
    sfml-window
    sfml-system
)
