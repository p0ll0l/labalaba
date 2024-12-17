#define WIN32_LEAN_AND_MEAN
#include <iostream> 
#include <string> 
#include <windows.h> 
#include <WinSock2.h> 
#include <WS2tcpip.h>
#include <map>
using namespace std;

int main()
{
    map<string, string> znach;
    znach["1"] = "301";
    znach["2"] = "3011";
    znach["3"] = "3012";
    znach["4"] = "3013";
    znach["5"] = "3014";
    znach["6"] = "3015";
    znach["7"] = "3016";
    znach["8"] = "3017";
    znach["9"] = "3071";
    //настройка среды и сокетов 
    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult = NULL;
    SOCKET ClientSocket = INVALID_SOCKET; //инициализация нерабочего сокета 
    SOCKET ListenSocket = INVALID_SOCKET; //инициализация слушающего сокета 

    string sendBuffer;
  
    char recvBuffer[512];

    int result;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData); //иницализация сокета  
    if (result != 0)
    {
        cout << "WSAStartup failed, result = " << result << endl;
        return 1; //вернуть ошибку 
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; //семейство сокетов по ip.v4 
    hints.ai_socktype = SOCK_STREAM; //потоковая работа сокета 
    hints.ai_protocol = IPPROTO_TCP; //выбор протокола тспайпи 
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo(NULL, "666", &hints, &addrResult); 
    if (result != 0)
    {
        cout << "getaddrinfo failed with error: " << result << endl;
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); //присвоение сокету семейства, типа и протокола 
    if (ListenSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    // для того что бы установить сокет в режим прослушивания нам нужно его привязать к определенному адресу
    result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen); //запрос на соединение(имя сокета, адрес сокета, размер 
    if (result == SOCKET_ERROR) //проверка на ошибку 
    {
        cout << "Binding socket failed" << endl;
        closesocket(ListenSocket); //если ошибка есть, то закрываем наш сокет 
        ListenSocket = INVALID_SOCKET; //возвращаем его обратно в нерабочее состояние 
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    // устанавливаем режим прослушивания данного сокета 
    result = listen(ListenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        cout << "Listening socket failed" << endl;
        closesocket(ListenSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    // возвращает получанный сокет  который уже получен при соеденении с клиентом 
    // функция аксепт возвращет клиенский сокет 
    
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Accepting socket failed" << endl;
        closesocket(ListenSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    closesocket(ListenSocket);

    do
    {
        ZeroMemory(recvBuffer, 512);

        result = recv(ClientSocket, recvBuffer, 512, 0); //прием с какого сокета, куда принимать, какйо размер буффера.
        if (result > 0)
        {
            cout << "Received " << result << " bytes" << endl;
            cout << "Received data: " << recvBuffer << endl;
            sendBuffer = znach[recvBuffer];

            result = send(ClientSocket, sendBuffer.c_str(), sendBuffer.length(), 0);
            if (result == SOCKET_ERROR)
            {
                cout << "Failed to send data back" << endl;
                closesocket(ClientSocket);
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }
        }
        else if (result == 0)
            cout << "Connection closing..." << endl;
        else
        {
            cout << "recv failed with error" << endl;
            closesocket(ClientSocket);
            freeaddrinfo(addrResult);
            WSACleanup();
            return 1;
        }
    } while (result > 0);

    result = shutdown(ClientSocket, SD_SEND);
    if (result == SOCKET_ERROR)
    {
        cout << "Shutdown client socker failed" << endl;
        closesocket(ClientSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;

}
