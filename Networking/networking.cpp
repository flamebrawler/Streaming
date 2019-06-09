#include "networking.h"


int setupSock()
{
	int sock;
#if _WIN32
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSA Setup Failed. Error Code : %d", WSAGetLastError());
		exit(1);
	}
#endif

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{

#if _WIN32
		printf("Socket Failed. Error Code : %d", WSAGetLastError());
#elif unix
#endif

		exit(1);
	}
	return sock;
}

void receive(SOCKET s, char* buf, int bytes)
{
	int result = recv(s, buf, bytes, 0);

	if (result == 0)
		printf("Connection closed\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());
}

int receiveInt(SOCKET s, int bytes)
{
	char buf[4] = { 0 };
	int result = recv(s, buf, bytes, 0);

	if (result == 0)
	{
		printf("Connection closed\n");
	}
	else if (result < 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
	}
	return bytestoint(buf, 0, 4);
}

void sendInt(SOCKET s, int data, int bytes)
{
	char buf[4] = { 0 };
	inttobytes(buf, data, 0, 4);
	send(s, buf, bytes, 0);
}

int bytestoint(const char* data, int pos, int bytes)
{
	int num = *((int*)data);
	num >>= pos;
	int mask = 0;
	for (int i = 0; i < bytes; i++)
	{
		mask |= 0xf;
		mask <<= 1;
	}
	return num & mask;
}

int inttobytes(char* buffer, int data, int pos, int bytes)
{
	for (int i = 0; i < bytes; i++)
	{
		buffer[i + pos] = data & 0xff;
		data >>= 8;
	}
	return 0;
}