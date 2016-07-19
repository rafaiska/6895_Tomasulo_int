#include "tomasulo.h"

int Atualiza_Clock()
{
	if(clock_state == 0)
		clock_state = 1;
	else
	{
		clock_state = 0;
		++clock_count;
		//printf("CICLO DE CLOCK: %d\n", clock_count);
	}
}

void Atualizar_Busca()
{
	int i, j;

	if(busca_instrucao_stall != 0 || fila_emissao_empty == 0)
		return;

	j = (tomasulo_registradores->pc)/4;
	for(i=0; i< n_busca_ciclo; ++i)
	{
		tomasulo_busca[i] = tomasulo_memoria->heap[j];
		++busca_instrucao_ready;
		printf("Buscando instrucao: 0x%x na posicao 0x%x\n", tomasulo_busca[i], tomasulo_registradores->pc); 	
		tomasulo_registradores->pc += 4;
		if(tomasulo_registradores->pc > tomasulo_memoria->text_end)
		{
			busca_instrucao_stall = 1;
			break;
		}
		++j;
	}
}

void Atualizar_Decodificacao()
{
	int i;

	if(fila_emissao_empty != 0 && busca_instrucao_ready != 0)
	{
		fila_emissao_first = 0;

		for(i=0; i<busca_instrucao_ready; ++i)
		{
			printf("Decodificando instrucao 0x%x\n", tomasulo_busca[i]);
			Decodificar_Instrucao(tomasulo_busca[i], tomasulo_decodificacao +i);
		}

		fila_emissao_last = i-1;

		fila_emissao_empty = 0;
		busca_instrucao_ready = 0;
	}
	if(fila_emissao_empty == 0)
	{
		while(fila_emissao_first <= fila_emissao_last)
		{
			switch(tomasulo_decodificacao[fila_emissao_first].tipo_instrucao)
			{
				//Instrucoes emitidas para BUFFER DE LOAD
				case LD_OPCODE:
					for(i=0; i<n_buff_load; ++i)
						if(tomasulo_buffer_load[i].estado == 0)
							break;
					if(i< n_buff_load)
					{
						printf("Emitindo instrucao 0x%x para buffer de load no. %d\n", tomasulo_decodificacao[fila_emissao_first].codificada, i+1);
						Emissao_Buffer(tomasulo_decodificacao +fila_emissao_first, tomasulo_buffer_load +i);
						++fila_emissao_first;
					}
				break;
				//Instrucoes emitidas para BUFFER DE STORE
				case ST_OPCODE:
					for(i=0; i<n_buff_store; ++i)
						if(tomasulo_buffer_store[i].estado == 0)
							break;
					if(i< n_buff_store)
					{
						printf("Emitindo instrucao 0x%x para buffer de store no. %d\n", tomasulo_decodificacao[fila_emissao_first].codificada, i+1);
						Emissao_Buffer(tomasulo_decodificacao +fila_emissao_first, tomasulo_buffer_load +i);
						++fila_emissao_first;
					}
				break;
				//Instrucoes emitidas para ESTACAO DE RESERVA DE ADICAO
				case B_OPCODE:
				case ADD_OPCODE:
				case SUB_OPCODE:
				case AND_OPCODE:
				case OR_OPCODE:
				case LI_OPCODE:
				case ADDI_OPCODE:
				case SUBI_OPCODE:
				case ANDI_OPCODE:
				case ORI_OPCODE:
				case BEQ_OPCODE:
				case BNE_OPCODE:
				case BGT_OPCODE:
				case BGE_OPCODE:
				case BLT_OPCODE:
				case BLE_OPCODE:
				case BEQZ_OPCODE:
				case BNEZ_OPCODE:
				case BGTZ_OPCODE:
				case BLEZ_OPCODE:
				case MOVE_OPCODE:
				case NEG_OPCODE:
				case NOT_OPCODE:
					for(i=0; i<n_uf_soma; ++i)
						if(tomasulo_er[i].b == 0)
							break;
					if(i< n_uf_soma)
					{
						printf("Emitindo instrucao 0x%x para estacao de reserva somadora no. %d\n", tomasulo_decodificacao[fila_emissao_first].codificada, i+1);
						Emissao_ER(tomasulo_decodificacao +fila_emissao_first, tomasulo_er +i);
						++fila_emissao_first;
					}
				break;
				//Instrucoes emitidas para ESTACAO DE RESERVA DE MULT
				case MULT_OPCODE:
				case MULTI_OPCODE:
				case SLR_OPCODE:
					for(i=0; i<n_uf_mult; ++i)
						if(tomasulo_er[n_uf_soma +i].b == 0)
							break;
					if(i< n_uf_mult)
					{
						printf("Emitindo instrucao 0x%x para estacao de reserva multiplicadora no. %d\n", tomasulo_decodificacao[fila_emissao_first].codificada, i+1);
						Emissao_ER(tomasulo_decodificacao +fila_emissao_first, tomasulo_er +n_uf_soma +i);
						++fila_emissao_first;
					}
				break;
				//Instrucoes emitidas para ESTACAO DE RESERVA DE DIVI
				case DIV_OPCODE:
				case DIVI_OPCODE:
				case SLL_OPCODE:
					for(i=0; i<n_uf_divi; ++i)
						if(tomasulo_er[n_uf_soma +n_uf_mult +i].b == 0)
							break;
					if(i< n_uf_divi)
					{
						printf("Emitindo instrucao 0x%x para estacao de reserva divisora no. %d\n", tomasulo_decodificacao[fila_emissao_first].codificada, i+1);
						Emissao_ER(tomasulo_decodificacao +fila_emissao_first, tomasulo_er +n_uf_soma +n_uf_mult +i);
						++fila_emissao_first;
					}
				break;
			}
		}
		if(fila_emissao_first > fila_emissao_last)
		{
			fila_emissao_empty = 1;
		}
	}
}

int Atualiza_Componentes()
{
	//TODO: Atualiza banco de registradores
	//TODO: Atualiza memoria
	//TODO: Atualiza unidades funcionais
	//TODO: Atualiza buffers de LOAD/STORE
	//TODO: Atualiza estacoes de reserva
	Atualizar_Decodificacao();
	Atualizar_Busca();
}

int Configurar_Tomasulo()
{
	uint32_t tamanho_mem = 1024;
	uint8_t largura_cdb = 32;
	char buffer_leitura[512];
	char c;
	int i;

	//CONFIGURANDO CLOCK
	clock_state = 0;
	clock_count = 0;

	//NULLIFICANDO PONTEIROS INICIAIS
	tomasulo_memoria = NULL;
	tomasulo_registradores = NULL;
	tomasulo_er = NULL;
	tomasulo_uf = NULL;
	tomasulo_buffer_load = NULL;
	tomasulo_buffer_store = NULL;
	tomasulo_cdb = NULL;

	//VARIAVEL PARA CONTROLAR O FIM DO ALGORITMO
	tomasulo_exit = 0;

	//NUMERO PADRAO DE COMPONENTES
	n_uf_soma = 2;
	n_uf_mult = 2;
	n_uf_divi = 2;
	n_buff_load = 2;
	n_buff_store = 2;
	n_busca_ciclo = 4;

	//TEMPO PADRAO DE EXECUCAO (EM CICLOS) PARA CADA INSTRUCAO
	XTIME_LD = 8;
	XTIME_ST = 8;
	XTIME_MOVE = 1;
	XTIME_NEG = 2;
	XTIME_NOT = 2;
	XTIME_ADD = 2;
	XTIME_SUB = 2;
	XTIME_MULT = 4;
	XTIME_DIV = 4;
	XTIME_AND = 2;
	XTIME_OR = 2;
	XTIME_SLL = 1;
	XTIME_SLR = 1;
	XTIME_LI = 1;
	XTIME_BEQZ = 2;
	XTIME_BNEZ = 2;
	XTIME_BGTZ = 2;
	XTIME_BLEZ = 2;
	XTIME_ADDI = 2;
	XTIME_SUBI = 2;
	XTIME_MULTI = 4;
	XTIME_DIVI = 4;
	XTIME_ANDI = 2;
	XTIME_ORI = 2;
	XTIME_BEQ = 2;
	XTIME_BNE = 2;
	XTIME_BGT = 2;
	XTIME_BGE = 2;
	XTIME_BLT = 2;
	XTIME_BLE = 2;
	XTIME_B = 1;

	gets(buffer_leitura);
	if(strcmp("ARQUITETURA", buffer_leitura) != 0)
	{
		printf("ERRO: Arquivo de entrada mal formatado: cabecalho ARQUITETURA nao encontrado\n");
		exit(1);
	}

	gets(buffer_leitura);
	while(strcmp("CICLOS", buffer_leitura) != 0)
	{
		if(Definir_Arquitetura(buffer_leitura) != 0)
			return 1;
		gets(buffer_leitura);
	}

	gets(buffer_leitura);
	while(strcmp("TEXTO", buffer_leitura) != 0)
	{
		if(Definir_Arquitetura(buffer_leitura) != 0)
			return 1;
		gets(buffer_leitura);
	}

	tomasulo_memoria = Inicializar_Memoria(tamanho_mem);

	printf("========================\nINICIALIZANDO SIMULADOR!\n========================\n");
	printf("\tNumero de UF somadoras: %d\n", n_uf_soma);	
	printf("\tNumero de UF multiplicadoras: %d\n", n_uf_mult);
	printf("\tNumero de UF divisoras: %d\n", n_uf_divi);
	printf("\tNumero de instrucoes buscadas por ciclo: %d\n", n_busca_ciclo);
	printf("\tNumero de buffers de carga: %d\n", n_buff_load);
	printf("\tNumero de buffers de escrita: %d\n", n_buff_store);
	printf("\tTamanho da memoria virtual: %d kB\n\n", (tamanho_mem*32)/1024);

	//ALOCANDO COMPONENTES PRINCIPAIS
	n_uf_total = n_uf_soma + n_uf_mult + n_uf_divi;
	tomasulo_registradores = malloc(sizeof(b_registrador_t));
	tomasulo_er = malloc(sizeof(estacao_reserva_t)*(n_uf_total));
	tomasulo_uf = malloc(sizeof(unidade_funcional_t)*(n_uf_total));
	tomasulo_buffer_load = malloc(sizeof(buffer_t)*(n_buff_load));
	tomasulo_buffer_store = malloc(sizeof(buffer_t)*(n_buff_store));
	tomasulo_cdb = Inicializar_CDB(largura_cdb);
	tomasulo_busca = malloc(sizeof(uint32_t)*n_busca_ciclo);
	tomasulo_decodificacao = malloc(sizeof(instrucao_t)*n_busca_ciclo);

	//ZERANDO Qi DO BANCO DE REGISTRADORES
	for(i=0; i< MAX_REGISTERS; ++i)
		tomasulo_registradores->qi[i] = 0;

	//DEFININDO ID DAS UNIDADES FUNCIONAIS E DOS BUFFERS
	for(i=0; i< n_uf_total; ++i)
		tomasulo_er[i].id = MAX_REGISTERS +1 +i;

	for(i=0; i< n_buff_load; ++i)
		tomasulo_buffer_load[i].id = MAX_REGISTERS +n_uf_total +1 +i;

	for(i=0; i< n_buff_store; ++i)
		tomasulo_buffer_store[i].id = MAX_REGISTERS +n_uf_total +n_buff_load +1 +i;

	//DEFININDO VALOR INICIAL DO PROGRAM COUNTER
	tomasulo_registradores->pc = tomasulo_memoria->text_start;

	//LIBERANDO STALL DA BUSCA, BUSCA AINDA NAO REALIZADA
	busca_instrucao_stall = 0;
	busca_instrucao_ready = 0;

	//FILA DE EMISSAO INICIALMENTE VAZIA
	fila_emissao_empty = 1;

	return 0;
}

void Encerrar_Tomasulo()
{
	Liberar_Memoria(&tomasulo_memoria);
	free(tomasulo_registradores);
	free(tomasulo_er);
	free(tomasulo_uf);
	free(tomasulo_buffer_load);
	free(tomasulo_buffer_store);
	Liberar_CDB(&tomasulo_cdb);
	free(tomasulo_busca);			//Vetor de instrucoes buscadas
	free(tomasulo_decodificacao);		//Vetor de instrucoes decodificadas
}

memoria_t *Inicializar_Memoria(uint32_t tamanho)
{
	memoria_t *retorno;
	char programa[tamanho*32]; //String para guardar o programa
	char c;
	int i=0;

	retorno = malloc(sizeof(memoria_t));
	retorno->tamanho = tamanho;
	retorno->heap = malloc(sizeof(uint32_t)*tamanho);

	while((c = getchar()) != EOF)
	{
		programa[i] = c;
		++i;
	}
	programa[i] = '\0';

	retorno->text_start = Montar_Codigo(programa, retorno->heap, retorno->tamanho, &(retorno->text_end));

	return retorno;	
}

void Liberar_Memoria(memoria_t **memoria)
{
	memoria_t *desaloc;
	desaloc = *memoria;

	free(desaloc->heap);
	free(desaloc);
}

cdb_t *Inicializar_CDB(uint8_t tamanho_barramento)
{
	cdb_t *retorno;

	retorno = malloc(sizeof(cdb_t));

	retorno->largura_barramento = tamanho_barramento;
	retorno->barramentos_em_uso = 0;
	retorno->sinais = malloc(sizeof(sinal_t)*tamanho_barramento);
	retorno->dados = malloc(sizeof(uint32_t)*tamanho_barramento);

	return retorno;
}

void Liberar_CDB(cdb_t **cdb)
{
	cdb_t *desaloc = *cdb;

	free(desaloc->sinais);
	free(desaloc->dados);

	free(desaloc);
}

int Definir_Arquitetura(char *linha)
{
	char componente[64];
	char quantidade[32];
	uint8_t quantidade_i;
	int i, j;

	i=0;
	j=0;
	while(linha[i] == '\t' || linha[i] == ' ')
		++i;
	while(linha[i] != '\t' && linha[i] != ' ')
	{
		componente[j] = linha[i];
		++j;
		++i;
	}
	componente[j] = '\0';
	
	while(linha[i] == '\t' || linha[i] == ' ')
		++i;

	if(linha[i] != '=')
		return 1;
	++i;
	while(linha[i] == '\t' || linha[i] == ' ')
		++i;

	j=0;
	while(linha[i] != '\t' && linha[i] != ' ' && linha[i] != '\0')
	{
		quantidade[j] = linha[i];
		++j;
		++i;
	}
	quantidade[j] = '\0';
	quantidade_i = atoi(quantidade);

	if(strcmp("somador", componente) == 0)
		n_uf_soma = quantidade_i;
	else if(strcmp("multiplicador", componente) == 0)
		n_uf_mult = quantidade_i;
	else if(strcmp("divisor", componente) == 0)
		n_uf_divi = quantidade_i;
	else if(strcmp("busca_de_instrucoes", componente) == 0)
		n_busca_ciclo = quantidade_i;
	else if(strcmp("buffer_de_carga", componente) == 0)
		n_buff_load = quantidade_i;
	else if(strcmp("buffer_de_escrita", componente) == 0)
		n_buff_store = quantidade_i;
	else if(strcmp("ld", componente) == 0)
		XTIME_LD = quantidade_i;
	else if(strcmp("st", componente) == 0)
		XTIME_ST = quantidade_i;
	else if(strcmp("li", componente) == 0)
		XTIME_LI = quantidade_i;
	else if(strcmp("move", componente) == 0)
		XTIME_MOVE = quantidade_i;
	else if(strcmp("beq", componente) == 0)
		XTIME_BEQ = quantidade_i;
	else if(strcmp("beqz", componente) == 0)
		XTIME_BEQZ = quantidade_i;
	else if(strcmp("bne", componente) == 0)
		XTIME_BNE = quantidade_i;
	else if(strcmp("bnez", componente) == 0)
		XTIME_BNEZ = quantidade_i;
	else if(strcmp("bgt", componente) == 0)
		XTIME_BGT = quantidade_i;
	else if(strcmp("bge", componente) == 0)
		XTIME_BGE = quantidade_i;
	else if(strcmp("bgtz", componente) == 0)
		XTIME_BGTZ = quantidade_i;
	else if(strcmp("blt", componente) == 0)
		XTIME_BLT = quantidade_i;
	else if(strcmp("ble", componente) == 0)
		XTIME_BLE = quantidade_i;
	else if(strcmp("blez", componente) == 0)
		XTIME_BLEZ = quantidade_i;
	else if(strcmp("b", componente) == 0)
		XTIME_B = quantidade_i;
	else if(strcmp("add", componente) == 0)
		XTIME_ADD = quantidade_i;
	else if(strcmp("addi", componente) == 0)
		XTIME_ADDI = quantidade_i;
	else if(strcmp("sub", componente) == 0)
		XTIME_SUB = quantidade_i;
	else if(strcmp("subi", componente) == 0)
		XTIME_SUBI = quantidade_i;
	else if(strcmp("mult", componente) == 0)
		XTIME_MULT = quantidade_i;
	else if(strcmp("multi", componente) == 0)
		XTIME_MULTI = quantidade_i;
	else if(strcmp("div", componente) == 0)
		XTIME_DIV = quantidade_i;
	else if(strcmp("divi", componente) == 0)
		XTIME_DIVI = quantidade_i;
	else if(strcmp("and", componente) == 0)
		XTIME_AND = quantidade_i;
	else if(strcmp("andi", componente) == 0)
		XTIME_ANDI = quantidade_i;
	else if(strcmp("or", componente) == 0)
		XTIME_OR = quantidade_i;
	else if(strcmp("ori", componente) == 0)
		XTIME_ORI = quantidade_i;
	else if(strcmp("neg", componente) == 0)
		XTIME_NEG = quantidade_i;
	else if(strcmp("not", componente) == 0)
		XTIME_NOT = quantidade_i;
	else if(strcmp("sll", componente) == 0)
		XTIME_SLL = quantidade_i;
	else if(strcmp("slr", componente) == 0)
		XTIME_SLR = quantidade_i;
	else
		return 1;

	return 0;
}

uint8_t Emissao_Buffer(instrucao_t *instrucao, buffer_t *buffer)
{
	buffer->tipo = instrucao->tipo_instrucao;
	switch(buffer->tipo)
	{
		case LD_OPCODE:
			//Verifica se o registrador com o endereco esta pronto	
			buffer->q2 = tomasulo_registradores->qi[instrucao->operando2];
			if(buffer->q2 == 0)
				buffer->v2 = tomasulo_registradores->registrador[instrucao->operando2];

			//Essa unidade produzira valor para o registrador destino
			tomasulo_registradores->qi[instrucao->operando1] = buffer->id;
			break;
				
		case ST_OPCODE:
			//Verifica se o registrador com o endereco esta pronto	
			buffer->q2 = tomasulo_registradores->qi[instrucao->operando2];
			if(buffer->q2 == 0)
				buffer->v2 = tomasulo_registradores->registrador[instrucao->operando2];

			//Verifica se o registrador com o valor a ser armazenado esta pronto
			buffer->q1 = tomasulo_registradores->qi[instrucao->operando1];
			if(buffer->q1 == 0)
				buffer->v1 = tomasulo_registradores->registrador[instrucao->operando1];
			break;
	}
	buffer->estado = 1; //Calculando endereco
	return 0;
}

uint8_t Emissao_ER(instrucao_t *instrucao, estacao_reserva_t *er)
{
	er->tipo = instrucao->tipo_instrucao;
	switch(er->tipo)
	{
		//INSTRUCOES DE SALTO op R, I/L 
		case BEQZ_OPCODE:
		case BNEZ_OPCODE:
		case BGTZ_OPCODE:
		case BLEZ_OPCODE:
			//Entra em stall ateh decidir se o salto foi tomado ou nao
			busca_instrucao_stall = 1;
	
			er->a = instrucao->imediato;
	
			//Verifica se o primeiro operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q1 = tomasulo_registradores->qi[instrucao->operando1];
			if(er->q1 == 0)
				er->v1 = tomasulo_registradores->registrador[instrucao->operando1];
		break;
		//INSTRUCOES DE SALTO op R, R, I/L
		case BEQ_OPCODE:
		case BNE_OPCODE:
		case BGT_OPCODE:
		case BGE_OPCODE:
		case BLT_OPCODE:
		case BLE_OPCODE:
			//Entra em stall ateh decidir se o salto foi tomado ou nao
			busca_instrucao_stall = 1;

			er->a = instrucao->imediato;
			
			//Verifica se o primeiro operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q1 = tomasulo_registradores->qi[instrucao->operando1];
			if(er->q1 == 0)
				er->v1 = tomasulo_registradores->registrador[instrucao->operando1];

			//Verifica se o segundo operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q2 = tomasulo_registradores->qi[instrucao->operando2];
			if(er->q2 == 0)
				er->v2 = tomasulo_registradores->registrador[instrucao->operando2];
		break;
		//INSTRUCOES op R, I/L
		case LI_OPCODE:
			er->q1 = 0;
			er->v1 = instrucao->imediato;	
			//Essa unidade produzira valor para o registrador destino
			tomasulo_registradores->qi[instrucao->operando1] = er->id;
		break;
		//INSTRUCOES op R, R, I/L
		case ADDI_OPCODE:
		case SUBI_OPCODE:
		case MULTI_OPCODE:
		case DIVI_OPCODE:
		case ORI_OPCODE:
		case ANDI_OPCODE:
			//Verifica se o segundo operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q1 = tomasulo_registradores->qi[instrucao->operando2];
			if(er->q1 == 0)
				er->v1 = tomasulo_registradores->registrador[instrucao->operando2];

			er->q2 = 0;
			er->v2 = instrucao->imediato;	

			//Essa unidade produzira valor para o registrador destino
			tomasulo_registradores->qi[instrucao->operando1] = er->id;
		break;
		//INSTRUCOES op R, R, R
		case ADD_OPCODE:
		case SUB_OPCODE:
		case AND_OPCODE:
		case OR_OPCODE:
		case MULT_OPCODE:
		case DIV_OPCODE:
		case SLL_OPCODE:
		case SLR_OPCODE:
			//Verifica se o segundo operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q1 = tomasulo_registradores->qi[instrucao->operando2];
			if(er->q1 == 0)
				er->v1 = tomasulo_registradores->registrador[instrucao->operando2];

			//Verifica se o terceiro operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q2 = tomasulo_registradores->qi[instrucao->operando3];
			if(er->q2 == 0)
				er->v2 = tomasulo_registradores->registrador[instrucao->operando3];

			//Essa unidade produzira valor para o registrador destino
			tomasulo_registradores->qi[instrucao->operando1] = er->id;
		break;
		//INSTRUCOES op R, R
		case MOVE_OPCODE:
		case NEG_OPCODE:
		case NOT_OPCODE:
			//Verifica se o segundo operando ja esta pronto. Senao, guarda a ER em que ele sera produzido 
			er->q1 = tomasulo_registradores->qi[instrucao->operando2];
			if(er->q1 == 0)
				er->v1 = tomasulo_registradores->registrador[instrucao->operando2];

			//Essa unidade produzira valor para o registrador destino
			tomasulo_registradores->qi[instrucao->operando1] = er->id;
			break;
	}
	er->b = 1;
	return 0;
}
