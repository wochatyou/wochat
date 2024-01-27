#ifndef __WOCHAT_WINDOWS1_H__
#define __WOCHAT_WINDOWS1_H__

#include "dui/dui_win.h"

U16 txt[] = { 0x0044,0x0042,0x0041,0x57f9,0x8bad,0x7fa4,0x0028,0x0032,0x0035,0x0037,0x0029,0x0000 };

class XWindow1 : public XWindowT <XWindow1>
{
public:
	XWindow1()
	{
		m_backgroundColor = 0xFFEAECED;
		m_property |= (DUI_PROP_MOVEWIN |DUI_PROP_HANDLETIMER | DUI_PROP_HANDLEKEYBOARD);
		m_message = WM_XWINDOWS01;
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

};

#endif  /* __WOCHAT_WINDOWS1_H__ */

