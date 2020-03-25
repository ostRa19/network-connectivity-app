#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

void main(void) {
	char PCName[30], ClientName[30], Message[200];
	WSAData WSADat; // �������� WinSock (��������� ������� WSAStartup)
	sockaddr_in sin; // ��������(�����) ������������ ������
	SOCKET Sock, Client; // ��������� � ���������� ������

	WSAStartup(0x0202, &WSADat); // ������������� WinSock
								 // 0x0202 - ������ WinSock. ����� ���� 1.0, 1.1, 2.0, 2.2
								 // WSADat - ���������, ���� ����� �������� ���. �������������
	gethostname(PCName, 30); // ��������� ����� �������� ��

	sin.sin_family = AF_INET; // ��� ������
	sin.sin_addr.s_addr = 0;  // IP-����� ������� (���� �������� ������� ����� 0)
	sin.sin_port = htons(2803); // ����� ����� �������
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // �������� ������
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // ���������� ���������� ������ � ������� sin

											  // ***** �������� �������
	printf("Wait of client...\n");

	listen(Sock, SOMAXCONN); // ������������� ������ �������� (��� ����������� �������)
	Client = accept(Sock, (sockaddr*)&sin, 0); // �������� �������
	recv(Client, ClientName, 30, 0); // ��������� ����� ���������� �������
	send(Client, PCName, strlen(PCName) + 1, 0); // �������� ����� ����� ���������� (�������)

	printf("Client ( \"%s\" ) has connected!\n", ClientName);

	// ***** ����
	int Menu;
	do {
		printf("1. Send message;\n");
		printf("2. Get Message;\n");
		printf("3. Quit;\n");

		printf("Make your selection: ");
		scanf_s("%i", &Menu);

		switch (Menu) {
		case 1:
			// �������� ��������� �������
			printf("Enter message: ");
			std::cin.ignore();
			std::cin.getline(Message, 200);
			if (send(Client, Message, strlen(Message) + 1, 0) != SOCKET_ERROR)printf("Sent!\n");
			else printf("Error of sending!\n");
			break;
		case 2:
			// ���� ��������� �� �������
			if (recv(Client, Message, 200, 0) != SOCKET_ERROR) {
				printf("%s\n", Message);
				_gettch();
			}
			else printf("Error of getting!\n");
			break;
		};

		printf("\n");
	} while (Menu != 3);

	// �������� ������� � ��������� ������ � WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
	_gettch();
}
