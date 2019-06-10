#include "client.h"


bool client::Connect(std::string IP)
{
	return Connect(inet_addr(IP.c_str()));
}

bool client::Connect(u_long IP)
{
	sockaddr_in temp = { family, port, *((in_addr*)& IP) };
	if (connect(socketfd, (struct sockaddr*) & temp, sizeof(temp)) < 0)
	{
		printf("Connection Failed: %d\n", WSAGetLastError());
		return 0;
	}
	return 1;
}

void client::Send(const char* bytes, int bytenum)
{
	send(socketfd, bytes, bytenum, 0);
}

client::~client() {
	delete[] buffer;
}

const char* client::Recv(int bytenum)
{
	delete[] buffer;
	buffer = new char[bytenum];
	if (recv(socketfd, buffer, bytenum, 0) <= 0)
	{
		printf("Failed to receive, connection closed %d\n", WSAGetLastError());
		return buffer;
	}
	return buffer;
}

char* client::Recv(char* buff, int bytenum) {
	int result = recv(socketfd, buff, bytenum, 0);
	if (result <= 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		return buffer;
	}
	return buff;
}

