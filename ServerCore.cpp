#include "Imaging/Recorder.h"
#include "Imaging/Image.h"
#include "Networking/server.h"
//#include "Networking/client.h"
#include <thread>
#include <stdlib.h>
#include <chrono>

using std::cout;
using std::endl;
using namespace std::chrono;
#pragma comment(lib,"ws2_32")

bool accepting = 1;


void addClient(server* server)
{
	while (1) {
		if (accepting) {
			cout << "accepting" << endl;
			server->Accept();
			accepting = 0;
			cout << "accepted" << endl;
		}
	}

}

int main(int argc, char** argv)
{
	auto timer = high_resolution_clock::now();
	
	u_short port = 12345;
	if (argc > 1) {
		port = atoi(argv[1]);
		printf("connected to port %d\n", port);
	}
	server server(port);
	server.Listen(5);

	std::thread addClients(addClient, &server);

	videoLoader vid("D:/Downloads/Top 5 iOS 13 Features!.mp4");
	printf("-----------------\n");
	imgdata::Image im(1,1);

	//client.Connect("127.0.0.1");
	
	for (int i = 0; i < im.getWidth(); i++) {
		for (int j = 0; j < im.getHeight(); j++) {
			//printf("hi %d\n",im.getWidth()*j +i);
			im(i,j)->R = 0;
			im(i, j)->G = 0;
			im(i, j)->B = 10;
		}
	}
	//get first frame info
	im.setImage(vid.getFrame());
	int width = im.getWidth();
	int height = im.getHeight();

	while (1) {
		duration<double> time_span = duration_cast<duration<double>>(high_resolution_clock::now() - timer);
		if (time_span.count()> 1.0/60.0) {
			timer = high_resolution_clock::now();
			im.setImage(vid.getFrame());
			width = im.getWidth();
			height = im.getHeight();

			//std::cout <<(int)(im.getImage()[0].R)<< std::endl;
			//std::cout << im.pixels.size() << std::endl;
			for (int i = 0; i < server.clients.size(); i++) {
				if (server.clients[i].getSocketfd()) {

					//cout << (int)im.getImage()[800000].B<<" "<<im.getBytes() << endl;
					server.Send(&server.clients[i], (char*)im.getImage() , width * height * 3);
				}
			}
		}

		if (!accepting) {
			
			for (int i = 0; i < server.waitlist.size(); i++) {
				printf((server.waitlist[i].getAddress() + " connected\n").c_str());
				server.Send(&server.waitlist[i], (char*)& width, 4);
				server.Send(&server.waitlist[i], (char*)& height, 4);
			}	
			server.updateClients();
			accepting = 1;
		}
		
	}
}
