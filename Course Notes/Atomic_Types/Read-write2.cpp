// Multiple reader, One writer
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std::literals;
template<typename T>
class RWLock{
    T data{0};
    int active_writer{0};
    int waiting_writer{0};
    int active_reader{0};
    int waiting_reader{0};
    std::condition_variable write_cv;
    std::condition_variable read_cv;
    std::condition_variable cv;
    std::mutex m;
public:
    void readlock(T& val){
        std::unique_lock<std::mutex> uk(m);
        ++waiting_reader;
        cv.wait(uk, [&](){return (waiting_writer == 0) && (active_writer == 0);});
        --waiting_reader;
        ++active_reader;
        val = data;
    }
    void readunlock(){
        {
            std::lock_guard<std::mutex> lk(m);
            --active_reader;
        }
        cv.notify_all();
    }
    void writelock(const T& val){
        std::unique_lock<std::mutex> uk(m);
        ++waiting_writer;
        cv.wait(uk, [&](){return ((active_reader == 0) && (active_writer == 0));});
        --waiting_writer;
        ++active_writer; 
        data += val;
    }
    void writeunlock(){
        std::lock_guard<std::mutex> lk(m);
        --active_writer;
        if(waiting_writer > 0){
            cv.notify_all();
        }else{
            cv.notify_all();
        }
    }
};

int res{0};
std::mutex moutput;

void Read(RWLock<int>& rw, int i){
    int tmp{0};
    rw.readlock(tmp);
    {
        std::lock_guard<std::mutex> lk(moutput);
        std::cout << "Thread " << i << ": " << "tmp is " << tmp << std::endl;
        std::this_thread::sleep_for(200ms); 
    }
    rw.readunlock();
};
void Write(RWLock<int>& rw, int i){
    
    rw.writelock(2);
    res += 2;
    std::cout << "Thread " << i << ":" << " is writing ... " << std::endl;
    std::this_thread::sleep_for(50ms);
    rw.writeunlock();
    // int tmp{};
    // rw.readlock(tmp);
    // rw.readunlock();
    
};

int main(){
    
    RWLock<int> rw{};
    std::vector<std::thread> Workers;
    for(int i = 1; i <= 5; i++){
        Workers.push_back(std::thread{
            Read, std::ref(rw), i
        });
    }
    for(int i = 6; i <= 10; i++){
        Workers.push_back(std::thread{
            Write, std::ref(rw), i
        });
    }
    for(int i = 11; i <= 15; i++){
        Workers.push_back(std::thread{
            Read, std::ref(rw), i
        });
    }
    for(int i = 16; i <= 20; i++){
        Workers.push_back(std::thread{
            Write, std::ref(rw), i
        });
    }
    for(int i = 21; i <= 25; i++){
        Workers.push_back(std::thread{
            Read, std::ref(rw), i
        });
    }
     
    for(auto& t: Workers) t.join();
    std::cout << "Finished" << std::endl;
    std::cout << "Global result: " << res << std::endl;  // 20
    rw.readlock(res);
    std::cout << "Compute result: " << res << std::endl;  // 20
    rw.readunlock();
    
    return 0;
}