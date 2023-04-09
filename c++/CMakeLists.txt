cmake_minimum_required(VERSION 3.10)

# Set the project name
project(simulation)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY)
# Find SFML
#set(SFML_DIR "<path-to-your-sfml-install>/lib/cmake/SFML") # Change this to the correct path on your system
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add source files
set(SOURCES
    main.cpp
    ant.cpp
    arena.cpp
    food_source.cpp
)

# Add executable and link libraries
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

