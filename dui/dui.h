#ifndef __WT_DUI_H__
#define __WT_DUI_H__

#include <stdint.h>
#include <assert.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#endif 

#include "dui_mempool.h"
#include "dui_render.h"

#define DUI_DEBUG	1
#define DUI_OK      0
#define DUI_FAILED  0xFF

#define S8      int8_t
#define S16     int16_t
#define S32     int32_t
#define S64     int64_t

#define U8      uint8_t
#define U16     uint16_t
#define U32     uint32_t
#define U64     uint64_t

/* DUI_ALIGN() is only to be used to align on a power of 2 boundary */
#define DUI_ALIGN(size, boundary)   (((size) + ((boundary) -1)) & ~((boundary) - 1))
#define DUI_ALIGN_DEFAULT32(size)   DUI_ALIGN(size, 4)
#define DUI_ALIGN_DEFAULT64(size)   DUI_ALIGN(size, 8)      /** Default alignment */
#define DUI_ALIGN_PAGE(size)        DUI_ALIGN(size, (1<<16))
#define DUI_ALIGN_TRUETYPE(size)    DUI_ALIGN(size, 64)    

/*
 * Fast MOD arithmetic, assuming that y is a power of 2 !
 */
#define DUI_MOD(x,y)			 ((x) & ((y)-1))

#ifdef _MSC_VER
#define DUI_NO_VTABLE __declspec(novtable)
#else
#define DUI_NO_VTABLE
#endif

typedef struct tagXPOINT
{
    int  x;
    int  y;
} XPOINT;

typedef struct tagXSIZE
{
    int  cx;
    int  cy;
} XSIZE;

typedef struct tagXRECT
{
    int    left;
    int    top;
    int    right;
    int    bottom;
} XRECT;

// a pure 32-bit true color bitmap object
typedef struct XBitmap
{
    U8    id;
    U32* data;
    int   w;
    int   h;
} XBitmap;

// determin if one object is hitted
#define XWinPointInRect(x, y, OBJ)      (((x) >= ((OBJ)->left)) && ((x) < ((OBJ)->right)) && ((y) >= ((OBJ)->top)) && ((y) < ((OBJ)->bottom)))

#define MESSAGEMAP_TABLE_SIZE       (1<<16)

extern U8 DUIMessageOSMap[MESSAGEMAP_TABLE_SIZE];

extern HCURSOR	dui_hCursorWE;
extern HCURSOR	dui_hCursorNS;
extern HCURSOR	dui_hCursorHand;
extern HCURSOR	dui_hCursorIBeam;


#define DUI_GLOBAL_STATE_IN_NONE_MODE   0x0000000000000000
#define DUI_GLOBAL_STATE_NEED_REDRAW    0x0000000000000001
#define DUI_GLOBAL_STATE_IN_DRAG_MODE   0x0000000000000002
#define DUI_GLOBAL_STATE_SET_CURSOR     0x0000000000000004
#define DUI_GLOBAL_STATE_INIT_FAILED    0x0000000000000008
#define DUI_GLOBAL_STATE_CTRL_PRESSED   0x0000000000000010
#define DUI_GLOBAL_STATE_SHIFT_PRESSED  0x0000000000000020
#define DUI_GLOBAL_STATE_ALT_PRESSED    0x0000000000000040

// this gloable variable is shared by all virtual windows so they can know each other
extern U64 dui_status;

#define DUIWindowInDragMode()           (DUI_GLOBAL_STATE_IN_DRAG_MODE & dui_status)
#define SetDUIWindowDragMode()          do { dui_status |= DUI_GLOBAL_STATE_IN_DRAG_MODE;  } while(0)
#define ClearDUIWindowDragMode()        do { dui_status &= ~DUI_GLOBAL_STATE_IN_DRAG_MODE; } while(0)

#define DUIWindowNeedReDraw()           (DUI_GLOBAL_STATE_NEED_REDRAW & dui_status)
#define InvalidateDUIWindow()           do { dui_status |= DUI_GLOBAL_STATE_NEED_REDRAW; } while(0)
#define ClearDUIWindowReDraw()          do { dui_status &= ~DUI_GLOBAL_STATE_NEED_REDRAW;} while(0)

#define DUIWindowCursorIsSet()          (DUI_GLOBAL_STATE_SET_CURSOR & dui_status)
#define SetDUIWindowCursor()            do { dui_status |= DUI_GLOBAL_STATE_SET_CURSOR; } while(0)
#define ClearDUIWindowCursor()          do { dui_status &= ~DUI_GLOBAL_STATE_SET_CURSOR;} while(0)

#define DUIWindowInitFailed()           (DUI_GLOBAL_STATE_INIT_FAILED & dui_status)
#define SetDUIWindowInitFailed()        do { dui_status |= DUI_GLOBAL_STATE_INIT_FAILED; } while(0)

#define DUIShiftKeyIsPressed()          (DUI_GLOBAL_STATE_SHIFT_PRESSED & dui_status)
#define DUICtrlKeyIsPressed()           (DUI_GLOBAL_STATE_CTRL_PRESSED & dui_status)
#define DUIAltKeyIsPressed()            (DUI_GLOBAL_STATE_ALT_PRESSED & dui_status)

#define SetDUIWindowShiftKey()          do { dui_status |= DUI_GLOBAL_STATE_SHIFT_PRESSED; } while(0)
#define SetDUIWindowCtrlKey()           do { dui_status |= DUI_GLOBAL_STATE_CTRL_PRESSED; } while(0)
#define SetDUIWindowAltKey()            do { dui_status |= DUI_GLOBAL_STATE_ALT_PRESSED; } while(0)

#define ClearDUIWindowShiftKey()        do { dui_status &= ~DUI_GLOBAL_STATE_SHIFT_PRESSED; } while(0)
#define ClearDUIWindowCtrlKey()         do { dui_status &= ~DUI_GLOBAL_STATE_CTRL_PRESSED; } while(0)
#define ClearDUIWindowAltKey()          do { dui_status &= ~DUI_GLOBAL_STATE_ALT_PRESSED; } while(0)

#define IsHexLetter(c)                  (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >='a' && (c) <= 'f'))
#define IsAlphabet(c)		            (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

// DUI Message that is OS independent;
#define DUI_ALLMESSAGE          0x00
#define DUI_NULL                0x00
#define DUI_CREATE              0x01
#define DUI_DESTROY             0x02
#define DUI_ERASEBKGND          0x03
#define DUI_PAINT         	    0x04
#define DUI_TIMER               0x05
#define DUI_MOUSEMOVE           0x06
#define DUI_MOUSEWHEEL          0x07
#define DUI_MOUSEFIRST          0x08
#define DUI_LBUTTONDOWN         0x0A
#define DUI_LBUTTONUP           0x0B
#define DUI_LBUTTONDBLCLK       0x0C
#define DUI_RBUTTONDOWN         0x0D
#define DUI_RBUTTONUP           0x0E
#define DUI_RBUTTONDBLCLK       0x0F
#define DUI_MBUTTONDOWN         0x10
#define DUI_MBUTTONUP           0x11
#define DUI_MBUTTONDBLCLK       0x12
#define DUI_CAPTURECHANGED      0x13
#define DUI_KEYFIRST            0x14
#define DUI_KEYDOWN             0x15
#define DUI_KEYUP               0x16
#define DUI_CHAR                0x17
#define DUI_DEADCHAR            0x18
#define DUI_SYSKEYDOWN          0x19
#define DUI_SYSKEYUP            0x1A
#define DUI_SYSCHAR             0x1B
#define DUI_SYSDEADCHAR         0x1C
#define DUI_GETMINMAXINFO       0x1D
#define DUI_SIZE                0x1E
#define DUI_SETCURSOR           0x1F
#define DUI_SETFOCUS            0x20
#define DUI_MOUSEACTIVATE       0x21
#define DUI_MOUSELEAVE          0x22
#define DUI_MOUSEHOVER          0x23

// User defined message
#define DUI_XWINDOWS00          0xFF
#define DUI_XWINDOWS01          0xFE
#define DUI_XWINDOWS02          0xFD
#define DUI_XWINDOWS03          0xFC
#define DUI_XWINDOWS04          0xFB
#define DUI_XWINDOWS05          0xFA
#define DUI_XWINDOWS06          0xF9
#define DUI_XWINDOWS07          0xF8
#define DUI_XWINDOWS08          0xF7
#define DUI_XWINDOWS09          0xF6

#ifdef _WIN32
#define WM_XWINDOWSXX       (WM_USER + DUI_NULL)
#define WM_XWINDOWS00       (WM_USER + DUI_XWINDOWS00)
#define WM_XWINDOWS01       (WM_USER + DUI_XWINDOWS01)
#define WM_XWINDOWS02       (WM_USER + DUI_XWINDOWS02)
#define WM_XWINDOWS03       (WM_USER + DUI_XWINDOWS03)
#define WM_XWINDOWS04       (WM_USER + DUI_XWINDOWS04)
#define WM_XWINDOWS05       (WM_USER + DUI_XWINDOWS05)
#define WM_XWINDOWS06       (WM_USER + DUI_XWINDOWS06)
#define WM_XWINDOWS07       (WM_USER + DUI_XWINDOWS07)
#define WM_XWINDOWS08       (WM_USER + DUI_XWINDOWS08)
#define WM_XWINDOWS09       (WM_USER + DUI_XWINDOWS09)
#define WM_XWINDOWSPAINT    (WM_USER + DUI_PAINT)
#endif 

#define DUI_KEY_RETURN      0x0D

int DUI_Init();
void DUI_Term();

#define XMOUSE_NULL         0
#define XMOUSE_MOVE         1
#define XMOUSE_LBDOWN       2
#define XMOUSE_LBUP         3
#define XMOUSE_RBDOWN       4
#define XMOUSE_RBUP         5

#define XKEYBOARD_NORMAL    0x0000
#define XKEYBOARD_CTRL      0x0001
#define XKEYBOARD_SHIF      0x0002
#define XKEYBOARD_ALT       0x0004

enum XControlProperty
{
    XCONTROL_PROP_NONE = 0x00000000,
    XCONTROL_PROP_ROUND = 0x00000001,
    XCONTROL_PROP_STATIC = 0x00000002,
    XCONTROL_PROP_CARET = 0x00000004,
    XCONTROL_PROP_EDIT = 0x00000008,
    XCONTROL_PROP_PASSWD = 0x00000010,
    XCONTROL_PROP_TEXT = 0x00000020
};

enum XControlState
{
    XCONTROL_STATE_HIDDEN = 0,
    XCONTROL_STATE_NORMAL,
    XCONTROL_STATE_HOVERED,
    XCONTROL_STATE_PRESSED,
    XCONTROL_STATE_ACTIVE,
    XCONTROL_STATE_FOCUSED = XCONTROL_STATE_PRESSED
};

typedef void* DUI_Surface;
typedef void* DUI_Brush;

class XControl
{
public:
    U8   m_id = 0;
    int  left = 0;
    int  top = 0;
    int  right = 0;
    int  bottom = 0;

    U32  m_property = XCONTROL_PROP_NONE;
    U32  m_status = XCONTROL_STATE_NORMAL;
    U32  m_statusPrev = XCONTROL_STATE_NORMAL;

    U32* m_parentBuf = nullptr;
    int  m_parentW = 0;
    int  m_parentH = 0;

    U32  m_Color0 = 0xFFFFFFFF;
    U32  m_Color1 = 0xFFFFFFFF;

    U16  m_tipMessage[32];

    void* m_Cursor = nullptr;

    void setId(U8 id)
    {
        m_id = id;
    }

    int ShowCursor()
    {
        int ret = 0;
        if (nullptr != m_Cursor)
        {
#ifdef _WIN32
            ::SetCursor((HCURSOR)m_Cursor);
            ret = 1;
#endif 
        }
        return ret;
    }

    void setRoundColor(U32 c0, U32 c1)
    {
        m_Color0 = c0;
        m_Color1 = c1;
    }

    int getLeft()
    {
        assert(left >= 0);
        return left;
    }

    int getTop()
    {
        assert(top >= 0);
        return top;
    }

    int getWidth()
    {
        assert(right > left);
        return(right - left);
    }

    int getHeight()
    {
        assert(bottom > top);
        return(bottom - top);
    }

    void setSize(int width, int height)
    {
        assert(width > 0);
        assert(height > 0);
        left = top = 0;
        right = width;
        bottom = height;
    }

    void setPosition(int left_, int top_, int right_, int bottom_)
    {
        assert(left_ >= 0);
        assert(top_ >= 0);
        assert(right_ >= left_);
        assert(bottom_ >= top_);
        left = left_;
        top = top_;
        right = right_;
        bottom = bottom_;
    }

    void setPosition(int left_, int top_)
    {
        int w = right - left;
        int h = bottom - top;

        assert(left_ >= 0);
        assert(top_ >= 0);
        assert(w > 0);
        assert(h > 0);

        left = left_;
        top = top_;
        right = left + w;
        bottom = top + h;
    }

    void setProperty(U32 property)
    {
        m_property |= property;
    }

    U32 getProperty()
    {
        return m_property;
    }

    void AttachParent(U32* parentBuf, int parentW, int parentH)
    {
        m_parentBuf = parentBuf;
        m_parentW = parentW;
        m_parentH = parentH;
    }

    bool IsOverMe(int xPos, int yPos)
    {
        bool bRet = false;
        if (!(XCONTROL_PROP_STATIC & m_property)) // this is not a static control
        {
            if (xPos >= left && xPos < right && yPos >= top && yPos < bottom)
                bRet = true;
        }
        return bRet;
    }

    int setStatus(U32 newStatus, U8 mouse_event = XMOUSE_NULL)
    {
        int r = 0;
        if (!(XCONTROL_PROP_STATIC & m_property))
        {
            U32 oldStatus = m_status;

            if (XCONTROL_PROP_EDIT & m_property) // for edit box
            {
                if (XCONTROL_STATE_PRESSED == m_status)
                {
                    if (XCONTROL_STATE_NORMAL == newStatus && XMOUSE_LBDOWN != mouse_event)
                    {
                        return 0;
                    }
                    if (XCONTROL_STATE_HOVERED == newStatus)
                    {
                        return 0;
                    }
                }
            }

            m_status = newStatus;
            if (oldStatus != newStatus)
                r = 1;
        }
        return r;
    }

    U32 getStatus()
    {
        return m_status;
    }

    virtual int OnTimer() { return 0; }
    virtual int OnKeyBoard(U16 flag, U16 keycode) { return 0; }
    virtual int  Draw() = 0;
    virtual int  DrawText(int dx, int dy, DUI_Surface surface = nullptr, DUI_Brush brush = nullptr, U32 flag = 0) = 0;
    virtual int  Init(void* ptr0 = nullptr, void* ptr1 = nullptr, void* ptr2 = nullptr, U32 flag = 0) = 0;
    virtual void Term() = 0;

    int (*pfAction) (void* obj, U32 uMsg, U64 wParam, U64 lParam);
};

class XButton : public XControl
{
public:
    int Draw();
    int DrawText(int dx, int dy, DUI_Surface surface = nullptr, DUI_Brush brush = nullptr, U32 flag = 0) { return 0; }
    int Init(void* ptr0 = nullptr, void* ptr1 = nullptr, void* ptr2 = nullptr, U32 flag = 0)
    {
        m_Cursor = ptr0;
        return 0;
    }
    void Term() {}

    // all XBitmpas should have extactly the same size
    XBitmap* imgNormal;
    XBitmap* imgHover;
    XBitmap* imgPress;
    XBitmap* imgActive;

    void setBitmap(XBitmap* n, XBitmap* h, XBitmap* p, XBitmap* a)
    {
        assert(nullptr != n);
        assert(nullptr != h);
        assert(nullptr != p);
        assert(nullptr != a);
        imgNormal = n; imgHover = h; imgPress = p; imgActive = a;
        setSize(imgNormal->w, imgNormal->h);
    }

};

#define DUI_MAX_LABEL_STRING    128
class XLabel : public XControl
{
private:
    U32  m_bkgColor = 0xFFFFFFFF;
    U32  m_caretAnchor = 0;
    U32  m_caretPosition = 0;
    U32  m_caretPositionOffset = 0;
    U16  m_Text[DUI_MAX_LABEL_STRING + 1] = { 0 };
    U16  m_TextLen = 0;
    IDWriteFactory*    m_pDWriteFactory = nullptr;
    IDWriteTextFormat* m_pTextFormat = nullptr;
    IDWriteTextLayout* m_pTextLayout = nullptr;
public:
    XLabel()
    {
        m_property = XCONTROL_PROP_TEXT;
    }

    int Draw() { return 0; }
    int DrawText(int dx, int dy, DUI_Surface surface = nullptr, DUI_Brush brush = nullptr, U32 flag = 0);
    int Init(void* ptr0 = nullptr, void* ptr1 = nullptr, void* ptr2 = nullptr, U32 flag = 0)
    {
        m_Cursor = ptr0;
        m_pDWriteFactory = (IDWriteFactory*)ptr1;
        m_pTextFormat = (IDWriteTextFormat*)ptr2;
        return 0;
    }
    void Term() {}
    void setText(U16* text, U16 len = 0);

};


#endif // __WT_DUI_H__