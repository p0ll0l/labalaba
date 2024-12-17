#include <iostream> 
#include <thread> 
#include <chrono>
#include <random>
#include <semaphore> 
#include <atomic>  
#include <vector> 
#include <windows.h>

using namespace std; // Использование стандартного пространства имен

// Создание семафора, позволяющего одновременно обслуживать до 4 машин
counting_semaphore<4> posts(4);
atomic<int> missed_cars(0); 
atomic<int> total_cars(0); 
atomic<bool> simulation_running(true); 


void car_service() {
    random_device rd; // Создание объекта для генерации случайных чисел
    mt19937 gen(rd()); // Инициализация генератора случайных чисел
    uniform_int_distribution<> refuel_time(1, 4);
    uniform_int_distribution<> arrival_time(1, 2);

    // Цикл, выполняющийся, пока симуляция активна
    while (simulation_running) {
        this_thread::sleep_for(chrono::milliseconds(arrival_time(gen))); // Ожидание времени прибытия машины
        total_cars++;

        // Попытка захватить семафор
        if (posts.try_acquire()) {
            this_thread::sleep_for(chrono::milliseconds(refuel_time(gen))); // Ожидание времени заправки
            posts.release(); // Освобождение семафора после завершения заправки
        }
        else {
            missed_cars++; 
        }
    }
}

int main() {
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251); 

    const int num_threads = thread::hardware_concurrency(); // Получение количества доступных потоков
    vector<thread> threads; // Вектор для хранения потоков

    auto start_time = chrono::steady_clock::now();
    auto simulation_duration = chrono::seconds(10);

    // Запуск потоков, отвечающих за обслуживание машин
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(car_service); // Добавление нового потока в вектор
    }

    this_thread::sleep_for(simulation_duration); // Ожидание завершения симуляции
    simulation_running = false;

    // Ожидание завершения всех потоков
    for (auto& t : threads) {
        t.join(); // Присоединение потока к основному потоку
    }

    cout << "Симуляция завершена.\n"; 
    cout << "Количество машин, которые не смогли заправиться: " << missed_cars << "\n";
    cout << "Общее количество прибывших машин: " << total_cars << "\n";

    return 0;
}
