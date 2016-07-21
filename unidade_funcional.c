#include "unidade_funcional.h"

void Atualizar_Unidade_Funcional(uint8_t clock_state, unidade_funcional_t *uf)
{
	int i;

	if(uf->estado == 0 && uf->er->b == 1 && uf->er->q1 == 0 && uf->er->q2 == 0) //Unidade esta livre e ha dados prontos na ER
	{
		uf->estado = 1;
		uf->tempo = 0;
		uf->taken = 2;	
	}
	if(uf->estado == 1)
	{
		if(clock_state == 0) //Foi de alta para baixa: descida de clock
			++(uf->tempo);
		switch(uf->er->tipo)
		{	
			case B_OPCODE: 
				if(uf->tempo >= XTIME_B)
				{
					uf->resultado = uf->er->a;
					uf->taken = 1;
					uf->estado = 2;
				}
				break;
			case ADD_OPCODE:
			case ADDI_OPCODE:
				if(uf->tempo >= XTIME_ADDI)
				{
					uf->resultado = uf->er->v1 + uf->er->v2;
					uf->estado = 2;
				}
				break;
			case SUB_OPCODE:
			case SUBI_OPCODE:
				if(uf->tempo >= XTIME_SUBI)
				{
					uf->resultado = uf->er->v1 - uf->er->v2;
					uf->estado = 2;
				}
				break;
			case MULT_OPCODE:
			case MULTI_OPCODE:
				if(uf->tempo >= XTIME_MULTI)
				{
					uf->resultado = uf->er->v1 * uf->er->v2;
					uf->estado = 2;
				}
				break;
			case DIV_OPCODE:
			case DIVI_OPCODE:
				if(uf->tempo >= XTIME_DIVI)
				{
					uf->resultado = uf->er->v1 * uf->er->v2;
					uf->estado = 2;	
				}
				break;
			case AND_OPCODE:
			case ANDI_OPCODE:
				if(uf->tempo >= XTIME_ANDI)
				{
					uf->resultado = uf->er->v1 & uf->er->v2;
					uf->estado = 2;	
				}
				break;
			case OR_OPCODE:
			case ORI_OPCODE:
				if(uf->tempo >= XTIME_ORI)
				{
					uf->resultado = uf->er->v1 | uf->er->v2;
					uf->estado = 2;	
				}
				break;
			case BEQ_OPCODE:
				if(uf->tempo >= XTIME_BEQ)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 == uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BNE_OPCODE:
				if(uf->tempo >= XTIME_BNE)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 != uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BGT_OPCODE:
				if(uf->tempo >= XTIME_BGT)
				{	
					uf->resultado = uf->er->a;
					if(uf->er->v1 > uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BGE_OPCODE:
				if(uf->tempo >= XTIME_BGE)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 >= uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BLT_OPCODE:
				if(uf->tempo >= XTIME_BLT)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 < uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BLE_OPCODE:
				if(uf->tempo >= XTIME_BLE)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 <= uf->er->v2)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case LI_OPCODE:
				if(uf->tempo >= XTIME_LI)
				{
					uf->resultado = uf->er->v1;
					uf->estado = 2;
				}
				break;
			case BEQZ_OPCODE:
				if(uf->tempo >= XTIME_BEQZ)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 == 0)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BNEZ_OPCODE:
				if(uf->tempo >= XTIME_BNEZ)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 != 0)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BGTZ_OPCODE:
				if(uf->tempo >= XTIME_BGTZ)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 > 0)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case BLEZ_OPCODE:
				if(uf->tempo >= XTIME_BLEZ)
				{
					uf->resultado = uf->er->a;
					if(uf->er->v1 < 0)
						uf->taken = 1;
					else
						uf->taken = 0;
					uf->estado = 2;	
				}
				break;
			case MOVE_OPCODE:
				if(uf->tempo >= XTIME_MOVE)
				{
					uf->resultado = uf->er->v1;
					uf->estado = 2;
				}
				break;
			case NEG_OPCODE:
				if(uf->tempo >= XTIME_NEG)
				{
					uf->resultado = -(uf->er->v1);
					uf->estado = 2;
				}
				break;
			case NOT_OPCODE:
				if(uf->tempo >= XTIME_NOT)
				{
					uf->resultado = !(uf->er->v1);
					uf->estado = 2;
				}
				break;
			case SLL_OPCODE:
				if(uf->tempo >= XTIME_SLL)
				{
					uf->resultado = uf->er->v1;
					for(i=0; i< uf->er->v2; ++i)
						uf->resultado *= 2;
					uf->estado = 2;
				}
				break;
			case SLR_OPCODE:
				if(uf->tempo >= XTIME_SLR)
				{
					uf->resultado = uf->er->v1;
					for(i=0; i< uf->er->v2; ++i)
						uf->resultado /= 2;
					uf->estado = 2;
				}
				break;
		}
	}
	if(uf->estado == 2)
	{
		if(uf->taken == 2)
		{
			if(Emitir_Sinal_CDB(uf->cdb, SINAL_RESULTADO_PRONTO, uf->resultado, uf->er->id))
			{
				uf->er->b = 0;
				uf->estado = 0;
			}
		}
		else if(uf->taken == 0)
		{
			if(Emitir_Sinal_CDB(uf->cdb, SINAL_SALTO_NOTTAKEN, uf->resultado, uf->er->id))
			{
				uf->er->b = 0;
				uf->estado = 0;
			}
		}
		else if(uf->taken == 1)
		{
			if(Emitir_Sinal_CDB(uf->cdb, SINAL_SALTO_TAKEN, uf->resultado, uf->er->id))
			{
				uf->er->b = 0;
				uf->estado = 0;
			}
		}	
	}
}
