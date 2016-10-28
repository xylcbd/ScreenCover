#include <windows.h>
#include <stdio.h>

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
	case WM_TIMER:
		setWindowOnTop(hwnd);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 250, 0));
	wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = winMainProc;

	wndcls.lpszClassName = "demo";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);


	HWND hwnd = CreateWindow("demo", "demo", WS_POPUP | WS_VISIBLE,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		return -1;
	}
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	LONG nRet = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	nRet = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
	::SetWindowLong(hwnd, GWL_EXSTYLE, nRet);
	::SetLayeredWindowAttributes(hwnd, 0, 50, LWA_ALPHA);

	::SetTimer(hwnd, 100, 10, NULL);
	setWindowOnTop(hwnd);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(hwnd, HWND_TOP,0, 0, screenWidth, screenHeight,SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	SetWindowPos(hwnd, HWND_TOP, 0, 100, 1600, 1200, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}