#if _WIN32

#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"ws2_32")

#include "Imaging/Image.h"
#include "Networking/client.h"
#include <windows.h>
#include <gdiplus.h>
#include <thread>
#include <iostream>

using namespace std;
using namespace imgdata;

vector<Image> buffers;
vector<Image> active;

int lastframe = -1;
bool buffering = 0;

char* buffer;
bool connected;
bool updating = 1;
int width, height;

void receiveData(client* client, HWND hwnd)
{
	while (1)
	{
			//cout << width << " " << height << endl;
				//buffer = new char[width * height * 3];
			buffers.push_back(Image(client->Recv(width * height * 3), width, height));
			//image.setImage(client->Recv(width * height * 3), width, height);
			cout << (int)buffers.back()(0, 0)->B << endl;
			if (updating) {
				active = buffers;
				buffers.clear();
				lastframe = 0;
				updating = 0;
			}
	}
}

void Onpaint(HDC hdc, PAINTSTRUCT& ps)
{

	Gdiplus::Graphics graphics(hdc);
	if (!updating) {
		
		Gdiplus::Bitmap bitmap(active[lastframe].getWidth(), active[lastframe].getHeight(), active[lastframe].getBytes() * active[lastframe].getWidth(), PixelFormat24bppRGB, (BYTE*)active[lastframe].getImage());

		float ratio = (float)bitmap.GetHeight() / (float)bitmap.GetWidth();

		//image positioning math
		int height = ps.rcPaint.bottom;
		int width = ps.rcPaint.right;
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 255, 255));

		if (ps.rcPaint.right * ratio > ps.rcPaint.bottom) {
			width = (int)(ps.rcPaint.bottom / ratio);
			graphics.FillRectangle(&brush, 0, 0, ps.rcPaint.right / 2 - width / 2, height);
			graphics.FillRectangle(&brush, ps.rcPaint.right / 2 + width / 2, 0, ps.rcPaint.right / 2 - width / 2, height);
		}
		else {
			height = (int)(ps.rcPaint.right * ratio);
			graphics.FillRectangle(&brush, 0, 0, width, ps.rcPaint.bottom / 2 - height / 2);
			graphics.FillRectangle(&brush, 0, ps.rcPaint.bottom / 2 + height / 2, width, ps.rcPaint.bottom / 2 - height / 2);
		}

		graphics.DrawImage(&bitmap, ps.rcPaint.right / 2 - width / 2, ps.rcPaint.bottom / 2 - height / 2, width, height);
	}
		
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
	{
		if (buffering) {
			updating = 1;//tell thread that it needs a refill
			buffering = 0;
			cout << "done buffering" << endl;
			SetTimer(hwnd, 1, (int)(1.0 / 60.0), 0);
		}
		if (!updating) {
			if (lastframe < active.size()-1) {//has frames in buffer
				SetTimer(hwnd, 1, (int)(1.0 / 60.0), 0);
				lastframe++;
				InvalidateRect(hwnd, 0, 0);
				buffering = 0;
			}
			else {//needs to buffer
				cout << "buffering" << endl;
				SetTimer(hwnd, 1, 3000, 0);
				buffering = 1;

			}
		}
		break;
	}
	case WM_SIZE:
		InvalidateRect(hwnd, 0, 0);
		break;
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hwnd, &ps);
		Onpaint(hdc, ps);
		EndPaint(hwnd, &ps);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main(int argc, char** argv)
{
	//command line use
	int port = 12345;
	std::string address = "127.0.0.1";
	if (argc > 1)
	{
		port = atoi(argv[1]);
		if (argc > 2)
			address = std::string(argv[2]);
		printf("connected to %s on port %d\n", address.c_str(), port);
	}

	//set up window
	HINSTANCE hInstance = GetModuleHandle(0);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	const char* CLASS_NAME = "Sample Window Class";
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                   // Optional window styles.
		CLASS_NAME,          // Window class
		"My Window",         // Window text
		WS_OVERLAPPEDWINDOW, // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,      // Parent window
		NULL,      // Menu
		hInstance, // Instance handle
		NULL       // Additional application data
	);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, 1);

	MSG msg = {};

	//init client
	client client(port);
	
	while (!client.Connect(address)) {
		cout << "attempting to connect\n" << endl;
	}

	//get init info
	printf("connected\n");
	//cout << client.Recv(4) << endl;

	width = *((int*)client.Recv(4)); //get width
	height = *((int*)client.Recv(4)); //get height

	printf("thread starting\n");
	//receive all necessary data

	std::thread receiving(receiveData, &client, hwnd);

	SetTimer(hwnd, 1, 1000, 0);

	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	printf("ended\n");

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0;
}

#else
#error "This can only be used on windows"
#endif