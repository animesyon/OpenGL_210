// 
//   Up Arrow Key     - move forward
//   Down Arrow Key   - move backward
//   Right Arrow Key  - rotate right
//   Left Arrow Key   - rotate left
//   S                - strafe left
//   D                - strafe right
//
//   To adjust how fast or slow the camera move
//   change the parameter value on function MoveCamera
//
//
#include "stdafx.h"
#include "firstpersonview.h"
#include "camera.h"
#include "hplane.h"
#include "vplane.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// light position
GLfloat lit_Pos[] = {0.0f, 0.0f, 0.0f, 1.0f};

// material
GLfloat mat_ambient[]   = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat mat_diffuse[]   = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat mat_specular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_shininess[] = { 0.0f };
GLfloat mat_emission[]  = {0.0f,0.0f,0.0f, 1.0f};

//
CCamera cam;
GLuint terrain, structure;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void DrawStructure();

void MoveCamera();

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC *hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(HWND hWnd, int cx, int cy);

void OnFileExit(HWND hWnd);

void OnViewWireframe(HWND hWnd);
void OnViewSolid(HWND hWnd);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FIRSTPERSONVIEW, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIRSTPERSONVIEW));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_FIRSTPERSONVIEW);
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

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIRSTPERSONVIEW));

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
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
			case IDM_EXIT:       OnFileExit(hWnd);		break;

			case IDM_WIREFRAME:  OnViewWireframe(hWnd);     break;
			case IDM_SOLID:		 OnViewSolid(hWnd);         break;

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

//
void DrawStructure()
{
	FILE *fp;
	errno_t err;
	unsigned int i, j, n, m;
	float v[2];
	CVPlane plane;
	float x1, z1, x2, z2, x0, z0;
	float h = 3.1f;

	// open file
	err = _wfopen_s(&fp, L"data._xy", L"rb");
	if(err != 0 ) return;

	// number of polygon
	fread(&n, sizeof(unsigned int), 1, fp);

	for(i=0;i<n;++i)
	{
		// number of vertices in a polygon
		fread(&m, sizeof(unsigned int), 1, fp);

		// read vertices
		fread(v, sizeof(float), 2, fp);

		x1 = x0 = 0.3f * v[0];
		z1 = z0 = 0.3f * v[1];

		for (j = 0; j < (m-1); ++j)
		{
			fread(v, sizeof(float), 2, fp);

			x2 = 0.3f * v[0];
			z2 = 0.3f * v[1];

			// create vertical plane
			plane.Create(x1, z1, x2, z2, h);
			plane.Draw();

			x1 = x2;
			z1 = z2;
		}

		// create vertical plane
		plane.Create(x1, z1, x0, z0, h);
		plane.Draw();

	}

	// close file
	fclose(fp);

}

// to speed up or slow down the movement
// change the function parameter value
void MoveCamera()
{
	if (GetKeyState(VK_UP) & 0x80)    cam.MoveForward(0.05);
	if (GetKeyState(VK_DOWN) & 0x80)  cam.MoveBackward(0.05);
	if (GetKeyState(VK_LEFT) & 0x80)  cam.RotateLeft(0.5);
	if (GetKeyState(VK_RIGHT) & 0x80) cam.RotateRight(0.5);
	if (GetKeyState('S') & 0x80)      cam.StrafeLeft(0.05);
	if (GetKeyState('D') & 0x80)      cam.StrafeRight(0.05);
}

// paint a portion of an application's window
void OnPaint(HDC hDC)
{
	// move camera
	MoveCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	glLightfv(GL_LIGHT0, GL_POSITION, lit_Pos);
	gluLookAt (cam.eyex, cam.eyey, cam.eyez, cam.centerx, cam.centery, cam.centerz, cam.upx, cam.upy, cam.upz);
	
	// terrain
	mat_ambient[0] = 1.0f; mat_ambient[1] = 1.0f; mat_ambient[2] = 0.8f;
	mat_diffuse[0] = 1.0f; mat_diffuse[1] = 1.0f; mat_diffuse[2] = 0.8f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glCallList(terrain);

	// structure
	mat_ambient[0] = 0.2f; mat_ambient[1] = 0.2f; mat_ambient[2] = 0.2f;
	mat_diffuse[0] = 0.4f; mat_diffuse[1] = 0.4f; mat_diffuse[2] = 1.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glCallList(structure);

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

	 // clear window to black
	 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	 glEnable(GL_DEPTH_TEST);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);

	 // set camera inital position
	 cam.SetPosition(8.710157, 1.6, 29.511346,       8.335551,  1.6, 28.584162,       0.0,  1.0, 0.0);

	 // set lightning
	GLfloat g_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };

	GLfloat lit_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lit_diffuse[]  = { 1.0f,1.0f,1.0f, 1.0f };
	GLfloat lit_specular[] = { 1.0f,1.0f,1.0f, 1.0f};

	GLfloat attenuation[] = {0.005f, 0.04f, 0.24f};

	GLfloat spot_cutoff      = 180.0f;
	GLfloat spot_direction[] = { 0.0f, 0.1f, 1.0f };
	GLfloat spot_exponent    = 0.0f;

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,g_ambient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lit_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR,lit_specular);

	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attenuation[0]);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, attenuation[1]);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attenuation[2]);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);

	// set material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	// terrain
	 CHPlane plane;
	 plane.Create(-30.0, 30.0f, -30.0f, 30.0f);
	 terrain = glGenLists(1);
	 glNewList(terrain, GL_COMPILE_AND_EXECUTE);
	 plane.Draw();
	 glEndList();

	 // structure
	 structure = glGenLists(1);
	 glNewList(structure, GL_COMPILE_AND_EXECUTE);
	 DrawStructure();
	 glEndList();

}

//
void OnDestroy(HWND hWnd, HDC hDC)
{
	glDeleteLists(structure, 1);
	glDeleteLists(terrain, 1);

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
	double fovy, aspect, zNear, zFar;

	fovy   = 45.0;
	aspect = (double)cx / (double)cy;
	zNear  = 0.1;
	zFar   = 1000.0;

	glViewport(0, 0, cx, cy);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(fovy, aspect, zNear, zFar);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

//
void OnFileExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}

//
void OnViewWireframe(HWND hWnd)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

//
void OnViewSolid(HWND hWnd)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
