cmake_minimum_required(VERSION 3.14)
project("Regular Expressions" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(std std.cpp)

target_compile_features(std PUBLIC cxx_std_17)

set_target_properties(std PROPERTIES CXX_EXTENSIONS OFF)
