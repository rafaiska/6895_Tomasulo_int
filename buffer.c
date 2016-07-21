#include "buffer.h"

void Buffer_Sinal_Pronto(buffer_t *buffer, uint8_t n_buffer, uint8_t produtor, int32_t dados)
{
	int i;

	for(i=0; i< n_buffer; ++i)
	{
		if(buffer[i].estado != 0)
		{
			if(buffer[i].q1 == produtor)
			{
				buffer[i].q1 = 0;
				buffer[i].v1 = dados;
			}
			if(buffer[i].q2 == produtor)
			{
				buffer[i].q2 = 0;
				buffer[i].v2 = dados;
			}
		}
	}
}
