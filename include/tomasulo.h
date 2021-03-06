#ifndef TOMASULO_H_
#define TOMASULO_H_

#define MAX_REGISTERS 32

#include "instrucao.h"
#include "cdb.h"
#include "buffer.h"
#include "estacao_reserva.h"
#include "unidade_funcional.h"
#include "tad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TIPO ABSTRATO DE DADOS PARA MEMORIA
typedef struct memoria_t
{
	uint32_t tamanho; //tamanho do heap de memoria do programa
	uint32_t *heap;
	uint32_t text_start; //posicao de inicio da area de texto
	uint32_t text_end; //posicao de fim da area de texto

	//Os atributos a seguir sao para acesso da memoria pelos buffers de E/L
	uint8_t buffer_req; //Buffer que solicitou a operacao
	uint32_t tipo; //Tipo de instrucao
	int32_t resultado; //Valor a ser lido/escrito
	uint32_t endereco_acessado; //Endereco de memoria a ser lido/escrito
	uint8_t estado; //0 = livre, 1 = busy, 2 = pronto
	uint8_t tempo; //para cronometrar o tempo de execucao da operacao
}memoria_t;

//TIPO ABSTRATO DE DADOS PARA BANCO DE REGISTRADORES
//Eh possivel efetivar 3 instrucoes por ciclo
typedef struct b_registrador_t
{
	int32_t registrador[MAX_REGISTERS]; //registradores
	uint8_t qi[MAX_REGISTERS]; //numero da unidade que produzira valor para o registrador correspondente
	uint32_t pc; //program counter
	uint32_t sp; //stack pointer
	int32_t buffer[4]; //buffer para guardar escritas nos registradores
	int8_t buffer_endereco[4]; //endereco das escritas, -1 para NULL
	cdb_t *cdb;
}b_registrador_t;

//VARIAVEIS DE CLOCK
uint8_t clock_state; //Valor 0 = baixa tensao, valor 1 = alta tensao
uint32_t clock_count; //Conta pulsos de clock desde o inicio
//MODULO DE CLOCK EM ASSEMBLY
extern int _Atualiza_Clock_as();

/*PONTEIROS DOS COMPONENTES
 *Os componentes sao alocados conforme as quantidades
 *indicadas pelo arquivo de entrada (ou valores padroes
 *do simulador). Nas estacoes de reserva e buffers de store,
 *os componentes sao referenciados por um indice especial, uma
 *vez que os numeros de 0 a MAX_REGISTERS-1 sao usados para
 *referenciar os registradores.
 *
 *PROGRAM COUNTER = (MAX_REGISTERS) a (MAX_REGISTERS)
 *UNIDADES FUNCIONAIS DE SOMA = (MAX_REGISTERS +1) a (MAX_REGISTERS +n_uf_soma)
 *UNIDADES FUNCIONAIS DE MULT = (MAX_REGISTERS +n_uf_soma +1) a (MAX_REGISTERS +n_uf_soma +n_uf_mult)
 *UNIDADES FUNCIONAIS DE DIVI = (MAX_REGISTERS +n_uf_soma +n_uf_mult +1) a (MAX_REGISTERS + n_uf_total)
 *BUFFERS DE LOAD = (MAX_REGISTERS +n_uf_total +1) a (MAX_REGISTERS +n_uf_total +n_buff_load) 
 *BUFFERS DE STORE = (MAX_REGISTERS +n_uf_total +n_buff_load +1) a (MAX_REGISTERS +n_uf_total +n_buff_load +n_buff_store) 
 */
memoria_t *tomasulo_memoria;			//Unidade de memoria
b_registrador_t *tomasulo_registradores;	//Banco de registradores
estacao_reserva_t *tomasulo_er;			//Estacoes de reserva
unidade_funcional_t *tomasulo_uf;		//Unidades funcionais
buffer_t *tomasulo_buffer_load;			//Buffers de LOAD
buffer_t *tomasulo_buffer_store;		//Buffers de STORE
cdb_t *tomasulo_cdb;				//Common Data Bus (CDB)
uint32_t *tomasulo_busca;			//Vetor de instrucoes buscadas
instrucao_t *tomasulo_decodificacao;		//Vetor de instrucoes decodificadas

//VARIAVEIS DE SIMULACAO
uint8_t tomasulo_exit;			//Indica fim da simulacao quando diferente de zero

//VARIAVEIS DA UNIDADE DE BUSCA E DECODIFICACAO
uint8_t busca_instrucao_stall;		//Indica stall na busca de instrucao quando diferente de zero
uint8_t busca_instrucao_fim;		//Indica que a busca de instrucoes chegou ao fim da parte de texto
uint8_t emissao_instrucao_stall;	//Indica stall na emissao de instrucoes quando diferente de zero
uint8_t busca_instrucao_ready;		//Caso seja diferente de zero, instrucoes foram buscadas e estao prontas para serem emitidas
uint8_t fila_emissao_first;		//Primeira instrucao para ser emitida no vetor *tomasulo_decodificacao
uint8_t fila_emissao_last;		//Ultima instrucao de *tomasulo_decodificacao. Se fila_emissao_first == fila_emissao_last, entao a ultima instrucao da unidade estah sendo emitida, e o buscador pode alcancar mais instrucoes na memoria apos essa emissao
uint8_t fila_emissao_empty;		//Diferente de zero se a fila de emissao estiver vazia, zero caso contrario.

//QUANTIDADE DE COMPONENTES
uint8_t n_uf_soma;			//Numero de unidades funcionais de soma
uint8_t n_uf_mult;			//Numero de unidades funcionais de multiplicacao
uint8_t n_uf_divi;			//Numero de unidades funcionais de divisao
uint8_t n_uf_total;			//Soma de todas UF
uint8_t n_buff_load;			//Numero de buffers de load
uint8_t n_buff_store;			//Numero de buffers de store
uint8_t n_busca_ciclo;			//Numero de instrucoes buscadas e decodificadas em um ciclo de clock

//FUNCOES DE CONTROLE
int Atualiza_Clock(); //Altera o sinal de clock de um estado de alta para um de baixa tensao, e vice-versa. Duas chamadas dessa funcao correspondem a um pulso de clock. Retorna quantidade de pulsos desde o inicio da simulacao.
int Atualiza_Componentes(); //Atualiza todos os componentes do pipeline, retorna a vazao do pipeline (instrucoes emitidas - instrucoes efetivadas) para o ciclo de clock atual.
int Configurar_Tomasulo(); //Configura o algoritmo de Tomasulo para valores inseridos na entrada padrao
void Encerrar_Tomasulo(); //Desaloca heap de memoria e componentes alocados para o funcionamento do algoritmo
memoria_t *Inicializar_Memoria(uint32_t tamanho); //Inicializa o registro de memoria com o tamanho maximo do heap de memoria indicado (em palavras de 32 bits)
void Liberar_Memoria(memoria_t **memoria); //Desaloca o heap de memoria e o ponteiro para o registro de memoria
cdb_t *Inicializar_CDB(uint8_t tamanho_barramento); //Inicializa o Common Data Bus com o mesmo tamanho de barramento para dados, controle e endereco
void Liberar_CDB(cdb_t **cdb); //Desaloca os barramentos e o ponteiro para o registro de barramentos
int Definir_Arquitetura(char *linha); //Usada por Configurar_Tomasulo() para definir parametros de simulacao (numero de componentes, custo em ciclos de cada operacao) a partir de strings do arquivo de entrada. Retorna 0 em caso de sucesso, 1 em caso de falha
void Atualizar_Busca(); //Atualiza a unidade de busca, obtendo novas instrucoes caso nao esteja em stall
void Atualizar_Decodificacao(); //Atualiza o decodificador e a fila de emissao
uint8_t Emissao_Buffer(instrucao_t *instrucao, buffer_t *buffer); //Emite instrucao para um buffer de load/store
uint8_t Emissao_ER(instrucao_t *instrucao, estacao_reserva_t *er); //Emite instrucao para uma estacao de reserva
void Atualizar_Memoria(); //Processa leituras e escritas na memoria, emite ao CDB quando estao prontas
void Atualizar_CDB(); //Captura sinais de broadcast do CDB e os transmite aas estacoes de reserva, buffers e banco de registradores
void Checar_Tomasulo_Fim(); //Verifica se o programa terminou sua execucao
void Tomasulo_Memory_Dump(); //Mostra o conteudo da memoria virtual na saida padrao
#endif
