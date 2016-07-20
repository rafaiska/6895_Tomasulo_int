#include "instrucao.h"

int Decodificar_Instrucao(uint32_t codigo, instrucao_t *destino)
{
	uint32_t opcode;
	uint32_t op1;
	uint32_t op2;
	uint32_t op3;
	uint32_t operandos;
	int32_t imediato;
	int i;

	destino->codificada = codigo;

	//FETCH OPCODE
	opcode = codigo;
	for(i=0; i<26; ++i) //shift right 26 vezes
		opcode /= 2;

	destino->tipo_instrucao = opcode;

	operandos = opcode;
	for(i=0;i<26;++i)
		operandos *= 2; //volta os shift right com shift left

	operandos = codigo - operandos; //exclui o opcode da instrucao, ficando so com os operandos

		switch(opcode)
		{
			//FORMATO op L
			case B_OPCODE:
				destino->imediato = operandos;
			break;

			//FORMATO op R, R, I ; FORMATO op R, R, L
			case ADDI_OPCODE:
			case SUBI_OPCODE:
			case MULTI_OPCODE:
			case DIVI_OPCODE:
			case ANDI_OPCODE:
			case ORI_OPCODE:
			case BEQ_OPCODE:
			case BNE_OPCODE:
			case BGT_OPCODE:
			case BGE_OPCODE:
			case BLT_OPCODE:
			case BLE_OPCODE:
				destino->operando1 = operandos;
				for(i=0; i<21; ++i) //shift right 21 vezes
					destino->operando1 /= 2;
				destino->operando2 = operandos & 0x1f0000;
				for(i=0; i<16; ++i) //shift right 16 vezes
					destino->operando2 /= 2;
				destino->imediato = operandos & 0xffff;
			break;

			//FORMATO op R, I ; FORMATO op R, L
			case LI_OPCODE:
			case BEQZ_OPCODE:
			case BNEZ_OPCODE:
			case BGTZ_OPCODE:
			case BLEZ_OPCODE:
				destino->operando1 = operandos;
				for(i=0; i<21; ++i) //shift right 21 vezes
					destino->operando1 /= 2;
				destino->imediato = operandos & 0x1fffff;
			break;

			//FORMATO op R, R
			case LD_OPCODE:
			case ST_OPCODE:
			case MOVE_OPCODE:
			case NEG_OPCODE:
			case NOT_OPCODE:
				destino->operando1 = operandos;
				for(i=0; i<21; ++i) //shift right 21 vezes
					destino->operando1 /= 2;
				destino->operando2 = operandos & 0x1f0000;
				for(i=0; i<16; ++i) //shift right 16 vezes
					destino->operando2 /= 2;
			break;

			//FORMATO op R, R, R
			case ADD_OPCODE:
			case SUB_OPCODE:
			case MULT_OPCODE:
			case DIV_OPCODE:
			case AND_OPCODE:
			case OR_OPCODE:
			case SLL_OPCODE:
			case SLR_OPCODE:
				destino->operando1 = operandos;
				for(i=0; i<21; ++i) //shift right 21 vezes
					destino->operando1 /= 2;
				destino->operando2 = operandos & 0x1f0000;
				for(i=0; i<16; ++i) //shift right 16 vezes
					destino->operando2 /= 2;
				destino->operando3 = operandos & 0xf800;
				for(i=0; i<11; ++i) //shift right 11 vezes
					destino->operando3 /= 2;
			break;
		}


	return 0;
}

uint32_t Codificar_Instrucao(char *instrucao, lista_rotulo_t *lista)
{
	int i = 0;
	int j = 0;
	int k;
	char aux[32];
	uint32_t retorno;
	uint32_t opcode;
	uint32_t op1;
	uint32_t op2;
	uint32_t op3;

	if(instrucao[0] == '.' && instrucao[1] == 'i')
	{
		while(instrucao[i] != ' ')
			++i;
		++i;
		while(instrucao[i] != '\0')
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
		while(instrucao[i] != ' ')
		{
			aux[j] = instrucao[i];
			++i;
			++j;
		}
		aux[j] = '\0';

		if(strcmp("ld\0", aux) == 0)
			opcode = LD_OPCODE;
		else if(strcmp("st\0", aux) == 0)
			opcode = ST_OPCODE;
		else if(strcmp("li\0", aux) == 0)
			opcode = LI_OPCODE;
		else if(strcmp("move\0", aux) == 0)
			opcode = MOVE_OPCODE;
		else if(strcmp("beq\0", aux) == 0)
			opcode = BEQ_OPCODE;
		else if(strcmp("beqz\0", aux) == 0)
			opcode = BEQZ_OPCODE;
		else if(strcmp("bne\0", aux) == 0)
			opcode = BNE_OPCODE;
		else if(strcmp("bnez\0", aux) == 0)
			opcode = BNEZ_OPCODE;
		else if(strcmp("bgt\0", aux) == 0)
			opcode = BGT_OPCODE;
		else if(strcmp("bge\0", aux) == 0)
			opcode = BGE_OPCODE;
		else if(strcmp("bgtz\0", aux) == 0)
			opcode = BGTZ_OPCODE;
		else if(strcmp("blt\0", aux) == 0)
			opcode = BLT_OPCODE;
		else if(strcmp("ble\0", aux) == 0)
			opcode = BLE_OPCODE;
		else if(strcmp("blez\0", aux) == 0)
			opcode = BLEZ_OPCODE;
		else if(strcmp("b\0", aux) == 0)
			opcode = B_OPCODE;
		else if(strcmp("add\0", aux) == 0)
			opcode = ADD_OPCODE;
		else if(strcmp("addi\0", aux) == 0)
			opcode = ADDI_OPCODE;
		else if(strcmp("sub\0", aux) == 0)
			opcode = SUB_OPCODE;
		else if(strcmp("subi\0", aux) == 0)
			opcode = SUBI_OPCODE;
		else if(strcmp("mult\0", aux) == 0)
			opcode = MULT_OPCODE;
		else if(strcmp("multi\0", aux) == 0)
			opcode = MULTI_OPCODE;
		else if(strcmp("div\0", aux) == 0)
			opcode = DIV_OPCODE;
		else if(strcmp("divi\0", aux) == 0)
			opcode = DIVI_OPCODE;
		else if(strcmp("and\0", aux) == 0)
			opcode = AND_OPCODE;
		else if(strcmp("andi\0", aux) == 0)
			opcode = ANDI_OPCODE;
		else if(strcmp("or\0", aux) == 0)
			opcode = OR_OPCODE;
		else if(strcmp("ori\0", aux) == 0)
			opcode = ORI_OPCODE;
		else if(strcmp("neg\0", aux) == 0)
			opcode = NEG_OPCODE;
		else if(strcmp("not\0", aux) == 0)
			opcode = NOT_OPCODE;
		else if(strcmp("sll\0", aux) == 0)
			opcode = SLL_OPCODE;
		else if(strcmp("slr\0", aux) == 0)
			opcode = SLR_OPCODE;
		else
		{
			printf("ERRO DE SINTAXE: operacao invalida em: %s\n", instrucao);
			exit(1);
		}

		retorno = opcode;
		for(k=0; k<26; ++k)
			retorno *= 2; //shift left 26 vezes

		while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;
		j=0;

		switch(opcode)
		{
			//FORMATO op L
			case B_OPCODE:
				while(instrucao[i] != '\0')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';

				op1 = Buscar_Rotulo(lista, aux);
				retorno = retorno | op1;
			break;

			//FORMATO op R, R, L/I
			case ADDI_OPCODE:
			case SUBI_OPCODE:
			case MULTI_OPCODE:
			case DIVI_OPCODE:
			case ANDI_OPCODE:
			case ORI_OPCODE:
			case BEQ_OPCODE:
			case BNE_OPCODE:
			case BGT_OPCODE:
			case BGE_OPCODE:
			case BLT_OPCODE:
			case BLE_OPCODE:
				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no primeiro operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op1 = atoi(aux);
				++i;

				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no segundo operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op2 = atoi(aux);
				++i;
				
				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				while(instrucao[i] != '\0')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';

				if(aux[0] >= 48 && aux[0] <= 57)		//op3 eh um imediato
					op3 = atoi(aux);
				else						//op3 eh um label
					op3 = Buscar_Rotulo(lista, aux);
					

				for(k=0; k<21; ++k)
					op1 *= 2;

				for(k=0; k<16; ++k)
					op2 *= 2;

				retorno = retorno | op1 | op2 | op3;
			break;

			//FORMATO op R, I/L
			case LI_OPCODE:
			case BEQZ_OPCODE:
			case BNEZ_OPCODE:
			case BGTZ_OPCODE:
			case BLEZ_OPCODE:
				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no primeiro operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op1 = atoi(aux);
				++i;
				
				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				while(instrucao[i] != '\0')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';

				if(aux[0] >= 48 && aux[0] <= 57)		//op2 eh um imediato
					op2 = atoi(aux);
				else						//op2 eh um label
					op2 = Buscar_Rotulo(lista, aux);

				for(k=0; k<21; ++k)
					op1 *= 2;

				retorno = retorno | op1 | op2;
			break;

			//FORMATO op R, R
			case LD_OPCODE:
			case ST_OPCODE:
			case MOVE_OPCODE:
			case NEG_OPCODE:
			case NOT_OPCODE:
				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no primeiro operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op1 = atoi(aux);
				++i;
				
				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no segundo operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != '\0')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op2 = atoi(aux);

				for(k=0; k<21; ++k)
					op1 *= 2;

				for(k=0; k<16; ++k)
					op2 *= 2;

				retorno = retorno | op1 | op2;

			break;

			//FORMATO op R, R, R
			case ADD_OPCODE:
			case SUB_OPCODE:
			case MULT_OPCODE:
			case DIV_OPCODE:
			case AND_OPCODE:
			case OR_OPCODE:
			case SLL_OPCODE:
			case SLR_OPCODE:
				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no primeiro operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op1 = atoi(aux);
				++i;
				
				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no segundo operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != ',')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op2 = atoi(aux);
				++i;

				j=0;
				while(instrucao[i] == ' ' || instrucao[i] == '\t') ++i;

				if(instrucao[i] != 'r')
				{
					printf("ERRO DE SINTAXE no terceiro operando de: %s\n", instrucao);
					exit(1);
				}
				++i;
				while(instrucao[i] != '\0')
				{
					aux[j] = instrucao[i];
					++j;
					++i;
				}
				aux[j] = '\0';
				op3 = atoi(aux);

				for(k=0; k<21; ++k)
					op1 *= 2;

				for(k=0; k<16; ++k)
					op2 *= 2;

				for(k=0; k<11; ++k)
					op3 *= 2;

				retorno = retorno | op1 | op2 | op3;
			break;
		}
	}

	return retorno;
}

uint32_t Montar_Codigo(char *codigo, uint32_t *heap, uint32_t tamanho_memoria, uint32_t *text_end)
{
	int i, j;
	char instrucao[128];
	lista_rotulo_t *lista = NULL;
	lista_rotulo_t *lista_aux; //usado para liberar memoria
	uint32_t posicao = 0;
	uint32_t text_start;

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
			if(strcmp(".text\0", instrucao) == 0)
				text_start = posicao;
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
	else
	{
		*text_end = posicao -4;
	}

	//FASE2: Codificar o programa

	i=0;
	j=0;
	posicao = 0;
	while(codigo[i] != '\0')
	{
		if(codigo[i] == ':')
		{
			j=0;
			++i;		
			while(codigo[i] == ' ' || codigo[i] == '\t' || codigo[i] == '\n') ++i;
		}
		else if(codigo[i] == '\n')
		{
			instrucao[j] = '\0';
			if(strlen(instrucao) > 0)
			{
				heap[posicao] = Codificar_Instrucao(instrucao, lista);
				++posicao;
			}
			j=0;
			++i;
			while(codigo[i] == ' ' || codigo[i] == '\t') ++i;
		}
		else if(codigo[i] == '.')
		{
			while(codigo[i] != ' ' && codigo[i] != '\n')
			{
				instrucao[j] = codigo[i];
				++i;
				++j;
			}
			instrucao[j] = '\0';

			if(strcmp(".data\0", instrucao) == 0 || strcmp(".text\0", instrucao) == 0)
			{
				while(codigo[i] == ' ' || codigo[i] == '\n' || codigo[i] == '\t')
					++i;
			}
			else
			{
				while(codigo[i] != '\n')
				{
					instrucao[j] = codigo[i];
					++i;
					++j;
				}
				instrucao[j] = '\0';
				heap[posicao] = Codificar_Instrucao(instrucao, lista);
				++posicao;
			}
			j=0;
		}
		else
		{
			instrucao[j] = codigo[i];
			++j;
			++i;
		}
	}

	//Para depuracao da memoria
	//for(i=0; i<20; ++i)
	//	printf("0x%x\n", heap[i]);
	//printf("Texto comecando em: 0x%x, terminando em 0x%x\n", text_start, *text_end);

	//Liberar memoria da lista de rotulos
	while(lista != NULL)
	{
		lista_aux = lista;
		lista = lista->prox;
		free(lista_aux);
	}

	return text_start;
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

uint32_t Buscar_Rotulo(lista_rotulo_t *lista, char *rotulo)
{
	while(lista != NULL && strcmp(rotulo, lista->rotulo) != 0)
		lista = lista->prox;

	if(lista == NULL)
		return 0;
	else
		return lista->endereco;
}

uint32_t Calcular_Tamanho_Instrucao(char *instrucao)
{
	int i = 0;
	int j = 0;
	char aux[32];
	uint32_t retorno;

	if(strlen(instrucao) == 0)
		return 0;

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
