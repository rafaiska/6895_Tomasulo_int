#include "tomasulo.h"

int Atualiza_Clock()
{
	if(clock_state == 0)
		clock_state = 1;
	else
	{
		clock_state = 0;
		++clock_count;
	}
}

int Atualiza_Componentes()
{
	//TODO: Atualiza banco de registradores
	//TODO: Atualiza memoria
	//TODO: Atualiza unidades funcionais
	//TODO: Atualiza buffers de LOAD/STORE
	//TODO: Atualiza estacoes de reserva
	//TODO: Atualiza decodificador (e lista de emissao)
	//TODO: Atualiza buscador
}

int Configurar_Tomasulo()
{
	//TODO: Carregar o arquivo do thomas zulu com as configuracoes da maquina
}

void Encerrar_Tomasulo()
{
	//TODO: DAR FREE EM TUTO, TUTO, TUTO
}
