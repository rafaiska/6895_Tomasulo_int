#ifndef INSTRUCAO_H_
#define INSTRUCAO_H_

#include <stdint.h>
#include <stdlib.h>

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

typedef struct t_instrucao
{
	uint32_t codificada;
	int tipo_instrucao;
	int r_operando1;
	int r_operando2;
	int r_operando3;
	int imediato;
}t_instrucao;

t_instrucao *decodificar_instrucao(uint32_t codigo); //cria uma nova struct instrucao a partir de uma instrucao codificada

#endif
