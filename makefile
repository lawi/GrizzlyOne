CC=clang
CFLAGS=-I.
DEPS =
OBJ = GrizzlyOne/PC/grizzlyone.o GrizzlyOne/PC/ui.o GrizzlyOne/Engine/chessglobal.o GrizzlyOne/Engine/chessutils.o GrizzlyOne/Engine/eval.o \
	GrizzlyOne/Engine/movegen.o GrizzlyOne/Engine/movemake.o GrizzlyOne/Engine/search.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

grizzly: $(OBJ) 
	$(CC) -g -o $@ $^ $(CFLAGS)



