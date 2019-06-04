#if _WIN32

#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"ws2_32")

#include "Imaging/Image.h"
#include "Networking/networking.h"
#include <windows.h>
#include <gdiplus.h>
#include <thread>
#include <iostream>

using namespace std;

imgdata::Image* image;
char* buffer;

void receiveData(client* client, HWND hwnd, int width, int height)
{
	while (1)
	{
		image->setImage(client->Recv(buffer, width * height * 3), width, height);
		InvalidateRect(hwnd, 0, 0);
	}
}

void Onpaint(HDC hdc, PAINTSTRUCT& ps)
{

	Gdiplus::Graphics graphics(hdc);

	Gdiplus::Bitmap bitmap(image->getWidth(), image->getHeight(), 3 * image->getWidth(), PixelFormat24bppRGB, (BYTE*)image->getImage());
	//Gdiplus::Image bitmap(L"tiger.bmp", false);

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
		//InvalidateRect(hwnd, 0, 0);
		SetTimer(hwnd, 1, 16, 0);
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
	SetTimer(hwnd, 1, 17, 0);

	MSG msg = {};

	//init client
	client client(port);
	int connected = client.Connect(address);
	if (connected)
		return 1;
	//get init info
	printf("connected\n");
	int width = *((int*)client.Recv(4)); //get width
	printf("got width %d\n", width);
	int height = *((int*)client.Recv(4)); //get height
	printf("got height %d\n", height);

	//set up image
	buffer = new char[width * height * 3];

	image = new imgdata::Image(buffer, width, height);
	printf("thread starting\n");
	//receive all necessary data

	std::thread receiving(receiveData, &client, hwnd, width, height);

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