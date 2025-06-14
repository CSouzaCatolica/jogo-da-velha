#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SCORE_FILE "placar.txt"
#define SEP_STR "--=================--\n"

typedef enum {
    NO_PLAYER = 0,
    PLAYER_X = 1,
    PLAYER_O = 2
} Player;

typedef struct {
    int status;     // 0 rodando,1 ganho, 2 empate, 3 saiu
    Player winner; 
} GameResult;

void welcome();
GameResult runGame();
void board(int movimentos[3][3]);
void sep();
Player checaVitoria(int movimentos[3][3]);
void mostraPlacar(int placar[2]);
void carregarPlacar(int placar[2]);
void salvarPlacar(int placar[2]);

// ##################################################################################################
int main() {
    char opc[20];
    int placar[2]; // 0 para PLAYER_X, 1 PLAYER_O

    carregarPlacar(placar);

    while (true) {
        welcome();
        scanf("%s", opc);

        if (strcmp(opc, "1") == 0) {
            GameResult resultadoFinal = runGame();

            if (resultadoFinal.status == 1) { // vitoria
                // enum pro players
                printf("\nParabens! O jogador %c venceu!\n", (resultadoFinal.winner == PLAYER_X) ? 'X' : 'O');
                placar[resultadoFinal.winner - 1]++; // add no placar
                salvarPlacar(placar);
                printf("Placar salvo!\n");
                mostraPlacar(placar);

            } else if (resultadoFinal.status == 2) { // empate
                printf("\nO jogo terminou em empate! Deu velha!\n");
                mostraPlacar(placar);

            } else if (resultadoFinal.status == 3) { // desiste
                printf("\nJogo abandonado. Voltando ao menu principal.\n");
                #ifdef _WIN32
                    Sleep(2000);
                #else
                    sleep(2);
                #endif
            }

        } else if (strcmp(opc, "2") == 0) {
            mostraPlacar(placar);

        } else if (strcmp(opc, "sair") == 0 || strcmp(opc, "SAIR") == 0) {
            printf("\nObrigado por jogar! Saindo...\n");
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
    system("cls || clear");
    sep();
    printf("   JOGO DA VELHA\n");
    sep();
    printf("[1]    - Novo Jogo\n");
    printf("[2]    - Ver Placar\n");
    printf("[sair] - Sair do Jogo\n");
    printf("Escolha uma opcao: ");
}

// ##################################################################################################
GameResult runGame() {
    GameResult resultado = {0, NO_PLAYER}; 
    int movimentos[3][3] = {{NO_PLAYER}}; 

    Player currentPlayer = PLAYER_X; 
    int moveCount = 0;
    int line, col;
    char input[20];

    system("cls || clear");
    printf("\n--- Jogo Iniciado! ---\n");
    printf("Jogador X (1) | Jogador O (2)\n");
    printf("Digite 'sair' a qualquer momento para desistir e voltar ao menu.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    while (true) {
        board(movimentos);
        printf("Jogador %c, faca sua jogada (linha,coluna): ", (currentPlayer == PLAYER_X) ? 'X' : 'O');
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            resultado.status = 3;
            return resultado;
        }
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "sair") == 0 || strcmp(input, "SAIR") == 0) {
            resultado.status = 3;
            return resultado;
        }
        if (sscanf(input, "%d,%d", &line, &col) != 2) {
            printf("Entrada invalida! Por favor, digite no formato linha,coluna (ex: 2,1) ou 'sair'.\n");
            continue;
        }
        line--;
        col--;
        if (line < 0 || line > 2 || col < 0 || col > 2) {
            printf("Jogada invalida! Linha e coluna devem estar entre 1 e 3.\n");
            continue;
        }

        if (movimentos[line][col] != NO_PLAYER) { // vazia?
            printf("Posicao ja ocupada! Tente outra.\n");
            continue;
        }

        movimentos[line][col] = currentPlayer;
        moveCount++;
        Player winner = checaVitoria(movimentos);
        if (winner != NO_PLAYER) { // ganho?
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

        // enum pra coisa o player x e o
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}

// ##################################################################################################
void board(int movimentos[3][3]) {
    char mov[3][3];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (movimentos[i][j] == NO_PLAYER) mov[i][j] = ' ';
            else if (movimentos[i][j] == PLAYER_X) mov[i][j] = 'X';
            else mov[i][j] = 'O';
        }
    }

    printf("\n     1   2   3\n");
    printf("   -----------\n");
    printf("1 |  %c | %c | %c  |\n", mov[0][0], mov[0][1], mov[0][2]);
    printf("  |----|---|----|\n");
    printf("2 |  %c | %c | %c  |\n", mov[1][0], mov[1][1], mov[1][2]);
    printf("  |----|---|----|\n");
    printf("3 |  %c | %c | %c  |\n", mov[2][0], mov[2][1], mov[2][2]);
    printf("   -----------\n\n");
}

// ##################################################################################################
Player checaVitoria(int movimentos[3][3]) {
    
    for (int i = 0; i < 3; i++) {
        if (movimentos[i][0] != NO_PLAYER && movimentos[i][0] == movimentos[i][1] && movimentos[i][1] == movimentos[i][2]) return movimentos[i][0];
        if (movimentos[0][i] != NO_PLAYER && movimentos[0][i] == movimentos[1][i] && movimentos[1][i] == movimentos[2][i]) return movimentos[0][i];
    }
    if (movimentos[0][0] != NO_PLAYER && movimentos[0][0] == movimentos[1][1] && movimentos[1][1] == movimentos[2][2]) return movimentos[0][0];
    if (movimentos[0][2] != NO_PLAYER && movimentos[0][2] == movimentos[1][1] && movimentos[1][1] == movimentos[2][0]) return movimentos[0][2];
    
    return NO_PLAYER; 
}

// ##################################################################################################
void carregarPlacar(int placar[2]) {
    FILE *arquivo = fopen(SCORE_FILE, "r");
    if (arquivo == NULL) {
        placar[0] = 0; //  X
        placar[1] = 0; //  O
    } else {
        fscanf(arquivo, "%d", &placar[0]);
        fscanf(arquivo, "%d", &placar[1]);
        fclose(arquivo);
    }
}

// ##################################################################################################
void salvarPlacar(int placar[2]) {
    FILE *arquivo = fopen(SCORE_FILE, "w");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel salvar o placar.\n");
        return;
    }
    fprintf(arquivo, "%d\n", placar[0]); // X
    fprintf(arquivo, "%d\n", placar[1]); // O
    fclose(arquivo);
}

// ##################################################################################################
void mostraPlacar(int placar[2]) {
    system("cls || clear");
    sep();
    printf("--- PLACAR ATUAL ---\n");
    printf("   Jogador 'X': %d vitorias\n", placar[0]);
    printf("   Jogador 'O': %d vitorias\n", placar[1]);
    sep();
    printf("Pressione Enter para continuar...");

    // buffer
    getchar();
    getchar();
}