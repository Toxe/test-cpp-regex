cmake_minimum_required(VERSION 3.14)
project("Regular Expressions" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Boost REQUIRED COMPONENTS regex)
find_package(re2 CONFIG REQUIRED)

find_library(Hyperscan_LIBRARY hs)
find_library(PCRE_LIBRARY pcre)
find_library(PCRE2_LIBRARY pcre2-8)

find_path(Hyperscan_INCLUDE_DIRS hs/hs.h)
find_path(PCRE_INCLUDE_DIRS pcre.h)
find_path(PCRE2_INCLUDE_DIRS pcre2.h)

set(ALL_TARGETS
    std
    boost
    pcre
    re2
    hyperscan
    pcre2
    pcre_jit
    pcre2_jit
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    target_compile_features(${target} PUBLIC cxx_std_17)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wmost -pedantic -Wconversion -Wfloat-equal -Wold-style-cast -fcolor-diagnostics)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif()
endforeach()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(boost PRIVATE -Wno-conversion -Wno-old-style-cast)
    target_compile_options(pcre2 PRIVATE -Wno-old-style-cast)
    target_compile_options(pcre2_jit PRIVATE -Wno-old-style-cast)
endif()

target_include_directories(boost PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(hyperscan PRIVATE ${Hyperscan_INCLUDE_DIRS})
target_include_directories(pcre PRIVATE ${PCRE_INCLUDE_DIRS})
target_include_directories(pcre2 PRIVATE ${PCRE2_INCLUDE_DIRS})
target_include_directories(pcre_jit PRIVATE ${PCRE_INCLUDE_DIRS})
target_include_directories(pcre2_jit PRIVATE ${PCRE2_INCLUDE_DIRS})

target_link_libraries(boost PRIVATE ${Boost_LIBRARIES})
target_link_libraries(re2 PRIVATE re2::re2)
target_link_libraries(hyperscan PRIVATE "${Hyperscan_LIBRARY}")
target_link_libraries(pcre PRIVATE "${PCRE_LIBRARY}")
target_link_libraries(pcre2 PRIVATE "${PCRE2_LIBRARY}")
target_link_libraries(pcre_jit PRIVATE "${PCRE_LIBRARY}")
target_link_libraries(pcre2_jit PRIVATE "${PCRE2_LIBRARY}")
