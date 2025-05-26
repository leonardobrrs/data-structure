// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h> // Para malloc e NULL
#include <string.h> // Para strcpy
#include "functions.h" // Para MAX_SEQ

// Definimos MAX_SEQ_STR_LEN aqui para o varSeq
// Deve ser grande o suficiente para MAX_SEQ + 1 (para '\0')
#define MAX_SEQ_STR_LEN (50 + 1)

typedef struct no {
    char varSeq[MAX_SEQ_STR_LEN]; // Variação da coluna (até 50 seqs + '\0')
    struct no *primogenito;   // Primeiro filho
    struct no *irmao;         // Irmão ao lado
    int score;                // Score da coluna (se necessário)

} No;

#endif // STRUCTS_H