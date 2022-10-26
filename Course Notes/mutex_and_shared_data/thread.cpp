// 1. The main thread can create additional threads.
// 2. Each thread has its own entry point function.
// 3. When the thread starts executing, its entry point function is called, and when this function returns, the thread ends.
// 4. The main thread does not wait for the other threads unless we explicitly tell it to wait other threads.
// 5. In Cpp, there is a close relationship between the threading object (thread class in the standard library) 
//    and the threads provided by the operating system.
// 6. A thread object manages a kernel thread, it is a resource management class. 
//    -> It cannot be copied, but it can be moved. Just like the file handling class (and object).


/* Thread Creation */
// 1. Use the std::thread class to create a new execution thread.
// 2. An std::thread instance cannot be copied, but can be moved.
// 3. Its constructor takes a callable object as its first argument 
//    (the entry point to the thread -- the code that the thread is going to start executing when it runs.)
//    (Callable object: e.g. funtion pointer, lambda expression, functor with an overloaded operator, 
//     member function pointer, object created by std::bind(), etc.)
// 4. Thread function (the first argument we passed in) must not be overloaded.
//    Thread function can take arguments by value, reference or by move.
//    The return value from the passed thread function will be ignored.
// 5. The std::thread object starts running (executing the code in the thread function) as soon as it is created.
// 6. Call .join() member function on the thread (e.g. t.join()) to make the parent (e.g. main 
//    thread.) stop and wait for the thread until the the thread has terminated.
//    If we don't do that, the parent will go on executing and finally terminate with the program, but the 
//    thread is still running.
// 
#include <iostream>
#include <thread>
#include <string>

// Callable object thread function - thread entry point.
void hello(){
    std::cout << "Hello Thread!" << std::endl;
}

void hello2(const std::string& str){
    std::cout << str << std::endl;
}

// params passed by reference
void hello3(std::string& str){
    str = "xyz";
    std::cout << str << std::endl;
}

// params passed by move
void hello4(std::string&& str){
    std::cout << str << std::endl;
    str = "xyz";
    std::cout << str << std::endl;
}

// This is call by value.
void call_by_value(int x, int y){
    std::cout << x + y << std::endl;
}

// This is call by reference.
void call_by_reference(int& x, int& y){
    int tmp = x;
    x = y;
    y = tmp;
}

// This is call by pointer.
void call_by_pointer(int* x, int* y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

// Callable object thread function - class member function.
class greeter {
public:
    void hello() {
        std::cout << "Hello Member Function ..." << std::endl;
    }
};

int main() {

    std::thread t1{ hello };  // function pointer to the above hello funcion (callable object).
    std::cout << "Doing some work ..." << std::endl;  // The output of this line is mixed up (randomizedly interleaved) with the thread object (t) output.
                                                      // The thread that we start up (main thread) and the thread (t1) that we created will run at the same time.
    t1.join();

    std::thread t2{ hello2, "Hello thread!" };  // The remaining arguments (after the first argument) to the thread constructor will get forwarded to that function.
                                                // We can use copy, reference, and move to manipulate the arguments.
    t2.join();

    std::string s {"abc"};
    std::cout << s << std::endl;
    std::thread t3{ hello3, std::ref(s) };  // In std::thread(), parameters are passed (called) by value by default.
                                            // If we want parameters be passed (called) by reference. std::ref() and std::cref() should be used instead.
                                            // std::cref() is used when const reference parameter is used in the thread function.
                                            // std::ref() will cast its parameter into an lvalue reference.
    t3.join();

    std::string ss {"def"};
    std::thread t4{ hello4, std::move(ss) };
    t4.join();
    std::cout << "ss now is" << ss << std::endl;  // ss is empty, the data ("def") has been moved out of ss, 
                                                  // and gets transferred to the local string instance str inside hello4().
                                                  // Objects that are moved should not be read. We can assign a new value to the objects.
                                                  // Here is just for demo.

    int xx = 3;
    int yy = 4;
    call_by_value(xx, yy);
    std::cout << xx << " " << yy << std::endl;
    call_by_reference(xx, yy);  // int& x = xx,  int& y = yy;
    std::cout << xx << " " << yy << std::endl;
    call_by_pointer(&xx, &yy);  // int* x = &xx, int* y = &yy;
    std::cout << xx << " " << yy << std::endl;

    greeter greet;
    std::thread t5{ &greeter::hello, &greet }; // member function pointer
    t5.join();



    return 0;
}
