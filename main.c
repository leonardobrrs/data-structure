#include "functions.h"
#include <errno.h>

int main() {
    NoSequencia *lista_de_sequencias = NULL;
    int sequencias_count = 0;
    int max_string_size_original = 0;
    char nome_arquivo_base[100];
    char caminho_completo_arquivo[150];

    char msa_final_resultado[MAX_SEQ][MAX_FINAL_LEN];
    int comprimento_final_msa = 0;

    memset(msa_final_resultado, 0, sizeof(msa_final_resultado));

    limparTela();
    printf("\n\t******** Alinhamento Multiplo de DNA - Nova Versao ********\n\n");

    printf("Digite o nome do arquivo .txt com as sequencias (ex: data.txt): ");
    scanf("%99s", nome_arquivo_base);
    sprintf(caminho_completo_arquivo, "data/%s", nome_arquivo_base);

    lista_de_sequencias = lerSequenciasParaLista(caminho_completo_arquivo, &sequencias_count, &max_string_size_original);

    if (lista_de_sequencias == NULL && sequencias_count == 0) {
        printf("Erro ao ler o arquivo '%s' ou nenhuma sequencia valida encontrada.\n", caminho_completo_arquivo);
        return 1;
    }
    if (sequencias_count == 0) {
        printf("Nenhuma sequencia valida encontrada em '%s'.\n", caminho_completo_arquivo);
        liberarListaSequencias(lista_de_sequencias);
        return 1;
    }
    if (sequencias_count < 2) {
         printf("Sao necessarias pelo menos 2 sequencias para o alinhamento. Lidas: %d.\n", sequencias_count);
         liberarListaSequencias(lista_de_sequencias);
         return 1;
    }

    printf("\nLidas %d sequencias. Tamanho maximo original: %d\n", sequencias_count, max_string_size_original);

    DetalheSequencia *conjunto_detalhes = NULL;
    conjunto_detalhes = (DetalheSequencia *)malloc(sequencias_count * sizeof(DetalheSequencia));
    if (conjunto_detalhes == NULL) {
        perror("Falha ao alocar memoria para conjunto_detalhes");
        liberarListaSequencias(lista_de_sequencias);
        return 1;
    }

    NoSequencia *no_atual_lista = lista_de_sequencias;
    for (int i = 0; i < sequencias_count; i++) {
        if (no_atual_lista == NULL) {
            fprintf(stderr, "Erro: contagem de sequencias inconsistente com a lista encadeada durante a populacao.\n");
            for (int k = 0; k < i; k++) {
                if(conjunto_detalhes[k].texto_seq != NULL) free(conjunto_detalhes[k].texto_seq);
            }
            free(conjunto_detalhes);
            liberarListaSequencias(lista_de_sequencias);
            return 1;
        }
        
        conjunto_detalhes[i].texto_seq = (char *)malloc(no_atual_lista->comprimento + 1);
        if (conjunto_detalhes[i].texto_seq == NULL) {
            perror("Falha ao alocar memoria para texto_seq em DetalheSequencia");
            for (int k = 0; k < i; k++) {
                 if(conjunto_detalhes[k].texto_seq != NULL) free(conjunto_detalhes[k].texto_seq);
            }
            free(conjunto_detalhes);
            liberarListaSequencias(lista_de_sequencias);
            return 1;
        }
        strcpy(conjunto_detalhes[i].texto_seq, no_atual_lista->textoDaSequencia);
        conjunto_detalhes[i].tamanho_original = no_atual_lista->comprimento;
        conjunto_detalhes[i].indice_antes_ordenar = no_atual_lista->indice_original_leitura;
        
        no_atual_lista = no_atual_lista->proximaSeq;
    }

    liberarListaSequencias(lista_de_sequencias);
    lista_de_sequencias = NULL;

    printf("\nArray de DetalheSequencia populado. Ordenando...\n");
    qsort(conjunto_detalhes, sequencias_count, sizeof(DetalheSequencia), compararPorTamanhoDesc);

    printf("\nCalculando melhor alinhamento...\n");
    alinharComReferenciaNW(conjunto_detalhes, sequencias_count, msa_final_resultado, &comprimento_final_msa);

    printf("\nMelhor alinhamento encontrado:\n\n");
    imprimirSequencia(msa_final_resultado, sequencias_count, comprimento_final_msa);
    
    printf("\nSCORE FINAL:\n");
    calcular_score(msa_final_resultado, sequencias_count, comprimento_final_msa);

    printf("\nLiberando memoria de conjunto_detalhes...\n");
    for (int i = 0; i < sequencias_count; i++) {
        if (conjunto_detalhes[i].texto_seq != NULL) {
            free(conjunto_detalhes[i].texto_seq);
        }
    }
    free(conjunto_detalhes);
    conjunto_detalhes = NULL;

    printf("\n\nPrograma concluido.\n");

    return 0;
}