compiler: lexer.o symbol.o init.o parser.o emitter.o error.o main.o
	gcc -g -o compiler *.o

main.o: main.c
	gcc -g -c main.c

init.o: init.c
	gcc -g -c init.c

symbol.o: symbol.c
	gcc -g -c symbol.c

lexer.o: lexer.c
	gcc -g -c lexer.c

parser.o: parser.c
	gcc -g -c parser.c

emitter.o: emitter.c
	gcc -g -c emitter.c

error.o: error.c
	gcc -g -c error.c

clean:
	rm -f *.o
	rm -f compiler

