// main.c
#include "functions.h"

int main() {
    char sequencias[MAX_SEQ][MAX_FINAL_LEN]; // MUDOU AQUI
    char sequencias_inicial[MAX_SEQ][MAX_FINAL_LEN]; // MUDOU AQUI
    char melhor_alinhamento[MAX_SEQ][MAX_FINAL_LEN];
    
    // --- INICIALIZAÇÃO ---
    memset(sequencias, 0, sizeof(sequencias));
    memset(sequencias_inicial, 0, sizeof(sequencias_inicial));
    memset(melhor_alinhamento, 0, sizeof(melhor_alinhamento));
    // ---------------------

    int sequencias_count = 0;
    int max_string_size = 0;
    int final_len = 0;

    char nome_arquivo_base[100];         // Para o nome do arquivo digitado pelo usuário
    char caminho_completo_arquivo[150]; // Para "data/nome_arquivo_base.txt"

    limparTela();
    printf("\n\t******** Alinhamento Multiplo de DNA - Nova Versao ********\n\n");

    // --- Passo 2: Leitura ---
    printf("Digite o nome do arquivo .txt com as sequencias (ex: data.txt): ");
    scanf("%99s", nome_arquivo_base); // Lê apenas o nome base do arquivo
    // Constrói o caminho completo para o arquivo dentro da pasta "data"
    sprintf(caminho_completo_arquivo, "data/%s", nome_arquivo_base);

    sequencias_count = lerSequencias(caminho_completo_arquivo, sequencias, &max_string_size); // Usa o caminho completo

    if (sequencias_count <= 0) {
        // Informa o caminho completo na mensagem de erro
        printf("Erro ao ler o arquivo '%s' ou nenhuma sequencia valida encontrada.\n", caminho_completo_arquivo); 
        return 1;
    }

    if (sequencias_count < 2) {
         printf("Sao necessarias pelo menos 2 sequencias para o alinhamento.\n");
         return 1;
    }

    printf("\nLidas %d sequencias. Tamanho maximo: %d\n", sequencias_count, max_string_size);

    // Copia para o alinhamento inicial
    for(int i = 0; i < sequencias_count; i++) {
        strcpy(sequencias_inicial[i], sequencias[i]);
    }


    // --- Passo 3: Gaps Iniciais ---
    printf("\nAlinhamento com gaps no inicio:\n\n");
    preencheGapInicial(sequencias_inicial, sequencias_count, max_string_size);
    imprimirSequencia(sequencias_inicial, sequencias_count, max_string_size); // Usar max_string_size aqui
    printf("\nSCORE INICIAL:\n");
    calcular_score(sequencias_inicial, sequencias_count, max_string_size); // Usar max_string_size aqui


    //     // --- DIAGNÓSTICO ---
    // printf("\nVerificando sequencias ANTES de alinhar:\n");
    // for(int i = 0; i < sequencias_count; i++) {
    //     printf("Seq %d: %s (len: %zu)\n", i, sequencias[i], strlen(sequencias[i]));
    // }
    // // -------------------

      // *** ADICIONE ESTAS LINHAS ***
    int lengths[MAX_SEQ];
    for(int i = 0; i < sequencias_count; i++) {
        lengths[i] = strlen(sequencias[i]);
    }
    // ***************************


    // --- Passo 4: Melhor Alinhamento ---
    printf("\nCalculando melhor alinhamento...\n");
    // alinhaSequenciasGreedy(sequencias, sequencias_count, max_string_size, melhor_alinhamento, &final_len);
    alinhaComArvore(sequencias, lengths, sequencias_count, melhor_alinhamento, &final_len);

    // --- Passo 5: Score Final ---
    printf("\nMelhor alinhamento encontrado:\n\n");
    imprimirSequencia(melhor_alinhamento, sequencias_count, final_len);
    printf("\nSCORE FINAL:\n");
    calcular_score(melhor_alinhamento, sequencias_count, final_len);

    printf("\n\n");

    return 0;
}