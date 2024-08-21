#pragma once
#include <iostream>
#include "message.h"

void send_file(SOCKET* sock, const std::string& file_name)
{
	std::fstream file;
	file.open(file_name, std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		int file_size = std::filesystem::file_size(file_name);

		char* bytes = new char[file_size];

		file.read((char*)bytes, file_size);
		//std::cout << "Размер файла: " << file_size << " байт;" << '\n';
		//std::cout << "   Имя файла: " << file_name << '\n';
		//std::cout << "      Данные: " << bytes << '\n';

		send(*sock, std::to_string(file_size).c_str(), 16, 0);
		send(*sock, file_name.c_str(), 32, 0);
		send(*sock, bytes, file_size, 0);

		delete[] bytes;
	}
	else
	{
		std::cout << "Ошибка открытия файла!" << '\n';
	}
	file.close();
}

void recv_file(SOCKET* sock)
{
	char file_size_str[16];
	char file_name[32];

	recv(*sock, file_size_str, 16, 0);
	int file_size = atoi(file_size_str);
	char* bytes = new char[file_size];

	recv(*sock, file_name, 32, 0);

	std::fstream file;
	file.open(file_name, std::ios_base::out | std::ios_base::binary);

	//std::cout << "Размер файла: " << file_size << " байт;" << '\n';
	//std::cout << "   Имя файла: " << file_name << '\n';

	if (file.is_open())
	{
		recv(*sock, bytes, file_size, 0);
		//std::cout << "      Данные: " << bytes << '\n';

		file.write(bytes, file_size);
		//std::cout << "Данные сохранены!" << '\n';
	}
	else
	{
		std::cout << "Ошибка открытия файла!" << '\n';
	}
	delete[] bytes;
	file.close();
}

Message ms;

void clientSocket()
{
	const char SERVER_IP[] = "";					// Enter IPv4 address of Server
	const short SERVER_PORT_NUM = 1234;				// Enter Listening port on Server side
	const short BUFF_SIZE = 1024;
	int erStat;
	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0)
	{
		std::cout << "Ошибка инициализации WinSock-функций!" << '\n';
		std::cout << WSAGetLastError();
	}

	/*ЭТАП 2 - СОЗДАНИЕ СОКЕТА*/

	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET)
	{
		std::cout << "Ошибка инициализации сокета: " << WSAGetLastError() << '\n';
		closesocket(ClientSock); //функция закрытия сокета
		WSACleanup();          //Метод деинициализации сокетов Win32API 
	}
	/*ЭТАП  4*/

	sockaddr_in servInfo;  //Создание  структуры типа sockaddr_in;

	ZeroMemory(&servInfo, sizeof(servInfo));
	//Зваполнение полей структуры servInfo:
	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;   //передача в данную структуру адреса
	servInfo.sin_port = htons(SERVER_PORT_NUM);

	/*Этап 4 (для Клиента). ОРГАНИЗАЦИЯ ПОДКЛЮЧЕНИЯ К СЕРВЕРУ*/

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0)
	{
		std::cout << "Сбой подключения к серверу. Код ошибки: " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
	}
	std::cout << "________________________________________________________________" << '\n';
	std::cout << "СООБЩЕНИЕ СЕРВЕРУ: " << '\n';
	ms.createMessages();
	std::string path = "Messages.txt";
	send_file(&ClientSock, path);
	std::cout << '\n';
	std::cout << "ОТВЕТ СЕРВЕРА: " << '\n';
	recv_file(&ClientSock);
	ms.readPersonalMessages();
	closesocket(ClientSock);
	WSACleanup();


}