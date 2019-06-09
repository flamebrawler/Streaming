#include "sock.h"

sock::sock(std::string addr, short family, u_short port, SOCKET socketfd) : sock(inet_addr(addr.c_str()), family, port, socketfd) {}

sock::sock(u_long addr, short family, u_short port, SOCKET socketfd)
{
	address = addr;
	this->family = family;
	this->port = port;
	this->socketfd = socketfd;
	buffer = 0;
}

sock::sock(u_short port, short family)
{
	SOCKET sock;
#if _WIN32
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSA Setup Failed. Error Code : %d", WSAGetLastError());
	}
#endif
	if ((sock = socket(family, SOCK_STREAM, 0)) < 0)
	{
#if _WIN32
		printf("Socket Failed. Error Code : %d\n", WSAGetLastError());
#elif unix
#endif
	}
	socketfd = sock;
	this->family = family;
	this->port = htons(port);
	address = INADDR_ANY;
	char opt = 1;
	buffer = 0;

	if ((setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))))
	{
		perror("setsockopt");
	}
}

sock::~sock()
{
	//closesocket(socketfd);

}

u_short sock::getPort() const
{
	return port;
}
SOCKET sock::getSocketfd() const
{
	return socketfd;
}
std::string sock::getAddress() const
{
	return std::string(inet_ntoa(*((in_addr*)& address)));
}

void sock::Disconnect() {
	closesocket(socketfd);
}