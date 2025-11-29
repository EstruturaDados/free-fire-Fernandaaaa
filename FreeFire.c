#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10   // capacidade máxima da mochila (versão vetor)

// ===================== ESTRUTURAS DE DADOS ========================

// Representa um item da mochila (usado tanto no vetor quanto na lista)
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Nó da lista encadeada: guarda um Item e o ponteiro para o próximo nó
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// ===================== CONTADORES DE BUSCA ========================
// Usados para comparar desempenho (número de comparações)

int comparacoesSeqVetor = 0;
int comparacoesBinVetor = 0;
int comparacoesSeqLista = 0;

// ===================== PROTÓTIPOS =================================

// Função utilitária
void limparBufferEntrada(void);

// Versão VETOR
void menuVetor(Item mochila[], int *qtd);
void inserirItemVetor(Item mochila[], int *qtd);
void removerItemVetor(Item mochila[], int *qtd);
void listarItensVetor(const Item mochila[], int qtd);
int  buscarSequencialVetor(const Item mochila[], int qtd, const char *nome);
void ordenarVetor(Item mochila[], int qtd);
int  buscarBinariaVetor(const Item mochila[], int qtd, const char *nome);

// Versão LISTA ENCADEADA
void menuLista(No **lista);
void inserirItemLista(No **lista);
void removerItemLista(No **lista);
void listarItensLista(const No *lista);
No* buscarSequencialLista(const No *lista, const char *nome);
void liberarLista(No **lista);

// Comparação das buscas (vetor x lista)
void compararBuscas(const Item mochila[], int qtdVetor, No *lista);

// =================================================================
//                             MAIN
// =================================================================

int main(void) {
    Item mochilaVetor[MAX_ITENS];  // mochila usando vetor (lista sequencial)
    int qtdVetor = 0;              // quantidade de itens no vetor

    No *mochilaLista = NULL;       // mochila usando lista encadeada

    int opcao;

    do {
        printf("\n==============================================\n");
        printf("  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("==============================================\n");
        printf("1. Operar mochila (VETOR)\n");
        printf("2. Operar mochila (LISTA ENCADEADA)\n");
        printf("3. Comparar buscas (vetor x lista)\n");
        printf("0. Sair\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            return 0;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                menuVetor(mochilaVetor, &qtdVetor);
                break;
            case 2:
                menuLista(&mochilaLista);
                break;
            case 3:
                compararBuscas(mochilaVetor, qtdVetor, mochilaLista);
                break;
            case 0:
                printf("\nEncerrando o sistema de mochila...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    liberarLista(&mochilaLista);
    return 0;
}

// =================================================================
//                     FUNÇÕES UTILITÁRIAS
// =================================================================

// Limpa o buffer de entrada (remove o '\n' deixado pelo scanf)
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

// =================================================================
//                        VERSÃO COM VETOR
// =================================================================

// Menu para operar a mochila baseada em vetor
void menuVetor(Item mochila[], int *qtd) {
    int opcao;
    char nome[30];

    do {
        printf("\n========== MOCHILA (VETOR) ==========\n");
        printf("Itens na Mochila: %d/%d\n\n", *qtd, MAX_ITENS);
        printf("1. Inserir novo item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (sequencial)\n");
        printf("5. Ordenar itens por nome\n");
        printf("6. Buscar (binaria)\n");
        printf("0. Voltar ao menu principal\n");
        printf("-------------------------------------\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) return;
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItemVetor(mochila, qtd);
                break;
            case 2:
                removerItemVetor(mochila, qtd);
                break;
            case 3:
                listarItensVetor(mochila, *qtd);
                break;
            case 4:
                printf("\nNome do item para busca sequencial: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                comparacoesSeqVetor = 0;
                {
                    int idx = buscarSequencialVetor(mochila, *qtd, nome);
                    if (idx == -1) {
                        printf("Item nao encontrado no vetor.\n");
                    } else {
                        printf("Item encontrado na posicao %d do vetor.\n", idx);
                        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                               mochila[idx].nome,
                               mochila[idx].tipo,
                               mochila[idx].quantidade);
                    }
                    printf("Comparacoes (busca sequencial vetor): %d\n",
                           comparacoesSeqVetor);
                }
                break;
            case 5:
                ordenarVetor(mochila, *qtd);
                printf("\nVetor ordenado por nome.\n");
                listarItensVetor(mochila, *qtd);
                break;
            case 6:
                printf("\nNome do item para busca binaria: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                ordenarVetor(mochila, *qtd);  // precisa estar ordenado
                comparacoesBinVetor = 0;
                {
                    int idx = buscarBinariaVetor(mochila, *qtd, nome);
                    if (idx == -1) {
                        printf("Item nao encontrado no vetor.\n");
                    } else {
                        printf("Item encontrado na posicao %d do vetor.\n", idx);
                        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                               mochila[idx].nome,
                               mochila[idx].tipo,
                               mochila[idx].quantidade);
                    }
                    printf("Comparacoes (busca binaria vetor): %d\n",
                           comparacoesBinVetor);
                }
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);
}

// Insere novo item no vetor
void inserirItemVetor(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("\nMochila (vetor) cheia! Nao e possivel inserir mais itens.\n");
        return;
    }

    Item novo;
    char buffer[32];

    printf("\n--- INSERIR ITEM (VETOR) ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    fgets(buffer, sizeof(buffer), stdin);
    novo.quantidade = atoi(buffer);

    mochila[*qtd] = novo;
    (*qtd)++;

    printf("\nItem adicionado ao vetor com sucesso.\n");
}

// Remove item do vetor pelo nome (deslocando elementos)
void removerItemVetor(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("\nMochila (vetor) vazia. Nada a remover.\n");
        return;
    }

    char nome[30];
    printf("\n--- REMOVER ITEM (VETOR) ---\n");
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    comparacoesSeqVetor = 0;
    int idx = buscarSequencialVetor(mochila, *qtd, nome);

    if (idx == -1) {
        printf("Item nao encontrado no vetor.\n");
        printf("Comparacoes (busca sequencial vetor): %d\n",
               comparacoesSeqVetor);
        return;
    }

    for (int i = idx; i < *qtd - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*qtd)--;

    printf("Item removido do vetor.\n");
    printf("Comparacoes (busca sequencial vetor): %d\n",
           comparacoesSeqVetor);
}

// Lista itens do vetor
void listarItensVetor(const Item mochila[], int qtd) {
    printf("\n--- ITENS NA MOCHILA (VETOR) ---\n");
    if (qtd == 0) {
        printf("Nenhum item cadastrado.\n");
        return;
    }

    printf("NOME            | TIPO           | QTD\n");
    printf("--------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-15s | %-13s | %d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("--------------------------------------\n");
}

// Busca sequencial no vetor (usa contador global)
int buscarSequencialVetor(const Item mochila[], int qtd, const char *nome) {
    for (int i = 0; i < qtd; i++) {
        comparacoesSeqVetor++;
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Ordena vetor por nome usando Bubble Sort
void ordenarVetor(Item mochila[], int qtd) {
    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < qtd - 1; i++) {
            if (strcmp(mochila[i].nome, mochila[i + 1].nome) > 0) {
                Item tmp = mochila[i];
                mochila[i] = mochila[i + 1];
                mochila[i + 1] = tmp;
                trocou = 1;
            }
        }
    } while (trocou);
}

// Busca binária no vetor (usa contador global)
int buscarBinariaVetor(const Item mochila[], int qtd, const char *nome) {
    int ini = 0, fim = qtd - 1;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comparacoesBinVetor++;

        int cmp = strcmp(nome, mochila[meio].nome);
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

// =================================================================
//                     VERSÃO COM LISTA ENCADEADA
// =================================================================

// Menu para operar mochila baseada em lista encadeada
void menuLista(No **lista) {
    int opcao;
    char nome[30];

    do {
        printf("\n========== MOCHILA (LISTA ENCADEADA) ==========\n");
        printf("1. Inserir novo item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (sequencial)\n");
        printf("0. Voltar ao menu principal\n");
        printf("----------------------------------------------\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) return;
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItemLista(lista);
                break;
            case 2:
                removerItemLista(lista);
                break;
            case 3:
                listarItensLista(*lista);
                break;
            case 4:
                printf("\nNome do item para busca na lista: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                comparacoesSeqLista = 0;
                {
                    No *n = buscarSequencialLista(*lista, nome);
                    if (n == NULL) {
                        printf("Item nao encontrado na lista.\n");
                    } else {
                        printf("Item encontrado na lista.\n");
                        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                               n->dados.nome, n->dados.tipo, n->dados.quantidade);
                    }
                    printf("Comparacoes (busca sequencial lista): %d\n",
                           comparacoesSeqLista);
                }
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);
}

// Insere novo nó na lista (no início)
void inserirItemLista(No **lista) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) {
        printf("Erro ao alocar memoria para a lista.\n");
        return;
    }

    char buffer[32];

    printf("\n--- INSERIR ITEM (LISTA) ---\n");
    printf("Nome do item: ");
    fgets(novo->dados.nome, sizeof(novo->dados.nome), stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(novo->dados.tipo, sizeof(novo->dados.tipo), stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    fgets(buffer, sizeof(buffer), stdin);
    novo->dados.quantidade = atoi(buffer);

    novo->proximo = *lista;
    *lista = novo;

    printf("\nItem adicionado na lista encadeada.\n");
}

// Remove nó da lista pelo nome
void removerItemLista(No **lista) {
    if (*lista == NULL) {
        printf("\nLista encadeada vazia. Nada a remover.\n");
        return;
    }

    char nome[30];
    printf("\n--- REMOVER ITEM (LISTA) ---\n");
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *lista;
    No *anterior = NULL;

    while (atual && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (!atual) {
        printf("Item nao encontrado na lista.\n");
        return;
    }

    if (anterior == NULL) {
        *lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("Item removido da lista encadeada.\n");
}

// Lista itens da lista encadeada
void listarItensLista(const No *lista) {
    printf("\n--- ITENS NA MOCHILA (LISTA) ---\n");
    if (!lista) {
        printf("Nenhum item cadastrado na lista.\n");
        return;
    }

    printf("NOME            | TIPO           | QTD\n");
    printf("--------------------------------------\n");
    while (lista) {
        printf("%-15s | %-13s | %d\n",
               lista->dados.nome,
               lista->dados.tipo,
               lista->dados.quantidade);
        lista = lista->proximo;
    }
    printf("--------------------------------------\n");
}

// Busca sequencial na lista (com contador global)
No* buscarSequencialLista(const No *lista, const char *nome) {
    while (lista) {
        comparacoesSeqLista++;
        if (strcmp(lista->dados.nome, nome) == 0) {
            return (No *)lista;  // remove const para devolver ponteiro editável
        }
        lista = lista->proximo;
    }
    return NULL;
}

// Libera toda a lista encadeada
void liberarLista(No **lista) {
    No *atual = *lista;
    while (atual) {
        No *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    *lista = NULL;
}

// =================================================================
//                 COMPARAÇÃO ENTRE AS DUAS ESTRUTURAS
// =================================================================

// Pede um nome e compara buscas: seq vetor, seq lista, bin vetor
void compararBuscas(const Item mochila[], int qtdVetor, No *lista) {
    if (qtdVetor == 0 || lista == NULL) {
        printf("\nPara comparar buscas, adicione itens no vetor e na lista primeiro.\n");
        return;
    }

    char nome[30];
    printf("\nNome do item critico para comparar as buscas: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // 1) Busca sequencial no vetor
    comparacoesSeqVetor = 0;
    int idxSeqVet = buscarSequencialVetor(mochila, qtdVetor, nome);

    // 2) Busca sequencial na lista
    comparacoesSeqLista = 0;
    No *noLista = buscarSequencialLista(lista, nome);

    // 3) Busca binária no vetor (em uma cópia ordenada)
    Item copia[MAX_ITENS];
    for (int i = 0; i < qtdVetor; i++) {
        copia[i] = mochila[i];
    }
    ordenarVetor(copia, qtdVetor);

    comparacoesBinVetor = 0;
    int idxBinVet = buscarBinariaVetor(copia, qtdVetor, nome);

    printf("\n--- RESULTADOS DA COMPARACAO DE BUSCAS ---\n");
    printf("Busca sequencial (vetor): indice = %d, comparacoes = %d\n",
           idxSeqVet, comparacoesSeqVetor);
    printf("Busca sequencial (lista): encontrado = %s, comparacoes = %d\n",
           noLista ? "sim" : "nao", comparacoesSeqLista);
    printf("Busca binaria   (vetor): indice = %d, comparacoes = %d\n",
           idxBinVet, comparacoesBinVetor);
    printf("-------------------------------------------\n");
}