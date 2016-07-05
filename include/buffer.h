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
	uint8_t tipo_buffer; //0 para buffer de LOAD, 1 para buffer de STORE
	uint8_t estado; //0 = livre, 1 = calculando endereco, 2 = pronta para acessar memoria
	uint8_t tipo_instrucao; //Pode ser LD (0x00), ST (0x01) ou LI (0x10)
	uint8_t registrador; //registrador destino para operacoes de LOAD, origem para operacoes STORE. Pode referenciar uma estacao de reserva, no caso de STORE
	uint32_t endereco; //endereco de memoria
}buffer_t;

#endif
