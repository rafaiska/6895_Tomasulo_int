#ifndef CDB_H
#define CDB_H

/*
 * cdb.h: Tipos de dados e funcoes para implementar o Common
 * Data Bus (CDB). Uma vez que ele conecta todos os demais
 * componentes do algoritmo de Tomasulo, seu funcionamento eh 
 * de vital importancia.
 */

#include "instrucao.h"

#define SINAL_NO_SIGNAL		0x00	//CDB livre
#define SINAL_RESULTADO_PRONTO	0x01	//Resultado pronto em uma unidade funcional/memoria. Estacoes de reserva e buffers sao informados
#define SINAL_SALTO_TAKEN	0x02	//Instrucao de salto tomada. Causa flush na fila de emissao, busca novas instrucoes
#define SINAL_SALTO_NOTTAKEN	0x03	//Instrucao de salto nao tomada. Libera stall da busca e continua emissao

typedef struct cdb_t
{
	uint8_t sinal; //Valores definidos acima
	int32_t dados; //Dados transmitidos
	uint8_t endereco; //Unidade que emitiu o sinal
}cdb_t;

uint8_t Emitir_Sinal_CDB(cdb_t *cdb, uint8_t sinal, int32_t dados, uint8_t endereco); //Tenta emitir um sinal ao CDB. Retorna 0 em caso de falha

#endif
