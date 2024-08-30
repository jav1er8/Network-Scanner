#-O3 optimizacion
FLAGS=-O3 -Wall -std=c2x 
#-shared para librería compartida
LDFLAGS= -fPIC -shared -ldl -D_GNU_SOURCE  

all: clean utils.so

#Libreria ($^ source file)
utils.so: utils.o
	cc $(LDFLAGS)  -o $@ $^

utils.o: utils.c
	cc ${FLAGS} -c $^

clean:
	rm -f utils.o utils.so


