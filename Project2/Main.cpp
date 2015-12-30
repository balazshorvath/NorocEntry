#include "Includes.h"


int main(int argc, char* argv[]) {
	SOCKET serverSocket;
	string input;
	thread tcpServer, udpServer;
	running = true;
	
	serverSocket = TCPServerInit(argv[1]);

	while (true) {
		cin >> input;
		if (input == "start") {
			if (!running) {
				tcpServer = thread(TCPServer, &serverSocket);
				udpServer = thread(UDPListener, &argv[2], &argv[3]);

				udpServer.detach();
				tcpServer.detach();
			}
		} else if (input == "stop") {
			running = false;
		}
	}
	
	return 0;
}