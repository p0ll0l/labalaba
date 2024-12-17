#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

// Структура для представления узла двусвязного списка
struct Node {
    int value;
    Node* next;
    Node* prev;
};

// Структура для хранения данных общего доступа
struct SharedData {
    Node* head;
    Node* tail;
    std::mutex mtx;  // Мьютекс для синхронизации доступа к данным
};

void displayList(SharedData* data) {
    std::lock_guard<std::mutex> lock(data->mtx);  // Блокируем мьютекс для безопасного доступа к данным

    if (data->head == nullptr) {
        std::cout << "Список пуст!" << std::endl;
        return;
    }

    Node* current = data->head;  // Указатель на текущий узел, начинаем с головы списка
    std::cout << "-----------------Элементы списка: ";
    while (current != nullptr) {  // Проходим по всем узлам списка
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// Функция для преобразования данных списка в строку
std::string listToString(SharedData* data) {
    std::lock_guard<std::mutex> lock(data->mtx);  // Блокируем мьютекс для безопасного доступа

    if (data->head == nullptr) {  // Проверка, пуст ли список
        return "";
    }

    std::ostringstream oss;  // Создаем строковый поток для формирования строки
    Node* current = data->head; 

    while (current != nullptr) {
        oss << current->value;
        if (current->next != nullptr) {
            oss << " ";
        }
        current = current->next;  // Переходим к следующему узлу
    }

    return oss.str();
}


// Инициализация сокета
void initSocket(SOCKET& serverSocket, sockaddr_in& serverAddr) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при создании сокета!" << std::endl;
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);  // Порт для соединения
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Привязка ко всем интерфейсам

    // Привязка сокета
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка при привязке сокета!" << std::endl;
        closesocket(serverSocket);
        exit(1);
    }

    // Прослушивание порта
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Ошибка при прослушивании сокета!" << std::endl;
        closesocket(serverSocket);
        exit(1);
    }
}

// Добавление элемента в список
void addNode(SharedData* data, int value) {
    std::lock_guard<std::mutex> lock(data->mtx);
    Node* newNode = new Node{ value, nullptr, nullptr };

    if (data->head == nullptr) {
        data->head = data->tail = newNode;
    }
    else {
        data->tail->next = newNode;
        newNode->prev = data->tail;
        data->tail = newNode;
    }
}

// Удаление элемента из списка
void removeNode(SharedData* data) {
    std::lock_guard<std::mutex> lock(data->mtx);

    if (data->head == nullptr) return;

    Node* nodeToRemove = data->head;
    if (data->head == data->tail) {
        data->head = data->tail = nullptr;
    }
    else {
        data->head = data->head->next;
        data->head->prev = nullptr;
    }

    delete nodeToRemove;
}

// Изменение списка
// Отправка списка клиенту после каждого изменения
void modifyList(SharedData* data, SOCKET clientSocket) {
    srand(time(0));

    while (true) {
        int action = rand() % 2;  // Случайное действие: добавление или удаление
        if (action == 0) {
            int value = rand() % 100;
            std::cout << "Добавляем элемент: " << value << std::endl;
            addNode(data, value);
        }
        else {
            std::cout << "Удаляем элемент" << std::endl;
            removeNode(data);
        }


        // Отправка текущего состояния списка клиенту
        std::string message = listToString(data);
        send(clientSocket, message.c_str(), message.size(), 0);

        // displayList(data);  // Для вывода на сервере
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Пауза между операциями
    }
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    SharedData* sharedData = nullptr;
    HANDLE hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedData), "SharedList");

    if (hMapFile == NULL) {
        std::cerr << "Не удалось создать или открыть объект общей памяти!" << std::endl;
        return 1;
    }

    sharedData = (SharedData*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
    if (sharedData == NULL) {
        std::cerr << "Не удалось отображать объект общей памяти!" << std::endl;
        return 1;
    }

    sharedData->head = nullptr;
    sharedData->tail = nullptr;

    // Инициализация сокета
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    initSocket(serverSocket, serverAddr);

    // Ожидаем подключения клиента (consumer)
    std::cout << "Ожидаем подключения клиента..." << std::endl;

    // Корректное использование accept с указателем на структуру sockaddr_in
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при принятии соединения!" << std::endl;
        closesocket(serverSocket);
        return 1;
    }
    std::cout << "Клиент подключен!" << std::endl;

    // Запуск процесса изменения списка
    modifyList(sharedData, clientSocket);

    // Освобождаем ресурсы
    UnmapViewOfFile(sharedData);
    CloseHandle(hMapFile);
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
