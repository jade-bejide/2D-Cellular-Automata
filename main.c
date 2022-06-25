
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include <string.h>

#include "automata.h"


int main(int n, char** argv) {
    setbuf(stdout, NULL);
    clearScreen();
    automaton **automata = createAutomata();
    establishNeighbours(automata);
    int start = population(automata);
    
    updateAutomata(automata);
    int end = population(automata);

    printf("Population At Start: %d\n", start);
    printf("Population At End: %d\n", end);
    freeAutomata(automata);

    return 0;
}