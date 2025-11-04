// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Atualizando para o nível aventureiro
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void cadastrarTerritorios(Territorio *mapa, int qtd);
void exibirMapa(Territorio *mapa, int qtd);
void atacar(Territorio *atacante, Territorio *defensor);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa, char **missoesJogadores, int numJogadores);


int main() {

// Nível Mestre
  srand(time(NULL)); // Garante aleatoriedade

    int qtdTerritorios, numJogadores;

    printf("Digite o número total de territórios: ");
    scanf("%d", &qtdTerritorios);

    // Alocação dinâmica dos territórios
    Territorio *mapa = (Territorio *)calloc(qtdTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória para territórios!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, qtdTerritorios);
    exibirMapa(mapa, qtdTerritorios);

    // Cadastro dos jogadores
    printf("\nDigite o número de jogadores: ");
    scanf("%d", &numJogadores);

    // Missões pré-definidas
    char *missoesDisponiveis[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar metade do mapa",
        "Conquistar o território Azul",
        "Manter pelo menos 5 territórios com 3 tropas ou mais"
    };
    int totalMissoes = 5;

    // Alocação dinâmica das missões dos jogadores
    char **missoesJogadores = (char **)malloc(numJogadores * sizeof(char *));
    for (int i = 0; i < numJogadores; i++) {
        missoesJogadores[i] = (char *)malloc(100 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    // Mostra as missões de cada jogador no início do jogo
    printf("\n=== Missões dos Jogadores ===\n");
    for (int i = 0; i < numJogadores; i++) {
        printf("Jogador %d: %s\n", i + 1, missoesJogadores[i]);
    }

    //  Simulação de jogo e ataques 
    int atacante, defensor;
    char continuar;
    int vencedor = -1;

    do {
        printf("\nEscolha o território atacante (1-%d): ", qtdTerritorios);
        scanf("%d", &atacante);
        printf("Escolha o território defensor (1-%d): ", qtdTerritorios);
        scanf("%d", &defensor);

        if (atacante < 1 || atacante > qtdTerritorios || defensor < 1 || defensor > qtdTerritorios || atacante == defensor) {
            printf("\n Escolhas inválidas!\n");
        } else {
            atacar(&mapa[atacante - 1], &mapa[defensor - 1]);
            exibirMapa(mapa, qtdTerritorios);
        }

        // Verifica se algum jogador completou sua missão
        for (int i = 0; i < numJogadores; i++) {
            if (verificarMissao(missoesJogadores[i], mapa, qtdTerritorios)) {
                vencedor = i;
                break;
            }
        }

        if (vencedor != -1) {
            printf("\n O Jogador %d venceu! Missão cumprida: %s\n", vencedor + 1, missoesJogadores[vencedor]);
            break;
        }

        printf("\nDeseja continuar jogando? (s/n): ");
        scanf(" %c", &continuar);

    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa, missoesJogadores, numJogadores);
    return 0;
}

// Cadastro
void cadastrarTerritorios(Territorio *mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Exibe os territórios do mapa
void exibirMapa(Territorio *mapa, int qtd) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("(%d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula o ataque entre dois territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n Não é possível atacar um território aliado!\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("\n O atacante precisa ter mais de uma tropa!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nDados: Atacante=%d | Defensor=%d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(" O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf(" O defensor resistiu!\n");
        atacante->tropas--;
    }
}

// Atribui uma missão aleatória ao jogador
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Verifica se a missão foi cumprida
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    if (strstr(missao, "vermelha")) {
        int dominou = 1;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0) {
                dominou = 0;
                break;
            }
        }
        return dominou;
    }

    if (strstr(missao, "3 territórios")) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 3) count++;
        }
        if (count >= 3) return 1;
    }

    return 0; 
}

// Libera toda a memória alocada dinamicamente
void liberarMemoria(Territorio *mapa, char **missoesJogadores, int numJogadores) {
    for (int i = 0; i < numJogadores; i++) {
        free(missoesJogadores[i]);
    }
    free(missoesJogadores);
    free(mapa);
    printf("\n Memória liberada com sucesso.\n");
}



    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.


// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
