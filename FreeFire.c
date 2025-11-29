#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10  // capacidade máxima da mochila

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

void inserirItem(Item inventario[], int *qtdItens);
void removerItem(Item inventario[], int *qtdItens);
void listarItens(const Item inventario[], int qtdItens);
void buscarItem(const Item inventario[], int qtdItens);
int  encontrarIndiceItem(const Item inventario[], int qtdItens, const char *nomeBuscado);
void limparBufferEntrada(void);

int main() {
    Item inventario[MAX_ITENS];
    int qtdItens = 0;
    int opcao;

    printf("=== SISTEMA DE INVENTARIO - MOCHILA INICIAL ===\n");

    do {
        printf("\n---- MENU DE OPCOES ----\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Buscar item\n");
        printf("4 - Listar itens\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItem(inventario, &qtdItens);
                listarItens(inventario, qtdItens);
                break;
            case 2:
                removerItem(inventario, &qtdItens);
                listarItens(inventario, qtdItens);
                break;
            case 3:
                buscarItem(inventario, qtdItens);
                listarItens(inventario, qtdItens);
                break;
            case 4:
                listarItens(inventario, qtdItens);
                break;
            case 0:
                printf("\nSaindo do sistema de inventario...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

void inserirItem(Item inventario[], int *qtdItens) {
    if (*qtdItens >= MAX_ITENS) {
        printf("\nMochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;
    char buffer[32];

    printf("\n--- INSERIR NOVO ITEM ---\n");

    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo do item (arma, municao, cura, etc): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    fgets(buffer, sizeof(buffer), stdin);
    novo.quantidade = atoi(buffer);

    inventario[*qtdItens] = novo;  // copia struct
    (*qtdItens)++;                 // ATUALIZA contador

    printf("\nItem inserido com sucesso! Agora ha %d item(ns) na mochila.\n", *qtdItens);
}

void removerItem(Item inventario[], int *qtdItens) {
    if (*qtdItens == 0) {
        printf("\nA mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indice = encontrarIndiceItem(inventario, *qtdItens, nomeBusca);
    if (indice == -1) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    // desloca elementos
    for (int i = indice; i < *qtdItens - 1; i++) {
        inventario[i] = inventario[i + 1];
    }
    (*qtdItens)--;

    printf("\nItem removido com sucesso! Agora ha %d item(ns) na mochila.\n", *qtdItens);
}

void listarItens(const Item inventario[], int qtdItens) {
    printf("\n=== ITENS NA MOCHILA ===\n");

    if (qtdItens == 0) {
        printf("Nenhum item cadastrado.\n");
        return;
    }

    for (int i = 0; i < qtdItens; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1,
               inventario[i].nome,
               inventario[i].tipo,
               inventario[i].quantidade);
    }
}

void buscarItem(const Item inventario[], int qtdItens) {
    if (qtdItens == 0) {
        printf("\nA mochila esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCAR ITEM ---\n");
    printf("Digite o nome do item que deseja procurar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indice = encontrarIndiceItem(inventario, qtdItens, nomeBusca);

    if (indice == -1) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeBusca);
    } else {
        printf("\nItem encontrado!\n");
        printf("Nome      : %s\n", inventario[indice].nome);
        printf("Tipo      : %s\n", inventario[indice].tipo);
        printf("Quantidade: %d\n", inventario[indice].quantidade);
    }
}

int encontrarIndiceItem(const Item inventario[], int qtdItens, const char *nomeBuscado) {
    for (int i = 0; i < qtdItens; i++) {
        if (strcmp(inventario[i].nome, nomeBuscado) == 0) {
            return i;
        }
    }
    return -1;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}
