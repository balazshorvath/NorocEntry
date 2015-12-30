#pragma once

SOCKET TCPServerInit(string port);
SOCKET TCPAccept(SOCKET serverSocket);

int TCPRecv(SOCKET client, char* buffer);
int TCPSend(SOCKET client, char* buffer);

