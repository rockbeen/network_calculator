#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)

SOCKET Connection;
bool SOCKETclosed = false;

void endSocet(SOCKET);

double calculator(char msg[])
{
    int sign = 1;//есть ли минут в начале
    int j = 0;
    int ten = 1;//порядок
    double first = 0;//первое число
    double second = 0;//второе число
    double result=0;
    char action;
    for (int i = 0; msg[i]; i++)//убираем пробелы
    {
        if (msg[i] == ' ') {
            int k = i;
            do {
                msg[k] = msg[k + 1];
                k++;
            } while (msg[k]);
        }
    }
   
   if (msg[j] == '-')
        {
            sign = -1;
            ++j;
        }
   if (!(msg[j] >= '0' && msg[j] <= '9'))
   {
       std::cout << "Input error\n";
       endSocet(Connection);
   }
   else
   {
       while (msg[j] >= '0' && msg[j] <= '9')//считываем первое число
       {
           first = (first * ten + msg[j] - '0');
           ten = 10;
           j++;
       }
       first = sign * first;
       ten = 1;

       action = msg[j];
       j++;
       if (!(msg[j] >= '0' && msg[j] <= '9'))
       {
           std::cout << "Input error\n";
           endSocet(Connection);
       }
       else
       {
           while (msg[j] >= '0' && msg[j] <= '9')//считываем второе число
           {
               second = (second * ten + msg[j] - '0');
               ten = 10;
               j++;
           }
           if (msg[j] != '\0')
           {
               std::cout << "Input error\n";
               endSocet(Connection);
           }
           switch (action)//производим вычисления
           {     
           case '+': result = first + second; break;
           case '-': result = first - second; break;
           case '*': result = first * second; break;
           case '/': 
           {
               if (second == 0)
               {
                   std::cout << "Error: Division by zero\n";
                   endSocet(Connection);
               }
               else
               {
                   result = first / second;
                   break;
               }
           }
           default:
           {
               std::cout << "Wrong action\n";
                endSocet(Connection);
           }
           }

           return result;
       }
   }
}
void endSocet(SOCKET s)//закрытие сокета
{
    
    char msgOutput[256] = { "Error: socket was closed!" };
    
    send(s, msgOutput, sizeof(msgOutput), NULL);
    shutdown(s, SD_BOTH);  

    closesocket(s);// сокет закрыт
    SOCKETclosed = true;

}
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
    addr.sin_family = AF_INET;//TCP/IP

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//слушаем
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));//привязать адрес
    listen(sListen, SOMAXCONN);
    //удерживам соединения

    Connection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

    if (Connection == 0) {
        std::cout << "Error\n";
    }
    else {
        std::cout << "'Accept the request' was connected!\n";
        char msgInput[256];
        char msgOutput[256];
        double result;
        while (true)
        {

            if (SOCKETclosed) break;                       
            recv(Connection, msgInput, sizeof(msgInput), NULL);//принимаем данные
            result = calculator(msgInput);//считаем

            _snprintf(msgOutput, sizeof(msgOutput), "%f", result);//из double в строку

            send(Connection, msgOutput, sizeof(msgOutput), NULL);//передает строчку
            
            
        }
    }


    system("pause");
    return 0;
}