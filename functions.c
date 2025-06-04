// functions.c
#include "functions.h"

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

NoSequencia* lerSequenciasParaLista(const char *nomeArquivo, int *numSeqLidas, int *maxLenOriginal) {
    FILE *arquivo;
    char linha_buffer[MAX_LEN + 10];
    
    NoSequencia *cabeca = NULL;
    NoSequencia *atual = NULL;
    int contador_seq = 0;
    int max_len = 0;
    int indice_leitura = 0;

    *numSeqLidas = 0;
    *maxLenOriginal = 0;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo em lerSequenciasParaLista");
        return NULL;
    }

    while (fgets(linha_buffer, sizeof(linha_buffer), arquivo) != NULL) {
        linha_buffer[strcspn(linha_buffer, "\r\n")] = 0;

        if (strlen(linha_buffer) == 0) {
            continue;
        }

        if (strlen(linha_buffer) >  MAX_LEN-3) {
            printf("Aviso: Sequencia '%s...' muito longa (max %d) e sera ignorada.\n", linha_buffer, MAX_LEN-3);
            continue;
        }
        
        if (!verificaCharValidos(linha_buffer)) {
            printf("Aviso: Sequencia '%s' contem caracteres invalidos e sera ignorada.\n", linha_buffer);
            continue;
        }

        NoSequencia *novoNo = (NoSequencia*)malloc(sizeof(NoSequencia));
        if (novoNo == NULL) {
            perror("Falha ao alocar NoSequencia");
            liberarListaSequencias(cabeca);
            fclose(arquivo);
            return NULL;
        }

        novoNo->textoDaSequencia = (char*)malloc(strlen(linha_buffer) + 1);
        if (novoNo->textoDaSequencia == NULL) {
            perror("Falha ao alocar textoDaSequencia");
            free(novoNo);
            liberarListaSequencias(cabeca);
            fclose(arquivo);
            return NULL;
        }
        strcpy(novoNo->textoDaSequencia, linha_buffer);
        novoNo->comprimento = strlen(novoNo->textoDaSequencia);
        novoNo->indice_original_leitura = indice_leitura++;
        novoNo->proximaSeq = NULL;

        if (novoNo->comprimento > max_len) {
            max_len = novoNo->comprimento;
        }

        if (cabeca == NULL) {
            cabeca = novoNo;
            atual = novoNo;
        } else {
            atual->proximaSeq = novoNo;
            atual = novoNo;
        }
        contador_seq++;
        if (contador_seq >= MAX_SEQ) {
            printf("Aviso: Limite de %d sequencias atingido.\n", MAX_SEQ);
            break;
        }
    }

    fclose(arquivo);
    *numSeqLidas = contador_seq;
    *maxLenOriginal = max_len;
    return cabeca;
}

void liberarListaSequencias(NoSequencia *cabeca) {
    NoSequencia *atual = cabeca;
    NoSequencia *proximo_a_liberar;

    while (atual != NULL) {
        proximo_a_liberar = atual;
        atual = atual->proximaSeq;
        
        free(proximo_a_liberar->textoDaSequencia);
        free(proximo_a_liberar);
    }
}

int verificaCharValidos(char seq[]) {
    if (seq[0] == '\0') {
        return 0;
    }

    for (int i = 0; seq[i] != '\0' && seq[i] != '\n'; i++) {
        char c_upper = toupper(seq[i]);

        if (c_upper != 'A' && c_upper != 'C' && c_upper != 'T' && c_upper != 'G') {
            return 0;
        }
    }
    return 1;
}

void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen_unused) {
    for (int i = 0; i < tamanhoSequencia; i++) {
        printf("%s\n", sequencia[i]);
    }
}

void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col) {
    int score_total = 0;
    int alpha_count = 0;
    int beta_count = 0;
    int delta_count = 0;

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < lin - 1; j++) {
            for (int k = j + 1; k < lin; k++) {
                char base1 = sequencias[j][i];
                char base2 = sequencias[k][i];

                if (base1 == '-' && base2 == '-') {
                    score_total += GAP_GAP;
                }
                else if (base1 == '-' || base2 == '-') {
                    score_total += DELTA;
                    delta_count++;
                }
                else {
                    if (base1 == base2) {
                        score_total += ALPHA;
                        alpha_count++;
                    }
                    else {
                        score_total += BETA;
                        beta_count++;
                    }
                }
            }
        }
    }
    printf("(α * %d) + (β * %d) + (δ * %d)", alpha_count, beta_count, delta_count);
    score_total > 0 ? printf(" = +%d\n", score_total) : printf(" = %d\n", score_total);
}

int max_tres(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    if (b >= a && b >= c) return b;
    return c;
}

int needleman_wunsch(const char *seq1, const char *seq2, char *aligned_seq1, char *aligned_seq2, int evitar_gaps_em_seq1) {
    int len1 = strlen(seq1);
    int len2 = strlen(seq2);

    int **dp_table = (int **)malloc((len1 + 1) * sizeof(int *));
    if (dp_table == NULL) {
        perror("Falha ao alocar linhas da tabela DP para Needleman-Wunsch");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= len1; i++) {
        dp_table[i] = (int *)malloc((len2 + 1) * sizeof(int));
        if (dp_table[i] == NULL) {
            perror("Falha ao alocar colunas da tabela DP para Needleman-Wunsch");
            for (int k = 0; k < i; k++) free(dp_table[k]);
            free(dp_table);
            exit(EXIT_FAILURE);
        }
    }

    dp_table[0][0] = 0;
    for (int i = 1; i <= len1; i++) {
        dp_table[i][0] = dp_table[i - 1][0] + DELTA;
    }
    for (int j = 1; j <= len2; j++) {
        if (evitar_gaps_em_seq1) {
            dp_table[0][j] = LIMITE_NEGATIVO;
        } else {
            dp_table[0][j] = dp_table[0][j - 1] + DELTA;
        }
    }

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int match_mismatch_score = (seq1[i - 1] == seq2[j - 1]) ? ALPHA : BETA;

            int score_diagonal = dp_table[i - 1][j - 1] + match_mismatch_score;
            int score_up = dp_table[i - 1][j] + DELTA;
            int score_left;

            if (evitar_gaps_em_seq1) {
                score_left = LIMITE_NEGATIVO;
            } else {
                score_left = dp_table[i][j - 1] + DELTA;
            }
            dp_table[i][j] = max_tres(score_diagonal, score_up, score_left);
        }
    }

    int optimal_score = dp_table[len1][len2];

    char temp_align1[MAX_FINAL_LEN * 2];
    char temp_align2[MAX_FINAL_LEN * 2];
    memset(temp_align1, 0, sizeof(temp_align1));
    memset(temp_align2, 0, sizeof(temp_align2));
    int align_idx = 0;

    int i_tb = len1;
    int j_tb = len2;

    while (i_tb > 0 || j_tb > 0) {
        int current_match_mismatch_score = (i_tb > 0 && j_tb > 0) ? ((seq1[i_tb - 1] == seq2[j_tb - 1]) ? ALPHA : BETA) : LIMITE_NEGATIVO;
        if (i_tb > 0 && j_tb > 0 && dp_table[i_tb][j_tb] == dp_table[i_tb - 1][j_tb - 1] + current_match_mismatch_score) {
            temp_align1[align_idx] = seq1[i_tb - 1];
            temp_align2[align_idx] = seq2[j_tb - 1];
            i_tb--;
            j_tb--;
        } else if (i_tb > 0 && dp_table[i_tb][j_tb] == dp_table[i_tb - 1][j_tb] + DELTA) {
            temp_align1[align_idx] = seq1[i_tb - 1];
            temp_align2[align_idx] = '-';
            i_tb--;
        } else if (j_tb > 0 && (!evitar_gaps_em_seq1 || dp_table[i_tb][j_tb -1] != LIMITE_NEGATIVO) && dp_table[i_tb][j_tb] == dp_table[i_tb][j_tb - 1] + DELTA) {
            temp_align1[align_idx] = '-';
            temp_align2[align_idx] = seq2[j_tb - 1];
            j_tb--;
        } else if (i_tb > 0) {
             temp_align1[align_idx] = seq1[i_tb - 1];
             temp_align2[align_idx] = '-';
             i_tb--;
        } else if (j_tb > 0) {
             if (!evitar_gaps_em_seq1) {
                 temp_align1[align_idx] = '-';
                 temp_align2[align_idx] = seq2[j_tb - 1];
                 j_tb--;
             } else {
                 temp_align1[align_idx] = '-';
                 temp_align2[align_idx] = seq2[j_tb - 1];
                 j_tb--;
             }
        }
        else {
            break;
        }
        align_idx++;
         if (align_idx >= (MAX_FINAL_LEN * 2) -1 ) {
            fprintf(stderr, "AVISO: Buffer de traceback do Needleman-Wunsch quase cheio!\n");
            break;
        }
    }

    for (int k = 0; k < align_idx; k++) {
        aligned_seq1[k] = temp_align1[align_idx - 1 - k];
        aligned_seq2[k] = temp_align2[align_idx - 1 - k];
    }
    aligned_seq1[align_idx] = '\0';
    aligned_seq2[align_idx] = '\0';

    for (int k = 0; k <= len1; k++) {
        free(dp_table[k]);
    }
    free(dp_table);

    return optimal_score;
}

int compararPorTamanhoDesc(const void *elem1, const void *elem2) {
    DetalheSequencia *seqInfo1 = (DetalheSequencia *)elem1;
    DetalheSequencia *seqInfo2 = (DetalheSequencia *)elem2;

    if (seqInfo2->tamanho_original > seqInfo1->tamanho_original) {
        return 1;
    } else if (seqInfo2->tamanho_original < seqInfo1->tamanho_original) {
        return -1;
    }
    return 0;
}

void alinharComReferenciaNW(DetalheSequencia *conjunto_ordenado_detalhes, int num_sequencias, char msa_final[][MAX_FINAL_LEN], int *comprimento_msa) {

    if (num_sequencias == 0) {
        *comprimento_msa = 0;
        return;
    }
    if (num_sequencias == 1) {
        int idx_original = conjunto_ordenado_detalhes[0].indice_antes_ordenar;
        strncpy(msa_final[idx_original], conjunto_ordenado_detalhes[0].texto_seq, MAX_FINAL_LEN -1);
        msa_final[idx_original][MAX_FINAL_LEN-1] = '\0';
        *comprimento_msa = conjunto_ordenado_detalhes[0].tamanho_original;
        
        if (*comprimento_msa < MAX_FINAL_LEN) {
             msa_final[idx_original][*comprimento_msa] = '\0';
        }
        return;
    }

    DetalheSequencia ref_principal = conjunto_ordenado_detalhes[0];
    
    strcpy(msa_final[ref_principal.indice_antes_ordenar], ref_principal.texto_seq);
    *comprimento_msa = ref_principal.tamanho_original;
    
    if (*comprimento_msa < MAX_FINAL_LEN) {
        msa_final[ref_principal.indice_antes_ordenar][*comprimento_msa] = '\0';
    } else {
        msa_final[ref_principal.indice_antes_ordenar][MAX_FINAL_LEN -1] = '\0';
    }

    char ref_alinhada_temp[MAX_FINAL_LEN * 2];
    char seq_atual_alinhada_temp[MAX_FINAL_LEN * 2];

    for (int i = 1; i < num_sequencias; i++) {
        DetalheSequencia seq_atual_para_alinhar = conjunto_ordenado_detalhes[i];

        memset(ref_alinhada_temp, 0, sizeof(ref_alinhada_temp));
        memset(seq_atual_alinhada_temp, 0, sizeof(seq_atual_alinhada_temp));

        needleman_wunsch(ref_principal.texto_seq, seq_atual_para_alinhar.texto_seq, ref_alinhada_temp,             seq_atual_alinhada_temp, 1);

        strncpy(msa_final[seq_atual_para_alinhar.indice_antes_ordenar], seq_atual_alinhada_temp, MAX_FINAL_LEN - 1);
        msa_final[seq_atual_para_alinhar.indice_antes_ordenar][MAX_FINAL_LEN - 1] = '\0';

        int len_seq_alinhada_agora = strlen(msa_final[seq_atual_para_alinhar.indice_antes_ordenar]);

        for (int k_pad = len_seq_alinhada_agora; k_pad < *comprimento_msa; k_pad++) {
            if (k_pad < MAX_FINAL_LEN -1) {
                msa_final[seq_atual_para_alinhar.indice_antes_ordenar][k_pad] = '-';
            }
        }
        if (*comprimento_msa < MAX_FINAL_LEN) {
            msa_final[seq_atual_para_alinhar.indice_antes_ordenar][*comprimento_msa] = '\0';
        } else {
             msa_final[seq_atual_para_alinhar.indice_antes_ordenar][MAX_FINAL_LEN -1] = '\0';
        }
    }
}