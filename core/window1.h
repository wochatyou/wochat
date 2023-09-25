#ifndef __WOCHAT_WINDOWS1_H__
#define __WOCHAT_WINDOWS1_H__

#include "dui/dui_win.h"
#include "dui/imstb_textedit.h"

U16 txt[] = { 0x0044,0x0042,0x0041,0x57f9,0x8bad,0x7fa4,0x0028,0x0032,0x0035,0x0037,0x0029,0x0000 };

enum
{
	XWIN1_BUTTON_SEARCH = 0,
	XWIN1_EDITBOX_SEARCH
};

class XWindow1 : public XWindowT <XWindow1>
{
private:
	enum
	{
		 XWIN1_BITMAP_SEARCHN = 0	// Normal
		,XWIN1_BITMAP_SEARCHH		// Hover
		,XWIN1_BITMAP_SEARCHP		// Press
		,XWIN1_BITMAP_SEARCHA		// Active
	};

public:
	XWindow1()
	{
		m_backgroundColor = 0xFFEAECED;
		m_property |= (DUI_PROP_MOVEWIN |DUI_PROP_HANDLETIMER | DUI_PROP_HANDLEKEYBOARD);
		m_message = WM_XWINDOWS01;
	}

	~XWindow1()	{}

	U16*	m_text = (U16*)txt;

	void InitBitmap()
	{
		U8 id;
		XBitmap* bmp;

		int w = 27;
		int h = 27;
		id = XWIN1_BITMAP_SEARCHN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachN; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachH; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachP; bmp->w = w; bmp->h = h;
		id = XWIN1_BITMAP_SEARCHA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSerachH; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{
		U32 objSize;
		U8 id, *mem;
		assert(0 == m_controlCount);
		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

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
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN1_BITMAP_SEARCHN];
				bmpH = &m_bitmap[XWIN1_BITMAP_SEARCHH];
				bmpP = &m_bitmap[XWIN1_BITMAP_SEARCHP];
				bmpA = &m_bitmap[XWIN1_BITMAP_SEARCHA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN1_EDITBOX_SEARCH;
	 	objSize = sizeof(XEditBox);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XEditBox* eb = new(mem)XEditBox;
			assert(nullptr != eb);
			if (0 != eb->Init(g_hCursorIBeam, g_ftFace0, 16))
			{
				pfree(mem);
			}
			else
			{
				eb->setId(id, m_controlCount);
				eb->setRoundColor(m_backgroundColor, m_backgroundColor);
				eb->setBkgFrontColor(0xFFFFFFFF, 0xFF555555);
				m_control[m_controlCount] = eb;
				m_controlCount++;
			}
		}
	}


public:
	void UpdateControlPosition()
	{
		XControl* xctl;
		int sw, sh, dx, dy, gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		xctl = m_control[XWIN1_BUTTON_SEARCH];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();

		assert(w > sw + gap);
		assert(h > sh);

		dy = (h - sh) >> 1;
		dx = w - gap - sw;
		xctl->setPosition(dx, dy);

		sw = dx - gap;
		xctl = m_control[XWIN1_EDITBOX_SEARCH];
		assert(nullptr != xctl);
		dx = gap;
		xctl->setPosition(dx, dy, sw, dy + sh);
	}

	int DoChar(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = 0;
		U16 charCode = static_cast<U16>(wParam);

		XEditBox* eb = (XEditBox *)m_control[XWIN1_EDITBOX_SEARCH];
		assert(nullptr != eb);

		r = eb->OnKeyBoard(XKEYBOARD_NORMAL, charCode);

		return r; 
	}

	int DoKeyPress(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{
		int r = DUI_STATUS_NODRAW;

		U32 keyCode = static_cast<U32>(wParam);

		bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
		bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

		XEditBox* eb = (XEditBox*)m_control[XWIN1_EDITBOX_SEARCH];
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
		return r;
	}

};

#endif  /* __WOCHAT_WINDOWS1_H__ */

