#ifndef __WOCHAT_WINDOWS1_H__
#define __WOCHAT_WINDOWS1_H__

#include "dui/dui_win.h"

wchar_t testxt[] = {
	0x6c38,0x548c,0x4e5d,0x5e74,0xff0c,0x5c81,0x5728,0x7678,0x4e11,0xff0c,0x66ae,0x6625,0x4e4b,0x521d,0xff0c,0x4f1a,
	0x4e8e,0x4f1a,0x7a3d,0x5c71,0x9634,0x4e4b,0x5170,0x4ead,0xff0c,0x4fee,0x798a,0x4e8b,0x4e5f,0x3002,0x7fa4,0x8d24,
	0x6bd5,0x81f3,0xff0c,0x5c11,0x957f,0x54b8,0x96c6,0x3002,0x6b64,0x5730,0x6709,0x5d07,0x5c71,0x5cfb,0x5cad,0xff0c,
	0x8302,0x6797,0x4fee,0x7af9,0xff1b,0x53c8,0x6709,0x6e05,0x6d41,0x6fc0,0x6e4d,0xff0c,0x6620,0x5e26,0x5de6,0x53f3,
	0xff0c,0x5f15,0x4ee5,0x4e3a,0x6d41,0x89de,0x66f2,0x6c34,0xff0c,0x5217,0x5750,0x5176,0x6b21,0x3002,0x867d,0x65e0,
	0x4e1d,0x7af9,0x7ba1,0x5f26,0x4e4b,0x76db,0xff0c,0x4e00,0x89de,0x4e00,0x548f,0xff0c,0x4ea6,0x8db3,0x4ee5,0x7545,
	0x53d9,0x5e7d,0x60c5,0x3002,0x662f,0x65e5,0x4e5f,0xff0c,0x5929,0x6717,0x6c14,0x6e05,0xff0c,0x60e0,0x98ce,0x548c,
	0x7545,0x3002,0x4ef0,0x89c2,0x5b87,0x5b99,0x4e4b,0x5927,0xff0c,0x4fef,0x5bdf,0x54c1,0x7c7b,0x4e4b,0x76db,0xff0c,0x000a,0x000a,
	0x6240,0x4ee5,0x6e38,0x76ee,0x9a8b,0x6000,0xff0c,0x8db3,0x4ee5,0x6781,0x89c6,0x542c,0x4e4b,0x5a31,0xff0c,0x4fe1,
	0x53ef,0x4e50,0x4e5f,0x3002,0x592b,0x4eba,0x4e4b,0x76f8,0x4e0e,0xff0c,0x4fef,0x4ef0,0x4e00,0x4e16,0xff0c,0x6216,
	0x53d6,0x8bf8,0x6000,0x62b1,0xff0c,0x609f,0x8a00,0x4e00,0x5ba4,0x4e4b,0x5185,0xff1b,0x6216,0x56e0,0x5bc4,0x6240,
	0x6258,0xff0c,0x653e,0x6d6a,0x5f62,0x9ab8,0x4e4b,0x5916,0x3002,0x867d,0x8da3,0x820d,0x4e07,0x6b8a,0xff0c,0x9759,
	0x8e81,0x4e0d,0x540c,0xff0c,0x5f53,0x5176,0x6b23,0x4e8e,0x6240,0x9047,0xff0c,0x6682,0x5f97,0x4e8e,0x5df1,0xff0c,
	0x5feb,0x7136,0x81ea,0x8db3,0xff0c,0x4e0d,0x77e5,0x8001,0x4e4b,0x5c06,0x81f3,0x3002,0x53ca,0x5176,0x6240,0x4e4b,
	0x65e2,0x5026,0xff0c,0x60c5,0x968f,0x4e8b,0x8fc1,0xff0c,0x611f,0x6168,0x7cfb,0x4e4b,0x77e3,0x3002,0x5411,0x4e4b,
	0x6240,0x6b23,0xff0c,0x4fef,0x4ef0,0x4e4b,0x95f4,0xff0c,0x5df2,0x4e3a,0x9648,0x8ff9,0xff0c,0x72b9,0x4e0d,0x80fd,
	0x4e0d,0x4ee5,0x4e4b,0x5174,0x6000,0x3002,0x51b5,0x4fee,0x77ed,0x968f,0x5316,0xff0c,0x7ec8,0x671f,0x4e8e,0x5c3d,
	0x3002,0x53e4,0x4eba,0x4e91,0xff1a,0x201c,0x6b7b,0x751f,0x4ea6,0x5927,0x77e3,0x3002,0x201d,0x5c82,0x4e0d,0x75db,
	0x54c9,0xff01,0x6bcf,0x89c8,0x6614,0x4eba,0x5174,0x611f,0x4e4b,0x7531,0xff0c,0x82e5,0x5408,0x4e00,0x5951,0xff0c,
	0x672a,0x5c1d,0x4e0d,0x4e34,0x6587,0x55df,0x60bc,0xff0c,0x4e0d,0x80fd,0x55bb,0x4e4b,0x4e8e,0x6000,0x3002,0x56fa,
	0x77e5,0x4e00,0x6b7b,0x751f,0x4e3a,0x865a,0x8bde,0xff0c,0x9f50,0x5f6d,0x6b87,0x4e3a,0x5984,0x4f5c,0x3002,0x540e,
	0x4e4b,0x89c6,0x4eca,0xff0c,0x4ea6,0x72b9,0x4eca,0x4e4b,0x89c6,0x6614,0x3002,0x60b2,0x592b,0xff01,0x6545,0x5217,
	0x53d9,0x65f6,0x4eba,0xff0c,0x5f55,0x5176,0x6240,0x8ff0,0xff0c,0x867d,0x4e16,0x6b8a,0x4e8b,0x5f02,0xff0c,0x6240,
	0x4ee5,0x5174,0x6000,0xff0c,0x5176,0x81f4,0x4e00,0x4e5f,0x3002,0x540e,0x4e4b,0x89c8,0x8005,0xff0c,0x4ea6,0x5c06,
	0x6709,0x611f,0x4e8e,0x65af,0x6587,0x3002,0
};


class XWindow1 : public XWindowT <XWindow1>
{
private:
	IDWriteTextFormat* m_pTextFormat = nullptr;
	IDWriteTextLayout* m_pTextLayout = nullptr;
public:
	XWindow1()
	{
		m_backgroundColor = 0xFFEAECED;
		m_backgroundColor = 0xFFFFFFFF;
		m_property |= (DUI_PROP_MOVEWIN |DUI_PROP_HANDLETIMER | DUI_PROP_HANDLEKEYBOARD);
		m_message = WM_XWINDOWS01;

		g_pDWriteFactory->CreateTextFormat(
			L"Microsoft Yahei",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16.0f,
			L"en-US",
			&m_pTextFormat
		);

		g_pDWriteFactory->CreateTextLayout(
			testxt,
			(UINT32)wcslen(testxt),
			m_pTextFormat,
			(FLOAT)600,
			(FLOAT)300,
			&m_pTextLayout
		);
	}

	~XWindow1()	{}

	void InitBitmap()
	{
		U8 id;
		XBitmap* bmp;

		int w = 27;
		int h = 27;
		id = XWIN1_BITMAP_SEARCHN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachN; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachH; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachP; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachH; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{
#if 0
		U32 objSize;
		U8 id, *mem;

		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource
		id = XWIN1_EDITBOX_SEARCH;
		m_startControl = id;
		objSize = sizeof(XEditBox);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XEditBox* eb = new(mem)XEditBox;
			assert(nullptr != eb);
			if (0 != eb->Init(g_hCursorIBeam))
			{
				pfree(mem);
			}
			else
			{
				eb->setRoundColor(m_backgroundColor, m_backgroundColor);
				eb->setBkgFrontColor(0xFFFFFFFF, 0xFF555555);
				eb->setId(id);
				dui_controlArray[id] = eb;
				m_endControl = id;
			}
		}
		else return;

		id = XWIN1_BUTTON_SEARCH;
		objSize = sizeof(XButton);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XBitmap* bmpN;
			XBitmap* bmpH;
			XBitmap* bmpP;
			XBitmap* bmpA;
			XButton* button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				bmpN = &dui_bitmapArray[XWIN1_BITMAP_SEARCHN];
				bmpH = &dui_bitmapArray[XWIN1_BITMAP_SEARCHH];
				bmpP = &dui_bitmapArray[XWIN1_BITMAP_SEARCHP];
				bmpA = &dui_bitmapArray[XWIN1_BITMAP_SEARCHA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				button->setId(id);
				dui_controlArray[id] = button;
				m_endControl = id;
			}
		}
#endif
	}


public:
	void UpdateControlPosition()
	{
#if 0
		XControl* xctl;
		int sw, sh, dx, dy, gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		xctl = dui_controlArray[XWIN1_BUTTON_SEARCH];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();

		assert(w > sw + gap);
		assert(h > sh);

		dy = (h - sh) >> 1;
		dx = w - gap - sw;
		xctl->setPosition(dx, dy);

		sw = dx - gap;
		xctl = dui_controlArray[XWIN1_EDITBOX_SEARCH];
		assert(nullptr != xctl);
		dx = gap;
		xctl->setPosition(dx, dy, sw, dy + sh);
#endif
	}

	int Do_DUI_CHAR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int r = 0;
#if 0
		U16 charCode = static_cast<U16>(wParam);

		XEditBox* eb = (XEditBox *)dui_controlArray[XWIN1_EDITBOX_SEARCH];
		assert(nullptr != eb);

		r = eb->OnKeyBoard(XKEYBOARD_NORMAL, charCode);
#endif
		return r; 
	}

	int Do_DUI_KEYDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{
		int r = DUI_STATUS_NODRAW;
#if 0
		U32 keyCode = static_cast<U32>(wParam);

		bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
		bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

		XEditBox* eb = (XEditBox*)dui_controlArray[XWIN1_EDITBOX_SEARCH];
		assert(nullptr != eb);

		switch (keyCode)
		{
		case VK_RETURN:
		case VK_BACK:
		case VK_DELETE:
		case VK_TAB:
		case VK_LEFT:
			r = eb->MoveCursorLR(-1);
			break;
		case VK_RIGHT:
			r = eb->MoveCursorLR(1);
			break;
		case VK_UP:
		case VK_DOWN:
		case VK_HOME:
		case VK_END:
		case VK_INSERT:
		case 'C':
		case 'X':
		case 'A':
		case 'V':
		default:
			break;
		}
#endif
		return r;
	}

	int Do_DUI_PAINT(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		DUI_ScreenFillRect(m_screen, w, h, 0xFF000000, w - 20, 1, 10, 60);

		return 0; 
	}

	int DoDraw(DUI_Surface surface, DUI_Brush brush)
	{ 
		ID2D1HwndRenderTarget* pD2DRenderTarget = (ID2D1HwndRenderTarget*)surface;
		ID2D1SolidColorBrush* pTextBrush = (ID2D1SolidColorBrush*)brush;

		pD2DRenderTarget->DrawTextLayout(D2D1::Point2F(static_cast<FLOAT>(m_area.left + 20), static_cast<FLOAT>(m_area.top + 100)), m_pTextLayout, pTextBrush);

		return 0; 
	}

	int Do_DUI_MOUSEMOVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		BOOL isTrailingHit = FALSE;
		BOOL isInside = FALSE;
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		FLOAT x = (FLOAT)(xPos - (m_area.left + 20));
		FLOAT y = (FLOAT)(yPos - (m_area.top + 100));

		DWRITE_HIT_TEST_METRICS htm = { 0 };

		if (nullptr != m_pTextLayout)
		{
			m_pTextLayout->HitTestPoint(x, y, &isTrailingHit, &isInside, &htm);
			if (isInside)
			{
				SetDUIWindowCursor();
				::SetCursor(dui_hCursorIBeam);
			}
		}
		return 0; 
	}
};

#endif  /* __WOCHAT_WINDOWS1_H__ */

