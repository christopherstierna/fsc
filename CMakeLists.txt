cmake_minimum_required(VERSION 4.0.0)

option(ASAN_ENABLED "Enable AddressSanitizer" OFF)

project(
    fsc
    LANGUAGES
    CXX    
)

add_executable(
    fsc
    ${PROJECT_SOURCE_DIR}/source/main.cpp
    ${PROJECT_SOURCE_DIR}/source/command_list.cpp
    ${PROJECT_SOURCE_DIR}/source/commands.cpp
    ${PROJECT_SOURCE_DIR}/source/command_functions.cpp
    ${PROJECT_SOURCE_DIR}/source/argument_parser.cpp
    ${PROJECT_SOURCE_DIR}/source/utilities.cpp
)

set_target_properties(
    fsc
    PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    fsc
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Release
target_compile_options(
    fsc
    PRIVATE

    # GCC/Clang
    $<$<AND:$<CONFIG:Release>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -O3 -flto -DNDEBUG
    >

    # MSVC
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:
        /O2 /Ob2 /GL /W4 /DNDEBUG
    >
)

# Debug
target_compile_options(
    fsc
    PRIVATE

    # GCC/Clang
    $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -g -O0 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wundef -Werror -fno-omit-frame-pointer
    >

    # MSVC
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:
        /Zi /Od /W4 /RTC1
    >
)

if (ASAN_ENABLED)
    target_link_options(
        fsc
        PRIVATE

        # GCC/Clang
        $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
            -fsanitize=address,undefined
        >

        # MSVC
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:
            /fsanitize=address
        >
    )
endif()