// Condition variables with predicate
// * Race condition: A race condition occurs when the timing or order of 
//                   events affects the correctness of a piece of code.
// * Spurious (Fake) wakeup: The waiting thread will be woken up by the notification sent
//                           from the condition variable even though the modifying thread
//                           has not notified the condition variable.
// * Lost wakeup: A notification from the modifying thread notifies the condition 
//                variable before the waiting thread calls wait(). 
//                -> wait() in the waiting thread cannot receive the notification after 
//                   being called.
//                -> race condition.
// * Solution: Condition variable with predicate -- Always add a predicate function (a 
//             callable argument which checks a shared bool) to the second argument 
//             of wait().
//             - This also applies to the wait_for() and wait_until().
//             - The shared boolean is first initialized to false, and be set to true when
//               the writing thread modifies the data.
//             - The waiting thread will only resume execution if the predicate returns 
//               true, or it will release the lock and keep waiting the notification again.
// * Mechanism: For the lost wakeup, the wait() function will call the predicate internally
//              when the wait() function is called by the waiting thread. At that point,
//              the mutex (mut in the following example) is still locked. If the predicate
//              returns true (means the modifying thread has already set the condition to 
//              true -> the modifying must have already sent a notification), the waiting 
//              thread will (resume) continue executing the subsequent critical section 
//              (contains the shared data) with the lock (guard in the following example) 
//              on the mutex. Contrarily, if the predicate returns false (means the modifying 
//              thread has not yet sent a notification), the waiting thread will release the 
//              lock and keep waiting the notification sent by the condition variable 
//              triggered from the modifying thread. 
//              For the spurious wakeup, after the waiting thread is notified by the condition 
//              variable, the waiting thread will first acquire a lock on the mutex. Then,
//              the waiting thread will check the predicate. If the predicate returns true 
//              (means the modifying thread sent a notification since it set the shared boolean),
//              the waiting thead will resume executing the subsequent code with the lock on the 
//              mutex. Contrarily, if the predicate returns false, which means the shared
//              boolean is still the initial value (false), i.e. false wakeup, the waiting thread 
//              will then release the lock on the mutex, and keep waiting the next notification 
//              sent by the condition variable.
// * notify_all() and notify_one() -- particalarly useful when multiple threads are waiting for 
//   the same event.
//   - notify_all():
//     # This will cause all threads which have called wait() on the condtion variable to be 
//       woken up.
//     # The waiting thread could be woken up in any scheduled order.
//     # For more info, please check https://stackoverflow.com/questions/70442963/does-notifying-a-condition-variable-guarantee-the-wake-up-of-a-thread-with-a-suc
//   - notify_one():
//     # Only one of waiting threads (chosen by the OS) which have called wait() will be waken up.
//     # The other waiting threads will remain blocked (no notification) until they eventually get 
//       their wake-up call.
//     # Each time the notify_one() calls, a different waiting thread will be waken up and processes 
//       the shared data. (Waiting thread who have once been notified will not be notified again 
//       by the same condition variable.))
// * Example:
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std::literals;
std::mutex mut;
std::string sdata {"Empty"};
bool modified = {false};                              // (Add a shared boolean to help avoid the lost 
                                                      //  wakeup and the spurious wakeup.)
std::condition_variable cv;

// Waiting thread
void reader(){
    std::cout << "Thread " << std::this_thread::get_id() << " calling wait" << std::endl;
    std::unique_lock<std::mutex> guard(mut);          // Acquire lock.
    cv.wait(guard, [] { return modified; });          // Wait for the condition variable to be notified. 
                                                      // (Add a lambda predicate to help avoid the lost 
                                                      //  wakeup and the spurious wakeup. The "modified" 
                                                      //  variable is protected by the "guard" lock 
                                                      //  while being accessed in the predicate, and 
                                                      //  that prevents the data race.)
    std::cout << "Thread " << std::this_thread::get_id() << " data is " << sdata << std::endl;    // Wake up and use the new value.
}

// Modifying thread
void writer(){
    std::cout << "Writing data ..." << std::endl;
    {
        std::lock_guard<std::mutex> lg(mut);    // Acquire lock.
        std::this_thread::sleep_for(1s);        // Pretend to be busy.
        sdata = "Populated";                    // Modify the data...
        modified = true;
    }
    cv.notify_one();                            // Notify the condition variable.
}

void writer3(){
    std::cout << "Writing data ..." << std::endl;
    {
        std::lock_guard<std::mutex> lg(mut);    // Acquire lock.
        std::this_thread::sleep_for(1s);        // Pretend to be busy.
        sdata = "Populated";                    // Modify the data...
        modified = true;
    }
    cv.notify_one();                            // Notify the condition variable.
    cv.notify_one();                            // Once for each thread.
    cv.notify_one();
    // or use cv.notify_all();                  // Notify for all threads at the 
                                                // same time.
}

int main(){
    // To demo the "Lost wakeup" and the "Spurious wakeup" problem.
    std::cout << "Data is " << sdata << std::endl;
    std::thread write{writer};
    std::this_thread::sleep_for(2s);
    std::thread read{reader};
    write.join();
    read.join();

    // To demo the effect of the notify_one().
    std::cout << std::endl;
    sdata = "Empty";
    modified = false;
    std::cout << "Data is " << sdata << std::endl;
    std::thread read1{reader};
    std::this_thread::sleep_for(10ms);
    std::thread read2{reader};
    std::this_thread::sleep_for(10ms);
    std::thread read3{reader};
    std::this_thread::sleep_for(10ms);
    std::thread write3{writer3};
    read1.join();
    read2.join();
    read3.join();
    write3.join();

    return 0;
}