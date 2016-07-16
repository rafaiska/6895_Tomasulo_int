#ifndef TAD_H_
#define TAD_H_

#include <stdlib.h>
#include <stdio.h>
#include "instrucao.h"

typedef struct lista_no_t
{
	struct t_lista_no *anterior;
	struct t_instrucao *instrucao;
	struct t_lista_no *proximo;
}lista_no_t;

typedef struct lista_t
{
	struct t_lista_no *primeiro;
	struct t_lista_no *ultimo;
	int tamanho;
}lista_t;

lista_t *criar_lista();
int inserir_instrucao_lista(lista_t *lista, instrucao_t *instrucao);
int remover_primeira_instrucao(lista_t *lista);
void imprimir_lista(lista_t *lista);
void esvaziar_lista(lista_t *lista);

#endif
