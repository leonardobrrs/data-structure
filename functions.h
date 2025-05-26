// functions.h

// Include Guard: Evita inclusões múltiplas do cabeçalho.
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Inclusões de bibliotecas padrão do C.
#include <stdio.h>   // Para entrada/saída padrão (printf, FILE, fopen, etc.).
#include <stdlib.h>  // Para funções gerais (malloc, free, atoi, system, etc.).
#include <string.h>  // Para manipulação de strings (strcpy, strlen, memset, etc.).
#include <ctype.h>   // Para funções de classificação de caracteres (isalpha, etc.).

// Inclusão do nosso cabeçalho de estruturas.
// Define a estrutura 'No' usada nas funções da árvore.
#include "structs.h"

// --- Constantes Globais ---

/** @brief Número máximo de sequências que o programa pode processar. */
#define MAX_SEQ 50

/** @brief Comprimento máximo de uma sequência original (sem contar gaps de alinhamento).
 * Considera 100 caracteres para a sequência, 2 para folga/gaps iniciais (lógica antiga) e 1 para o '\0'.
 */
#define MAX_LEN 103

/** @brief Comprimento máximo estimado para uma sequência após o alinhamento (incluindo gaps).
 * Definido como o dobro de MAX_LEN para acomodar a inserção de gaps.
 */
#define MAX_FINAL_LEN (MAX_LEN * 2)

// --- Parâmetros de Pontuação para o Alinhamento ---

/** @brief Pontuação para um par de bases idênticas (match). */
#define ALPHA 1
/** @brief Pontuação para um par de bases diferentes (mismatch). */
#define BETA 0
/** @brief Pontuação para um par formado por uma base e um gap. */
#define DELTA -2
/** @brief Pontuação para um par formado por dois gaps. */
#define GAP_GAP 0

// --- Protótipos das Funções ---

// --- Protótipos das Funções da Árvore N-ária ---

/**
 * @brief Cria um novo nó para a árvore de alinhamento.
 * @param seq A string (coluna) que este nó representará.
 * @param score_coluna O score calculado para esta coluna.
 * @return Ponteiro para o novo nó criado e alocado dinamicamente.
 */
No* criaNovoNo(char seq[], int score_coluna);

/**
 * @brief Inicializa a árvore criando um nó raiz dummy.
 * @return Ponteiro para o nó raiz da árvore.
 */
No* iniciaArvore();

/**
 * @brief Insere um nó 'filho' como descendente de um nó 'pai'.
 * O filho é adicionado ao final da lista de irmãos do primogênito do pai.
 * @param pai Ponteiro para o nó pai.
 * @param filho Ponteiro para o nó filho a ser inserido.
 */
void inserirFilho(No* pai, No* filho);

/**
 * @brief Encontra e retorna o filho de um nó 'pai' que possui o maior score.
 * @param pai Ponteiro para o nó pai cujos filhos serão avaliados.
 * @param nSeq O número de sequências no alinhamento (usado para calcular o score da coluna nos filhos).
 * @return Ponteiro para o nó filho com o maior score, ou NULL se não houver filhos.
 */
No* devolveMelhorFilho(No *pai, int nSeq);

/**
 * @brief Libera recursivamente toda a memória alocada para uma árvore (ou subárvore) a partir do nó fornecido.
 * @param no Ponteiro para o nó raiz da árvore/subárvore a ser liberada.
 */
void liberaArvore(No* no);

/**
 * @brief Reconstrói a matriz de alinhamento final percorrendo o caminho guloso na árvore.
 * @param raiz Ponteiro para o nó raiz da árvore de alinhamento (o nó dummy inicial).
 * @param nSeq Número de sequências.
 * @param final_seq Matriz 2D onde o alinhamento final será armazenado.
 * @param final_len Ponteiro para um inteiro que armazenará o comprimento do alinhamento final.
 */
void reconstroiAlinhamento(No* raiz, int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len);

// --- Nova Função de Alinhamento Principal ---

/**
 * @brief Realiza o alinhamento múltiplo de sequências usando uma abordagem gulosa baseada em árvore N-ária.
 * @param orig_seq Matriz contendo as sequências originais a serem alinhadas.
 * @param lengths Array contendo os comprimentos pré-calculados de cada sequência original.
 * @param nSeq Número de sequências a serem alinhadas.
 * @param final_seq Matriz 2D onde o melhor alinhamento encontrado será armazenado.
 * @param final_len Ponteiro para um inteiro que armazenará o comprimento do alinhamento final.
 */
void alinhaComArvore(char orig_seq[][MAX_FINAL_LEN], int lengths[], int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len);


// --- Funções de Leitura e Validação de Sequências ---

/**
 * @brief Lê sequências de DNA de um arquivo de texto especificado.
 * @param nomeArquivo O nome do arquivo (incluindo caminho, se necessário) a ser lido.
 * @param sequencias Matriz 2D para armazenar as sequências lidas do arquivo.
 * @param maxLen Ponteiro para um inteiro que armazenará o comprimento da maior sequência original encontrada.
 * @return O número de sequências lidas com sucesso, ou -1 em caso de erro ao abrir o arquivo.
 */
int lerSequencias(const char *nomeArquivo, char sequencias[][MAX_FINAL_LEN], int *maxLen);

/**
 * @brief Verifica se uma dada string (sequência) contém apenas caracteres alfabéticos.
 * @param seq A string (sequência de DNA) a ser validada.
 * @return 1 se a sequência for válida (apenas letras), 0 caso contrário.
 */
int verificaCharValidos(char seq[]);

// --- Funções de Alinhamento e Cálculo de Score ---

/**
 * @brief Preenche o início das sequências com gaps ('-') para que todas tenham o mesmo comprimento.
 * Usado para o "Alinhamento com gaps no inicio".
 * @param seq Matriz 2D de sequências a ser modificada.
 * @param nSeq Número de sequências na matriz.
 * @param maxLen O comprimento alvo para todas as sequências após o preenchimento (geralmente o da maior sequência original).
 */
void preencheGapInicial(char seq[][MAX_FINAL_LEN], int nSeq, int maxLen);

/**
 * @brief Calcula o score de uma única coluna de um alinhamento.
 * @param vet Array de caracteres representando a coluna (cada caractere é de uma sequência).
 * @param nCol Número de sequências (e, portanto, de caracteres na coluna/vetor).
 * @return O score total para a coluna fornecida.
 */
int calcScoreColuna(char vet[], int nCol);

/**
 * @brief Calcula e imprime o score SP (Sum-of-Pairs) total de uma matriz de alinhamento.
 * @param sequencias Matriz 2D contendo o alinhamento completo.
 * @param lin Número de sequências (linhas) no alinhamento.
 * @param col Número de colunas (comprimento) do alinhamento.
 */
void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col);

/**
 * @brief Verifica se todas as sequências já foram completamente processadas no algoritmo de alinhamento.
 * @param proximo_char Array contendo o índice do próximo caractere a ser considerado para cada sequência original.
 * @param lengths Array contendo os comprimentos pré-calculados de cada sequência original.
 * @param nSeq Número de sequências.
 * @return 1 se todas as sequências terminaram, 0 caso contrário.
 */
int todas_terminaram(int proximo_char[], int lengths[], int nSeq);

// Protótipo comentado de uma versão anterior da função de alinhamento.
// void alinhaSequenciasGreedy(char orig_seq[][MAX_FINAL_LEN], int nSeq, int orig_max_len, char final_seq[][MAX_FINAL_LEN], int *final_len);

// --- Funções Auxiliares ---

/**
 * @brief Imprime uma matriz de sequências no console.
 * @param sequencia Matriz 2D de sequências a ser impressa.
 * @param tamanhoSequencia Número de sequências (linhas) a serem impressas.
 * @param maxLen O comprimento das colunas a serem consideradas (parâmetro não utilizado ativamente na função, pois printf("%s") usa '\0').
 */
void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen);

/**
 * @brief Limpa a tela do console (usa comandos específicos do sistema operacional).
 */
void limparTela();

#endif // FUNCTIONS_H