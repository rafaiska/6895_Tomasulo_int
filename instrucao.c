#include "instrucao.h"

t_instrucao *Decodificar_Instrucao(uint32_t codigo)
{
	t_instrucao *nova;
	int aux;
	int operandos;
	int i;

	nova = malloc(sizeof(t_instrucao));
	nova->codificada = codigo;

	//FETCH OPCODE
	aux = codigo;
	for(i=0; i<26; ++i) //shift right 26 vezes
		aux /= 2;

	nova->tipo_instrucao = aux;

	for(i=0;i<26;++i)
		aux *= 2;

	operandos = codigo - aux;

	//FETCH OPERANDOS
	if(nova->tipo_instrucao == B_OPCODE) //Pega o label
	{
		nova->imediato = operandos;
	}
	else //Pega primeiro registrador
	{
		aux = operandos;
		for(i=0;i<21;++i) //shift right 21 vezes
			aux /= 2;

		nova->r_operando1 = aux;
	}
	if(1) //TODO: INSTRUCOES QUE TENHAM SEGUNDO REGISTRADOR
	{

	}
	else
	{

	}
	if(1) //TODO: INSTRUCOES QUE TENHAM TERCEIRO REGISTRADOR
	{

	}
	else
	{

	}

	return nova;
}

uint32_t Codificar_Instrucao(char *instrucao)
{
	int i = 0;
	int j = 0;
	char aux[32];
	uint32_t retorno;

	if(instrucao[0] == '.' && instrucao[1] == 'i')
	{
		while(instrucao[i] != ' ')
			++i;
		++i;
		while(instrucao[i] != ' ')
		{
			aux[j] = instrucao[i];
			++j;
			++i;
		}
		aux[j] = '\0';
		retorno = atoi(aux);
	}
	else
	{
		//TODO: Codificar
	}

	return retorno;
}

uint32_t *Montar_Codigo(char *codigo, uint32_t *heap, uint32_t tamanho_memoria)
{
	int i, j;
	char instrucao[128];
	lista_rotulo_t *lista = NULL;
	uint32_t posicao = 0;

	//FASE1: Percorrer todo o codigo para enumerar as labels
	i=0;
	j=0;
	while(codigo[i] != '\0')
	{
		if(codigo[i] == ':')
		{
			instrucao[j] = '\0';
			Adicionar_Rotulo(&lista, instrucao, posicao);
			j=0;		
		}
		else if(codigo[i] == '\n')
		{
			instrucao[j] = '\0';
			posicao += Calcular_Tamanho_Instrucao(instrucao);
			j=0;
		}
		else if(codigo[i] == '\t');
		else
		{
			instrucao[j] = codigo[i];
			++j;
		}
		++i;
	}
	instrucao[j] = '\0';
	posicao += Calcular_Tamanho_Instrucao(instrucao);

	if(posicao > tamanho_memoria*4)
	{
		printf("ERRO: Memoria virtual eh muito pequena para o programa\n");
		exit(1);
	}

	//TODO: FASE2: Codificar o programa
	
}

int Adicionar_Rotulo(lista_rotulo_t **lista, char *nome, uint32_t posicao)
{
	lista_rotulo_t *novo;
	novo = malloc(sizeof(lista_rotulo_t));
	lista_rotulo_t *anterior = *lista;

	if(anterior == NULL)
	{
		*lista = novo;
	}
	else
	{
		while(anterior->prox != NULL)
			anterior = anterior->prox;
		anterior->prox = novo;
	}

	novo->prox = NULL;
	novo->endereco = posicao;
	strcpy(novo->rotulo, nome);

	return 0;
}

uint32_t Calcular_Tamanho_Instrucao(char *instrucao)
{
	int i = 0;
	int j = 0;
	char aux[32];
	uint32_t retorno;

	while(instrucao[i] == ' ' || instrucao[i] == '\t')
		++i;

	if(instrucao[i] == '.')
	{
		if(instrucao[i+1] == 'i')
			return 4;
		else
			return 0;
	}
	
	return 4;
}
