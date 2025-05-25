#include "functions.h"

No* criaNovoNo(const char seq[], No* pai_no, int col_idx, double score_col) {
    // 1. Aloca memória
    No* novo = (No*)malloc(sizeof(No));
    
    // 2. Verifica se a alocação falhou
    if (novo == NULL) {
        perror("ERRO: Falha na alocacao de memoria para novo No");
        exit(1); // Encerra o programa se falhar
    }

    // 3. Inicializa os campos do novo nó
    strcpy(novo->varSeq, seq);      // Copia a coluna para o nó
    novo->primogenito = NULL;       // Inicializa com NULL
    novo->irmao = NULL;             // Inicializa com NULL
    novo->pai = pai_no;             // Define quem é o pai
    novo->coluna_index = col_idx;   // Define o índice da coluna

    // Calcula o score acumulado: score do pai + score desta coluna
    // Se for o nó raiz (pai_no == NULL), o score do pai é 0.
    novo->score = (pai_no ? pai_no->score : 0.0) + score_col;

    // 4. Retorna o ponteiro para o nó criado
    return novo;
}

void inserirFilho(No* pai, No* filho) {
    // Verifica se os ponteiros são válidos
    if (pai == NULL || filho == NULL) {
        fprintf(stderr, "AVISO: Tentativa de inserir com ponteiro NULO (pai ou filho).\n"); //Lança aviso
        return;
    }

    // Se o pai não tem nenhum filho (primogênito)
    if (pai->primogenito == NULL) {
        pai->primogenito = filho; // O novo nó se torna o primeiro filho
    } else {
        // Se já tem filhos, percorre a lista de irmãos até encontrar o último
        No* p_aux = pai->primogenito;
        while (p_aux->irmao != NULL) {
            p_aux = p_aux->irmao; // Avança para o próximo irmão
        }
        // O último irmão encontrado agora aponta para o novo filho
        p_aux->irmao = filho;
    }
}

void liberaArvore(No* raiz) {
    // Se o nó é nulo, encerra.
    if (raiz == NULL) {
        return;
    }

    // Ponteiro auxiliar para percorrer os filhos
    No* filho_atual = raiz->primogenito;

    // Loop para liberar CADA sub-árvore dos filhos
    while (filho_atual != NULL) {
        No* proximo_irmao = filho_atual->irmao; // Guarda o próximo irmão antes de liberar o atual
        liberaArvore(filho_atual);              // Chama recursivamente para liberar a sub-árvore do filho
        filho_atual = proximo_irmao;            // Vai para o próximo irmão (que foi guardado)
    }

    // Após liberar todas as sub-árvores dos filhos, libera o próprio nó raiz.
    free(raiz);
}

int lerSequencias(const char *filename, char seq[][MAX_LEN_ALN], int *nSeq, int *maxLen, int orig_lengths[]) {
    FILE *file_ptr;
    char line_buffer[MAX_LEN_ALN + 2]; // +2 para \n e \0
    int count = 0;
    int current_len = 0;
    int line_num = 0; // Para reportar a linha do arquivo

    // 1. Tenta abrir o arquivo para leitura ("r")
    file_ptr = fopen(filename, "r");

    // 2. Verifica se o arquivo foi aberto com sucesso
    if (file_ptr == NULL) {
        perror("ERRO ao abrir o arquivo"); // Mostra o erro específico do sistema
        fprintf(stderr, "Verifique se o arquivo '%s' existe no mesmo diretorio.\n", filename);
        return -1; // Retorna -1 para indicar falha
    }

    printf("Lendo arquivo '%s'...\n", filename);

    // 3. Lê o arquivo linha por linha
    while (fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL) {
        
        // Verifica se excedeu o limite de sequências
        if (count >= MAX_SEQS) {
            fprintf(stderr, "AVISO: Limite de %d sequencias atingido. Ignorando o resto do arquivo.\n", MAX_SEQS);
            break;
        }

        // Remove a quebra de linha (\n ou \r\n) que fgets() pode incluir
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;

        // Ignora linhas vazias
        if (strlen(line_buffer) == 0) {
            continue;
        }

        // Verifica o tamanho da linha (sequência)
        current_len = strlen(line_buffer);
        if (current_len > MAX_LEN_INIT) {
            fprintf(stderr, "AVISO: Sequencia %d ('%.10s...') excede %d caracteres. Ignorando.\n", 
                    count + 1, line_buffer, MAX_LEN_INIT);
            continue; 
        }

        // Validando a sequência
        int is_valid = 1; // Flag para indicar se a sequência é válida
        for (int i = 0; i < current_len; i++) {
            char c = toupper(line_buffer[i]); // Converte para maiúscula para facilitar
            if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
                fprintf(stderr, "AVISO: Caractere invalido ('%c') encontrado na linha %d. Ignorando sequencia.\n",
                        line_buffer[i], line_num);
                is_valid = 0; // Marca como inválida
                break;        // Sai do loop de validação
            }
            line_buffer[i] = c; // Armazena a versão maiúscula (opcional)
        }

        // Se a sequência não for válida, pula para a próxima linha
        if (!is_valid) {
            continue;
        }

        // 4. Armazena a sequência e seu tamanho
        strcpy(seq[count], line_buffer);
        orig_lengths[count] = current_len;

        // 5. Atualiza o tamanho máximo
        if (current_len > *maxLen) {
            *maxLen = current_len;
        }

        // 6. Incrementa o contador de sequências lidas
        count++;
    }

    // 7. Fecha o arquivo
    fclose(file_ptr);

    // 8. Verifica se pelo menos 2 sequências foram lidas
    if (count < 2) {
        fprintf(stderr, "ERRO: O arquivo deve conter pelo menos 2 sequencias validas.\n");
        return -1;
    }

    // 9. Atualiza o número de sequências e retorna sucesso
    *nSeq = count;
    printf("%d sequencias lidas com sucesso. Maior sequencia: %d.\n", *nSeq, *maxLen);
    return 0; // Retorna 0 para indicar sucesso
}

double calcScoreColuna(const char vet[], int nSeq) {
    double score_col = 0.0;

    // Itera por todos os pares únicos de sequências na coluna
    for (int i = 0; i < nSeq - 1; i++) {
        for (int j = i + 1; j < nSeq; j++) {
            
            char char1 = vet[i];
            char char2 = vet[j];

            // 1. Par Gap-Gap: Geralmente tem score 0 (não penaliza nem bonifica)
            if (char1 == '-' && char2 == '-') {
                score_col += 0; // Ou DELTA se quiser penalizar
            } 
            // 2. Par Base-Gap: Aplica a penalidade DELTA
            else if (char1 == '-' || char2 == '-') {
                score_col += DELTA; 
            } 
            // 3. Par Base-Base
            else {
                // 3a. Match (Bases Iguais): Aplica o bônus ALPHA
                if (char1 == char2) {
                    score_col += ALPHA; 
                } 
                // 3b. Mismatch (Bases Diferentes): Aplica a penalidade BETA
                else {
                    score_col += BETA;  
                }
            }
        }
    }
    return score_col;
}

void gerarVariacoes(No* pai, int nSeq, char orig_seq[][MAX_LEN_ALN], int orig_lengths[], int col_idx) {
    char coluna_base[MAX_SEQS + 1];
    int tem_base = 0; // 1 se a coluna base tiver pelo menos uma base, 0 se for só gaps.

    // 1. Constrói a 'coluna_base' com base nas sequências e comprimentos originais
    for(int i = 0; i < nSeq; i++) {
       // Se o índice da coluna atual for menor que o tamanho original da sequência 'i'...
       if (col_idx < orig_lengths[i]) {
           coluna_base[i] = orig_seq[i][col_idx]; // ...usa a base original.
           tem_base = 1; // Marca que encontramos pelo menos uma base.
       } else {
           coluna_base[i] = '-'; // ...senão, usa um gap.
       }
    }
    coluna_base[nSeq] = '\0'; // Termina a string da coluna.

    // 2. Se a coluna_base contém pelo menos uma base nitrogenada...
    if (tem_base) {
        // 2a. Adiciona a própria 'coluna_base' como um filho possível.
        double score_base = calcScoreColuna(coluna_base, nSeq);
        No* filho_base = criaNovoNo(coluna_base, pai, col_idx, score_base);
        inserirFilho(pai, filho_base);

        // 2b. Gera variações com 1 gap (substituindo uma base por '-')
        for (int k = 0; k < nSeq; k++) {
            // Só gera variação se na posição 'k' HOUVER uma base (não um gap).
            if (coluna_base[k] != '-') { 
                char coluna_gap[MAX_SEQS + 1];
                strcpy(coluna_gap, coluna_base); // Copia a base.
                coluna_gap[k] = '-';             // Insere o gap na posição 'k'.
                
                double score_gap = calcScoreColuna(coluna_gap, nSeq);
                No* filho_gap = criaNovoNo(coluna_gap, pai, col_idx, score_gap);
                inserirFilho(pai, filho_gap);
            }
        }
        
        // 2c. Adiciona a coluna 'só-gaps' como uma opção *adicional*.
        //     O algoritmo guloso poderá escolhê-la se o score for melhor.
        char coluna_so_gap[MAX_SEQS+1];
        for(int i = 0; i < nSeq; i++) coluna_so_gap[i] = '-';
        coluna_so_gap[nSeq] = '\0';
        double score_so_gap = calcScoreColuna(coluna_so_gap, nSeq);
        No* filho_so_gap = criaNovoNo(coluna_so_gap, pai, col_idx, score_so_gap);
        inserirFilho(pai, filho_so_gap);

    } 
    // 3. Se a 'coluna_base' já é 'só-gaps' (porque todas as sequências originais acabaram)...
    else {
        // ... a única opção é gerar a coluna 'só-gaps'.
        double score = calcScoreColuna(coluna_base, nSeq); // Score será 0
        No* filho = criaNovoNo(coluna_base, pai, col_idx, score);
        inserirFilho(pai, filho);
    }
}

bool is_all_gaps(const char vet[], int nSeq) {
    for (int i = 0; i < nSeq; i++) {
        if (vet[i] != '-') {
            return false; // Encontrou uma base, não é só gaps
        }
    }
    return true; // Não encontrou bases, é só gaps
}

// No* devolveMelhorFilho(No* pai) {
//     if (pai == NULL || pai->primogenito == NULL) {
//         return NULL; 
//     }

//     No* filho_atual = pai->primogenito;
//     No* melhor_filho = NULL; // Começa sem melhor filho
//     double max_score = -DBL_MAX; // Começa com um score muito baixo
//     No* fallback_all_gaps = NULL; // Guarda o nó 'só-gaps' caso seja a única opção
//     int nSeq = strlen(filho_atual->varSeq); // Pega o nSeq do primeiro filho

//     // Percorre todos os filhos
//     while (filho_atual != NULL) {
//         bool eh_so_gaps = is_all_gaps(filho_atual->varSeq, nSeq);

//         // Se o filho atual for 'só-gaps'...
//         if (eh_so_gaps) {
//             fallback_all_gaps = filho_atual; // ... guarda ele como última opção.
//         } 
//         // Se o filho atual NÃO for 'só-gaps'...
//         else {
//             // ... e se o score dele for o maior encontrado ATÉ AGORA...
//             if (filho_atual->score > max_score) {
//                 max_score = filho_atual->score; // ... atualiza o score máximo.
//                 melhor_filho = filho_atual;   // ... e marca ele como o melhor.
//             }
//         }
//         filho_atual = filho_atual->irmao; // Vai para o próximo irmão.
//     }

//     // Se encontramos um 'melhor_filho' que NÃO é 'só-gaps', retorna ele.
//     if (melhor_filho != NULL) {
//         return melhor_filho;
//     } 
//     // Senão, retorna o 'só-gaps' (ou NULL se nem isso existia).
//     else {
//         return fallback_all_gaps;
//     }
// }

void reconstruirAlinhamento(No* no_final, char alinhamento_final[][MAX_LEN_ALN], int nSeq, int max_len) {
    if (no_final == NULL) {
        fprintf(stderr, "ERRO: Nó final nulo para reconstrução.\n");
        return;
    }

    No* no_atual = no_final;
    int col_atual = no_final->coluna_index;

    // 1. Preenche toda a matriz final com '-' inicialmente.
    //    Isso garante que colunas não preenchidas fiquem com gaps.
    for (int i = 0; i < nSeq; i++) {
        for (int j = 0; j < max_len; j++) {
            alinhamento_final[i][j] = '-';
        }
        alinhamento_final[i][max_len] = '\0'; // Termina a string
    }

    // 2. Preenche da direita para a esquerda, voltando pela árvore.
    while (no_atual != NULL && no_atual->pai != NULL && col_atual >= 0) {
        // Verifica se o índice da coluna é válido
        if (col_atual < max_len) {
            for (int i = 0; i < nSeq; i++) {
                alinhamento_final[i][col_atual] = no_atual->varSeq[i];
            }
        }
        no_atual = no_atual->pai; // Sobe para o pai
        col_atual--;             // Vai para a coluna anterior
    }
}

void imprimirAlinhamento(char seq[][MAX_LEN_ALN], int nSeq) {
    if (nSeq <= 0) return;

    for (int i = 0; i < nSeq; i++) {
        printf("%s\n", seq[i]); // Imprime cada sequência alinhada
    }
}

int compare_nodes(const void *a, const void *b) {
    // Converte os ponteiros genéricos para ponteiros para ponteiros de No
    // Isso é necessário porque qsort trabalha com arrays de 'No*'
    No* nodeA = *(No**)a;
    No* nodeB = *(No**)b;

    // Compara os scores para ordenar do MAIOR para o MENOR
    if (nodeA->score < nodeB->score) return 1;  // B é maior, B vem antes
    if (nodeA->score > nodeB->score) return -1; // A é maior, A vem antes
    return 0; // Scores iguais, a ordem não importa
}