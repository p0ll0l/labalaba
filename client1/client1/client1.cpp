#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Для подключения библиотеки сокетов

void initWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock" << std::endl;
        exit(1);
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    initWinsock();

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);  // Порт, на котором слушает сервер
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Адрес сервера (localhost)

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка при подключении к серверу" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[512];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Ошибка при получении данных" << std::endl;
            break;
        }
        if (bytesReceived == 0) {
            std::cout << "Соединение с сервером закрыто." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';  // Завершаем строку
        std::cout << "Текущий список от сервера: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
