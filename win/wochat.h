#ifndef __DUIAPP_H__
#define __DUIAPP_H__

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include <stdint.h>
#define _CRT_RAND_S
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>

#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <dwmapi.h>
#include <d2d1.h>

#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "comctl32.lib")

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <memoryapi.h>

#include <cassert>
#include <string>
#if 0
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#endif
#include <modpbase64/modp_b64.h>
#include <mbedtls/chacha20.h>

#include "dui/dui.h"
#include "dui/dui_mempool.h"

typedef struct XChatMessage
{
	XChatMessage* next;
	XChatMessage* prev;
	U32  id;
	U32* icon;     // the bitmap data of this icon
	U8   w;        // the width in pixel of this icon
	U8   h;        // the height in pixel of this icon
	int  height;   // in pixel
	int  width;    // in pixel
	U16  state;
	U64  ts;		// the time stamp. 
	U16* name;      // The name of this people      
	U16* message;  // real message
	U16* textWrapTab; // the struct of this table is the same as _TextWrapIdxTab
	U8* obj;       // point to GIF/APNG/Video/Pic etc
} XChatMessage;

typedef struct XChatGroup
{
	XChatGroup* next;
	U16  id;			// Group ID
	U32* icon;			// the bitmap data of this icon
	U8   w;			    // the width in pixel of this icon
	U8   h;			    // the height in pixel of this icon
	U16  width;		    // in pixel
	U16  height;		// in pixel
	U16  unread;		// how many unread messages? if more than 254, use ... 
	U16  member;		// how many members in this group?
	U16* name;			// the group name
	U64  ts;			// the time stamp. 
	U16* tsText;			// the group name
	U16* lastmsg;		// the last message of this group
	XChatMessage* headMessage;
	XChatMessage* tailMessage;
	MemoryContext mempool;
} XChatGroup;

typedef struct XMQTTMessage
{
	char* host;
	int   port;
	char* topic;
	char* message;
	int   msglen;
} XMQTTMessage;

extern UINT				  g_Quit;
extern LONG				  g_threadCount;
extern HINSTANCE		  g_hInstance;
extern ID2D1Factory*      g_pD2DFactory;
extern IDWriteFactory*    g_pDWriteFactory;

#define TEXT_TITLE				1
#define TEXT_MESSAGE			2
#define TEXT_MESSAGE_SMALL0		3
#define TEXT_MESSAGE_SMALL1		4

extern IDWriteTextFormat* g_pTextFormatTitle;
extern IDWriteTextFormat* g_pTextFormatMessage;
extern IDWriteTextFormat* g_pTextFormatMessageSmall0;
extern IDWriteTextFormat* g_pTextFormatMessageSmall1;

extern HCURSOR g_hCursorWE;
extern HCURSOR g_hCursorNS;
extern HCURSOR g_hCursorHand;
extern HCURSOR g_hCursorIBeam;

#define XWIN_MAX_INPUTSTRING		(1<<16)
extern uint8_t  g_MSG[(XWIN_MAX_INPUTSTRING) << 1];
extern uint8_t  g_Nonce[12];
extern uint8_t  g_KEY[32];
extern uint8_t  g_SKey[32];
extern uint8_t  g_PKey[33];
extern uint8_t  g_PKeyPlain[67];
extern uint8_t  g_PKey1[33];
extern uint8_t  g_PKey1Plain[67];

#define XFONT_SIZE0		15
#define XFONT_SIZE1		11

#if 0
extern FT_Library		g_ftLibrary;
extern FT_Face			g_ftFace0;
extern FT_Face			g_ftFace1;
extern FT_Face			g_ftFace2;
#endif
void InitToolTipMessage();
int GetPKfromSK(U8* sk, U8* pk, U8* pkPlain);
int GetKeyfromSKPK(U8* sk, U8* pk, U8* k);

int GetTextHeightInPixel(U16* text, U16 length, int width, int* h, int* w);
int GetLineHeightInPixel(U32 txtype);

#endif  /* __DUIAPP_H__ */

