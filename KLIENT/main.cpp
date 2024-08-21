#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "message.h"
#include <WinSock2.h> 
#include <WS2tcpip.h> 
#include <stdio.h>
#include <Windows.h> 
#include "client_socket.h"
#pragma comment(lib, "Ws2_32.lib") 

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool repeat = true;
	while (true)
	{
		clientSocket();

        int z{};
        std::cout << '\n';
        std::cout << "Продолжить - 1, завершить - '0'" << '\n';
        std::cin >> z;
        if (z == 1)
        {
            repeat = true;
        }
		else
		{
			repeat = false;
			break;
		}

	}

	return 0;
}