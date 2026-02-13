#define _USE_MATH_DEFINES // for C++
#include <cmath>
#define MY_TIMERID1 16
#include <afxwin.h>
#include <GL/GL.h>

class MyApp :public CWinApp
{
	virtual BOOL InitInstance();
};

MyApp myApp;

class Frame : public CFrameWnd
{
	DECLARE_MESSAGE_MAP()
	HGLRC glRC;
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
public:
	Frame() :glRC(NULL)
	{
		Create(AfxRegisterWndClass(CS_OWNDC, LoadCursor(NULL, IDC_ARROW)), L"OpenGLCube");
		SetTimer(1, MY_TIMERID1, NULL);
		angleX = 0;
		angleY = 0;
	}
	~Frame()
	{
		wglDeleteContext(glRC);
	}

	int angleX;
	int angleY;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

BEGIN_MESSAGE_MAP(Frame, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL MyApp::InitInstance()
{
	m_pMainWnd = new Frame();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

void Frame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (glRC != NULL) wglDeleteContext(glRC);

	CDC* dc = GetDC();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 32
	};
	//
	pfd.cDepthBits = 24;
	//
	int pixelFormat = ChoosePixelFormat(*dc, &pfd);
	SetPixelFormat(*dc, pixelFormat, &pfd);
	glRC = wglCreateContext(*dc);
	ReleaseDC(dc);

	Invalidate(FALSE);


}

void Frame::OnPaint()
{
	CRect rect;
	GetClientRect(&rect);

	CPaintDC dc(this);
	wglMakeCurrent(dc, glRC);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	if (rect.Height() < rect.Width())
		glViewport(rect.Width() / 2 - rect.Height() / 2, 0, rect.Height(), rect.Height());
	else glViewport(0, rect.Height() / 2 - rect.Width() / 2, rect.Width(), rect.Width());

	glCullFace(GL_BACK);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -5);
	glRotatef(angleX, 1, 0, 0); // поворот относительно оси х
	glRotatef(angleY, 0, 1, 0); // поворот относительно оси y

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 3, 7);

	//glBegin(GL_POINTS);
	glBegin(GL_TRIANGLE_STRIP);
	float R = 1.2;
	float r = 0.8;

	float du = 2 * M_PI / 360;
	float dv = 2 * M_PI / 360;

	//float du = 1 * M_PI / 360;
	//float dv = 1 * M_PI / 360;

	//float du = 2 * M_PI / 180;
	//float dv = 2 * M_PI / 180;

	//float du = 2 * M_PI / 80;
	//float dv = 2 * M_PI / 80;

	for (size_t i = 0; i < 360; i++) {

		float u = i * du;

		for (size_t j = 0; j <= 360; j++) {

			float v = (j % 360) * dv;

			for (size_t k = 0; k < 2; k++)
			{
				float uu = u + k * du;
				// compute vertex
				float x = (R + r * cos(v)) * cos(uu);
				float y = (R + r * cos(v)) * sin(uu);
				float z = r * sin(v);

				//glColor3d(255, 0, 127);
				//glColor3d(0, 127, 255);
				glColor3d(x, y, z);
				glVertex3d(x, y, z);
			}
			// incr angle
			v += dv;
		}
	}


	glEnd();

	glFlush();
	SwapBuffers(dc);
	wglMakeCurrent(NULL, NULL);
}

void Frame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		angleY -= 3;
		break;
	case VK_RIGHT:
		angleY += 3;
		break;
	case VK_DOWN:
		angleX += 3;
		break;
	case VK_UP:
		angleX -= 3;
		break;
	case 'P':
		KillTimer(1);
		break;
	case 'S':
		SetTimer(1, MY_TIMERID1, NULL);
		break;
	}
	Invalidate(0);

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Frame::OnTimer(UINT_PTR nIDEvent)
{
	angleX += 1;
	angleY += 1;
	Invalidate(0);
	CFrameWnd::OnTimer(nIDEvent);
}