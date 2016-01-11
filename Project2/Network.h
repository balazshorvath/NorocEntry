#ifndef NETWORK_H
#define NETWORK_H

#define TCP_MAX_PACKET_SIZE 1024

SOCKET TCPServerInit(string port);
SOCKET TCPAccept(SOCKET serverSocket);
SOCKET TCPConnect(string ip, string port);

int TCPRecv(SOCKET client, char* buffer, int size);
int TCPSend(SOCKET client, char* buffer, int size);

#endif