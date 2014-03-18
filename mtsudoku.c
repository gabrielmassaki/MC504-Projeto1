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

// Verifica se há algum número repetido na linha
// O vetor numbers é inicializado com 0, é somado 1 em cada índice do vetor que aparece na linha
// Os índices que possuirem valor maior que 1 apareceram mais de uma vez na linha
void* checkLine(void* line) {

    int numbers[SIZE], i, l, *r;
    r = malloc(sizeof(int));
    *r = 0;
    l = *(int *) line;

    for (i = 0; i < SIZE; i ++)
        numbers[i] = 0;

    for (i = 0; i < SIZE; i ++) {
        if (sudoku[l][i] != 0)
            numbers[i] ++;
    }

    for (i = 0; i < SIZE; i ++) {
        if (numbers[i] > 1) {
            sprintf (messages[counter], "A linha %d contém %d ocorrências do número %d.\n", l, numbers[i], i + 1);
            *r = 1;
        }
    }
    free(line);
    return (void*) r;
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

        // Dá dicas do Sudoku
        } else if (option == 2) {


        // Resolve o Sudoku
        } else if (option == 3) {


        // Sai do programa
        } else if (option == 4) {


        }
    }

    return 0;

}
