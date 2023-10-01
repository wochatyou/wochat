#ifndef __WOCHAT_WINDOWS5_H__
#define __WOCHAT_WINDOWS5_H__

#include "dui/dui_win.h"

U16  inputMessage[XWIN_MAX_INPUTSTRING + 1] = { 0 };

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
		id = XWIN5_BITMAP_EMOJIN;       bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiN;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIH;       bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiH;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIP;       bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiP;       bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_EMOJIA;       bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpEmojiN;       bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_UPLOADN;      bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadN;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADH;      bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadH;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADP;      bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadP;      bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_UPLOADA;      bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpUploadN;      bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_CAPTUREN;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureN;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREH;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureH;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREP;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureP;     bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CAPTUREA;     bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpCaptureN;     bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_CHATHISTORYN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryN; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryH; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryP; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_CHATHISTORYA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpChatHistoryN; bmp->w = w; bmp->h = h;

		id = XWIN5_BITMAP_AUDIOCALLN;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallN;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLH;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallH;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLP;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallP;  bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_AUDIOCALLA;  bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpAudioCallN;  bmp->w = w; bmp->h = h;

		w = 21; h = 19;
		id = XWIN5_BITMAP_VIDEOCALLN;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallN;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLH;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallH;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLP;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallP;   bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_VIDEOCALLA;   bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpVideoCallN;   bmp->w = w; bmp->h = h;

		w = 52; h = 34;
		id = XWIN5_BITMAP_SENDMESSAGEN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageN; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageH; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageP; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_SENDMESSAGEA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpSendMessageH; bmp->w = w; bmp->h = h;

		w = 108; h = 14;
		id = XWIN5_BITMAP_HINTN; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTH; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTP; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
		id = XWIN5_BITMAP_HINTA; bmp = &dui_bitmapArray[id]; bmp->id = id; bmp->data = (U32*)xbmpHint; bmp->w = w; bmp->h = h;
	}

	void InitControl()
	{
		U8 id;
		int offsetX = 10, offsetY = 10, gap = 20;
		int dx, dy, sw, sh;
		U32 objSize = sizeof(XButton);
		U8* mem;
		XBitmap* bmpN;
		XBitmap* bmpH;
		XBitmap* bmpP;
		XBitmap* bmpA;
		XButton* button;

		assert(nullptr != m_pool);

		InitBitmap(); // inital all bitmap resource

		id = XWIN5_EDITBOX2_INPUT;
		m_startControl = id;
		objSize = sizeof(XEditBox2);
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			XEditBox2* eb2 = new(mem)XEditBox2;
			assert(nullptr != eb2);
			if (0 != eb2->Init(g_hCursorIBeam))
			{
				pfree(mem);
				return;
			}
			else
			{
				eb2->setRoundColor(m_backgroundColor, m_backgroundColor);
				//eb2->setBkgFrontColor(0xFFBBBBBB, 0xFF555555);
				eb2->setId(id);
				dui_controlArray[id] = eb2;
				m_endControl = id;
			}
		}
		else return;

		objSize = sizeof(XButton);
		id = XWIN5_BUTTON_EMOJI;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_EMOJIN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_EMOJIH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_EMOJIP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_EMOJIA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_UPLOAD;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_UPLOADN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_UPLOADH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_UPLOADP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_UPLOADA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_CAPTURE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_CAPTUREN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_CAPTUREH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_CAPTUREP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_CAPTUREA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_CHATHISTORY;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_CHATHISTORYN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_CHATHISTORYH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_CHATHISTORYP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_CHATHISTORYA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_AUDIOCALL;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_AUDIOCALLN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_AUDIOCALLH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_AUDIOCALLP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_AUDIOCALLA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_VIDEOCALL;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_VIDEOCALLN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_VIDEOCALLH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_VIDEOCALLP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_VIDEOCALLA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_SENDMESSAGE;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init(g_hCursorHand);
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_SENDMESSAGEN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_SENDMESSAGEH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_SENDMESSAGEP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_SENDMESSAGEA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
		else return;

		id = XWIN5_BUTTON_HINT;
		mem = (U8*)palloc(m_pool, objSize);
		if (NULL != mem)
		{
			button = new(mem)XButton;
			assert(nullptr != button);
			button->Init();
			bmpN = &dui_bitmapArray[XWIN5_BITMAP_HINTN];
			bmpH = &dui_bitmapArray[XWIN5_BITMAP_HINTH];
			bmpP = &dui_bitmapArray[XWIN5_BITMAP_HINTP];
			bmpA = &dui_bitmapArray[XWIN5_BITMAP_HINTA];
			button->setBitmap(bmpN, bmpH, bmpP, bmpA);
			sw = button->getWidth(); sh = button->getHeight();
			dx = offsetX;
			dy = offsetY;
			offsetX = dx + sw + gap;
			button->setPosition(dx, dy);
			button->setProperty(XCONTROL_PROP_STATIC);
			button->setRoundColor(m_backgroundColor, m_backgroundColor);
			button->setId(id);
			dui_controlArray[id] = button;
			m_endControl = id;
		}
	}

	void UpdateControlPosition()
	{
		XControl* xctl;
		int sw, sh, dx, dy, top, bottom, gap = 10; // pixel
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		xctl = dui_controlArray[XWIN5_BUTTON_HINT];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dx = gap; dy = h - gap - sh;
		xctl->setPosition(dx, dy);

		xctl = dui_controlArray[XWIN5_BUTTON_SENDMESSAGE];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dx = w - sw - gap/2; dy = h - sh - gap/2;
		xctl->setPosition(dx, dy);
		bottom = dy - gap / 2;

		xctl = dui_controlArray[XWIN5_BUTTON_VIDEOCALL];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop(); 
		dx = w - sw - gap - 1;
		xctl->setPosition(dx, dy);
		top = dy + sh + gap / 2;

		xctl = dui_controlArray[XWIN5_BUTTON_AUDIOCALL];
		assert(nullptr != xctl);
		sw = xctl->getWidth();
		sh = xctl->getHeight();
		dy = xctl->getTop();
		dx = dx - sw - gap;
		xctl->setPosition(dx, dy);

		xctl = dui_controlArray[XWIN5_EDITBOX2_INPUT];
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

	int Do_DUI_TIMER(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
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


	int Do_DUI_CHAR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int r = DUI_STATUS_NODRAW;

		U16 charCode = static_cast<U16>(wParam);

		if (charCode >= 0x20) // tab key
		{
			XEditBox2* eb2 = (XEditBox2*)dui_controlArray[XWIN5_EDITBOX2_INPUT];
			assert(nullptr != eb2);
			r = eb2->OnKeyBoard(XKEYBOARD_NORMAL, charCode);
		}

		return r;
	}

	int Do_DUI_KEYDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int r = DUI_STATUS_NODRAW;

		U32 keyCode = static_cast<U32>(wParam);

		bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
		bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

		XEditBox2* eb2 = (XEditBox2*)dui_controlArray[XWIN5_EDITBOX2_INPUT];
		assert(nullptr != eb2);

		switch (keyCode)
		{
		case VK_RETURN:
			{
				U16 len, *txt;
				txt = eb2->getText(&len);
				if (len > 0)
				{
					int m;
					mbedtls_chacha20_context cxt;

					for (m = 0; m < len; m++)
						inputMessage[m] = txt[m];

					PostWindowMessage(WM_UPDATE_MSG, (WPARAM)inputMessage, len);

					mbedtls_chacha20_init(&cxt);
					m = mbedtls_chacha20_setkey(&cxt, g_KEY);
					assert(0 == m);
					m = mbedtls_chacha20_starts(&cxt, g_Nonce, 0);
					assert(0 == m);
					m = mbedtls_chacha20_update(&cxt, len << 1, (const unsigned char*)txt, g_MSG);
					assert(0 == m);
					mbedtls_chacha20_free(&cxt);
					PostWindowMessage(WM_MQTT_PUBMSG, (WPARAM)g_MSG, len << 1);

					eb2->clearText();
					r++;
				}
			}
			break;
		case VK_BACK:
			r++;
			break;
		case VK_DELETE:
			r++;
			break;
		case VK_TAB:
			r++;
			break;
		case VK_LEFT:
			r++;
			r = eb2->MoveCursorLR(-1);
			break;
		case VK_RIGHT:
			r++;
			r = eb2->MoveCursorLR(1);
			break;
		case VK_UP:
		case VK_DOWN:
		case VK_HOME:
		case VK_END:
		case VK_INSERT:
		case 'C':
			if (heldControl)
			{
				r++;
			}
			break;
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

