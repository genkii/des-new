cmake_minimum_required(VERSION 3.25)

project(des-new
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

# ─────────────────────────────────────────────
# Raylib
# ─────────────────────────────────────────────

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

FetchContent_MakeAvailable(raylib)

# ─────────────────────────────────────────────
# tmxlite
# ─────────────────────────────────────────────

FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG master
)

FetchContent_MakeAvailable(tmxlite)

# ─────────────────────────────────────────────
# Asio
# ─────────────────────────────────────────────

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-38-2
)

FetchContent_MakeAvailable(asio)

# ─────────────────────────────────────────────
# MessagePack
# ─────────────────────────────────────────────

FetchContent_Declare(
    msgpack
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
    GIT_TAG cpp-8.0.0
)

set(MSGPACK_USE_BOOST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(msgpack)

# ─────────────────────────────────────────────
# tmxlite sources
# ─────────────────────────────────────────────

file(GLOB TMXLITE_SOURCES
    ${tmxlite_SOURCE_DIR}/tmxlite/src/*.cpp
)

set(TMXLITE_SOURCES
    ${TMXLITE_SOURCES}
    ${tmxlite_SOURCE_DIR}/tmxlite/src/detail/pugixml.cpp
    ${tmxlite_SOURCE_DIR}/tmxlite/src/miniz.c
)

set_source_files_properties(
    ${tmxlite_SOURCE_DIR}/tmxlite/src/miniz.c
    PROPERTIES LANGUAGE CXX
)

# ─────────────────────────────────────────────
# Executable
# ─────────────────────────────────────────────

add_executable(des-new
    src/main.cpp
    ${TMXLITE_SOURCES}
)

# ─────────────────────────────────────────────
# Include directories
# ─────────────────────────────────────────────

target_include_directories(des-new PRIVATE
    ${tmxlite_SOURCE_DIR}/tmxlite/include
    ${asio_SOURCE_DIR}/asio/include
    ${msgpack_SOURCE_DIR}/include
)

# ─────────────────────────────────────────────
# Compiler options
# ─────────────────────────────────────────────

target_compile_options(des-new PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# ─────────────────────────────────────────────
# Definitions
# ─────────────────────────────────────────────

target_compile_definitions(des-new PRIVATE
    ASIO_STANDALONE
)

# ─────────────────────────────────────────────
# Libraries
# ─────────────────────────────────────────────

target_link_libraries(des-new PRIVATE
    raylib
)
