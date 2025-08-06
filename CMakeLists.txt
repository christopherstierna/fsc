cmake_minimum_required(VERSION 4.0.0)

project(
    fsc
    LANGUAGES
    CXX    
)

add_executable(
    fsc
    ${PROJECT_SOURCE_DIR}/source/main.cpp
)

set_target_properties(
    fsc
    PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

# CLANG DEBUG
target_compile_options(
    fsc
    PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:
        -g;
        -O0;
        -Wall; -Wextra; -Wpedantic;
        -Wconversion; -Wshadow; -Wundef; -Werror;
    >
)

# CLANG DEBUG
target_link_options(
    fsc
    PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:
        -fsanitize=address,undefined;
    >
)

# CLANG RELEASE
target_compile_options(
    fsc
    PRIVATE
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:
        -O3;
        -DNDEBUG
    >
)