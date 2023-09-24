#include "bitcoin.h"
#include "src/span.h"
#include "src/wallet/db.h"
#include "src/wallet/sqlite.h"
//#include "secp256k1/include/secp256k1.h"
//#include "sqlite//sqlite3.h"
#include "bitcoin/src/key.h"

static int _BitCoinInitialized = 0;

int BitCoinInit()
{
	int ret;

	ret = ECC_Start();
	if (0 == ret)
		return (-1);

	{
		CKey key;
		key.MakeNewKey(true);
	}
	
	_BitCoinInitialized = 1;
	return 0;
}

void BitCoinTerm()
{
	if (_BitCoinInitialized != 0)
	{
		ECC_Stop();
	}
}


using namespace wallet;

int bitcointest()
{

#if 0
	wallet::DatabaseOptions opt;
	opt.require_format = wallet::DatabaseFormat::SQLITE;
	opt.require_create = true;

	fs::path dir_path("c:\\windev");
	fs::path file_path("wochatx.db");

	SQLiteDatabase db(dir_path, file_path, opt);

	db.Open();
	db.Close();
#endif
    return 0;
}

#if 0
secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
secp256k1_context_destroy(ctx);

sqlite3* db;
sqlite3_stmt* stmt = NULL;
int           rc;

rc = sqlite3_open("wochat.db", &db);
if (rc)
{
	sqlite3_close(db);
	return -1;
}

sqlite3_close(db);
#endif
