// * Threads Management
//   透過傳入一個可呼叫(callable)的物件來建構新的std::thread物件以產生新的執行緒。
//   a. 函式指標：使用std::ref()的方式傳入參考，並且回傳結果。
//   b. 匿名函式：使用[&]capture 參考的方式來回傳結果。
//   不同thread之間共用的資料使用mutex或是atomic來保護資料。
// * std::thread不能被複製(建構)，只能被轉移所有權(移動(建構)std::move())
// * C++規定函式的回傳值預設會驅動移動建構而不是複製建構。

#include <iostream>
#include <thread>

void Hello_ptr(int& output){
    std::cout << "Hello World!" << std::endl;
    output += 2;
}

class Hello_functor{
public:
    void operator()(int& output){   // operator()()中，第一個()代表符號名稱，第二個()內放"()"符號內需要的參數。
        std::cout << "Hello World!" << std::endl;
        output += 2;
    }
};

std::thread CreateThread(){
    int tmp{0};
    std::thread t1{Hello_ptr, std::ref(tmp)};
    std::thread t2 = std::move(t1);  // 透過std::move來觸發t2的移動建構。
    return t2;     // 此處要回傳時相當於有個暫時物件(回傳值)t4在外面(i.e. caller) 
                   // return t2; 可以想像成編譯器自動幫我們做return std::move(t2); 
                   // ==> 透過std::move()轉型來觸發t4的移動建構(C++規定函式的回傳值預設會驅動移動建構而不是複製建構。)
}

class RAIIThread{
    std::thread t_;
public:
    RAIIThread(std::thread&& t): t_(std::move(t)) {}  
    // 此處的parameter type可以是std::thread&&，也可以是std::thread，
    // 這當中的差別差在，如果使用std::thread，不管傳入的參數是rvalue expression
    // (i.e. 也就是 std::move(...)後出來的東西)，
    // 還是暫時物件，到這裡都會觸發std::thread的移動建構來產生t。然而，如果使用
    // std::thread&&的話，就會直接將t bind到rvalue expression或是暫時物件身上，
    // 也就是說，可以少一次移動建構。
    // 另外，expression "t"為type為rvalue reference of std::thread的lvalue 
    // expression，透過expression "std::move(t)"產生type為rvalue reference 
    // of std::thread的rvalue expression 來觸發std::thread的移動建構。
    ~RAIIThread(){
        if(t_.joinable()) t_.join();
    }
};

RAIIThread CreateRAIIThread(){
    int tmp{0};
    return RAIIThread{std::thread{Hello_ptr, std::ref(tmp)}};
    // std::thread{Hello_ptr, std::ref(tmp)} 為一個std::thread的暫時物件，
    // 此std::thread的暫時物件會觸發RAIIThread的一般建構，產生RAIIThread的暫時物件，
    // 接著再利用此RAIIThread的暫時物件觸發回傳給caller的RAIIThread移動建構，
    // 在caller(i.e. main)中產生RAIIThread的暫時物件，而此暫時物件也就是此function的回傳值。
}

int main(){

    std::thread t0;  // 產生一個執行緒(std::thread)物件，但是不會執行任何事情。

    int res{0};

    // 透過callable物件(一般函式(函式指標)、
    // 函式類別物件(functor, function obecjt)、
    // 匿名函式(lambda exp))來建立執行緒(std::thread)物件，
    // 並且實際去啟動一個執行中的執行緒(thread of execution)，並且將該執行緒與執行緒物件給綁在一起。
    // 這樣一來就可以透過t1這個執行緒物件(std::thread)去控制執行中的執行緒(thread of execution)。
    std::thread t1{Hello_ptr, std::ref(res)};   // 透過std::ref去使用一個物件將res給包起來，該物件裡面存的資料是res物件的位址(pointer)。
                                                // 在thread這邊傳入參數時一樣是使用複製的方式，
                                                // 只不過在此複製的是那個用來把res位址(pointer)包起來的那個物件，
                                                // 接著在std::thread中再將res位址轉回物件(dereference)，然後當成參考傳入Hello_ptr中。

    t1.join();
    std::cout << res << std::endl;

    Hello_functor hello_functor{};
    std::thread t2{hello_functor, std::ref(res)};  // 呼叫hello_functor()相當於去呼叫 hello_functor.operator()() -> operator overloading
                                                   // 使用Hello_functor()()代表建立一個暫時物件，並且對該暫時物件呼叫operator()()。    
    t2.join();
    std::cout << res << std::endl;

    std::thread t3{[&res]{std::cout << "Hello World!" << std::endl; res+=2;}};  // 透過lambda expression 讓編譯器自動幫我們生成相對應的functor類別物件。
    t3.join();
    std::cout << res << std::endl;

    std::thread t5 = CreateThread();  // CreateThread()產生暫時物件t4 ==> 執行緒物件在函式內部產生，透過一層一層將執行緒物件的所有權轉移出來到main上。
                                      // CreateThread()產生暫時物件t4，接著觸發t5的移動建構，最後執行緒物件管理所有權落到t5上。
    t5.join();

    RAIIThread t6 = CreateRAIIThread();  // 透過CreateRAIIThread()回傳的RAIIThread暫時物件來觸發t6的移動建構，最後將CreateRAIIThread()內的
                                         // std::thread物件管理所有權轉移到t6上。

    return 0;
}