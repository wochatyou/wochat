#ifndef __WT_DUI_WIN_H__
#define __WT_DUI_WIN_H__

#include "dui.h"

#define DUI_MINIMAL_THUMB_SIZE          64
#define DUI_ALLOCSET_DEFAULT_INITSIZE   (8 * 1024)
#define DUI_ALLOCSET_DEFAULT_MAXSIZE    (8 * 1024 * 1024)
#define DUI_ALLOCSET_SMALL_INITSIZE     (1 * 1024)
#define DUI_ALLOCSET_SMALL_MAXSIZE	    (8 * 1024)

enum
{
    DUI_STATUS_NODRAW    = 0x00000000,    // do not need to draw, any value not zero need redraw      
    DUI_STATUS_VISIBLE   = 0x00000001,    // is this virtual window visible?
    DUI_STATUS_VSCROLL   = 0x00000002,    // is this virtual window has vertical scroll bar?
    DUI_STATUS_HSCROLL   = 0x00000004,    // is this virtual window has horizonal scroll bar?
    DUI_STATUS_ISFOCUS   = 0x00000008,    // is the keyboard input redirected into this virutal window?
    DUI_STATUS_NEEDRAW   = 0x00000010     // does this virtual windows need to be redraw?
};

enum
{
    DUI_PROP_NONE             = 0x00000000,   // None Properties
    DUI_PROP_MOVEWIN          = 0x00000001,   // Move the whole window while LButton is pressed
    DUI_PROP_BTNACTIVE        = 0x00000002,   // have active button on this virtual window
    DUI_PROP_HASVSCROLL       = 0x00000004,    // have vertical scroll bar
    DUI_PROP_HASHSCROLL       = 0x00000008,
    DUI_PROP_HANDLEVWHEEL     = 0x00000010,   // does this window need to handle mouse wheel?
    DUI_PROP_HANDLEHWHEEL     = 0x00000020,
    DUI_PROP_HANDLETIMER      = 0x00000040,
    DUI_PROP_HANDLEKEYBOARD   = 0x00000080,
    DUI_PROP_LARGEMEMPOOL     = 0x00000100
};

enum
{
    DEFAULT_BORDER_COLOR       = 0xFFBBBBBB,
    DEFAULT_SCROLLBKG_COLOR    = 0xFFF9F3F1,
    DEFAULT_SCROLLTHUMB_COLOR  = 0xFFC0C1C4,
    DEFAULT_SCROLLTHUMB_COLORA = 0xFFAAABAD
};

template <class T>
class DUI_NO_VTABLE XWindowT
{
private:
    enum class XDragMode { DragNone, DragVertical, DragHorizonal };
    XDragMode  m_DragMode = XDragMode::DragNone;

public:
    //typedef int (T::*ProcessOSMessage)(U32 uMsg, U64 wParam, U64 lParam, void* lpData);

#ifdef _WIN32
    HWND m_hWnd = nullptr;  // the pointer to point the host real/platform window
#else
    void* m_hWnd = nullptr;
#endif
    U32*    m_screen = nullptr;  // the memory block that will be rendered to the screen by the platform
    U32     m_size = 0;
    U8      m_id[8] = { 0 }; // for debugging 
    XRECT   m_area = { 0 };  // the area of this window in the client area of parent window

    MemoryPoolContext m_pool = nullptr;

    int m_startControl  =  0;  // dui_controlArray[0] is not used. We start from 1
    int m_endControl    = -1;
    int m_activeControl = -2;

    //ProcessOSMessage m_messageFuncPointerTab[256] = { 0 };  // we only handle 255 messages that should be enough

    const int m_scrollWidth = 8; // in pixel

    XPOINT m_ptOffset      = { 0 };
    XPOINT m_ptOffsetOld   = { 0 };
    XSIZE  m_sizeAll       = { 0 };
    XSIZE  m_sizeLine      = { 0 };
    XSIZE  m_sizePage      = { 0 };
    int    m_cxyDragOffset = 0;

    U32  m_status   = DUI_STATUS_VISIBLE;
    U32  m_property = DUI_PROP_NONE;
    U32  m_message  = DUI_NULL;

    U32  m_backgroundColor = DEFAULT_BACKGROUND_COLOR;
    U32  m_scrollbarColor  = DEFAULT_SCROLLBKG_COLOR;
    U32  m_thumbColor      = DEFAULT_SCROLLTHUMB_COLOR;

    //XTextDrawInfo* m_textDrawInfo = nullptr;
    U16            m_textDrawInfoCount = 0;
public:
    XWindowT()
    {
#if 0        
        //m_messageFuncPointerTab[DUI_NULL] = nullptr;
        for (int i = 0; i < 256; i++) 
            m_messageFuncPointerTab[i] = nullptr;

        ProcessOSMessage* pf = m_messageFuncPointerTab;
        pf[DUI_CREATE]      = &T::OnCreate;
        pf[DUI_DESTROY]     = &T::OnDestroy;
        pf[DUI_MOUSEMOVE]   = &T::OnMouseMove;
        pf[DUI_LBUTTONDOWN] = &T::OnLButtonDown;
        pf[DUI_LBUTTONUP]   = &T::OnLButtonUp;
        pf[DUI_SIZE]        = &T::OnSize;
        pf[DUI_TIMER]       = &T::OnTimer;
        pf[DUI_MOUSEWHEEL]  = &T::OnMouseWheel;
        pf[DUI_KEYDOWN]     = &T::OnKeyPress;
        pf[DUI_CHAR]        = &T::OnChar;
        pf[DUI_SETCURSOR]   = &T::OnSetCursor;
        pf[DUI_MOUSELEAVE]  = &T::On_DUI_MOUSELEAVE;
        pf[DUI_MOUSEHOVER]  = &T::On_DUI_MOUSEHOVER;
#endif
    }

    ~XWindowT()
    {
        XControl* xctl;
        for (int i = m_startControl; i <= m_endControl; i++)
        {
            xctl = dui_controlArray[i];
            assert(nullptr != xctl);
            xctl->Term();
        }

        mempool_destroy(m_pool);
        m_pool = nullptr;
    }

    static int XControlAction(void* obj, U32 uMsg, U64 wParam, U64 lParam)
    {
        int ret = 0;

        T* pT = static_cast<T*>(obj);
        if (nullptr != pT)
            ret = pT->NotifyParent(uMsg, wParam, lParam);

        return ret;
    }

    void InitControl() {}
#if 0
    tagXTextDrawInfo* GetTextDrawInfo(U16* count)
    {
        if (nullptr != count)
            *count = m_textDrawInfoCount;
        return m_textDrawInfo;
    }
#endif
    // < 0 : I do not handle this message
    // = 0 : I handled, but I do not need to upgrade the screen
    // > 0 : I handled this message, and need the host window to upgrade the screen
    int HandleOSMessage(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = -1;

        U16 msgIdOS = (U16)uMsg;
        U8 msgId = DUIMessageOSMap[msgIdOS];  // lookup the message map from Platform to our DUI message

        switch (msgId)
        {
        case DUI_PAINT:
            r = On_DUI_PAINT(uMsg, wParam, lParam, lpData);
            break;
        case DUI_MOUSEMOVE:
            r = On_DUI_MOUSEMOVE(uMsg, wParam, lParam, lpData);
            break;
        case DUI_TIMER:
            r = On_DUI_TIMER(uMsg, wParam, lParam, lpData);
            break;
        case DUI_CHAR:
            r = On_DUI_CHAR(uMsg, wParam, lParam, lpData);
            break;
        case DUI_KEYDOWN:
            r = On_DUI_KEYDOWN(uMsg, wParam, lParam, lpData);
            break;
        case DUI_SETCURSOR:
            r = On_DUI_SETCURSOR(uMsg, wParam, lParam, lpData);
            break;
        case DUI_MOUSEWHEEL:
            r = On_DUI_MOUSEWHEEL(uMsg, wParam, lParam, lpData);
            break;
        case DUI_LBUTTONDOWN:
            r = On_DUI_LBUTTONDOWN(uMsg, wParam, lParam, lpData);
            break;
        case DUI_LBUTTONUP:
            r = On_DUI_LBUTTONUP(uMsg, wParam, lParam, lpData);
            break;
        case DUI_LBUTTONDBLCLK:
            r = On_DUI_LBUTTONDBLCLK(uMsg, wParam, lParam, lpData);
            break;
#if 0
        case DUI_SIZE:
            r = On_DUI_SIZE(uMsg, wParam, lParam, lpData);
#endif
            break;
        case DUI_CREATE:
            r = On_DUI_CREATE(uMsg, wParam, lParam, lpData);
            break;
        case DUI_DESTROY:
            r = On_DUI_DESTROY(uMsg, wParam, lParam, lpData);
            break;
        default:
            break;
        }

#if 0
        ProcessOSMessage pfMSG = m_messageFuncPointerTab[msgId];
        if (nullptr != pfMSG)
        {
            T* pT = static_cast<T*>(this);
            r = (pT->*pfMSG)(uMsg, wParam, lParam, lpData);
        }
#endif
        return r;
    }

    void InvalidateScreen()
    {
        m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
        InvalidateDUIWindow();
    }

    bool IsRealWindow(void* hwnd)
    {
#ifdef _WIN32
        return (::IsWindow((HWND)hwnd));
#else
        return false;
#endif
    }

    // Windows Id is used for debugging purpose
    void SetWindowId(const U8* id, U8 bytes)
    {
        if (bytes > 7)
            bytes = 7;

        for(U8 i = 0; i < bytes; i++)
            m_id[i] = *id++;

        m_id[bytes] = 0;
    }

    bool IsVisible() const
    {
        return (m_status & DUI_STATUS_VISIBLE);
    }

    XRECT* GetWindowArea()
    {
        return &m_area;
    }

    U32* GetDUIBuffer()
    {
        U32* buff = nullptr;
        U8 status = m_status & (DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW);

        if ((DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW) == status)
            buff = m_screen;
        return buff;
    }

    void SetScreenValide()
    {
        m_status &= ~DUI_STATUS_NEEDRAW;
    }

    void PostWindowHide() {}
    void WindowHide()
    {
        m_status &= ~DUI_STATUS_VISIBLE;

        T* pT = static_cast<T*>(this);
        pT->PostWindowHide();
    }

    void PostWindowShow() {}
    void WindowShow()
    {
        m_status |= DUI_STATUS_VISIBLE;

        T* pT = static_cast<T*>(this);
        pT->PostWindowShow();

    }

    bool PostWindowMessage(U32 message, U64 wParam = 0, U64 lParam = 0)
    {
        bool bRet = false;
        assert(IsRealWindow(m_hWnd));
#if defined(_WIN32)
        bRet = ::PostMessage((HWND)m_hWnd, (UINT)message, (WPARAM)wParam, (LPARAM)lParam);
#endif
        return bRet;
    }

    int NotifyParent(U32 uMsg, U64 wParam, U64 lParam)
    {
        PostWindowMessage(uMsg, wParam, lParam);
        return 0;
    }

#if 0
    int CreatePlatformThread(ThreadFunc threadfunc, void* udata)
    {
        int ret = 0;
#ifdef _WIN32
        DWORD dwThreadID;
        HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadfunc, udata, 0, &dwThreadID);
        if (nullptr == hThread)
            ret = 1;
#endif
        return ret;
    }
#endif

    // scan the whole control array and reset them to the status one by one
    int SetAllControlStatus(U32 status, U8 mouse_event) 
    { 
        int ret = 0;
        XControl* xctl;

        if (DUI_PROP_BTNACTIVE & m_property)
        {
            U32 ctlStatus;
            for (int i = m_startControl; i <= m_endControl; i++)
            {
                xctl = dui_controlArray[i];
                assert(nullptr != xctl);
                assert(xctl->m_id == i);
                ctlStatus = (i != m_activeControl) ? status : XCONTROL_STATE_ACTIVE;
                ret += xctl->setStatus(ctlStatus, mouse_event);
            }
        }
        else
        {
            for (int i = m_startControl; i <= m_endControl; i++)
            {
                xctl = dui_controlArray[i];
                assert(nullptr != xctl);
                assert(xctl->m_id == i);
                ret += xctl->setStatus(status, mouse_event);
            }
        }
        return ret;
    }

    void UpdateControlPosition() {}

    int DoDraw(DUI_Surface surface, DUI_Brush brush) { return 0; }
    int Draw(DUI_Surface surface, DUI_Brush brush)
    {
        if (nullptr != surface)
        {
            // the derived class will draw its content
            T* pT = static_cast<T*>(this);
            pT->DoDraw(surface, brush);
        }
        return 0;
    }

    int Do_DUI_PAINT(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_PAINT(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        U8 status = m_status & (DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW);

        // We only draw this virtual window when 1: it is visible, and 2: it needs draw
        if (((DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW) == status) && (nullptr != m_screen))
        {
            int w = m_area.right - m_area.left;
            int h = m_area.bottom - m_area.top;
            XControl* xctl;

            // assert(nullptr != m_screen);
            // fill the whole screen of this virutal window with a single color
            DUI_ScreenClear(m_screen, m_size, m_backgroundColor);

            if (DUI_STATUS_VSCROLL & m_status) // We have the vertical scroll bar to draw
            {
                int thumb_start, thumb_height, thumb_width;
                int vOffset = m_ptOffset.y;
                int vHeight = m_sizeAll.cy;
                assert(m_sizeAll.cy > h);

                thumb_width = m_scrollWidth - 2;
                thumb_start = (vOffset * h) / vHeight;
                thumb_height = (h * h) / vHeight;
                if(thumb_height < DUI_MINIMAL_THUMB_SIZE)
                    thumb_height = DUI_MINIMAL_THUMB_SIZE; // we keep the thumb mini size to some pixels

                // Draw the vertical scroll bar
                DUI_ScreenFillRect(m_screen, w, h, m_scrollbarColor, m_scrollWidth, h, w - m_scrollWidth, 0);
                DUI_ScreenFillRectRound(m_screen, w, h, m_thumbColor, thumb_width, thumb_height, w - m_scrollWidth + 1, thumb_start, m_scrollbarColor, 0xFFD6D3D2);
            }

            // draw the controls within this window
            for (int i = m_startControl; i <= m_endControl; i++)
            {
                xctl = dui_controlArray[i];
                assert(nullptr != xctl);
                assert(xctl->m_id == i);
                xctl->Draw();
            }
            // the derived class will draw its content
            T* pT = static_cast<T*>(this);
            pT->Do_DUI_PAINT(uMsg, wParam, lParam, lpData);
        }

        return 0;
    }

    int UpdateSize(XRECT* r, U32* screenbuf)
    {
        if (nullptr != r)
        {
            m_area.left   = r->left;
            m_area.top    = r->top;
            m_area.right  = r->right;
            m_area.bottom = r->bottom;
            m_size = (U32)((r->right - r->left) * (r->bottom - r->top));
        }
        else
        {
            m_area.left = m_area.top = m_area.right = m_area.bottom = 0;
            m_size = 0;
        }
        m_screen = screenbuf;
        if (nullptr == m_screen)
        {
            m_area.left = m_area.top = m_area.right = m_area.bottom = 0;
            m_size = 0;
        }

        if (nullptr != r && nullptr != m_screen)
        {
            XControl* xctl;
            int w = r->right - r->left;
            int h = r->bottom - r->top;
            assert(w > 0);
            assert(h > 0);
            for (int i = m_startControl; i <= m_endControl; i++)
            {
                xctl = dui_controlArray[i];
                assert(nullptr != xctl);
                assert(xctl->m_id == i);
                xctl->AttachParent(m_screen, w, h);
            }

            T* pT = static_cast<T*>(this);
            pT->UpdateControlPosition();

            // enable tool tips
            if (m_startControl > 0 && m_message != DUI_NULL)
            {
                assert(m_endControl >= m_startControl);
                PostWindowMessage(m_message, (U64)m_startControl, (U64)m_endControl);
            }
        }
        m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
        InvalidateDUIWindow();
        return DUI_STATUS_NEEDRAW;
    }


    int Do_DUI_MOUSEWHEEL(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_MOUSEWHEEL(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = DUI_STATUS_NODRAW;

        if ((DUI_PROP_HANDLEVWHEEL & m_property) && !DUIWindowInDragMode())
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            if (XWinPointInRect(xPos, yPos, &m_area))
            {
                int h = m_area.bottom - m_area.top;
                int delta = GET_WHEEL_DELTA_WPARAM(wParam);

                if (m_sizeAll.cy > h)
                {
                    int yOld = m_ptOffset.y;

                    assert(m_sizeLine.cy > 0);
                    if (delta < 0)
                        m_ptOffset.y += m_sizeLine.cy;
                    else
                        m_ptOffset.y -= m_sizeLine.cy;

                    if (m_ptOffset.y < 0)
                        m_ptOffset.y = 0;
                    if (m_ptOffset.y > (m_sizeAll.cy - h))
                        m_ptOffset.y = m_sizeAll.cy - h;

                    if(yOld != m_ptOffset.y)
                        r++;
                }

                {  // let the derived class to do its stuff
                    T* pT = static_cast<T*>(this);
                    r += pT->Do_DUI_MOUSEWHEEL(uMsg, wParam, lParam, lpData);
                }
            }
        }

        if (DUI_STATUS_NODRAW != r)
        {
            m_status |= DUI_STATUS_NEEDRAW;
            InvalidateDUIWindow();
        }

        return r;
    }

    int Do_DUI_MOUSEMOVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_MOUSEMOVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r  = DUI_STATUS_NODRAW;

        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int w = m_area.right - m_area.left;
        int h = m_area.bottom - m_area.top;

        if (XDragMode::DragNone != m_DragMode) // this window is in drag mode
        {
            assert(DUIWindowInDragMode());
            m_status |= DUI_STATUS_VSCROLL;
            m_ptOffset.y = m_ptOffsetOld.y + ((yPos - m_cxyDragOffset) * m_sizeAll.cy) / h;
            if (m_ptOffset.y < 0)
                m_ptOffset.y = 0;
            if (m_ptOffset.y > (m_sizeAll.cy - h))
                m_ptOffset.y = m_sizeAll.cy - h;
            r++;
        }
        else 
        {
            if (XWinPointInRect(xPos, yPos, &m_area)) // the mosue is in this area
            {
                if (!DUIWindowInDragMode())
                {
                    int hit = -1;  // no hit so far
                    U32 ctlStatus;
                    XControl* xctl;
                    if (DUI_PROP_HASVSCROLL & m_property) // handle the vertical bar
                    {
                        U32 statusOld = m_status;  // save previous state
                        m_status &= (~DUI_STATUS_VSCROLL);

                        assert(m_area.right > m_scrollWidth);

                        if (m_sizeAll.cy > h) // the virutal window size is bigger than the real window size
                        {
                            U32 thumbColor = m_thumbColor;
                            m_status |= DUI_STATUS_VSCROLL;
                            m_thumbColor = DEFAULT_SCROLLTHUMB_COLOR;
                            if (xPos >= (m_area.right - m_scrollWidth))
                                m_thumbColor = DEFAULT_SCROLLTHUMB_COLORA;

                            if (thumbColor != m_thumbColor) // the thumb color has been changed, we need to redraw
                                r++;
                        }
                        if ((DUI_STATUS_VSCROLL & statusOld) != (DUI_STATUS_VSCROLL & m_status))
                            r++;
                    }
                    // transfer the coordination from real window to local virutal window
                    xPos -= m_area.left;
                    yPos -= m_area.top;
                    assert(xPos >= 0);
                    assert(yPos >= 0);
                    for (int i = m_startControl; i <= m_endControl; i++)
                    {
                        xctl = dui_controlArray[i];
                        assert(nullptr != xctl);
                        if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                        {
                            hit = i;
                            break;
                        }
                    }
                    if (-1 != hit) // we are hovering on some control
                    {
                        r += xctl->setStatus(XCONTROL_STATE_HOVERED, XMOUSE_MOVE);
                        xctl->ShowCursor();
                    }
                    else // we have to scan the whole control array
                    {
                        r += SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_MOVE);
                    }
                }
            }
            else // the mouse is not in our area, we do a quick check to be sure this window need to be redraw
            {
                // handle the vertical bar
                if (DUI_STATUS_VSCROLL & m_status)
                {
                    m_status &= (~DUI_STATUS_VSCROLL); // we should not dispaly the vertical bar
                    r++;
                }
                r += SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_MOVE);
            }
        }
          // let the derived class to do its stuff
        if (!DUIWindowInDragMode())
        {
            T* pT = static_cast<T*>(this);
            r += pT->Do_DUI_MOUSEMOVE(uMsg, wParam, lParam, lpData);
        }

        if (DUI_STATUS_NODRAW != r)
        {
            m_status |= DUI_STATUS_NEEDRAW;
            InvalidateDUIWindow();
        }

        return r;
    }

    int Do_DUI_LBUTTONDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_LBUTTONDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = DUI_STATUS_NODRAW;
        U32 statusOld = m_status;
        XControl* xctl;

        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (XWinPointInRect(xPos, yPos, &m_area))
        {
            if (!DUIWindowInDragMode())
            {
                int hit = -1;
                int w = m_area.right - m_area.left;
                int h = m_area.bottom - m_area.top;

                m_status |= DUI_STATUS_ISFOCUS;  // the mouse click my area, so I have the focus

                if (DUI_PROP_HASVSCROLL & m_property) // handle the vertical bar
                {
                    if (xPos >= (m_area.right - m_scrollWidth))
                    {
                        m_status &= (~DUI_STATUS_VSCROLL);
                        if (m_sizeAll.cy > h)
                        {
                            int thumb_start = (m_ptOffset.y * h) / m_sizeAll.cy;
                            int thumb_height = (h * h) / m_sizeAll.cy;

                            if (thumb_height < DUI_MINIMAL_THUMB_SIZE)
                                thumb_height = DUI_MINIMAL_THUMB_SIZE; // we keep the thumb mini size to some pixels

                            m_status |= DUI_STATUS_VSCROLL;

                            assert(m_ptOffset.y >= 0);
                            assert(m_ptOffset.y <= m_sizeAll.cy - h);

                            if (yPos > (m_area.top + thumb_start) && yPos < (m_area.top + thumb_start + thumb_height))
                            {
                                // we hit the thumb
                                m_cxyDragOffset = yPos;
                                m_ptOffsetOld.y = m_ptOffset.y;
                                m_DragMode = XDragMode::DragVertical;
                                SetDUIWindowDragMode();
                            }
                            else
                            {
                                int thumb_start_new = (yPos - m_area.top) - (thumb_height >> 1);
                                if (thumb_start_new < 0)
                                    thumb_start_new = 0;
                                if (thumb_start_new > h - thumb_height)
                                    thumb_start_new = h - thumb_height;

                                m_ptOffset.y = (thumb_start_new * m_sizeAll.cy) / h;
                            }
                            SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_MOVE);

                            m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
                            InvalidateDUIWindow();
                            return DUI_STATUS_NEEDRAW;
                        }
                        if ((DUI_STATUS_VSCROLL & statusOld) != (DUI_STATUS_VSCROLL & m_status))
                            r++;
                    }
                }
                // transfer the coordination from real window to local virutal window
                xPos -= m_area.left;
                yPos -= m_area.top;
                assert(xPos >= 0);
                assert(yPos >= 0);

                SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBDOWN);
                hit = -1;
                for (int i = m_startControl; i <= m_endControl; i++)
                {
                    xctl = dui_controlArray[i];
                    assert(nullptr != xctl);
                    assert(xctl->m_id == i);
                    if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                    {
                        hit = i;
                        break;
                    }
                }
                if (-1 != hit) // we are hitting some button
                {
                    r += xctl->setStatus(XCONTROL_STATE_PRESSED, XMOUSE_LBDOWN);
                    xctl->ShowCursor();
                }
                else // we do not hit any control of this virtual window
                {
                    // if the mouse does not hit the button, we can move the whole real window
                    if (DUI_PROP_MOVEWIN & m_property)
                        PostWindowMessage(WM_NCLBUTTONDOWN, HTCAPTION, lParam);
                }
            }
        }
        else
        {
            m_status &= ~(DUI_STATUS_ISFOCUS| DUI_STATUS_VSCROLL); // this window lose focus
            if ((DUI_STATUS_VSCROLL & statusOld) != (DUI_STATUS_VSCROLL & m_status))
                r++;
            r += SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBDOWN);
        }
        // let the derived class to do its stuff
        {
            T* pT = static_cast<T*>(this);
            r += pT->Do_DUI_LBUTTONDOWN(uMsg, wParam, lParam, lpData);
        }

        if (DUI_STATUS_NODRAW != r)
        {
            m_status |= DUI_STATUS_NEEDRAW;
            InvalidateDUIWindow();
        }
        return r;
    }

    int Do_DUI_LBUTTONUP(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_LBUTTONUP(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = DUI_STATUS_NODRAW;
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        XControl* xctl;

        ClearDUIWindowDragMode();
        m_DragMode = XDragMode::DragNone;
        m_ptOffsetOld.x = -1, m_ptOffsetOld.y = -1;

        if (XWinPointInRect(xPos, yPos, &m_area))
        {
            if (m_startControl > 0)
            {
                int hit = -1;
                    // transfer the coordination from real window to local virutal window
                    xPos -= m_area.left;
                    yPos -= m_area.top;
                    assert(xPos >= 0);
                    assert(yPos >= 0);
                    assert(m_endControl >= m_startControl);

                    for (int i = m_startControl; i <= m_endControl; i++)
                    {
                        xctl = dui_controlArray[i];
                        assert(nullptr != xctl);
                        assert(xctl->m_id == i);
                        if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                        {
                            hit = i;
                            break;
                        }
                    }
                if (-1 != hit) // we are hitting some button
                {
                    assert(hit == xctl->m_id);
                    if (DUI_PROP_BTNACTIVE & m_property)
                    {
                        int oldActive = m_activeControl;
                        m_activeControl = hit;
                        if (oldActive >= 0)
                        {
                            assert(oldActive >= m_startControl);
                            assert(oldActive <= m_endControl);
                            XControl* xctlOld = dui_controlArray[oldActive];
                            r += xctlOld->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBUP);
                        }
                        r += xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_LBUP);
                    }
                    else
                    {
                        m_activeControl = -1;
                        r += xctl->setStatus(XCONTROL_STATE_HOVERED, XMOUSE_LBUP);
                    }
                    xctl->ShowCursor();
                    if (nullptr != xctl->pfAction)
                    {
                        xctl->pfAction(this, m_message, 0, xctl->m_id);
                    }
                }
                else
                {
                    r += SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBUP);
                }
            }
            else 
            {
                if (DUI_PROP_HASVSCROLL & m_property)
                {
                    if (DUI_STATUS_VSCROLL & m_status)
                    {
                        r++;
                    }
                }
            }
        }
        else // the mouse is not in our area
        {
            r += SetAllControlStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBUP);
            if (DUI_PROP_HASVSCROLL & m_property)
            {
                if (DUI_STATUS_VSCROLL & m_status)
                {
                    m_status &= ~DUI_STATUS_VSCROLL;
                    r++;
                }
            }
        }

        {
            T* pT = static_cast<T*>(this);
            r += pT->Do_DUI_LBUTTONUP(uMsg, wParam, lParam, lpData);
        }

        if (r)
        {
            m_status |= DUI_STATUS_NEEDRAW;
            InvalidateDUIWindow();
        }

        return r;
    }

    int Do_DUI_LBUTTONDBLCLK(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_LBUTTONDBLCLK(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = 0;
        T* pT = static_cast<T*>(this);

        r = pT->Do_DUI_LBUTTONDBLCLK(uMsg, wParam, lParam, lpData);

        if (r)
        {
            m_status |= DUI_STATUS_NEEDRAW;
            InvalidateDUIWindow();
        }

        return r;
    }

    int Do_DUI_DESTROY(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_DESTROY(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        T* pT = static_cast<T*>(this);
        int ret = pT->Do_DUI_DESTROY(uMsg, wParam, lParam, lpData);
        return ret;
    }

    int Do_DUI_SETCURSOR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_SETCURSOR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        XControl* xctl;
        int r = 0;
        int xPos = (int)wParam;
        int yPos = (int)lParam;

        // the original xPos/yPos is related to the client area system of the host window. 
        xPos -= m_area.left;
        yPos -= m_area.top;
        for (int i = m_startControl; i <= m_endControl; i++)
        {
            xctl = dui_controlArray[i];
            assert(nullptr != xctl);
            if (xctl->IsOverMe(xPos, yPos))
            {
                r++;
                break;
            }
        }

        T* pT = static_cast<T*>(this);
        r += pT->Do_DUI_SETCURSOR(uMsg, wParam, lParam, lpData);

        if (r)
            SetDUIWindowCursor();
        return r;
    }

    int Do_DUI_CHAR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_CHAR(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = 0;
        if ((DUI_PROP_HANDLEKEYBOARD & m_property) && !DUIWindowInDragMode())
        {
            if (DUI_STATUS_ISFOCUS & m_status) // this window has the focus
            {
                T* pT = static_cast<T*>(this);
                r = pT->Do_DUI_CHAR(uMsg, wParam, lParam, lpData);
                if (r)
                {
                    m_status |= DUI_STATUS_NEEDRAW;
                    InvalidateDUIWindow();
                }
            }
        }
        return r;
    }

    int Do_DUI_KEYDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_KEYDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = 0;
        if ((DUI_PROP_HANDLEKEYBOARD & m_property) && !DUIWindowInDragMode())
        {
            if (DUI_STATUS_ISFOCUS & m_status)  // this window has the focus
            {
                T* pT = static_cast<T*>(this);
                r = pT->Do_DUI_KEYDOWN(uMsg, wParam, lParam, lpData);
                if (r)
                {
                    m_status |= DUI_STATUS_NEEDRAW;
                    InvalidateDUIWindow();
                }
            }
        }
        return r;
    }

    int Do_DUI_TIMER(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_TIMER(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = 0;
        if ((DUI_PROP_HANDLETIMER & m_property))
        {
            XControl* xctl;
            for (int i = m_startControl; i <= m_endControl; i++)
            {
                xctl = dui_controlArray[i];
                assert(nullptr != xctl);
                assert(xctl->m_id == i);
                r += xctl->OnTimer();
            }

            T* pT = static_cast<T*>(this);
            r += pT->Do_DUI_TIMER(uMsg, wParam, lParam, lpData);
            if (r)
            {
                m_status |= DUI_STATUS_NEEDRAW;
                InvalidateDUIWindow();
            }
        }
        return r;
    }

    int Do_DUI_MOUSELEAVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_MOUSELEAVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        return 0;
    }

    int Do_DUI_MOUSEHOVER(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_MOUSEHOVER(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        return 0;
    }

    int Do_DUI_SIZE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_SIZE(U32 uMsg, U64 wParam, U64 lParam, void* lpData)
    {
        T* pT = static_cast<T*>(this);
        int ret = pT->Do_DUI_SIZE(uMsg, wParam, lParam, lpData);
        return ret;
    }

    int Do_DUI_CREATE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int On_DUI_CREATE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = 0;
        T* pT = static_cast<T*>(this);

#ifdef _WIN32
        m_hWnd = (HWND)wParam;
        assert(IsRealWindow(m_hWnd));
#else
        m_hWnd = nullptr;
#endif
        {
            U32 initSize = DUI_ALLOCSET_SMALL_INITSIZE;
            U32 maxSize = DUI_ALLOCSET_SMALL_MAXSIZE;

            if (DUI_PROP_LARGEMEMPOOL & m_property)
            {
                initSize = DUI_ALLOCSET_DEFAULT_INITSIZE;
                maxSize = DUI_ALLOCSET_DEFAULT_MAXSIZE;
            }
            
            if(m_id[0] != '\0')
                m_pool = mempool_create((const char*)m_id, 0, initSize, maxSize);
            else 
                m_pool = mempool_create("DUI_WIN", 0, initSize, maxSize);

            if (nullptr != m_pool)
            {
                pT->InitControl();
                XControl* xctl;
                for (int i = m_startControl; i <= m_endControl; i++)
                {
                    xctl = dui_controlArray[i];
                    assert(nullptr != xctl);
                    assert(xctl->m_id = i);
                    xctl->pfAction = XControlAction;
                }
            }
            else
                SetDUIWindowInitFailed();
        }

        r = pT->Do_DUI_CREATE(uMsg, wParam, lParam, lpData);
        if (r)
            SetDUIWindowInitFailed();

        return r;
    }
};

#endif  /* __WT_DUI_WIN_H__ */

