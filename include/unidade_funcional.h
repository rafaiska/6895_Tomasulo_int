#ifndef UNIDADE_FUNCIONAL_H_
#define UNIDADE_FUNCIONAL_H_

#include "instrucao.h"
#include "estacao_reserva.h"
#include "cdb.h"

typedef struct unidade_funcional_t
{
	estacao_reserva_t *er; //estacao de reserva correspondente a essa unidade
	int32_t resultado; //guarda o resultado da operacao ou endereco de salto
	uint8_t estado; //0 = livre, 1 = busy, 2 = pronto
	uint8_t tempo; //para cronometrar o tempo de execucao da instrucao
	uint8_t taken; //usado para registrar se instrucao de salto foi tomada (==1) ou nao (==0). Caso seja ==2, instrucao processada nao eh de salto
	cdb_t *cdb; //barramento comum de dados
}unidade_funcional_t;

void Atualizar_Unidade_Funcional(uint8_t clock_state, unidade_funcional_t *uf);

#endif
