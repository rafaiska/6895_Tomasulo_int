#ifndef BUFFER_H_
#define BUFFER_H_

/*
 * Tipos abstratos de dados e funcoes para manipular buffers
 * de LOAD/STORE. Esses funcionam de maneira similar aas
 * estacoes de reserva, mas para gerir acessos aa memoria, ao
 * inves de unidades funcionais.
 */

#include "instrucao.h"

typedef struct buffer_t
{
	uint8_t id; //id para referenciar essa unidade
	uint8_t estado; //0 = livre, 1 = calculando endereco, 2 = pronta para acessar memoria
	uint8_t tipo; //Pode ser LD (0x00), ST (0x01) ou LI (0x10)
	int32_t v1; //Valor a ser escrito/lido
	int32_t v2; //Endereco da leitura/escrita
	uint8_t q1; //Unidade que produzira o valor a ser escrito (para operacoes STORE)
	uint8_t q2; //Unidade que produzira o valor de endereco para E/L
}buffer_t;
			
void Buffer_Sinal_Pronto(buffer_t *buffer, uint8_t n_buffer, uint8_t produtor, int32_t dados);

#endif
