// Shared Data
// 1. Global variable or static variable at the namespace scope.
//    -> Shared by all threads in the program.
// 2. Class member which is declared static.
//    -> Shared by all threads which execute member functions of the class.
// 3. Local variable in a task function which is declared static. (static, but only visible in that function.)
//    -> Shared by all threads which execute that function.
// 4. Singleton class (design pattern). (e.g. A logger class.)
//    - Has only a single global instance.
//    - Copy and move operators (assignment operator and constructor) are deleted.
//      -> Prevents the program from creating new instances.
//    - A global function returns the instance of the class. 
//      -> If the instance does not already exist, it is created and initialized. 
//         Otherwise, the existing instance is returned.
// * A global or static variable is initialized when the program starts up, 
//   and at that point, only one thread is running. main() has not even be called yet -> no threading issues 
//   (threads are launching in main()).
// * (After C++11) For the third case, local variable will be initialized by the first thread that executes 
//   the task function body. Any other thread that reaches the code is blocked until the first thread has 
//   finished initializing the variable. i.e. The initialization "happens before" any access by other threads.
//   -> No data race. However, if the variable is modified subsequently, it will need to protected against a 
//      a data race.

#include <iostream>
#include <vector>
#include <thread>

class singleton {
    /* Explicitly specify these functions never to be called. */
    singleton (const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;
    singleton (singleton&&) = delete;
    singleton& operator=(singleton&&) = delete;
  public:
    singleton() {std::cout << "Initializing singleton." << std::endl;}
};

singleton& get_singleton() {
    static singleton single; 
    // Use the static local variable technique to avoid a data race.
    // The first thread to execute this code will initialize this code, 
    // and this code cannot be interrupted until it has finished the 
    // initialization. When another thread runs, the single variable
    // has already been initialized, and it will not be initialized again.
    // i.e. The first thread initialize the single variable and return it,
    // and the second and other threads return it directly without 
    // initializing the variable again.

    return single;
}

void task(){
    singleton& s = get_singleton();
    std::cout << &s << std::endl;
}

int main(){
    std::vector<std::thread> threads;
    for(int i = 0; i < 10; i++){
        threads.push_back(std::thread{ task });
    }
    for(auto& t: threads){
        t.join();
    }

    return 0;
}