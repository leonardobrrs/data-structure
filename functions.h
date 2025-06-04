#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"

/**
 * @brief Lê sequências de um arquivo de texto e as armazena em uma lista encadeada.
 * Aloca dinamicamente memória para cada nó da lista e para cada string de sequência.
 * @param nomeArquivo O nome do arquivo a ser lido.
 * @param numSeqLidas Ponteiro para um inteiro onde o número de sequências lidas será armazenado.
 * @param maxLenOriginal Ponteiro para um inteiro onde o comprimento da maior sequência original será armazenado.
 * @return Ponteiro para o início (cabeça) da lista encadeada de sequências, ou NULL em caso de erro.
 */
NoSequencia* lerSequenciasParaLista(const char *nomeArquivo, int *numSeqLidas, int *maxLenOriginal);

/**
 * @brief Libera toda a memória alocada para uma lista encadeada de NoSequencia,
 * incluindo as strings de sequência dentro de cada nó.
 * @param cabeca Ponteiro para o início (cabeça) da lista encadeada a ser liberada.
 */
void liberarListaSequencias(NoSequencia *cabeca);

/**
 * @brief Verifica se uma dada string (sequência) contém apenas caracteres de DNA válidos (A, C, T, G).
 * A verificação é case-insensitive (ignora maiúsculas/minúsculas).
 * @param seq A string (sequência de DNA) a ser validada.
 * @return 1 se a sequência contiver apenas bases de DNA válidas, 0 caso contrário (incluindo string vazia).
 */
int verificaCharValidos(char seq[]);

// --- Funções de Alinhamento e Cálculo de Score ---

/**
 * @brief Calcula e imprime o score SP (Sum-of-Pairs) total de uma matriz de alinhamento.
 * @param sequencias Matriz 2D contendo o alinhamento completo.
 * @param lin Número de sequências (linhas) no alinhamento.
 * @param col Número de colunas (comprimento) do alinhamento.
 */
void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col);

/**
 * @brief Imprime uma matriz de sequências no console.
 * @param sequencia Matriz 2D de sequências a ser impressa.
 * @param tamanhoSequencia Número de sequências (linhas) a serem impressas.
 * @param maxLen O comprimento das colunas a serem consideradas
 */
void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen);

/**
 * @brief Limpa a tela do console (usa comandos específicos do sistema operacional).
 */
void limparTela();

/**
 * @brief Retorna o maior valor entre três inteiros.
 * @param a O primeiro inteiro.
 * @param b O segundo inteiro.
 * @param c O terceiro inteiro.
 * @return O maior valor entre a, b e c.
 */
int max_tres(int a, int b, int c);

/**
 * @brief Alinha duas sequências globalmente usando o algoritmo de Needleman-Wunsch.
 * @param seq1 A primeira sequência de DNA.
 * @param seq2 A segunda sequência de DNA.
 * @param aligned_seq1 Buffer para armazenar a primeira sequência alinhada.
 * @param aligned_seq2 Buffer para armazenar a segunda sequência alinhada.
 * @param evitar_gaps_em_seq1 Se 1, penaliza fortemente a inserção de gaps em seq1.
 * Se 0, comportamento padrão do Needleman-Wunsch.
 * @return O score ótimo do alinhamento global.
 */
int needleman_wunsch(const char *seq1, const char *seq2, char *aligned_seq1, char *aligned_seq2, int evitar_gaps_em_seq1);

/**
 * @brief Função de comparação para qsort, para ordenar DetalheSequencia
 * em ordem decrescente de tamanho_original.
 * @param elem1 Ponteiro para o primeiro elemento DetalheSequencia.
 * @param elem2 Ponteiro para o segundo elemento DetalheSequencia.
 * @return >0 se elem2 > elem1, <0 se elem1 > elem2, 0 se iguais (para decrescente).
 */
int compararPorTamanhoDesc(const void *elem1, const void *elem2);

/**
 * @brief Alinha múltiplas sequências usando a mais longa como referência e o
 * algoritmo de Needleman-Wunsch para os alinhamentos par a par.
 * @param conjunto_ordenado_detalhes Array de estruturas DetalheSequencia, já ordenado
 * pelo tamanho da sequência em ordem decrescente.
 * @param num_sequencias Número total de sequências no array.
 * @param msa_final Matriz 2D para armazenar o alinhamento múltiplo resultante.
 * @param comprimento_msa Ponteiro para um inteiro que armazenará o comprimento do alinhamento final.
 */
void alinharComReferenciaNW(DetalheSequencia *conjunto_ordenado_detalhes, int num_sequencias, char msa_final[][MAX_FINAL_LEN], int *comprimento_msa);

#endif