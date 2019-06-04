#include "networking.h"

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
	buffer = 0;
	socketfd = sock;
	this->family = family;
	this->port = htons(port);
	address = INADDR_ANY;
	char opt = 1;

	if ((setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))))
	{
		perror("setsockopt");
	}
}

sock::~sock()
{
	//if(buffer)
	//	delete[] buffer;
	closesocket(socketfd);

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
}

void server::Accept()
{
	SOCKET clientfd;
	sockaddr_in temp;
	int addrlen;

	if ((clientfd = accept(socketfd, (struct sockaddr*) & temp, &addrlen)) < 0)
	{
		perror("accepting error");
	}
	clients.push_back(sock(temp.sin_addr.S_un.S_addr, temp.sin_family, temp.sin_port, clientfd));
}

void server::Send(SOCKET clientfd, const char* bytes, int bytenum)
{
	send(clientfd, bytes, bytenum, 0);
}

void server::Send(sock* client, const char* bytes, int bytenum)
{
	if (send(client->getSocketfd(), bytes, bytenum, 0) == SOCKET_ERROR) {
	}
}

const char* server::Recv(SOCKET clientfd, int bytenum)
{
	//if (buffer)
	//	delete buffer;
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
	//	delete buffer;
	buffer = new char[bytenum];
	int result = recv(client->getSocketfd(), buffer, bytenum, 0);

	if (result == 0)
		printf("Connection closed\n");
	else
		printf("recv failed: %d\n", WSAGetLastError());
	return buffer;
}

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
	//	delete buffer;
	buffer = new char[bytenum];
	if (recv(socketfd, buffer, bytenum, 0) <= 0)
	{
		printf("Failed to receive, connection closed\n");
		return 0;
	}
	return buffer;
}

char* client::Recv(char* buff, int bytenum) {
	int result = recv(socketfd, buff, bytenum, 0);
	if (result <= 0)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		return 0;
	}
	return buff;
}


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