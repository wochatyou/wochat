#include "pch.h"

#include "wochat.h"


int GetSecretKey(LPCTSTR path, U8* sk)
{
	bool bRet = false;
	U8  hexSK[32];
	wchar_t iniFile[MAX_PATH + 1] = { 0 };
	
	swprintf((wchar_t*)iniFile, MAX_PATH, L"%s\\wochat.ini", path);

	DWORD fileAttributes = GetFileAttributesW(iniFile);
	if (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		bRet = true;

	if (bRet)
	{
		wchar_t hexSK[65] = { 0 };
		DWORD bytes = GetPrivateProfileString(L"global", L"SK", NULL, hexSK, 65, iniFile);
		if (bytes != 64)
		{
			bRet = false;
		}
		else
		{
			U8 hiChar, lowChar, hiValue, lowValue;
			for (int i = 0; i < 32; i++)
			{
				hiChar = hexSK[(i << 1)];
				if (hiChar >= '0' && hiChar <= '9')
					hiValue = hiChar - '0';
				else if (hiChar >= 'A' && hiChar <= 'F')
					hiValue = (hiChar - 'A') + 0x0A;
				else return 1;

				lowChar = hexSK[(i << 1) + 1];
				if (lowChar >= '0' && lowChar <= '9')
					lowValue = lowChar - '0';
				else if (lowChar >= 'A' && lowChar <= 'F')
					lowValue = (lowChar - 'A') + 0x0A;
				else return 1;

				sk[i] = (hiValue << 4 | lowValue);
			}
		}
	}

	if (!bRet)
	{
		int fd;
		U8  len, oneChar;
		U8  hexString[65];
		U8 text[256 + 1] = { 0 };

		NTSTATUS status = BCryptGenRandom(NULL, (PUCHAR)hexSK, 32, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
		if (STATUS_SUCCESS != status)
			return 1;

		if (0 != _tsopen_s(&fd, iniFile, _O_CREAT | _O_WRONLY | _O_TRUNC | _O_TEXT, _SH_DENYWR, 0))
			return 2;
		for (int i = 0; i < 32; i++)
		{
			sk[i] = hexSK[i];
			oneChar = (sk[i] >> 4) & 0x0F;
			if (oneChar < 0x0A)
				hexString[(i << 1)] = oneChar + '0';
			else
				hexString[(i << 1)] = (oneChar - 0x0A) + 'A';

			oneChar = sk[i] & 0x0F;
			if (oneChar < 0x0A)
				hexString[(i << 1) + 1] = oneChar + '0';
			else
				hexString[(i << 1) + 1] = (oneChar - 0x0A) + 'A';
		}
		hexString[64] = 0;

		sprintf_s((char*)text, 256, "[global]\nSK=%s\n", hexString);
		len = strlen((const char*)text);
		_write(fd, text, len);
		_close(fd);
	}

    return 0;
}