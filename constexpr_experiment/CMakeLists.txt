cmake_minimum_required(VERSION 3.16)

# include(CXXModules.cmake)

project(constexpr_experiment LANGUAGES CXX)

add_executable(constexpr_experiment)
target_compile_features(constexpr_experiment PUBLIC cxx_std_20)
target_sources(constexpr_experiment
PRIVATE fibonacci_sequence_array.cpp)