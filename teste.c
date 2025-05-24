#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <float.h> // Para DBL_MIN

// --- Constantes ---
#define MAX_SEQS 50   // Limite para ABN2
#define MAX_LEN_INIT 100 // Limite inicial das sequências
#define MAX_LEN_ALN 150  // Estimativa do tamanho máximo alinhado

// --- Variáveis Globais (Calcular no Início) ---
float g_alpha = 1.0;
float g_beta = 0.0;
float g_delta = -1.0;
int g_gap_pref = 0; // 0: início, 1: fim, 2: meio (Ainda não usado nesta versão)

// --- Estrutura do Nó da Árvore ---
typedef struct No {
    char varSeq[MAX_SEQS + 1]; // A coluna representada por este nó
    struct No *primogenito;    // Primeiro filho
    struct No *irmao;          // Próximo irmão
    struct No *pai;            // Ponteiro para o pai (útil para reconstruir)
    double score_acumulado;    // Score total do alinhamento até este nó
    int coluna_index;          // A qual coluna este nó pertence
} No;



// --- Funções Auxiliares (Cálculo de Pesos) ---
int get_value(char c) {
    c = tolower(c);
    switch (c) {
        case 'q': return 1; case 'w': return 2; case 'e': return 3;
        case 'r': return 4; case 't': return 5; case 'y': return 6;
        case 'u': return 7; case 'i': return 8; case 'o': return 9;
        case 'p': return 10; case 'a': case 'á': case 'ã': return 11;
        case 's': return 9; case 'd': return 7; case 'f': return 5;
        case 'g': return 3; case 'h': return 1; case 'j': return 11;
        case 'k': return 9; case 'l': return 7; case 'ç': return 5;
        case 'é': return 3; case 'í': return 1; case 'z': return 2;
        case 'x': return 4; case 'c': return 6; case 'v': return 8;
        case 'b': return 10; case 'n': return 12; case 'm': return 2;
        case 'ó': return 4; case 'õ': return 6; case 'ô': return 8;
        case 'â': return 10; case 'ê': return 12;
        default: return 0;
    }
}

void calculate_weights(const char *first_name, const char *last_name) {
    int sum1 = 0, sum2 = 0;
    for (int i = 0; first_name[i] != '\0'; i++) sum1 += get_value(first_name[i]);
    for (int i = 0; last_name[i] != '\0'; i++) sum2 += get_value(last_name[i]);

    int resto1 = sum1 % 3;
    int resto2 = sum2 % 3;

    if (resto1 == 0) { g_alpha = +1; g_beta = 0; g_delta = -1; }
    else if (resto1 == 1) { g_alpha = +2; g_beta = 0; g_delta = -1; }
    else { g_alpha = +1; g_beta = 0; g_delta = -2; }

    g_gap_pref = resto2;
    printf("Pesos Calculados: α = %.0f, β = %.0f, δ = %.0f\n", g_alpha, g_beta, g_delta);
    printf("Preferência de Gap: %d (%s)\n", g_gap_pref,
           g_gap_pref == 0 ? "Início" : (g_gap_pref == 1 ? "Fim" : "Meio"));
}

// --- Funções da Árvore N-ária ---
No* criaNovoNo(const char seq[], No* pai_no, int col_idx, double score_col) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        perror("Erro de alocação no criaNovoNo");
        exit(1);
    }
    strcpy(novo->varSeq, seq);
    novo->primogenito = NULL;
    novo->irmao = NULL;
    novo->pai = pai_no;
    novo->coluna_index = col_idx;
    novo->score_acumulado = (pai_no ? pai_no->score_acumulado : 0) + score_col;
    return novo;
}

void inserirFilho(No* pai, No* filho) {
    if (!pai || !filho) return;

    if (!pai->primogenito) {
        pai->primogenito = filho;
    } else {
        No* p = pai->primogenito;
        while (p->irmao) {
            p = p->irmao;
        }
        p->irmao = filho;
    }
}

void liberaArvore(No* raiz) {
    if (!raiz) return;
    liberaArvore(raiz->primogenito);
    liberaArvore(raiz->irmao);
    // printf("Liberando no da coluna %d: %s\n", raiz->coluna_index, raiz->varSeq); // Para debug
    free(raiz);
}

// --- Funções de Alinhamento e Score ---
double calcScoreColuna(const char vet[], int nSeq) {
    double score_col = 0;
    for (int i = 0; i < nSeq - 1; i++) {
        for (int j = i + 1; j < nSeq; j++) {
            char char1 = vet[i];
            char char2 = vet[j];

            if (char1 == '-' && char2 == '-') {
                score_col += 0; // Penalidade gap-gap = 0
            } else if (char1 == '-' || char2 == '-') {
                score_col += g_delta;
            } else if (char1 == char2) {
                score_col += g_alpha;
            } else {
                score_col += g_beta;
            }
        }
    }
    return score_col;
}

// GERA VARIAÇÕES (Corrigido para usar tamanhos originais e forçar bases)
void gerarVariacoes(No* pai, int nSeq, char orig_seq[][MAX_LEN_ALN], int orig_lengths[], int col_idx, int max_len) {
    char coluna_base[MAX_SEQS + 1];
    int tem_base = 0; // Flag para verificar se há alguma base (não-gap)

    // 1. Constrói a coluna base a partir das sequências ORIGINAIS
    for(int i = 0; i < nSeq; i++) {
       if (col_idx < orig_lengths[i]) {
           coluna_base[i] = orig_seq[i][col_idx]; // Usa a base original
           tem_base = 1;
       } else {
           coluna_base[i] = '-'; // Usa gap se já passou do fim original
       }
    }
    coluna_base[nSeq] = '\0';

    // Se a coluna base tem pelo menos uma base:
    if (tem_base) {
        // 2a. Adiciona a coluna base como variação
        double score = calcScoreColuna(coluna_base, nSeq);
        No* filho = criaNovoNo(coluna_base, pai, col_idx, score);
        inserirFilho(pai, filho);

        // 2b. Adiciona variações com 1 gap (onde era base)
        for (int k = 0; k < nSeq; k++) {
            if (coluna_base[k] != '-') { // Só insere gap se era base
                char coluna_gap[MAX_SEQS + 1];
                strcpy(coluna_gap, coluna_base);
                coluna_gap[k] = '-';
                double score_gap = calcScoreColuna(coluna_gap, nSeq);
                No* filho_gap = criaNovoNo(coluna_gap, pai, col_idx, score_gap);
                inserirFilho(pai, filho_gap);
            }
        }
        // 2c. ADICIONA a coluna 'só-gaps' como opção MESMO se tiver base
        //     Isso é importante para permitir que o algoritmo insira colunas de gaps
        //     se for realmente a melhor opção (mas ele não será forçado a isso).
        char coluna_so_gap[MAX_SEQS+1];
        for(int i = 0; i < nSeq; i++) coluna_so_gap[i] = '-';
        coluna_so_gap[nSeq] = '\0';
        double score_so_gap = calcScoreColuna(coluna_so_gap, nSeq);
        No* filho_so_gap = criaNovoNo(coluna_so_gap, pai, col_idx, score_so_gap);
        inserirFilho(pai, filho_so_gap);

    } else {
        // Se a coluna base já é só gaps (porque passamos todas as sequências),
        // a única opção é continuar com 'só-gaps'.
        double score = calcScoreColuna(coluna_base, nSeq); // Score será 0
        No* filho = criaNovoNo(coluna_base, pai, col_idx, score);
        inserirFilho(pai, filho);
    }
}

// Devolve o MELHOR filho (abordagem Gulosa)
No* devolveMelhorFilho(No* pai) {
    if (!pai || !pai->primogenito) return NULL;

    No* filho_atual = pai->primogenito;
    No* melhor_filho = filho_atual;
    double max_score = filho_atual->score_acumulado;

    while (filho_atual->irmao) {
        filho_atual = filho_atual->irmao;
        if (filho_atual->score_acumulado > max_score) {
            max_score = filho_atual->score_acumulado;
            melhor_filho = filho_atual;
        }
    }
    return melhor_filho;
}

void reconstruirAlinhamento(No* no_final, char alinhamento_final[][MAX_LEN_ALN], int nSeq, int max_len) {
    if (!no_final) return;

    No* no_atual = no_final;
    int col_atual = no_final->coluna_index;

    // Limpa a matriz final com um caractere placeholder ou gap
    for(int i=0; i<nSeq; i++) {
        for(int j=0; j<max_len; j++) {
            alinhamento_final[i][j] = '-'; // Preenche tudo com gap inicialmente
        }
         alinhamento_final[i][max_len] = '\0';
    }

    // Preenche da direita para a esquerda com os dados da árvore
    while (no_atual != NULL && no_atual->pai != NULL && col_atual >= 0) {
        for (int i = 0; i < nSeq; i++) {
            alinhamento_final[i][col_atual] = no_atual->varSeq[i];
        }
        col_atual--;
        no_atual = no_atual->pai;
    }
    
    // Garante que o tamanho está correto (caso a árvore pare antes)
    for(int i=0; i<nSeq; i++) {
        alinhamento_final[i][max_len] = '\0';
    }
}

// --- Em lerSequencias ---
void lerSequencias(char seq[][MAX_LEN_ALN], int *nSeq, int *maxLen, int orig_lengths[]) { // Adiciona orig_lengths
    int count;
    printf("Digite o numero de sequencias (2 a %d): ", MAX_SEQS);
    scanf("%d", &count);
    while (count < 2 || count > MAX_SEQS) {
        printf("Invalido. Digite o numero de sequencias (2 a %d): ", MAX_SEQS);
        scanf("%d", &count);
    }
    *nSeq = count;
    *maxLen = 0;

    printf("Digite as %d sequencias (apenas A, C, G, T, max %d):\n", *nSeq, MAX_LEN_INIT);
    for (int i = 0; i < *nSeq; i++) {
        printf("%d: ", i + 1);
        scanf("%s", seq[i]);
        // TODO: Adicionar validação de caracteres e tamanho
        int len = strlen(seq[i]);
        orig_lengths[i] = len; // <-- GUARDA O TAMANHO ORIGINAL
        if (len > *maxLen) {
            *maxLen = len;
        }
    }
}

void preencherGapsIniciais(char seq[][MAX_LEN_ALN], int nSeq, int maxLen, int maxLenAln) {
    for (int i = 0; i < nSeq; i++) {
        int len_atual = strlen(seq[i]);
        for (int j = len_atual; j < maxLenAln; j++) {
            seq[i][j] = '-'; // Preenche com Gaps
        }
        seq[i][maxLenAln] = '\0'; // Garante terminação
    }
}

void imprimirAlinhamento(char seq[][MAX_LEN_ALN], int nSeq) {
    if (nSeq <= 0) return;
    int len = strlen(seq[0]);
    for (int i = 0; i < nSeq; i++) {
        printf("%.*s\n", len, seq[i]); // Imprime até o tamanho do primeiro
    }
}

// --- Em main ---
int main() {
    // ... (cálculo de pesos) ...

    char sequencias_orig[MAX_SEQS][MAX_LEN_ALN];
    int n_sequencias = 0;
    int max_len_inicial = 0;
    int orig_lengths[MAX_SEQS]; // <-- DECLARA ARRAY PARA TAMANHOS

    lerSequencias(sequencias_orig, &n_sequencias, &max_len_inicial, orig_lengths); // <-- PASSA PARA LER

    int max_len_alinhado = max_len_inicial + (max_len_inicial / 3); // Ajuste: 33% de gaps
    if (max_len_alinhado > MAX_LEN_ALN) max_len_alinhado = MAX_LEN_ALN -1;

    // --- Construção da Árvore (Guloso) ---
    No* raiz = criaNovoNo("RAIZ", NULL, -1, 0.0);
    No* pai_atual = raiz;

    printf("\nIniciando construção da arvore (Guloso)...\n");

    for (int col = 0; col < max_len_alinhado; col++) {
        // Passa sequencias_orig e orig_lengths AGORA!
        gerarVariacoes(pai_atual, n_sequencias, sequencias_orig, orig_lengths, col, max_len_alinhado);

        No* melhor_filho = devolveMelhorFilho(pai_atual);

        if (melhor_filho == NULL) {
            printf("Fim da arvore na coluna %d (sem filhos validos).\n", col);
            max_len_alinhado = col;
            break;
        }

        pai_atual = melhor_filho;
    }
    
    // ... (Resto do main - Reconstruir, Imprimir, Liberar) ...
    // É importante que reconstruirAlinhamento e imprimirAlinhamento 
    // usem o 'max_len_alinhado' final.
    No* melhor_no = pai_atual; 

    if (melhor_no && melhor_no != raiz) {
        char alinhamento_final[MAX_SEQS][MAX_LEN_ALN];
        // É crucial que max_len_alinhado seja o tamanho final real.
        reconstruirAlinhamento(melhor_no, alinhamento_final, n_sequencias, max_len_alinhado);
        printf("\nMelhor Alinhamento Encontrado (Score: %.2f):\n", melhor_no->score_acumulado);
        imprimirAlinhamento(alinhamento_final, n_sequencias);
    } else {
        printf("Não foi possível encontrar um alinhamento.\n");
    }

    liberaArvore(raiz);
    printf("\nMemória liberada.\n");

    return 0;
}
