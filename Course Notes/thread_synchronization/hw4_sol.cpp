#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace std::literals;

// Global variables
mutex data_mutex;
mutex completed_mutex;
condition_variable cv;

string sdata {"Empty"};
bool update_progress{ false };
bool data_to_process{ false };
bool completed{ false };

// Waiting thread
void progress_bar() {
	size_t len{0};
	
	while (true) {
		std::this_thread::sleep_for(200ms);
		
		unique_lock<std::mutex> guard(data_mutex);          // Acquire lock
		cout << "Displaying progress bar..." << endl;
		cv.wait(guard, [] { return update_progress; });     // Wait for condition variable to be notified
		
		len += sdata.size();                                 // Wake up and use the new value

		cout << "Received " << len << " bytes so far" << endl;
		update_progress = false;
		guard.unlock();
		
		lock_guard<std::mutex> lg(completed_mutex);
		if (completed) {
			cout << "Progress bar has ended" << endl;
			break;
		}
	}
}

void process_data() {
	while (true) {
		std::this_thread::sleep_for(200ms);
	
		unique_lock<std::mutex> guard(data_mutex);          // Acquire lock
		cout << "Waiting for sdata..." << endl;
		cv.wait(guard, [] { return data_to_process; });     // Wait for condition variable to be notified
		
		cout << "Processing sdata: " << sdata << endl;          // Wake up and use the new value
		data_to_process = false;
		guard.unlock();
		
		lock_guard<std::mutex> lg(completed_mutex);
		if (completed) {
			cout << "sdata processing has ended" << endl;
			break;
		}
	}
}

// Modyifing thread
void fetch_data() {
	for (int i = 0; i < 5; ++i) {
		unique_lock<std::mutex> ul(data_mutex);              // Acquire lock
		
		std::this_thread::sleep_for(1s);                    // Pretend to be busy...
		sdata = "Block " + to_string(i+1);                   // Modify the sdata
		cout << "Fetched sdata: " << sdata << endl;
		update_progress = true;
		data_to_process = true;
		ul.unlock();
		cv.notify_all();                                    // Notify the condition variable
    }
	cout << "Fetch sdata has ended\n";

	lock_guard<std::mutex> lg(completed_mutex);
	completed = true;
}

int main() {
	thread fetcher{ fetch_data };
	std::this_thread::sleep_for(2s);
	thread prog{ progress_bar };
	thread processor{ process_data };
	
	fetcher.join();
	prog.join();
	processor.join();
}