#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <errno.h> // Para usar 'perror' em erros de arquivo

#define MAX_SEQS 50   // Limite para AB2
#define MAX_LEN_INIT 100 // Limite inicial das sequências
#define MAX_LEN_ALN 150  // Estimativa do tamanho máximo alinhado

#define K_BEAM_WIDTH 5 // Largura do beam search

#define ALPHA 1
#define BETA 0
#define DELTA -2

// 0: os gaps devem ocorrer preferencialmente no início da sequência
// 1: os gaps devem ocorrer preferencialmente no final da sequência
// 2:  os gaps devem ocorrer preferencialmente no meio da sequência
#define GAP_PREF 0

typedef struct No {
    char varSeq[MAX_SEQS + 1]; // Coluna representada por este nó
    struct No *primogenito;    // Ponteiro para o primeiro filho
    struct No *irmao;          // Ponteiro para o próximo irmão
    struct No *pai;            // Ponteiro para o pai (essencial para reconstruir)
    double score;              // Score total do alinhamento até este nó
    int coluna_index;          // Índice da coluna (debug/reconstrução)
} No;