
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

#include "Network.h"



SOCKET TCPServerInit(string port) {
	WSADATA wsaData;
	SOCKET listenSocket;
	int iResult;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (!iResult) 
		return INVALID_SOCKET;
	

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = WSA_FLAG_OVERLAPPED;

	iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (iResult != 0) {
		//Getaddrinfo
		WSACleanup();
		return INVALID_SOCKET;
	}

	listenSocket = INVALID_SOCKET;

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		//Error at serversocket initialization!
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		//bind failed with error: %d WSAGetLastError()
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	freeaddrinfo(result);

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		//Listen failed!
		//WSAGetLastError
		closesocket(listenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	return listenSocket;
}

SOCKET TCPAccept(SOCKET serverSocket) {
	SOCKET client = INVALID_SOCKET;
	FD_SET fd;
	timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(serverSocket, &fd);

	if (select(serverSocket + 1, &fd, NULL, NULL, &tv) > 0) {
		client = accept(serverSocket, NULL, NULL);
	}
	return client;
}

int TCPRecv(SOCKET client, char* buffer, int size) {
	ZeroMemory(buffer, sizeof(char) * size);
	int result;
	FD_SET fd;
	timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&fd);
	FD_SET(client, &fd);

	if (select(client + 1, &fd, NULL, NULL, &tv) > 0) {
		result = recv(client, buffer, size, 0);
		return result;
	}
	return 0;
}
int inline TCPSend(SOCKET client, char* buffer, int size) {
	return send(client, buffer, size, 0);
}