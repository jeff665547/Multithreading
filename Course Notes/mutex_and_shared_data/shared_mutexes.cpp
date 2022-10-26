// shared_mutex
// * Usage: Multiple reader threads, but single writer thread at a time.
// * Let many threads to access a resource concurrently, and provided that only one can modify it at a time.
// * This is not supported in C++11, C++14 provide a shared_timed_mutex and a shared_lock, and C++17 provide shared_mutex.
// * It can be locked in two different ways:
//   - Exclusive lock -- No other thread can lock the mutex until this thread releases the lock.
//                       No other thread can acquire an shared lock until this thread releases the lock.
//     * lock_guard<shared_mutex>, unique_lock<shared_mutex>, scoped_lock<shared_mutex>
//   - Shared lock -- Any other thread can acquire the shared lock without waiting this thread release the lock. 
//                    No other thread can acquire an exclusive lock until all the threads with shared locks on the mutex released them.
//     * shared_lock<shared_mutex>
#include <iostream>
#include <vector>
#include <thread>
#include <shared_mutex>
#include <chrono>

using namespace std::literals;
std::shared_mutex the_mutex;

void write(int i){
    std::lock_guard<std::shared_mutex> lg(the_mutex);  // Write thread has exclusive lock.
    std::cout << "Write thread " << i << " with the exclusive lock." << std::endl;
    std::this_thread::sleep_for(15ms);
}

void read(int i){
    std::shared_lock<std::shared_mutex> sl(the_mutex);  // Read thread has shared lock.
    std::cout << "Read thread " << i << " with the shared lock" << std::endl;
}

int main(){
    std::vector<std::thread> threads;

    for(int i = 0; i < 10; i++){
        threads.push_back(std::thread { read, i });  // the value is moved to the element of the threads vector.
    }
    // threads.push_back(std::thread { write, 10 });
    for(int i = 10; i < 20; i++){
        threads.push_back(std::thread { write, i});
    }
    for(int i = 20; i < 30; i++){
        threads.push_back(std::thread { read, i });
    }
    // threads.push_back(std::thread { write, 21 });
    for(int i = 30; i < 40; i++){
        threads.push_back(std::thread { write, i});
    }

    for(auto& t: threads){  // Use the reference here since the thread object cannot be copied.
        t.join();
    }

    /* Explanation */
    // 1. The writer thread cannot get the exclusive lock unless all the reader threads (0~9) accessing the critical section release
    //    their shared locks.
    // 2. These reader threads' (0~9) accesses "happen before" the writer thread's class.
    // 3. Subsequent reader threads (11~20) cannot get a shared lock until the writer thread finishes accessing the critical section and 
    //    releases its exclusive lock.
    // 4. The writer thread's (10) access "happens before" these other reader threads (11~20).
    // 5. There is no case in which reader (0~9, 11~20) and writer threads (10) have interleaved access (executed the critical section 
    //    at the same time).
    // 6. The accessing among multiple reading threads do not have the "happen before" ordering, but that does not matter because they 
    //    don't modify the data, so they don't conflict. -> No data race.

    return 0;
}