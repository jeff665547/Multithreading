// 0. Race Condition: 執行結果會與不同執行緒間執行的先後順序有關，不同執行順序會有不同結果。
//    Data Race: 當有兩個以上的執行緒同時去讀取同一塊記憶體位置時，其中一個執行緒去對那塊記憶體位置做操作然後更改它的值。
//    避免方式：
//    a. 以最小範圍保護共享的資料(記憶體位置) -- mutex(對象是資料), lock
//    b. lock-free programming -- atomic
// 1. 一般 所謂無鎖(lock-free) 就是指atomic，但是實際上atomic本身在不同的compiler底下會有不同的實作，有的是用鎖下去實作的，並沒有保證所有的atomic都一定是無鎖的。
// 2. chrono::high_resolution_clock可能是代表chrono::steady_clock，也有可能是代表chrono::system_clock，依照不同的compiler會有不同的實作。
//    chrono::system_clock 代表目前作業系統上面所顯示的(真實世界)時間(幾分幾秒)。
//    chrono::steady_clock 主要是用來測量時間的距離(可用來計時，因其較準確)。
// 3. 生產者與消費者問題
//    a. 當生產者與消費者為一對一時，則可以使用std::future or std::promise。
//    b. 當生產者與消費者為一對多時，則可以使用std::shared_future。
//    c. 當生產者與消費者為一對多或是多對多時，則可以使用thread pool (task queue)來解決問題。
// 4. Memory Model
//    a. 編譯器可以重新編寫程式碼 e.g. x = 1; x += 2; ==> x = 3;
//    b. 編譯器可以重新改變資料存取順序  e.g. x = 3; y = 2; ==> y = 2; x = 3;
//    c. 硬體可以重新改變資料存取順序  e.g. 不同運算單元(CPU & register)看到的記憶體狀態也可能不同。
//    因為上述原因才需要lock或是atomic來保護共用的資料。
//    * Acquire: 從主記憶體(RAM)下載更新補丁紀錄(從其他thread的紀錄)，用來同步thread的內部資料
//               (和主記憶體資料同步)，mutex lock時使用。
//    * Release: 上傳發布更新補丁紀錄到主記憶體(RAM)，用來提供最新資料給別的thread使用，
//               mutex unlock時使用。
//    * Relaxed: 只上傳發布當前某個指定變數更新的補丁紀錄到主記憶體(RAM)。
//    * Sequentially Consistent: 確保每個thread看到的程式碼執行順序(變數修改順序)一致。
// 5. Mutex
//    std::mutex, std::shared_mutex, std::recursive_mutex
//    std::lock_guard, std::unique_lock, std::shared_lock, std::scoped_lock
// 6. 如果要使用mutex來保護共享的資料時，盡量使用class將共享資料、mutex以及操作的函式給包起來。
// 7. 在實作class API的時候，API的設計在一般單執行緒的情況下會和使用多執行緒的情況下可能會有所差異，
//    因為會把需要lock的放在一起做，以節省成本。
// 8. 避免deadlock
//    a. 當有多個lock時，不同執行緒(thread)間lock mutex的(全域)順序一致。 -> std::scoped_lock
//    b. 使用unique_lock初始化多個lock (與std::defer_lock搭配使用)，然後再使用std::lock()一起鎖。
// 9. explicit 關鍵字
//    用法一 -- 放在constructor前面: 在C++中，statement "T t = e;"這樣的寫法被稱作(定義)
//          expression e 被隱性轉型(換)成T(implicit conversion)，而statement "T t(e);"
//          或是"T t{e};"這兩種寫法都被叫做direct initialization(直接初始化)，如果在
//          constructor前面加上explicit則代表強迫使用者在初始化時要使用direct initialization，
//          而不能使用implicit conversion的方式初始化。
//    用法二 -- 放在轉型運算子前面: 類別實作了任意型別的轉型operator之後能不能被隱式轉型成
//          該型別的變數。 例如: explicit operator double() const {return 10.;}
//          => 假設此函式是C類的成員函式，那麼我們在對C類實例進行double的轉型時就會被強制要求使用
//             static_cast<double>來轉型，如：C e{2}; double d = static_cast<double>(e);
//             或是使用 double d = (double) e; 不能使用 double d = e; 這種隱性轉型的方式。
int main(){
    return 0;
}