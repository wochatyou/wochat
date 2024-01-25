#include "pch.h"

#ifndef __cplusplus
#error Wochat requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
#error xwindow.h requires atlbase.h to be included first
#endif

#ifndef __ATLWIN_H__
#error xwindow.h requires atlwin.h to be included first
#endif

#if !defined(_WIN64)
#error WoChat App can only compiled in X64 mode
#endif

#include "resource.h"
#include "wochat.h"
#include "xbitmapdata.h"

#include "win0.h"

#define DECLARE_XWND_CLASS(WndClassName, uIcon, uIconSmall) \
static ATL::CWndClassInfo& GetWndClassInfo() \
{ \
	static ATL::CWndClassInfo wc = \
	{ \
		{ sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, StartWindowProc, \
		  0, 0, NULL, NULL, NULL, (HBRUSH)(COLOR_WINDOW + 1), NULL, WndClassName, NULL }, \
		NULL, NULL, IDC_ARROW, TRUE, 0, _T("") \
	}; \
	if (0 != uIcon) \
		wc.m_wc.hIcon = LoadIcon(ATL::_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(uIcon)); \
	if (0 != uIconSmall) \
		wc.m_wc.hIconSm = LoadIcon(ATL::_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(uIconSmall)); \
	return wc; \
}

#define MESSAGE_HANDLER_DUIWINDOW(msg, func) \
	if(true) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

template <class T>
void SafeRelease(T** ppT)
{
	if (nullptr != *ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

////////////////////////////////////////////////////////////////
// global variables
////////////////////////////////////////////////////////////////
LONG 				g_threadCount = 0;
UINT				g_Quit = 0;
HINSTANCE			g_hInstance = nullptr;
ID2D1Factory*		g_pD2DFactory = nullptr;
IDWriteFactory*		g_pDWriteFactory = nullptr;
HCURSOR				g_hCursorWE = nullptr;
HCURSOR				g_hCursorNS = nullptr;
HCURSOR				g_hCursorHand = nullptr;
HCURSOR				g_hCursorIBeam = nullptr;
wchar_t				g_AppPath[MAX_PATH + 1] = {0};
// private key and public key
U8					g_SK[32] = { 0 };
U8					g_PK[33] = { 0 };

CAtlWinModule _Module;

/************************************************************************************************
*  The layout of the Main Window
*
* +------+------------------------+-------------------------------------------------------------+
* |      |         Win1           |                       Win3                                  |
* |      +------------------------+-------------------------------------------------------------+
* |      |                        |                                                             |
* |      |                        |                                                             |
* |      |                        |                       Win4                                  |
* | Win0 |                        |                                                             |
* |      |         Win2           |                                                             |
* |      |                        +-------------------------------------------------------------+
* |      |                        |                                                             |
* |      |                        |                       Win5                                  |
* |      |                        |                                                             |
* +------+------------------------+-------------------------------------------------------------+
*
*
* We have one vertical bar and three horizonal bars.
*
*************************************************************************************************
*/

class XWindow : public ATL::CWindowImpl<XWindow>
{
public:
	DECLARE_XWND_CLASS(NULL, IDR_MAINFRAME, 0)

	BEGIN_MSG_MAP(XWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL bHandled)
	{
		SafeRelease(&m_pixelBitmap);
		SafeRelease(&m_pD2DRenderTarget);
		PostQuitMessage(0);
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SafeRelease(&m_pD2DRenderTarget);
		return 0;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		HRESULT hr = S_OK;
		RECT rc;
		PAINTSTRUCT ps;
		BeginPaint(&ps);

		GetClientRect(&rc);

		if (nullptr == m_pD2DRenderTarget)
		{
			D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			);

			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
			renderTargetProperties.dpiX = 96;
			renderTargetProperties.dpiY = 96;
			renderTargetProperties.pixelFormat = pixelFormat;

			D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTragetproperties
				= D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right, rc.bottom));

			hr = g_pD2DFactory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTragetproperties, &m_pD2DRenderTarget);
			if (S_OK == hr && nullptr != m_pD2DRenderTarget)
			{
				unsigned int pixel[1] = { 0xFF0000FF };
				SafeRelease(&m_pixelBitmap);
				hr = m_pD2DRenderTarget->CreateBitmap(
					D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
					&m_pixelBitmap);
			}
		}

		if (S_OK == hr && nullptr != m_pD2DRenderTarget)
		{
			m_pD2DRenderTarget->BeginDraw();
			m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
			if (nullptr != m_pixelBitmap)
			{
				D2D1_RECT_F rect = D2D1::RectF(
					static_cast<FLOAT>(rc.left),
					static_cast<FLOAT>((rc.bottom - 10) / 2),
					static_cast<FLOAT>(rc.right),
					static_cast<FLOAT>((rc.bottom - 10) / 2 + 10)
				);
				m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap, &rect);
			}
			hr = m_pD2DRenderTarget->EndDraw();
			if (FAILED(hr) || D2DERR_RECREATE_TARGET == hr)
			{
				SafeRelease(&m_pD2DRenderTarget);
			}
		}

		EndPaint(&ps);
		return 0;
	}

private:
	enum
	{
		STEPXY = 1,
		SPLITLINE_WIDTH = 1
	};

	enum class DrapMode { dragModeNone, dragModeV, dragModeH };
	DrapMode m_dragMode = DrapMode::dragModeNone;

	U32* m_screenBuff = nullptr;
	U32  m_screenSize = 0;

	RECT m_rectClient = { 0 };

	int m_splitterVPos = -1;               // splitter bar position
	int m_splitterVPosNew = -1;            // internal - new position while moving
	int m_splitterVPosOld = -1;            // keep align value

	int m_splitterHPos = -1;
	int m_splitterHPosNew = -1;
	int m_splitterHPosOld = -1;

	int m_cxDragOffset = 0;		// internal
	int m_cyDragOffset = 0;		// internal

	int m_splitterVPosToLeft = 300;       // the minum pixel to the left of the client area.
	int m_splitterVPosToRight = 400;       // the minum pixel to the right of the client area.
	int m_splitterHPosToTop = (XWIN3_HEIGHT + XWIN4_HEIGHT);        // the minum pixel to the top of the client area.
	int m_splitterHPosToBottom = XWIN5_HEIGHT;       // the minum pixel to the right of the client area.

	int m_marginLeft = 64;
	int m_marginRight = 0;

	int m_splitterHPosfix0 = XWIN1_HEIGHT;
	int m_splitterHPosfix1 = XWIN3_HEIGHT;

	UINT m_nDPI = 96;

	XWindow0 m_win0;
private:
	ID2D1HwndRenderTarget* m_pD2DRenderTarget = nullptr;
	ID2D1Bitmap* m_pixelBitmap = nullptr;

};

class CWoChatThreadManager
{
public:
	// the main UI thread proc
	static DWORD WINAPI RunThread(LPVOID lpData)
	{
		DWORD dwRet = 0;
		RECT rw = { 0 };
		MSG msg = { 0 };

		InterlockedIncrement(&g_threadCount);

		XWindow winMain;

		rw.left = 100; rw.top = 100; rw.right = rw.left + 1024; rw.bottom = rw.top + 768;
		winMain.Create(NULL, rw, _T("WoChat"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE);
		if (FALSE == winMain.IsWindow())
			goto ExitMainUIThread;

		//winMain.CenterWindow();
		winMain.ShowWindow(SW_SHOW);

		// Main message loop:
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			if (!TranslateAcceleratorW(msg.hwnd, nullptr, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}

	ExitMainUIThread:
		InterlockedDecrement(&g_threadCount);
		return dwRet;
	}

	DWORD m_dwCount = 0;
	HANDLE m_arrThreadHandles[MAXIMUM_WAIT_OBJECTS - 1];

	CWoChatThreadManager()
	{
		for (int i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++)
			m_arrThreadHandles[i] = nullptr;
	}

	// Operations
	DWORD AddThread(LPTSTR lpstrCmdLine, int nCmdShow)
	{
		if (m_dwCount == (MAXIMUM_WAIT_OBJECTS - 1))
		{
			::MessageBox(NULL, _T("ERROR: Cannot create ANY MORE threads!!!"), _T("WoChat"), MB_OK);
			return 0;
		}

		DWORD dwThreadID;
		HANDLE hThread = ::CreateThread(NULL, 0, RunThread, nullptr, 0, &dwThreadID);
		if (hThread == NULL)
		{
			::MessageBox(NULL, _T("ERROR: Cannot create thread!!!"), _T("WoChat"), MB_OK);
			return 0;
		}

		m_arrThreadHandles[m_dwCount] = hThread;
		m_dwCount++;
		return dwThreadID;
	}

	void RemoveThread(DWORD dwIndex)
	{
		::CloseHandle(m_arrThreadHandles[dwIndex]);
		if (dwIndex != (m_dwCount - 1))
			m_arrThreadHandles[dwIndex] = m_arrThreadHandles[m_dwCount - 1];
		m_dwCount--;
	}

	int Run(LPTSTR lpstrCmdLine, int nCmdShow)
	{
		MSG msg;
		// force message queue to be created
		::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

		AddThread(lpstrCmdLine, nCmdShow);

		int nRet = m_dwCount;
		DWORD dwRet;
		while (m_dwCount > 0)
		{
			dwRet = ::MsgWaitForMultipleObjects(m_dwCount, m_arrThreadHandles, FALSE, INFINITE, QS_ALLINPUT);

			if (dwRet == 0xFFFFFFFF)
			{
				::MessageBox(NULL, _T("ERROR: Wait for multiple objects failed!!!"), _T("WoChat"), MB_OK);
			}
			else if (dwRet >= WAIT_OBJECT_0 && dwRet <= (WAIT_OBJECT_0 + m_dwCount - 1))
			{
				RemoveThread(dwRet - WAIT_OBJECT_0);
			}
			else if (dwRet == (WAIT_OBJECT_0 + m_dwCount))
			{
				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_USER)
						AddThread(nullptr, SW_SHOWNORMAL);
				}
			}
			else
			{
				::MessageBeep((UINT)-1);
			}
		}

		return nRet;
	}
};

static int InitInstance(HINSTANCE hInstance)
{
	int iRet = 0;
	DWORD length = 0;
	HRESULT hr = S_OK;

	g_hCursorWE = ::LoadCursor(NULL, IDC_SIZEWE);
	g_hCursorNS = ::LoadCursor(NULL, IDC_SIZENS);
	g_hCursorHand = ::LoadCursor(nullptr, IDC_HAND);
	g_hCursorIBeam = ::LoadCursor(NULL, IDC_IBEAM);
	if (NULL == g_hCursorWE || NULL == g_hCursorNS || NULL == g_hCursorHand || NULL == g_hCursorIBeam)
	{
		MessageBox(NULL, _T("The calling of LoadCursor() is failed"), _T("WoChat Error"), MB_OK);
		return 1;
	}

	// Initialize Direct2D and DirectWrite
	g_pD2DFactory = nullptr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
	if (S_OK != hr || nullptr == g_pD2DFactory)
	{
		MessageBox(NULL, _T("The calling of D2D1CreateFactory() is failed"), _T("WoChat Error"), MB_OK);
		return 2;
	}

	g_pDWriteFactory = nullptr;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWriteFactory));
	if (S_OK != hr || nullptr == g_pDWriteFactory)
	{
		MessageBox(NULL, _T("The calling of DWriteCreateFactory() is failed"), _T("Wochat Error"), MB_OK);
		return 3;
	}

	length = GetModuleFileNameW(hInstance, g_AppPath, MAX_PATH);
	if (length > 0)
	{
		DWORD i;
		wchar_t iniFilePath[MAX_PATH + 1] = { 0 };
		wchar_t sk[64 + 1] = { 0 };

		for(i = length - 1; i > 0; i--)
		{
			if (g_AppPath[i] == L'\\')
				break;
		}
		g_AppPath[i] = L'\0';
	}
	else 
	{
		g_AppPath[0] = L'\0';
		return 4;
	}

	iRet = GetSecretKey(g_AppPath, g_SK);

	if (0 != iRet)
		return 5;

	return 0;
}

static void ExitInstance(HINSTANCE hInstance)
{
	UINT i, tries;

	// tell all threads to quit
	InterlockedIncrement(&g_Quit);

	SafeRelease(&g_pDWriteFactory);
	SafeRelease(&g_pD2DFactory);

	// wait for all threads to quit gracefully
	tries = 10;
	while (g_threadCount && tries > 0)
	{
		Sleep(1000);
		tries--;
	}
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	int iRet = 0;
	HRESULT hr;

	UNREFERENCED_PARAMETER(hPrevInstance);

	g_Quit = 0;
	g_threadCount = 0;
	g_hInstance = hInstance;  // save the instance

	// The Microsoft Security Development Lifecycle recommends that all
	// applications include the following call to ensure that heap corruptions
	// do not go unnoticed and therefore do not introduce opportunities
	// for security exploits.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (S_OK != hr) 
		return 0;

	iRet = InitInstance(hInstance);
	if (0 != iRet)
		goto ExitThisApplication;

	// BLOCK: Run application
	{
		CWoChatThreadManager mgr;
		iRet = mgr.Run(lpCmdLine, nShowCmd);
	}

ExitThisApplication:
	ExitInstance(hInstance);
	CoUninitialize();

	return 0;
}
