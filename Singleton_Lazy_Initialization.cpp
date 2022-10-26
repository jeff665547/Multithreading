// Background: 因為初始化操作成本較高，所以只在(在multithreaded情況下)
//             需要用到的時候才去初始化(歸零或是去new一個新物件)。
// Sol: 
// a. Initialize static local variable to ensure thread safe (Meyers Singleton).  => Very fast
// b. Double-checked locking pattern (with the lock and atomic implementation).  => Fast
// c. Use the std::once_flag & std::call_once().  => Very slow
// d. Double-checked locking pattern without atomic (with the C++17 compiler --> ensure the memory allocation order).
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <mutex>
using namespace std::literals;
class complex_data_type{
    int val{0};
    // ... complex data
public:
    void do_it(){
        std::cout << "I am initialized. val is " << val << std::endl;
    }
    int plus(){
        return ++val;
    }
};

class Singleton{
public:
    static Singleton& get_instance_general() {
        // Meyers Singleton 的典型使用範例
        static Singleton tmp_instance;  // local static object 在此函式第一次被呼叫使用才
                                        // 初始化，直到整個程式結束才結束其變數生命週期
                                        // 複雜資料在其類別的建構函式中完成。
        return tmp_instance;
    }

    int get_instance_static_local_object() {
        // Meyers Singleton (主要的使用方式為上面的user case，底下這個範例為demo使用)
        static complex_data_type tmp_instance;  // local static object 在函式第一次被呼叫使用才
                                                // 初始化，直到整個程式結束才結束其變數生命週期
                                                // 複雜資料在其類別的建構函式中完成。
        return tmp_instance.plus();
    }

    int get_instance_DCLP() const {
        if(!Valid.load()){  // 第一個check用來確保value不重複初始化
            std::lock_guard<std::mutex> lk(mv);  // 這個lock用來確保接下來的code
                                                 // 一次只能有一個thread進去執行，但是
                                                 // 不保證下面程式碼的程式執行順序和source
                                                 // code 寫的一致，因為compiler或是CPU可能
                                                 // 為了優化而去更動程式碼執行順序。
            if(!Valid.load()){  // 第二個check用來過濾掉不小心通過第一個check的thread，
                                // 確保進到這裡的thread是真的需要初始化的。
                                // 此外，此處使用atomic來檢查可以確保接下來下面的動作
                value = Dosomethingexpensive();
                std::cout << "Initialized!" << std::endl;
                Valid.store(true);  // 此處的atomic除了確保自己的值在被設定的過程中不被其他的thread讀取打斷以外，
                                    // 同時也確保了從第二個check以來的所有操作都會在此atomic被設定成true以前
                                    // 執行完畢(柵欄功能)，另外，atomic的操作預設記憶體模型為Sequentially 
                                    // Consistent，也就是確保每個thread執行程式碼的順序一致。
            }
        }
        return value;
    }
    complex_data_type get_instance_DCLP_pointer() const {
        if(!Valid_for_some_complex_data.load()){
            std::lock_guard<std::mutex> lk(mc);
            if(!Valid_for_some_complex_data.load()){
                Valid_for_some_complex_data = new complex_data_type();  // 因為有()，代表有產生實體。
                (*Valid_for_some_complex_data).do_it();
            }
        }
        return *Valid_for_some_complex_data.load();
    }

    void get_instance_from_call_once() const {
        std::call_once(initialized, [&]{  
            // 這邊的once_flag (i.e. initialized)精神正如同DCLP中的bool ( i.e. Valid)
            std::cout << "Initialization for val2" << std::endl;
            value2 = 0;
        });
    }

    int Dosomethingexpensive() const {
        std::this_thread::sleep_for(3s);
        return 3;
    }
private:
    mutable std::mutex mv;
    mutable std::mutex mc;
    mutable std::atomic<bool> Valid {false};
    mutable std::atomic<complex_data_type*> Valid_for_some_complex_data {nullptr};
    mutable std::once_flag initialized;
    mutable int value;
    mutable int value2;
    // 成員函式的body前面有const修飾(e.g. void f() const {...})
    // 代表該函式保證不會修改物件實體自己的任何成員變數，也就代表const物件可以使用該成員函式，
    // 對於非const物件則不會有影響，一定可以用該函式。一般來說，成員函式body在有const修飾的情況下
    // 成員函式body內部的成員變數也只能使用有const修飾的函式(避免成員變數的狀態遭到變動)。
    // 若有需要在const修飾的成員函式body中去修改某個特定的成員變數值(e.g. mutex)，
    // 則需要在該成員變數宣告時加上mutable關鍵字來修飾該成員變數(e.g. mutable std::mutex mdq;)。
    // 意思就是代表該成員變數即便在物件是const的情況下其值仍然可以被更動。

    Singleton() = default;
    // 底下刪除是為了避免在程式外面創建新的實體，為Singleton的設計模式特色。
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

int main() {

    Singleton& tmp = Singleton::get_instance_general();
    std::thread t1{[&]{std::cout << "val: " << tmp.get_instance_static_local_object() << std::endl;}};
    std::thread t2{[&]{std::cout << "val: " << tmp.get_instance_static_local_object() << std::endl;}};
    std::thread t3{[&]{std::cout << tmp.get_instance_DCLP() << std::endl;}};
    std::thread t4{[&]{std::cout << tmp.get_instance_DCLP() << std::endl;}};
    std::thread t5{[&]{tmp.get_instance_DCLP_pointer().do_it();}};
    std::thread t6{[&]{tmp.get_instance_DCLP_pointer().do_it();}};
    std::thread t7{[&]{tmp.get_instance_from_call_once();}};
    std::thread t8{[&]{tmp.get_instance_from_call_once();}};
    t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join(); t7.join(); t8.join();

    return 0;
}