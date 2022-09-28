#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

struct Client {
	int socket;
	int message = -1;
};

int main(int argc, char const *argv[]) {
	WSADATA wsaData;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		std::cout<<"WSAStartup failed"<<std::endl;
	}
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(7777);
	int _connection = connect(clientSocket,(struct sockaddr*) &server, sizeof(server));
    if (_connection == SOCKET_ERROR) {
        std::cout<<"Connection failed: "<<WSAGetLastError()<<std::endl;
    }
    else {
        std::cout<<"Connection success"<<std::endl;
    };
	int _msg;
	std::cout<<"Enter number to send:"<<std::endl;
	std::cin>>_msg;
	Client _client = {clientSocket,_msg};
	send(clientSocket,(char*)&_client,sizeof(_client),0);
	bool _exit = 0;
	while (!_exit) {
		for (int _i = 0; _i < 12; _i++) {
			Client _client {-1,-1};
			//std::cout<<"Searching for message"<<std::endl;
			recv(clientSocket,(char*)&_client,sizeof(_client),0);
			if (_client.message != -1) {
				std::cout<<"Message received: "<<_client.message<<" from socket: "<<_client.socket<<std::endl;
			};
		};
	};
	WSACleanup();
    return 0;
};