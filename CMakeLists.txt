cmake_minimum_required(VERSION 3.20)
project(winter)

set(WARNING_FLAGS "-W -Wall -Wextra -Wno-comment")
set(COMPILE_FLAGS "-fPIC -march=native -mtune=native -fvisibility=hidden -flto" )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS_DEBUG "${WARNING_FLAGS} ${COMPILE_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++20")

set(CMAKE_C_FLAGS_RELEASE "${WARNING_FLAGS} ${COMPILE_FLAGS} -s -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pipe -frename-registers -fomit-frame-pointer --no-gnu-unique -fno-rtti -std=c++20")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/imgui/GL/gl3w.c")

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_subdirectory("deps/hopscotch-map")
find_package(fmt)

add_library(winter SHARED ${SOURCES})
target_link_libraries(winter PRIVATE dl pthread SDL2 GL fmt::fmt-header-only hopscotch_map)
