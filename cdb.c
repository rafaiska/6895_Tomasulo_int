#include "cdb.h"

uint8_t Emitir_Sinal_CDB(cdb_t *cdb, uint8_t sinal, int32_t dados, uint8_t endereco)
{
	if(cdb->sinal != SINAL_NO_SIGNAL)
		return 0;
	cdb->sinal = sinal;
	cdb->dados = dados;
	cdb->endereco = endereco;
	return 1;
}
