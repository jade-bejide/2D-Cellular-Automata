#include "automata.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>


#define DIMENSION 40
#define TICKS 5

//defining an automaton
typedef struct automaton {
    int state;
    struct automaton *neighbours;
    int numOfNeighbours;
} automaton;

//create cellular automata, allocating memory
automaton *createAutomaton() {
    automaton *cell = malloc(sizeof(automaton));
    //1 - live, 0 - dead
    cell->state = rand() % 2;
    cell->neighbours = malloc(DIMENSION * sizeof(automaton));
    cell->numOfNeighbours = 0;

    return cell;
}

automaton **createAutomata() {  
    automaton **automata = malloc((DIMENSION * DIMENSION)*sizeof(automaton));

    srand(time(0));

    for (int i = 0; i < DIMENSION; i++) {
        automaton *automataN = malloc(DIMENSION * sizeof(automaton));
        for (int j = 0; j < DIMENSION; j++) automataN[j] = *createAutomaton();
        automata[i] = automataN;
    }
   
   return automata;
}

void establishNeighbours(automaton** automata) {
    for (int x = 0; x < DIMENSION; x++) {
        for (int y = 0; y < DIMENSION; y++) {
            //establish neighbours array
            int pos = 0;

            if (y-1 >= 0) {
                automata[x][y].neighbours[pos] = automata[x][y-1]; pos++;
            }
            if (y + 1 < DIMENSION) {
                automata[x][y].neighbours[pos] = automata[x][y+1]; pos++;
            }
            if (x - 1 >= 0) {
                automata[x][y].neighbours[pos] = automata[x-1][y]; pos++;
                if (y - 1 >= 0) {
                    
                    automata[x][y].neighbours[pos] = automata[x-1][y-1]; pos++;
                }
                if (y + 1 < DIMENSION) {
                    
                    automata[x][y].neighbours[pos] = automata[x-1][y+1]; pos++;
                }
            }

            if (x + 1 < DIMENSION) {
                automata[x][y].neighbours[pos] = automata[x+1][y]; pos++;
                if (y - 1 >= 0) {
                    automata[x][y].neighbours[pos] = automata[x+1][y-1]; pos++;
                }
                if (y + 1 < DIMENSION) {
                    automata[x][y].neighbours[pos] = automata[x+1][y+1]; pos++;
                }
            }
            automata[x][y].numOfNeighbours = pos;
        }
    } 
}


//free memory (for cellular automata)
void freeAutomata(automaton** automata) {
    for (int i = 0; i < DIMENSION; i++) {
        free((automata[i])->neighbours);
        free(automata[i]);
    }
    free(automata);
}

//display current automata
void printAutomata(automaton** automata) {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            automaton cell = automata[i][j];
            printf("%d ", cell.state);
        }
        printf("\n");
    }
}

void printNeighbours(automaton cell) {
    for (int i = 0; i < cell.numOfNeighbours; i++) printf("%d ", (cell.neighbours)[i].state);
    printf("\n");
}

//game play implementations

//update automaton at a tick

int checkNeighboursState(automaton *cell, int state) {
    int count = 0;

    for (int i = 0; i < cell->numOfNeighbours; i++) {
        if (((cell->neighbours)[i]).state == state) count++;
    }

    return count;
}
void updateState(automaton *cell, int tick) {
    int liveNeighbours = checkNeighboursState(cell, 1);
    int deadNeighbours = checkNeighboursState(cell, 0);

    //underpopulation
    if (liveNeighbours < 2 && cell->state == 1) cell->state = 0;
    
    //lives on
    if (liveNeighbours > 1 && liveNeighbours < 4 && cell->state == 1) cell->state = 1;
    
    //overpopulation
    if (liveNeighbours > 3 && cell->state == 1) cell->state = 0;
    
    //reproduction
    if (liveNeighbours == 3 && cell->state == 0) cell->state = 1;
}

void updateAutomata(automaton** automata) {
    for (int t = 0; t < TICKS; t++) {
        // printf("\r");
        printAutomata(automata);
        sleep(1);
        system("clear");
        // fflush(stdout);
        for (int x = 0; x < DIMENSION; x++) {
            for (int y = 0; y < DIMENSION; y++) {
                updateState(&automata[x][y], t);
            }
        }
    }
}

int population(automaton** automata) {
    int popSize = 0;

    for (int x = 0; x < DIMENSION; x++) {
        for (int y = 0; y < DIMENSION; y++) {
            if (automata[x][y].state == 1) popSize++;
        }
    }

    return popSize;
}

int main(int n, char** argv) {
    setbuf(stdout, NULL);
    system("clear");
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



