#ifndef __DUI_WINDOW0_H__
#define __DUI_WINDOW0_H__

#include "dui/dui_win.h"

enum 
{
	XWIN0_BUTTON_ME = 0
	, XWIN0_BUTTON_TALK
	, XWIN0_BUTTON_FRIEND
	, XWIN0_BUTTON_QUAN
	, XWIN0_BUTTON_COIN
	, XWIN0_BUTTON_FAVORITE
	, XWIN0_BUTTON_FILE
	, XWIN0_BUTTON_SETTING
	, XWIN0_BUTTON_NETWORK
};

class XWindow0 : public XWindowT <XWindow0>
{
private:
	enum 
	{
		XWIN0_BITMAP_ME = 0
		, XWIN0_BITMAP_TALKN		// Normal
		, XWIN0_BITMAP_TALKH		// Hover
		, XWIN0_BITMAP_TALKP		// Press
		, XWIN0_BITMAP_TALKA		// Active

		, XWIN0_BITMAP_FRIENDN		// Normal
		, XWIN0_BITMAP_FRIENDH		// Hover
		, XWIN0_BITMAP_FRIENDP		// Press
		, XWIN0_BITMAP_FRIENDA		// Active

		, XWIN0_BITMAP_QUANN		// Normal
		, XWIN0_BITMAP_QUANH		// Hover
		, XWIN0_BITMAP_QUANP		// Press
		, XWIN0_BITMAP_QUANA		// Active

		, XWIN0_BITMAP_FAVORITEN	// Normal
		, XWIN0_BITMAP_FAVORITEH	// Hover
		, XWIN0_BITMAP_FAVORITEP	// Press
		, XWIN0_BITMAP_FAVORITEA	// Active

		, XWIN0_BITMAP_COINN		// Normal
		, XWIN0_BITMAP_COINH		// Hover
		, XWIN0_BITMAP_COINP		// Press
		, XWIN0_BITMAP_COINA		// Active

		, XWIN0_BITMAP_FILEN		// Normal
		, XWIN0_BITMAP_FILEH		// Hover
		, XWIN0_BITMAP_FILEP		// Press
		, XWIN0_BITMAP_FILEA		// Active

		, XWIN0_BITMAP_SETTINGN		// Normal
		, XWIN0_BITMAP_SETTINGH		// Hover
		, XWIN0_BITMAP_SETTINGP		// Press
		, XWIN0_BITMAP_SETTINGA		// Active

		, XWIN0_BITMAP_NETWORKN		// Normal
		, XWIN0_BITMAP_NETWORKH		// Hover
		, XWIN0_BITMAP_NETWORKP		// Press
		, XWIN0_BITMAP_NETWORKA		// Active
	};

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

		xctl = m_control[XWIN0_BUTTON_NETWORK];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = h - gap - sh;
		dx = (w - sw) >> 1;
		xctl->setPosition(dx, dy);
		T = dy;

		xctl = m_control[XWIN0_BUTTON_SETTING];
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
		id = XWIN0_BITMAP_ME;        bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpMe;        bmp->w = w; bmp->h = h;

		w = 32; h = 32;
		id = XWIN0_BITMAP_TALKN;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKH;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKP;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_TALKA;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpTalkA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FRIENDN;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendN;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDH;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendH;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDP;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendP;   bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FRIENDA;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFriendA;   bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_QUANN;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANH;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANP;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_QUANA;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpQuanA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_SETTINGN;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGH;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingH;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGP;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingP;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_SETTINGA;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSettingA;  bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FILEN;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFileN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEH;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFileH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEP;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFileP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FILEA;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFileA;     bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_FAVORITEN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteN; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteH; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteP; bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_FAVORITEA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpFavoriteA; bmp->w = w; bmp->h = h;

		id = XWIN0_BITMAP_COINN;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinN;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINH;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinH;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINP;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinP;     bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_COINA;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCoinA;     bmp->w = w; bmp->h = h;

		w = 13; h = 13;
		id = XWIN0_BITMAP_NETWORKN;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKH;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKP;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
		id = XWIN0_BITMAP_NETWORKA;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpNetWorkN;  bmp->w = w; bmp->h = h;
	}

public:
	void InitControl()
	{
		U8 id;
		int	w = XWIN0_WIDTH;
		int left, top, sw, sh, T;
		U32 objSize = sizeof(XButton2);

		U8* mem;
		XBitmap* bmpN;
		XBitmap* bmpH;
		XBitmap* bmpP;
		XBitmap* bmpA;
		XButton2* button;

		assert(0 == m_controlCount);
		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		id = XWIN0_BUTTON_ME;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_ME];
				button->setBitmap(bmpN, bmpN, bmpN, bmpN);
				sw = button->getWidth(); sh = button->getHeight();
				left = top = (w - sw) >> 1;
				button->setPosition(left, top);
				T = top + sh + 20;
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_TALK;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_TALKN];
				bmpH = &m_bitmap[XWIN0_BITMAP_TALKH];
				bmpP = &m_bitmap[XWIN0_BITMAP_TALKP];
				bmpA = &m_bitmap[XWIN0_BITMAP_TALKA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				button->setStatus(XCONTROL_STATE_ACTIVE);
				m_control[m_controlCount] = button;
				m_activeControl = m_controlCount;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_FRIEND;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_FRIENDN];
				bmpH = &m_bitmap[XWIN0_BITMAP_FRIENDH];
				bmpP = &m_bitmap[XWIN0_BITMAP_FRIENDP];
				bmpA = &m_bitmap[XWIN0_BITMAP_FRIENDA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_QUAN;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_QUANN];
				bmpH = &m_bitmap[XWIN0_BITMAP_QUANH];
				bmpP = &m_bitmap[XWIN0_BITMAP_QUANP];
				bmpA = &m_bitmap[XWIN0_BITMAP_QUANA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_COIN;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_COINN];
				bmpH = &m_bitmap[XWIN0_BITMAP_COINH];
				bmpP = &m_bitmap[XWIN0_BITMAP_COINP];
				bmpA = &m_bitmap[XWIN0_BITMAP_COINA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_FAVORITE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_FAVORITEN];
				bmpH = &m_bitmap[XWIN0_BITMAP_FAVORITEH];
				bmpP = &m_bitmap[XWIN0_BITMAP_FAVORITEP];
				bmpA = &m_bitmap[XWIN0_BITMAP_FAVORITEA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_FILE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_FILEN];
				bmpH = &m_bitmap[XWIN0_BITMAP_FILEH];
				bmpP = &m_bitmap[XWIN0_BITMAP_FILEP];
				bmpA = &m_bitmap[XWIN0_BITMAP_FILEA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_SETTING;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_SETTINGN];
				bmpH = &m_bitmap[XWIN0_BITMAP_SETTINGH];
				bmpP = &m_bitmap[XWIN0_BITMAP_SETTINGP];
				bmpA = &m_bitmap[XWIN0_BITMAP_SETTINGA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}

		id = XWIN0_BUTTON_NETWORK;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init();
			{
				button->setId(id, m_controlCount);
				bmpN = &m_bitmap[XWIN0_BITMAP_NETWORKN];
				bmpH = &m_bitmap[XWIN0_BITMAP_NETWORKH];
				bmpP = &m_bitmap[XWIN0_BITMAP_NETWORKP];
				bmpA = &m_bitmap[XWIN0_BITMAP_NETWORKA];
				button->setBitmap(bmpN, bmpH, bmpP, bmpA);
				sw = button->getWidth(); sh = button->getHeight();
				left = (w - sw) >> 1;
				top = T;
				T = top + sh + sh;
				button->setPosition(left, top);
				button->setProperty(XCONTROL_PROP_STATIC);
				button->setRoundColor(m_backgroundColor, m_backgroundColor);
				m_control[m_controlCount] = button;
				m_controlCount++;
			}
		}
	}
};

#endif  /* __DUI_WINDOW0_H__ */

