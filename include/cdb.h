#ifndef CDB_H
#define CDB_H

/*
 * cdb.h: Tipos de dados e funcoes para implementar o Common
 * Data Bus (CDB). Uma vez que ele conecta todos os demais
 * componentes do algoritmo de Tomasulo, seu funcionamento eh 
 * de vital importancia.
 */

#include "instrucao.h"

#define SINAL_RESULTADO_PRONTO 0x01 //Resultado pronto em uma unidade funcional/buffer

//Registro para sinais de controle/endereco
typedef struct sinal_t
{
	void *origem;
	uint8_t origem_tipo;
	uint8_t tipo;
	uint32_t endereco;
}sinal_t;

typedef struct cdb_t
{
	uint8_t largura_barramento;
	uint8_t barramentos_em_uso;
	struct sinal_t *sinais;
	uint32_t *dados;
}cdb_t;

#endif
