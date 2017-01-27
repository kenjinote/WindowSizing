#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static double fixedRate = 1.0;
	switch (msg)
	{
	case WM_ENTERSIZEMOVE:
		OutputDebugString(TEXT("WM_ENTERSIZEMOVE\n"));
		break;
	case WM_SIZING:
		{
			LPRECT lpRect = (LPRECT)lParam;

			int w = lpRect->right - lpRect->left;
			int h = lpRect->bottom - lpRect->top;
			int dw = (int)(h * fixedRate - w);
			int dh = (int)(w / fixedRate - h);

			TCHAR szText[1024];
			wsprintf(szText, TEXT("WM_SIZING %d x %d\n"), w, h);
			OutputDebugString(szText);

			switch (wParam)
			{
			case WMSZ_TOP:
			case WMSZ_BOTTOM:
				lpRect->right += dw;
				break;
			case WMSZ_LEFT:
			case WMSZ_RIGHT:
				lpRect->bottom += dh;
				break;
			case WMSZ_TOPLEFT:
				if (dw > 0) lpRect->left -= dw;
				else lpRect->top -= dh;
				break;
			case WMSZ_TOPRIGHT:
				if (dw > 0) lpRect->right += dw;
				else lpRect->top -= dh;
				break;
			case WMSZ_BOTTOMLEFT:
				if (dw > 0) lpRect->left -= dw;
				else lpRect->bottom += dh;
				break;
			case WMSZ_BOTTOMRIGHT:
				if (dw > 0) lpRect->right += dw;
				else lpRect->bottom += dh;
				break;
			}
		}
		break;
	case WM_EXITSIZEMOVE:
		OutputDebugString(TEXT("WM_EXITSIZEMOVE\n"));
		break;
	case WM_SIZE:
		OutputDebugString(TEXT("WM_SIZE\n"));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window Sizing テスト"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
