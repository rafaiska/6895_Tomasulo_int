#include "instrucao.h"

t_instrucao *decodificar_instrucao(uint32_t codigo)
{
	t_instrucao *nova;
	int aux;
	int operandos;
	int i;

	nova = malloc(sizeof(t_instrucao));
	nova->codificada = codigo;

	//FETCH OPCODE
	aux = codigo;
	for(i=0; i<26; ++i) //shift right 26 vezes
		aux /= 2;

	nova->tipo_instrucao = aux;

	for(i=0;i<26;++i)
		aux *= 2;

	operandos = codigo - aux;

	//FETCH OPERANDOS
	if(nova->tipo_instrucao == B_OPCODE) //Pega o label
	{
		this->imediato = operandos;
	}
	else //Pega primeiro registrador
	{
		aux = operandos;
		for(i=0;i<21;++i) //shift right 21 vezes
			aux /= 2;

		this->r_operando1 = aux;
	}
	if() //TODO: INSTRUCOES QUE TENHAM SEGUNDO REGISTRADOR
	{

	}
	else
	{

	}
	if() //TODO: INSTRUCOES QUE TENHAM TERCEIRO REGISTRADOR
	{

	}
	else
	{

	}

	return nova;
}
