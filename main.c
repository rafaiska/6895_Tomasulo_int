#include "tad.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
	t_lista *teste;
	int i;
	t_instrucao *instro;

	srand(time(NULL));

	teste = criar_lista();

	for(i=0; i<10; ++i)
	{
		instro = malloc(sizeof(t_instrucao));
		instro->codificada = (uint32_t) rand();
		inserir_instrucao_lista(teste, instro);
	}		
	imprimir_lista(teste);
	esvaziar_lista(teste);
	imprimir_lista(teste);

	return 0;
}
