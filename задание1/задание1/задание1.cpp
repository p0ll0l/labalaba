#include <iostream> 
#include <thread> 
#include <chrono> 
#include <random> 
#include <semaphore> 
#include <atomic> 
#include <vector> 
#include <windows.h>   

using namespace std;

counting_semaphore<4> posts(4);
atomic<int> missed_cars(0);
atomic<int> total_cars(0);
atomic<bool> simulation_running(true);

void car_service() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> refuel_time(1, 4);
    uniform_int_distribution<> arrival_time(1, 2);

    while (simulation_running) {
        this_thread::sleep_for(chrono::milliseconds(arrival_time(gen)));
        total_cars++;

        if (posts.try_acquire()) {
            this_thread::sleep_for(chrono::milliseconds(refuel_time(gen)));
            posts.release();
        }
        else {
            missed_cars++;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int num_threads = thread::hardware_concurrency();
    vector<thread> threads;

    auto start_time = chrono::steady_clock::now();
    auto simulation_duration = chrono::seconds(10);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(car_service);
    }

    this_thread::sleep_for(simulation_duration);
    simulation_running = false;

    for (auto& t : threads) {
        t.join();
    }

    cout << "Симуляция завершена.\n";
    cout << "Количество машин, которые не смогли заправиться: " << missed_cars << "\n";
    cout << "Общее количество прибывших машин: " << total_cars << "\n";

    return 0;
}
