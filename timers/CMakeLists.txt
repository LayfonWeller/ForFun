cmake_minimum_required(VERSION 3.16)

project(timers C CXX)

add_library(timers INTERFACE)
target_include_directories(timers INTERFACE .)
target_compile_features(timers INTERFACE cxx_std_20)

add_subdirectory(exemple)

