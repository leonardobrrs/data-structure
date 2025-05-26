#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Define o tamanho máximo da string que armazena uma coluna na árvore.
// Se temos até 50 sequências (MAX_SEQ), e cada caractere na coluna representa uma base de uma sequência, então a string precisa de 50 caracteres + 1 para o terminador nulo '\0'.
#define MAX_SEQ_STR_LEN (50 + 1)

// Esta estrutura representa um nó na árvore que será usada para construir e avaliar os alinhamentos.
typedef struct no {
    char varSeq[MAX_SEQ_STR_LEN]; // Campo para armazenar a representação da coluna de alinhamento deste nó.
    struct no *primogenito; // Ponteiro para o primeiro filho deste nó na árvore.
    struct no *irmao; // Ponteiro para o próximo nó irmão.
    int score; // Campo para armazenar o score associado à coluna representada por 'varSeq'.
} No;

#endif