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

int x = 0;
int y = 0;

unsigned int xSize = 80;
unsigned int ySize = 35;

unsigned int mwhOffset = 8;
unsigned int kbOffset = 8;

const WPARAM KEY_CHANGE_APPMODE = VK_TAB;
const WPARAM KEY_CHANGE_PICMODE = VK_SPACE;

PAINTSTRUCT ps;

HPEN stdPen;
HBRUSH stdBrush;

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
			if (appMode == ModeManual) y = (y + wHeight - kbOffset) % wHeight;
			break;
		}
		case VK_DOWN: {
			if (appMode == ModeManual) y = (y + kbOffset) % wHeight;
			break;
		}
		case VK_LEFT: {
			if (appMode == ModeManual) x = (x + wWidth - kbOffset) % wWidth;
			break;
		}
		case VK_RIGHT: {
			if (appMode == ModeManual) x = (x + kbOffset) % wWidth;
			break;
		}

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_MOUSEWHEEL: {
		if (appMode == ModeManual) {
			WORD loWord = LOWORD(wParam);
			short hoWord = HIWORD(wParam);
			if (loWord == 0x0004) { //if shift pressed
				if (hoWord > 0) { //far from user
					x = (x + mwhOffset) % wWidth;
				}
				else {  //to user
					x = (x + wWidth - mwhOffset) % wWidth;
				}
			}
			else {  //if not
				if (hoWord > 0) {
					y = (y + wHeight - mwhOffset) % wHeight;
				}
				else {
					y = (y + mwhOffset) % wHeight;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_TIMER: {


		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT: {

		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdcDoubleBuff = CreateCompatibleDC(hdc);
		HBITMAP bitmap = CreateCompatibleBitmap(hdc, wWidth, wHeight);
		SelectObject(hdcDoubleBuff, bitmap);
		if (picMode == ModeRectangle) {
			HGDIOBJ oldPen = SelectObject(hdcDoubleBuff, stdPen);
			HGDIOBJ oldBrush = SelectObject(hdcDoubleBuff, stdBrush);

			Rectangle(hdcDoubleBuff, x, y, x + xSize, y + ySize);

			SelectObject(hdcDoubleBuff, oldPen);
			SelectObject(hdcDoubleBuff, oldBrush);
		}
		else {
		
		}
		BitBlt(hdc,0,0,wWidth,wHeight,hdcDoubleBuff,0,0,SRCCOPY);

		DeleteObject(bitmap);
		DeleteDC(hdcDoubleBuff);
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

	stdPen = CreatePen(PS_SOLID, 4, RGB(230, 30, 75));
	stdBrush = CreateSolidBrush(RGB(211, 193, 36));

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	DeleteObject(stdPen);
	DeleteObject(stdBrush);

	return (int)msg.wParam;
}
