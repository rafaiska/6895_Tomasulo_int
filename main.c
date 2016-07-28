#include "tomasulo.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{

	//Obtem configuracoes da entrada padrao e inicializa os componentes
	if(Configurar_Tomasulo() != 0)
	{
		printf("Erro ao configurar o simulador!\n");
		return 1;
	}

	printf("\n==========================\nESTADO INICIAL DA MEMORIA:\n==========================\n");
	Tomasulo_Memory_Dump();

	while(tomasulo_exit == 0)
	{
		_Atualiza_Clock_as(); //FUNCAO ASSEMBLY x86_64 para atualizar o clock. Caso nao funcione, habilite a de baixo
		//Atualiza_Clock(); //Altera o sinal de clock de um estado de alta para um de baixa tensao, ou vice-versa.
		Atualiza_Componentes(); //Atualiza todos os componentes do pipeline.
	}
	
	printf("\n========================\nESTADO FINAL DA MEMORIA:\n========================\n");
	Tomasulo_Memory_Dump();
	
	Encerrar_Tomasulo(); //Desaloca heap de memoria e componentes alocados para o funcionamento do algoritmo
	return 0;
}
