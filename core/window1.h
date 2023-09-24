#ifndef __WOCHAT_WINDOWS1_H__
#define __WOCHAT_WINDOWS1_H__

#include "dui/dui_win.h"
#include "dui/imstb_textedit.h"

U16 txt[] = { 0x0044,0x0042,0x0041,0x57f9,0x8bad,0x7fa4,0x0028,0x0032,0x0035,0x0037,0x0029,0x0000 };

enum
{
	XWIN1_BUTTON_SEARCH = 0
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
		m_property |= DUI_PROP_MOVEWIN;
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
		assert(0 == m_controlCount);
		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource
		U32 objSize = sizeof(XButton2);
		assert(nullptr != m_pool);
		U8* mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XBitmap* bmpN;
			XBitmap* bmpH;
			XBitmap* bmpP;
			XBitmap* bmpA;
			XButton2* button = new(mem)XButton2;
			assert(nullptr != button);
			button->setId(XWIN1_BUTTON_SEARCH);
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


public:
	void UpdateControlPosition()
	{
		int gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		XControl* xctl = m_control[XWIN1_BUTTON_SEARCH];
		assert(nullptr != xctl);
		int sw = xctl->getWidth();
		int sh = xctl->getHeight();

		if (w > sw + gap && h > sh)
		{
			int top = (h - sh) >> 1;
			int left = w - gap - sw;
			xctl->setPosition(left, top);
		}
	}
};

#endif  /* __WOCHAT_WINDOWS1_H__ */

