#ifndef ESTACAO_RESERVA_H_
#define ESTACAO_RESERVA_H_

#include "instrucao.h"

typedef struct estacao_reserva_t
{
	uint8_t tipo; //OPCODE DA INSTRUCAO
	int q1; //produtor do operando 1 (-1 caso ja tenha sido obtido o valor)
	int q2; //produtor do operando 2 (-1 caso ja tenha sido obtido o valor)
	int v1; //valor do operando 1
	int v2; //valor do operando 2
	uint32_t a; //endereco
	int b; //busy
}estacao_reserva_t;

#endif
