#include "Imaging/Recorder.h"
#include "Imaging/Image.h"
#include "Networking/networking.h"
#include <thread>
#include <stdlib.h>

#pragma comment(lib,"ws2_32")

bool accepting = 1;


void addClient(server* server)
{
	while (1)
	{
		server->Accept();

	}
}

int main(int argc, char** argv)
{
	u_short port = 12345;
	if (argc > 1) {
		port = atoi(argv[1]);
		printf("connected to port %d\n", port);
	}
	server server(port);
	server.Listen(20);
	std::thread addClients(addClient, &server);

	int init = 1;
	int width = 1920;
	int height = 1080;

	imgdata::Image im(1920, 1080);
	for (int i = 0; i < im.getWidth(); i++) {
		for (int j = 0; j < im.getHeight(); j++) {
			im.getPixel(i, j)->R = 0;
			im.getPixel(i, j)->G = 0;
			im.getPixel(i, j)->B = 10;
		}
	}
	while (1) {
		for (int i = 0; i < server.clients.size(); i++) {
			if (server.clients[i].getSocketfd()>=0) {
				if (server.clients.size() == init) {
					init += 1;
					printf((server.clients.back().getAddress() + " connected\n").c_str());
					server.Send(&server.clients[i], (char*)& width, 4);
					server.Send(&server.clients[i], (char*)& height, 4);
				}
				server.Send(&server.clients[i], (char*)im.getImage(), width * height * 3);
			}
		}
	}
}