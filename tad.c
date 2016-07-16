#include "tad.h"

lista_t *criar_lista()
{
	lista_t *nova = malloc(sizeof(lista_t));
	nova->primeiro = NULL;
	nova->ultimo = NULL;
	nova->tamanho = 0;
	return nova;
}

int inserir_instrucao_lista(lista_t *lista, instrucao_t *instrucao)
{
	lista_no_t *novo = malloc(sizeof(lista_no_t));
	lista_no_t *p = lista->primeiro;

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

void esvaziar_lista(lista_t *lista)
{
	while(remover_primeira_instrucao(lista) > 0);
}

int remover_primeira_instrucao(lista_t *lista)
{
	lista_no_t *d;

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

void imprimir_lista(lista_t *lista)
{
	lista_no_t *p;
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
