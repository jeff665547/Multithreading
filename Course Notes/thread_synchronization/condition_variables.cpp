// Condition Variables
// * Condition Variables is a role of supervisor (help us synchronize different threads).
// * Background: Processing thread need the resource generated from another thread.
//   1. Processing thread sends a "wait" message to the condition variable and then 
//      go to sleep.
//   2. After the thread that generated the resource that the processing thread depends, 
//      It will send a "notify" message to the condition variable.
//   3. The condition variable will then send a "wake up" message to the processing thread.
// * std::condition_variable
//   - wait() takes an argument of type std::unique_lock<std::mutex> to unlocks its
//     argument and blocks the thread until a notification is received. 
//   - Timeout version: wait_for() and wait_until() - They will re-lock their argument
//     if a notification is not received in time. -> can be run in a while-loop.
//   - notify_one() sends a notification to a single waiting thread (chosen by the system).
//   - notify_all() sends a notification to all waiting threads.
// 
// * Analogy: Worker B cannot start doing their job until worker A has finished.
//   - Solution: Worker A signals to the foreman or supervisor when he has finished, 
//               and worker B receives the signal from the foreman or supervisor 
//               to start the work.
// * Example 1: 1. A program with a thread which fetches data over the network.
//              2. Another thread shows the progress bar for fetching data.
//              3. Another thread processes the data fetched from the network.
//              -> 1. The data-fetching thread terminates.
//              -> 2. The progress bar thread terminates.
//              -> 3. The data processing thread runs.
//              -> Traditionally, we can use a shared boolean variable, protected by a 
//                 mutex, to indicate whether the data-fetching thread finishes its job, 
//                 and use while-loops in the prgress bar thread and the data processing 
//                 thread to monitor the value of that shared boolean variable to finish
//                 what they should do.
//              -> This is not very efficient.
//                 a. The loops in the waiting threads will consume a lot of CPU 
//                    processing power.
//                 b. When the other threads are checking the flag, the fetching thread  
//                    is blocked from setting it.
//              -> Condition variables will be a better solution.
// * Details: 
//   1. The processing thread creates an unique_lock instance to lock the mutex. 
//      (critical section is locked.)
//      - It calls wait() on the condition vairable.
//      - The condition variable releases the lock and block this thread.
//   2. The fetching thread create a lock_guard instance to acquire a lock.
//      - It fetches the data.
//      - Releases the lock.
//      - Notifies the condition variable.
//   3. When the condition variable is notified by the fetching thread, the processing 
//      thread is woken up.
//      - The processing thead acquires a lock (to lock the critical section), resumes 
//        execution and processes the data.
// 
// * Example 2: Two threads: "Writer" and "Reader".
//   - The writer thread will modify a shared string.
//   - The reader thread will stop and wait for the writer to modify the string.
//   - Once the string has been modified, the reader thread will start up again and use
//     the new value.
//   - Use a condition variable to coordinate the two threads.
//   - Use a mutex to protect the shared data.

#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>

using namespace std::literals;

std::mutex mut;
std::condition_variable cv;
std::string sdata {"Empty"};

// Waiting thread
void reader(){
    std::unique_lock<std::mutex> guard(mut);
    cv.wait(guard);                                       // Unlock the mutex and let the 
                                                          // thread to sleep and wait for 
                                                          // the condition variable to be
                                                          // notified.
    std::cout << "The data is " << sdata << std::endl;    // The thread wake up and use 
                                                          // the new value.
}

// Modifying thread
void writer(){
    std::cout << "Writing data ..." << std::endl;
    {
        std::lock_guard<std::mutex> lg(mut);              // Acquire lock.
        std::this_thread::sleep_for(2s);                  // Pretend to be busy.
        sdata = "Populated";                              // Modify the data.
    }
    cv.notify_one();                                      // Notify the condition variable.
}

int main(){
    std::cout << "The data is " << sdata << std::endl;
    std::thread write{writer};
    std::thread read{reader};
    write.join();  read.join();
    return 0;
}