cmake_minimum_required (VERSION 3.25.1)

project(6502)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(${PROJECT_NAME}
  cpu.hpp
  cpu.cpp
  main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wextra -Wpedantic -Werror -Wall -Wshadow -fno-strict-aliasing -std=c++17 -O0
)


include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
                             ${SDL2_IMAGE_LIBRARY})

