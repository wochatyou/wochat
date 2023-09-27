#include "dui/dui.h"
#include "xwindef.h"

U16 wochat_ME[]			= { 0x5173,0x4e8e,0x6211,0x81ea,0x5df1,0};
U16 wochat_TALK[]		= { 0x804a,0x5929,0 };
U16 wochat_FRIEND[]		= { 0x6211,0x7684,0x597d,0x53cb,0 };
U16 wochat_QUAN[]		= { 0x670b,0x53cb,0x5708,0 };
U16 wochat_COIN[]		= { 0x6211,0x7684,0x865a,0x62df,0x8d27,0x5e01,0 };
U16 wochat_FAVORITE[]	= { 0x6211,0x7684,0x6536,0x85cf,0 };
U16 wochat_FILE[]       = { 0x6587,0x4ef6,0x7ba1,0x7406,0 };
U16 wochat_SETTING[]	= { 0x7cfb,0x7edf,0x8bbe,0x7f6e,0 };
U16 wochat_NETWORK[]	= { 0x7f51,0x7edc,0x72b6,0x51b5,0 };

U16 wochat_EMOJI[]       = { 0x53d1, 0x8868, 0x60c5, 0x5305, 0 };
U16 wochat_UPLOAD[]      = { 0x4e0a, 0x4f20, 0x6587, 0x4ef6, 0 };
U16 wochat_CAPTURE[]     = { 0x622a, 0x5c4f, 0 };
U16 wochat_CHATHISTORY[] = { 0x804a, 0x5929, 0x5386, 0x53f2, 0x8bb0, 0x5f55, 0 };
U16 wochat_AUDIOCALL[]   = { 0x8bed, 0x97f3, 0x901a, 0x8bdd, 0 };
U16 wochat_VIDEOCALL[]   = { 0x89c6, 0x9891, 0x901a, 0x8bdd, 0 };
U16 wochat_SENDMESSAGE[] = { 0x53d1, 0x9001, 0x6d88, 0x606f, 0 };

void InitToolTipMessage()
{
	for (int i = 0; i < DUI_MAX_CONTROLS; i++)
		dui_tooltip[i] = nullptr;

	dui_tooltip[XWIN0_BUTTON_ME]       = (U16*)wochat_ME;
	dui_tooltip[XWIN0_BUTTON_TALK]     = (U16*)wochat_TALK;
	dui_tooltip[XWIN0_BUTTON_FRIEND]   = (U16*)wochat_FRIEND;
	dui_tooltip[XWIN0_BUTTON_QUAN]     = (U16*)wochat_QUAN;
	dui_tooltip[XWIN0_BUTTON_COIN]     = (U16*)wochat_COIN;
	dui_tooltip[XWIN0_BUTTON_FAVORITE] = (U16*)wochat_FAVORITE;
	dui_tooltip[XWIN0_BUTTON_FILE]     = (U16*)wochat_FILE;
	dui_tooltip[XWIN0_BUTTON_SETTING]  = (U16*)wochat_SETTING;
	dui_tooltip[XWIN0_BUTTON_NETWORK]  = (U16*)wochat_NETWORK;

	dui_tooltip[XWIN5_BUTTON_EMOJI]       = (U16*)wochat_EMOJI;
	dui_tooltip[XWIN5_BUTTON_UPLOAD]      = (U16*)wochat_UPLOAD;
	dui_tooltip[XWIN5_BUTTON_CAPTURE]     = (U16*)wochat_CAPTURE;
	dui_tooltip[XWIN5_BUTTON_CHATHISTORY] = (U16*)wochat_CHATHISTORY;
	dui_tooltip[XWIN5_BUTTON_AUDIOCALL]   = (U16*)wochat_AUDIOCALL;
	dui_tooltip[XWIN5_BUTTON_VIDEOCALL]   = (U16*)wochat_VIDEOCALL;
	dui_tooltip[XWIN5_BUTTON_SENDMESSAGE] = (U16*)wochat_SENDMESSAGE;
}
