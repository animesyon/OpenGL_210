//    
//    Draw two triangles using index buffer.
//       
#include "stdafx.h"
#include "square.h"
#include "extensions.h"
#include "shader.h"
#include "matrix4.h"
#include "model.h"

#define MAX_LOADSTRING 100

// variables

TCHAR szTitle[MAX_LOADSTRING];       // the title bar text
TCHAR szWindowClass[MAX_LOADSTRING]; // the window class name

CShader shader;
CMatrix4 proj;
CModel model;

// functions
LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC *hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(HWND hWnd, int cx, int cy);

void OnFileExit(HWND hWnd);

// main program
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    WNDCLASSEX wcex;
    MSG msg;
    HACCEL hAccelTable;
    HWND hWnd;

    // initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_SQUARE, szWindowClass, MAX_LOADSTRING);

    // registers the window class
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SQUARE));
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassEx(&wcex);

    int X, Y, nWidth, nHeight, Cx, Cy;

    // 480p: 854x480

    Cx = 854;
    Cy = 480;

    nWidth  = Cx + 16;
    nHeight = Cy + 58;

    X = (GetSystemMetrics(SM_CXSCREEN) - nWidth)/2;
    Y = (GetSystemMetrics(SM_CYSCREEN) - nHeight)/20;

    // creates window
    hWnd = CreateWindowEx(NULL,                                                                             // The extended window style.
                        szWindowClass,                                                                      // window class name
                        szTitle,                                                                            // window name
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,  // window style
                        X, Y,                                                                               // window x, y coordinate.
                        nWidth, nHeight,                                                                    // window width and height
                        NULL,                                                                               // handle to parent window
                        NULL,                                                                               // menu.
                        hInstance,                                                                          // handle to app instance.
                        NULL);                                                                              // pointer to window creation data.

    if(!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SQUARE));

    // main message loop:
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

// processes messages for the main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hDC;

    switch (message)
	{

	case WM_COMMAND:        // Parse the menu selections
        switch (LOWORD(wParam))
		{
		case IDM_EXIT:    OnFileExit(hWnd);               break;
		default:
            return DefWindowProc(hWnd, message, wParam, lParam);
		}
    break;

	case WM_PAINT:   OnPaint(hDC);                                   break;
	case WM_CREATE:  OnCreate(hWnd, &hDC);                           break;
	case WM_DESTROY: OnDestroy(hWnd, hDC);                           break;
	case WM_SIZE:    OnSize(hWnd, LOWORD (lParam), HIWORD (lParam)); break;
	default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}

    return 0;
}

// render onto a screen
void OnPaint(HDC hDC)
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 model.Draw(proj);

     SwapBuffers(hDC);
}

// initialize opengl
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

     // check the extensions and load OpenGL functions
     char szText[100];
     if(!LoadFunctions())
	 {
	      MessageBox(NULL, L"shader not supported!", L"OpenGL Extensions", MB_OK);
	      PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		  return;
	 }

	 OutputDebugStringA("-----------------------------------------------------------------------------\n");
     sprintf_s(szText,100,"OpenGL Version :%s\n",glGetString(GL_VERSION));   OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLES Version   :%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLU Version    :%s\n",glGetString(GLU_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"Vendor         :%s\n",glGetString(GL_VENDOR));    OutputDebugStringA(szText);
     sprintf_s(szText,100,"Renderer       :%s\n",glGetString(GL_RENDERER));  OutputDebugStringA(szText);
     OutputDebugStringA("-----------------------------------------------------------------------------\n");


     // create shader
     if(!shader.Create(L"vertex.txt", L"fragment.txt"))
	 {
		 MessageBoxA(NULL, shader.infolog, shader.caption, MB_OK);
	     PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		 return;
	 }

	 // create model
	 model.Create(50.0f, 25.0f);
	 model.PutHandle(shader.GetHandle());

     //
     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);     // clear window to black
	 glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // draw outline
	 glEnable(GL_DEPTH_TEST);                  // depth testing
}

// clean-up
void OnDestroy(HWND hWnd, HDC hDC)
{
    HGLRC hglRC;                // rendering context

     shader.Destroy();
	 model.Destroy();

     hglRC = wglGetCurrentContext(); // get current OpenGL rendering context
     wglMakeCurrent(hDC, NULL);      // make the rendering context not current
     wglDeleteContext(hglRC);        // delete the rendering context
     ReleaseDC(hWnd, hDC);           // releases a device context

     PostQuitMessage(0);             // close the program
}

// resize window
void OnSize(HWND hWnd, int cx, int cy)
{
     if(cx==0) cx = 1;
     if(cy==0) cy = 1;

     // set up the coordinate system for drawing so that
	 // the lower-left corner is (-28.103, -50.0), and the upper-right corner is (28.103, 50.0)
     glViewport(0, 0, cx, cy);
	 proj.Orthographic(-50.0f,    50.0f,   -28.103f,    28.103f,    0.0f,     1.0f);
}

// close program
void OnFileExit(HWND hWnd)
{
     DestroyWindow(hWnd);
}

//
