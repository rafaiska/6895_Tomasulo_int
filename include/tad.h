#ifndef TAD_H_
#define TAD_H_

#include <stdlib.h>
#include <stdio.h>
#include "instrucao.h"

typedef struct t_lista_no
{
	struct t_lista_no *anterior;
	struct t_instrucao *instrucao;
	struct t_lista_no *proximo;
}t_lista_no;

typedef struct t_lista
{
	struct t_lista_no *primeiro;
	struct t_lista_no *ultimo;
	int tamanho;
}t_lista;

t_lista *criar_lista();
int inserir_instrucao_lista(t_lista *lista, t_instrucao *instrucao);
int remover_primeira_instrucao(t_lista *lista);
void imprimir_lista(t_lista *lista);
void esvaziar_lista(t_lista *lista);

#endif
