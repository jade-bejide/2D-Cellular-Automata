main: main.o automata.o
	gcc -Wall -g $^ -o $@

*.o: *.c automata.h
	gcc -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o automata.exe
