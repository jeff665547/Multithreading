// Atomic types
// * If we make a variable atomic, that will have the same effect as using mutex locks
//   around accesses to that variable. (Make the operations of a variable a single, 
//   an uninterruptible operation. Only loading and storing the variable is supported.)
//   (mutex -> critical section (region), atomics -> variable (point))
// * The parameter type for the templated atomic variable must be "trivially copyable".
//   i.e. The copy and move constructors, assignments, and destructor are using the 
//   implicitly defined one or been deleted. Moreover, it has no virtual members. 
//   e.g. Some built-in type, like int, bool, pointers (e.g. shared_ptr for more complex types).
// * If we use complex types as the template type parameter directly, locks may be added.
//   -> Defeats the point of trying to avoid locks.
// * Atomic objects must be initialized and changed value in the following operations.
// 
// * The Volatile keyword
// * This is used to tell the program to read the value of the variable each time it needs.
//   -> It prevents the compiler optimizing that variable (the optimization will assume the 
//   values does not change).
// * The value of the variable may be changed (e.g. by some hardware operations) without 
//   being explicitly specified.
// * It is typically used when accessing hardware.
// * In C++, it cannot replace the function of atomic, since it cannot protect the 
//   variable-updating procedure. It just ensure the variable-reading process.
// * Example:
//   int some_int = 100;
//   while(some_int == 100){
//      // do somethings ...
//   }
//   When this program gets compiled, if it finds that the program never ever makes 
//   any attempt to change the value of some_int, it may be tempted to optimize the 
//   while loop by changing it from while(some_int == 100) to something which is 
//   equivalent to while(true) so that the execution could be fast (since the condition 
//   in while loop appears to be true always). (if the compiler doesn't optimize it, 
//   then it has to fetch the value of some_int and compare it with 100, in each 
//   iteration which obviously is a little bit slow.) However, sometimes, optimization 
//   (of some parts of your program) may be undesirable, because it may be that someone 
//   else is changing the value of some_int from outside the program which compiler is 
//   not aware of, since it can't see it; but it's how you've designed it. In that case, 
//   compiler's optimization would not produce the desired result!
//   So, to ensure the desired result, you need to somehow stop the compiler from 
//   optimizing the while loop. That is where the volatile keyword plays its role.
//   int some_int = 100;   ->   volatile int some_int = 100;
//   -> Fetch the value of some_int and compare it with 100 in each iteration.
#include <thread>
#include <iostream>
#include <vector>
#include <atomic>

using namespace std::literals;

std::atomic<int> counter{0};

void task() {
    for(int i = 0; i < 100'000; ++i){
        ++counter;
    }
}

int main(){

    // Just demo
    std::atomic<int> x{0};  // Atomic int, initialized to x.
    int y;
    x = 2;                  // Atomic assignment to x.
    y = x;                  // Atomic assignment from x. y can be non-atomic.
    // Each of the above two operations is atomic in itself, but the two together are not.
    // It is quite possible another thread could run between the above two statements and 
    // modify the value of x.
    std::cout << x << std::endl;
    std::cout << y << std::endl;


    // Example
    std::vector<std::thread> tasks;
    for(int i = 0; i < 10; ++i){
        tasks.push_back(std::thread{task});
    }
    for(auto& t: tasks) t.join();
    std::cout << counter << std::endl;

    return 0;
}