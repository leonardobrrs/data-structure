// functions.c
#include "functions.h" // Inclui os protótipos de funções, definições de constantes e estruturas.
#include <errno.h>    // Para perror, usado na função lerSequencias para reportar erros de arquivo.

// Implementação simples para limpar a tela do console (multiplataforma).
// Usa diretivas de pré-processador para escolher o comando apropriado.
void limparTela() {
#ifdef _WIN32      // Se o sistema operacional for Windows
    system("cls"); // Executa o comando "cls" para limpar a tela.
#else              // Para outros sistemas (Linux, macOS, etc.)
    system("clear"); // Executa o comando "clear" para limpar a tela.
#endif
}

// --- Implementações das Funções da Árvore N-ária ---

No* criaNovoNo(char seq[], int score_coluna) {
    // Aloca memória dinamicamente para a estrutura do nó.
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) { // Verifica se a alocação de memória falhou.
        perror("Erro ao alocar memoria para No"); // Reporta o erro.
        exit(1); // Termina o programa se não puder alocar memória.
    }
    // Inicializa os ponteiros do novo nó como NULL (sem filhos ou irmãos ainda).
    novo->primogenito = NULL;
    novo->irmao = NULL;
    // Copia a string da coluna (variação) para o campo varSeq do nó.
    strcpy(novo->varSeq, seq);
    // Define o score do nó.
    novo->score = score_coluna;
    return novo; // Retorna o ponteiro para o novo nó.
}

No* iniciaArvore() {
    // Cria um nó com a string "RAIZ" e score 0 para servir como raiz.
    return criaNovoNo("RAIZ", 0);
}

void inserirFilho(No* pai, No* filho) {
    if (!pai || !filho) return; // Retorna se o pai ou o filho forem nulos.

    if (!pai->primogenito) { // Se o pai não tem nenhum filho (primogênito é NULL),
        pai->primogenito = filho; // o novo 'filho' se torna o primogênito.
    } else { // Se o pai já tem filhos,
        No* p = pai->primogenito; // 'p' começa no primeiro filho.
        while (p->irmao) {      // Percorre a lista de irmãos até encontrar o último.
            p = p->irmao;         // Avança para o próximo irmão.
        }
        p->irmao = filho; // O último irmão agora aponta para o novo 'filho'.
    }
}

No* devolveMelhorFilho(No *pai, int nSeq_unused) {
    if (!pai || !pai->primogenito) return NULL; // Se o pai é nulo ou não tem filhos, retorna NULL.

    No *filho_atual = pai->primogenito; // Começa a busca pelo primeiro filho.
    No *no_maior_score = filho_atual; // Assume inicialmente que o primeiro filho tem o maior score.

    // Itera sobre todos os filhos (irmãos) do nó pai.
    while (filho_atual != NULL) {
        // O score de cada filho já foi calculado e armazenado quando o nó foi criado.
        // Apenas compara os scores.
        if (no_maior_score->score < filho_atual->score) {
            no_maior_score = filho_atual; // Encontrou um filho com score maior, atualiza.
        }
        filho_atual = filho_atual->irmao; // Move para o próximo irmão.
    }
    return no_maior_score; // Retorna o ponteiro para o filho com o maior score.
}

void liberaArvore(No* no) {
    if (no == NULL) return;        // Condição de parada da recursão: nó nulo.
    liberaArvore(no->primogenito); // Libera recursivamente a subárvore do primogênito.
    liberaArvore(no->irmao);       // Libera recursivamente a subárvore do irmão.
    free(no);                      // Libera a memória do nó atual.
}

void reconstroiAlinhamento(No* raiz, int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len) {
    *final_len = 0; // Inicializa o comprimento do alinhamento final.
    No* atual = raiz->primogenito; // Começa do primeiro filho real (pula a raiz dummy "RAIZ").

    // Percorre o caminho principal da árvore (o caminho escolhido pelo algoritmo guloso).
    while (atual != NULL) {
        // Verificação de segurança para não exceder o tamanho da matriz 'final_seq'.
        if (*final_len >= MAX_FINAL_LEN - 1) break;

        // Copia a coluna (varSeq) do nó atual para a coluna correspondente em 'final_seq'.
        for (int i = 0; i < nSeq; i++) {
            final_seq[i][*final_len] = atual->varSeq[i];
        }
        (*final_len)++; // Incrementa o comprimento do alinhamento.
        atual = atual->primogenito; // Avança para o próximo nó no caminho guloso.
    }

    // Adiciona o terminador nulo ('\0') ao final de cada sequência no alinhamento final.
    for (int i = 0; i < nSeq; i++) {
        final_seq[i][*final_len] = '\0';
    }
}

void alinhaComArvore(char orig_seq[][MAX_FINAL_LEN], int lengths[], int nSeq, char final_seq[][MAX_FINAL_LEN], int *final_len) {
    int proximo_char[MAX_SEQ]; // Array para rastrear o índice do próximo caractere a ser usado de cada sequência original.
    char candidatos[MAX_SEQ + 1][MAX_SEQ_STR_LEN]; // Matriz para gerar colunas candidatas.

    No* raiz = iniciaArvore(); // Cria o nó raiz (dummy) da árvore.
    No* paiAtual = raiz;     // 'paiAtual' aponta para o nó ao qual os novos filhos (candidatos) serão adicionados.

    // Inicializa os índices de 'proximo_char' para 0 para todas as sequências.
    for (int i = 0; i < nSeq; i++) {
        proximo_char[i] = 0;
    }
    // O comprimento final (*final_len) será determinado pela função reconstroiAlinhamento.

    // Inicializa a matriz do alinhamento final com strings vazias (terminador nulo no início).
    for(int i=0; i<nSeq; i++) {
        final_seq[i][0] = '\0';
    }

    int iter_count = 0; // Contador de iterações para segurança contra loops infinitos.

    // Loop principal: continua enquanto houver sequências que não foram completamente processadas.
    while (!todas_terminaram(proximo_char, lengths, nSeq)) {

        iter_count++;
        // Verificação de segurança para evitar loops infinitos.
        if (iter_count > (MAX_LEN * 3) + nSeq) { // MAX_LEN aqui é uma aproximação do comprimento máximo esperado.
            printf("ERRO: Loop excedeu limite de iteracoes!\n");
            break;
        }

        // Antes de gerar novos filhos para 'paiAtual', limpa quaisquer filhos
        // que possam ter sido gerados em uma iteração anterior.
        // Na lógica atual de poda, isso garante que 'paiAtual' está pronto para novos filhos.
        if (paiAtual->primogenito != NULL) {
            liberaArvore(paiAtual->primogenito);
            paiAtual->primogenito = NULL;
        }
        
        memset(candidatos, 0, sizeof(candidatos)); // Limpa o buffer de candidatos.
        No* no_cand0 = NULL; // Ponteiro para o nó do Candidato 0, usado na lógica de "forçar progresso".

        // Geração do Candidato 0: Usa o próximo caractere de cada sequência, ou '-' se a sequência terminou.
        for (int k = 0; k < nSeq; k++) {
            if (proximo_char[k] < lengths[k]) { // Se a sequência k ainda tem caracteres.
                candidatos[0][k] = orig_seq[k][proximo_char[k]];
            } else { // Se a sequência k já terminou.
                candidatos[0][k] = '-';
            }
        }
        candidatos[0][nSeq] = '\0'; // Adiciona terminador nulo para formar uma string válida.
        no_cand0 = criaNovoNo(candidatos[0], calcScoreColuna(candidatos[0], nSeq));
        inserirFilho(paiAtual, no_cand0); // Insere o Candidato 0 como filho.

        // Geração dos Candidatos 1 a N: Força um gap em uma sequência 'j' e usa o próximo caractere das outras.
        for (int j = 0; j < nSeq; j++) { // 'j' é o índice da sequência que receberá o gap forçado.
            int indice_candidato_array = j + 1; // Índice no array 'candidatos'.
            int pode_gerar_este = 1;            // Flag para indicar se este candidato é viável.

            for (int k = 0; k < nSeq; k++) { // Monta a coluna para este candidato.
                if (k == j) { // Se 'k' é a sequência 'j' que deve receber o gap.
                    if(proximo_char[k] < lengths[k]) { // Só pode forçar gap se a sequência ainda está ativa.
                        candidatos[indice_candidato_array][k] = '-';
                    } else { pode_gerar_este = 0; break; } // Se já terminou, este candidato não é viável.
                } else { // Para as outras sequências (k != j).
                    if (proximo_char[k] < lengths[k]) { // Se ainda tem caracteres.
                        candidatos[indice_candidato_array][k] = orig_seq[k][proximo_char[k]];
                    } else { // Se já terminou.
                        candidatos[indice_candidato_array][k] = '-';
                    }
                }
            }
            candidatos[indice_candidato_array][nSeq] = '\0'; // Adiciona terminador nulo.

            if(pode_gerar_este) { // Se o candidato é viável.
                inserirFilho(paiAtual, criaNovoNo(candidatos[indice_candidato_array], calcScoreColuna(candidatos[indice_candidato_array], nSeq)));
            }
        }

        // Escolhe o melhor filho (candidato com maior score) gerado para 'paiAtual'.
        No* melhorFilho = devolveMelhorFilho(paiAtual, nSeq);
        if (melhorFilho == NULL) { // Verificação de segurança (não deve acontecer).
             printf("ERRO INTERNO: Nenhum melhor Filho encontrado. Parando.\n");
             break; 
        }

        // Lógica para Forçar Progresso:
        // Verifica se a escolha gulosa (melhorFilho) faria algum progresso
        // (ou seja, se consumiria algum caractere de uma sequência ainda ativa).
        int progresso_seria_feito = 0;
        for(int k=0; k < nSeq; k++) {
            if(melhorFilho->varSeq[k] != '-' && proximo_char[k] < lengths[k]) {
                progresso_seria_feito = 1; // Sim, progresso seria feito.
                break;
            }
        }

        // Se a escolha gulosa NÃO faz progresso E AINDA HÁ sequências ativas,
        // força a escolha do Candidato 0 (que garantidamente faz progresso se houver seq. ativas).
        if (!progresso_seria_feito && !todas_terminaram(proximo_char, lengths, nSeq)) {
            melhorFilho = no_cand0; // no_cand0 é o nó que representa o Candidato 0.
        }
        // Fim da Lógica para Forçar Progresso.

        // Poda (Pruning): Mantém apenas o 'melhorFilho' e libera os outros irmãos.
        No* lista_para_liberar = NULL;
        No* prev = NULL;
        No* curr = paiAtual->primogenito; // Começa do primeiro filho de paiAtual.

        // Percorre a lista de filhos para encontrar o melhorFilho e separá-lo.
        while(curr != NULL && curr != melhorFilho) {
            prev = curr;
            curr = curr->irmao;
        }
        // Após o loop, 'curr' é o 'melhorFilho' (ou NULL se algo deu muito errado).
        // 'prev' é o irmão anterior ao 'melhorFilho' (ou NULL se 'melhorFilho' era o primeiro).

        if (curr == melhorFilho) { // Confirma que encontrou o melhorFilho.
            if (prev == NULL) { // Se melhorFilho era o primeiro da lista.
                // A lista a ser liberada começa a partir do irmão do melhorFilho.
                lista_para_liberar = melhorFilho->irmao;
            } else { // Se melhorFilho estava no meio ou fim da lista.
                prev->irmao = melhorFilho->irmao; // Remove melhorFilho da lista encadeada de irmãos.
                                                 // Agora 'prev' aponta para o irmão seguinte ao 'melhorFilho'.
                // A lista a ser liberada ainda começa do primogênito original do paiAtual,
                // mas o 'melhorFilho' já foi "desligado" dela.
                lista_para_liberar = paiAtual->primogenito;
            }
        } else { 
             // Se melhorFilho não foi encontrado na lista (não deveria acontecer),
             // libera toda a lista de filhos por segurança.
             lista_para_liberar = paiAtual->primogenito;
        }
        
        liberaArvore(lista_para_liberar); // Libera os nós que não são o 'melhorFilho'.
                                         // Se melhorFilho era o primeiro, lista_para_liberar é o resto.
                                         // Se melhorFilho estava no meio, lista_para_liberar é a lista original
                                         // mas com melhorFilho já "desviado".
        
        paiAtual->primogenito = melhorFilho; // Define o 'melhorFilho' como o único primogênito do 'paiAtual'.
        if(melhorFilho) melhorFilho->irmao = NULL; // Garante que o 'melhorFilho' não tem irmãos.

        // Avança o 'paiAtual' para ser o 'melhorFilho' para a próxima iteração.
        paiAtual = melhorFilho;

        // Atualiza os índices 'proximo_char' para cada sequência.
        // Incrementa se um caractere (não um gap) foi usado da sequência.
        for (int k = 0; k < nSeq; k++) {
            if (paiAtual->varSeq[k] != '-') {
                 proximo_char[k]++;
            }
        }
        // O '*final_len' não é incrementado aqui; ele será determinado por reconstroiAlinhamento.
    }

    // Após o loop principal, reconstrói o alinhamento final a partir do caminho na árvore.
    reconstroiAlinhamento(raiz, nSeq, final_seq, final_len);
    // Libera toda a memória alocada para a árvore.
    liberaArvore(raiz);
}


int verificaCharValidos(char seq[]) {
    // Se a string estiver vazia, considera-se inválida como uma sequência de DNA.
    if (seq[0] == '\0') {
        return 0;
    }

    for (int i = 0; seq[i] != '\0' && seq[i] != '\n'; i++) {
        // Converte o caractere atual para maiúsculo para fazer uma verificação case-insensitive.
        char c_upper = toupper(seq[i]);

        // Verifica se o caractere maiúsculo é uma das bases de DNA válidas.
        if (c_upper != 'A' && c_upper != 'C' && c_upper != 'T' && c_upper != 'G') {
            return 0; // Encontrou um caractere que não é A, C, T, ou G; retorna 0 (inválido).
        }
    }
    return 1;
}

void imprimirSequencia(char sequencia[][MAX_FINAL_LEN], int tamanhoSequencia, int maxLen_unused) {
    for (int i = 0; i < tamanhoSequencia; i++) { // Itera sobre cada sequência (linha).
        printf("%s\n", sequencia[i]); // Imprime a string da sequência seguida de uma nova linha.
    }
}

int lerSequencias(const char *nomeArquivo, char sequencias[][MAX_FINAL_LEN], int *maxLen) {
    FILE *arquivo; // Ponteiro para o arquivo.
    char linha[MAX_LEN + 10]; // Buffer para ler cada linha (MAX_LEN é para a seq. original, +10 de folga).
    int contador_validas = 0; // Contador para sequências válidas lidas.
    int numero_linha = 0;     // Contador para o número total de linhas lidas do arquivo.
    size_t len_atual = 0;     // Comprimento da linha atual.

    *maxLen = 0; // Inicializa o comprimento máximo.

    arquivo = fopen(nomeArquivo, "r"); // Tenta abrir o arquivo em modo de leitura ("r").
    if (arquivo == NULL) { // Se não conseguiu abrir o arquivo.
        perror("Erro ao abrir o arquivo"); // Imprime a mensagem de erro do sistema.
        return -1; // Retorna -1 indicando falha.
    }

    // Lê o arquivo linha por linha.
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        numero_linha++; // Incrementa o contador de linhas do arquivo.

        if (contador_validas >= MAX_SEQ) { // Verifica se o limite de sequências foi atingido.
            printf("Aviso: Limite de %d sequencias atingido. Linha %d e seguintes ignoradas.\n", MAX_SEQ, numero_linha);
            break; // Para de ler se o limite foi excedido.
        }

        linha[strcspn(linha, "\r\n")] = 0; // Remove caracteres de nova linha ('\n') e retorno de carro ('\r').

        if (strlen(linha) == 0) { // Pula linhas vazias.
            continue;
        }

        len_atual = strlen(linha);

        // Verifica se a sequência excede o comprimento máximo permitido (MAX_LEN - 3, que é 100).
        if (len_atual > (MAX_LEN - 3)) {
            printf("Aviso: Sequencia '%.80s...' (linha %d) excede 100 caracteres e sera ignorada.\n", linha, numero_linha);
            continue; // Pula para a próxima linha.
        }

        if (!verificaCharValidos(linha)) { // Verifica se a sequência contém apenas caracteres válidos.
            printf("Aviso: Sequencia '%s' (linha %d) contem caracteres invalidos e sera ignorada.\n", linha, numero_linha);
            continue; // Pula para a próxima linha.
        }

        // Se a sequência passou em todas as validações:
        strcpy(sequencias[contador_validas], linha); // Copia a linha para a matriz de sequências.
        if (len_atual > *maxLen) { // Atualiza o comprimento máximo se a linha atual for maior.
            *maxLen = len_atual;
        }
        contador_validas++; // Incrementa o contador de sequências válidas.
    }

    fclose(arquivo); // Fecha o arquivo.
    return contador_validas; // Retorna o número de sequências válidas lidas.
}

void preencheGapInicial(char seq[][MAX_FINAL_LEN], int nSeq, int maxLen) {
    for (int i = 0; i < nSeq; i++) { // Itera sobre cada sequência.
        int len_atual = strlen(seq[i]); // Pega o comprimento atual da sequência.
        int gaps_a_adicionar = maxLen - len_atual; // Calcula quantos gaps são necessários.

        if (gaps_a_adicionar > 0) { // Se precisa adicionar gaps.
            // Desloca os caracteres existentes da sequência para a direita.
            // 'memmove' é usado porque as regiões de origem e destino podem se sobrepor.
            // len_atual + 1 para incluir o terminador nulo '\0'.
            memmove(&seq[i][gaps_a_adicionar], &seq[i][0], len_atual + 1);

            // Preenche o início da string com o número calculado de gaps.
            memset(&seq[i][0], '-', gaps_a_adicionar);
        }
    }
}

void calcular_score(char sequencias[][MAX_FINAL_LEN], int lin, int col) {
    int score_total = 0;
    int alpha_count = 0; // Contador para matches.
    int beta_count = 0;  // Contador para mismatches.
    int delta_count = 0; // Contador para base-gap.

    // Itera por cada coluna do alinhamento.
    for (int i = 0; i < col; i++) {
        // Itera por cada par único de sequências (j, k) dentro da coluna atual.
        for (int j = 0; j < lin - 1; j++) {
            for (int k = j + 1; k < lin; k++) {
                char base1 = sequencias[j][i]; // Caractere da sequência j na coluna i.
                char base2 = sequencias[k][i]; // Caractere da sequência k na coluna i.

                if (base1 == '-' && base2 == '-') { // Se ambos são gaps.
                    score_total += GAP_GAP; // Adiciona o score de gap-gap (definido como 0).
                }
                else if (base1 == '-' || base2 == '-') { // Se um é gap e o outro é base.
                    score_total += DELTA; // Adiciona o score DELTA.
                    delta_count++;
                }
                else { // Se ambos são bases.
                    if (base1 == base2) { // Se as bases são iguais (match).
                        score_total += ALPHA; // Adiciona o score ALPHA.
                        alpha_count++;
                    }
                    else { // Se as bases são diferentes (mismatch).
                        score_total += BETA; // Adiciona o score BETA.
                        beta_count++;
                    }
                }
            }
        }
    }

    // Imprime o resultado detalhado do score.
    printf("(α * %d) + (β * %d) + (δ * %d)", alpha_count, beta_count, delta_count);
    score_total > 0 ? printf(" = +%d\n", score_total) : printf(" = %d\n", score_total);
}

int calcScoreColuna(char vet[], int nSeq) {
    int score = 0; // Inicializa o score da coluna.

    // Itera por cada par único de caracteres (i, j) dentro do vetor 'vet'.
    for (int i = 0; i < nSeq - 1; i++) {
        for (int j = i + 1; j < nSeq; j++) {
            char base1 = vet[i];
            char base2 = vet[j];

            if (base1 == '-' && base2 == '-') { // Gap + Gap.
                score += GAP_GAP;
            }
            else if (base1 == '-' || base2 == '-') { // Gap + Base.
                score += DELTA;
            }
            else { // Base + Base.
                if (base1 == base2) { // Match.
                    score += ALPHA;
                }
                else { // Mismatch.
                    score += BETA;
                }
            }
        }
    }
    return score; // Retorna o score da coluna.
}

int todas_terminaram(int proximo_char[], int lengths[], int nSeq) {
    for (int i = 0; i < nSeq; i++) { // Itera sobre cada sequência.
        // Se o índice do próximo caractere for menor que o comprimento da sequência,
        // significa que esta sequência ainda não terminou.
        if (proximo_char[i] < lengths[i]) {
            return 0; // Retorna 0 (falso) imediatamente.
        }
    }
    return 1; // Se o loop completou, todas as sequências terminaram; retorna 1 (verdadeiro).
}