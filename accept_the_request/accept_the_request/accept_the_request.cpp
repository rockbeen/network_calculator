#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)

int main(int argc, char* argv[]) {
	
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);//загружаем библиотеку
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	//соединяемся с калькулятором
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to calculator.\n";
		return 1;
	}
	std::cout << "The calculator is connected!\n";
	std::cout <<"Enter an example in the format: '2*2' or '35-10' or '1 + 1'"<< std::endl;
	char msgInput[256];
	char msgOutput[256];
	
	while (true)
	{
		
		std::cin.getline(msgInput, sizeof(msgInput));//ввод данных
		send(Connection, msgInput, sizeof(msgInput), NULL);//их отправка
		recv(Connection, msgOutput, sizeof(msgOutput), NULL);//получаем результат
		std::cout << msgOutput << std::endl;
		if (msgOutput == "Error: socket was closed!")//Если передано сообщение об ошибке
			break;

	}
	system("pause");
	return 0;
}