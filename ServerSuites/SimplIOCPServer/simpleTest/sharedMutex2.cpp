#include <iostream>
#include <shared_mutex>

#include <thread>
#include <chrono>

std::shared_mutex mtx;

int main(void)
{
    using namespace std::chrono_literals;

    auto readOperation = [&](std::chrono::seconds secondsWaits){
        if(secondsWaits > 0s)
            std::this_thread::sleep_for(secondsWaits);


        std::cout << std::this_thread::get_id() << ">> try acquired shared lock" << std::endl;
        {
            std::shared_lock<decltype(mtx)> grd(mtx);
            std::cout << std::this_thread::get_id() << ">> acquired shared lock" << std::endl;
            std::this_thread::sleep_for(15s);
        }

        std::cout << std::this_thread::get_id() << ">> unlock shared lock" << std::endl;        
    };

    auto writeOperation = [&]{
        std::this_thread::sleep_for(3s);

        std::cout << std::this_thread::get_id() << ">> try acquired unique lock" << std::endl;
        {
            std::unique_lock<decltype(mtx)> grd(mtx);
            std::cout << std::this_thread::get_id() << ">> acquired unique lock" << std::endl;
            std::this_thread::sleep_for(3s);
        }
        std::cout << std::this_thread::get_id() << ">> unlock unique lock" << std::endl;        

    };

    std::thread threads[3]
    {
        std::thread(readOperation, 0s),
        std::thread(writeOperation),
        std::thread(readOperation, 5s),
    };

    for( auto& cur : threads )
    {
        cur.join();
    }

    system("pause");

    return 0;
}