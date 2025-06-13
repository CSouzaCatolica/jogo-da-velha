#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int status; 
    int winner; 
} GameResult;

void welcome();
GameResult runGame();
void board(int movimentos[3][3]);
void sep();
int checaVitoria(int movimentos[3][3]);

#define SEP_STR "--=================--\n"
#define BOARD_V "   %c | %c | %c\n"
#define BOARD_H "   -----------\n"
// ##################################################################################################
int main() {
    char opc[20];

    while (true) {
        welcome();
        scanf("%s", opc);

        if (strcmp(opc, "1") == 0) {
            GameResult resultadoFinal = runGame();

            if (resultadoFinal.status == 1) { // vitoria
                printf("\nParabens! O jogador %c venceu!\n", (resultadoFinal.winner == 1) ? 'X' : 'O');
            } else if (resultadoFinal.status == 2) { // empate
                printf("\nO jogo terminou em empate! Deu velha!\n");
            }
            
            break; 

        } else if (strcmp(opc, "sair") == 0 || strcmp(opc, "SAIR") == 0) {
            printf("\nSaindo...\n");
            break;
        }
    }
    return 0;
}
// ##################################################################################################
void sep() {
    printf("%s", SEP_STR);
}
// ##################################################################################################
void welcome() {
    sep();
    printf("[1]    - Iniciar o jogo\n");
    printf("[sair] - Sair\n");
    printf("Escolha uma opcao: ");
}
// ##################################################################################################
GameResult runGame() {

    GameResult resultado = {0, 0}; 
    int movimentos[3][3] = {{0}};  
    int currentPlayer = 1; // X começa
    int moveCount = 0;
    int move;

    printf("\n--- Jogo Iniciado! ---\n");
    printf("Jogador 1: X | Jogador 2: O\n");

    while (true) {
        board(movimentos); 

        printf("Jogador %d (%c), faca sua jogada (1-9): ", currentPlayer, (currentPlayer == 1) ? 'X' : 'O');
        scanf("%d", &move);

        if (move < 1 || move > 9) {
            printf("Jogada invalida! Digite um numero entre 1 e 9!\n");
            continue;
        }

        int row = (move - 1) / 3;
        int col = (move - 1) % 3;

        if (movimentos[row][col] != 0) {
            printf("Posicao ja ocupada! Tente outra!!!\n");
            continue;
        }

        movimentos[row][col] = currentPlayer;
        moveCount++;

        int winner = checaVitoria(movimentos);
        if (winner != 0) {
            resultado.status = 1;
            resultado.winner = winner;
            board(movimentos);
            return resultado;
        }

        if (moveCount == 9) {
            resultado.status = 2;
            board(movimentos); 
            return resultado;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
}
// ##################################################################################################
void board(int movimentos[3][3]) {
    char mov[3][3];
    int pos = 1; 

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (movimentos[i][j] == 0) {
                mov[i][j] = pos + '0'; 
            } else if (movimentos[i][j] == 1) {
                mov[i][j] = 'X';
            } else { // movimentos[i][j] == 2
                mov[i][j] = 'O';
            }
            pos++;
        }
    }

    printf("\n");
    printf(BOARD_V, mov[0][0], mov[0][1], mov[0][2]);
    printf(BOARD_H);
    printf(BOARD_V, mov[1][0], mov[1][1], mov[1][2]);
    printf(BOARD_H);
    printf(BOARD_V, mov[2][0], mov[2][1], mov[2][2]);
    printf("\n");
}
// ##################################################################################################
int checaVitoria(int movimentos[3][3]) {

    // linhas
    for (int i = 0; i < 3; i++) {
        if (movimentos[i][0] != 0 && movimentos[i][0] == movimentos[i][1] && movimentos[i][1] == movimentos[i][2]) {
            return movimentos[i][0];
        }
    }
    // colunas
    for (int i = 0; i < 3; i++) {
        if (movimentos[0][i] != 0 && movimentos[0][i] == movimentos[1][i] && movimentos[1][i] == movimentos[2][i]) {
            return movimentos[0][i];
        }
    }
    // diagonais
    if (movimentos[0][0] != 0 && movimentos[0][0] == movimentos[1][1] && movimentos[1][1] == movimentos[2][2]) {
        return movimentos[0][0];
    }
    if (movimentos[0][2] != 0 && movimentos[0][2] == movimentos[1][1] && movimentos[1][1] == movimentos[2][0]) {
        return movimentos[0][2];
    }
    return 0; // ngm vence
}