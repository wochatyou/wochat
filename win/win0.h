#ifndef __DUI_WINDOW0_H__
#define __DUI_WINDOW0_H__

#include "dui/dui_win.h"
class XWindow0 : public XWindowT <XWindow0>
{
public:
	XWindow0()
	{
		m_backgroundColor = 0xFF2A2928;
		m_property |= (DUI_PROP_MOVEWIN | DUI_PROP_BTNACTIVE);
		m_message = WM_XWINDOWS00;
	}
	~XWindow0() {}

public:
	void UpdateControlPosition()
	{
		int id, bottom, sw, sh, dx, dy, T;
		int gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;
		XControl* xctl;

		xctl = dui_controlArray[XWIN0_BUTTON_NETWORK];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = h - gap - sh;
		dx = (w - sw) >> 1;
		xctl->setPosition(dx, dy);
		T = dy;

		xctl = dui_controlArray[XWIN0_BUTTON_SETTING];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = T - gap - sh;
		dx = (w - sw) >> 1;
		xctl->setPosition(dx, dy);
	}

private:
	void InitBitmap()
	{
		U8 id;
		XBitmap* bmp;

		int w = 48;
		int h = 48;
		id = XWIN0_BITMAP_ME;        bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpMe;        bmp->w = w; bmp->h = h;

		w = 32; h = 32;
		id = XWIN0_BITMAP_TALKN;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKH;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKP;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKA;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FRIENDN;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendN;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDH;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendH;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDP;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendP;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDA;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendA;   bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_QUANN;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANH;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANP;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANA;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_SETTINGN;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGH;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingH;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGP;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingP;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGA;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingA;  bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FILEN;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFileN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEH;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFileH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEP;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFileP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEA;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFileA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FAVORITEN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteN; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteH; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteP; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteA; bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_COINN;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINH;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINP;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINA;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinA;     bmp->w = w; bmp->h = h;

		w = 13; h = 13;
		id = XWIN0_BITMAP_NETWORKN;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKH;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKP;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKA;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
	}

public:
	void InitControl()
	{
		U8 id;
		int	w = XWIN0_WIDTH;
		int left, top, sw, sh, T;
		U32 objSize = sizeof(XButton);

		U8* mem;
		XBitmap* bmpN;
		XBitmap* bmpH;
		XBitmap* bmpP;
		XBitmap* bmpA;
		XButton* button;

		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		id = XWIN0_BUTTON_ME;
		m_startControl = id;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_ME];
			button->setBitmap(bmpN, bmpN, bmpN, bmpN);
			sw = button->getWidth(); sh = button->getHeight();
			left = top = (w - sw) >> 1;
			button->setPosition(left, top);
			T = top + sh + 20;
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_TALK;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_TALKN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_TALKH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_TALKP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_TALKA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setStatus(XCONTROL_STATE_ACTIVE);
			button->setId(id);
			dui_controlArray[id] = button;
			m_activeControl = id;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_FRIEND;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_FRIENDN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_FRIENDH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_FRIENDP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_FRIENDA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_QUAN;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_QUANN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_QUANH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_QUANP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_QUANA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_COIN;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_COINN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_COINH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_COINP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_COINA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_FAVORITE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_FAVORITEN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_FAVORITEH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_FAVORITEP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_FAVORITEA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_FILE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_FILEN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_FILEH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_FILEP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_FILEA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_SETTING;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(dui_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_SETTINGN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_SETTINGH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_SETTINGP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_SETTINGA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN0_BUTTON_NETWORK;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init();
			bmpN = &dui_bitmapArray[XWIN0_BITMAP_NETWORKN];
			bmpH = &dui_bitmapArray[XWIN0_BITMAP_NETWORKH];
			bmpP = &dui_bitmapArray[XWIN0_BITMAP_NETWORKP];
			bmpA = &dui_bitmapArray[XWIN0_BITMAP_NETWORKA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			left = (w - sw) >> 1;
			top = T;
			T = top + sh + sh;
			button->setPosition(left, top);
			button->setProperty(XCONTROL_PROP_STATIC);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}

	}
};

#endif  /* __DUI_WINDOW0_H__ */

