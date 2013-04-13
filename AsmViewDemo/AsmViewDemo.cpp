// AsmViewDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AsmViewDemo.h"
#include "..\AsmView\AsmView.h"
#include <commctrl.h>
#include <CommDlg.h>


#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HWND g_hwndAsmView = NULL;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASMVIEWDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	InitAsmView();

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASMVIEWDEMO));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASMVIEWDEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASMVIEWDEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:
			{
				char *szFilter		= _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
				char szFileName[MAX_PATH] = {0};
				char szTitleName[MAX_PATH] = {0};

				OPENFILENAME ofn	= { sizeof(ofn) };

				ofn.hwndOwner		= hWnd;
				ofn.hInstance		= GetModuleHandle(0);
				ofn.lpstrFilter		= szFilter;
				ofn.lpstrFile		= szFileName;
				ofn.lpstrFileTitle	= szTitleName;

				ofn.nFilterIndex	= 1;
				ofn.nMaxFile		= _MAX_PATH;
				ofn.nMaxFileTitle	= _MAX_FNAME + _MAX_EXT; 

				// flags to control appearance of open-file dialog
				ofn.Flags			=	OFN_EXPLORER			| 
					OFN_ENABLESIZING		|
					OFN_ALLOWMULTISELECT	| 
					OFN_FILEMUSTEXIST;

				if (GetOpenFileName(&ofn))
				{
					if(AsmView_OpenFile(g_hwndAsmView, szFileName))
					{
						SetWindowText(hWnd, szTitleName);
						return TRUE;
					}
					else
					{
						MessageBox(hWnd, _T("Error opening file"), NULL, MB_ICONEXCLAMATION);
						return FALSE;
					}
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		{
			g_hwndAsmView = CreateAsmView(hWnd);

			HIMAGELIST hImgList = ImageList_LoadImage(
				hInst, 
				MAKEINTRESOURCE(IDB_BITMAP1), 
				16, 0, 
				RGB(255,0,255),
				IMAGE_BITMAP,
				LR_LOADTRANSPARENT|LR_CREATEDIBSECTION
				);

			AsmView_SetImageList(g_hwndAsmView, hImgList);
			AsmView_SetLineImage(g_hwndAsmView, 5,  2);

			AsmView_SetLineSpacing(g_hwndAsmView, 5, 5);

			AsmView_SetStyleBool(g_hwndAsmView, TXS_SELMARGIN,	TRUE);
			AsmView_SetStyleBool(g_hwndAsmView, TXS_LINENUMBERS,	FALSE);
			AsmView_SetStyleBool(g_hwndAsmView, TXS_LONGLINES,	FALSE);
			AsmView_SetStyleBool(g_hwndAsmView, TXS_HIGHLIGHTCURLINE,	TRUE);
			AsmView_SetCaretWidth(g_hwndAsmView, 2);

			HDC hdc      = GetDC(0);
			int nLogSize = 0-MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
			ReleaseDC(0, hdc);
			HFONT hFont = CreateFont(nLogSize, 
				0, 0, 0, 
				0,
				0,0,0,0,0,0,
				0,
				0,
				"������");
			SendMessage(g_hwndAsmView, WM_SETFONT, (WPARAM)hFont, 0);

			//AsmView_OpenFile(g_hwndAsmView, "D:\\1.txt");
			break;
		}
	case WM_SIZE:
		{
			short width  = (short)LOWORD(lParam);
			short height = (short)HIWORD(lParam);

			MoveWindow(g_hwndAsmView, 0, 0, width, height, TRUE);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
