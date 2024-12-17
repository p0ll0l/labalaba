#define WIN32_LEAN_AND_MEAN
#include <iostream> 
#include <windows.h> 
#include <WinSock2.h> 
#include <WS2tcpip.h> 
#include <string>
using namespace std;

int main()
{
	//настройка среды и сокетов 
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult = NULL;
	SOCKET ConnectSocket = INVALID_SOCKET; //инициализация нерабочего сокета 

	string sendBuffer;
	char recvBuffer[512];

	int result;
	//инициализайия
	result = WSAStartup(MAKEWORD(2, 2), &wsaData); //запуск  
	if (result != 0)
	{
		cout << "WSAStartup failed, result = " << result << endl;
		return 1; //вернуть ошибку 
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET; //семейство сокетов по ip.v4 
	hints.ai_socktype = SOCK_STREAM; //потоковая работа сокета 
	hints.ai_protocol = IPPROTO_TCP; //выбор протокола тспайпи 
	//задаем парамтры соеденения 
	result = getaddrinfo("localhost", "666", &hints, &addrResult); 
	if (result != 0)
	{
		cout << "getaddrinfo failed with error: " << result << endl;
		WSACleanup();
		return 1; 
	}
	// создаем сокет 
	ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); //присвоение сокету семейства, типа и протокола 
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	//заставляем сокет постучаться к серверу 
	result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) 
	{
		cout << "Unable connect to server" << endl;
		closesocket(ConnectSocket); 
		ConnectSocket = INVALID_SOCKET; 
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	//отправка сообщения на сервер 


	cout << "Sent: " << result << " bytes" << endl;
	cout << "Successfully connected." << endl;
	do
	{
		getline(cin, sendBuffer);
		result = send(ConnectSocket, sendBuffer.c_str(), sendBuffer.length(), 0); //функция отправвки сообщения 
		if (result == SOCKET_ERROR)
		{
			cout << "Sending is failed, error: " << result << endl;
			closesocket(ConnectSocket);
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}

		ZeroMemory(recvBuffer, 512);
		result = recv(ConnectSocket, recvBuffer, 512, 0); //прием с какого сокета, куда принимать, какйо размер буффера. 0 если сокет закрыт с другой стороны 
		if (result > 0)
		{
			cout << "Received " << result << " bytes" << endl;
			cout << "Received data: " << recvBuffer << endl;
		}
		else if (result == 0)
			cout << "Connection closed" << endl;
		else
			cout << "recv failed with error" << endl;
	} while (result > 0);

	closesocket(ConnectSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;

}
