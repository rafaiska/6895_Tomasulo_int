#ifndef INSTRUCAO_H_
#define INSTRUCAO_H_

/* Tipos de dados e funcoes para tratar instrucoes
 * codificadas. 
 */

#include <stdint.h>
#include <stdlib.h>

#define ROTULO_MAX_SIZE 32 //Maior tamanho admitido para um rotulo em codigo assembly

//INSTRUCTION SET DA ARQUITETURA:
#define LD_OPCODE	0x00
#define ST_OPCODE	0x01
#define MOVE_OPCODE	0x02
#define NEG_OPCODE	0x03
#define NOT_OPCODE	0x04
#define ADD_OPCODE	0x05
#define SUB_OPCODE	0x06
#define MULT_OPCODE	0x07
#define DIV_OPCODE	0x08
#define AND_OPCODE	0x09
#define OR_OPCODE	0x0A
#define SLL_OPCODE	0x0B
#define SLR_OPCODE	0x0C

#define LI_OPCODE	0x10
#define BEQZ_OPCODE	0x11
#define BNEZ_OPCODE	0x12
#define BGTZ_OPCODE	0x13
#define BLEZ_OPCODE	0x14

#define ADDI_OPCODE	0x20
#define SUBI_OPCODE	0x21
#define MULTI_OPCODE	0x22
#define DIVI_OPCODE	0x23
#define ANDI_OPCODE	0x24
#define ORI_OPCODE	0x25
#define BEQ_OPCODE	0x26
#define BNE_OPCODE	0x27
#define BGT_OPCODE	0x28
#define BGE_OPCODE	0x29
#define BLT_OPCODE	0x2A
#define BLE_OPCODE	0x2B

#define B_OPCODE	0x30

//Tempos de execucao para cada tipo de instrucao (podem ser modificados pelo usuario)
uint8_t XTIME_LD;
uint8_t XTIME_ST;
uint8_t XTIME_MOVE;
uint8_t XTIME_NEG;
uint8_t XTIME_NOT;
uint8_t XTIME_ADD;
uint8_t XTIME_SUB;
uint8_t XTIME_MULT;
uint8_t XTIME_DIV;
uint8_t XTIME_AND;
uint8_t XTIME_OR;
uint8_t XTIME_SLL;
uint8_t XTIME_SLR;
uint8_t XTIME_LI;
uint8_t XTIME_BEQZ;
uint8_t XTIME_BNEZ;
uint8_t XTIME_BGTZ;
uint8_t XTIME_BLEZ;
uint8_t XTIME_ADDI;
uint8_t XTIME_SUBI;
uint8_t XTIME_MULTI;
uint8_t XTIME_DIVI;
uint8_t XTIME_ANDI;
uint8_t XTIME_ORI;
uint8_t XTIME_BEQ;
uint8_t XTIME_BNE;
uint8_t XTIME_BGT;
uint8_t XTIME_BGE;
uint8_t XTIME_BLT;
uint8_t XTIME_BLE;
uint8_t XTIME_B;

//Registro de instrucao: utilizado pelo codificador/decodificador
typedef struct t_instrucao
{
	uint32_t codificada;
	int tipo_instrucao;
	int r_operando1;
	int r_operando2;
	int r_operando3;
	int imediato;
}t_instrucao;

//Lista de rotulos para um programa em assembly
typedef struct t_lista_rotulo
{
	char rotulo[ROTULO_MAX_SIZE];
	uint32_t endereco;
	struct t_lista_rotulo *prox;
}t_lista_rotulo;

t_instrucao *decodificar_instrucao(uint32_t codigo); //cria uma nova struct instrucao a partir de uma instrucao codificada
uint32_t codificar_instrucao(char *instrucao); //codifica uma instrucao em uma linha de codigo em linguagem de montagem
uint32_t *montar_codigo(char *codigo, t_lista_rotulo *labels); //monta codigo em linguagem de montagem, aplicando analise sintatica e preenchendo tabela de labels (rotulos). Retorna um vetor de instrucoes de 32 bits em codigo de maquina

#endif