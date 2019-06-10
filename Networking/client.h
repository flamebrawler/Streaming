#pragma once
#include "sock.h"
#include <iostream>

class client : virtual public sock
{
public:
	client(u_short port, short family = AF_INET) : sock(port, family) {};
	~client();
	bool Connect(std::string IP);
	bool Connect(u_long IP);
	void Send(const char* bytes, int bytenum);
	const char* Recv(int bytenum);
	char* Recv(char* buff, int bytenum);
};
