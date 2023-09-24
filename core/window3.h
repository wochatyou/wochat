#ifndef __WOCHAT_WINDOWS3_H__
#define __WOCHAT_WINDOWS3_H__

#include "dui/dui_win.h"

uint16_t titlewin3[] = { 0x0044,0x0042,0x0041,0x57f9,0x8bad,0x7fa4,0x0028,0x0032,0x0035,0x0037,0x0029, 0 };
enum 
{
	XWIN3_BUTTON_DOT = 0,
	XWIN32_LABEL_TITLE 
};

class XWindow3 : public XWindowT <XWindow3>
{
private:
	enum 
	{
		 XWIN3_BITMAP_DOTN = 0			// Normal
		,XWIN3_BITMAP_DOTH				// Hover
		,XWIN3_BITMAP_DOTP				// Press
		,XWIN3_BITMAP_DOTA				// Active
	};
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
		id = XWIN3_BITMAP_DOTN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpDotN; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpDotH; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpDotP; bmp->w = w; bmp->h = h;
		id = XWIN3_BITMAP_DOTA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpDotH; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{
		U8* mem;
		U32 objSize;
		assert(0 == m_controlCount);
		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		objSize = sizeof(XButton2);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XBitmap* bmpN;
			XBitmap* bmpH;
			XBitmap* bmpP;
			XBitmap* bmpA;
			XButton2* button = new(mem)XButton2;
			assert(nullptr != button);
			button->setId(XWIN3_BUTTON_DOT);
			bmpN = &m_bitmap[XWIN3_BITMAP_DOTN];
			bmpH = &m_bitmap[XWIN3_BITMAP_DOTH];
			bmpP = &m_bitmap[XWIN3_BITMAP_DOTP];
			bmpA = &m_bitmap[XWIN3_BITMAP_DOTA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		objSize = sizeof(XLabel);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XLabel* label = new(mem)XLabel;
			assert(nullptr != label);
			if (0 != label->Init(g_ftFace0, 16))
			{
				pfree(mem);
			}
			else
			{
				label->setId(XWIN32_LABEL_TITLE);
				label->setText(titlewin3);
				label->setRoundColor(m_backgroundColor, m_backgroundColor);
				label->SetBkgFrontColor(m_backgroundColor, 0xFF333333);
				m_control[m_controlCount] = label;
				m_controlCount++;
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

		xctl = m_control[XWIN1_BUTTON_SEARCH];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();

		if (w > (sw + GAP_RIGHT3) && h > (sh + GAP_BOTTOM3))
		{
			int dx = w - sw - GAP_RIGHT3;
			int dy = h - sh - GAP_BOTTOM3;
			xctl->setPosition(dx, dy);
		}

		xctl = m_control[XWIN32_LABEL_TITLE];
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

