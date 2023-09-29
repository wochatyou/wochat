#ifndef __WOCHAT_WINDOWS3_H__
#define __WOCHAT_WINDOWS3_H__

#include "dui/dui_win.h"

uint16_t titlewin3[] = { 
	0x0041,0x0049,0x804a,0x5929,0x673a,0x5668,0x4eba,
	0 
};

class XWindow3 : public XWindowT <XWindow3>
{
	enum 
	{
		GAP_TOP3 = 40,
		GAP_BOTTOM3 = 10,
		GAP_LEFT3 = 6,
		GAP_RIGHT3 = 10
	};
public:
	XWindow3()
	{
		m_backgroundColor = 0xFFF5F5F5;
		m_property |= DUI_PROP_MOVEWIN;
		m_message = WM_XWINDOWS03;
	}

	~XWindow3()
	{
	}

	void InitBitmap()
	{
		U8 id;
		XBitmap* bmp;

		int w = 19;
		int h = 7;
		id = XWIN3_BITMAP_DOTN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpDotN; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpDotH; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpDotP; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpDotH; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{

		U8 id, *mem;
		U32 objSize;

		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		id = XWIN3_BUTTON_DOT;
		m_startControl = id;
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
			bmpN = &dui_bitmapArray[XWIN3_BITMAP_DOTN];
			bmpH = &dui_bitmapArray[XWIN3_BITMAP_DOTH];
			bmpP = &dui_bitmapArray[XWIN3_BITMAP_DOTP];
			bmpA = &dui_bitmapArray[XWIN3_BITMAP_DOTA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN3_LABEL_TITLE;
		objSize = sizeof(XLabel);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XLabel* label = new(mem)XLabel;
			assert(nullptr != label);
			m_textDrawInfo = (XTextDrawInfo*)palloc(m_pool, sizeof(XTextDrawInfo));
			if (nullptr != m_textDrawInfo)
			{
				m_textDrawInfoCount = 1;
				if (0 != label->Init(m_textDrawInfo, g_ftFace0, XFONT_SIZE0))
				{
					pfree(mem);
				}
				else
				{
					label->setText(titlewin3, 7);
					label->setRoundColor(m_backgroundColor, m_backgroundColor);
					label->setBkgFrontColor(m_backgroundColor, 0xFF333333);
					label->setId(id);
					dui_controlArray[id] = label;
					m_endControl = id;
				}
			}
		}
	}

public:
	void UpdateControlPosition()
	{
		XControl* xctl;
		int dx, dy, sw, sh, gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		xctl = dui_controlArray[XWIN3_BUTTON_DOT];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();

		if (w > (sw + GAP_RIGHT3) && h > (sh + GAP_BOTTOM3))
		{
			int dx = w - sw - GAP_RIGHT3;
			int dy = h - sh - GAP_BOTTOM3;
			xctl->setPosition(dx, dy);
		}

		xctl = dui_controlArray[XWIN3_LABEL_TITLE];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		assert(h > sh);
		assert(w > sw);
		dx = GAP_LEFT3;
		dy = (h - sh) >> 1;
		xctl->setPosition(dx, dy);
	}

};

#endif  /* __WOCHAT_WINDOWS3_H__ */

