#ifndef UNIDADE_FUNCIONAL_H_
#define UNIDADE_FUNCIONAL_H_

#include "instrucao.h"

typedef struct unidade_funcional_t
{
	uint32_t op1;
	uint32_t op2;
	uint8_t tipo_instrucao;
	uint8_t tipo_uf;
	uint8_t pronto;
	uint8_t tempo;
}unidade_funcional_t;

#endif
