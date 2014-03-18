/*

    Projeto 1 - MC504
    Desenvolvido por Gabriel Massaki

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9

typedef struct {
    int line, column, number;
} Info;


int sudoku[SIZE][SIZE];
char messages[100][128];
int counter;

// Imprime o menu inicial
void printMenu() {

    printf("\n\n\n");
    printf("Seleciona uma opção:\n");
    printf("1 - Verificar um Sudoku\n");
    printf("2 - Dar dicas de um Sudoku\n");
    printf("3 - Resolver um Sudoku\n");
    printf("4 - Sair\n");
    printf("\n\n\n");

}

// Lê o Sudoku
void readSudoku() {

    int counter;
    char c;

    counter = 0;

    while (counter < SIZE * SIZE) {
        scanf("%c", &c);
        if (c >= '1' && c<= '9') {
            sudoku[counter / SIZE][counter % SIZE] = c - '0';
            counter ++;
        } else if (c == 'X') {
            sudoku[counter / SIZE][counter % SIZE] = 0;
            counter ++;
        }
    }
}

// Imprime o Sudoku
void printSudoku() {

    int i, j;

    printf("\n\n\n");
    for (i = 0; i < SIZE; i ++) {
        for (j = 0; j < SIZE; j ++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}

// Imprime mensagens
void printMessages() {

    int i;

    for (i = 0; i < counter; i ++)
        printf("%s\n", messages[i]);

}

// Verifica se há algum número repetido na linha
// O vetor numbers é inicializado com 0, é somado 1 em cada índice do vetor que aparece na linha
// Os índices que possuirem valor maior que 1 apareceram mais de uma vez na linha
// Se foram encontrados erros, a mensagem é salva
// Retorna 1 sse houve erro e 0 caso contrario
void* checkLine(void* line) {

    int numbers[SIZE], i, l, *r;
    r = malloc(sizeof(int));
    *r = 0;
    l = *(int *) line;

    for (i = 0; i < SIZE; i ++)
        numbers[i] = 0;

    for (i = 0; i < SIZE; i ++) {
        if (sudoku[l][i] != 0)
            numbers[sudoku[l][i]] ++;
    }

    for (i = 0; i < SIZE; i ++) {
        if (numbers[i] > 1) {
            sprintf (messages[counter], "A linha %d contém %d ocorrências do número %d.", l + 1, numbers[i], i + 1);
            counter ++;
            *r = 1;
        }
    }
    free(line);
    return (void*) r;
}

// Verifica se há algum número repetido na coluna
// O vetor numbers é inicializado com 0, é somado 1 em cada índice do vetor que aparece na coluna
// Os índices que possuirem valor maior que 1 apareceram mais de uma vez na coluna
// Se foram encontrados erros, a mensagem é salva
// Retorna 1 sse houve erro e 0 caso contrario
void* checkColumn(void* column) {

    int numbers[SIZE], i, c, *r;
    r = malloc(sizeof(int));
    *r = 0;
    c = *(int *) column;

    for (i = 0; i < SIZE; i ++)
        numbers[i] = 0;

    for (i = 0; i < SIZE; i ++) {
        if (sudoku[i][c] != 0)
            numbers[sudoku[i][c]] ++;
    }

    for (i = 0; i < SIZE; i ++) {
        if (numbers[i] > 1) {
            sprintf (messages[counter], "A coluna %d contém %d ocorrências do número %d.", c + 1, numbers[i], i + 1);
            counter ++;
            *r = 1;
        }
    }
    free(column);
    return (void*) r;
}

// Verifica se há algum número repetido no bloco
// O vetor numbers é inicializado com 0, é somado 1 em cada índice do vetor que aparece no bloco
// Os índices que possuirem valor maior que 1 apareceram mais de uma vez no bloco
// Se foram encontrados erros, a mensagem é salva
// Retorna 1 sse houve erro e 0 caso contrario
/*

    Blocos:

    -------------------------
    |       |       |       |
    |   0   |   1   |   2   |
    |       |       |       |
    -------------------------
    |       |       |       |
    |   3   |   4   |   5   |
    |       |       |       |
    -------------------------
    |       |       |       |
    |   6   |   7   |   8   |
    |       |       |       |
    -------------------------

*/
void* checkBlock(void* block) {

    int numbers[SIZE], i, j, b, *r, iLim, jLim;
    r = malloc(sizeof(int));
    *r = 0;
    b = *(int *) block;

    iLim = b / 3;
    jLim = b % 3;

    for (i = 0; i < SIZE; i ++)
        numbers[i] = 0;

    for (i = iLim * 3; i < (iLim + 1) * 3; i ++) {
        for (j = jLim * 3; j < (jLim + 1) * 3; j ++) {
            if (sudoku[i][j] != 0)
                numbers[sudoku[i][j]] ++;
        }
    }

    for (i = 0; i < SIZE; i ++) {
        if (numbers[i] > 1) {
            sprintf (messages[counter], "O bloco %d contém %d ocorrências do número %d.", b, numbers[i], i + 1);
            counter ++;
            *r = 1;
        }
    }
    free(block);
    return (void*) r;
}


// Verifica se o Sudoku possui erros
int check() {

    int i, count, *n, r, rTemp;
    pthread_t thr[27];

    r = 0;
    count = 0;

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, lineCheck, (void*) n);
        count ++;
    }

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, columnCheck, (void*) n);
        count ++;
    }

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, blockCheck, (void*) n);
        count ++;
    }

    for (i = 0; i < counter; i ++) {
        pthread_join(thr[i], (void**) &rTemp);
        r = r | rTemp;
    }

    return r;
}

int main() {

    int option;

    while(1) {

        counter = 0;
        printMenu();
        scanf("%d", &option);

        // Verifica o Sudoku    
        if (option == 1) {

            readSudoku();
            check();

        // Dá dicas do Sudoku
        } else if (option == 2) {

            readSudoku();

        // Resolve o Sudoku
        } else if (option == 3) {

            readSudoku();

        // Sai do programa
        } else if (option == 4) {
            break;
        }
    }

    return 0;

}
