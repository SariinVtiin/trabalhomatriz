#include <stdio.h>
#include <stdlib.h>

typedef struct Celula {
    struct Celula *direita, *abaixo;
    int linha, coluna;
    double valor;
} Celula;

typedef struct {
    int m, n; // dimensão da matriz (m x n)
    Celula *inicio, *fimLinha, *fimColuna;
} Matriz;

void CabecaPrincipal(Matriz *pmat) {
    Celula *temp = (Celula *)malloc(sizeof(Celula));

    if (temp == NULL) {
        printf("**Erro: Não há espaço para alocar memória**");
        exit(1);
    }

    temp->linha = -1;
    temp->coluna = -1;
    temp->direita = temp;
    temp->abaixo = temp;

    pmat->inicio = temp;
    pmat->fimLinha = temp;
    pmat->fimColuna = pmat->fimLinha;
}

void FLinhaVazia(Matriz *pmat) {
    Celula *temp = (Celula *)malloc(sizeof(Celula));

    if (temp == NULL) {
        printf("**Erro: Não há espaço para alocar memória**");
        exit(1);
    }

    temp->linha = -1;
    temp->coluna = 0;

    pmat->fimLinha->abaixo = temp;
    pmat->fimLinha = temp;

    temp->abaixo = pmat->inicio;
    temp->direita = temp;
}

void FColunaVazia(Matriz *pmat) {
    Celula *temp = (Celula *)malloc(sizeof(Celula));

    if (temp == NULL) {
        printf("**Erro: Não há espaço para alocar memória**");
        exit(1);
    }

    temp->linha = 0;
    temp->coluna = -1;

    pmat->fimColuna->direita = temp;
    pmat->fimColuna = temp;

    temp->direita = pmat->inicio;
    temp->abaixo = temp;
}

void Insere(Matriz *pmat, int i, int j, double elem) {
    Celula *auxL, *auxC, *temp;
    int count;

    temp = (Celula *)malloc(sizeof(Celula));

    if (temp == NULL) {
        printf("**Erro: Não há espaço para alocar memória**");
        exit(1);
    }

    temp->linha = i;
    temp->coluna = j;
    temp->valor = elem;

    auxL = pmat->inicio->abaixo;

    for (count = 1; count <= (i - 1); count++)
        auxL = auxL->abaixo;

    for (count = 1; count <= j; count++) {
        if (temp->coluna < auxL->direita->direita->coluna) {
            temp->direita = auxL->direita->direita;
            auxL->direita->direita = temp;
            break;
        } else if (auxL->direita->direita->linha == -1) {
            temp->direita = auxL->direita->direita;
            auxL->direita->direita = temp;
            break;
        } else {
            auxL = auxL->direita;
        }
    }

    auxC = pmat->inicio->direita;

    for (count = 1; count <= (j - 1); count++)
        auxC = auxC->direita;

    for (count = 1; count <= i; count++) {
        if (temp->linha < auxC->abaixo->abaixo->linha) {
            temp->abaixo = auxC->abaixo->abaixo;
            auxC->abaixo->abaixo = temp;
            break;
        } else if (auxC->abaixo->abaixo->coluna == -1) {
            temp->abaixo = auxC->abaixo->abaixo;
            auxC->abaixo->abaixo = temp;
            break;
        } else {
            auxC = auxC->abaixo;
        }
    }
}

void CriaMatriz(Matriz *MAux, int i, int j) {
    int count;

    CabecaPrincipal(MAux);

    MAux->m = i;
    MAux->n = j;

    for (count = 1; count <= i; count++)
        FLinhaVazia(MAux);

    for (count = 1; count <= j; count++)
        FColunaVazia(MAux);
}

void LeMatriz(Matriz *pmat, FILE *dados) {
    int i, j;
    double valor;
    char tmp[BUFSIZ];

    fgets(tmp, BUFSIZ, dados);
    sscanf(tmp, "%d %d", &pmat->m, &pmat->n);

    CriaMatriz(pmat, pmat->m, pmat->n);

    while ((!feof(dados)) && (strcmp(tmp, "\n"))) {
        fgets(tmp, BUFSIZ, dados);
        sscanf(tmp, "%d %d %lf", &i, &j, &valor);
        Insere(pmat, i, j, valor);
    }
}

void ImprimeMatriz(Matriz *pmat) {
    int i, j;
    Celula *auxL = (Celula *)malloc(sizeof(Celula));

    auxL = pmat->inicio->abaixo;

    for (i = 1; i <= pmat->m; i++) {
        printf("|");

        for (j = 1; j <= pmat->n; j++) {
            if (i == auxL->direita->linha && j == auxL->direita->coluna) {
                printf("\t%0.1lf ", auxL->direita->valor);
                auxL = auxL->direita;
            } else {
                printf("\t0 ");
            }
        }

        printf("\t|\n");
        auxL = auxL->direita->abaixo;
    }

    printf("\n");
}

int Soma(Matriz *pC, Matriz *pA, Matriz *pB) {
    double soma;
    int x, y;
    Celula *auxA = (Celula *)malloc(sizeof(Celula));
    Celula *auxB = (Celula *)malloc(sizeof(Celula));

    if (pA->m != pB->m || pA->n != pB->n)
        return 0;

    CriaMatriz(pC, pA->m, pA->n);

    auxA = pA->inicio->abaixo;
    auxB = pB->inicio->abaixo;

    for (x = 1; x <= pA->m; x++) {
        for (y = 1; y <= pA->n; y++) {
            if (y == auxA->direita->direita->coluna && y == auxB->direita->direita->coluna) {
                soma = auxA->direita->direita->valor + auxB->direita->direita->valor;

                if (soma != 0)
                    Insere(pC, x, y, soma);

                auxA = auxA->direita;
                auxB = auxB->direita;
            } else if (auxA->direita->direita->coluna == y) {
                Insere(pC, x, y, auxA->direita->direita->valor);
                auxA = auxA->direita;
            } else if (auxB->direita->direita->coluna == y) {
                Insere(pC, x, y, auxB->direita->direita->valor);
                auxB = auxB->direita;
            }
        }

        auxA = auxA->direita->abaixo;
        auxB = auxB->direita->abaixo;
    }

    return 1;
}

int Multiplica(Matriz *pC, Matriz *pA, Matriz *pB) {
    int i, j, k;
    double total = 0;
    Celula *auxA, *auxB;

    if (pA->n != pB->m)
        return 0;

    auxA = (Celula *)malloc(sizeof(Celula));
    auxB = (Celula *)malloc(sizeof(Celula));

    CriaMatriz(pC, pA->m, pB->n);

    auxA = pA->inicio->abaixo;
    auxB = pB->inicio->direita;

    for (i = 1; i <= pA->m; i++) {
        if (auxA->direita != auxA && auxA->abaixo != auxA) {
            for (j = 1; j <= pB->n; j++) {
                if (auxB->direita != auxB && auxB->abaixo != auxB) {
                    for (k = 1; k <= pB->n; k++) {
                        if (auxA->direita->coluna == k || auxB->abaixo->linha == k) {
                            if (auxA->direita->coluna == k && auxB->abaixo->linha == k) {
                                total += auxA->direita->valor * auxB->abaixo->valor;

                                auxA = auxA->direita;
                                auxB = auxB->abaixo;
                            } else {
                                if (auxA->direita->coluna == k)
                                    auxA = auxA->direita;
                                else if (auxB->abaixo->linha == k)
                                    auxB = auxB->abaixo;
                            }
                        }
                    }
                }

                auxB = auxB->abaixo->direita;
            }

            if (total != 0)
                Insere(pC, i, j, total);

            total = 0;
            auxB = auxB->abaixo->direita;
        }

        auxA = auxA->direita->abaixo;
    }

    return 1;
}

int main() {
    Matriz A, B, C;
    FILE *dados;

    dados = fopen("Data.txt", "r");

    if (!dados) {
        printf("**Erro na abertura de arquivo. Fim do programa**");
        exit(1);
    }

    LeMatriz(&A, dados);
    printf("Matriz A:\n");
    ImprimeMatriz(&A);

    LeMatriz(&B, dados);
    printf("Matriz B:\n");
    ImprimeMatriz(&B);

    if (Soma(&C, &A, &B)) {
        printf("Matriz A + B:\n");
        ImprimeMatriz(&C);
    } else
        printf("Impossivel A + B\n");

    if (Multiplica(&C, &A, &A)) {
        printf("Matriz A x A:\n");
        ImprimeMatriz(&C);
    } else
        printf("Impossível A x A\n");

    LeMatriz(&B, dados);
    printf("Nova matriz B:\n");
    ImprimeMatriz(&B);

    if (Soma(&C, &A, &B)) {
        printf("Matriz A+B:\n");
        ImprimeMatriz(&C);
    } else
        printf("Impossivel A+B\n");

    if (Multiplica(&C, &A, &B)) {
        printf("Matriz A x B:\n");
        ImprimeMatriz(&C);
    } else
        printf("Impossivel A x B\n");

    if (Multiplica(&C, &B, &B)) {
        printf("Matriz B x B:\n");
        ImprimeMatriz(&C);
    } else
        printf("Impossivel B x B\n");

    printf("Matriz A:\n");
    ImprimeMatriz(&A);
    printf("Matriz B:\n");
    ImprimeMatriz(&B);

    fclose(dados);
    system("PAUSE");
    return 0;
}