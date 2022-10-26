// Interger Operations and Threads
// * Integer operatins are always performed as a single processor instruction on the mordern hardware.
//   (Provided that the data fits into a single word. If it is spread across more than one word, that 
//    rule does not apply.)
// * A processor (CPU core) cannot be interrupted while it is performing a instruction.
//   -> If we have a thread that is performing an integer operation, it cannot be interrupted.
// * How a simple computer works:
//   1. Processor has some memory cells on its own, and that is called registers. These are used for 
//      storing data which the processor is working on.
//   2. When we execute a C++ program, the instructions are executed by the processor. 
//   3. The variable are stored in RAM.
//   4. When the processor needs to do some operation on a variable, it fetches the values from RAM into
//      registers (copy the data from RAM into registers). 
//   5. The processor then does its opeation and save (update) the new value of the variable back to RAM.
//   6. Every time a thread is interrupted (temporarily), a task switch occurs. This means that the 
//      values of the registers are backed up on the processor and the values that the new thread needs 
//      for its registers are loaded up.
// * Therefore, if we are using an integer for shared data, we still need to lock it or declare that 
//   integer as "atomic". Since most of the operations contain several separate operations, and the 
//   thread could be interrupted by another thread at any time, and another thead could run and do any 
//   one of the operations. For example, incrementing a variable (++x) is actually contain three separate 
//   operaitons: 
//   (1. Copy the value of the int variable from RAM into the processor's register.
//    2. The processor will increment the value in its register.
//    3. Update the int variable in RAM.)
// * Declare the integer as an atomic variable will make the compiler generate special processor 
//   instructions (codes), which will make the processor perform in a special way, and synchronize the 
//   threads, so the thread will not be interrupted until it has written the new value into RAM, and
//   the new thread will get the new value. -> Using an atomic variable will have a similar result
//   to use a mutex in terms of safety. -> Avoiding data race.
// * Mutex is a library object it will make calls into operating system (os) for the operating system
//   to manage the locks.
#include <thread>
#include <iostream>
#include <vector>

using namespace std::literals;

int counter{0};

void task() {
    for(int i = 0; i < 100'000; ++i){
        ++counter;
    }
}

int main() {

    std::vector<std::thread> tasks;
    for(int i = 0; i < 10; ++i){
        tasks.push_back(std::thread{task});
    }
    for(auto& t: tasks) t.join();
    std::cout << counter << std::endl;

    return 0;
}

// Data Race in Detail
// * Thread A runs
//   1. Copy the value 123 from count (in RAM) into the processor's register.
//   2. Increments the value in the processor's register to 124.
//   3. Thread A is interrupted and its register values are backed up.
// * Thread B runs
//   1. Copy the value 123 from count (in RAM) into the processor's register.
//   2. Increments the value in the processor's register to 124.
//   3. Thread B saves the value 124 in count, then sleeps.
// * Thread A wakes up
//   1. Its register values are restored and it writes the value 124 in count 
//      (in RAM) -> Overwrite the thread B's result. 
// 