#pragma once
#include "message.h"

Message ms;
void send_file(SOCKET* sock, const std::string& file_name)
{
	std::fstream file;
	file.open(file_name, std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		int file_size = std::filesystem::file_size(file_name);

		char* bytes = new char[file_size];

		file.read((char*)bytes, file_size);
		//std::cout << "������ �����: " << file_size << " ����;" << '\n';
		//std::cout << "   ��� �����: " << file_name << '\n';
		//std::cout << "      ������: " << bytes << '\n';

		send(*sock, std::to_string(file_size).c_str(), 16, 0);
		send(*sock, file_name.c_str(), 32, 0);
		send(*sock, bytes, file_size, 0);

		delete[] bytes;

	}
	else
	{
		std::cout << "������ �������� �����!" << '\n';
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

	//std::cout << "������ �����: " << file_size << " ����;" << '\n';
	//std::cout << "   ��� �����: " << file_name << '\n';

	if (file.is_open())
	{
		recv(*sock, bytes, file_size, 0);
		//std::cout << "      ������: " << bytes << '\n';

		file.write(bytes, file_size);
		//std::cout << "������ ���������!" << '\n';
	}
	else
	{
		std::cout << "������ �������� �����!" << '\n';
	}
	delete[] bytes;
	file.close();
}


void servSocket()
{
	const char IP_SERV[] = "";			// Enter local Server IP address
	const int PORT_NUM = 1234;			// Enter Open working server port
	const short BUFF_SIZE = 1024;		// Maximum size of buffer for exchange info between server and client

	// Key variables for all program
	int erStat;
	in_addr ip_to_num; //��������� in_addr ������������ ����� ��������-����� IPv4.
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (erStat <= 0)
	{
		std::cout << "������ �������� IP-������" << '\n';
	}

	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0)
	{
		std::cout << "������ ������������� WinSock-�������!" << '\n';
		std::cout << WSAGetLastError();
	}
	/*���� 2 - �������� ������*/

	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET)
	{
		std::cout << "������ ������������� ������: " << WSAGetLastError() << '\n';
		closesocket(ServSock); //������� �������� ������
		WSACleanup();          //����� ��������������� ������� Win32API 
	}

	sockaddr_in servInfo;  //��������  ��������� ���� sockaddr_in;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(PORT_NUM);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0)
	{
		std::cout << "������ �������� ������ � ������ ��� �����. ��� ������: " << WSAGetLastError() << '\n';
		closesocket(ServSock);
		WSACleanup();
	}

	/*���� 4.������������� ������������ �����*/

	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0)
	{
		std::cout << "������������� ����� ����������. ��� ������: " << WSAGetLastError() << '\n';
		closesocket(ServSock);
		WSACleanup();
	}
	else
	{
		std::cout << "���� ��������������..." << '\n';
	}

	/*���� 5*/
	sockaddr_in clientInfo;

	ZeroMemory(&clientInfo, sizeof(clientInfo));

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		std::cout << "������ ������, �� ���������� ����������. ������ " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
	}
	std::cout << "________________________________________________________________________" << '\n';

	std::cout << "��������� �� �������: " << '\n';
	recv_file(&ClientConn);
	ms.readPersonalMessages();
	std::cout << '\n';
	std::cout << "����� �������: " << '\n';
	ms.createMessages();
	std::string path = "Messages.txt";
	send_file(&ClientConn, path);

	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();

}
