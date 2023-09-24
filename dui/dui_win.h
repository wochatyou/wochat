#ifndef __DUI_WIN_H__
#define __DUI_WIN_H__

#include "dui.h"
#include "dui_editbox.h"
#include "dui_mempool.h"

#define DUI_MAX_CONTROLS    64
#define DUI_ALLOCSET_DEFAULT_INITSIZE  (8 * 1024)
#define DUI_ALLOCSET_DEFAULT_MAXSIZE   (8 * 1024 * 1024)
#define DUI_ALLOCSET_SMALL_INITSIZE    (1 * 1024)
#define DUI_ALLOCSET_SMALL_MAXSIZE	   (8 * 1024)


typedef U32(*ThreadFunc)(void* lpData);

enum
{
    DUI_STATUS_NODRAW    = 0x00,    // do not need to draw      
    DUI_STATUS_VISIBLE   = 0x01,    // is this virtual window visible?
    DUI_STATUS_VSCROLL   = 0x02,    // is this virtual window has vertical scroll bar?
    DUI_STATUS_HSCROLL   = 0x04,    // is this virtual window has horizonal scroll bar?
    DUI_STATUS_ISFOCUS   = 0x08,    // is the input redirected into this virutal window?
    DUI_STATUS_NEEDRAW   = 0x10,    // does this virtual windows need to be redraw?
    DUI_STATUS_HASCARET  = 0x20,    // is this virutal window has a caret?
};

enum
{
    DUI_PROP_NONE             = 0x00000000,   // None Properties
    DUI_PROP_MOVEWIN          = 0x00000001,   // Move the whole window while LButton is pressed
    DUI_PROP_BTNACTIVE        = 0x00000002,   // have active button on this virutal window
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
    DEFAULT_BORDER_COLOR = 0xFFBBBBBB,
    DEFAULT_SCROLLBKG_COLOR = 0xFFF9F3F1,
    DEFAULT_SCROLLTHUMB_COLOR = 0xFFC0C1C4,
    DEFAULT_SCROLLTHUMB_COLORA = 0xFFAAABAD
};

template <class T>
class DUI_NO_VTABLE XWindowT
{
private:
    enum class XDragMode { DragNone, DragVertical, DragHorizonal };

public:
    typedef int (T::*ProcessOSMessage)(U32 uMsg, U64 wParam, U64 lParam, void* lpData);

#ifdef _WIN32
    HWND m_hWnd = nullptr;  // the pointer to point the host real/platform window
#else
    void* m_hWnd = nullptr;
#endif

    U32*    m_screen = nullptr;  // the memory block that will be rendered to the screen by the platform
    U32     m_size = 0;
    U8      m_Id[8] = { 0 }; // for debugging 
    XRECT   m_area = { 0 };  // the area of this window in the client area of parent window

    MemoryContext m_pool = nullptr;
    
    XControl* m_control[DUI_MAX_CONTROLS];
    U8  m_controlCount = 0;
    int m_activeControl = -1;

    ProcessOSMessage m_messageFuncPointerTab[256] = { 0 };  // we only handle 255 messages that should be enough

    const int m_scrollWidth = 8; // in pixel

    bool  m_cursorNormal = true;

    XPOINT m_ptOffset = { 0 };
    XPOINT m_ptOffsetOld = { 0 };
    XSIZE  m_sizeAll = { 0 };
    XSIZE  m_sizeLine = { 0 };
    XSIZE  m_sizePage = { 0 };
    int    m_cxyDragOffset = 0;
    XPOINT m_ptMouse = { 0 };

    XDragMode  m_DragMode = XDragMode::DragNone;

    U32  m_status = DUI_STATUS_VISIBLE;
    U32  m_property = DUI_PROP_NONE;
    U32  m_message = DUI_NULL;
    U32  m_backgroundColor = DEFAULT_BACKGROUND_COLOR;
    U32  m_scrollbarColor = DEFAULT_SCROLLBKG_COLOR;
    U32  m_thumbColor = DEFAULT_SCROLLTHUMB_COLOR;

    enum {
        MAX_XBUTTONS = 16,
        MAX_XBUTTON_BITMAPS = (MAX_XBUTTONS << 2)
    };

    XBitmap  m_bitmap[MAX_XBUTTON_BITMAPS];

public:
    XWindowT()
    {
        int i;
        U8 id;
        XBitmap* bmp;

        m_controlCount = 0;
        for (i = 0; i < DUI_MAX_CONTROLS; i++)
            m_control[i] = nullptr;
        
        //m_messageFuncPointerTab[DUI_NULL] = nullptr;
        for (i = 0; i < 256; i++) 
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

        static_assert(MAX_XBUTTONS < (1 << 6));

        // initialize the bitmap's status
        for (id = 0; id < MAX_XBUTTON_BITMAPS; id++)
        {
            bmp = &m_bitmap[id];
            bmp->id = id;
            bmp->data = nullptr;
            bmp->w = bmp->h = 0;
        }
    }

    ~XWindowT()
    {
        XControl* xctl;
        for (int i = 0; i < m_controlCount; i++)
        {
            xctl = m_control[i];
            assert(nullptr != xctl);
            xctl->Term();
        }

        if (nullptr != m_pool)
        {
            mempool_destroy(m_pool);
            m_pool = nullptr;
        }
    }

    void InitControl() {}

    // < 0 : I do not handle this message
    // = 0 : I handled, but I do not need to upgrade the screen
    // > 0 : I handled this message, and need the host window to upgrade the screen
    int HandleOSMessage(U32 uMsg, U64 wParam, U64 lParam, void* lpData)
    {
        int r = -1;

        U16 msgIdOS = DUI_MOD(uMsg, MESSAGEMAP_TABLE_SIZE);
        U8 msgId = DUIMessageOSMap[msgIdOS];  // lookup the message map from Platform to our DUI message

        if (DUI_NULL != msgId)
        {
            ProcessOSMessage pfMSG = m_messageFuncPointerTab[msgId];
            if (nullptr != pfMSG)
            {
                T* pT = static_cast<T*>(this);
                r = (pT->*pfMSG)(uMsg, wParam, lParam, lpData);
            }
        }

        return r;
    }

    static int ButtonAction(void* obj, U32 uMsg, U64 wParam, U64 lParam)
    {
        int ret = 0;

        T* pT = static_cast<T*>(obj);
        if (nullptr != pT)
            ret = pT->NotifyParent(uMsg, wParam, lParam);

        return ret;
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
            m_Id[i] = *id++;

        m_Id[bytes] = 0;
    }

    bool IsVisible() const
    {
        return (0 != (m_status & DUI_STATUS_VISIBLE));
    }

    XRECT* GetWindowArea()
    {
        return &m_area;
    }

    void PostWindowHide() {}
    void WindowHide()
    {
        m_status &= (~DUI_STATUS_VISIBLE);

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

    int NotifyParent(U32 uMsg, U64 wParam, U64 lParam)
    {
        PostWindowMessage(uMsg, wParam, lParam);
        return 0;
    }

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

    int ClearButtonStatus() 
    { 
        int ret = DUI_STATUS_NODRAW;
        U8 state;
        XButton* button;

        assert(m_buttonEndIdx < MAX_XBUTTONS);
        assert(m_buttonStartIdx >= 0 &&  m_buttonStartIdx < MAX_XBUTTONS);

        for (int i = m_buttonStartIdx; i <= m_buttonEndIdx; i++)
        {
            button = &m_button[i]; 

            if (XBUTTON_STATE_NORMAL != button->state && XBUTTON_STATE_ACTIVE != button->state)
            {
                m_status |= DUI_STATUS_NEEDRAW; // Invalidate the screen
                ret = DUI_STATUS_NEEDRAW;
            }

            button->state = XBUTTON_STATE_NORMAL;
        }

        if (m_buttonActiveIdx >= m_buttonStartIdx && m_buttonActiveIdx <= m_buttonEndIdx)
        {
            button = &m_button[m_buttonActiveIdx];
            button->state = XBUTTON_STATE_ACTIVE;
        }

        return ret;
    }

    U32* GetScreenBuffer()
    {
        return m_screen;
    }

    void* SetWindowCapture()
    {
        void* winhandle = nullptr;

        assert(IsRealWindow(m_hWnd));
        
        if (IsRealWindow(m_hWnd))
        {
#if defined(_WIN32)
            winhandle = ::SetCapture((HWND)m_hWnd);
#endif
        }

        return winhandle;
    }

    void* GetWindowCapture()
    {
        void* winhandle = nullptr;
        
        assert(IsRealWindow(m_hWnd));

#if defined(_WIN32)
        winhandle = ::GetCapture();
#endif
        return winhandle;
    }

    void ReleaseWindowCapture()
    {
#if defined(_WIN32)
        ::ReleaseCapture();
#endif
    }

    bool PostWindowMessage(U32 message, U64 wParam = 0, U64 lParam = 0)
    {
        bool bRet = false;

        assert(IsRealWindow(m_hWnd));

        if (IsRealWindow(m_hWnd))
        {
#if defined(_WIN32)
            bRet = ::PostMessage((HWND)m_hWnd, (UINT)message, (WPARAM)wParam, (LPARAM)lParam);
#endif
        }

        return bRet;
    }

#if 0
    void DrawButton(XButton* button)
    {
        U32* dst;
        U32* src;
        int dx = button->left;
        int dy = button->top;
        int w = m_area.right - m_area.left;
        int h = m_area.bottom - m_area.top;

        dst = m_screen;
        assert(nullptr != dst);
        assert(nullptr != button);

        if (XBUTTON_STATE_HIDDEN != button->state) // this button is visible
        {

            XBitmap* bitmap = button->imgNormal;
            switch (button->state)
            {
            case XBUTTON_STATE_PRESSED:
                bitmap = button->imgPress;
                break;
            case XBUTTON_STATE_HOVERED:
                bitmap = button->imgHover;
                break;
            case XBUTTON_STATE_ACTIVE:
                bitmap = button->imgActive;
                break;
            default:
                break;
            }

            assert(nullptr != bitmap);

            src = bitmap->data;
            assert(nullptr != src);

            if (XBUTTON_PROP_ROUND & button->property)
            {
                ScreenDrawRectRound(dst, w, h, src, bitmap->w, bitmap->h, dx, dy, m_backgroundColor, m_backgroundColor);
            }
            else
            {
                ScreenDrawRect(dst, w, h, src, bitmap->w, bitmap->h, dx, dy);
            }
        }
    }
#endif
    void UpdateControlPosition() {}
    void UpdatePosition() 
    {
        T* pT = static_cast<T*>(this);
        pT->UpdateControlPosition();
    }

    void SetPosition(RECT* r, U32* screen, U32 size = 0)
    {
        if (nullptr != r)
        {
            m_area.left   = r->left; 
            m_area.top    = r->top; 
            m_area.right  = r->right; 
            m_area.bottom = r->bottom;
        }
        else
        {
            m_area.left = m_area.top = m_area.right = m_area.bottom = 0;
        }

        m_screen = screen;
        
        if(0 != size)
            m_size = size;
        else
            m_size = (U32)((m_area.right - m_area.left) * (m_area.bottom - m_area.top));

        m_status |= DUI_STATUS_NEEDRAW;
        
        if (nullptr != r)
        {
            T* pT = static_cast<T*>(this);
            pT->UpdatePosition();
        }
    }

    int Draw() { return 0; }

    U32* Render() 
    {
        U32* screenBuf = nullptr;
        U8 status = m_status & (DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW);

        // We only draw this virtual window when 1: it is visible, and 2: it needs draw
        if ((DUI_STATUS_VISIBLE | DUI_STATUS_NEEDRAW) == status)
        {
            int w = m_area.right - m_area.left;
            int h = m_area.bottom - m_area.top;
            XControl* xctl;

            assert(nullptr != m_screen);
            // fill the whole screen of this virutal window with a single color
            ScreenClear(m_screen, m_size, m_backgroundColor);

            if (DUI_STATUS_VSCROLL & m_status) // We have the vertical scroll bar to draw
            {
                int thumb_start, thumb_height, thumb_width;
                int vOffset = m_ptOffset.y;
                int vHeight = m_sizeAll.cy;
                assert(m_sizeAll.cy > h);

                thumb_width = m_scrollWidth - 2;
                thumb_start = (vOffset * h) / vHeight;
                thumb_height = (h * h) / vHeight;
                if(thumb_height < 32)
                    thumb_height = 32; // we keep the thumb mini size to 32 pixels

                // Draw the vertical scroll bar
                ScreenFillRect(m_screen, w, h, m_scrollbarColor, m_scrollWidth, h, w - m_scrollWidth, 0);
                ScreenFillRectRound(m_screen, w, h, m_thumbColor, thumb_width, thumb_height, w - m_scrollWidth + 1, thumb_start, m_scrollbarColor, 0xFFD6D3D2);
            }

            for (int i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                xctl->Draw();
            }

            T* pT = static_cast<T*>(this);
            pT->Draw();

            screenBuf = m_screen;
        }

        // to avoid another needless draw
        m_status &= (~DUI_STATUS_NEEDRAW);
        
        return screenBuf;
    }

    int DoSize(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnSize(U32 uMsg, U64 wParam, U64 lParam, void* lpData)
    {
        XRECT* r = (XRECT*)lParam;
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
        m_screen = (U32*)lpData;
        if (nullptr == m_screen)
        {
            m_area.left = m_area.top = m_area.right = m_area.bottom = 0;
            m_size = 0;
        }

        if (nullptr != r && nullptr != m_screen)
        {
            XControl* xctl;
            for (U8 i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                xctl->AttachParent(m_screen, r->right - r->left, r->bottom - r->top);
            }

            T* pT = static_cast<T*>(this);
            pT->UpdateControlPosition();
            pT->DoSize(uMsg, wParam, lParam, lpData);
        }

        m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window

        return DUI_STATUS_NEEDRAW;
    }

    int DoMouseWheel(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnMouseWheel(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        if (DUI_PROP_HANDLEVWHEEL & m_property)
        {
            int r0 = DUI_STATUS_NODRAW;
            int r1 = DUI_STATUS_NODRAW;
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
                        r0 = DUI_STATUS_NEEDRAW;
                }

                {  // let the derived class to do its stuff
                    T* pT = static_cast<T*>(this);
                    r1 = pT->DoMouseWheel(uMsg, wParam, lParam, lpData);
                }
            }

            if (DUI_STATUS_NODRAW != r0 || DUI_STATUS_NODRAW != r1)
            {
                m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
                return DUI_STATUS_NEEDRAW;
            }
        }

        return DUI_STATUS_NODRAW;

    }

    int DoMouseMove(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnMouseMove(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r0  = DUI_STATUS_NODRAW;
        int r1  = DUI_STATUS_NODRAW;
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int w = m_area.right - m_area.left;
        int h = m_area.bottom - m_area.top;

        if (XDragMode::DragVertical == m_DragMode)
        {
            m_status |= DUI_STATUS_VSCROLL;
            m_ptOffset.y = m_ptOffsetOld.y + ((yPos - m_cxyDragOffset) * m_sizeAll.cy) / h;
            if (m_ptOffset.y < 0)
                m_ptOffset.y = 0;
            if (m_ptOffset.y > (m_sizeAll.cy - h))
                m_ptOffset.y = m_sizeAll.cy - h;
        }
        else 
        {
            if (XWinPointInRect(xPos, yPos, &m_area)) // the mosue is in this area
            {
                if (DUI_PROP_HASVSCROLL & m_property) // handle the vertical bar
                {
                    U32 status = m_status;  // save previous state
                    m_status &= (~DUI_STATUS_VSCROLL);

                    assert(m_area.right > m_scrollWidth);
                    
                    if (m_sizeAll.cy > h) // the virutal window size is bigger than the real window size
                    {
                        U32 thumbColor = m_thumbColor;
                        m_status |= DUI_STATUS_VSCROLL;
                        m_thumbColor = DEFAULT_SCROLLTHUMB_COLOR;
                        if(xPos >= (m_area.right - m_scrollWidth))
                            m_thumbColor = DEFAULT_SCROLLTHUMB_COLORA;

                        if(thumbColor != m_thumbColor)
                            r0 = DUI_STATUS_NEEDRAW;
                    }

                    if ((DUI_STATUS_VSCROLL & status) != (DUI_STATUS_VSCROLL & m_status))
                        r0 = DUI_STATUS_NEEDRAW;
                }

                if (GetWindowCapture() != m_hWnd)
                {
                    XControl* xctl;
                    int hit = -1;  // no hit so far
                    // transfer the coordination from real window to local virutal window
                    xPos -= m_area.left;
                    yPos -= m_area.top;
                    assert(xPos >= 0);
                    assert(yPos >= 0);

                    for (int i = 0; i < m_controlCount; i++)
                    {
                        xctl = m_control[i];
                        assert(nullptr != xctl);
                        if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                        {
                            hit = i;
                            break;
                        }
                    }
                    if (-1 != hit) // we are hovering on some control
                    {
                        r0 = xctl->setStatus(XCONTROL_STATE_HOVERED, XMOUSE_MOVE);
                        xctl->ShowCursor();
                    }
                    else // we have to scan the whole control array
                    {
                        r0 = 0;
                        for (int i = 0; i < m_controlCount; i++)
                        {
                            xctl = m_control[i];
                            assert(nullptr != xctl);
                            if(i != m_activeControl)
                                r0 += xctl->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_MOVE);
                            else 
                                r0 += xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_MOVE);
                        }
                    }
                }
            }
            else // the mouse is not in our area
            {
                // handle the vertical bar
                if (DUI_STATUS_VSCROLL & m_status)
                {
                    m_status &= (~DUI_STATUS_VSCROLL); // we should not dispaly the vertical bar
                    r0 = DUI_STATUS_NEEDRAW;
                }
            }
        }

        {  // let the derived class to do its stuff
            T* pT = static_cast<T*>(this);
            r1 = pT->DoMouseMove(uMsg, wParam, lParam, lpData);
        }

        if (DUI_STATUS_NODRAW != r0 || DUI_STATUS_NODRAW != r1 || XDragMode::DragNone != m_DragMode)
        {
            m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
            return DUI_STATUS_NEEDRAW;
        }
        return DUI_STATUS_NODRAW;
    }

    int DoFocusGet(U32 uMsg, int xPos, int yPos, void* lpData = nullptr) { return 0; }
    int DoFocusLose(U32 uMsg, int xPos, int yPos, void* lpData = nullptr) { return 0; }

    int DoLButtonDown(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnLButtonDown(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        XControl* xctl;
        int rx = DUI_STATUS_NODRAW;
        int r0 = DUI_STATUS_NODRAW;
        int r1 = DUI_STATUS_NODRAW;
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        T* pT = static_cast<T*>(this);

        m_DragMode = XDragMode::DragNone;

        if (XWinPointInRect(xPos, yPos, &m_area))
        {
            int hit = -1;
            int w = m_area.right - m_area.left;
            int h = m_area.bottom - m_area.top;

            // the mouse click my area, so I have the focus
            m_status |= DUI_STATUS_ISFOCUS; 
            rx = pT->DoFocusGet(uMsg, xPos, yPos, lpData);

            // handle the vertical bar
            if (DUI_PROP_HASVSCROLL & m_property)
            {
                if (xPos >= (m_area.right - m_scrollWidth))
                {
                    U32 status = m_status;
                    m_status &= (~DUI_STATUS_VSCROLL);

                    if (m_sizeAll.cy > h)
                    {
                        int thumb_start = (m_ptOffset.y * h) / m_sizeAll.cy;
                        int thumb_height = (h * h) / m_sizeAll.cy;
                
                        if(thumb_height < 16)
                            thumb_height = 16; // we keep the thumb mini size to 16 pixels

                        m_status |= DUI_STATUS_VSCROLL;

                        assert(m_ptOffset.y >= 0);
                        assert(m_ptOffset.y <= m_sizeAll.cy - h);

                        if (yPos > (m_area.top + thumb_start) && yPos < (m_area.top + thumb_start + thumb_height))
                        {
                            // we hit the thumb
                            m_cxyDragOffset = yPos;
                            m_ptOffsetOld.y = m_ptOffset.y;
                            m_DragMode = XDragMode::DragVertical;
                            SetWindowCapture();
                        } 
                        else
                        {
                            int thumb_start_new = (yPos - m_area.top) - (thumb_height >> 1);
                            if (thumb_start_new < 0)
                                thumb_start_new = 0;
                            if (thumb_start_new > h - thumb_height)
                                thumb_start_new = h - thumb_height;

                            m_ptOffset.y = (thumb_start_new * m_sizeAll.cy)/h;
                        }

                        m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
                        return DUI_STATUS_NEEDRAW;
                    }

                    if ((DUI_STATUS_VSCROLL & status) != (DUI_STATUS_VSCROLL & m_status))
                        r0 = DUI_STATUS_NEEDRAW;
                }
            }

            // transfer the coordination from real window to local virutal window
            xPos -= m_area.left; 
            yPos -= m_area.top;
            assert(xPos >= 0);
            assert(yPos >= 0);

            for (int i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                {
                    hit = i;
                    break;
                }
            }
            if (-1 != hit) // we are hitting some button
            {
                r0 = xctl->setStatus(XCONTROL_STATE_PRESSED, XMOUSE_LBDOWN);
                xctl->ShowCursor();
            }
            else
            {  
                r0 = 0;
                for (int i = 0; i < m_controlCount; i++)
                {
                    xctl = m_control[i];
                    assert(nullptr != xctl);
                    if (i != m_activeControl)
                        r0 += xctl->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBDOWN);
                    else
                        r0 += xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_LBDOWN);
                }
                // if the mouse does not hit the button, we can move the whole real window
                if (DUI_PROP_MOVEWIN & m_property)
                    PostWindowMessage(WM_NCLBUTTONDOWN, HTCAPTION, lParam);
            }
        }
        else
        {
            m_status &= ~DUI_STATUS_ISFOCUS; // this window lose focus

            for (int i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                if (i != m_activeControl)
                    r0 += xctl->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBDOWN);
                else
                    r0 += xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_LBDOWN);
            }

            rx = pT->DoFocusLose(uMsg, xPos, yPos, lpData);
        }

        r1 = pT->DoLButtonDown(uMsg, wParam, lParam, lpData);

        if (DUI_STATUS_NODRAW != r0 || DUI_STATUS_NODRAW != r1 || DUI_STATUS_NODRAW != rx)
        {
            m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
            return DUI_STATUS_NEEDRAW;
        }
        return DUI_STATUS_NODRAW;

    }

    int DoLButtonUp(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnLButtonUp(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r0 = DUI_STATUS_NODRAW;
        int r1 = DUI_STATUS_NODRAW;
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (GetWindowCapture() == m_hWnd)
        {
            ReleaseWindowCapture();
        }
        m_DragMode = XDragMode::DragNone;

        m_ptOffsetOld.x = -1, m_ptOffsetOld.y = -1;

        if (XWinPointInRect(xPos, yPos, &m_area))
        {
            XControl* xctl;
            int hit = -1;
            // transfer the coordination from real window to local virutal window
            xPos -= m_area.left;
            yPos -= m_area.top;
            assert(xPos >= 0);
            assert(yPos >= 0);

            for (int i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                if (xctl->IsOverMe(xPos, yPos))  // we find the control that the mouse is hovering
                {
                    hit = i;
                }
            }
            if (-1 != hit) // we are hitting some button
            {
                if (DUI_PROP_BTNACTIVE & m_property)
                {
                    int oldActive = m_activeControl;
                    m_activeControl = hit;
                    r0 = xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_LBUP);
                    if (oldActive >= 0)
                    {
                        assert(oldActive < m_controlCount);
                        XControl* xctlOld = m_control[oldActive];
                        r0 += xctlOld->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBUP);
                    }
                }
                else
                {
                    m_activeControl = -1;
                    r0 = xctl->setStatus(XCONTROL_STATE_HOVERED, XMOUSE_LBUP);
                }
                xctl->ShowCursor();
            }
            else
            {
                r0 = 0;
                for (int i = 0; i < m_controlCount; i++)
                {
                    xctl = m_control[i];
                    assert(nullptr != xctl);
                    if (i != m_activeControl)
                        r0 += xctl->setStatus(XCONTROL_STATE_NORMAL, XMOUSE_LBUP);
                    else
                        r0 += xctl->setStatus(XCONTROL_STATE_ACTIVE, XMOUSE_LBUP);
                }
            }
        }

        {
            T* pT = static_cast<T*>(this);
            r1 = pT->DoLButtonUp(uMsg, wParam, lParam, lpData);
        }

        if (DUI_STATUS_NODRAW != r0 || DUI_STATUS_NODRAW != r1)
        {
            m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window
            return DUI_STATUS_NEEDRAW;
        }

        return DUI_STATUS_NODRAW;
    }

    int DoLButtonDoubleClick(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnLButtonDoubleClick(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int ret = DUI_STATUS_NODRAW;
        T* pT = static_cast<T*>(this);

        ret = pT->DoLButtonDoubleClick(uMsg, wParam, lParam, lpData);

        if (DUI_STATUS_NODRAW != ret)
            m_status |= DUI_STATUS_NEEDRAW;  // need to redraw this virtual window

        return ret;
    }

    int DoCreate(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0;  }
    int OnCreate(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int ret = 0;
        U32 initSize = DUI_ALLOCSET_SMALL_INITSIZE;
        U32 maxSize = DUI_ALLOCSET_SMALL_MAXSIZE;

#ifdef _WIN32
        m_hWnd = (HWND)wParam;
#else
        m_hWnd = (void*)wParam;
#endif
        assert(IsRealWindow(m_hWnd));

        if (DUI_PROP_LARGEMEMPOOL & m_property)
        {
            initSize = DUI_ALLOCSET_DEFAULT_INITSIZE;
            maxSize = DUI_ALLOCSET_DEFAULT_MAXSIZE;
        }
        m_pool = mempool_create(0, initSize, maxSize);
        if (nullptr == m_pool)
            return 1;

        T* pT = static_cast<T*>(this);
        pT->InitControl();
        ret = pT->DoCreate(uMsg, wParam, lParam, lpData);

        return ret;
    }

    int DoDestroy(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnDestroy(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        T* pT = static_cast<T*>(this);
        int ret = pT->DoDestroy(uMsg, wParam, lParam, lpData);
        return ret;
    }

    int DoSetCursor(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnSetCursor(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        XControl* xctl;
        int r = 0;
        int r0 = 0;
        int r1 = 0;
        int xPos = (int)wParam;
        int yPos = (int)lParam;

        // the original xPos/yPos is related to the client area system of the host window. 
        xPos -= m_area.left;
        yPos -= m_area.top;

        for (int i = 0; i < m_controlCount; i++)
        {
            xctl = m_control[i];
            assert(nullptr != xctl);
            if (xctl->IsOverMe(xPos, yPos))
            {
                r0 = 1;
                break;
            }
        }

        T* pT = static_cast<T*>(this);
        r1 = pT->DoSetCursor(uMsg, wParam, lParam, lpData);
        if (r0 || r1)
            r = 1;
        return r;
    }

    int DoChar(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnChar(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = DUI_STATUS_NODRAW;
        if (DUI_PROP_HANDLEKEYBOARD & m_property)
        {
            T* pT = static_cast<T*>(this);

            //ProcessOSMessage pfOSM = m_functionTab[0];
            //r = (pT->*pfOSM)(uMsg, wParam, lParam, lpData);
            r = pT->DoChar(uMsg, wParam, lParam, lpData);
            if (DUI_STATUS_NODRAW != r)
                m_status |= DUI_STATUS_NEEDRAW;
        }

        return r;
    }

    int DoKeyPress(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnKeyPress(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r = DUI_STATUS_NODRAW;
        if (DUI_PROP_HANDLEKEYBOARD & m_property)
        {
            T* pT = static_cast<T*>(this);
            r = pT->DoKeyPress(uMsg, wParam, lParam, lpData);
            if (DUI_STATUS_NODRAW != r)
                m_status |= DUI_STATUS_NEEDRAW;
        }

        return r;
    }

    int DoTimer(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr) { return 0; }
    int OnTimer(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
    {
        int r0 = DUI_STATUS_NODRAW;
        int r1 = DUI_STATUS_NODRAW;

        if (DUI_PROP_HANDLETIMER & m_property)
        {
            XControl* xctl;
            for (int i = 0; i < m_controlCount; i++)
            {
                xctl = m_control[i];
                assert(nullptr != xctl);
                r0 += xctl->OnTimer();
            }

            T* pT = static_cast<T*>(this);
            r1 = pT->DoTimer(uMsg, wParam, lParam, lpData);
            if (0 != r0 + r1)
                m_status |= DUI_STATUS_NEEDRAW;
        }

        return (r0 + r1);
    }


};

#endif  /* __DUI_WIN_H__ */

