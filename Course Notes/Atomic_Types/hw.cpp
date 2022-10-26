#include <mutex>
#include <atomic>
#include <thread>
#include <vector>

class some_type {
	// ...
public:
	void do_it() { }
};

std::atomic<some_type*> ptr{nullptr};            // Variable to be lazily initialized
std::mutex process_mutex;

void process() {
    if (!ptr) {                     // First check of ptr
        std::lock_guard<std::mutex> lk(process_mutex);
        
        if (!ptr)                  // Second check of ptr
            ptr = new some_type;   // Initialize ptr
    }
	ptr.load()->do_it();
    // (*ptr).do_it();
}

int main() {
	std::vector<std::thread> threads;
	
	for (int i = 0; i < 10; ++i)
		threads.push_back(std::thread{process});
	for (auto&t : threads)
		t.join();
}