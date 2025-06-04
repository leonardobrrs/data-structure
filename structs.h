#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>
#include <string.h>

/** @brief Número máximo de sequências que o programa pode processar. */
#define MAX_SEQ 50

/** @brief Comprimento máximo de uma sequência original (sem contar gaps de alinhamento).
 * Considera 100 caracteres para a sequência, 2 para folga/gaps iniciais e 1 para o '\0'.
 */
#define MAX_LEN 103
/** @brief Comprimento máximo estimado para uma sequência após o alinhamento (incluindo gaps).
 * Definido como o dobro de MAX_LEN para acomodar a inserção de gaps.
 */
#define MAX_FINAL_LEN (MAX_LEN * 2)

/** @brief Pontuação para um par de bases idênticas (match). */
#define ALPHA 1
/** @brief Pontuação para um par de bases diferentes (mismatch). */
#define BETA 0
/** @brief Pontuação para um par formado por uma base e um gap. */
#define DELTA -2
/** @brief Pontuação para um par formado por dois gaps. */
#define GAP_GAP 0
/** @brief Pontuação para proibir certos caminhos. */
#define LIMITE_NEGATIVO -100000000

/**
 * @brief Nó de uma lista encadeada para armazenar uma sequência de DNA,
 * seu comprimento e seu índice original de leitura.
 */
typedef struct NoSequencia {
    char *textoDaSequencia;
    int comprimento;
    int indice_original_leitura;
    struct NoSequencia *proximaSeq;
} NoSequencia;

/**
 * @brief Estrutura para armazenar uma sequência, seu comprimento original
 * e seu índice original antes da ordenação.
 * Usada para facilitar a ordenação das sequências por comprimento.
 */
typedef struct {
    char *texto_seq;
    int tamanho_original;
    int indice_antes_ordenar;
} DetalheSequencia;

#endif