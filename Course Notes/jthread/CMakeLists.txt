cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe jthread_construct jthread)

add_executable(jthread_construct jthread_construct.cpp)
add_executable(jthread jthread.cpp)

foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_20
    )
endforeach()