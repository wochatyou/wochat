#ifndef __WT_WOCHAT_H__
#define __WT_WOCHAT_H__

#include "dui/dui.h"

extern ID2D1Factory* g_pD2DFactory;
extern IDWriteFactory* g_pDWriteFactory;
extern IDWriteTextFormat* g_pTextFormat0;

int GetSecretKey(LPCTSTR path, U8* sk);

#endif // __WT_WOCHAT_H__