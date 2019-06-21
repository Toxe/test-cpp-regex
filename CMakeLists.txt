cmake_minimum_required(VERSION 3.14)
project("Regular Expressions" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("$ENV{SUITE_BUILD}/include")

set(LibPCRE2_LIBRARY "$ENV{SUITE_BUILD}/lib/libpcre2-8.dylib")

add_executable(std std.cpp)
add_executable(pcre2 pcre2.cpp)

target_compile_features(std PUBLIC cxx_std_17)
target_compile_features(pcre2 PUBLIC cxx_std_17)

target_link_libraries(pcre2 PRIVATE "${LibPCRE2_LIBRARY}")

set_target_properties(std PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(pcre2 PROPERTIES CXX_EXTENSIONS OFF)
