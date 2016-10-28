#include <windows.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstdint>

static void setWindowOnTop(HWND hWnd)
{
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
static LRESULT CALLBACK winMainProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

static int32_t string2int(const WCHAR* str)
{
	std::wstringstream ss;
	ss << str;
	int32_t result = 0;
	ss >> result;
	return result;
}
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	LPWSTR *szArgList = NULL;
	int argCount = 0;
	//parser argument
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (argCount != 9)
	{
		MessageBox(NULL, L"usage:\n\tScreenCover [r] [g] [b] [a] [x] [y] [w] [h]", L"warning message", MB_OK);
		return -1;
	}
	//R G B A x y w h
	const int32_t r = string2int(szArgList[1]);
	const int32_t g = string2int(szArgList[2]);
	const int32_t b = string2int(szArgList[3]);
	const int32_t a = string2int(szArgList[4]);
	const int32_t x = string2int(szArgList[5]);
	const int32_t y = string2int(szArgList[6]);
	const int32_t w = string2int(szArgList[7]);
	const int32_t h = string2int(szArgList[8]);

	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
	wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = winMainProc;

	wndcls.lpszClassName = L"demo";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);


	HWND hwnd = CreateWindow(L"demo", L"demo", WS_POPUP | WS_EX_TOOLWINDOW,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		return -1;
	}
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	//const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	//const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(hwnd, HWND_TOP,0, 0, screenWidth, screenHeight,SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	LONG nRet = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	nRet = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST| WS_EX_TOOLWINDOW;
	::SetWindowLong(hwnd, GWL_EXSTYLE, nRet);
	::SetLayeredWindowAttributes(hwnd, 0, a, LWA_ALPHA);

	SetWindowPos(hwnd, HWND_TOP, x, y, w, h, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	setWindowOnTop(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		::Sleep(10);
	}
	return 0;
}