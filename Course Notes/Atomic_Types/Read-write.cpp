#include <iostream> 
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;
using namespace std::literals;
class RWLock {
    public:
        RWLock()
            : shared()
              , readerQ(), writerQ()
              , active_readers(0), waiting_writers(0), active_writers(0){}

        bool no_one_writing(){
            return active_readers > 0 || active_writers == 0;
        }
        bool no_one_read_and_no_one_write(){
            return active_readers == 0 && active_writers == 0;
        }
        void ReadLock() {
            std::unique_lock<std::mutex> lk(shared);
            readerQ.wait(lk, bind(&RWLock::no_one_writing, this));
            ++active_readers;
            lk.unlock();
        }

        void ReadUnlock() {
            std::unique_lock<std::mutex> lk(shared);
            --active_readers;
            lk.unlock();
            writerQ.notify_one();
        }

        void WriteLock() {
            std::unique_lock<std::mutex> lk(shared);
            ++waiting_writers;
            writerQ.wait(lk, bind(&RWLock::no_one_read_and_no_one_write, this));
            --waiting_writers;
            ++active_writers;
            lk.unlock();
        }

        void WriteUnlock() {
            std::unique_lock<std::mutex> lk(shared);
            --active_writers;
            if(waiting_writers > 0)
                writerQ.notify_one();
            else
                readerQ.notify_all();
            lk.unlock();
        }

    private:
        std::mutex              shared;
        std::condition_variable readerQ;
        std::condition_variable writerQ;
        int                     active_readers;
        int                     waiting_writers;
        int                     active_writers;
}; 
int result = 0;
std::mutex mt;
void func(RWLock &rw, int i){
    if(i % 2 == 0){
        rw.WriteLock();
        std::cout << "Writing..." << std::endl;
        result += 1;
        std::this_thread::sleep_for(50ms);
        rw.WriteUnlock();
        rw.ReadLock();
        std::cout << "Reading..." << std::endl;
        rw.ReadUnlock();
    }else{
        rw.ReadLock();
        std::cout << "Reading..." << std::endl;
        rw.ReadUnlock();
        rw.WriteLock();
        std::cout << "Writing..." << std::endl;
        std::this_thread::sleep_for(25ms);
        result += 2;
        std::this_thread::sleep_for(50ms);
        rw.WriteUnlock();

    }
}
void not_safe(int i){
    if(i%2 == 0){
        // std::lock_guard<std::mutex> lk(mt);
        result += 1;
    }else{
        // std::lock_guard<std::mutex> lk(mt);
        result += 2;
    }
}
int main(){
    RWLock rw;
    std::vector<std::thread> threads;
    for(int i = 0; i < 100; i++){
        threads.push_back(std::thread(func, ref(rw), i));
        // threads.push_back(std::thread(not_safe, i));
    }
    for(int i = 0; i < threads.size() ; i++)
    {
        threads[i].join();
    }
    cout << result << endl;
    return 0;
}