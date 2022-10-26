cmake_minimum_required(VERSION 3.23)
project(multithreading)
set(all_exe condition_variables condition_variables_with_predicate futures_and_promises promises_with_multiple_waiting_threads)

add_executable(condition_variables condition_variables.cpp)
add_executable(condition_variables_with_predicate condition_variables_with_predicate.cpp)
add_executable(futures_and_promises futures_and_promises.cpp)
add_executable(promises_with_multiple_waiting_threads promises_with_multiple_waiting_threads.cpp)



foreach(target IN LISTS all_exe)
    target_compile_features(
        ${target}
        PRIVATE
        cxx_std_17
    )
endforeach()