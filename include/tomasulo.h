#ifndef TOMASULO_H_
#define TOMASULO_H_

#define MAX_REGISTERS 32

#include "instrucao.h"
#include "cdb.h"
#include "buffer.h"
#include "estacao_reserva.h"
#include "unidade_funcional.h"
#include "tad.h"

//TIPO ABSTRATO DE DADOS PARA MEMORIA
typedef struct memoria_t
{
	uint8_t status; //especifica se a unidade de memoria esta livre (valor zero) ou ocupada (valor diferente de zero).
	uint32_t tamanho; //tamanho do heap de memoria do programa
	uint32_t *heap;
	uint32_t data; //posicao de inicio da parte de dados
	uint32_t text; //posicao de inicio da area de texto
}memoria_t;

//TIPO ABSTRATO DE DADOS PARA BANCO DE REGISTRADORES
typedef struct b_registrador_t
{
	uint32_t registrador[MAX_REGISTERS]; //registradores
	uint32_t pc; //program counter
	uint32_t sp; //stack pointer
}b_registrador_t;

//VARIAVEIS DE CLOCK
uint8_t clock_state; //Valor 0 = baixa tensao, valor 1 = alta tensao
uint32_t clock_count; //Conta pulsos de clock desde o inicio

//PONTEIROS DOS COMPONENTES
memoria_t *tomasulo_memoria;			//Unidade de memoria
b_registrador_t *tomasulo_registradores;	//Banco de registradores
estacao_reserva_t *tomasulo_er;			//Estacoes de reserva
unidade_funcional_t *tomasulo_uf;		//Unidades funcionais
buffer_t *tomasulo_buffer_load;			//Buffers de LOAD
buffer_t *tomasulo_buffer_store;		//Buffers de STORE
cdb_t *tomasulo_cdb;				//Common Data Bus (CDB)

//VARIAVEIS DE SIMULACAO
uint8_t tomasulo_exit;			//Indica fim da simulacao
uint8_t n_uf_soma;			//Numero de unidades funcionais de soma
uint8_t n_uf_mult;			//Numero de unidades funcionais de multiplicacao
uint8_t n_uf_divi;			//Numero de unidades funcionais de divisao
uint8_t n_buff_load;			//Numero de buffers de load
uint8_t n_buff_store;			//Numero de buffers de store
uint8_t n_busca_ciclo;			//Numero de instrucoes buscadas e decodificadas em um ciclo de clock

//FUNCOES DE CONTROLE
int Atualiza_Clock(); //Altera o sinal de clock de um estado de alta para um de baixa tensao, e vice-versa. Duas chamadas dessa funcao correspondem a um pulso de clock. Retorna quantidade de pulsos desde o inicio da simulacao.
int Atualiza_Componentes(); //Atualiza todos os componentes do pipeline, retorna a vazao do pipeline (instrucoes emitidas - instrucoes efetivadas) para o ciclo de clock atual.
int Configurar_Tomasulo(); //Configura o algoritmo de Tomasulo para valores inseridos na entrada padrao
void Encerrar_Tomasulo(); //Desaloca heap de memoria e componentes alocados para o funcionamento do algoritmo

#endif
