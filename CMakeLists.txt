cmake_minimum_required(VERSION 3.14)
project("Regular Expressions" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("$ENV{SUITE_BUILD}/include")

set(LibPCRE_LIBRARY "$ENV{SUITE_BUILD}/lib/libpcre.dylib")
set(LibPCRE2_LIBRARY "$ENV{SUITE_BUILD}/lib/libpcre2-8.dylib")

add_executable(std std.cpp)
add_executable(pcre pcre.cpp)
add_executable(pcre2 pcre2.cpp)
add_executable(pcre_jit pcre_jit.cpp)
add_executable(pcre2_jit pcre2_jit.cpp)

target_compile_features(std PUBLIC cxx_std_17)
target_compile_features(pcre PUBLIC cxx_std_17)
target_compile_features(pcre2 PUBLIC cxx_std_17)
target_compile_features(pcre_jit PUBLIC cxx_std_17)
target_compile_features(pcre2_jit PUBLIC cxx_std_17)

target_link_libraries(pcre PRIVATE "${LibPCRE_LIBRARY}")
target_link_libraries(pcre2 PRIVATE "${LibPCRE2_LIBRARY}")
target_link_libraries(pcre_jit PRIVATE "${LibPCRE_LIBRARY}")
target_link_libraries(pcre2_jit PRIVATE "${LibPCRE2_LIBRARY}")

set_target_properties(std PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pcre PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pcre2 PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pcre_jit PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pcre2_jit PROPERTIES CXX_EXTENSIONS OFF)
