cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe Quick_sort execution_policy)

add_executable(Quick_sort quick_sort.cpp)
add_executable(execution_policy execution_policy.cpp)

foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_20
    )
endforeach()