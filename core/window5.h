#ifndef __WOCHAT_WINDOWS5_H__
#define __WOCHAT_WINDOWS5_H__

#include "dui/dui_win.h"

//uint16_t inputMessage[XWIN_MAX_INPUTSTRING + 1] = { 0 };

enum {
	  XWIN5_EDITBOX2_INPUT = 0
	, XWIN5_BUTTON_EMOJI
	, XWIN5_BUTTON_UPLOAD
	, XWIN5_BUTTON_CAPTURE
	, XWIN5_BUTTON_CHATHISTORY
	, XWIN5_BUTTON_AUDIOCALL
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

		, XWIN5_BITMAP_AUDIOCALLN			// Normal
		, XWIN5_BITMAP_AUDIOCALLH			// Hover
		, XWIN5_BITMAP_AUDIOCALLP			// Press
		, XWIN5_BITMAP_AUDIOCALLA			// Active

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

		id = XWIN5_BITMAP_AUDIOCALLN;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallN;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLH;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallH;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLP;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallP;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLA;  bmp = &m_bitmap[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallN;  bmp->w = w; bmp->h = h;

		w = 21; h = 19;
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
		int offsetX = 10, offsetY = 5, gap = 20;
		int dx, dy, sw, sh;
		U32 objSize = sizeof(XButton);
		U8* mem;
		XBitmap* bmpN;
		XBitmap* bmpH;
		XBitmap* bmpP;
		XBitmap* bmpA;
		XButton* button;

		assert(0 == m_controlCount);
		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		id = XWIN5_EDITBOX2_INPUT;
		objSize = sizeof(XEditBox2);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XEditBox2* eb2 = new(mem)XEditBox2;
			assert(nullptr != eb2);
			if (0 != eb2->Init(g_hCursorIBeam, g_ftFace0, 16))
			{
				pfree(mem);
			}
			else
			{
				eb2->setId(id, m_controlCount);
				eb2->setRoundColor(m_backgroundColor, m_backgroundColor);
				eb2->setBkgFrontColor(0xFFBBBBBB, 0xFF555555);
				m_control[m_controlCount] = eb2;
				m_controlCount++;
			}
		}

		objSize = sizeof(XButton);
		id = XWIN5_BUTTON_EMOJI;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
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
			button = new(mem)XButton;
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
			button = new(mem)XButton;
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
			button = new(mem)XButton;
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

		id = XWIN5_BUTTON_AUDIOCALL;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			button->setId(id, m_controlCount);
			bmpN = &m_bitmap[XWIN5_BITMAP_AUDIOCALLN];
			bmpH = &m_bitmap[XWIN5_BITMAP_AUDIOCALLH];
			bmpP = &m_bitmap[XWIN5_BITMAP_AUDIOCALLP];
			bmpA = &m_bitmap[XWIN5_BITMAP_AUDIOCALLA];
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
			button = new(mem)XButton;
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
			button = new(mem)XButton;
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
			button = new(mem)XButton;
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
		int sw, sh, dx, dy, top, bottom, gap = 10; // pixel
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
		bottom = dy - gap / 2;

		xctl = m_control[XWIN5_BUTTON_VIDEOCALL];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop(); 
		dx = w - sw - gap - 1;
		xctl->setPosition(dx, dy);
		top = dy + sh + gap / 2;

		xctl = m_control[XWIN5_BUTTON_AUDIOCALL];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop();
		dx = dx - sw - gap;
		xctl->setPosition(dx, dy);

		xctl = m_control[XWIN5_EDITBOX2_INPUT];
		xctl->setPosition(0, top, w, bottom);
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


	int DoChar(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = DUI_STATUS_NODRAW;
		U16 charCode = static_cast<U16>(wParam);

		XEditBox2* eb2 = (XEditBox2*)m_control[XWIN5_EDITBOX2_INPUT];
		assert(nullptr != eb2);

		if (DUI_KEY_RETURN == charCode)
		{
			U16 msgLen = 0;
			U16* msg = eb2->getText(&msgLen);
			
			r = DUI_STATUS_NEEDRAW;
		}
		else
		{
			r = eb2->OnKeyBoard(XKEYBOARD_NORMAL, charCode);
		}

		return r;
	}

	int DoKeyPress(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) 
	{ 
		int r = DUI_STATUS_NODRAW;

		U32 keyCode = static_cast<U32>(wParam);

		bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
		bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

		XEditBox2* eb2 = (XEditBox2*)m_control[XWIN5_EDITBOX2_INPUT];
		assert(nullptr != eb2);

		switch (keyCode)
		{
		case VK_RETURN:
		case VK_BACK:
		case VK_DELETE:
		case VK_TAB:
		case VK_LEFT:
			r = eb2->MoveCursorLR(-1);
			break;
		case VK_RIGHT:
			r = eb2->MoveCursorLR(1);
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

#endif  /* __WOCHAT_WINDOWS5_H__ */

