#ifndef __WOCHAT_WINDOWS2_H__
#define __WOCHAT_WINDOWS2_H__

#include "dui/dui_win.h"

U16 msg[] = { 12, 0x5468,0x661f,0x661f,0xff1a,0x4e0d,0x77e5,0x6211,0x8005,0x8c13,0x6211,0x4f55,0x6c42 };

U16 gname[15];

U16 gname1[] = { 6, 0x0044,0x0042,0x0041,0x57f9,0x8bad,0x7fa4 };
U16 gname2[] = { 4, 0x6587, 0x4ef6, 0x4f20, 0x8f93 };
U16 gname3[] = { 7, 0x0041,0x0049,0x804a,0x5929,0x673a,0x5668,0x4eba };
U16 timestamp[] = { 11, 0x0030,0x0039,0x003a,0x0032,0x0038,0x003a,0x0033,0x0032,0x0020,0x0041,0x004d };

class XWindow2 : public XWindowT <XWindow2>
{
private:
	enum {
		SELECTED_COLOR = 0xFFC6C6C6,
		HOVERED_COLOR  = 0xFFDADADA,
		DEFAULT_COLOR  = 0xFFE5E5E5
	};

	enum {
		ITEM_HEIGHT = 64,
		ICON_HEIGHT = 40,
		ITEM_MARGIN = ((ITEM_HEIGHT - ICON_HEIGHT)>>1)
	};

	int		m_lineHeight0 = 0;
	int		m_lineHeight1 = 0;

	XChatGroup* m_chatgroupRoot       = nullptr;
	XChatGroup* m_chatgroupSelected   = nullptr;
	XChatGroup* m_chatgroupSelectPrev = nullptr;
	XChatGroup* m_chatgroupHovered    = nullptr;

public:
	XWindow2()
	{
		m_backgroundColor = DEFAULT_COLOR;
		m_scrollbarColor = DEFAULT_COLOR;
		m_message = WM_XWINDOWS02;
		m_property |= (DUI_PROP_HASVSCROLL | DUI_PROP_HANDLEVWHEEL | DUI_PROP_LARGEMEMPOOL);
	}

	~XWindow2()	{}

public:
	XChatGroup* GetSelectedChatGroup()
	{
		return m_chatgroupSelected;
	}

	int LoadChatGroupList()
	{
		int i, total = 0;
		assert(nullptr == m_chatgroupRoot);
		assert(nullptr != m_pool);

		m_sizeAll.cy = 0;
		m_sizeLine.cy = 0;

		m_chatgroupRoot = (XChatGroup*)palloc0(m_pool, sizeof(XChatGroup));
		if (nullptr != m_chatgroupRoot)
		{
			XChatGroup* p;
			XChatGroup* q;

			p = m_chatgroupRoot;
			p->mempool = mempool_create(0, DUI_ALLOCSET_SMALL_INITSIZE, DUI_ALLOCSET_SMALL_MAXSIZE);
			if (nullptr != p->mempool)
			{
				m_chatgroupSelected = m_chatgroupRoot;
				p->id = 0;
				p->icon = (U32*)xbmpGroup;
				gname[0] = 14;
				gname[1] = g_PKey1Plain[0];
				gname[2] = g_PKey1Plain[1];
				gname[3] = g_PKey1Plain[2];
				gname[4] = g_PKey1Plain[3];
				gname[5] = g_PKey1Plain[4];
				gname[6] = g_PKey1Plain[5];
				gname[7] = L'-';
				gname[8] = L'-';
				gname[9] = g_PKey1Plain[60];
				gname[10] = g_PKey1Plain[61];
				gname[11] = g_PKey1Plain[62];
				gname[12] = g_PKey1Plain[63];
				gname[13] = g_PKey1Plain[64];
				gname[14] = g_PKey1Plain[65];

				p->name = (U16*)gname;
				p->w = ICON_HEIGHT;
				p->h = ICON_HEIGHT;
				p->height = ITEM_HEIGHT;
				p->width = 0;
				p->lastmsg = (U16*)msg;
				p->tsText = (U16*)timestamp;
				p->next = nullptr;
			}
			else
			{
				pfree(m_chatgroupRoot);
				m_chatgroupRoot = nullptr;
				return 1;
			}

			total = 1;
			for (i = 1; i < 64; i++)
			{
				q = (XChatGroup*)palloc0(m_pool, sizeof(XChatGroup));
				if (nullptr == q)
					break;
				q->mempool = mempool_create(0, DUI_ALLOCSET_SMALL_INITSIZE, DUI_ALLOCSET_SMALL_MAXSIZE);
				if (nullptr == q->mempool)
				{
					pfree(q);
					break;
				}
				total++;
				p->next = q;
				p = q;
				p->next = nullptr;
				p->id = i;
				p->height = ITEM_HEIGHT;
				p->lastmsg = (U16*)msg;
				p->tsText = (U16*)timestamp;
				switch (i % 3)
				{
				case 0:
					p->icon = (U32*)xbmpFileTransfer;
					p->name = (U16*)gname2;
					break;
				case 1:
					p->icon = (U32*)xbmpChatGPT;
					p->name = (U16*)gname3;
					break;
				case 2:
					p->icon = (U32*)xbmpGroup;
					p->name = (U16*)gname1;
					break;
				}
				p->w = ICON_HEIGHT;
				p->h = ICON_HEIGHT;
			}

		}
		else return 0;

		m_sizeAll.cy = total * ITEM_HEIGHT;
		m_sizeLine.cy = ITEM_HEIGHT/3;
		return 0;
	}

	int Do_DUI_CREATE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int ret = 0;
		ret = LoadChatGroupList();

		return ret; 
	}

	int Do_DUI_DESTROY(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{
		XChatGroup* p = m_chatgroupRoot;
		
		while (nullptr != p)
		{
			assert(nullptr != p->mempool);
			mempool_destroy(p->mempool);
			p->mempool = nullptr;
			p = p->next;
		}

		return 0;
	}

	void UpdateControlPosition()
	{
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;
		int H = ITEM_HEIGHT;
		XTextDrawInfo* tdi;
		XTextDrawInfo* p;
		XTextDrawInfo* q;

		assert(nullptr != m_pool);

		U16 count = (U16)(h / H + 2);
		U16 total = 0;
		p = m_textDrawInfo;
		while (nullptr != p)
		{
			total++;
			p = p->next;
		}

		if (total < count)
		{
			U16 items = count - total;
			tdi = (XTextDrawInfo*)palloc0(m_pool, sizeof(XTextDrawInfo)*items);
			if (nullptr != tdi)
			{
				q = nullptr;
				for (U16 i = 0; i < items; i++)
				{
					p = tdi + i;
					p->next = q;
					q = p;
				}
				if (nullptr == m_textDrawInfo)
					m_textDrawInfo = p;
				else
				{
					q = m_textDrawInfo;
					while (nullptr != q->next)
						q = q->next;
					q->next = p;
				}
			}
		}
	}

	int Do_DUI_PAINT(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{
		U32 color;
		int dx, dy, pos;
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;
		int margin = (DUI_STATUS_VSCROLL & m_status) ? m_scrollWidth : 0;
		int W = DUI_ALIGN_DEFAULT32(w - ITEM_MARGIN - ITEM_MARGIN - ICON_HEIGHT - margin - 4);
		int H = ITEM_HEIGHT;

		XTextDrawInfo* tdi = m_textDrawInfo;
		m_textDrawInfoCount = 0;
		dx = ITEM_MARGIN;
		XChatGroup* p = m_chatgroupRoot;
		pos = 0;
		while (nullptr != p)
		{
			if (pos + ITEM_HEIGHT > m_ptOffset.y)
			{
				if (p == m_chatgroupSelected)
				{
					color = SELECTED_COLOR;
				}
				else if (p == m_chatgroupHovered)
				{
					color = HOVERED_COLOR;
				}
				else
				{
					color = DEFAULT_COLOR;
				}
				dy = pos - m_ptOffset.y;
				ScreenFillRect(m_screen, w, h, color, w - margin, ITEM_HEIGHT, 0, dy);
				ScreenDrawRectRound(m_screen, w, h, p->icon, p->w, p->h, dx, dy + ITEM_MARGIN, color, color);
				assert(nullptr != tdi);
				tdi->left = ITEM_MARGIN + ITEM_MARGIN + ICON_HEIGHT;
				tdi->top = dy;
				tdi->right = w - margin;
				tdi->bottom = tdi->top + ITEM_HEIGHT;
				tdi->textLen0 = p->name[0];
				tdi->text0 = p->name+1;
				tdi->textLen1 = p->lastmsg[0];
				tdi->text1 = p->lastmsg + 1;
				tdi->textLen2 = p->tsText[0];
				tdi->text2 = p->tsText + 1;
				m_textDrawInfoCount++;
				tdi = tdi->next;
			}
			p = p->next;
			pos += ITEM_HEIGHT;
			if (pos >= (m_ptOffset.y + h))
				break;

		}
		return 0;
	}

	int Do_DUI_MOUSEMOVE(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int hit = -1, margin;
		int ret = DUI_STATUS_NODRAW;
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;
		XChatGroup* hovered = m_chatgroupHovered;

		RECT area = { 0 };
		RECT* r = &area;
		r->left = 0;
		margin = (DUI_STATUS_VSCROLL & m_status) ? m_scrollWidth : 0;
		r->right = w - margin;

		if (XWinPointInRect(xPos, yPos, &m_area))
		{
			int pos = 0;
			// transfer the coordination from screen to local virutal window
			xPos -= m_area.left;
			yPos -= m_area.top;

			assert(xPos >= 0);
			assert(yPos >= 0);

			XChatGroup* p = m_chatgroupRoot;
			while (nullptr != p)
			{
				r->top = pos; r->bottom = r->top + ITEM_HEIGHT;
				if (XWinPointInRect(xPos, yPos + m_ptOffset.y, r))
				{
					m_chatgroupHovered = p;
					hit++;
					if (hovered != m_chatgroupHovered) // The hovered item is changed, we need to redraw
						ret = DUI_STATUS_NEEDRAW;
					break;
				}
				p = p->next;
				pos += ITEM_HEIGHT;
				if (pos >= (m_ptOffset.y + h))
					break;
			}
		}
		if (-1 == hit)
		{
			if (nullptr != m_chatgroupHovered)
			{
				m_chatgroupHovered = nullptr;
				ret = DUI_STATUS_NEEDRAW;
			}
		}

		return ret;
	}

	int Do_DUI_LBUTTONDOWN(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int hit = -1, margin;
		int ret = DUI_STATUS_NODRAW;
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		RECT area = { 0 };
		RECT* r = &area;
		r->left = 0;
		margin = (DUI_STATUS_VSCROLL & m_status) ? m_scrollWidth : 0;
		r->right = w - margin;

		if (XWinPointInRect(xPos, yPos, &m_area))
		{
			int pos = 0;
			// transfer the coordination from screen to local virutal window
			xPos -= m_area.left;
			yPos -= m_area.top;

			XChatGroup* p = m_chatgroupRoot;
			while (nullptr != p)
			{
				r->top = pos; r->bottom = r->top + ITEM_HEIGHT;
				if (XWinPointInRect(xPos, yPos + m_ptOffset.y, r))
				{
					m_chatgroupHovered = p;
					m_chatgroupSelected = p;
					hit++;
					ret = DUI_STATUS_NEEDRAW;
					break;
				}
				p = p->next;
				pos += ITEM_HEIGHT;
				if (pos >= (m_ptOffset.y + h))
					break;
			}
		}
		if (-1 == hit)
		{
			if (nullptr != m_chatgroupHovered)
			{
				m_chatgroupHovered = nullptr;
				ret = DUI_STATUS_NEEDRAW;
			}
		}

		return ret;
	}

	int Do_DUI_LBUTTONUP(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		int hit = -1, margin;
		int ret = DUI_STATUS_NODRAW;
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int w = m_area.right - m_area.left;
		int h = m_area.bottom - m_area.top;

		RECT area = { 0 };
		RECT* r = &area;
		r->left = 0;
		margin = (DUI_STATUS_VSCROLL & m_status) ? m_scrollWidth : 0;
		r->right = w - margin;

		if (XWinPointInRect(xPos, yPos, &m_area))
		{
			int pos = 0;
			// transfer the coordination from screen to local virutal window
			xPos -= m_area.left;
			yPos -= m_area.top;

			XChatGroup* p = m_chatgroupRoot;
			while (nullptr != p)
			{
				r->top = pos; r->bottom = r->top + ITEM_HEIGHT;
				if (XWinPointInRect(xPos, yPos + m_ptOffset.y, r))
				{
					m_chatgroupHovered = p;
					m_chatgroupSelected = p;
					hit++;
					ret = DUI_STATUS_NEEDRAW;
					break;
				}
				p = p->next;
				pos += ITEM_HEIGHT;
				if (pos >= (m_ptOffset.y + h))
					break;
			}
		}
		if (-1 == hit)
		{
			if (nullptr != m_chatgroupHovered)
			{
				m_chatgroupHovered = nullptr;
				ret = DUI_STATUS_NEEDRAW;
			}
		}
		else // we hit some item
		{
			assert(nullptr != m_chatgroupSelected);
			if (m_chatgroupSelectPrev != m_chatgroupSelected)
			{
				// do something here
				NotifyParent(m_message, (U64)m_chatgroupSelected, 0);
			}
			m_chatgroupSelectPrev = m_chatgroupSelected; // prevent double selection
		}

		return ret;
	}

	int Do_DUI_MOUSEWHEEL(U32 uMsg, U64 wParam, U64 lParam, void* lpData = nullptr)
	{ 
		return Do_DUI_MOUSEMOVE(uMsg, wParam, lParam, lpData);
	}
};

#endif  /* __WOCHAT_WINDOWS2_H__ */

