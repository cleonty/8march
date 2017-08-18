// SlideShow.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "SlideShow.h"
#include <fstream>
#include <sstream>

#define MAX_LOADSTRING 100
#define FILENAME _T("Природа\\P4262146.JPG")
#define TEXTFILE _T("Природа\\text.txt")
//#define FILENAME _T("Природа\\P4262155.JPG")
// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
RECT rct={0,0,0,0};
std::wstring text;
//std::wstring basename;
std::wstring username;


// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

std::wstring MyReadFile(LPCTSTR lpFilename)
{
	std::string str;
	std::wstring wstr;
	std::ifstream in(lpFilename);
	if(in) {
		std::stringstream ss;
		ss << in.rdbuf();
		str = ss.str();

		USES_CONVERSION;
		wstr = A2W(str.c_str());
	}
	return wstr;
}

std::wstring MyGetUserName()
{
	static LPTSTR lpUsername;
	DWORD dwSize = 0;
	GetUserName(lpUsername, &dwSize);
	lpUsername = new TCHAR[dwSize];
	GetUserName(lpUsername, &dwSize);

	std::wstring str = lpUsername;
	delete [] lpUsername;
	return str;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;


	HRESULT hRes = ::CoInitialize(NULL);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	username = MyGetUserName();
	//MessageBox(NULL, MyGetUserName().c_str(), _T(""), MB_OK);
	//std::wstring textFile = basename + _T(".txt");
	std::wstring textFile = std::wstring(_T("Природа\\")) + username + _T(".txt");
	text = MyReadFile(textFile.c_str());
	if(text.empty()) {
		text = _T("Поздравляем с 8 Марта!");
	}
	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SLIDESHOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SLIDESHOW));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	::CoUninitialize();
	return (int) msg.wParam;


}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	//wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.style			= CS_VREDRAW | CS_HREDRAW | CS_SAVEBITS;// | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SLIDESHOW));
	wcex.hCursor		= NULL; //LoadCursor(hInst, IDC_NO);// LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_SLIDESHOW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL CALLBACK EnumMonitorCallback(
  HMONITOR hMonitor,  // handle to display monitor
  HDC hdcMonitor,     // handle to monitor-appropriate device context
  LPRECT lprcMonitor, // pointer to monitor intersection rectangle
  LPARAM dwData       // data passed from EnumDisplayMonitors
)
{
	LPRECT rct = (LPRECT)dwData;
	UnionRect(rct, rct, lprcMonitor);
	return TRUE;
}


//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   DWORD dwExStyle = WS_EX_TOPMOST;
   DWORD dwStyle = WS_POPUP;
	EnumDisplayMonitors (
	  NULL,                   // handle to a display device context 
	  NULL,          // specifies a clipping rectangle 
	  EnumMonitorCallback,  // pointer to a callback function
	  (LPARAM)&rct              // data passed to the callback function 
	);

   hWnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle,
	   rct.left, rct.top, rct.right - rct.left, rct.bottom - rct.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowCursor(FALSE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	
	case WM_PAINT:
		{
		hdc = BeginPaint(hWnd, &ps);
		Graphics g(hdc);
		//std::wstring imgFile = basename + _T(".jpg");
		std::wstring imgFile = std::wstring(_T("Природа\\")) + username + _T(".jpg");

		Image *img = new Image(imgFile.c_str());
		if(img->GetHeight() < 1) {
			img = Image::FromFile(_T("Природа\\default.jpg"));
		}
		if(img) {
			g.DrawImage(img, (REAL)rct.left, (REAL)rct.top, (REAL)(rct.right - rct.left), (REAL)(rct.bottom - rct.top));
		}
		Font font(FontFamily::GenericSansSerif(), 32, FontStyleRegular);
		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		
		//strFormat.SetLineAlignment(StringAlignment);
		SolidBrush brText(Color::Red);
		PointF ptOrigin((REAL)(rct.left + 10), (REAL)(rct.top + 10)), pt;
		pt = ptOrigin;
		//_T("Поздравляю с 8 Марта!\nЖелаю счастья!\n   Пух.")
		g.DrawString(text.c_str(),
			-1, &font, pt, &strFormat, &brText);

		delete img;
		EndPaint(hWnd, &ps);

		}
		break;
	case WM_CHAR:
		if(wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
