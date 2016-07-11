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
	uint32_t tamanho_mem = 1024;
	uint8_t largura_cdb = 32;
	char buffer_leitura[512];
	char c;
	int i;

	clock_state = 0;
	clock_count = 0;

	tomasulo_memoria = NULL;
	tomasulo_registradores = NULL;
	tomasulo_er = NULL;
	tomasulo_uf = NULL;
	tomasulo_buffer_load = NULL;
	tomasulo_buffer_store = NULL;
	tomasulo_cdb = NULL;

	tomasulo_exit = 0;
	n_uf_soma = 2;
	n_uf_mult = 2;
	n_uf_divi = 2;
	n_buff_load = 2;
	n_buff_store = 2;
	n_busca_ciclo = 4;

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

	printf("INICIALIZANDO SIMULADOR!\n");
	printf("\tNumero de UF somadoras: %d\n", n_uf_soma);	
	printf("\tNumero de UF multiplicadoras: %d\n", n_uf_mult);
	printf("\tNumero de UF divisoras: %d\n", n_uf_divi);
	printf("\tNumero de instrucoes buscadas por ciclo: %d\n", n_busca_ciclo);
	printf("\tNumero de buffers de carga: %d\n", n_buff_load);
	printf("\tNumero de buffers de escrita: %d\n", n_buff_store);
	printf("\tTamanho da memoria virtual: %d kB\n", (tamanho_mem*32)/1024);

	tomasulo_registradores = malloc(sizeof(b_registrador_t));
	tomasulo_er = malloc(sizeof(estacao_reserva_t)*(n_uf_soma+n_uf_mult+n_uf_divi));
	tomasulo_uf = malloc(sizeof(unidade_funcional_t)*(n_uf_soma+n_uf_mult+n_uf_divi));
	tomasulo_buffer_load = malloc(sizeof(buffer_t)*(n_buff_load));
	tomasulo_buffer_store = malloc(sizeof(buffer_t)*(n_buff_store));
	tomasulo_cdb = Inicializar_CDB(largura_cdb);

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

	Montar_Codigo(programa, retorno->heap, retorno->tamanho);

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

