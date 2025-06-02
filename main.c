#include "functions.h"

int main() {
    char sequencias[MAX_SEQ][MAX_FINAL_LEN];
    char sequencias_inicial[MAX_SEQ][MAX_FINAL_LEN];
    char melhor_alinhamento[MAX_SEQ][MAX_FINAL_LEN];
    
    memset(sequencias, 0, sizeof(sequencias));
    memset(sequencias_inicial, 0, sizeof(sequencias_inicial));
    memset(melhor_alinhamento, 0, sizeof(melhor_alinhamento));
  
    int sequencias_count = 0;
    int max_string_size = 0;
    int final_len = 0;
    int lengths[MAX_SEQ];

    char nome_arquivo_base[100];
    char caminho_completo_arquivo[150];

    limparTela();
    printf("\n\t******** Alinhamento Multiplo de DNA - Nova Versao ********\n\n");

    printf("Digite o nome do arquivo .txt com as sequencias (ex: data.txt): ");
    scanf("%99s", nome_arquivo_base); 

    sprintf(caminho_completo_arquivo, "data/%s", nome_arquivo_base);

    sequencias_count = lerSequencias(caminho_completo_arquivo, sequencias, &max_string_size);

    if (sequencias_count <= 0) {
        printf("Erro ao ler o arquivo '%s' ou nenhuma sequencia valida encontrada.\n", caminho_completo_arquivo); 
        return 1;
    }
    if (sequencias_count < 2) {
         printf("Sao necessarias pelo menos 2 sequencias para o alinhamento.\n");
         return 1;
    }

    printf("\nLidas %d sequencias. Tamanho maximo: %d\n", sequencias_count, max_string_size);

    for(int i = 0; i < sequencias_count; i++) {
        lengths[i] = strlen(sequencias[i]);
    }

    for(int i = 0; i < sequencias_count; i++) {
        strcpy(sequencias_inicial[i], sequencias[i]);
    }

    // --- Melhor Alinhamento ---
    printf("\nCalculando melhor alinhamento...\n");
    alinharComReferenciaNW(sequencias, lengths, sequencias_count, melhor_alinhamento, &final_len);

    printf("\nMelhor alinhamento encontrado:\n\n");
    imprimirSequencia(melhor_alinhamento, sequencias_count, final_len);
    
    printf("\nSCORE FINAL:\n");
    calcular_score(melhor_alinhamento, sequencias_count, final_len);

    printf("\n\n");

    return 0;
}