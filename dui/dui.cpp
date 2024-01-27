#include <algorithm>
#include "dui.h"

// global variables
U8          DUIMessageOSMap[MESSAGEMAP_TABLE_SIZE] = { 0 };
U64         dui_status;

HCURSOR	dui_hCursorWE = nullptr;
HCURSOR	dui_hCursorNS = nullptr;
HCURSOR	dui_hCursorHand = nullptr;
HCURSOR	dui_hCursorIBeam = nullptr;

int DUI_Init()
{
    int i;
    U8* p = DUIMessageOSMap;

    //g_hCursorWE = ::LoadCursor(NULL, IDC_SIZEWE);
    //g_hCursorNS = ::LoadCursor(NULL, IDC_SIZENS);
    dui_hCursorHand = ::LoadCursor(nullptr, IDC_HAND);
    dui_hCursorIBeam = ::LoadCursor(NULL, IDC_IBEAM);

    if (nullptr == dui_hCursorHand || nullptr == dui_hCursorIBeam)
        return DUI_FAILED;

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


int XLabel::DrawText(int dx, int dy, DUI_Surface surface, DUI_Brush brush, U32 flag)
{
    if (XCONTROL_STATE_HIDDEN == m_status)
        return 0;

    if (nullptr != m_pTextLayout)
    {
        ID2D1HwndRenderTarget* pD2DRenderTarget = (ID2D1HwndRenderTarget*)surface;
        ID2D1SolidColorBrush* pTextBrush = (ID2D1SolidColorBrush*)brush;
        pD2DRenderTarget->DrawTextLayout(D2D1::Point2F(static_cast<FLOAT>(dx + left), static_cast<FLOAT>(dy + top)), m_pTextLayout, pTextBrush);
    }
    return 0;
}

void XLabel::setText(U16* text, U16 len)
{
    if (len > 0)
    {
        m_TextLen = (len <= DUI_MAX_LABEL_STRING)? len : DUI_MAX_LABEL_STRING;

        for (U16 i = 0; i < m_TextLen; i++)
            m_Text[i] = text[i];
        
        m_Text[m_TextLen] = L'\0';

        if (nullptr != m_pTextLayout)
        {
            m_pTextLayout->Release();
            m_pTextLayout = nullptr;
        }
        assert(m_pDWriteFactory);
        assert(m_pTextFormat);
        m_pDWriteFactory->CreateTextLayout((const WCHAR*)m_Text, m_TextLen, m_pTextFormat, (FLOAT)1, (FLOAT)1, &m_pTextLayout);
        if (nullptr != m_pTextLayout)
        {
            DWRITE_TEXT_METRICS tm;
            m_pTextLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
            m_pTextLayout->GetMetrics(&tm);
            left = 0;
            right = static_cast<int>(tm.width) + 1;
            top = 0;
            bottom = static_cast<int>(tm.height) + 1;
        }
    }

}