#include "server.h"

server::server(u_short port, short family) : sock(port, family)
{
	Bind();
	printf("%d %d %d\n", port, family, socketfd);
}

void server::Bind()
{
	sockaddr_in temp = { family, port, *((in_addr*)& address) };
	if ((bind(socketfd, (struct sockaddr*) & temp, sizeof(temp))) < 0)
	{
#if _WIN32
		printf("bind error %d\n", WSAGetLastError());
#elif unix
#endif
	}
}

void server::Listen(int clients)
{
	if (listen(socketfd, clients) < 0)
	{
		perror("listening error");
	}
	//printf("listen error %d\n", WSAGetLastError());
}

void server::Accept()
{
	
	SOCKET clientfd;
	sockaddr_in temp;
	int addrlen = sizeof(temp);

	if ((clientfd = accept(socketfd, (struct sockaddr*) & temp, &addrlen)) <= 0)
	{
		perror("accepting error");
	}
	
	//printf("accept error %d\n", WSAGetLastError());
	
	//printf("made connection %d %d %d\n",clientfd, temp.sin_port,addrlen);
	waitlist.push_back(sock(temp.sin_addr.S_un.S_addr, temp.sin_family, temp.sin_port, clientfd));
}

void server::updateClients() {
	if (waitlist.size()) {
		clients.insert(clients.end(), waitlist.begin(), waitlist.end());
		waitlist.clear();
	}
}

void server::Send(SOCKET clientfd, const char* bytes, int bytenum)
{
	send(clientfd, bytes, bytenum, 0);
}

void server::Send(sock* client, const char* bytes, int bytenum)
{
	//std::cout << "hello " << (int)bytes[2400000] << "\n";
	if(client->getSocketfd()!=0)
	if (send(client->getSocketfd(), bytes, bytenum, 0) == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
	}
}

const char* server::Recv(SOCKET clientfd, int bytenum)
{
	//if (buffer)
	//	delete[] buffer;
	buffer = new char[bytenum];

	int result = recv(clientfd, buffer, bytenum, 0);

	if (result <= 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		return 0;
	}
	return buffer;
}

char* server::Recv(sock* client, char* buff, int bytenum) {
	int result = recv(client->getSocketfd(), buff, bytenum, 0);

	if (result <= 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		return 0;
	}
	return buff;
}

const char* server::Recv(sock* client, int bytenum)
{
	//if (buffer)
	//	delete[] buffer;
	buffer = new char[bytenum];
	int result = recv(client->getSocketfd(), buffer, bytenum, 0);

	if (result == 0)
		printf("Connection closed\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());
	return buffer;
}

server::~server() {
	for (int i = 0; i < waitlist.size(); i++) {
		waitlist[i].Disconnect();
	}
	for (int i = 0; i < clients.size(); i++) {
		clients[i].Disconnect();
	}
}