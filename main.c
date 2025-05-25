#include "functions.h"

int main() {
    srand(time(NULL));

    printf("Usando pesos predefinidos: ALPHA=%d, BETA=%d, DELTA=%d\n", ALPHA, BETA, DELTA);

    char sequencias_orig[MAX_SEQS][MAX_LEN_ALN];
    int n_sequencias = 0;
    int max_len_inicial = 0;
    int orig_lengths[MAX_SEQS];
    char filename[100];

    printf("Digite o nome do arquivo .txt com as sequencias (ex: dna.txt): ");
    scanf("%99s", filename);

    if (lerSequencias(filename, sequencias_orig, &n_sequencias, &max_len_inicial, orig_lengths) != 0) {
        return 1;
    }

    printf("\nSequencias Lidas:\n");
    for(int i = 0; i < n_sequencias; i++) {
        printf("%d (%d): %s\n", i+1, orig_lengths[i], sequencias_orig[i]);
    }

    int max_len_alinhado = max_len_inicial + (max_len_inicial / 3);
    if (max_len_alinhado >= MAX_LEN_ALN) max_len_alinhado = MAX_LEN_ALN - 1;
    printf("Tamanho maximo estimado para alinhamento: %d\n", max_len_alinhado);

    // --- Construção da Árvore (Guloso) ---
    No* raiz = criaNovoNo("RAIZ", NULL, -1, 0.0);
    No* pai_atual = raiz;
    int col_final = 0; // Para saber onde realmente parou

    printf("\nIniciando construcao da arvore (Guloso)...\n");

    for (int col = 0; col < max_len_alinhado; col++) {
        gerarVariacoes(pai_atual, n_sequencias, sequencias_orig, orig_lengths, col);

        No* melhor_filho = devolveMelhorFilho(pai_atual);

        if (melhor_filho == NULL) {
            printf("Fim da arvore na coluna %d (sem filhos validos).\n", col);
            col_final = col; // Guarda a última coluna válida
            break;
        }

        pai_atual = melhor_filho;
        col_final = col + 1; // Guarda a próxima coluna (que será o tamanho)
    }

    // Se o loop terminou por chegar ao fim, col_final é max_len_alinhado
    if (col_final == max_len_alinhado) {
       printf("Atingiu o tamanho maximo estimado (%d).\n", max_len_alinhado);
    }
    
    // --- Resultado ---
    No* melhor_no = pai_atual; // No guloso, o último é o melhor

    if (melhor_no && melhor_no != raiz) {
        char alinhamento_final[MAX_SEQS][MAX_LEN_ALN];
        // Usa col_final como o tamanho real do alinhamento
        reconstruirAlinhamento(melhor_no, alinhamento_final, n_sequencias, col_final); 
        printf("\nMelhor Alinhamento Encontrado (Score: %.2f):\n", melhor_no->score);
        imprimirAlinhamento(alinhamento_final, n_sequencias);
    } else {
        printf("Nao foi possivel encontrar um alinhamento.\n");
    }

    // --- Limpeza ---
    printf("\nLiberando memoria...\n");
    liberaArvore(raiz);
    printf("Memoria liberada.\n");

    printf("\nPrograma finalizado.\n");

    return 0;
}