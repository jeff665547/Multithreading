cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe Integer_operations_and_threads Atomic_types hw hw6 queue_thread Read-write Read-write2 tmp)

add_executable(Integer_operations_and_threads Integer_operations_and_threads.cpp)
add_executable(Atomic_types Atomic_types.cpp)
add_executable(hw hw.cpp)
add_executable(hw6 hw6.cpp)
add_executable(queue_thread queue_thread.cpp)
add_executable(Read-write Read-write.cpp)
add_executable(Read-write2 Read-write2.cpp)
add_executable(tmp tmp.cpp)

foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_20
    )
endforeach()