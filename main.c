#include "automata.h"


int main(int n, char** argv) {
    automaton **system = createSystem();
    printSystem(system);
    freeSystem(system);

    return 0;
}