#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_ALPHABET 10
#define MAX_WORDS 100

typedef struct {
    int numStates;
    int alphabetSize;
    int transition[MAX_STATES][MAX_ALPHABET][MAX_STATES];
    int initialState;
    int isFinal[MAX_STATES];
} Afnd;

void initAfnd(Afnd *afnd, int numStates, int alphabetSize, int initialState) {
    afnd->numStates = numStates;
    afnd->alphabetSize = alphabetSize;
    afnd->initialState = initialState;

    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_ALPHABET; j++) {
            for (int k = 0; k < MAX_STATES; k++) {
                afnd->transition[i][j][k] = -1;
            }
        }
        afnd->isFinal[i] = 0;
    }
}

void addTransition(Afnd *afnd, int fromState, char symbol, int toState) {
    int symbolIdx = symbol - 'a';
    for (int i = 0; i < MAX_STATES; i++) {
        if (afnd->transition[fromState][symbolIdx][i] == -1) {
            afnd->transition[fromState][symbolIdx][i] = toState;
            break;
        }
    }
}

void setFinalState(Afnd *afnd, int state) {
    afnd->isFinal[state] = 1;
}

int simulateAfnd(Afnd *afnd, int currentState, const char *input, int pos) {
    if (input[pos] == '\0') {
        return afnd->isFinal[currentState];
    }
    int symbolIdx = input[pos] - 'a';
    for (int i = 0; i < MAX_STATES; i++) {
        int nextState = afnd->transition[currentState][symbolIdx][i];
        if (nextState != -1 && simulateAfnd(afnd, nextState, input, pos + 1)) {
            return 1;
        }
    }
    return 0;
}

int main() {
    Afnd afnd;
    char alphabetString[MAX_ALPHABET];
    int numStates, numFinals, numTransitions, alphabetSize, inputs;
    FILE *file = fopen("automatos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(file, "%s", alphabetString);
    fscanf(file, "%d", &numStates);
    fscanf(file, "%d", &numFinals);

    alphabetSize = strlen(alphabetString);
    initAfnd(&afnd, numStates, alphabetSize, 0);

    int finalStates[numFinals];
    for (int i = 0; i < numFinals; i++) {
        fscanf(file, "%d", &finalStates[i]);
        setFinalState(&afnd, finalStates[i]);
    }

    fscanf(file, "%d", &numTransitions);
    for (int i = 0; i < numTransitions; i++) {
        int actualState, nextState;
        char data;
        fscanf(file, "%d %c %d", &actualState, &data, &nextState);
        addTransition(&afnd, actualState, data, nextState);
    }

    fscanf(file, "%d", &inputs);
    char words[inputs][MAX_WORDS];
    for (int i = 0; i < inputs; i++) {
        fscanf(file, "%s", words[i]);
        if (simulateAfnd(&afnd, afnd.initialState, words[i], 0)) {
            printf("%d: '%s' OK\n", i + 1, words[i]);
        } else {
            printf("%d: '%s' not OK\n", i + 1, words[i]);
        }
    }

    fclose(file);
    return 0;
}
