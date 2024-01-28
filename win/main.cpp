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
#include "wochatdef.h"
#include "wochat.h"
#include "xbitmapdata.h"

#include "win0.h"
#include "win1.h"

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
void SafeRelease(T * *ppT)
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
IDWriteTextFormat*  g_pTextFormat0 = nullptr;

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

	XWindow()
	{
		m_rectClient.left = m_rectClient.right = m_rectClient.top = m_rectClient.bottom = 0;

		m_win0.SetWindowId((const U8*)"DUIWin0", 7);
		m_win1.SetWindowId((const U8*)"DUIWin1", 7);

		m_win1.SetMode(WIN1_MODE_SHOWSEARCH);
#if 0
		m_win2.SetWindowId((const U8*)"DUIWin2", 7);
		m_win3.SetWindowId((const U8*)"DUIWin3", 7);
		m_win4.SetWindowId((const U8*)"DUIWin4", 7);
		m_win5.SetWindowId((const U8*)"DUIWin5", 7);
#endif
	}

	~XWindow()
	{
		if (nullptr != m_screenBuff)
			VirtualFree(m_screenBuff, 0, MEM_RELEASE);

		m_screenBuff = nullptr;
		m_screenSize = 0;
	}

	BEGIN_MSG_MAP(XWindow)
		MESSAGE_HANDLER_DUIWINDOW(DUI_ALLMESSAGE, OnDUIWindowMessage)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_XWINDOWS00, OnWin0Message)
		MESSAGE_HANDLER(WM_XWINDOWS01, OnWin1Message)
		MESSAGE_HANDLER(WM_XWINDOWS02, OnWin2Message)
		MESSAGE_HANDLER(WM_XWINDOWS03, OnWin3Message)
		MESSAGE_HANDLER(WM_XWINDOWS04, OnWin4Message)
		MESSAGE_HANDLER(WM_XWINDOWS05, OnWin5Message)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnDUIWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		WPARAM wp = wParam;
		LPARAM lp = lParam;
		switch (uMsg)
		{
		case WM_SETCURSOR:
		{
			DWORD dwPos = ::GetMessagePos();
			POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			ScreenToClient(&ptPos);
			wp = (WPARAM)ptPos.x;
			lp = (LPARAM)ptPos.y;
		}
		break;
		case WM_MOUSEWHEEL:
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			ScreenToClient(&pt);
			lp = MAKELONG(pt.x, pt.y);
		}
		break;
		case WM_CREATE:
			wp = (WPARAM)m_hWnd;
			break;
		default:
			break;
		}

		m_win0.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win1.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
#if 0
		m_win2.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win3.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win4.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win5.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
#endif
#if 10
		if (DUIWindowNeedReDraw())
			Invalidate();
#endif
		// to allow the host window to continue to handle the windows message
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL bHandled)
	{
		m_nDPI = GetDpiForWindow(m_hWnd);
		//SetTimer(XWIN_666MS_TIMER, 666);
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0; // don't want flicker
	}

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL bHandled)
	{
		KillTimer(XWIN_666MS_TIMER);

		SafeRelease(&m_pTextBrushSel);
		SafeRelease(&m_pTextBrush0);
		SafeRelease(&m_pTextBrush1);
		SafeRelease(&m_pixelBitmap0);
		SafeRelease(&m_pixelBitmap1);
		SafeRelease(&m_pixelBitmap2);
		SafeRelease(&m_pD2DRenderTarget);

		PostQuitMessage(0);
		return 0;
	}

	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = DUIWindowCursorIsSet() ? TRUE : FALSE;

		ClearDUIWindowCursor();
#if 0
		if (((HWND)wParam == m_hWnd) && (LOWORD(lParam) == HTCLIENT))
		{
			DWORD dwPos = ::GetMessagePos();

			POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };

			ScreenToClient(&ptPos);

			DrapMode mode = IsOverSplitterBar(ptPos.x, ptPos.y);
			if (DrapMode::dragModeNone != mode)
				bHandled = TRUE;
		}
#endif
		return 0;
	}

	LRESULT OnWin0Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == 0)
		{
			U8 ctlId = (U8)lParam;
		}

		return 0;
	}

	LRESULT OnWin1Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnWin2Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnWin3Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnWin4Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnWin5Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SafeRelease(&m_pD2DRenderTarget);

		if (SIZE_MINIMIZED != wParam)
		{
			XRECT area;
			XRECT* r = &area;

			GetClientRect(&m_rectClient);
			ATLASSERT(0 == m_rectClient.left);
			ATLASSERT(0 == m_rectClient.top);
			ATLASSERT(m_rectClient.right > 0);
			ATLASSERT(m_rectClient.bottom > 0);

			U32 w = (U32)(m_rectClient.right - m_rectClient.left);
			U32 h = (U32)(m_rectClient.bottom - m_rectClient.top);

			if (nullptr != m_screenBuff)
			{
				VirtualFree(m_screenBuff, 0, MEM_RELEASE);
				m_screenBuff = nullptr;
				m_screenSize = 0;
			}

			m_screenSize = DUI_ALIGN_PAGE(w * h * sizeof(U32));
			ATLASSERT(m_screenSize >= (w * h * sizeof(U32)));

			m_screenBuff = (U32*)VirtualAlloc(NULL, m_screenSize, MEM_COMMIT, PAGE_READWRITE);
			if (nullptr == m_screenBuff)
			{
				m_win0.UpdateSize(nullptr, nullptr);
				m_win1.UpdateSize(nullptr, nullptr);
#if 0
				m_win2.UpdateSize(nullptr, nullptr);
				m_win3.UpdateSize(nullptr, nullptr);
				m_win4.UpdateSize(nullptr, nullptr);
				m_win5.UpdateSize(nullptr, nullptr);
#endif
				Invalidate();
				return 0;
			}

			if (m_splitterVPos < 0)
			{
				m_splitterVPos = (XWIN0_WIDTH + XWIN1_WIDTH);
				if (m_splitterVPos < m_splitterVPosToLeft)
					m_splitterVPos = m_splitterVPosToLeft;

				if (m_splitterVPos > (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight))
				{
					m_splitterVPos = (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight);
					ATLASSERT(m_splitterVPos > m_splitterVPosToLeft);
				}
				m_splitterVPosOld = m_splitterVPos;
			}

			m_splitterVPos = m_splitterVPosOld;

			if (m_splitterHPos < 0)
			{
				m_splitterHPos = m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom;
				if (m_splitterHPos < m_splitterHPosToTop)
					m_splitterHPos = m_splitterHPosToTop;

				if (m_splitterHPos > (m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom))
				{
					m_splitterHPos = m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom;
					ATLASSERT(m_splitterHPos > m_splitterHPosToTop);
				}
				m_splitterHPosOld = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPos;
			}

			if (m_splitterHPos > 0) // if(m_splitterHPos <= 0) then windows 5 is hidden
			{
				m_splitterHPos = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPosOld;
			}


			if (nullptr != m_screenBuff)
			{
				U32* dst = m_screenBuff;
				U32 size;

				r->left = m_rectClient.left;
				r->right = XWIN0_WIDTH;
				r->top = m_rectClient.top;
				r->bottom = m_rectClient.bottom;
				m_win0.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = XWIN0_WIDTH;
				r->right = m_rectClient.right;
				r->top = m_rectClient.top;
				r->bottom = m_rectClient.bottom;
				m_win1.UpdateSize(r, dst);
			}
			Invalidate();
		}
		return 0;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		HRESULT hr = S_OK;
		PAINTSTRUCT ps;
		BeginPaint(&ps);

		if (nullptr == m_pD2DRenderTarget)
		{
			D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
			renderTargetProperties.dpiX = m_nDPI;
			renderTargetProperties.dpiY = m_nDPI;
			renderTargetProperties.pixelFormat = pixelFormat;

			D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTragetproperties
				= D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(m_rectClient.right - m_rectClient.left, m_rectClient.bottom - m_rectClient.top));

			ATLASSERT(nullptr != g_pD2DFactory);
			hr = g_pD2DFactory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTragetproperties, &m_pD2DRenderTarget);
			if (S_OK == hr && nullptr != m_pD2DRenderTarget)
			{
				U32 pixel[1] = { 0xFFEEEEEE };
				SafeRelease(&m_pixelBitmap0);
				hr = m_pD2DRenderTarget->CreateBitmap(
					D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
					&m_pixelBitmap0);
				if (S_OK == hr && nullptr != m_pixelBitmap0)
				{
					pixel[0] = 0xFFFFFFFF;
					hr = m_pD2DRenderTarget->CreateBitmap(
						D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
						&m_pixelBitmap1);
					ATLASSERT(S_OK == hr);

					pixel[0] = 0xFF6AEA9E;
					hr = m_pD2DRenderTarget->CreateBitmap(
						D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
						&m_pixelBitmap2);
					ATLASSERT(S_OK == hr);

					hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x666666), &m_pTextBrush0);
					if (S_OK == hr && nullptr != m_pTextBrush0)
					{
						hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x000000), &m_pTextBrush1);
						if (S_OK == hr && nullptr != m_pTextBrush1)
						{
							hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x87CEFA), &m_pTextBrushSel);
						}
					}
				}
			}
		}

		if (S_OK == hr && nullptr != m_pD2DRenderTarget && nullptr != m_pixelBitmap0 && nullptr != m_pTextBrush0 && nullptr != m_pTextBrush1 && nullptr != m_pTextBrushSel)
		{
			int w, h;
			U32* src = nullptr;

			m_pD2DRenderTarget->BeginDraw();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			//m_pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			// draw window 0
			src = m_win0.GetDUIBuffer();
			if (nullptr != src)
			{
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win0.GetWindowArea();
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);
				m_win0.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 1
			src = m_win1.GetDUIBuffer();
			if (nullptr != src)
			{
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win1.GetWindowArea();
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);
				m_win1.DrawText(m_pD2DRenderTarget, m_pTextBrush0, m_pTextBrushSel);
				m_win1.SetScreenValide(); // prevent un-necessary draw again
			}

			hr = m_pD2DRenderTarget->EndDraw();
			////////////////////////////////////////////////////////////////////////////////////////////////////
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
	XWindow1 m_win1;
#if 0
	XWindow2 m_win2;
	XWindow3 m_win3;
	XWindow4 m_win4;
	XWindow4 m_win5;
#endif
private:
	ID2D1HwndRenderTarget* m_pD2DRenderTarget = nullptr;
	ID2D1Bitmap* m_pixelBitmap0 = nullptr;
	ID2D1Bitmap* m_pixelBitmap1 = nullptr;
	ID2D1Bitmap* m_pixelBitmap2 = nullptr;
	ID2D1SolidColorBrush* m_pTextBrushSel = nullptr;
	ID2D1SolidColorBrush* m_pTextBrush0 = nullptr;
	ID2D1SolidColorBrush* m_pTextBrush1 = nullptr;

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

	g_pTextFormat0 = nullptr;
	hr = g_pDWriteFactory->CreateTextFormat(
		L"Microsoft Yahei",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-US",
		&g_pTextFormat0
	);

	if (S_OK != hr || nullptr == g_pTextFormat0)
	{
		MessageBox(NULL, _T("The calling of CreateTextFormat() is failed"), _T("Wochat Error"), MB_OK);
		return 4;
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

	iRet = DUI_Init();

	return iRet;
}

static void ExitInstance(HINSTANCE hInstance)
{
	UINT i, tries;

	// tell all threads to quit
	InterlockedIncrement(&g_Quit);

	DUI_Term();

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
