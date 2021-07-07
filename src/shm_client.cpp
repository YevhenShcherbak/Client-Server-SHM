#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include <WS2tcpip.h>
#include <iostream>
#include <string>

int main()
{
    std::string ipAddress = "127.0.0.1";
    int port = 54000;

    WSAData wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &wsData);
    if (wsResult != 0)
    {
        std::cerr << "Error, can not initialize win socket" << std::endl;
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Error, can not create socket!" << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in hint; 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    

    int conResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (conResult == SOCKET_ERROR)
    {
        std::cerr << "Error, can not connect to socket" << std::endl;
        closesocket(sock);
        return -1;
    }

    char buf[4096];
    std::cout << buf << std::endl;
    std::string userInput;

    do
    {
        ZeroMemory(buf, 4096);
        std::cout << "> ";
        std::getline(std::cin, userInput);
        if(userInput.size() > 0)
        {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            std::cout << sendResult << std::endl;
            if (sendResult == SOCKET_ERROR)
            {
                std::cerr << "Error, can not send data" << std::endl; 
            }
            else
            {
                std::cout << buf << std::endl;
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    std::cout << "Server> " << std::string(buf, 0, bytesReceived) << std::endl;
                }
            }
        }
    } while (userInput.size() > 0);

    closesocket(sock);
    WSACleanup();
}