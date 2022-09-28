#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>

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
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	// Make socket nonblocking
	u_long iMode = 1;
	int serverSocket_nonblock = ioctlsocket(serverSocket, FIONBIO, &iMode);
	if (serverSocket == INVALID_SOCKET) {
		std::cout<<"Failed to create socket, ERROR: "<<errno<<std::endl;
	};
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(7777);
	if (bind(serverSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout<<"Failed to bind to port 7777, ERROR: "<<errno<<std::endl;
	}
    else {
        std::cout<<"Server successfully created."<<std::endl;
    };
	std::cout<<"Listening..."<<std::endl;
	int _listen = listen(serverSocket,10);
	if (_listen == SOCKET_ERROR) {
		std::cout<<"Listen fail."<<std::endl;
	}
	else {
		std::cout<<"Listen success."<<std::endl;
	};
	std::list<Client> clients;
	while (true) {
		//Scan for clients
		auto addrlen = sizeof(sockaddr);
		int _accept = accept(serverSocket,(struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
		if (_accept == INVALID_SOCKET) {
			//std::cout<<"No client received."<<std::endl;
		}
		else {
			std::cout<<"Client with socket: "<<_accept<<" received."<<std::endl;
			clients.push_back({_accept,-1});
		};
		//Receive each client message
		for (Client& _a : clients) {
			Client _client;
			//std::cout<<"Searching for message"<<std::endl;
			recv(_a.socket,(char*)&_client,sizeof(_client),0);
			if (_client.message != -1 /*&& _client.socket == _a.socket*/) {
				std::cout<<"Message received: "<<_client.message<<" from socket: "<<_client.socket<<std::endl;
				_a.message = _client.message;
			};
		};
		//Send each client message to all clients
		for (int _i = 0; _i < 12; _i++) {
			for (Client _a : clients) {
				for (Client _b : clients) {
					int _send = send(_a.socket,(char*)&_b,sizeof(_b),0);
					if (_send != SOCKET_ERROR) {
						std::cout<<"Sending socket: "<<_b.socket<<" to socket: "<<_a.socket<<std::endl;
					};
				};
			};
		};
	};
	std::cout<<"Press ENTER to exit."<<std::endl;
	std::getchar();
	WSACleanup();
    return 0;
};