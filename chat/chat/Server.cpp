#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

void main(void) {
	char PCName[30], ClientName[30], Message[200];
	WSAData WSADat; // Свойства WinSock (результат функции WSAStartup)
	sockaddr_in sin; // Свойства(адрес) создаваемого сокета
	SOCKET Sock, Client; // Серверный и клиентский сокеты

	WSAStartup(0x0202, &WSADat); // Инициализация WinSock
								 // 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
								 // WSADat - структура, куда будут занесены рез. инициализации
	gethostname(PCName, 30); // Получение имени текущего ПК

	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = 0;  // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(2803); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // Связывание созданного сокета с адресом sin

											  // ***** Ожиданеи клиента
	printf("Wait of client...\n");

	listen(Sock, SOMAXCONN); // Прослушивание сокета сервером (для подключения клиента)
	Client = accept(Sock, (sockaddr*)&sin, 0); // Ожидание клиента
	recv(Client, ClientName, 30, 0); // Получение имени компьютера клиента
	send(Client, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (сервера)

	printf("Client ( \"%s\" ) has connected!\n", ClientName);

	// ***** Меню
	int Menu;
	do {
		printf("1. Send message;\n");
		printf("2. Get Message;\n");
		printf("3. Quit;\n");

		printf("Make your selection: ");
		scanf_s("%i", &Menu);

		switch (Menu) {
		case 1:
			// Отправка сообщения клиенту
			printf("Enter message: ");
			std::cin.ignore();
			std::cin.getline(Message, 200);
			if (send(Client, Message, strlen(Message) + 1, 0) != SOCKET_ERROR)printf("Sent!\n");
			else printf("Error of sending!\n");
			break;
		case 2:
			// Приём сообщения от клиента
			if (recv(Client, Message, 200, 0) != SOCKET_ERROR) {
				printf("%s\n", Message);
				_gettch();
			}
			else printf("Error of getting!\n");
			break;
		};

		printf("\n");
	} while (Menu != 3);

	// Закрытие сокетов и окончание работы с WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
	_gettch();
}
