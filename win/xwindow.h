#ifndef __WOCHAT_XWINDOW_H__
#define __WOCHAT_XWINDOW_H__

#ifndef __cplusplus
#error Wochat requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
#error xwindow.h requires atlbase.h to be included first
#endif

#ifndef __ATLWIN_H__
#error xwindow.h requires atlwin.h to be included first
#endif

#include "../core/window0.h"
#include "../core/window1.h"
#include "../core/window2.h"
#include "../core/window3.h"
#include "../core/window4.h"
#include "../core/window5.h"

/************************************************************************************************
*  The layout of the Main Window
* 
* +------+------------------------+-------------------------------------------------------------+
* |      |         Win1           |                       Win3                                  |
* |      +------------------------+-------------------------------------------------------------+
* |      |                        |                                                             |
* |      |                        |                                                             |
* |      |                        |                       Win4                                  |
* | Win0 |                        |                                                             |
* |      |         Win2           |                                                             |
* |      |                        +-------------------------------------------------------------+
* |      |                        |                                                             |
* |      |                        |                       Win5                                  |
* |      |                        |                                                             |
* +------+------------------------+-------------------------------------------------------------+
*
* 
* We have one vertical bar and three horizonal bars.
*
*************************************************************************************************
*/

static bool process_messages = true;
static int msg_count = 0;
static int last_mid = 0;
static bool timed_out = false;
static int connack_result = 0;
static bool connack_received = false;

static U8  xmsgUTF8[XWIN_MAX_INPUTSTRING + XWIN_MAX_INPUTSTRING + 1] = { 0 };
static U8  xmsgUTF8GET[XWIN_MAX_INPUTSTRING + XWIN_MAX_INPUTSTRING + 1] = { 0 };

static int PostMQTTMessage(HWND hWnd, const struct mosquitto_message* message, const mosquitto_property* properties)
{
	U8* p;
	U8* msg;
	size_t len;
	size_t bytes;

	assert(nullptr != message);

	msg = (U8*)message->payload;
	assert(nullptr != msg);
	len = (size_t)message->payloadlen;
	assert(len > 0);

	if (::IsWindow(hWnd))
	{
		for (size_t i = 0; i < len; i++)
			xmsgUTF8GET[i] = msg[i];
		::PostMessage(hWnd, WM_MQTT_SUBMSG, (WPARAM)xmsgUTF8GET, (LPARAM)len);
	}

	return 0;
}

static void MQTT_Message_Callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message, const mosquitto_property* properties)
{
	int i;
	bool res;
	struct mosq_config* pMQTTConf;
	HWND hWnd;

	UNUSED(properties);

	pMQTTConf = (struct mosq_config*)obj;
	assert(nullptr != pMQTTConf);

	hWnd = (HWND)(pMQTTConf->userdata);
	assert(::IsWindow(hWnd));

	if (process_messages == false) return;

	if (pMQTTConf->retained_only && !message->retain && process_messages)
	{
		process_messages = false;
		if (last_mid == 0)
		{
			mosquitto_disconnect_v5(mosq, 0, pMQTTConf->disconnect_props);
		}
		return;
	}

	if (message->retain && pMQTTConf->no_retain)
		return;

	if (pMQTTConf->filter_outs)
	{
		for (i = 0; i < pMQTTConf->filter_out_count; i++)
		{
			mosquitto_topic_matches_sub(pMQTTConf->filter_outs[i], message->topic, &res);
			if (res) return;
		}
	}

	if (pMQTTConf->remove_retained && message->retain)
	{
		mosquitto_publish(mosq, &last_mid, message->topic, 0, NULL, 1, true);
	}

	PostMQTTMessage(hWnd, message, properties);

	if (pMQTTConf->msg_count > 0)
	{
		msg_count++;
		if (pMQTTConf->msg_count == msg_count)
		{
			process_messages = false;
			if (last_mid == 0)
			{
				mosquitto_disconnect_v5(mosq, 0, pMQTTConf->disconnect_props);
			}
		}
	}
}

static void MQTT_Connect_Callback(struct mosquitto* mosq, void* obj, int result, int flags, const mosquitto_property* properties)
{
	int i;
	struct mosq_config* pMQTTConf;

	UNUSED(flags);
	UNUSED(properties);

	pMQTTConf = (struct mosq_config*)obj;
	assert(nullptr != pMQTTConf);

	connack_received = true;

	connack_result = result;
	if (!result)
	{
		mosquitto_subscribe_multiple(mosq, NULL, pMQTTConf->topic_count, pMQTTConf->topics, pMQTTConf->qos, pMQTTConf->sub_opts, pMQTTConf->subscribe_props);

		for (i = 0; i < pMQTTConf->unsub_topic_count; i++)
		{
			mosquitto_unsubscribe_v5(mosq, NULL, pMQTTConf->unsub_topics[i], pMQTTConf->unsubscribe_props);
		}
	}
	else
	{
		if (result)
		{
			if (pMQTTConf->protocol_version == MQTT_PROTOCOL_V5)
			{
				if (result == MQTT_RC_UNSUPPORTED_PROTOCOL_VERSION)
				{
					//err_printf(&cfg, "Connection error: %s. Try connecting to an MQTT v5 broker, or use MQTT v3.x mode.\n", mosquitto_reason_string(result));
				}
				else
				{
					//err_printf(&cfg, "Connection error: %s\n", mosquitto_reason_string(result));
				}
			}
			else {
				//err_printf(&cfg, "Connection error: %s\n", mosquitto_connack_string(result));
			}
		}
		mosquitto_disconnect_v5(mosq, 0, pMQTTConf->disconnect_props);
	}
}

static void MQTT_Disconnect_Callback(struct mosquitto* mosq, void* obj, int result, const mosquitto_property* properties)
{

}

static void MQTT_Publish_Callback(struct mosquitto* mosq, void* obj, int mid, int reason_code, const mosquitto_property* properties)
{
	int i;

	i = 1;
}

static void MQTT_Subscribe_Callback(struct mosquitto* mosq, void* obj, int mid, int qos_count, const int* granted_qos)
{
	int i;
	struct mosq_config* pMQTTConf;
	bool some_sub_allowed = (granted_qos[0] < 128);
	bool should_print;

	pMQTTConf = (struct mosq_config*)obj;
	assert(nullptr != pMQTTConf);
	should_print = pMQTTConf->debug && !pMQTTConf->quiet;

#if 0
	if (should_print)
		printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
#endif
	for (i = 1; i < qos_count; i++)
	{
		//if (should_print) printf(", %d", granted_qos[i]);
		some_sub_allowed |= (granted_qos[i] < 128);
	}
	//if (should_print) printf("\n");

	if (some_sub_allowed == false)
	{
		mosquitto_disconnect_v5(mosq, 0, pMQTTConf->disconnect_props);
		//err_printf(&cfg, "All subscription requests were denied.\n");
	}

	if (pMQTTConf->exit_after_sub)
	{
		mosquitto_disconnect_v5(mosq, 0, pMQTTConf->disconnect_props);
	}

}

static void MQTT_Log_Callback(struct mosquitto* mosq, void* obj, int level, const char* str)
{
}


static MQTT_Methods mqtt_callback =
{
	MQTT_Message_Callback,
	MQTT_Connect_Callback,
	MQTT_Disconnect_Callback,
	MQTT_Subscribe_Callback,
	MQTT_Publish_Callback,
	MQTT_Log_Callback
};

static XMQTTMessage mqtt_message = { 0 };

static HANDLE xMQTTHandles[2];

static DWORD WINAPI MQTTPubThread(LPVOID lpData)
{
	int ret;
	DWORD dwRet;
	Mosquitto q;
	HWND hWnd = (HWND)(lpData);

	ATLASSERT(::IsWindow(hWnd));

	InterlockedIncrement(&g_threadCount);

	q = MQTT::MQTT_PubInit(hWnd, mqtt_message.host, mqtt_message.port, &mqtt_callback);
	if (nullptr == q) // something is wrong in MQTT pub routine
	{
		PostMessage(hWnd, WM_INIT_THREAD, 2, 0);
		goto QuitMQTTPubThread;
	}

	while (true)
	{
		dwRet = MsgWaitForMultipleObjects(2, xMQTTHandles, FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
		case WAIT_OBJECT_0 + 0:		// the request from UI thread
		{
			char* topic = mqtt_message.topic;
			char* message = mqtt_message.message;
			int msglen = mqtt_message.msglen;
			if (nullptr != topic && nullptr != message && 0 != msglen)
			{
				ret = MQTT::MQTT_PubMessage(q, topic, message, msglen);
			}
		}
		break;
		case WAIT_OBJECT_0 + 1:		// we have to quit
			goto QuitMQTTPubThread;
		default:
			break;
		}
	}

QuitMQTTPubThread:
	MQTT::MQTT_PubTerm(q);
	InterlockedDecrement(&g_threadCount);

	return 0;
}

static DWORD WINAPI MQTTSubThread(LPVOID lpData)
{
	int ret;
	Mosquitto q;
	HWND hWnd = (HWND)(lpData);

	ATLASSERT(::IsWindow(hWnd));

	InterlockedIncrement(&g_threadCount);

	// We put some long running intialized work in the seperated thread
	// to speed up the start of WoChat application.
	ret = 0; // BitCoinInit();
	if (0 != ret) // The intialization is failed
	{
		PostMessage(hWnd, WM_INIT_THREAD, 1, 0);
		goto QuitMQTTSubThread;
	}

	q = MQTT::MQTT_SubInit(hWnd, (char*)DEFAULT_MQTT_SERVER, DEFAULT_MQTT_PORT, &mqtt_callback);

	if (nullptr == q) // something is wrong in MQTT sub routine
	{
		PostMessage(hWnd, WM_INIT_THREAD, 2, 0);
		goto QuitMQTTSubThread;
	}

	ret = MQTT::MQTT_AddSubTopic(CLIENT_SUB, (char*)g_PKeyPlain);
	if (0 != ret) // The intialization is failed
	{
		PostMessage(hWnd, WM_INIT_THREAD, 3, 0);
		goto QuitMQTTSubThread;
	}

#if 0
	ret = MQTT::MQTT_AddSubTopic(CLIENT_SUB, (char*)"WOCHAT");
	if (0 != ret) // The intialization is failed
	{
		PostMessage(hWnd, WM_INIT_THREAD, 3, 0);
		goto QuitMQTTSubThread;
	}
#endif
	MQTT::MQTT_SubLoop(q);  // main loop go here.

	MQTT::MQTT_SubTerm(q);

	q = nullptr;

QuitMQTTSubThread:
	InterlockedDecrement(&g_threadCount);
	return 0;
}

// Splitter extended styles
#define SPLIT_LEFTALIGNED		0x00000001
#define SPLIT_BOTTOMLIGNED		0x00000002

#ifdef _DEBUG
wchar_t xtitle[256] = { 0 };
#endif
class XWindow : public ATL::CWindowImpl<XWindow>
{
private:
	enum { 
		STEPXY = 1, 
		SPLITLINE_WIDTH = 1 
	};

	CToolTipCtrl m_tooltip;
	enum class DrapMode { dragModeNone, dragModeV, dragModeH };

	DrapMode m_dragMode = DrapMode::dragModeNone;

	// the memory to hold the context to paint to the screen
	U32* m_screenBuff = nullptr;
	U32  m_screenSize = 0;

	RECT m_rectClient = { 0 };
	
	int m_splitterVPos = -1;               // splitter bar position
	int m_splitterVPosNew = -1;            // internal - new position while moving
	int m_splitterVPosOld = -1;            // keep align value

	int m_splitterHPos = -1;
	int m_splitterHPosNew = -1;
	int m_splitterHPosOld = -1;

	int m_cxDragOffset = 0;		// internal
	int m_cyDragOffset = 0;		// internal

	int m_splitterVPosToLeft   = 300;       // the minum pixel to the left of the client area.
	int m_splitterVPosToRight  = 400;       // the minum pixel to the right of the client area.
	int m_splitterHPosToTop    = (XWIN3_HEIGHT + XWIN4_HEIGHT);        // the minum pixel to the top of the client area.
	int m_splitterHPosToBottom = XWIN5_HEIGHT;       // the minum pixel to the right of the client area.

	int m_marginLeft = 64;
	int m_marginRight = 0;

	int m_splitterHPosfix0 = XWIN1_HEIGHT;
	int m_splitterHPosfix1 = XWIN3_HEIGHT;

	UINT m_nDPI = 96;

	DWORD m_dwExtendedStyle = (SPLIT_LEFTALIGNED | SPLIT_BOTTOMLIGNED);    // splitter specific extended styles

	XWindow0 m_win0;
	XWindow1 m_win1;
	XWindow2 m_win2;
	XWindow3 m_win3;
	XWindow4 m_win4;
	XWindow5 m_win5;

#ifdef _DEBUG
	U32 m0 = 0;
	U32 m1 = 0;
	U32 m2 = 0;
	U32 m3 = 0;
	U32 m4 = 0;
	U32 m5 = 0;
	U32 m6 = 0;
	U32 m7 = 0;
	U32 drag = 0;
#endif
	ID2D1HwndRenderTarget* m_pD2DRenderTarget = nullptr;
	ID2D1Bitmap*           m_pixelBitmap0 = nullptr;
	ID2D1Bitmap*           m_pixelBitmap1 = nullptr;
	ID2D1Bitmap*           m_pixelBitmap2 = nullptr;
	ID2D1SolidColorBrush*  m_pTextBrush0 = nullptr;
	ID2D1SolidColorBrush*  m_pTextBrush1 = nullptr;

public:
	DECLARE_XWND_CLASS(NULL, IDR_MAINFRAME, 0)

	XWindow() 
	{
		m_rectClient.left = m_rectClient.right = m_rectClient.top = m_rectClient.bottom = 0;
		m_win0.SetWindowId((const U8*)"DUIWin0", 7);
		m_win1.SetWindowId((const U8*)"DUIWin1", 7);
		m_win2.SetWindowId((const U8*)"DUIWin2", 7);
		m_win3.SetWindowId((const U8*)"DUIWin3", 7);
		m_win4.SetWindowId((const U8*)"DUIWin4", 7);
		m_win5.SetWindowId((const U8*)"DUIWin5", 7);
	}

	~XWindow() 
	{
		if (nullptr != m_screenBuff)
			VirtualFree(m_screenBuff, 0, MEM_RELEASE);

		m_screenBuff = nullptr;
		m_screenSize = 0;
	}

	int SetToolTips(int startIdx, int endIdx, int dx, int dy)
	{
		RECT area, *r;
		r = &area;

		XControl* xctl;
		for (int i = startIdx; i <= endIdx; i++)
		{
			xctl = dui_controlArray[i];
			ATLASSERT(nullptr != xctl);
			ATLASSERT(xctl->right > xctl->left);
			ATLASSERT(xctl->bottom > xctl->top);
			r->left = xctl->left + dx;
			r->top = xctl->top + dy;
			r->right = r->left + (xctl->right - xctl->left);
			r->bottom = r->top + (xctl->bottom - xctl->top);
			m_tooltip.DelTool(m_hWnd, xctl->m_id);
			m_tooltip.AddTool(m_hWnd, LPSTR_TEXTCALLBACK, r, xctl->m_id);
		}
		return 0;
	}

	BEGIN_MSG_MAP(XWindow)
		MESSAGE_HANDLER_DUIWINDOW(DUI_ALLMESSAGE, OnDUIWindowMessage)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_MQTT_PUBMSG, OnMQTTPubMessage)
		MESSAGE_HANDLER(WM_MQTT_SUBMSG, OnMQTTSubMessage)
		MESSAGE_HANDLER(WM_UPDATE_MSG, OnUpdateMessage)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnGetToolTipInfo)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDoubleClick)
		MESSAGE_HANDLER(WM_XWINDOWSPAINT, OnDrawWindow)
		MESSAGE_HANDLER(WM_XWINDOWS00, OnWin0Message)
		MESSAGE_HANDLER(WM_XWINDOWS01, OnWin1Message)
		MESSAGE_HANDLER(WM_XWINDOWS02, OnWin2Message)
		MESSAGE_HANDLER(WM_XWINDOWS03, OnWin3Message)
		MESSAGE_HANDLER(WM_XWINDOWS04, OnWin4Message)
		MESSAGE_HANDLER(WM_XWINDOWS05, OnWin5Message)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_INIT_THREAD, OnInitThread)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnDUIWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		WPARAM wp = wParam;
		LPARAM lp = lParam;

		switch (uMsg)
		{
		case WM_SETCURSOR:
			{
				DWORD dwPos = ::GetMessagePos();
				POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
				ScreenToClient(&ptPos);
				wp = (WPARAM)ptPos.x;
				lp = (LPARAM)ptPos.y;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				POINT pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);
				ScreenToClient(&pt);
				lp = MAKELONG(pt.x, pt.y);
			}
			break;
		case WM_CREATE:
			wp = (WPARAM)m_hWnd;
			break;
		default:
			break;
		}

		m_win0.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win1.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win2.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win3.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win4.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);
		m_win5.HandleOSMessage((U32)uMsg, (U64)wp, (U64)lp);

		{
			if (DUIWindowNeedReDraw())
				Invalidate();
		}
		// to allow the host window to continue to handle the windows message
		bHandled = FALSE;
		return 0; 
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0; // don't want flicker
	}

	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
#if 0
		if (m_tooltip.IsWindow()) 
		{
			MSG msg = { m_hWnd, uMsg, wParam, lParam };
			m_tooltip.RelayEvent(&msg);
		}
#endif
		bHandled = FALSE; // allow further mouse event process. It is important.
		return 0;
	}

	LRESULT OnInitThread(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MessageBox(_T("Initilization failed!"), _T("WoChat"), MB_OK);

		PostMessage(WM_CLOSE);

		return 0;
	}

	LRESULT OnUpdateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		U16* msg = (U16*)wParam;
		U16 len = (U16)lParam;

		if (len > 0)
		{
			m_win4.UpdateChatHistory(msg, len, 1);
			Invalidate();
		}
		return 0;
	}

	
	LRESULT OnMQTTPubMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int i;
		U8* data = (U8*)wParam;
		U16 len = (U16)lParam;
		size_t size;

		U8* p = xmsgUTF8;
		for (i = 0; i < 88; i++)
			p[i] = g_PKeyPlain[i];
		p += 66;
		p[0] = '|';
		p++;
		size = modp_b64_encode((char*)p, (const char*)data, len);
		mqtt_message.topic = (char*)g_PKey1Plain;
		mqtt_message.message = (char*)xmsgUTF8;
		mqtt_message.msglen = size + 67;
		SetEvent(xMQTTHandles[0]); // send this message to remote
		return 0;
	}

	LRESULT OnMQTTSubMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		U8* msg = (U8*)wParam;
		U16 len = (U16)lParam;
		U8 pk[33] = { 0 };
		U8 K[32] = { 0 };

		if (len > 67)
		{
			size_t size;
			int m;
			mbedtls_chacha20_context cxt;
			U8 cH, cL;
			U8* p = msg;
			assert('|' == p[66]);

			for (int i = 0; i < 33; i++)
			{
				cH = msg[i << 1]; cL = msg[(i << 1) + 1];
				if (!IsHexLetter(cH))
					return -1;
				if (!IsHexLetter(cL))
					return -1;

				if (cH <= '9') cH = cH - '0';
				else if (cH <= 'F') cH = (cH - 'A') + 10;
				else cH = (cH - 'a') + 10;

				if (cL <= '9') cL = cL - '0';
				else if (cL <= 'F') cL = (cL - 'A') + 10;
				else cL = (cL - 'a') + 10;
				pk[i] = cH << 4 | cL;
			}
			GetKeyfromSKPK(g_SKey, pk, K);
			size = modp_b64_decode((char*)g_MSG, (const char*)p + 67, len - 67);

			mbedtls_chacha20_init(&cxt);
			m = mbedtls_chacha20_setkey(&cxt, K);
			assert(0 == m);
			m = mbedtls_chacha20_starts(&cxt, g_Nonce, 0);
			assert(0 == m);
			m = mbedtls_chacha20_update(&cxt, size, (const unsigned char*)g_MSG, g_MSG);
			assert(0 == m);
			mbedtls_chacha20_free(&cxt);
			m_win4.UpdateChatHistory((U16*)g_MSG, size >> 1);
			Invalidate();
		}

		return 0;
	}

	LRESULT OnGetToolTipInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMTTDISPINFO pToolTipInfo = (LPNMTTDISPINFO)pnmh;

		if (pToolTipInfo) 
		{
			UINT id = pToolTipInfo->hdr.idFrom;
			ATLASSERT(id > 0);
			ATLASSERT(id < DUI_MAX_CONTROLS);
			pToolTipInfo->lpszText = (LPWSTR)dui_tooltip[id];
		}
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SetEvent(xMQTTHandles[1]); // tell MQTT pub thread to quit gracefully
		Sleep(1000);
		
		CloseHandle(xMQTTHandles[0]);
		CloseHandle(xMQTTHandles[1]);

		KillTimer(XWIN_666MS_TIMER);

		SafeRelease(&m_pTextBrush0);
		SafeRelease(&m_pTextBrush1);
		SafeRelease(&m_pixelBitmap0);
		SafeRelease(&m_pixelBitmap1);
		SafeRelease(&m_pixelBitmap2);
		SafeRelease(&m_pD2DRenderTarget);

		if (m_tooltip.IsWindow()) 
		{
			m_tooltip.DestroyWindow();
		}

		PostQuitMessage(0);
		return 0;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DWORD dwThreadID0;
		DWORD dwThreadID1;
		HANDLE hThread0;
		HANDLE hThread1;

		xMQTTHandles[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
		xMQTTHandles[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (NULL == xMQTTHandles[0] || NULL == xMQTTHandles[1])
		{
			PostMessage(WM_CLOSE);
			return 0;
		}

		mqtt_message.host = (char*)DEFAULT_MQTT_SERVER;
		mqtt_message.port = DEFAULT_MQTT_PORT;
		mqtt_message.topic = nullptr;
		mqtt_message.message = nullptr;
		mqtt_message.msglen = 0;
#if 0
		hThread0 = ::CreateThread(NULL, 0, MQTTSubThread, m_hWnd, 0, &dwThreadID0);
		hThread1 = ::CreateThread(NULL, 0, MQTTPubThread, m_hWnd, 0, &dwThreadID1);

		if (nullptr == hThread0 || nullptr == hThread1)
		{
			MessageBox(TEXT("MQTT thread creation is failed!"), TEXT("WoChat Error"), MB_OK);
		}
#endif
		if (DUIWindowInitFailed())
		{
			MessageBox(_T("WM_CREATE failed!"), _T("Error"), MB_OK);
			PostMessage(WM_CLOSE);
			return 0;
		}


		m_nDPI = GetDpiForWindow(m_hWnd);

		/* Initialize Tooltips */
		ATLASSERT(FALSE == m_tooltip.IsWindow());
		// Be sure InitCommonControlsEx is called before this, 
		// with one of the flags that includes the tooltip control
		m_tooltip.Create(m_hWnd, NULL, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP /* | TTS_BALLOON */, WS_EX_TOOLWINDOW);
		if (m_tooltip.IsWindow()) 
		{
			RECT area = { 0 };
			UINT ms = ::GetDoubleClickTime();
			m_tooltip.SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			m_tooltip.SetDelayTime(TTDT_INITIAL, ms * 8);
			//m_tooltip.SetDelayTime(TTDT_AUTOPOP, ms * 1000);
			//m_tooltip.SetDelayTime(TTDT_RESHOW,  ms * 1000);
			m_tooltip.Activate(TRUE);
		}
		
		{
			XChatGroup* cg = m_win2.GetSelectedChatGroup();
			ATLASSERT(nullptr != cg);
			m_win3.UpdateTitle(cg->name);
			m_win4.SetChatGroup(cg);
		}

		SetTimer(XWIN_666MS_TIMER, 666);

		return 0;
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

		lpMMI->ptMinTrackSize.x = 800;
		lpMMI->ptMinTrackSize.y = 600;

		return 0;
	}

	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = DUIWindowCursorIsSet() ? TRUE : FALSE;

		ClearDUIWindowCursor();

		if(((HWND)wParam == m_hWnd) && (LOWORD(lParam) == HTCLIENT))
		{
			DWORD dwPos = ::GetMessagePos();

			POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };

			ScreenToClient(&ptPos);

			DrapMode mode = IsOverSplitterBar(ptPos.x, ptPos.y);
			if(DrapMode::dragModeNone != mode)
				bHandled = TRUE;
		}
		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (SIZE_MINIMIZED != wParam)
		{
			XRECT area;
			XRECT* r = &area;

			GetClientRect(&m_rectClient);
			ATLASSERT(0 == m_rectClient.left);
			ATLASSERT(0 == m_rectClient.top);
			ATLASSERT(m_rectClient.right > 0);
			ATLASSERT(m_rectClient.bottom > 0);

			U32 w = (U32)(m_rectClient.right - m_rectClient.left);
			U32 h = (U32)(m_rectClient.bottom - m_rectClient.top);

			if (nullptr != m_screenBuff)
			{
				VirtualFree(m_screenBuff, 0, MEM_RELEASE);
				m_screenBuff = nullptr;
				m_screenSize = 0;
			}
			m_screenSize = DUI_ALIGN_PAGE(w * h * sizeof(U32));
			ATLASSERT(m_screenSize >= (w * h * sizeof(U32)));

			m_screenBuff = (U32*)VirtualAlloc(NULL, m_screenSize, MEM_COMMIT, PAGE_READWRITE);
			if (nullptr == m_screenBuff)
			{
				m_win0.UpdateSize(nullptr, nullptr);
				m_win1.UpdateSize(nullptr, nullptr);
				m_win2.UpdateSize(nullptr, nullptr);
				m_win3.UpdateSize(nullptr, nullptr);
				m_win4.UpdateSize(nullptr, nullptr);
				m_win5.UpdateSize(nullptr, nullptr);
				Invalidate();
				return 0;
			}

			ATLASSERT(m_splitterVPosToLeft >= 0);
			ATLASSERT(m_splitterVPosToRight >= 0);
			ATLASSERT(m_splitterHPosToTop >= 0);
			ATLASSERT(m_splitterHPosToBottom >= 0);

			if (m_splitterVPos < 0)
			{
				m_splitterVPos = (XWIN0_WIDTH + XWIN1_WIDTH);
				if (m_splitterVPos < m_splitterVPosToLeft)
					m_splitterVPos = m_splitterVPosToLeft;

				if (m_splitterVPos > (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight))
				{
					m_splitterVPos = (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight);
					ATLASSERT(m_splitterVPos > m_splitterVPosToLeft);
				}
				m_splitterVPosOld = m_splitterVPos;
			}

			m_splitterVPos = m_splitterVPosOld;

			if (m_splitterHPos < 0)
			{
				m_splitterHPos = m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom;
				if (m_splitterHPos < m_splitterHPosToTop)
					m_splitterHPos = m_splitterHPosToTop;

				if (m_splitterHPos > (m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom))
				{
					m_splitterHPos = m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom;
					ATLASSERT(m_splitterHPos > m_splitterHPosToTop);
				}
				m_splitterHPosOld = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPos;
			}

			if (m_splitterHPos > 0) // if(m_splitterHPos <= 0) then windows 5 is hidden
			{
				m_splitterHPos = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPosOld;
			}

			SafeRelease(&m_pD2DRenderTarget);
			if (nullptr != m_screenBuff)
			{
				U32* dst = m_screenBuff;
				U32 size;

				r->left = m_rectClient.left;
				r->right = XWIN0_WIDTH;
				r->top = m_rectClient.top;
				r->bottom = m_rectClient.bottom;
				m_win0.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = XWIN0_WIDTH;
				r->right = m_splitterVPos;
				r->top = m_rectClient.top;
				r->bottom = m_splitterHPosfix0;
				m_win1.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = XWIN0_WIDTH;
				r->right = m_splitterVPos;
				r->top = m_splitterHPosfix0 + SPLITLINE_WIDTH;
				r->bottom = m_rectClient.bottom;
				m_win2.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = m_splitterVPos + SPLITLINE_WIDTH;
				r->right = m_rectClient.right;
				r->top = m_rectClient.top;
				r->bottom = m_splitterHPosfix1;
				m_win3.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = m_splitterVPos + SPLITLINE_WIDTH;
				r->right = m_rectClient.right;
				r->top = m_splitterHPosfix1 + SPLITLINE_WIDTH;
				r->bottom = m_splitterHPos;
				m_win4.UpdateSize(r, dst);
				size = (U32)((r->right - r->left) * (r->bottom - r->top));
				dst += size;

				r->left = m_splitterVPos + SPLITLINE_WIDTH;
				r->right = m_rectClient.right;
				r->top = m_splitterHPos + SPLITLINE_WIDTH;
				r->bottom = m_rectClient.bottom;
				m_win5.UpdateSize(r, dst);
			}
		}

		Invalidate();
		return 0;
	}

	// When the virtial or horizonal bar is changed, 
	// We need to change the position of windows 1/2/3/4/5
	void AdjustDUIWindowPosition()
	{
		if (nullptr != m_screenBuff)
		{
			U32* dst;
			U32  size;

			// window 0 does not need to change
			XRECT area, *r;
			XRECT* xr = m_win0.GetWindowArea();

			area.left = xr->left;  area.top = xr->top; area.right = xr->right; area.bottom = xr->bottom;
			r = &area;

			dst = m_screenBuff;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			dst += size;

			// windows 1
			r->left = r->right; r->right = m_splitterVPos; r->top = m_rectClient.top; r->bottom = m_splitterHPosfix0;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			m_win1.UpdateSize(r, dst);
			dst += size;

			// windows 2
			r->top = m_splitterHPosfix0 + SPLITLINE_WIDTH; r->bottom = m_rectClient.bottom;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			m_win2.UpdateSize(r, dst);

			// windows 3
			dst += size;
			r->left = m_splitterVPos + SPLITLINE_WIDTH; r->right = m_rectClient.right; r->top = m_rectClient.top; r->bottom = m_splitterHPosfix1;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			m_win3.UpdateSize(r, dst);

			// windows 4
			dst += size;
			r->top = m_splitterHPosfix1 + SPLITLINE_WIDTH; r->bottom = m_splitterHPos;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			m_win4.UpdateSize(r, dst);

			// windows 5
			dst += size;
			r->top = m_splitterHPos + SPLITLINE_WIDTH; r->bottom = m_rectClient.bottom;
			size = (U32)((r->right - r->left) * (r->bottom - r->top));
			m_win5.UpdateSize(r, dst);
		}
	}

	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int r = 0;
#ifdef _DEBUG
		m7++;
#endif
		return r;
	}

	LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int r = 0;
#ifdef _DEBUG
		m6++;
#endif
		return r;
	}
	
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		if (!DUIWindowInDragMode())
		{
			bool bChanged = false;
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			if (::GetCapture() == m_hWnd)
			{
				int	newSplitPos;
				switch (m_dragMode)
				{
				case DrapMode::dragModeV:
					{
						newSplitPos = xPos - m_cxDragOffset;
						if (newSplitPos == -1)   // avoid -1, that means default position
							newSplitPos = -2;
						if (m_splitterVPos != newSplitPos)
						{
							if (newSplitPos >= m_splitterVPosToLeft && newSplitPos < (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight))
							{
								if (SetSplitterPos(newSplitPos, true))
									bChanged = true;
							}
						}
					}
					break;
				case DrapMode::dragModeH:
					{
						newSplitPos = yPos - m_cyDragOffset;
						if (newSplitPos == -1)   // avoid -1, that means default position
							newSplitPos = -2;
						if (m_splitterHPos != newSplitPos)
						{
							if (newSplitPos >= m_splitterHPosToTop && newSplitPos < (m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom))
							{
								if (SetSplitterPos(newSplitPos, false))
									bChanged = true;
							}
						}
					}
					break;
				default:
					break;
				}
				if (bChanged)
					AdjustDUIWindowPosition();
			}
			else	// not dragging, just set cursor
			{
				DrapMode mode = IsOverSplitterBar(xPos, yPos);
				switch (mode)
				{
				case DrapMode::dragModeV:
					ATLASSERT(nullptr != g_hCursorWE);
					::SetCursor(g_hCursorWE);
					break;
				case DrapMode::dragModeH:
					ATLASSERT(nullptr != g_hCursorNS);
					::SetCursor(g_hCursorNS);
					break;
				default:
					break;
				}
				if (DrapMode::dragModeNone == mode && m_tooltip.IsWindow())
				{
					MSG msg = { m_hWnd, uMsg, wParam, lParam };
					m_tooltip.RelayEvent(&msg);
				}
			}
			if (bChanged)
				Invalidate();
		}

		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		if (DUIWindowInDragMode())
		{
			if (::GetCapture() != m_hWnd)
			{
				SetCapture();
#ifdef _DEBUG
				drag = 1;
#endif
			}
		}
		else
		{
			m_dragMode = IsOverSplitterBar(xPos, yPos);
			if (::GetCapture() != m_hWnd)
			{
				switch (m_dragMode)
				{
				case DrapMode::dragModeV:
					m_splitterVPosNew = m_splitterVPos;
					m_cxDragOffset = xPos - m_splitterVPos;
					ATLASSERT(nullptr != g_hCursorWE);
					::SetCursor(g_hCursorWE);
					break;
				case DrapMode::dragModeH:
					m_splitterHPosNew = m_splitterHPos;
					m_cyDragOffset = yPos - m_splitterHPos;
					ATLASSERT(nullptr != g_hCursorNS);
					::SetCursor(g_hCursorNS);
					break;
				default:
					break;
				}

				if (DrapMode::dragModeNone != m_dragMode)
					SetCapture();
			}
			else
			{
				ATLASSERT(::GetCapture() == m_hWnd);
				::ReleaseCapture();
				m_dragMode = DrapMode::dragModeNone;
			}
		}

		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLASSERT(!DUIWindowInDragMode());

		if (::GetCapture() == m_hWnd)
		{
			::ReleaseCapture();

			switch (m_dragMode)
			{
			case DrapMode::dragModeV:
				m_splitterVPosNew = m_splitterVPos;
				break;
			case DrapMode::dragModeH:
				m_splitterHPosNew = m_splitterHPos;
				break;
			default:
				break;
			}
			m_dragMode = DrapMode::dragModeNone;
#ifdef _DEBUG
			drag = 0;
#endif
		}
		return 0;
	}

	LRESULT OnLButtonDoubleClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (!DUIWindowInDragMode())
		{
#if 0
			bool bChanged = false;
			if ((m_splitterVPosNew != -1) && (m_splitterVPos != m_splitterVPosNew))
			{
				m_splitterVPos = m_splitterVPosNew;
				m_splitterVPosNew = -1;
				bChanged = true;
				m_splitterVPosOld = m_splitterVPos;
				//UpdateWindow();
			}
			// m_splitterHPos may <= 0 which means window 5 is hidden
			if ((m_splitterHPos > 0) && (m_splitterHPosNew != -1) && (m_splitterHPos != m_splitterHPosNew))
			{
				m_splitterHPos = m_splitterHPosNew;
				m_splitterHPosNew = -1;
				bChanged = true;

				m_splitterHPosOld = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPos;
				//UpdateWindow();
			}
			if (bChanged)
			{
				AdjustDUIWindowPosition();
				Invalidate();
			}
#endif
		}
		return 0;
	}

	LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(::GetCapture() == m_hWnd)
		{
			switch(wParam)
			{
			case VK_RETURN:
				m_splitterVPosNew = m_splitterVPos;
				// FALLTHROUGH
			case VK_ESCAPE:
				::ReleaseCapture();
				break;
			case VK_LEFT:
			case VK_RIGHT:
				{
					POINT pt = {};
					::GetCursorPos(&pt);
					int xyPos = m_splitterVPos + ((wParam == VK_LEFT) ? -STEPXY : STEPXY);
					int cxyMax = m_rectClient.right - m_rectClient.left;

					ATLASSERT(m_splitterVPosToLeft > 100);
					ATLASSERT(m_splitterVPosToRight > 100);

					if (xyPos >= m_splitterVPosToLeft && xyPos < (cxyMax - SPLITLINE_WIDTH - m_splitterVPosToRight))
					{
						pt.x += xyPos - m_splitterVPos;
						::SetCursorPos(pt.x, pt.y);
					}
				}
				break;
			case VK_UP:
			case VK_DOWN:
				{
					POINT pt = {};
					::GetCursorPos(&pt);
					int xyPos = m_splitterHPos + ((wParam == VK_UP) ? -STEPXY : STEPXY);
					int cxyMax = m_rectClient.bottom - m_rectClient.top;
			
					ATLASSERT(m_splitterHPosToTop > 100);
					ATLASSERT(m_splitterHPosToBottom > 100);

					if (xyPos >= m_splitterHPosToTop && xyPos < (cxyMax - m_splitterHPosToBottom))
					{
						pt.y += xyPos - m_splitterHPos;
						::SetCursorPos(pt.x, pt.y);
					}
			}
				break;
			default:
				break;
			}
		}

		return 0;
	}

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM, BOOL& bHandled)
	{
#if 0
		if(::GetCapture() != m_hWnd)
		{
			if(m_nSinglePane == SPLIT_PANE_NONE)
			{
				if((m_nDefActivePane == SPLIT_PANE_LEFT) || (m_nDefActivePane == SPLIT_PANE_RIGHT))
				{
					//::SetFocus(m_hWndPane[m_nDefActivePane]);
				}
			}
			else
			{
				//::SetFocus(m_hWndPane[m_nSinglePane]);
			}
		}
#endif
		return 0;
	}

	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		if((lRet == MA_ACTIVATE) || (lRet == MA_ACTIVATEANDEAT))
		{
			DWORD dwPos = ::GetMessagePos();
			POINT pt = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			ScreenToClient(&pt);
			RECT rcPane = {};
#if 0
			for(int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if(GetSplitterPaneRect(nPane, &rcPane) && (::PtInRect(&rcPane, pt) != FALSE))
				{
					m_nDefActivePane = nPane;
					break;
				}
			}
#endif
		}

		return 0;
	}

	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT OnWin0Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == 0)
		{
			U8 ctlId = (U8)lParam;
		}
		else
		{
			int startIdx = (int)wParam;
			int endIdx = (int)lParam;
			assert(startIdx > 0);
			assert(endIdx >= startIdx);
			XRECT* xr = m_win0.GetWindowArea();
			ATLASSERT(xr->left >= 0);
			ATLASSERT(xr->top >= 0);
			ATLASSERT(xr->right > xr->left);
			ATLASSERT(xr->bottom > xr->top);

			SetToolTips(startIdx, endIdx, xr->left, xr->top);
		}

		return 0;
	}

	LRESULT OnWin1Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		U8 buttonId = (U8)wParam;
		return 0;
	}

	LRESULT OnWin2Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int r = 0;
		XChatGroup* cg = (XChatGroup*)wParam;
		ATLASSERT(nullptr != cg);

		r += m_win3.UpdateTitle(cg->name);
		r += m_win4.SetChatGroup(cg);

		if (r)
			Invalidate();

		return 0;
	}

	LRESULT OnWin3Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		U8 buttonId = (U8)wParam;
		return 0;
	}

	LRESULT OnWin4Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnWin5Message(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == 0)
		{
			U8 ctlId = (U8)lParam;
		}
		else
		{
			int startIdx = (int)wParam;
			int endIdx = (int)lParam;
			assert(startIdx > 0);
			assert(endIdx >= startIdx);
			XRECT* xr = m_win5.GetWindowArea();
			ATLASSERT(xr->left >= 0);
			ATLASSERT(xr->top >= 0);
			ATLASSERT(xr->right > xr->left);
			ATLASSERT(xr->bottom > xr->top);

			SetToolTips(startIdx, endIdx, xr->left, xr->top);
		}

		return 0;
	}

	LRESULT OnDrawWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		Invalidate();
		//UpdateWindow();
		return 0; // don't want flicker
	}

	LRESULT OnNCPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
#if 0
		int w, h;
		HDC hdc;
		RECT rc, rw, rn;

		GetWindowRect(&rw);
		GetClientRect(&rc);

		w = rw.right - rw.left;
		h = rw.bottom - rw.top - (rc.bottom - rc.top);
		rn.left = rw.left; rn.right = rw.right;
		rn.top = rw.top; rn.bottom = rn.top + h;

		bHandled = FALSE;
		//hdc = GetDCEx((HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
		hdc = GetWindowDC();
		if (NULL != hdc)
		{
			HDC hdcMem = ::CreateCompatibleDC(hdc);
			if (NULL != hdcMem)
			{
				HBITMAP hBitmapMem = ::CreateCompatibleBitmap(hdc, w, h);
				if (NULL != hBitmapMem)
				{
					HBITMAP hBitmapOld = (HBITMAP)::SelectObject(hdcMem, hBitmapMem);
					if (NULL != hBitmapOld)
					{
						HBRUSH brush = ::CreateSolidBrush(RGB(255, 0, 0));
						if (NULL != brush)
						{
							int ret = ::FillRect(hdcMem, &rn, brush);
							if (0 != ret)
							{
								BOOL bRet = ::BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
								if (bRet)
								{
									bHandled = TRUE;
								}
							}
							::DeleteObject(brush);
						}
						::DeleteObject(hBitmapOld);
					}
				}
				::DeleteDC(hdcMem);
			}
			ReleaseDC(hdc);
		}
#endif
		bHandled = FALSE;
		return 0;
	}


	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		HRESULT hr = S_OK;
		PAINTSTRUCT ps;
		BeginPaint(&ps);

		if (nullptr == m_pD2DRenderTarget)
		{
			D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);

			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
			renderTargetProperties.dpiX = m_nDPI;
			renderTargetProperties.dpiY = m_nDPI;
			renderTargetProperties.pixelFormat = pixelFormat;

			D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTragetproperties
				= D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(m_rectClient.right - m_rectClient.left, m_rectClient.bottom - m_rectClient.top));

			ATLASSERT(nullptr != g_pD2DFactory);
			hr = g_pD2DFactory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTragetproperties, &m_pD2DRenderTarget);
			if (S_OK == hr && nullptr != m_pD2DRenderTarget)
			{
				U32 pixel[1] = { 0xFFEEEEEE };
				SafeRelease(&m_pixelBitmap0);
				hr = m_pD2DRenderTarget->CreateBitmap(
					D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
					&m_pixelBitmap0);
				if (S_OK == hr && nullptr != m_pixelBitmap0)
				{
					pixel[0] = 0xFFFFFFFF;
					hr = m_pD2DRenderTarget->CreateBitmap(
						D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
						&m_pixelBitmap1);
					ATLASSERT(S_OK == hr);

					pixel[0] = 0xFF6AEA9E;
					hr = m_pD2DRenderTarget->CreateBitmap(
						D2D1::SizeU(1, 1), pixel, 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
						&m_pixelBitmap2);
					ATLASSERT(S_OK == hr);

					hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x666666), &m_pTextBrush0);
					if (S_OK == hr && nullptr != m_pTextBrush0)
					{
						hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x000000), &m_pTextBrush1);
					}
				}

			}
		}

		if (S_OK == hr && nullptr != m_pD2DRenderTarget && nullptr != m_pixelBitmap0 && nullptr != m_pTextBrush0 && nullptr != m_pTextBrush1)
		{
			int w, h;
			U32* src = nullptr;
			U16 count;
			XTextDrawInfo* p;
			D2D1_RECT_F clipRect;
			D2D1_RECT_F layoutRect;

			m_pD2DRenderTarget->BeginDraw();
			////////////////////////////////////////////////////////////////////////////////////////////////////
			//m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			//m_pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			// draw seperator lines
			if (nullptr != m_pixelBitmap0)
			{
				if(m_splitterVPos > 0)
				{
					D2D1_RECT_F rect = D2D1::RectF(
						static_cast<FLOAT>(m_splitterVPos),
						static_cast<FLOAT>(m_rectClient.top),
						static_cast<FLOAT>(m_splitterVPos + SPLITLINE_WIDTH), // m_cxySplitBar),
						static_cast<FLOAT>(m_rectClient.bottom)
					);
					m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap0, &rect);
				}
				if (m_splitterHPosfix0 > 0)
				{
					D2D1_RECT_F rect = D2D1::RectF(
						static_cast<FLOAT>(XWIN0_WIDTH),
						static_cast<FLOAT>(m_splitterHPosfix0),
						static_cast<FLOAT>(m_splitterVPos),
						static_cast<FLOAT>(m_splitterHPosfix0 + SPLITLINE_WIDTH)
					);
					m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap0, &rect);
				}
				if (m_splitterHPosfix1 > 0)
				{
					D2D1_RECT_F rect = D2D1::RectF(
						static_cast<FLOAT>(m_splitterVPos + 2),
						static_cast<FLOAT>(m_splitterHPosfix1),
						static_cast<FLOAT>(m_rectClient.right),
						static_cast<FLOAT>(m_splitterHPosfix1 + SPLITLINE_WIDTH)
					);
					m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap0, &rect);
				}
				if (m_splitterHPos > 0)
				{
					D2D1_RECT_F rect = D2D1::RectF(
						static_cast<FLOAT>(m_splitterVPos + SPLITLINE_WIDTH),
						static_cast<FLOAT>(m_splitterHPos),
						static_cast<FLOAT>(m_rectClient.right),
						static_cast<FLOAT>(m_splitterHPos + SPLITLINE_WIDTH)
					);
					m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap0, &rect);
				}
			}

			// draw window 0
			src = m_win0.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m0++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win0.GetWindowArea();
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);
				m_win0.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 1
			src = m_win1.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m1++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win1.GetWindowArea();
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);
				m_win1.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 2
			src = m_win2.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m2++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win2.GetWindowArea();
				clipRect.left = xr->left; clipRect.right = xr->right; clipRect.top = xr->top; clipRect.bottom = xr->bottom;
				m_pD2DRenderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);

				p = m_win2.GetTextDrawInfo(&count);
				while (nullptr != p && count > 0)
				{
					layoutRect.left = p->left + xr->left; layoutRect.top = p->top + xr->top + 10;
					layoutRect.right = p->right + xr->left; layoutRect.bottom = layoutRect.top + 20; // p->bottom + xr->top;
					m_pD2DRenderTarget->DrawText((const WCHAR*)p->text0, p->textLen0, g_pTextFormatTitle, layoutRect, m_pTextBrush0);

					layoutRect.bottom = p->bottom + xr->top - 10;
					layoutRect.top = layoutRect.bottom - 16;
					m_pD2DRenderTarget->DrawText((const WCHAR*)p->text1, p->textLen1, g_pTextFormatMessageSmall0, layoutRect, m_pTextBrush0);

					layoutRect.left = p->left + xr->left; layoutRect.top = p->top + xr->top + 10;
					layoutRect.right = xr->right - 10; layoutRect.bottom = layoutRect.top + 20; // p->bottom + xr->top;
					m_pD2DRenderTarget->DrawText((const WCHAR*)p->text2, p->textLen2, g_pTextFormatMessageSmall1, layoutRect, m_pTextBrush0);

					p = p->next;
					count--;
					if (0 == count)
						break;
				}

				m_pD2DRenderTarget->PopAxisAlignedClip();
				m_win2.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 3
			src = m_win3.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m3++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win3.GetWindowArea();
				clipRect.left = xr->left; clipRect.right = xr->right; clipRect.top = xr->top; clipRect.bottom = xr->bottom;
				m_pD2DRenderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);

				p = m_win3.GetTextDrawInfo(&count);
				while (nullptr != p && count > 0)
				{
					layoutRect.left = p->left + xr->left; layoutRect.top = p->top + xr->top;
					layoutRect.right = xr->right; layoutRect.bottom = p->bottom + xr->top;
					m_pD2DRenderTarget->DrawText((const WCHAR*)p->text0, p->textLen0, g_pTextFormatTitle, layoutRect, m_pTextBrush0);
					p = p->next;
					count--;
					if (0 == count)
						break;
				}
				m_pD2DRenderTarget->PopAxisAlignedClip();
				m_win3.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 4
			src = m_win4.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m4++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win4.GetWindowArea();
				clipRect.left = xr->left; clipRect.right = xr->right; clipRect.top = xr->top; clipRect.bottom = xr->bottom;
				m_pD2DRenderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);

				p = m_win4.GetTextDrawInfo(&count);
				while (nullptr != p && count > 0)
				{
					layoutRect.left = p->left + xr->left + 4; layoutRect.top = p->top + xr->top;
					layoutRect.right = p->right + xr->left; layoutRect.bottom = p->bottom + xr->top - 2 ;
					if(XTEXTDRAWINFO_ISME & p->status)
						m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap2, &layoutRect);
					else 
						m_pD2DRenderTarget->DrawBitmap(m_pixelBitmap1, &layoutRect);
					layoutRect.left += 4;
					m_pD2DRenderTarget->DrawText((const WCHAR*)p->text0, p->textLen0, g_pTextFormatMessage, layoutRect, m_pTextBrush1);
					p = p->next;
					count--;
					if (0 == count)
						break;
				}

				m_pD2DRenderTarget->PopAxisAlignedClip();
				m_win4.SetScreenValide(); // prevent un-necessary draw again
			}

			// draw window 5
			src = m_win5.GetDUIBuffer();
			if (nullptr != src)
			{
#ifdef _DEBUG
				m5++;
#endif
				ID2D1Bitmap* pBitmap = nullptr;
				XRECT* xr = m_win5.GetWindowArea();
				w = xr->right - xr->left; h = xr->bottom - xr->top;
				hr = m_pD2DRenderTarget->CreateBitmap(D2D1::SizeU(w, h), src, (w << 2),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), &pBitmap);
				if (S_OK == hr && nullptr != pBitmap)
				{
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(xr->left), static_cast<FLOAT>(xr->top), static_cast<FLOAT>(xr->right), static_cast<FLOAT>(xr->bottom));
					m_pD2DRenderTarget->DrawBitmap(pBitmap, &rect);
				}
				SafeRelease(&pBitmap);
				m_win5.SetScreenValide(); // prevent un-necessary draw again
			}
			
			ClearDUIWindowReDraw(); // prevent un-necessary redraw
			////////////////////////////////////////////////////////////////////////////////////////////////////
			hr = m_pD2DRenderTarget->EndDraw();
			if (FAILED(hr) || D2DERR_RECREATE_TARGET == hr)
			{
				SafeRelease(&m_pD2DRenderTarget);
			}
		}

		EndPaint(&ps);

#ifdef _DEBUG
		swprintf((wchar_t*)xtitle, 255, L"WoChat[%d] ~ [H:%d|L:%d]OnPaint Call :  W0: %04d - W1: %04d  - W2: %04d - W3: %04d - W4: %04d - W5: %04d", drag, m6, m7, m0, m1, m2, m3, m4, m5);
		::SetWindowTextW(m_hWnd, (LPCWSTR)xtitle);
#endif
		return 0;
	}

	bool SetSplitterPos(int xyPos = -1, bool isVertial = true)
	{
		int limit, cxyMax = 0;
		bool bRet;

		if(xyPos == -1)   // -1 == default position
		{
			ATLASSERT(FALSE);
		}

		// Adjust if out of valid range
		if (isVertial)
		{
			cxyMax = m_rectClient.right - m_rectClient.left;
			limit = cxyMax - m_splitterVPosToRight;
		}
		else
		{
			cxyMax = m_rectClient.bottom - m_rectClient.top;
			limit = cxyMax - m_splitterHPosToBottom;
		}
				
		if(xyPos < 0)
			xyPos = 0;
		else if(xyPos > limit)
			xyPos = limit;

		// Set new position and update if requested
		if (isVertial)
		{
			bRet = (m_splitterVPos != xyPos);
			m_splitterVPos = xyPos;
			if (m_splitterVPos < m_splitterVPosToLeft)
				m_splitterVPos = m_splitterVPosToLeft;

			if (m_splitterVPos > (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight))
			{
				m_splitterVPos = (m_rectClient.right - m_rectClient.left - m_splitterVPosToRight);
				ATLASSERT(m_splitterVPos > m_splitterVPosToLeft);
			}
			m_splitterVPosOld = m_splitterVPos;
		}
		else
		{
			bRet = (m_splitterHPos != xyPos);
			m_splitterHPos = xyPos;
			if (m_splitterHPos < m_splitterHPosToTop)
				m_splitterHPos = m_splitterHPosToTop;

			if (m_splitterHPos > (m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom))
			{
				m_splitterHPos = (m_rectClient.bottom - m_rectClient.top - m_splitterHPosToBottom);
				ATLASSERT(m_splitterHPos > m_splitterHPosToTop);
			}
			m_splitterHPosOld = (m_rectClient.bottom - m_rectClient.top) - m_splitterHPos;
		}
		return bRet;
	}

	bool IsOverSplitterRect(int x, int y) const
	{
		// -1 == don't check
		return (((x == -1) || ((x >= m_rectClient.left) && (x < m_rectClient.right))) &&
			((y == -1) || ((y >= m_rectClient.top) && (y < m_rectClient.bottom))));
	}

	DrapMode IsOverSplitterBar(int x, int y) const
	{
		int width = SPLITLINE_WIDTH << 1;
		if(!IsOverSplitterRect(x, y))
			return DrapMode::dragModeNone;

		ATLASSERT(m_splitterVPos > 0);
		if((x >= m_splitterVPos) && (x < (m_splitterVPos + width)))
			return DrapMode::dragModeV;

		if (m_splitterHPos > 0)
		{
			if ((x >= m_splitterVPos + width) && (y >= m_splitterHPos) && (y < (m_splitterHPos + width)))
				return DrapMode::dragModeH;
		}
		return DrapMode::dragModeNone;
	}
};

#endif /* __WOCHAT_XWINDOW_H__ */