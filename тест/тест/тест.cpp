#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
using namespace std;

const int PAGE_SIZE =32;
const int MEMORY_SIZE = 128;
const int NUM_PAGES = MEMORY_SIZE / PAGE_SIZE;

uint8_t memory[MEMORY_SIZE];
uint8_t external[MEMORY_SIZE];
vector<bool> page_used(NUM_PAGES, false); // Флаги использования страниц

int get_page_number(int address) {
    return address / PAGE_SIZE;
}

// Получение смещения внутри страницы
int get_offset(int address) {
    return address % PAGE_SIZE;
}

int find_unused_page() {
    for (int i = 0; i < NUM_PAGES; i++) {
        if (!page_used[i]) {
            return i;
        }
    }
    return -1;
}

// Перемещение страницы между оперативной памятью и внешним носителем
void swap_page(int page_number) {
    int external_index = page_number * PAGE_SIZE; // Указатель на страницу на внешнем носителе
    int memory_index = find_unused_page() * PAGE_SIZE; // Ищем не использованную страницу в памяти

    if (memory_index != -1) {
        // Записываем содержимое внешнего носителя в оперативную память
        memcpy(&memory[memory_index], &external[external_index], PAGE_SIZE);
        // Включаем флаг использования страницы
        page_used[memory_index / PAGE_SIZE] = true;

        cout << "Страница " << page_number << " загружена в память.\n";
    }
    else {
        cout << "Нет свободной памяти! Применяем алгоритм NRU\n";

        // Если нет свободной, ищем страницу для выгрузки
        int page_to_evict = find_unused_page();
        if (page_to_evict != -1) {
            // Сохраняем данные из памяти на внешний носитель
            memcpy(&external[page_to_evict * PAGE_SIZE], &memory[page_to_evict * PAGE_SIZE], PAGE_SIZE);
            page_used[page_to_evict] = false; // Помечаем страницу как "не использованную"
            // Заменяем её на новую страницу
            memcpy(&memory[page_to_evict * PAGE_SIZE], &external[external_index], PAGE_SIZE);
            page_used[page_to_evict] = true; // Помечаем новую страницу как "использованную"

            cout << "Страница " << page_to_evict << " выгружена на внешний носитель.\n";
            cout << "Страница " << page_number << " загружена в память.\n";
        }
    }
}

// Чтение ячейки памяти
void read_memory(int address) {
    int page_number = get_page_number(address);
    if (page_used[page_number]) {
        cout << "Чтение из адреса " << address << ": " << (int)memory[address] << endl;
    }
    else {
        cout << "Страница с адресом " << address << " находится на внешнем носителе, загружаем в память.\n";
        swap_page(page_number);
        cout << "Чтение из адреса " << address << ": " << (int)memory[address] << endl;
    }
}

// Запись в ячейку памяти
void write_memory(int address, uint8_t value) {
    int page_number = get_page_number(address);
    if (page_used[page_number]) {
        memory[address] = value;
        cout << "Запись значения " << (int)value << " в адрес " << address << endl;
    }
    else {
        cout << "Страница с адресом " << address << " находится на внешнем носителе, загружаем в память.\n";
        swap_page(page_number);
        memory[address] = value;
        cout << "Запись значения " << (int)value << " в адрес " << address << endl;
    }
}

// Отображение карты распределения страниц
void display_page_map() {
    cout << "Карта распределения страниц:\n";
    for (int i = 0; i < NUM_PAGES; i++) {
        cout << "Страница " << i << ": "

            << (page_used[i] ? "В оперативной памяти" : "На внешнем носителе") << "\n";
    }
}

// Главная функция с пользовательским интерфейсом
int main() {

    setlocale(LC_ALL, "ru");

    // Инициализация памяти и внешнего носителя нулями
    memset(memory, 0, MEMORY_SIZE);
    memset(external, 0, MEMORY_SIZE);

    int choice, address, value;
        while (true) {
            cout << "\nВыберите действие:\n";
            cout << "1. Чтение из памяти\n";
            cout << "2. Запись в память\n";
            cout << "3. Отображение карты распределения страниц\n";
            cout << "4. Выход\n";
            cout << "Ваш выбор: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Введите адрес для чтения: ";
                cin >> address;
                if (address < 0 ||  address >= MEMORY_SIZE) {
                    cout << "Некорректный адрес! Попробуйте снова.\n";
                }
                else {
                    read_memory(address);
                }
                break;
            case 2:
                cout << "Введите адрес для записи: ";
                cin >> address;
                if (address < 0 ||  address >= MEMORY_SIZE) {
                    cout << "Некорректный адрес! Попробуйте снова.\n";
                }
                else {
                    cout << "Введите значение (0 - 255): ";
                    cin >> value;
                    if (value < 0 || value > 255) {
                        cout << "Некорректное значение! Попробуйте снова.\n";
                    }
                    else {
                        write_memory(address, value);
                    }
                }
                break;
            case 3:
                display_page_map();
                break;
            case 4:
                cout << "Выход из программы.\n";
                return 0;
            default:
                cout << "Некорректный выбор! Попробуйте снова.\n";
                break;
            }
        }

    return 0;
}