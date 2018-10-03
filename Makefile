TARGET = compiler
CC = gcc
CFLAGS = -g

compiler: bin/io.o bin/lexer.o bin/symbol.o bin/init.o bin/parser.o bin/emitter.o bin/error.o bin/main.o 
	$(CC) $(CFLAGS) -o $(TARGET) bin/*.o

bin/main.o: main.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/init.o: init.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/symbol.o: util/symbol.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/lexer.o: lexer/lexer.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/parser.o: parser/parser.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/emitter.o: generator/emitter.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/error.o: util/error.c
	$(CC) $(CFLAGS) -c $^ -o $@

bin/io.o: lexer/io.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f bin/*.o
	rm -f compiler

