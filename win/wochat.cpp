#include "wochat.h"
#include <secp256k1/secp256k1.h>
#include <secp256k1/secp256k1_ecdh.h>
#include "dui/dui.h"
#include "xwindef.h"
#include "bitcoin/src/crypto/chacha20.h"

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

int GetPKfromSK(U8* sk, U8* pk, U8* pkPlain)
{
	int r;
	U8 c, cH, cL;
	size_t len;
	secp256k1_context* ctx;
	secp256k1_pubkey pubkey;

	ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

	r = secp256k1_ec_pubkey_create(ctx, &pubkey, sk);
	len = 33;
	r = secp256k1_ec_pubkey_serialize(ctx, pk, &len, &pubkey, SECP256K1_EC_COMPRESSED);

	for (int i = 0; i < 33; i++)
	{
		cH = pk[i] >> 4;
		cL = pk[i] & 0x0F;
		c = (cH > 9) ? (cH - 10 + 'A') : (cH + '0');
		pkPlain[i << 1] = c;
		c = (cL > 9) ? (cL - 10 + 'A') : (cL + '0');
		pkPlain[(i << 1) + 1] = c;
	}

	secp256k1_context_destroy(ctx);
	return 0;
}

int GetKeyfromSKPK(U8* sk, U8* pk, U8* k)
{
	int r;
	secp256k1_context* ctx;
	secp256k1_pubkey pubkey;
	ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

	r = secp256k1_ec_pubkey_parse(ctx, &pubkey, pk, 33);
	r = secp256k1_ecdh(ctx, k, &pubkey, sk, NULL, NULL);

	secp256k1_context_destroy(ctx);
	return 0;
}

int GetTextHeightInPixel(U16* text, U16 length, int width)
{
	int height = 0;
	HRESULT hr = S_OK;
	IDWriteTextLayout* pTextLayout = nullptr;

	assert(nullptr != g_pDWriteFactory);

	hr = g_pDWriteFactory->CreateTextLayout((const wchar_t*)text, length, g_pTextFormatMessage, width, 1, &pTextLayout);
	if (S_OK == hr && nullptr != pTextLayout)
	{
		DWRITE_TEXT_METRICS tm;
		pTextLayout->GetMetrics(&tm);
		height = (int)(tm.height) + 1;
		SafeRelease(&pTextLayout);
	}

	return height;
}