/*

    Projeto 1 - MC504
    Desenvolvido por Gabriel Massaki

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 9

typedef struct {
    int line, column, block;
} Info;


int sudoku[SIZE][SIZE];
int sudokuAux[SIZE][SIZE];
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

// Retorna a largura do numero
int getLength(int n) {
    return floor(log10(abs(n))) + 1;
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

// Imprime SudokuAux, onde ficam salvas as dicas, lembrando que as dicas ficam no formato
// 1289 = 1, 2, 8 e 9
void printSudokuAux() {

    int i, j;

    printf("\n\n\n");
    for (i = 0; i < SIZE; i ++) {
        for (j = 0; j < SIZE; j++) {
            if (sudoku[i][j] != 0) {
                printf("%d ", sudoku[i][j]);
            } else {
                printf("(%d) ", sudokuAux[i][j]);
            }
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

// Determina o bloco dadas linha e coluna
int getBlock(int line, int column) {
    if (line < 3) {
        if (column < 3) {
            return 0;
        } else if (column < 6) {
            return 1;
        } else {
            return 2;
        }
    } else if (line < 6) {
        if (column < 3) {
            return 3;
        } else if (column < 6) {
            return 4;
        } else {
            return 5;
        }
    } else {
        if (column < 3) {
            return 6;
        } else if (column < 6) {
            return 7;
        } else  {
            return 8;
        }
    }
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
            numbers[sudoku[l][i] - 1] ++;
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
            numbers[sudoku[i][c] - 1] ++;
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

    int numbers[SIZE], i, j, b, *r, iAux, jAux;
    r = malloc(sizeof(int));
    *r = 0;
    b = *(int *) block;

    iAux = b / 3;
    jAux = b % 3;

    for (i = 0; i < SIZE; i ++)
        numbers[i] = 0;

    for (i = iAux * 3; i < (iAux + 1) * 3; i ++) {
        for (j = jAux * 3; j < (jAux + 1) * 3; j ++) {
            if (sudoku[i][j] != 0)
                numbers[sudoku[i][j] - 1] ++;
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
// É criada uma thread para verificar cada linha, cada coluna e cada bloco
// totalizando 27 thread criadas.
// Retorna 1 se foi encontrado algum erro e 0 caso contrario
int check() {

    int i, count, *n, r, rTemp;
    pthread_t thr[27];

    r = 0;
    count = 0;

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, checkLine, (void*) n);
        count ++;
    }

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, checkColumn, (void*) n);
        count ++;
    }

    for (i = 0; i < SIZE; i ++) {
        n = malloc(sizeof(int));
        *n = i;
        pthread_create(&thr[count], NULL, checkBlock, (void*) n);
        count ++;
    }

    for (i = 0; i < count; i ++) {
        pthread_join(thr[i], (void**) &rTemp);
        r = r | rTemp;
    }

    return r;
}

// Salva em sudokuAux os numeros possiveis para a posição passada como parametro
// Se o numero salvo é 1289, significa que as opções para aquela posição são 1, 2, 8 e 9
// Usa a mesma lógica do check para achar quais numeros são válidos
void* tipsAux(void* position) {

    int i, j, iAux, jAux, numbers[SIZE], num;
    Info *info;

    num = 0;
    info = (Info*) position;
    iAux = info->block / 3;
    jAux = info->block % 3;

    for (i = 0; i < SIZE; i ++) {
        numbers[i] = 0;
    }

    for (i = 0; i < SIZE; i ++) {
        if (sudoku[info->line][i] != 0)
            numbers[sudoku[info->line][i] - 1] = 1;
        if (sudoku[i][info->column] != 0)
            numbers[sudoku[i][info->column] - 1] = 1;
    }

    for (i = iAux * 3; i < (iAux + 1) * 3; i ++) {
        for (j = jAux * 3; j < (jAux + 1) * 3; j ++) {
            if (sudoku[i][j] != 0)
                numbers[sudoku[i][j] - 1] = 1;
        }
    }

    for (i = 0; i < SIZE; i ++) {
        if (numbers[i] == 0) {
            num = num * 10 + (i + 1);
        }
    }

    sudokuAux[info->line][info->column] = num;
    free(position);
    return NULL;
}


// Cria uma thread para cada posição vazia, procurando as possívies opções para preencher
// a determinada posição, salvando essas opções em sudokuAux.
void tips() {

    int i, j, count;
    pthread_t thr[81];
    Info *info;

    count = 0;

    for (i = 0; i < SIZE; i ++) {
        for (j = 0; j < SIZE; j ++) {
            if (sudoku[i][j] == 0) {
                info = malloc(sizeof(Info));
                info->line = i;
                info->column = j;
                info->block = getBlock(i, j);
                pthread_create(&thr[count], NULL, tipsAux, (void*) info);
                count ++;
            } else {
                sudokuAux[i][j] = sudoku[i][j];
            }
        }
    }
    for (i = 0; i < count; i ++)
        pthread_join(thr[i], NULL);

}

// Resolve usando forca bruta, testando todos os valores possives para as posicoes até
// encontrar o valor certo para posicao
int solveBruteForce(int line, int column) {
    
    int l, c, num, aux;
    
    c = column + 1;
    if (c == 9) {
        c = 0;
        l = line + 1;
        if (l == 9) {
            return 1;
        }
    }
    
    if (sudoku[line][column] == 0) {
        num = sudokuAux[line][column];
        while(num != 0) {
            aux = num % 10;
            sudoku[line][column] = aux;
            if (solveBruteForce(c, l) == 1) {
                return 1;
            } else {
                sudoku[line][column] = 0;
            }
            num = num / 10;
        }
        return 0;
    } else {
        if (solveBruteForce(l, c) == 1)
            return 1;
        else
            return 0;
    }
}


// Resolve o sudoku usando as dicas, se aproveitando do fato de que as vezes alguma dica retorna 
// um único número, que portanto deve ser colocado na posição
// Caso a quantidade de zeros chegue a 0, significa que o sudoku foi completo
// Caso a quantidade de zeros não se altere após uma iteração, significa que não é possível resolver usando
// esse método, pois existe alguma inferência. Nesse caso é usada força bruta para achar a solução
void solve() {

    int i, j, zeros, oldZeros;

    zeros = 1;
    oldZeros = 0;
    while(zeros != 0) {
        zeros = 0;
        tips();
        for (i = 0; i < SIZE; i ++) {
            for (j = 0; j < SIZE; j ++) {
                if (sudoku[i][j] == 0) {
                    zeros ++;
                    if (getLength(sudokuAux[i][j]) == 1) {
                        sudoku[i][j] = sudokuAux[i][j];
                    }
                }
            }
        }
        if (oldZeros == zeros)
            break;
        oldZeros = zeros;
    }
    if (zeros != 0) {
        solveBruteForce(0, 0);
    }
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
            printMessages();

        // Dá dicas do Sudoku
        } else if (option == 2) {

            readSudoku();
            tips();
            printSudokuAux();

        // Resolve o Sudoku
        } else if (option == 3) {

            readSudoku();
            solve();

        // Sai do programa
        } else if (option == 4) {
            break;
        }
    }

    return 0;

}
