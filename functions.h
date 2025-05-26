// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Inclusão do nosso cabeçalho de structs
#include "structs.h"

// --- Constantes ---
#define MAX_SEQ 50    // Máximo de 50 sequências
#define MAX_LEN 103   // 100 caracteres + 2 para folga/gaps + 1 para '\0'
#define MAX_FINAL_LEN (MAX_LEN * 2) // Tamanho estimado para alinhamento final

// Scores (Valores do código de referência, ajustar se necessário)
#define ALPHA 1      // Match (Base = Base)
#define BETA 0        // Mismatch (Base != Base)
#define DELTA -2       // Gap + Base
#define GAP_GAP 0     // Gap + Gap (Novo requisito)

// --- Protótipos das Funções ---

// --- Protótipos das Funções da Árvore ---
No* criaNovoNo(char seq[], int score_coluna);
No* iniciaArvore();
void inserirFilho(No* pai, No* filho);
No* devolveMelhorFilho(No *pai, int nSeq); // Modificado
void liberaArvore(No* no);
void reconstroiAlinhamento(No* raiz, int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len);

// --- Nova Função de Alinhamento ---
void alinhaComArvore(char orig_seq[][MAX_FINAL_LEN], int lengths[], int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len);


// Funções de Leitura e Validação
int lerSequencias(const char *nomeArquivo, char sequencias[][MAX_FINAL_LEN], int *maxLen);
int verificaCharValidos(char seq[]);

// Funções de Alinhamento e Score
void preencheGapInicial(char seq[][MAX_FINAL_LEN], int nSeq, int maxLen);
int calcScoreColuna(char vet[], int nCol);
void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col);
int todas_terminaram(int proximo_char[], int lengths[], int nSeq);

// void alinhaSequenciasGreedy(char orig_seq[][MAX_FINAL_LEN], int nSeq, int orig_max_len, char final_seq[][MAX_FINAL_LEN], int *final_len);

// Funções Auxiliares
void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen); // Ajustado para MAX_FINAL_LEN
void limparTela(); // Função para system("clear") ou system("cls")

#endif // FUNCTIONS_H