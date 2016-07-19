#ifndef UNIDADE_FUNCIONAL_H_
#define UNIDADE_FUNCIONAL_H_

#include "instrucao.h"

typedef struct unidade_funcional_t
{
	int32_t op1;
	int32_t op2;
	uint8_t tipo_instrucao;
	uint8_t estado; //0 = livre, 1 = busy, 2 = pronto
	uint8_t tempo;
}unidade_funcional_t;

#endif
