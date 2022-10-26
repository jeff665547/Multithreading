#include <iostream>
#include <vector>
#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <cmath>
#include <atomic>
#include <algorithm>
#include <chrono>
#include <execution>

using namespace std::literals;

void quickSort(std::vector<int>& nums, int first, int last) {
	if(first == last) return;
	// The picked element is randomly choosed (better), in this case, it's the first element of the vector in this current run.
	// i is responsible for recording element position whose value is greater than the picked element.
	// j is responsible for recording element position whose value is less than the picked element.
	// Classification (greater or less than)
	int j = first+1;
	for(int i = first+1; i < last; i++){
		if(nums[i] < nums[first]) {
			std::swap(nums[i], nums[j]);
			j++;
		}
	}
	// Put the picked element into the right position of the vector.
	int mid = j-1;
	std::swap(nums[first], nums[mid]);
	// In each run, only the originally first element (the current mid position element) will be in the correct position.
	quickSort(nums, first, mid);
	quickSort(nums, mid+1, last);
}

void quickSort(std::vector<int>& nums) {
	quickSort(nums, 0, nums.size());
}

template<typename T>
class Queue {
    std::deque<T> d_;
    std::mutex m_;
    std::atomic<bool> is_closed_{false};
    std::condition_variable cond_;
public:
    void Enqueue(T value) { 
        std::lock_guard lk(m_);
        d_.push_back(value);
        cond_.notify_one();
    }
    void Close() {
        is_closed_.store(true);
        cond_.notify_all();
    }
    bool WaitAndDequeue(T& value) {
        std::unique_lock lk(m_);
        cond_.wait(lk, [&]() { return !d_.empty() || is_closed_.load(); });
        if (d_.empty()) return false;
        value = d_.front();
        d_.pop_front();
        return true;
    }
};

// from + (from + 1) + ... + to
struct Job {
    int from;
    int to;
};


void quickSortMThread(std::vector<int>& nums, int first, int last, Queue<Job>& jobs, std::atomic<int>& remains) {
	// Concept:
	// * First manually divide the task (quick sort on the input vector) multiple times (level). After 
	//   several level's dividing, send the subtask (quick sort on each section) into to the worker 
	//   queue list to achieve the multithreading.
	// * vector is divided into several sections for multiple threads, one section for each thread.
	// * Quicksort each section in each thread.
	// * Merge each sorted section of each thread into a completely sorted one.
	// * Since the quick sort is a recursion operation, there will be multiple quick sort operations.
	// * Use the worker queue list to let the worker (thread) take the job (quick sort).
    while(true) {
        if(first == last) return;
        int j = first+1;
        for(int i = first+1; i < last; i++){
            if(nums[i] < nums[first]) {
                std::swap(nums[i], nums[j]);
                j++;
            }
        }
        int mid = j-1;
        std::swap(nums[first], nums[mid]);
        
        if(last - first < 100) {                        
            quickSortMThread(nums, first, mid, jobs, remains);               
                                      // Optimization 1: 
                                      // If the task is affordable (small) enough, there is no need 
                                      // to launch a new thread. This will prevent the extra overhead.
                                      // -> More efficiently.
        } else {
            remains += 1;                 // Add a task and increase the indicator.
            jobs.Enqueue({first, mid});   // Assign the task with enqueue
            // quickSortMThread(nums, first, mid);
        }

        first = mid + 1;              // Optimization 2 (this line and the above while-true loop): 
                                      // Continue perform the quick sort on the right section on the 
                                      // current thread. (Since current thread has finished its work, 
                                      // and no further work is requested.)
    }
}

void quickSortMThread(std::vector<int>& nums, Queue<Job>& jobs, std::atomic<int>& remains) {
	quickSortMThread(nums, 0, nums.size(), jobs, remains);
}


int main() {
	std::cout << "Hello there!!" << std::endl;
	
	std::vector<int> v1 = {1, 3, 2, 5, 4};
	std::vector<int> v2 = {3, 1, 2, 4, 5};
    int size = 3000; 
	std::vector<int> v3(size);

	for(int i = size; i > 0; i--){
		v3[size-i] = i;
	}

    size = 1000000;
    std::vector<int> v4(size);
	for(int i = size; i > 0; i--){
        v4[size-i] = i;
	}    
	
    /* Single-threaded Quick Sort */
	quickSort(v1);
	quickSort(v2);

    /* Multiple-threaded Quick Sort */
	int thread_num = std::thread::hardware_concurrency();
    // std::cout << thread_num << std::endl;
	std::vector<std::thread> workers(thread_num);
	Queue<Job> jobs;
	std::atomic<int> remains{1};              // Add a task and increase the indicator.
	jobs.Enqueue({0, (int)v3.size()});
	// quickSortMThread(v3, jobs);
	for(int i = 0; i < thread_num; i++){
		workers[i] = std::thread([&v3, &jobs, &remains](){
			Job job;
			while(jobs.WaitAndDequeue(job)){  // Do the task with dequeue.
				quickSortMThread(v3, job.from, job.to, jobs, remains);
				remains -= 1;                 // Finish the job. Decrease the indicator.
			}
		});
	}
	while(remains != 0){ // -> busy waiting ... -> not efficiently
                         // Waiting for the remains decrease to 0. 
                         // i.e. All the works are finished.
        std::this_thread::yield();            // Optimization 3 (improve for the busy-waiting):
                                              // Force to wait other threads to work 
                                              // (release the resource).
    }
	jobs.Close();

	for(auto& w: workers){
		w.join();
	}

    /* std::sort with C++20 parallelized sorting (compiler or library supported required) */
    auto start = std::chrono::steady_clock::now();
    std::sort(v4.begin(), v4.end());
    auto end = std::chrono::steady_clock::now();
    std::cout << "Default: \t" << (end - start).count() << "ns" << std::endl;

    start = std::chrono::steady_clock::now();
    std::sort(std::execution::seq, v4.begin(), v4.end());
    end = std::chrono::steady_clock::now();
    std::cout << "seq: \t\t" << (end - start).count() << "ns" << std::endl;

    start = std::chrono::steady_clock::now();
    std::sort(std::execution::par, v4.begin(), v4.end());
    end = std::chrono::steady_clock::now();
    std::cout << "par: \t\t" << (end - start).count() << "ns" << std::endl;

    start = std::chrono::steady_clock::now();
    std::sort(std::execution::par_unseq, v4.begin(), v4.end());
    end = std::chrono::steady_clock::now();
    std::cout << "par_unseq: \t" << (end - start).count() << "ns" << std::endl;

    start = std::chrono::steady_clock::now();
    std::sort(std::execution::unseq, v4.begin(), v4.end());
    end = std::chrono::steady_clock::now();
    std::cout << "unseq: \t\t" << (end - start).count() << "ns" << std::endl;

    /* Print the results. */
	for(int x: v1) {
		std::cout << x << " ";
	}
	std::cout << std::endl;

	for(int x: v2) {
		std::cout << x << " ";
	}
	std::cout << std::endl;

	for(int x: v3) {
		std::cout << x << " ";
	}
	std::cout << std::endl;

	return 0;
}