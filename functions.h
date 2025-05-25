#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"

/**
 * @brief Aloca dinamicamente e inicializa um novo nó da árvore.
 * @param seq A sequência de caracteres (coluna) para este nó.
 * @param pai_no O nó pai deste novo nó (NULL se for a raiz simbólica).
 * @param col_idx O índice da coluna que este nó representa.
 * @param score_col O score calculado apenas para esta coluna.
 * @return No* Um ponteiro para o nó recém-criado.
 */
No* criaNovoNo(const char seq[], No* pai_no, int col_idx, double score_col);

/**
 * @brief Insere um nó 'filho' na lista de filhos de um nó 'pai'.
 * @param pai O nó pai onde o filho será inserido.
 * @param filho O nó filho a ser inserido.
 */
void inserirFilho(No* pai, No* filho);

/**
 * @brief Libera toda a memória alocada para a árvore a partir da raiz.
 * @param raiz O nó raiz da árvore (ou sub-árvore) a ser liberada.
 */
void liberaArvore(No* raiz);

/**
 * @brief Lê sequências de DNA de um arquivo de texto.
 * Cada linha do arquivo deve conter uma sequência.
 * @param filename O nome do arquivo a ser lido.
 * @param seq A matriz 2D para armazenar as sequências lidas.
 * @param nSeq Ponteiro para armazenar o número de sequências lidas.
 * @param maxLen Ponteiro para armazenar o comprimento da maior sequência lida.
 * @param orig_lengths Array para armazenar o comprimento original de cada sequência.
 * @return int Retorna 0 em caso de sucesso, -1 em caso de falha (ex: arquivo não encontrado).
 */
int lerSequencias(const char *filename, char seq[][MAX_LEN_ALN], int *nSeq, int *maxLen, int orig_lengths[]);

/**
 * @brief Calcula o score de uma única coluna do alinhamento.
 * Soma os scores de todos os pares de caracteres na coluna.
 * @param vet A coluna (array de caracteres) a ser avaliada.
 * @param nSeq O número de sequências (tamanho do array vet).
 * @return double O score calculado para a coluna.
 */
double calcScoreColuna(const char vet[], int nSeq);

/**
 * @brief Gera as variações (nós filhos) para a próxima coluna.
 * Cria nós para a coluna base, variações com 1 gap e a coluna toda-gap.
 * @param pai O nó pai (coluna anterior) ao qual os filhos serão adicionados.
 * @param nSeq O número de sequências.
 * @param orig_seq A matriz com as sequências ORIGINAIS.
 * @param orig_lengths Array com os comprimentos originais de cada sequência.
 * @param col_idx O índice da coluna (próxima) a ser gerada.
 */
void gerarVariacoes(No* pai, int nSeq, char orig_seq[][MAX_LEN_ALN], int orig_lengths[], int col_idx);

/**
 * @brief Encontra e retorna o nó filho com o maior score acumulado.
 * Implementa a escolha gulosa do algoritmo.
 * @param pai O nó pai cujos filhos serão avaliados.
 * @return No* Um ponteiro para o nó filho com o maior score, 
 * ou NULL se o pai não tiver filhos.
 */
No* devolveMelhorFilho(No* pai);

/**
 * @brief Reconstrói a matriz de alinhamento a partir do nó final.
 * Navega de volta à raiz usando os ponteiros 'pai'.
 * @param no_final O último nó do caminho escolhido (melhor alinhamento).
 * @param alinhamento_final Matriz 2D para armazenar o resultado.
 * @param nSeq Número de sequências.
 * @param max_len O comprimento final do alinhamento.
 */
void reconstruirAlinhamento(No* no_final, char alinhamento_final[][MAX_LEN_ALN], int nSeq, int max_len);

/**
 * @brief Imprime a matriz de alinhamento formatada.
 * @param seq Matriz 2D contendo o alinhamento final.
 * @param nSeq Número de sequências.
 */
void imprimirAlinhamento(char seq[][MAX_LEN_ALN], int nSeq);

#endif // FUNCTIONS_H