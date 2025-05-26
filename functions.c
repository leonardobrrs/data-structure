// functions.c
#include "functions.h"
#include <errno.h> // Para verificar erros ao abrir arquivos

// Implementações das funções virão aqui...

// Implementação simples para limpar a tela (multiplataforma)
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- Implementações da Árvore ---

No* criaNovoNo(char seq[], int score_coluna) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        perror("Erro ao alocar memoria para No");
        exit(1);
    }
    novo->primogenito = NULL;
    novo->irmao = NULL;
    strcpy(novo->varSeq, seq);
    novo->score = score_coluna;
    return novo;
}

No* iniciaArvore() {
    return criaNovoNo("RAIZ", 0); // Nó raiz dummy
}

// Insere um filho a um nó pai. Adiciona no final da lista de irmãos.
void inserirFilho(No* pai, No* filho) {
    if (!pai || !filho) return;

    if (!pai->primogenito) { // Se não tem filho, este é o primeiro
        pai->primogenito = filho;
    } else { // Se já tem, vai até o último irmão
        No* p = pai->primogenito;
        while (p->irmao) {
            p = p->irmao;
        }
        p->irmao = filho;
    }
}

// Encontra o filho com o maior score
No* devolveMelhorFilho(No *pai, int nSeq) {
    if (!pai || !pai->primogenito) return NULL;

    No *filho = pai->primogenito;
    No *maior = filho;

    while (filho != NULL) {
        // O score já foi calculado ao criar o nó.
        // Apenas comparamos.
        if (maior->score < filho->score) {
            maior = filho;
        }
        filho = filho->irmao;
    }
    return maior;
}

// Libera a memória da árvore (pós-ordem)
void liberaArvore(No* no) {
    if (no == NULL) return;
    liberaArvore(no->primogenito);
    liberaArvore(no->irmao);
    free(no);
}

// Percorre o caminho guloso (apenas primogênitos) e monta o alinhamento
void reconstroiAlinhamento(No* raiz, int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len) {
    *final_len = 0;
    No* atual = raiz->primogenito; // Pula a raiz dummy

    while (atual != NULL) {
        if (*final_len >= MAX_FINAL_LEN - 1) break; // Segurança

        for (int i = 0; i < nSeq; i++) {
            final_seq[i][*final_len] = atual->varSeq[i];
        }
        (*final_len)++;
        atual = atual->primogenito; // Vai para o próximo (e único) filho no caminho
    }

    // Adiciona o terminador nulo
    for (int i = 0; i < nSeq; i++) {
        final_seq[i][*final_len] = '\0';
    }
}

void alinhaComArvore(char orig_seq[][MAX_FINAL_LEN], int lengths[], int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len) {
    int proximo_char[MAX_SEQ];
    char candidatos[MAX_SEQ + 1][MAX_SEQ_STR_LEN];

    No* raiz = iniciaArvore();
    No* paiAtual = raiz;

    for (int i = 0; i < nSeq; i++) {
        proximo_char[i] = 0;
    }
    // *final_len será definido por reconstroiAlinhamento

    for(int i=0; i<nSeq; i++) {
        final_seq[i][0] = '\0';
    }

    int iter_count = 0; // Ainda útil para a segurança contra loops inesperados

    while (!todas_terminaram(proximo_char, lengths, nSeq)) {

        iter_count++;
        if (iter_count > (MAX_LEN * 3) + nSeq) { // Limite de segurança
            printf("ERRO: Loop excedeu limite de iteracoes!\n");
            break;
        }

        if (paiAtual->primogenito != NULL) {
            liberaArvore(paiAtual->primogenito);
            paiAtual->primogenito = NULL;
        }

        memset(candidatos, 0, sizeof(candidatos));
        No* no_cand0 = NULL;

        // Candidato 0
        for (int k = 0; k < nSeq; k++) {
            if (proximo_char[k] < lengths[k]) {
                candidatos[0][k] = orig_seq[k][proximo_char[k]];
            } else {
                candidatos[0][k] = '-';
            }
        }
        candidatos[0][nSeq] = '\0';
        no_cand0 = criaNovoNo(candidatos[0], calcScoreColuna(candidatos[0], nSeq));
        inserirFilho(paiAtual, no_cand0);

        // Candidatos 1 a N
        for (int j = 0; j < nSeq; j++) {
            int indice_candidato_array = j + 1;
            int pode_gerar_este = 1;

            for (int k = 0; k < nSeq; k++) {
                if (k == j) {
                    if(proximo_char[k] < lengths[k]) {
                        candidatos[indice_candidato_array][k] = '-';
                    } else { pode_gerar_este = 0; break; }
                } else {
                    if (proximo_char[k] < lengths[k]) {
                        candidatos[indice_candidato_array][k] = orig_seq[k][proximo_char[k]];
                    } else { candidatos[indice_candidato_array][k] = '-'; }
                }
            }
            candidatos[indice_candidato_array][nSeq] = '\0';

            if(pode_gerar_este) {
                inserirFilho(paiAtual, criaNovoNo(candidatos[indice_candidato_array], calcScoreColuna(candidatos[indice_candidato_array], nSeq)));
            }
        }

        No* melhorFilho = devolveMelhorFilho(paiAtual, nSeq);
        if (melhorFilho == NULL) {
             break;
        }

        int progresso_seria_feito = 0;
        for(int k=0; k < nSeq; k++) {
            if(melhorFilho->varSeq[k] != '-' && proximo_char[k] < lengths[k]) {
                progresso_seria_feito = 1;
                break;
            }
        }

        if (!progresso_seria_feito && !todas_terminaram(proximo_char, lengths, nSeq)) {
            melhorFilho = no_cand0;
        }

        No* lista_para_liberar = NULL;
        No* prev = NULL;
        No* curr = paiAtual->primogenito;

        while(curr != NULL && curr != melhorFilho) {
            prev = curr;
            curr = curr->irmao;
        }
        if (curr == melhorFilho) {
            if (prev == NULL) {
                lista_para_liberar = melhorFilho->irmao;
            } else {
                prev->irmao = melhorFilho->irmao;
                lista_para_liberar = paiAtual->primogenito;
            }
        } else {
             lista_para_liberar = paiAtual->primogenito;
        }
        liberaArvore(lista_para_liberar);
        paiAtual->primogenito = melhorFilho;
        melhorFilho->irmao = NULL;

        paiAtual = melhorFilho;

        for (int k = 0; k < nSeq; k++) {
            if (paiAtual->varSeq[k] != '-') {
                 proximo_char[k]++;
            }
        }
    }

    reconstroiAlinhamento(raiz, nSeq, final_seq, final_len);
    liberaArvore(raiz);
}


// Implementação de verificaCharValidos (adaptada do original)
int verificaCharValidos(char seq[]) {
    for (int i = 0; seq[i] != '\0' && seq[i] != '\n'; i++) {
        if (!isalpha(seq[i])) {
            return 0; // Inválido se não for letra
        }
        // Opcional: Validar se são apenas A, C, T, G
        // char c = toupper(seq[i]);
        // if (c != 'A' && c != 'C' && c != 'T' && c != 'G') {
        //     return 0;
        // }
    }
    return 1; // Válido
}

// Implementação de imprimirSequencia (adaptada do original)
void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen) {
    for (int i = 0; i < tamanhoSequencia; i++) {
        printf("%s\n", sequencia[i]);
    }
}

int lerSequencias(const char *nomeArquivo, char sequencias[][MAX_FINAL_LEN], int *maxLen) {
    FILE *arquivo;
    char linha[MAX_LEN + 10]; // Buffer um pouco maior
    int contador_validas = 0; // Conta apenas as válidas
    int numero_linha = 0;     // Conta todas as linhas lidas
    size_t len_atual = 0;

    *maxLen = 0;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        numero_linha++; // Incrementa a linha atual

        // Verifica se excedemos o limite de sequências válidas
        if (contador_validas >= MAX_SEQ) {
            printf("Aviso: Limite de %d sequencias atingido. Linha %d e seguintes ignoradas.\n", MAX_SEQ, numero_linha);
            break;
        }

        // Remove \n e \r (mais robusto)
        linha[strcspn(linha, "\r\n")] = 0;

        // Pula linhas vazias
        if (strlen(linha) == 0) {
            continue;
        }

        len_atual = strlen(linha);

        // Verifica o tamanho máximo
        if (len_atual > (MAX_LEN - 3)) {
            // Imprime apenas os primeiros 80 chars para não poluir
            printf("Aviso: Sequencia '%.80s...' (linha %d) excede 100 caracteres e sera ignorada.\n", linha, numero_linha);
            continue;
        }

        // Verifica caracteres válidos
        if (!verificaCharValidos(linha)) {
            printf("Aviso: Sequencia '%s' (linha %d) contem caracteres invalidos e sera ignorada.\n", linha, numero_linha);
            continue;
        }

        // Se passou, copia e atualiza
        strcpy(sequencias[contador_validas], linha);
        if (len_atual > *maxLen) {
            *maxLen = len_atual;
        }
        contador_validas++; // Incrementa apenas as válidas
    }

    fclose(arquivo);

    // Se nenhuma sequência válida foi lida, mas o arquivo foi aberto,
    // o retorno será 0. O main.c já trata isso.
    return contador_validas;
}

void preencheGapInicial(char seq[][MAX_FINAL_LEN], int nSeq, int maxLen) {
    for (int i = 0; i < nSeq; i++) {
        int len_atual = strlen(seq[i]);
        int gaps_a_adicionar = maxLen - len_atual;

        if (gaps_a_adicionar > 0) {
            // Desloca os caracteres existentes para a direita
            // Usamos MAX_LEN como tamanho do buffer para memmove
            memmove(&seq[i][gaps_a_adicionar], &seq[i][0], len_atual + 1); // +1 para o '\0'

            // Preenche o início com gaps
            memset(&seq[i][0], '-', gaps_a_adicionar);
        }
    }
}

void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col) {
    int score_total = 0;
    int alpha_count = 0;
    int beta_count = 0;
    int delta_count = 0;

    // Itera por cada coluna
    for (int i = 0; i < col; i++) {
        // Itera por cada par de sequências (j, k) dentro da coluna
        for (int j = 0; j < lin - 1; j++) {
            for (int k = j + 1; k < lin; k++) {
                char base1 = sequencias[j][i];
                char base2 = sequencias[k][i];

                // Regra 1: Gap + Gap
                if (base1 == '-' && base2 == '-') {
                    score_total += GAP_GAP; // Soma 0
                }
                // Regra 2: Gap + Base (ou Base + Gap)
                else if (base1 == '-' || base2 == '-') {
                    score_total += DELTA;
                    delta_count++;
                }
                // Regra 3: Base + Base
                else {
                    // Sub-regra 3a: Match
                    if (base1 == base2) {
                        score_total += ALPHA;
                        alpha_count++;
                    }
                    // Sub-regra 3b: Mismatch
                    else {
                        score_total += BETA;
                        beta_count++;
                    }
                }
            }
        }
    }

    // Imprime o resultado detalhado
    printf("(α * %d) + (β * %d) + (δ * %d)", alpha_count, beta_count, delta_count);
    score_total > 0 ? printf(" = +%d\n", score_total) : printf(" = %d\n", score_total);
}

int calcScoreColuna(char vet[], int nSeq) {
    int score = 0;

    // Itera por cada par de sequências (i, j) dentro da coluna
    for (int i = 0; i < nSeq - 1; i++) {
        for (int j = i + 1; j < nSeq; j++) {
            char base1 = vet[i];
            char base2 = vet[j];

            // Regra 1: Gap + Gap
            if (base1 == '-' && base2 == '-') {
                score += GAP_GAP;
            }
            // Regra 2: Gap + Base (ou Base + Gap)
            else if (base1 == '-' || base2 == '-') {
                score += DELTA;
            }
            // Regra 3: Base + Base
            else {
                // Sub-regra 3a: Match
                if (base1 == base2) {
                    score += ALPHA;
                }
                // Sub-regra 3b: Mismatch
                else {
                    score += BETA;
                }
            }
        }
    }
    return score;
}

int todas_terminaram(int proximo_char[], int lengths[], int nSeq) {
    for (int i = 0; i < nSeq; i++) {
        if (proximo_char[i] < lengths[i]) {
            return 0; // Pelo menos uma não terminou
        }
    }
    return 1; // Todas terminaram
}

// void alinhaSequenciasGreedy(char orig_seq[][MAX_FINAL_LEN], int nSeq, int orig_max_len, char final_seq[][MAX_FINAL_LEN], int *final_len) {
//     int proximo_char[MAX_SEQ];
//     char candidatos[MAX_SEQ + 1][MAX_SEQ];
//     int scores[MAX_SEQ + 1];
//     int melhor_indice;
//     int melhor_score;

//     // Inicializa os índices e o comprimento final
//     for (int i = 0; i < nSeq; i++) {
//         proximo_char[i] = 0;
//     }
//     *final_len = 0;

//     // Inicializa final_seq com '\0'
//     for(int i=0; i<nSeq; i++) {
//         final_seq[i][0] = '\0';
//     }

//     // Loop principal
//     while (!todas_terminaram(proximo_char, orig_seq, nSeq)) {

//         memset(candidatos, 0, sizeof(candidatos));
//         memset(scores, 0, sizeof(scores));

//         // Candidato 0
//         for (int k = 0; k < nSeq; k++) {
//             if (proximo_char[k] < strlen(orig_seq[k])) {
//                 candidatos[0][k] = orig_seq[k][proximo_char[k]];
//             } else {
//                 candidatos[0][k] = '-';
//             }
//         }
//         scores[0] = calcScoreColuna(candidatos[0], nSeq);
//         melhor_score = scores[0];
//         melhor_indice = 0;

//         // Candidatos 1 a N
//         for (int j = 0; j < nSeq; j++) {
//             int indice_candidato = j + 1;
//             int pode_gerar_este = 1;

//             for (int k = 0; k < nSeq; k++) {
//                 if (k == j) {
//                     if(proximo_char[k] < strlen(orig_seq[k])) {
//                         candidatos[indice_candidato][k] = '-';
//                     } else {
//                         pode_gerar_este = 0; break;
//                     }
//                 } else {
//                     if (proximo_char[k] < strlen(orig_seq[k])) {
//                         candidatos[indice_candidato][k] = orig_seq[k][proximo_char[k]];
//                     } else {
//                         candidatos[indice_candidato][k] = '-';
//                     }
//                 }
//             }

//             if(pode_gerar_este) {
//                 scores[indice_candidato] = calcScoreColuna(candidatos[indice_candidato], nSeq);
//                 if (scores[indice_candidato] > melhor_score) {
//                     melhor_score = scores[indice_candidato];
//                     melhor_indice = indice_candidato;
//                 }
//             } else {
//                 scores[indice_candidato] = -99999;
//             }
//         }

//         // <<<--- ADICIONAR ESTA VERIFICAÇÃO AQUI ---<<<
//         int so_gaps = 1;
//         for(int k = 0; k < nSeq; k++) {
//             if (candidatos[melhor_indice][k] != '-') {
//                 so_gaps = 0;
//                 break;
//             }
//         }
//         // Se a melhor coluna só tem gaps, paramos o loop.
//         if (so_gaps) {
//             break;
//         }
//         // <<<-----------------------------------------<<<

//         // Adiciona a melhor coluna
//         for (int k = 0; k < nSeq; k++) {
//             final_seq[k][*final_len] = candidatos[melhor_indice][k];
//         }
//         (*final_len)++;

//         // Atualiza os índices
//         for (int k = 0; k < nSeq; k++) {
//             if (candidatos[melhor_indice][k] != '-') {
//                  proximo_char[k]++;
//             }
//         }

//         // Medida de segurança
//         if(*final_len >= MAX_FINAL_LEN -1) {
//             printf("Aviso: Alinhamento excedeu o tamanho máximo!\n");
//             break;
//         }
//     }

//     // Garante terminação nula
//     for(int i = 0; i < nSeq; i++) {
//         final_seq[i][*final_len] = '\0';
//     }
// }