#-O3 optimizacion
FLAGS=-O3 -Wall -std=c2x
LDFLAGS=

all: clean netscan-basic

netscan-basic: netscan-basic.o
	cc $(FLAGS) $^ -o $@ ${LDFLAGS} 

netscan-basic.o: netscan-basic.c
	cc $(FLAGS) -c $^ 

clean:
	rm -f *.o netscan-basic