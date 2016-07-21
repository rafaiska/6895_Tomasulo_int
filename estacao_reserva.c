#include "estacao_reserva.h"

void ER_Sinal_Pronto(estacao_reserva_t *er, uint8_t n_er, uint8_t produtora, int32_t dados)
{
	int i;

	for(i=0; i< n_er; ++i)
	{
		if(er[i].b != 0)
		{
			if(er[i].q1 == produtora)
			{
				er[i].q1 = 0;
				er[i].v1 = dados;
			}
			if(er[i].q2 == produtora)
			{
				er[i].q2 = 0;
				er[i].v2 = dados;
			}
		}
	}
}
