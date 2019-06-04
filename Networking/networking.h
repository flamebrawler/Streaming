#ifndef NETWORKING_H
#define NETWORKING_H

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
	//void Disconnect();
	u_short getPort() const;
	SOCKET getSocketfd() const;
	std::string getAddress() const;
};

class server : virtual public sock
{
private:
	void Bind();
	void Send(SOCKET clientfd, const char* bytes, int bytenum);
	const char* Recv(SOCKET clientfd, int bytenum);

public:
	std::vector<sock> clients;
	server(u_short port, short family = AF_INET);
	void Listen(int clients);
	void Accept();
	void Send(sock* client, const char* bytes, int bytenum);
	const char* Recv(sock* client, int bytenum);
	char* Recv(sock* client, char* buff, int bytenum);
};

class client : virtual public sock
{
public:
	client(u_short port, short family = AF_INET) : sock(port, family) {};
	int Connect(std::string IP);
	int Connect(u_long IP);
	void Send(const char* bytes, int bytenum);
	const char* Recv(int bytenum);
	char* Recv(char* buff, int bytenum);
};

int setupSock();

void receive(SOCKET s, char* buf, int bytes);

int receiveInt(SOCKET s, int bytes);

void sendInt(SOCKET s, int data, int bytes);

int bytestoint(const char* data, int pos, int bytes);

int inttobytes(char* buffer, int data, int pos, int bytes);

#endif