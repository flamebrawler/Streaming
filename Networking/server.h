#pragma once
#include "sock.h"

class server : virtual public sock
{
private:
	void Bind();
	void Send(SOCKET clientfd, const char* bytes, int bytenum);
	const char* Recv(SOCKET clientfd, int bytenum);
	
public:
	std::vector<sock> clients;
	std::vector<sock> waitlist;
	server(u_short port, short family = AF_INET);
	~server();
	void Listen(int clients);
	void Accept();
	void Send(sock* client, const char* bytes, int bytenum);
	const char* Recv(sock* client, int bytenum);
	char* Recv(sock* client, char* buff, int bytenum);
	void updateClients();
};


