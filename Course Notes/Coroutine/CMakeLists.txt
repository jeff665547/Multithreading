cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe Coroutine_basic)

add_executable(Coroutine_basic Coroutine_basic.cpp)

foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_20
    )
endforeach()