/*

    Projeto 1 - MC504
    Desenvolvido por Gabriel Massaki

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 9

int sudoku[SIZE][SIZE];


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

int main() {

    int option;

    while(1) {

        printMenu();
        scanf("%d", &option);
        readSudoku();
        printSudoku();
    
        if (option == 1) {

        } else if (option == 2) {


        } else if (option == 3) {


        } else if (option == 4) {


        }
    }

    return 0;

}
