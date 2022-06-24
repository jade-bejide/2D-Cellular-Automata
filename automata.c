#include "automata.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include <string.h>


#define DIMENSION_X 50
#define DIMENSION_Y 150
#define TICKS 20
#define BLOCK '\u258a'

//defining an automaton
typedef struct automaton {
    int state;
    int nxtState;
    struct automaton *neighbours;
    int numOfNeighbours;
} automaton;

//create cellular automata, allocating memory
automaton *createAutomaton() {
    automaton *cell = malloc(sizeof(automaton));
    //1 - live, 0 - dead
    cell->state = rand() % 2;
    cell->nxtState = cell->state;
    cell->neighbours = malloc(DIMENSION_X * sizeof(automaton));
    cell->numOfNeighbours = 0;

    return cell;
}

automaton **createAutomata() {  
    automaton **automata = malloc((DIMENSION_X * DIMENSION_Y)*sizeof(automaton));

    srand(time(0));

    for (int i = 0; i < DIMENSION_X; i++) {
        automaton *automataN = malloc(DIMENSION_Y * sizeof(automaton));
        for (int j = 0; j < DIMENSION_Y; j++) automataN[j] = *createAutomaton();
        automata[i] = automataN;
    }
   
   return automata;
}

void establishNeighbours(automaton** automata) {
    for (int x = 0; x < DIMENSION_X; x++) {
        for (int y = 0; y < DIMENSION_Y; y++) {
            //establish neighbours array
            int pos = 0;

            if (y-1 >= 0) {
                automata[x][y].neighbours[pos] = automata[x][y-1]; pos++;
            }
            if (y + 1 < DIMENSION_Y) {
                automata[x][y].neighbours[pos] = automata[x][y+1]; pos++;
            }
            if (x - 1 >= 0) {
                automata[x][y].neighbours[pos] = automata[x-1][y]; pos++;
                if (y - 1 >= 0) {
                    
                    automata[x][y].neighbours[pos] = automata[x-1][y-1]; pos++;
                }
                if (y + 1 < DIMENSION_Y) {
                    
                    automata[x][y].neighbours[pos] = automata[x-1][y+1]; pos++;
                }
            }

            if (x + 1 < DIMENSION_X) {
                automata[x][y].neighbours[pos] = automata[x+1][y]; pos++;
                if (y - 1 >= 0) {
                    automata[x][y].neighbours[pos] = automata[x+1][y-1]; pos++;
                }
                if (y + 1 < DIMENSION_Y) {
                    automata[x][y].neighbours[pos] = automata[x+1][y+1]; pos++;
                }
            }
            automata[x][y].numOfNeighbours = pos;
        }
    } 
}

void clearScreen() {
    #if _WIN32
        system("clear");
    #else
        system("cls");
    #endif
}


//free memory (for cellular automata)
void freeAutomata(automaton** automata) {
    for (int i = 0; i < DIMENSION_X; i++) {
        free((automata[i])->neighbours);
        free(automata[i]);
    }
    free(automata);
}

//display current automata
void printAutomata(automaton** automata) {
    for (int i = 0; i < DIMENSION_X; i++) {
        for (int j = 0; j < DIMENSION_Y; j++) {
            automaton cell = automata[i][j];
            if (cell.state == 1) printf("\033[0;37m");
            else printf("\033[0;30m");
            printf("\u2588");
            //reset to default (black)
            printf("\033[0;37m");
            // printf("%d ", cell.state);
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
    if (liveNeighbours < 2 && cell->state == 1) cell->nxtState = 0;
    
    //lives on
    if (liveNeighbours > 1 && liveNeighbours < 4 && cell->state == 1) cell->nxtState = 1;
    
    //overpopulation
    if (liveNeighbours > 3 && cell->state == 1) cell->nxtState = 0;
    
    //reproduction
    if (liveNeighbours == 3 && cell->state == 0) cell->nxtState = 1;

    
}

void parseState(automaton **automata) {
    for (int x = 0; x < DIMENSION_X; x++) {
        for (int y = 0; y < DIMENSION_Y; y++) {
            automata[x][y].state = automata[x][y].nxtState;
        }
    }
}

void updateAutomata(automaton** automata) {
    for (int t = 0; t < TICKS; t++) {
        // printf("\r");
        printAutomata(automata);
        sleep(1);
        system("clear");
        fflush(stdout);
        for (int x = 0; x < DIMENSION_X; x++) {
            for (int y = 0; y < DIMENSION_Y; y++) {
                updateState(&automata[x][y], t);
            }
        }

        parseState(automata);
        establishNeighbours(automata);
    }
}

int population(automaton** automata) {
    int popSize = 0;

    for (int x = 0; x < DIMENSION_X; x++) {
        for (int y = 0; y < DIMENSION_Y; y++) {
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



