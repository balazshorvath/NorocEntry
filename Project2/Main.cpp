#include "Includes.h"

#define WAIT_BEFORE_STOP 30
#define LISTEN_PORT "1234"

int main(int argc, char* argv[]) {
//	if (argc < 2) {
//		return -1;
//	}
	SOCKET serverSocket = INVALID_SOCKET;
	string input;
	thread tcpServer, udpServer;
	running = false;
	threadCount = 0;
	

	while (true) {
		cin >> input;
		if (input == "start") {
			if (!running) {
				if(serverSocket == INVALID_SOCKET)
					serverSocket = TCPServerInit(LISTEN_PORT);

				if (serverSocket == INVALID_SOCKET) {
					cout << "Socket init failed : " << WSAGetLastError() << endl;
					continue;
				}
				running = true;
				tcpServer = thread(TCPServer, serverSocket);
				//udpServer = thread(UDPListener, &argv[2], &argv[3]);

				//udpServer.detach();
				tcpServer.detach();
				threadCount++;
			}
		} else if (input == "stop") {
			running = false;
			closesocket(serverSocket);
			WSACleanup();
			serverSocket = INVALID_SOCKET;
		} else if (input == "exit") {
			if (running) {
				running = false;
				closesocket(serverSocket);
				WSACleanup();
				serverSocket = INVALID_SOCKET;
			}
			break;
		} else Log("PLS!4! (start/stop/exit)");
	}


	time_t startTime = time(0);
	time_t currentTime;
	int seconds;

	do {
		system("cls");
		currentTime = time(0);

		seconds = WAIT_BEFORE_STOP - (currentTime - startTime);
		
		cout << FormatString("Stopping in : %d\nThreads running : %d", 128, seconds, threadCount) << endl;
		Sleep(500);
	} while (seconds > 0);

	return 0;
}


void TCPServer(SOCKET sock) {
	SOCKET client = INVALID_SOCKET;
	DWORD timeoutMs = TCP_CLIENT_TIMEOUT;


	while (running) {
		client = INVALID_SOCKET;

		client = TCPAccept(sock);
		if (client != INVALID_SOCKET) {

			if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutMs, sizeof(timeoutMs)) != 0) {
				Log("Client connection failed. Couldn't set socket timeout.");
				closesocket(client);
				continue;
			}

			Log("Client connected.");
			thread(TCPClient, client).detach();
			threadCount++;
		}
		Sleep(200);
	}

	threadCount--;
}

void TCPClient(SOCKET sock) {
	char buffer[TCP_MAX_PACKET_SIZE];
	char errorMsg[3] = { 0 };
	char session[32] = { 0 };
	int recvSize;
	SOCKET database;

	errorMsg[1] = ';';
	errorMsg[2] = '\n';

	while (running && sock != INVALID_SOCKET) {
		recvSize = TCPRecv(sock, buffer, TCP_MAX_PACKET_SIZE);
		if (recvSize == -1) {
			Log("Client timed out.");
			closesocket(sock);
			sock = INVALID_SOCKET;
			break;
		}
		if (!recvSize) {
			Sleep(20);
			continue;
		}

		switch (buffer[0] ){//& 0x0F) {
			case TCP_MESSAGE_LOGOUT:
				closesocket(sock);
				break;
			case TCP_MESSAGE_LOGIN:
				database = TCPConnect("127.0.0.1", "1111");
				if (database == INVALID_SOCKET) {
					Log("Cannot connect to DB. Disconnectiong client.");
					errorMsg[0] = TCP_MESSAGE_ERROR | MESSAGE_ERROR_DB;
					TCPSend(sock, errorMsg, 3);
					sock = INVALID_SOCKET;
					continue;
				}
				buffer[1] = ';';
				TCPSend(database, buffer, recvSize);
				recvSize = TCPRecv(database, buffer, TCP_MAX_PACKET_SIZE);
				TCPSend(sock, buffer, recvSize);
				if (buffer[0] == TCP_MESSAGE_LOGIN) 
					memcpy_s(session, 24, buffer + 2, 24);
				
				break;
			case TCP_MESSAGE_QUERY:
			case TCP_MESSAGE_UPDATE:
				database = TCPConnect("127.0.0.1", "1111");
				if (database == INVALID_SOCKET) {
					Log("Cannot connect to DB. Disconnectiong client.");
					errorMsg[0] = TCP_MESSAGE_ERROR | MESSAGE_ERROR_DB;
					TCPSend(sock, errorMsg, 3);
					sock = INVALID_SOCKET;
					continue;
				}
				if (session[0] == 0) {
					char requestType = buffer[0];
					char requestSubtype = buffer[1];
					char tmp[2];
					/*Use recv buffer to deliver to DB*/
					buffer[0] = DB_SESSION_EXISTS;

					TCPSend(database, buffer, 28);
					TCPRecv(database, tmp, 2);
					//TODO: FIXME!!
					if (tmp[0] & 0xFF != DB_SESSION_EXISTS & 0xFF) {
						errorMsg[0] = TCP_MESSAGE_ERROR | MESSAGE_ERROR_LOGIN_REQ;
						TCPSend(sock, errorMsg, 3);
						continue;
					} 
					memcpy_s(session, 24, buffer + 2, 24);
					/*set original back*/
					buffer[0] = requestType;
					buffer[0] = requestType;
				}
				TCPSend(database, buffer, recvSize);
				recvSize = TCPRecv(database, buffer, TCP_MAX_PACKET_SIZE);
				TCPSend(sock, buffer, recvSize);

				break;
		}
		closesocket(database);
	}

	threadCount--;
}

/*
void UDPListener(string portRel, string portUnrel) {

threadCount--;
}
*/