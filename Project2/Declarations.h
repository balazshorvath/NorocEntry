#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define TCP_MESSAGE_LOGIN	0x01
#define TCP_MESSAGE_QUERY	0x02
#define TCP_MESSAGE_GAME	0x03
#define TCP_MESSAGE_UPDATE	0x04
#define TCP_MESSAGE_LOGOUT	0x05

#define TCP_MESSAGE_ERROR	0x10

#define MESSAGE_ERROR_DB		0x05
#define MESSAGE_ERROR_LOGIN_REQ	0x06

/*QUERY types*/
#define QUERY_GET_CHARACTERS	0x01
#define QUERY_GET_ITEMS			0x02
/*UPDATE types*/
#define UPDATE_SET_CHARACTERS	0x01
#define UPDATE_SET_ITEMS		0x02

/*session handling*/
#define DB_SESSION_EXISTS 0xE0
#define DB_SESSION_DELETE 0xF0

/*20 secs, b4 recv times out, after colse connection*/
#define TCP_CLIENT_TIMEOUT	1200000

bool running;
int threadCount;
vector<string> users;

void TCPServer(SOCKET sock);
void TCPClient(SOCKET sock);
//void UDPListener(string portRel, string portUnrel);

bool SessionExists(string session);

#endif
