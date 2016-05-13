#include "tad.h"

t_lista *criar_lista()
{
	t_lista *nova = malloc(sizeof(t_lista));
	nova->primeiro = NULL;
	nova->ultimo = NULL;
	nova->tamanho = 0;
	return nova;
}

int inserir_instrucao_lista(t_lista *lista, t_instrucao *instrucao)
{
	t_lista_no *novo = malloc(sizeof(t_lista_no));
	t_lista_no *p = lista->primeiro;

	if(p == NULL)
	{
		lista->primeiro = novo;
	}
	else
	{
		while(p->proximo != NULL)
			p = p->proximo;

		p->proximo = novo;
	}
	lista->ultimo = novo;
	novo->proximo = NULL;
	novo->instrucao = instrucao;
	++(lista->tamanho);
	return lista->tamanho;
}

void esvaziar_lista(t_lista *lista)
{
	while(remover_primeira_instrucao(lista) > 0);
}

int remover_primeira_instrucao(t_lista *lista)
{
	t_lista_no *d;

	d = lista->primeiro;
	if(d != NULL)
	{
		lista->primeiro = lista->primeiro->proximo;
		free(d->instrucao);
		free(d);
		--(lista->tamanho);
	}
	else
	{
		printf("ERRO: REMOVENDO DE LISTA VAZIA\n");
	}

	if(lista->primeiro == NULL)
		lista->ultimo = NULL;

	return lista->tamanho;
}

void imprimir_lista(t_lista *lista)
{
	t_lista_no *p;
	int i=0;

	p = lista->primeiro;

	printf("Tamanho da lista: %d. Elementos:\n", lista->tamanho);

	while(p != NULL)
	{
		printf("\tInstrucao %d: %08X\n", i, p->instrucao->codificada);
		p = p->proximo;
		++i;
	}
}
