DIRINCLUDE=./include
DIROBJETO=./obj
CC=gcc
CFLAGS=-I$(DIRINCLUDE)

_DEPEND = tad.h instrucao.h estacao_reserva.h cdb.h tomasulo.h buffer.h unidade_funcional.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = tad.o main.o instrucao.o estacao_reserva.o cdb.o tomasulo.o buffer.o unidade_funcional.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.c $(DEPEND)
	$(CC) -c -o $@ $< $(CFLAGS)

tomasulo.out: $(OBJETO)
	$(CC) -o ./bin/$@ $^ $(CFLAGS)