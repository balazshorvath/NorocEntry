#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define TCP_MESSAGE_LOGIN	0x01
#define TCP_MESSAGE_QUERY	0x02
#define TCP_MESSAGE_GAME	0x03
#define TCP_MESSAGE_UPDATE	0x04
#define TCP_MESSAGE_LOGOUT	0x05

#define TCP_MESSAGE_ERROR	0x10

#define MESSAGE_ERROR_DB	0

/*QUERY types*/
#define QUERY_GET_CHARACTERS	0x01
#define QUERY_GET_ITEMS			0x02
#define QUERY_SET_CHARACTERS	0x01
#define QUERY_SET_ITEMS			0x01

/*20 secs, b4 recv times out, after colse connection*/
#define TCP_CLIENT_TIMEOUT	20000

bool running;
int threadCount;

void TCPServer(SOCKET sock);
void TCPClient(SOCKET sock);
//void UDPListener(string portRel, string portUnrel);

#endif
