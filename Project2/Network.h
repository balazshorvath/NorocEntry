#pragma once

SOCKET TCPServerInit(string port);
SOCKET TCPAccept(SOCKET serverSocket);

int TCPRecv(SOCKET client, char* buffer, int size);
int inline TCPSend(SOCKET client, char* buffer, int size);

