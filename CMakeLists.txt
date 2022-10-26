cmake_minimum_required(VERSION 3.23)
project(multithreading)

find_package(Threads REQUIRED)
# find_package(TBB REQUIRED)

add_executable(Coroutine_Custom_Generator Coroutine_Custom_Generator.cpp)
add_executable(Coroutine_Custom_Thread_Synchronization Coroutine_Custom_Thread_Synchronization.cpp)
add_executable(Jthread_Construction Jthread_Construction.cpp)
add_executable(Jthread_Cpp20 Jthread_Cpp20.cpp)
add_executable(Execution_Policy Execution_Policy.cpp)
add_executable(Vector_Map_Reduce_with_Tasks Vector_Map_Reduce_with_Tasks.cpp)
add_executable(Vector_Map_Reduce Vector_Map_Reduce.cpp)

set(cpp20_exe Coroutine_Custom_Generator Coroutine_Custom_Thread_Synchronization 
    Jthread_Construction Jthread_Cpp20 Execution_Policy 
    Vector_Map_Reduce_with_Tasks Vector_Map_Reduce)
foreach(target IN LISTS cpp20_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_20
    )
    target_compile_options(
        ${target} 
        PRIVATE 
        -fcoroutines
        -pthread
    )
    target_link_libraries(
        ${target} 
        PRIVATE 
        Threads::Threads
        # TBB::tbb        // Use -ltbb in compiler explorer
    )
endforeach()

add_executable(Future_and_Promise Future_and_Promise.cpp)
add_executable(Quick_Sort_with_Simple_Thread_Pool Quick_Sort_with_Simple_Thread_Pool.cpp)
add_executable(Shared_Mutex_and_Shared_lock Shared_Mutex_and_Shared_lock.cpp)
add_executable(Simple_Thread_Pool Simple_Thread_Pool.cpp)
add_executable(Singleton_Lazy_Initialization Singleton_Lazy_Initialization.cpp)
add_executable(Threads_Management Threads_Management.cpp)

set(exe Future_and_Promise Quick_Sort_with_Simple_Thread_Pool 
    Shared_Mutex_and_Shared_lock Simple_Thread_Pool Singleton_Lazy_Initialization
    Threads_Management)
foreach(target IN LISTS exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_17
    )
    target_link_libraries(
        ${target} 
        PRIVATE 
        Threads::Threads
    )
endforeach()