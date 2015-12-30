#include "Includes.h"


int main(int argc, char* argv[]) {
	SOCKET serverSocket = INVALID_SOCKET;
	string input;
	thread tcpServer, udpServer;
	running = true;
	

	while (true) {
		cin >> input;
		if (input == "start") {
			if (!running) {
				if(serverSocket == INVALID_SOCKET)
					serverSocket = TCPServerInit(argv[1]);
				
				tcpServer = thread(TCPServer, &serverSocket);
				udpServer = thread(UDPListener, &argv[2], &argv[3]);

				udpServer.detach();
				tcpServer.detach();
			}
		} else if (input == "stop") {
			running = false;
			closesocket(serverSocket);
			WSACleanup();
			serverSocket = INVALID_SOCKET;
		}
	}
	
	return 0;
}