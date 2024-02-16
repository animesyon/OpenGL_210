// 
//     This is a program to simulate rain. To do this, I create a
//     thread that will add circle (scale, color) to the list at interval.
//     A second thread will create a ripple effect. Expand the circle while
//     fading the color. Circles that are no longer visible on 
//     screen are removed from the list.
//
//     To start the animation select Animate -> Play or press Enter
//     To stop  the animation select Animate -> Stop or press Space
//     To increase rainfall press any number fron 1 to 6

#include "stdafx.h"
#include "rain.h"
#include "queue.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// thread
HANDLE hThread1 = NULL;
HANDLE hThread2 = NULL;
CQueue que;
DWORD interval;

//
GLuint circle;
double left, right, bottom, top;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI     ThreadFunc1(LPVOID lpParam);
DWORD WINAPI     ThreadFunc2(LPVOID lpParam);

//
void DrawCircle();

void OnAddRainDrop(LPARAM lParam);
void OnAnimateRainDrop(LPARAM lParam);

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC *hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(HWND hWnd, int cx, int cy);

void OnRainExit(HWND hWnd);

void OnAnimatePlay(HWND hWnd);
void OnAnimateStop(HWND hWnd);

void OnRainfall1(HWND hWnd);
void OnRainfall2(HWND hWnd);
void OnRainfall3(HWND hWnd);
void OnRainfall4(HWND hWnd);
void OnRainfall5(HWND hWnd);
void OnRainfall6(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RAIN, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAIN));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_RAIN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

    // save the instance handle in a global variable
	hInst = hInstance;

	// create the main program window
	int X, Y, nWidth, nHeight, Cx, Cy;
	
    // 480p: 854x480

    Cx = 854;
    Cy = 480;

    nWidth = Cx + 16;
    nHeight = Cy + 58;

    X = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2;
    Y = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 4;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		                      X, Y,
		                      nWidth, nHeight,
		                      nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return FALSE;

	// display the main program window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAIN));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;

    switch (message)
    {
	case WM_ADD_RAINDROP:		OnAddRainDrop(lParam);		break;
	case WM_ANIMATE_RAINDROP:	OnAnimateRainDrop(lParam);	break;
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
			case IDM_RAIN_EXIT:		OnRainExit(hWnd);		break;

			case IDM_ANIMATE_PLAY:	OnAnimatePlay(hWnd);	break;
			case IDM_ANIMATE_STOP:	OnAnimateStop(hWnd);	break;

			case IDM_RAINFALL_1:	OnRainfall1(hWnd);		break;
			case IDM_RAINFALL_2:	OnRainfall2(hWnd);		break;
			case IDM_RAINFALL_3:	OnRainfall3(hWnd);		break;
			case IDM_RAINFALL_4:	OnRainfall4(hWnd);		break;
			case IDM_RAINFALL_5:	OnRainfall5(hWnd);		break;
			case IDM_RAINFALL_6:	OnRainfall6(hWnd);		break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:   OnPaint(hDC);									break;
	case WM_CREATE:  OnCreate(hWnd, &hDC);							break;
	case WM_DESTROY: OnDestroy(hWnd, hDC);							break;
	case WM_SIZE:    OnSize(hWnd, LOWORD (lParam), HIWORD (lParam));break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// execute function OnAddRainDrop at interval
DWORD WINAPI ThreadFunc1(LPVOID lpParam)
{
	HWND* phWnd = (HWND*)lpParam;
	HWND hWnd = *phWnd;

   srand( (unsigned)time( NULL ) );

    while(TRUE)
    {
        Sleep(interval);
        SendMessage(hWnd, WM_ADD_RAINDROP, 0, 0);
    }

	return 0;
}

// execute function OnAnimateRainDrop at interval
DWORD WINAPI ThreadFunc2(LPVOID lpParam)
{
	HWND* phWnd = (HWND*)lpParam;
	HWND hWnd = *phWnd;
	DWORD tm = 40;

    while(TRUE)
    {
        Sleep(tm);
        SendMessage(hWnd, WM_ANIMATE_RAINDROP, 0, (LPARAM)&tm);
    }

	return 0;
}

// draw circle in xz plane
void DrawCircle()
{
	double a, rad, x, y, z;
	int i;

	glBegin(GL_LINE_LOOP);
	a = 0.0;
	for(i=0;i<359;++i)
	{
		rad = (a / 180.0)*M_PI;

		x = sin(rad);
		y = 0.0;
		z = cos(rad);
		
		glVertex3d(x, y, z);

		a -= 1.0;
	}
	glEnd();
}

// generate random position and add it to the list
void OnAddRainDrop(LPARAM lParam)
{
	float x, y;
	float min_x_axis, max_x_axis, min_z_axis, max_z_axis;

	min_x_axis = -70.0f;
	max_x_axis =  70.0f;
	min_z_axis = -70.0f;
	max_z_axis =  70.0f;

    y = min_x_axis + (max_x_axis - min_x_axis) *((float)rand()/(float)RAND_MAX);
    x = min_z_axis + (max_z_axis - min_z_axis) *((float)rand()/(float)RAND_MAX);

	que.Add(x, y, 1.0f, 0.0f, true);
}

// expand the circle while fading the color
// circles that are no longer visible are removed
void OnAnimateRainDrop(LPARAM lParam)
{
	float x, z, c, s;
	bool f;
	DWORD* p = (DWORD*)lParam;
	DWORD t = *p;
	float d;

	// ripple effect
	que.MoveFirst();
	while(!que.IsEndOfQueue())
	{
		que.Read(&x, &z, &c, &s, &f);

		// circle radius expands from 0 to 4 in 2500 millisecond
		// v = d / t = 4 / 2500 = 0.0016 unit per millisecond
        // d = v * t
		d = 0.0016f * t;
		s += d;

		// color value changes from 1 to 0 in 2500 millisecond
		// v = d / t = 1 / 0.0004 =  unit per millisecond
		d = 0.0004f * t;
		c -= d;

		que.Update(c, s);

		// add second ripple
		// d = v * t
		// 0.0016 * 1240 = 1.984
		if(fabs(s - 1.984f) <= 0.001f && f)
		{
			que.Update(false);
			que.Add(x, z, 1.0f, 0.0f, false);
		}

		que.MoveNext();

	}

	// remove circle that no longer visible
	que.MoveFirst();

	if (!que.IsEndOfQueue())
	{
		que.Read(&x, &z, &c, &s, &f);
		if (c <= 0.0f) que.Remove();
	}
}

//
void OnPaint(HDC hDC)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	// camera position
	gluLookAt(200.0, 150.0, 200.0, 0.0, 0.0, 0.0,  0.0,  1.0,  0.0);

	// draw circles
	float x, z, c, s;
	bool f;

	que.MoveFirst();
	while(!que.IsEndOfQueue())
	{
		que.Read(&x, &z, &c, &s, &f);

		glPushMatrix();
		glColor3f(c, c, c);
		glTranslatef(x, 0.0f, z);
		glScalef(s, s, s);
		glCallList(circle);
		glPopMatrix();

		que.MoveNext();
	}

	SwapBuffers(hDC);
}

//
void OnCreate(HWND hWnd, HDC *hDC)
{
    int iPixelFormat;
    HGLRC hglRC;                // rendering context

    // create a pixel format
    static PIXELFORMATDESCRIPTOR pfd = {
         sizeof(PIXELFORMATDESCRIPTOR),   // size of structure.
         1,                               // version number
         PFD_DRAW_TO_WINDOW |             // support window
         PFD_SUPPORT_OPENGL |             // support OpenGl
         PFD_DOUBLEBUFFER,                // double buffered
         PFD_TYPE_RGBA,                   // RGBA type
         24,                              // 24-bit color depth
         0, 0, 0, 0, 0, 0,                // color bits ignored
         0,                               // no alpha buffer
         0,                               // shift bit ignored
         0,                               // no accumulation buffer
         0, 0, 0, 0,                      // accum bits ignored
         32,                              // 32-bit z-buffer
         0,                               // no stencil buffer
         0,                               // no auxiliary buffer
         PFD_MAIN_PLANE,                  // main layer
         0,                               // reserved
         0, 0, 0 };                       // layer masks ignored.

     *hDC = GetDC(hWnd);                                 // get the device context for our window
     iPixelFormat = ChoosePixelFormat(*hDC, &pfd);       // get the best available match of pixel format for the device context
     SetPixelFormat(*hDC, iPixelFormat, &pfd);           // make that the pixel format of the device context
     hglRC = wglCreateContext(*hDC);                     // create an OpenGL rendering context
     wglMakeCurrent(*hDC, hglRC);                        // make it the current rendering context

	 // initialize opengl parameter
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth (2.0f);

	 // create display list
	circle = glGenLists (1);
	glNewList(circle, GL_COMPILE_AND_EXECUTE);
	DrawCircle();
	glEndList();
 
	// set rainfall
	OnRainfall3(hWnd);

	// 
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_ANIMATE_PLAY,   MF_BYCOMMAND | MF_ENABLED);
	EnableMenuItem(hMenu, IDM_ANIMATE_STOP,   MF_BYCOMMAND | MF_DISABLED);

	// create a thread
	DWORD dwThreadId;
	static HWND hwnd = hWnd;
	hThread1 = CreateThread( NULL, 0, ThreadFunc1, &hwnd, CREATE_SUSPENDED, &dwThreadId);
	if (hThread1 == NULL) OutputDebugString(L"..... Create thread 1 failed.\n" );

	hThread2 = CreateThread( NULL, 0, ThreadFunc2, &hwnd, 0, &dwThreadId);
	if (hThread2 == NULL) OutputDebugString(L"..... Create thread 2 failed.\n" );
}

//
void OnDestroy(HWND hWnd, HDC hDC)
{
	// close thread
	if(hThread1 != NULL)    CloseHandle(hThread1);
	if(hThread2 != NULL)    CloseHandle(hThread2);

	//delete display list
	glDeleteLists(circle, 1);

	//
	que.RemoveAll();

	HGLRC hglRC;                // rendering context

	hglRC = wglGetCurrentContext(); // get current OpenGL rendering context
	wglMakeCurrent(hDC, NULL);      // make the rendering context not current
	wglDeleteContext(hglRC);        // delete the rendering context
	ReleaseDC(hWnd, hDC);           // releases a device context

	PostQuitMessage(0); // close the program.
}

//
void OnSize(HWND hWnd, int cx, int cy)
{
	double ox, oy, width, height;

	// ratio and proportion
	//  
	//  height     cy
	// -------- = ----
	//  width      cx
	//  

	width  = 100.0;
	height = width * ((double)cy/(double)cx);

	ox = width  / 2.0;
	oy = height / 2.0;

	left   = -ox;
	right  = left + width;
	bottom = -oy;
	top    = bottom + height;

	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, 0.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

//
void OnRainExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}

void OnAnimatePlay(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_ANIMATE_PLAY,   MF_BYCOMMAND | MF_DISABLED);
	EnableMenuItem(hMenu, IDM_ANIMATE_STOP,   MF_BYCOMMAND | MF_ENABLED);

	ResumeThread(hThread1);
}

//
void OnAnimateStop(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	EnableMenuItem(hMenu, IDM_ANIMATE_PLAY,   MF_BYCOMMAND | MF_ENABLED);
	EnableMenuItem(hMenu, IDM_ANIMATE_STOP,   MF_BYCOMMAND | MF_DISABLED);

	SuspendThread(hThread1);
}

//
void OnRainfall1(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_UNCHECKED);

	// interval in millisecond between raindrop
	interval = 1000;

}

//
void OnRainfall2(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_UNCHECKED);

	// interval in millisecond between raindrop
	interval = 500;
}

//
void OnRainfall3(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_UNCHECKED);

	// interval in millisecond between raindrop
	interval = 250;
}

//
void OnRainfall4(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_UNCHECKED);

	// interval in millisecond between raindrop
	interval = 125;
}

//
void OnRainfall5(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_UNCHECKED);

	// interval in millisecond between raindrop
	interval = 62;
}

//
void OnRainfall6(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	CheckMenuItem(hMenu,  IDM_RAINFALL_1,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_2,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_3,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_4,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_5,   MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu,  IDM_RAINFALL_6,   MF_BYCOMMAND | MF_CHECKED);

	// interval in millisecond between raindrop
	interval = 31;
}

//
