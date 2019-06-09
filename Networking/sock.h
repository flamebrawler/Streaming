#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#pragma warning(disable:4996) 
#pragma comment(lib,"ws2_32.lib")

#if _WIN32
#include <winsock2.h>
#elif unix
#include <sys/socket.h>
#endif

class sock
{
protected:
	u_long address;
	short family;
	u_short port;
	SOCKET socketfd;
	char* buffer;

public:
	sock(std::string addr, short family, u_short port, SOCKET socketfd);
	sock(u_long addr, short family, u_short port, SOCKET socketfd);
	sock(u_short port, short family = AF_INET);
	~sock();
	void Disconnect();
	u_short getPort() const;
	SOCKET getSocketfd() const;
	std::string getAddress() const;
};

