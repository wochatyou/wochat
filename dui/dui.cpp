#include "dui.h"

// global variables
U8 DUIMessageOSMap[MESSAGEMAP_TABLE_SIZE] = { 0 };
XControl* dui_controlArray[DUI_MAX_CONTROLS];
XBitmap   dui_bitmapArray[DUI_MAX_BUTTON_BITMAPS];
U16* dui_tooltip[DUI_MAX_CONTROLS];
U64       dui_status;

int DUI_Init()
{
    int i;
    U8* p = DUIMessageOSMap;

    for (i = 0; i < DUI_MAX_CONTROLS; i++)
        dui_controlArray[i] = nullptr;

    for (i = 0; i < MESSAGEMAP_TABLE_SIZE; i++)
        p[i] = DUI_NULL;

#ifdef _WIN32
    // We establish the mapping relationship between Windows message to DUI message
    p[WM_CREATE] = DUI_CREATE;
    p[WM_DESTROY] = DUI_DESTROY;
    p[WM_ERASEBKGND] = DUI_ERASEBKGND;
    p[WM_PAINT] = DUI_PAINT;
    p[WM_TIMER] = DUI_TIMER;
    p[WM_MOUSEMOVE] = DUI_MOUSEMOVE;
    p[WM_MOUSEWHEEL] = DUI_MOUSEWHEEL;
    p[WM_LBUTTONDOWN] = DUI_LBUTTONDOWN;
    p[WM_LBUTTONUP] = DUI_LBUTTONUP;
    p[WM_LBUTTONDBLCLK] = DUI_LBUTTONDBLCLK;
    p[WM_RBUTTONDOWN] = DUI_RBUTTONDOWN;
    p[WM_RBUTTONUP] = DUI_RBUTTONUP;
    p[WM_RBUTTONDBLCLK] = DUI_RBUTTONDBLCLK;
    p[WM_MBUTTONDOWN] = DUI_MBUTTONDOWN;
    p[WM_MBUTTONUP] = DUI_MBUTTONUP;
    p[WM_MBUTTONDBLCLK] = DUI_MBUTTONDBLCLK;
    p[WM_CAPTURECHANGED] = DUI_CAPTURECHANGED;
    p[WM_KEYFIRST] = DUI_KEYFIRST;
    p[WM_KEYDOWN] = DUI_KEYDOWN;
    p[WM_KEYUP] = DUI_KEYUP;
    p[WM_CHAR] = DUI_CHAR;
    p[WM_DEADCHAR] = DUI_DEADCHAR;
    p[WM_SYSKEYDOWN] = DUI_SYSKEYDOWN;
    p[WM_SYSKEYUP] = DUI_SYSKEYUP;
    p[WM_SYSCHAR] = DUI_SYSCHAR;
    p[WM_SYSDEADCHAR] = DUI_SYSDEADCHAR;
    p[WM_GETMINMAXINFO] = DUI_GETMINMAXINFO;
    p[WM_SIZE] = DUI_SIZE;
    p[WM_SETCURSOR] = DUI_SETCURSOR;
    p[WM_SETFOCUS] = DUI_SETFOCUS;
    p[WM_MOUSEACTIVATE] = DUI_MOUSEACTIVATE;
    p[WM_MOUSELEAVE] = DUI_MOUSELEAVE;
    p[WM_MOUSEHOVER] = DUI_MOUSEHOVER;
#endif

    return DUI_OK;
}

void DUI_Term()
{

}

int XButton::Draw()
{
    if (nullptr != m_parentBuf)
    {
        U32* dst = m_parentBuf;
        U32* src;
        int w = right - left;
        int h = bottom - top;
        XBitmap* bitmap;

        assert(w > 0 && h > 0);
        assert(m_parentW > w);
        assert(m_parentH > h);
        switch (m_status)
        {
        case XCONTROL_STATE_PRESSED:
            bitmap = imgPress;
            break;
        case XCONTROL_STATE_HOVERED:
            bitmap = imgHover;
            break;
        case XCONTROL_STATE_ACTIVE:
            bitmap = imgActive;
            break;
        default:
            bitmap = imgNormal;
            break;
        }
        assert(nullptr != bitmap);
        assert(w == bitmap->w);
        assert(h == bitmap->h);

        src = bitmap->data;
        assert(nullptr != src);

        if (XCONTROL_PROP_ROUND & m_property)
            DUI_ScreenDrawRectRound(dst, m_parentW, m_parentH, src, bitmap->w, bitmap->h, left, top, m_Color0, m_Color1);
        else
            DUI_ScreenDrawRect(dst, m_parentW, m_parentH, src, bitmap->w, bitmap->h, left, top);
    }
    return 0;
}
