cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe mutex_intro lock_guard unique_lock timeouts_and_mutexes shared_mutexes 
shared_data_init thread_local_variables lazy_initialization deadlock livelock mutex_conclusion)

add_executable(mutex_intro mutex_introduction.cpp)
add_executable(lock_guard lock_guard.cpp)
add_executable(unique_lock unique_lock.cpp)
add_executable(timeouts_and_mutexes timeouts_and_mutexes.cpp)
add_executable(shared_mutexes shared_mutexes.cpp)
add_executable(shared_data_init shared_data_initialization.cpp)
add_executable(thread_local_variables thread_local_variables.cpp)
add_executable(lazy_initialization lazy_initialization.cpp)
add_executable(deadlock deadlock.cpp)
add_executable(livelock livelock.cpp)
add_executable(mutex_conclusion mutex_conclusion.cpp)

foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_17
    )
endforeach()