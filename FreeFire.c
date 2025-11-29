#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20

/* ============================================================
   Struct Componente
   Representa um componente da torre de fuga.
   - nome:      nome do componente (ex: "ChaveMestra")
   - tipo:      categoria (ex: "controle", "suporte")
   - prioridade: valor de 1 a 10 indicando importância
   ============================================================ */
typedef struct {
    char nome[30];
    char tipo[20];
    int  prioridade;
} Componente;

/* ------------------------------------------------------------
   Protótipos das funções obrigatórias
   ------------------------------------------------------------ */
void bubbleSortNome(Componente vet[], int n, long *comparacoes);
void insertionSortTipo(Componente vet[], int n, long *comparacoes);
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes);
int  buscaBinariaPorNome(Componente vet[], int n, char chave[], long *comparacoes);
void mostrarComponentes(Componente vet[], int n);
double medirTempo(void (*algoritmo)(Componente[], int, long*),
                  Componente vet[], int n, long *comparacoes);

/* Outras funções auxiliares */
void limparBufferEntrada(void);
void adicionarComponente(Componente vet[], int *qtd);
void descartarComponente(Componente vet[], int *qtd);

/* ============================================================
   FUNÇÃO PRINCIPAL
   ============================================================ */
int main(void) {
    Componente mochila[MAX_COMPONENTES];
    int qtd = 0;                   // componentes cadastrados
    int opcao;
    int ordenadoPorNome = 0;       // 0 = NÃO ORDENADO, 1 = ORDENADO POR NOME
    char chave[30];

    do {
        printf("\n=====================================================\n");
        printf("   PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
        printf("=====================================================\n");
        printf("Itens na Mochila: %d/%d\n", qtd, MAX_COMPONENTES);
        printf("Status da Ordenacao por Nome: %s\n",
               ordenadoPorNome ? "ORDENADO" : "NAO ORDENADO");
        printf("-----------------------------------------------------\n");
        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binaria por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("-----------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) return 0;
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                adicionarComponente(mochila, &qtd);
                break;

            case 2:
                descartarComponente(mochila, &qtd);
                break;

            case 3:
                mostrarComponentes(mochila, qtd);
                break;

            case 4: {
                if (qtd == 0) {
                    printf("\nNenhum componente cadastrado para ordenar.\n");
                    break;
                }

                int escolhaOrdenacao;
                long comps = 0;
                double tempo;

                printf("\n--- ORGANIZAR MOCHILA (ORDENAR COMPONENTES) ---\n");
                printf("1. Bubble Sort  - Ordenar por NOME\n");
                printf("2. Insertion Sort - Ordenar por TIPO\n");
                printf("3. Selection Sort - Ordenar por PRIORIDADE\n");
                printf("Escolha o metodo de ordenacao: ");
                if (scanf("%d", &escolhaOrdenacao) != 1) return 0;
                limparBufferEntrada();

                switch (escolhaOrdenacao) {
                    case 1:
                        tempo = medirTempo(bubbleSortNome, mochila, qtd, &comps);
                        ordenadoPorNome = 1;  // ficou ordenado por nome
                        printf("\n[BUBBLE SORT - ORDENACAO POR NOME]\n");
                        break;
                    case 2:
                        tempo = medirTempo(insertionSortTipo, mochila, qtd, &comps);
                        ordenadoPorNome = 0;  // critério agora é tipo
                        printf("\n[INSERTION SORT - ORDENACAO POR TIPO]\n");
                        break;
                    case 3:
                        tempo = medirTempo(selectionSortPrioridade, mochila, qtd, &comps);
                        ordenadoPorNome = 0;  // critério agora é prioridade
                        printf("\n[SELECTION SORT - ORDENACAO POR PRIORIDADE]\n");
                        break;
                    default:
                        printf("\nOpcao de ordenacao invalida.\n");
                        break;
                }

                if (escolhaOrdenacao >= 1 && escolhaOrdenacao <= 3) {
                    printf("Comparacoes realizadas na ordenacao: %ld\n", comps);
                    printf("Tempo de execucao: %.6f segundos\n", tempo);
                    mostrarComponentes(mochila, qtd);
                }
                break;
            }

            case 5: {
                if (qtd == 0) {
                    printf("\nNenhum componente cadastrado para buscar.\n");
                    break;
                }
                if (!ordenadoPorNome) {
                    printf("\nALERTA: A busca binaria requer que a mochila esteja ORDENADA POR NOME.\n");
                    printf("Use a opcao 4 e escolha Bubble Sort (ordenar por nome) primeiro.\n");
                    break;
                }

                printf("\n--- Busca Binaria por Componente-Chave ---\n");
                printf("Nome do componente a buscar: ");
                fgets(chave, sizeof(chave), stdin);
                chave[strcspn(chave, "\n")] = '\0';

                long compsBusca = 0;
                int indice = buscaBinariaPorNome(mochila, qtd, chave, &compsBusca);

                if (indice == -1) {
                    printf("\nComponente-Chave '%s' NAO encontrado.\n", chave);
                } else {
                    printf("\n--- Componente-Chave Encontrado! ---\n");
                    printf("Nome: %s, Tipo: %s, Prio: %d\n",
                           mochila[indice].nome,
                           mochila[indice].tipo,
                           mochila[indice].prioridade);
                }
                printf("Comparacoes realizadas na busca binaria: %ld\n", compsBusca);
                break;
            }

            case 0:
                printf("\nTorre de fuga ativada. Fim de jogo!\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ============================================================
   Funções de cadastro e remoção
   ============================================================ */

// Adiciona novo componente na mochila
void adicionarComponente(Componente vet[], int *qtd) {
    if (*qtd >= MAX_COMPONENTES) {
        printf("\nMochila cheia! Nao e possivel adicionar mais componentes.\n");
        return;
    }

    Componente c;
    char buffer[32];

    printf("\n--- ADICIONAR COMPONENTE ---\n");
    printf("Nome do componente: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Tipo do componente (controle, suporte, propulsao...): ");
    fgets(c.tipo, sizeof(c.tipo), stdin);
    c.tipo[strcspn(c.tipo, "\n")] = '\0';

    printf("Prioridade (1 a 10): ");
    fgets(buffer, sizeof(buffer), stdin);
    c.prioridade = atoi(buffer);

    vet[*qtd] = c;
    (*qtd)++;

    printf("\nComponente adicionado com sucesso!\n");
}

// Remove componente pelo nome (descartar)
void descartarComponente(Componente vet[], int *qtd) {
    if (*qtd == 0) {
        printf("\nMochila vazia. Nada para descartar.\n");
        return;
    }

    char nome[30];
    printf("\n--- DESCARTAR COMPONENTE ---\n");
    printf("Nome do componente a descartar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int i, j, encontrado = 0;
    for (i = 0; i < *qtd; i++) {
        if (strcmp(vet[i].nome, nome) == 0) {
            encontrado = 1;
            // Desloca para "tapar o buraco"
            for (j = i; j < *qtd - 1; j++) {
                vet[j] = vet[j + 1];
            }
            (*qtd)--;
            printf("Componente '%s' descartado.\n", nome);
            break;
        }
    }

    if (!encontrado) {
        printf("Componente '%s' nao encontrado.\n", nome);
    }
}

/* ============================================================
   Impressão do inventário
   ============================================================ */

void mostrarComponentes(Componente vet[], int n) {
    int i;

    printf("\n--- INVENTARIO DA MOCHILA ---\n");
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }

    printf("NOME                          | TIPO            | PRIO\n");
    printf("--------------------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%-30s | %-14s | %d\n",
               vet[i].nome,
               vet[i].tipo,
               vet[i].prioridade);
    }
    printf("--------------------------------------------------------\n");
}

/* ============================================================
   Algoritmos de Ordenação
   ============================================================ */

// Bubble Sort por NOME (string)
void bubbleSortNome(Componente vet[], int n, long *comparacoes) {
    int i;
    int trocou;

    do {
        trocou = 0;
        for (i = 0; i < n - 1; i++) {
            (*comparacoes)++;
            if (strcmp(vet[i].nome, vet[i + 1].nome) > 0) {
                Componente tmp = vet[i];
                vet[i] = vet[i + 1];
                vet[i + 1] = tmp;
                trocou = 1;
            }
        }
    } while (trocou);
}

// Insertion Sort por TIPO (string)
void insertionSortTipo(Componente vet[], int n, long *comparacoes) {
    int i, j;
    for (i = 1; i < n; i++) {
        Componente chave = vet[i];
        j = i - 1;

        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(chave.tipo, vet[j].tipo) < 0) {
                vet[j + 1] = vet[j];
                j--;
            } else {
                break;
            }
        }
        vet[j + 1] = chave;
    }
}

// Selection Sort por PRIORIDADE (int)
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes) {
    int i, j, idxMin;

    for (i = 0; i < n - 1; i++) {
        idxMin = i;
        for (j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (vet[j].prioridade < vet[idxMin].prioridade) {
                idxMin = j;
            }
        }
        if (idxMin != i) {
            Componente tmp = vet[i];
            vet[i] = vet[idxMin];
            vet[idxMin] = tmp;
        }
    }
}

/* ============================================================
   Busca Binária por Nome (após ordenar por nome)
   ============================================================ */

int buscaBinariaPorNome(Componente vet[], int n, char chave[], long *comparacoes) {
    int ini = 0, fim = n - 1;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        (*comparacoes)++;

        int cmp = strcmp(chave, vet[meio].nome);
        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            fim = meio - 1;
        } else {
            ini = meio + 1;
        }
    }
    return -1;
}

/* ============================================================
   Função para medir o tempo de execução de um algoritmo
   ============================================================ */

double medirTempo(void (*algoritmo)(Componente[], int, long*),
                  Componente vet[], int n, long *comparacoes) {
    clock_t inicio, fim;
    *comparacoes = 0;           // zera comparações

    inicio = clock();
    algoritmo(vet, n, comparacoes);
    fim = clock();

    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* ============================================================
   Utilitário: limpar buffer de entrada
   ============================================================ */

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}