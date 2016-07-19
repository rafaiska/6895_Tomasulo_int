#ifndef ESTACAO_RESERVA_H_
#define ESTACAO_RESERVA_H_

#include "instrucao.h"

typedef struct estacao_reserva_t
{
	uint8_t id; //usado para identificar essa unidade
	uint32_t tipo; //OPCODE DA INSTRUCAO
	uint8_t q1; //produtor do operando 1 (0 caso ja tenha sido obtido o valor)
	uint8_t q2; //produtor do operando 2 (0 caso ja tenha sido obtido o valor)
	int32_t v1; //valor do operando 1
	int32_t v2; //valor do operando 2
	uint32_t a; //endereco
	int b; //busy: 0 caso esteja livre, 1 caso esteja ocupada
}estacao_reserva_t;

#endif
