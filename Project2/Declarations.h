#pragma once

bool running;

void TCPServer(SOCKET sock);
void TCPClient(SOCKET sock);
void UDPListener(string portRel, string portUnrel);

