#pragma once
#include <Windows.h>

enum AppMode {
	ModeManual = 0,
	ModeAutomatic = 1
};
AppMode appMode = ModeManual;

enum PicMode {
	ModeRectangle = 0,
	ModePicture = 1
};
PicMode picMode = ModeRectangle;

int wHeight = 600;
int wWidth = 1200;
int mwhOffset = 8;

const WPARAM KEY_CHANGE_APPMODE = VK_TAB;
const WPARAM KEY_CHANGE_PICMODE = VK_SPACE;

PAINTSTRUCT ps;

HBITMAP hBitmap;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	
	case WM_SIZE: {
		wWidth = LOWORD(lParam);
		wHeight = HIWORD(lParam);
		break;
	}
	case WM_KEYDOWN: {
		
		switch (wParam) {
		
		case KEY_CHANGE_APPMODE: {
			if (appMode == ModeManual) {
				appMode = ModeAutomatic;
				SetTimer(hWnd, 1, 1, NULL);
			}
			else {
				appMode = ModeManual;
				KillTimer(hWnd, 1);
			}
			break;
		}
		case KEY_CHANGE_PICMODE: {
			picMode = (picMode == ModeRectangle)?ModePicture:ModeRectangle;
		}
		case VK_UP: {
		
		}
		case VK_DOWN: {

		}
		case VK_LEFT: {

		}
		case VK_RIGHT: {

		}
		default:

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT: {
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_ERASEBKGND:
		return 1;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWindowClass";

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(L"MainWindowClass", L"Main Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, wWidth, wHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}
