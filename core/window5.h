#ifndef __WOCHAT_WINDOWS5_H__
#define __WOCHAT_WINDOWS5_H__

#include "dui/dui_win.h"

//uint16_t inputMessage[XWIN_MAX_INPUTSTRING + 1] = { 0 };

enum {
	XWIN5_BUTTON_EMOJI = 0
	, XWIN5_BUTTON_UPLOAD
	, XWIN5_BUTTON_CAPTURE
	, XWIN5_BUTTON_CHATHISTORY
	, XWIN5_BUTTON_LIVESTREAM
	, XWIN5_BUTTON_VIDEOCALL
	, XWIN5_BUTTON_SENDMESSAGE
	, XWIN5_BUTTON_HINT
};

class XWindow5 : public XWindowT <XWindow5>
{
private:
	enum 
	{
		GAP_TOP4 = 40,
		GAP_BOTTOM4 = 40,
		GAP_LEFT4 = 4,
		GAP_RIGHT4 = 4
	};

	enum 
	{
		XWIN5_BITMAP_EMOJIN
		, XWIN5_BITMAP_EMOJIH				// Hover
		, XWIN5_BITMAP_EMOJIP				// Press
		, XWIN5_BITMAP_EMOJIA				// Active

		, XWIN5_BITMAP_UPLOADN				// Normal
		, XWIN5_BITMAP_UPLOADH				// Hover
		, XWIN5_BITMAP_UPLOADP				// Press
		, XWIN5_BITMAP_UPLOADA				// Active

		, XWIN5_BITMAP_CAPTUREN				// Normal
		, XWIN5_BITMAP_CAPTUREH				// Hover
		, XWIN5_BITMAP_CAPTUREP				// Press
		, XWIN5_BITMAP_CAPTUREA				// Active

		, XWIN5_BITMAP_CHATHISTORYN			// Normal
		, XWIN5_BITMAP_CHATHISTORYH			// Hover
		, XWIN5_BITMAP_CHATHISTORYA			// Press
		, XWIN5_BITMAP_CHATHISTORYP			// Active

		, XWIN5_BITMAP_LIVESTREAMN			// Normal
		, XWIN5_BITMAP_LIVESTREAMH			// Hover
		, XWIN5_BITMAP_LIVESTREAMP			// Press
		, XWIN5_BITMAP_LIVESTREAMA			// Active

		, XWIN5_BITMAP_VIDEOCALLN			// Normal
		, XWIN5_BITMAP_VIDEOCALLH			// Hover
		, XWIN5_BITMAP_VIDEOCALLP			// Press
		, XWIN5_BITMAP_VIDEOCALLA			// Active

		, XWIN5_BITMAP_SENDMESSAGEN			// Normal			
		, XWIN5_BITMAP_SENDMESSAGEH			// Hover
		, XWIN5_BITMAP_SENDMESSAGEP			// Press
		, XWIN5_BITMAP_SENDMESSAGEA			// Active

		, XWIN5_BITMAP_HINTN				// Normal			
		, XWIN5_BITMAP_HINTH				// Hover
		, XWIN5_BITMAP_HINTP				// Press
		, XWIN5_BITMAP_HINTA				// Active

	};

public:
	XWindow5()
	{
		m_backgroundColor = 0xFFFFFFFF;
		m_property |= (DUI_PROP_MOVEWIN | DUI_PROP_HANDLETIMER | DUI_PROP_HANDLEKEYBOARD);
		m_message = WM_XWINDOWS05;
	}

	~XWindow5()
	{
	}

	void InitBitmap()
	{
		U8 id;
		XBitmap* bmp;

		int w = 19;
		int h = 19;
		id = XWIN5_BITMAP_EMOJIN;       bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiN;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIH;       bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiH;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIP;       bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiP;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIA;       bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiN;       bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_UPLOADN;      bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadN;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADH;      bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadH;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADP;      bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadP;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADA;      bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadN;      bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_CAPTUREN;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureN;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREH;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureH;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREP;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureP;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREA;     bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureN;     bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_CHATHISTORYN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryN; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryH; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryP; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryN; bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_LIVESTREAMN;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpLiveStreamN;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_LIVESTREAMH;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpLiveStreamH;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_LIVESTREAMP;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpLiveStreamP;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_LIVESTREAMA;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpLiveStreamN;  bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_VIDEOCALLN;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallN;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLH;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallH;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLP;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallP;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLA;   bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallN;   bmp->w = w; bmp->h = h;

		w = 52; h = 34;
		id = XWIN5_BITMAP_SENDMESSAGEN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageN; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageH; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageP; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageH; bmp->w = w; bmp->h = h;

		w = 108; h = 14;
		id = XWIN5_BITMAP_HINTN; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTH; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTP; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTA; bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{
		U8 id;
		int offsetX = 10, offsetY = 20, gap = 20;
		int dx, dy, sw, sh;
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

		id = XWIN5_BUTTON_EMOJI;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_EMOJIN];
			bmpH = &m_bitmap[XWIN5_BITMAP_EMOJIH];
			bmpP = &m_bitmap[XWIN5_BITMAP_EMOJIP];
			bmpA = &m_bitmap[XWIN5_BITMAP_EMOJIA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_UPLOAD;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_UPLOADN];
			bmpH = &m_bitmap[XWIN5_BITMAP_UPLOADH];
			bmpP = &m_bitmap[XWIN5_BITMAP_UPLOADP];
			bmpA = &m_bitmap[XWIN5_BITMAP_UPLOADA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_CAPTURE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_CAPTUREN];
			bmpH = &m_bitmap[XWIN5_BITMAP_CAPTUREH];
			bmpP = &m_bitmap[XWIN5_BITMAP_CAPTUREP];
			bmpA = &m_bitmap[XWIN5_BITMAP_CAPTUREA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_CHATHISTORY;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_CHATHISTORYN];
			bmpH = &m_bitmap[XWIN5_BITMAP_CHATHISTORYH];
			bmpP = &m_bitmap[XWIN5_BITMAP_CHATHISTORYP];
			bmpA = &m_bitmap[XWIN5_BITMAP_CHATHISTORYA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_LIVESTREAM;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_LIVESTREAMN];
			bmpH = &m_bitmap[XWIN5_BITMAP_LIVESTREAMH];
			bmpP = &m_bitmap[XWIN5_BITMAP_LIVESTREAMP];
			bmpA = &m_bitmap[XWIN5_BITMAP_LIVESTREAMA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_VIDEOCALL;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_VIDEOCALLN];
			bmpH = &m_bitmap[XWIN5_BITMAP_VIDEOCALLH];
			bmpP = &m_bitmap[XWIN5_BITMAP_VIDEOCALLP];
			bmpA = &m_bitmap[XWIN5_BITMAP_VIDEOCALLA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_SENDMESSAGE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_SENDMESSAGEN];
			bmpH = &m_bitmap[XWIN5_BITMAP_SENDMESSAGEH];
			bmpP = &m_bitmap[XWIN5_BITMAP_SENDMESSAGEP];
			bmpA = &m_bitmap[XWIN5_BITMAP_SENDMESSAGEA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

		id = XWIN5_BUTTON_HINT;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton2;
			assert(nullptr != button);
			button->Init();
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_HINTN];
			bmpH = &m_bitmap[XWIN5_BITMAP_HINTH];
			bmpP = &m_bitmap[XWIN5_BITMAP_HINTP];
			bmpA = &m_bitmap[XWIN5_BITMAP_HINTA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setProperty(XCONTROL_PROP_STATIC);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			m_control[m_controlCount] = button;
			m_controlCount++;
		}

	}

	void UpdateControlPosition()
	{
		XControl* xctl;
		int sw, sh, dx, dy, gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		xctl = m_control[XWIN5_BUTTON_HINT];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dx = gap; dy = h - gap - sh;
		xctl->setPosition(dx, dy);

		xctl = m_control[XWIN5_BUTTON_SENDMESSAGE];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dx = w - sw - gap/2; dy = h - sh - gap/2;
		xctl->setPosition(dx, dy);

		xctl = m_control[XWIN5_BUTTON_VIDEOCALL];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop(); 
		dx = w -sw - gap;
		xctl->setPosition(dx, dy);

		xctl = m_control[XWIN5_BUTTON_LIVESTREAM];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop();
		dx = dx - sw - gap;
		xctl->setPosition(dx, dy);
	}

public:

	void PostWindowHide()
	{
	}

	void PostWindowShow()
	{
	}

	int Draw()
	{
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		return 0;
	}

	int DoTimer(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{
		int r = DUI_STATUS_NODRAW;
		return r;
	}

	int DoMouseMove(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r0 = DUI_STATUS_NODRAW;
		int r1 = DUI_STATUS_NODRAW;
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		// transfer the coordination from real window to local virutal window
		xPos -= m_area.left;
		yPos -= m_area.top;
		return 0; 
	}

	int DoLButtonDown(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = DUI_STATUS_NODRAW;
		return r; 
	}

    int DoFocusGet(U32 uMsg, int xPos, int yPos, void* lpData = nullptr) 
    { 
    	int r = DUI_STATUS_NODRAW;

		xPos -= m_area.left;
		yPos -= m_area.top;
    	return r; 
    }

    int DoFocusLose(U32 uMsg, int xPos, int yPos, void* lpData = nullptr) 
    { 
#if 0
    	m_editBox.ClearFocusedStatus();
#endif
    	return DUI_STATUS_NEEDRAW;
    } 

	int DoChar(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = DUI_STATUS_NODRAW;

		U16 charCode = static_cast<U16>(wParam);
#if 0
		bool isFocused = m_editBox.IsFocused();
		if (isFocused)
		{
			m_editBox.OnChar(charCode);
			r = DUI_STATUS_NEEDRAW;
		}
#endif
		return r; 
	}

	int DoKeyPress(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = DUI_STATUS_NODRAW;
#if 0
		U32 keyCode = static_cast<U32>(wParam);

		bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
		bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

		switch (keyCode)
		{
		case VK_RETURN:
		case VK_BACK:
		case VK_DELETE:
		case VK_TAB:
		case VK_LEFT:
			m_editBox.MoveCursorLR(-1);
			r = DUI_STATUS_NEEDRAW;
			break;
		case VK_RIGHT:
			m_editBox.MoveCursorLR(1);
			r = DUI_STATUS_NEEDRAW;
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

#endif  /* __WOCHAT_WINDOWS5_H__ */

