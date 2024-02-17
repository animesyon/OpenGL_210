// 
//     Game and Watch - Chef
//
//     Press Enter to start the game
//     Press Space to stop the game
//     Press the Left or Right Arrow Key to move the chef
//

#include "stdafx.h"
#include "game_n_watch.h"
#include "extensions.h"
#include "matrix4.h"
#include "shader.h"
#include "pngfile.h"
#include "kitchen.h"
#include "cat.h"
#include "chef.h"
#include "food.h"
#include "mouse.h"
#include "miss.h"
#include "score.h"
#include "audev.h"

#define MAX_LOADSTRING 100
#define MAX_FOOD         3

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//
HANDLE hThread;
HANDLE hStopEvent;
DWORD dwThreadId;
bool gameinprogress;

//
GLuint textureID;
CMatrix4 proj;
CPngFile png;
CShader shader;
CKitchen kitchen;
CCat cat;
CChef chef;
CFood food[MAX_FOOD];
CMouse mouse;
CMiss miss;
CScore score;
CAuDev audev;
int sndmiss, sndhit, sndtick;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI	 ThreadFunc(LPVOID lpParam);

int GenRandomHeight();
void RandomCatMove();

void OnStart();
void OnStop();

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC *hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(HWND hWnd, int cx, int cy);

// main program
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMENWATCH, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMENWATCH));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

    // save the instance handle in a global variable
	hInst = hInstance;

	int X, Y, nWidth, nHeight, Cx, Cy;

	// 480p: 854x480
	Cx = 854;
    Cy = 480;

    nWidth  = Cx + 16;
    nHeight = Cy + 38;

    X =(GetSystemMetrics(SM_CXSCREEN) - nWidth)/2;
    Y = (GetSystemMetrics(SM_CYSCREEN) - nHeight)/3;

	// create the main program window
	HWND hWnd = CreateWindowW(szWindowClass, szTitle,
							  WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		                      X, Y,
							  nWidth, nHeight,
		                      nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return FALSE;

	// display the main program window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMENWATCH));

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
    case MM_WOM_DONE:  audev.OnWaveOutDone(hWnd, (HWAVEOUT)wParam, (LPWAVEHDR)lParam);   break;
    case MM_WOM_OPEN:  audev.OnWaveOutOpen(hWnd, (HWAVEOUT)wParam, (LPWAVEHDR)lParam);   break;
    case MM_WOM_CLOSE: audev.OnWaveOutClose(hWnd, (HWAVEOUT)wParam, (LPWAVEHDR)lParam);  break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_LEFT:   chef.MoveLeft();  break;
			case VK_RIGHT:  chef.MoveRight(); break;
			case VK_RETURN: OnStart();		  break;
			case VK_SPACE:  OnStop();		  break;
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

//
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	char szText[100];
	sprintf_s(szText, 100, "THREAD (0x%X) HAS STARTED.\n", dwThreadId);
	OutputDebugStringA(szText);

	// prevent the game from restarting
	// when game in progress
	gameinprogress = true;

	DWORD dwWaitResult;
	int i, j, k, s;
	DWORD t1, t2, t;

	// generate a different sequence every time we play the game
	DWORD seed = GetTickCount();
	srand(seed);
	miss = 0;
	score = 0;

	for (i = 0; i < 3; i++)
	{
		k = 0;

		// initially set max height for every food
		food[0].SetMaxHeight(GenRandomHeight());
		food[1].SetMaxHeight(GenRandomHeight());
		food[2].SetMaxHeight(GenRandomHeight());

		// all chefs ready to flip foods
		food[0].PutOnPan();
		food[1].PutOnPan();
		food[2].PutOnPan();
		chef.AllIdle();
		cat.Idle();

		// delay
		Sleep(500);

		// all chefs flip foods
		food[0].Move();
		food[1].Move();
		food[2].Move();
		chef.AllFlip();

		// delay
		Sleep(300);

		// one chef stay
		chef.OneStay();

		// delay
		Sleep(300);

		// use this time to get the delay
		t1 = GetTickCount();

		// loop
		j = 1;    s = 0;
		while (TRUE)
		{
			// delay
			t2 = GetTickCount();
			t = t2 - t1;

			if (t >= 250)
			{
				// play sound
				audev.Play(sndtick);

				// move food one at a time
				++j;
				if (j == MAX_FOOD) j = s;
				food[j].Move();

				// a cat often pokes the left piece of food
				RandomCatMove();

				if (cat.CanPokeHam() && food[0].CanBePoked())
				{
					s = 1;
					food[0].SetToFallDown();
				}
				else
				{
					s = 0;
				}

				// when food reaches a max height
				// it moves downward
				if (food[j].ReachMaxHeight()) food[j].Toggle();

				// use this time to get the delay
				t1 = t2;
			}



			// chef flips the food
			if (food[j].CanBeFlipped() && chef.CanFlipFood(j))
			{
				// play sound
				audev.Play(sndhit);

				food[j].Toggle();

				// delay
				Sleep(20);

				// chef flips food
				chef.Flip();
				food[j].Move();

				// increment score
				++score;

				// delay
				Sleep(20);

				chef.Idle();

				// set new max height
				food[j].SetMaxHeight(GenRandomHeight());
			}

			// food fall on the floor
			// mouse steal the food off the floor
			if (food[j].FallOnFloor())
			{
				k = j+1;
				break;
			}

			// break the loop
			dwWaitResult = WaitForSingleObject(hStopEvent, 0);
			if (dwWaitResult == WAIT_OBJECT_0) break;
		}

		// play sound
		audev.Play(sndmiss);

		// mouse catch the food
		mouse.Move(k);

		// delay
		Sleep(500);

		// mouse run
		for (j = k + 1; j < mouse.GetMoveCount(); j++)
		{
			// play sound
			audev.Play(sndmiss);

			// mouse run
			mouse.Move();

			// delay
			Sleep(200);
		}
	
		// delay
		Sleep(300);

		// play sound
		audev.Play(sndmiss);

		// mouse go back
		mouse.Move(0);
		++miss;

		// break the loop
		dwWaitResult = WaitForSingleObject(hStopEvent, 0);
		if (dwWaitResult == WAIT_OBJECT_0) break;
	}

	// prevent the game from restarting
	// when game in progress
	gameinprogress = false;

	sprintf_s(szText,100,"THREAD (0x%X) HAS EXITED.\n", dwThreadId); 
	OutputDebugStringA(szText);

	return 0;
}

// any number from 0 to 2
//
//      0   <---
//      1   <---
//      2   <---
//      3
//      4
//      5
//
int GenRandomHeight()
{
	int n = rand() % 3;

	return n;
}

// cat poke food at random
void RandomCatMove()
{
	DWORD p = rand() % 100;

	switch (p)
	{
	case 0:
	case 74:
		cat.Poke();
		break;
	case 75:
	case 99:
		cat.Idle();
		break;
	}
}

//
void OnStart()
{
	// prevent the game from restarting
	// when game in progress
	if (gameinprogress) return;

	// start thread
	if(hThread != NULL)    CloseHandle(hThread);

	ResetEvent(hStopEvent);

	hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dwThreadId);                
   if (hThread == NULL) OutputDebugString(L"CreateThread failed.\n" ); 
}

//
void OnStop()
{
	// stop thread
	SetEvent(hStopEvent);
}

//
void OnPaint(HDC hDC)
{
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	kitchen.Draw(proj);
	cat.Draw(proj);
	chef.Draw(proj);
	food[0].Draw(proj);
	food[1].Draw(proj);
	food[2].Draw(proj);
	mouse.Draw(proj);
	miss.Draw(proj);
	score.Draw(proj);

	// Swap buffers to make it visible on screen
	SwapBuffers(hDC);
}

//
void OnCreate(HWND hWnd, HDC *hDC)
{
	// set handle to null
	hThread    = NULL;
	hStopEvent = NULL;

	gameinprogress = false;

	// create event
    hStopEvent = CreateEvent(NULL, TRUE, FALSE, L"bouncestopevent");
    if (hStopEvent == NULL) 
    {
        OutputDebugString(L"CreateEvent hEventStop failed.\n");
    }

	//
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

     // check the extensions
     char szText[100];
	 OutputDebugStringA("-----------------------------------------------------------------------------\n");
     sprintf_s(szText,100,"OpenGL Version :%s\n",glGetString(GL_VERSION));   OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLES Version   :%s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"GLU Version    :%s\n",glGetString(GLU_VERSION));  OutputDebugStringA(szText);
     sprintf_s(szText,100,"Vendor         :%s\n",glGetString(GL_VENDOR));    OutputDebugStringA(szText);
     sprintf_s(szText,100,"Renderer       :%s\n",glGetString(GL_RENDERER));  OutputDebugStringA(szText);
     OutputDebugStringA("-----------------------------------------------------------------------------\n");

     // check the extensions
     if(!LoadFunctions())
     {
	      MessageBox(NULL, L"shader is not supported!", L"OpenGL Extensions", MB_OK);
	      PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		  return;
     }

	 // get current directory
	 wchar_t szPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, szPath);

	// load sound
	wchar_t sndFile[MAX_PATH];

	wcscpy_s(sndFile, MAX_PATH, szPath);
	wcscat_s(sndFile, MAX_PATH, L"\\sound\\miss.wav");

	OutputDebugString(sndFile);
	OutputDebugString(L"\n");

	sndmiss = audev.Load(hWnd, sndFile);

	// load sound
	wcscpy_s(sndFile, MAX_PATH, szPath);
	wcscat_s(sndFile, MAX_PATH, L"\\sound\\hit.wav");

	OutputDebugString(sndFile);
	OutputDebugString(L"\n");

	sndhit = audev.Load(sndFile);

	// load sound
	wcscpy_s(sndFile, MAX_PATH, szPath);
	wcscat_s(sndFile, MAX_PATH, L"\\sound\\tick.wav");

	OutputDebugString(sndFile);
	OutputDebugString(L"\n");

	sndtick = audev.Load(sndFile);

	 // get shader filename
	wchar_t szVertFile[MAX_PATH], szFragFile[MAX_PATH];

	wcscpy_s(szVertFile, MAX_PATH, szPath);
	wcscat_s(szVertFile, MAX_PATH, L"\\shader\\vertex.txt");

	wcscpy_s(szFragFile, MAX_PATH, szPath);
	wcscat_s(szFragFile, MAX_PATH, L"\\shader\\fragment.txt");

	OutputDebugString(szVertFile);
	OutputDebugString(L"\n");

	OutputDebugString(szFragFile);
	OutputDebugString(L"\n");

     OutputDebugStringA("-----------------------------------------------------------------------------\n");

     // create shader
     if(!shader.Create( szVertFile, szFragFile))
     {
		 MessageBoxA(NULL, shader.infolog, shader.caption, MB_OK);
	     PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		 return;
     }

	 // get texture image
	 wchar_t szPngFile[MAX_PATH];

	wcscpy_s(szPngFile, MAX_PATH, szPath);
	wcscat_s(szPngFile, MAX_PATH, L"\\texture\\texture.png");

	OutputDebugString(szPngFile);
	OutputDebugString(L"\n");

    OutputDebugStringA("-----------------------------------------------------------------------------\n");

	if(!png.Open(szPngFile))
	{
		MessageBox(NULL, L"error reading png file", L"Texture", MB_OK);
	     PostMessage(hWnd, WM_COMMAND, IDM_EXIT, 0);
		return;
	}

	// kitchen
	 kitchen.Create(-5.0f, png.width, png.height);
	 kitchen.SetShader(shader.GetHandle());

	 // cat
	 cat.Create(-4.0f, png.width, png.height);
	 cat.SetShader(shader.GetHandle());

	 // cat
	 chef.Create(-3.0f, png.width, png.height);
	 chef.SetShader(shader.GetHandle());

	 // ham
	 food[0].Create(-2.0f, 64.0f, 114.0f, 98.0f, 81.0f, 64.0f, 54.0f,
		 272.0f, 292.0f, 120.0f, 140.0f, 155.0f, 173.0f, 190.0f, 200.0f, png.width, png.height);
	 food[0].SetShader(shader.GetHandle());

	 // sausage
	 food[1].Create(-2.0f, 108.0f, 115.0f, 99.0f, 88.0f, 69.0f, 55.0f,
		 315.0f, 341.0f, 120.0f, 139.0f, 153.0f, 167.0f, 186.0f, 200.0f, png.width, png.height);
	 food[1].SetShader(shader.GetHandle());

	 // fish
	 food[2].Create(-2.0f, 155.0f, 114.0f, 98.0f, 81.0f, 64.0f, 54.0f,
		 362.0f, 387.0f, 120.0f, 139.0f, 155.0f, 171.0f, 190.0f, 200.0f, png.width, png.height);
	 food[2].SetShader(shader.GetHandle());

	 // mouse
	 mouse.Create(-1.0f, png.width, png.height);
	 mouse.SetShader(shader.GetHandle());

	 // miss
	 miss.Create(-1.0f, png.width, png.height);
	 miss.SetShader(shader.GetHandle());
	 miss = 0;

	 // score
	 score.Create(-1.0f, png.width, png.height);
	 score.SetShader(shader.GetHandle());

	// initalize opengl
	 float r, g, b;
	 r = g = b = 192.0f / 255.0f;

	glClearColor(r, g, b, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set up texture

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch(png.color_type)
	{
	case PNG_COLOR_TYPE_RGB:       gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,  png.width, png.height, GL_RGB,  GL_UNSIGNED_BYTE, png.buffer); break;
	case PNG_COLOR_TYPE_RGB_ALPHA: gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, png.width, png.height, GL_RGBA, GL_UNSIGNED_BYTE, png.buffer); break;
	}
}

//
void OnDestroy(HWND hWnd, HDC hDC)
{
	//
	if(hThread != NULL)    CloseHandle(hThread);
	if(hStopEvent != NULL) CloseHandle(hStopEvent);

	//
	audev.Close();

	//
	glDeleteTextures(1, &textureID);

	// release resources
    shader.Destroy();
	kitchen.Destroy();
	cat.Destroy();
	chef.Destroy();
	food[0].Destroy();
	food[1].Destroy();
	food[2].Destroy();
	mouse.Destroy();
	miss.Destroy();
	score.Destroy();

	HGLRC hglRC;					// rendering context

	hglRC = wglGetCurrentContext(); // get current OpenGL rendering context
	wglMakeCurrent(hDC, NULL);      // make the rendering context not current
	wglDeleteContext(hglRC);        // delete the rendering context
	ReleaseDC(hWnd, hDC);           // releases a device context

	PostQuitMessage(0);             // close the program
}

//
void OnSize(HWND hWnd, int cx, int cy)
{
     if(cx==0) cx = 1;
     if(cy==0) cy = 1;

	 float left, right, bottom, top, znear, zfar;
	 float height, width, kitchen_width, kitchen_height;
	 float x, y;

	 // fit the kitchen inside the window
	 kitchen_width  = 256.0f;
	 kitchen_height = 172.0f;

	 height = kitchen_height;
	 width  = height * ((float)cx / (float)cy);

	 x = (width - kitchen_width) / 2.0f;
	 y = 0.0f;

	 if (width < kitchen_width)
	 {
		 width  = kitchen_width;
		 height = width * ((float)cy / (float)cx);

		 x = 0.0f;
		 y = (height - kitchen_height) / 2.0f;
	 }

	left   = -x;
	right  = left + width;
	bottom = -y;
	top    = bottom + height;
	znear  = 0.0f;
	zfar   = 5.0f;

	// set the coordinate of the screen
	glViewport(0, 0, cx, cy);
	proj.Orthographic(left, right, bottom, top, znear, zfar);
}

//
