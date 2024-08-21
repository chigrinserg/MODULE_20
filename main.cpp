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
#include "serv_socket.h"
#pragma comment(lib, "Ws2_32.lib") 

int main(void)
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool repeat = true;
	while (true)
	{
		servSocket();
	}
	return 0;

}

