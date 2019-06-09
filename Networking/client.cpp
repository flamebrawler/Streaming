#include "client.h"


int client::Connect(std::string IP)
{
	return Connect(inet_addr(IP.c_str()));
}

int client::Connect(u_long IP)
{
	sockaddr_in temp = { family, port, *((in_addr*)& IP) };
	if (connect(socketfd, (struct sockaddr*) & temp, sizeof(temp)) < 0)
	{
		printf("Connection Failed: %d\n", WSAGetLastError());
		return 1;
	}
	return 0;
}

void client::Send(const char* bytes, int bytenum)
{
	send(socketfd, bytes, bytenum, 0);
}

const char* client::Recv(int bytenum)
{
	//if (buffer)
		//delete buffer;
	buffer = new char[bytenum];
	if (recv(socketfd, buffer, bytenum, 0) <= 0)
	{
		printf("Failed to receive, connection closed %d %d\n", WSAGetLastError());
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

