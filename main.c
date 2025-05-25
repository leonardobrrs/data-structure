#include "functions.h"

int main() {
    srand(time(NULL));

    printf("Usando pesos predefinidos: ALPHA=%d, BETA=%d, DELTA=%d\n", ALPHA, BETA, DELTA);

    char sequencias_orig[MAX_SEQS][MAX_LEN_ALN];
    int n_sequencias = 0;
    int max_len_inicial = 0;
    int orig_lengths[MAX_SEQS];
    char filename[100];

    printf("Digite o nome do arquivo .txt com as sequencias (ex: dna.txt): ");
    scanf("%99s", filename);

    if (lerSequencias(filename, sequencias_orig, &n_sequencias, &max_len_inicial, orig_lengths) != 0) {
        return 1;
    }

    printf("\nSequencias Lidas:\n");
    for(int i = 0; i < n_sequencias; i++) {
        printf("%d (%d): %s\n", i+1, orig_lengths[i], sequencias_orig[i]);
    }

    int max_len_alinhado = max_len_inicial + (max_len_inicial / 3);
    if (max_len_alinhado >= MAX_LEN_ALN) max_len_alinhado = MAX_LEN_ALN - 1;
    printf("Tamanho maximo estimado para alinhamento: %d\n", max_len_alinhado);
    printf("Usando Beam Search com K = %d\n", K_BEAM_WIDTH);

    // --- Construção da Árvore (Beam Search) ---
    No* raiz = criaNovoNo("RAIZ", NULL, -1, 0.0);
    
    No* current_beam[K_BEAM_WIDTH]; // Array para guardar os K melhores nós atuais
    int n_current_beam = 1;       // Começa com 1 nó (a raiz)
    current_beam[0] = raiz;

    int col_final = 0; // Guarda o tamanho real do alinhamento

    printf("\nIniciando construcao da arvore (Beam Search)...\n");

    // Loop principal: Itera por cada coluna do alinhamento
    for (int col = 0; col < max_len_alinhado; col++) {
        
        // Array grande o suficiente para guardar TODOS os filhos gerados neste nível
        No* candidates[K_BEAM_WIDTH * (MAX_SEQS + 5)]; 
        int n_candidates = 0;

        // 1. GERAÇÃO: Para cada nó no feixe atual, gere seus filhos
        for (int i = 0; i < n_current_beam; i++) {
            No* pai_atual = current_beam[i];
            
            // Gera os filhos e os adiciona à árvore
            gerarVariacoes(pai_atual, n_sequencias, sequencias_orig, orig_lengths, col);

            // 2. COLETA: Adiciona os filhos recém-criados ao array de candidatos
            No* filho = pai_atual->primogenito;
            while (filho != NULL) {
                // Adiciona apenas se for da coluna atual e houver espaço no array
                if (filho->coluna_index == col && n_candidates < (K_BEAM_WIDTH * (MAX_SEQS + 5))) {
                    candidates[n_candidates++] = filho;
                }
                filho = filho->irmao;
            }
        }

        // Se nenhum candidato foi gerado, a busca para.
        if (n_candidates == 0) {
            printf("Fim da arvore na coluna %d (sem candidatos).\n", col);
            col_final = col;
            break;
        }

        // 3. ORDENAÇÃO: Ordena todos os candidatos pelo score (maior primeiro)
        qsort(candidates, n_candidates, sizeof(No*), compare_nodes);

        // 4. SELEÇÃO MODIFICADA: Prioriza nós com bases!
        n_current_beam = 0;
        No* new_beam[K_BEAM_WIDTH];
        No* best_gap_node = NULL; // Guarda o melhor nó '---'

        // 4a. Tenta preencher o feixe APENAS com nós que contêm bases.
        for (int i = 0; i < n_candidates && n_current_beam < K_BEAM_WIDTH; i++) {
            if (!is_all_gaps(candidates[i]->varSeq, n_sequencias)) {
                new_beam[n_current_beam++] = candidates[i];
            } else if (best_gap_node == NULL) {
                // Guarda o PRIMEIRO (e portanto melhor) nó '---' encontrado
                best_gap_node = candidates[i];
            }
        }

        // 4b. Se o feixe não ficou cheio E existe um nó '---', completa com ele.
        //    (Isso só deve acontecer se todas as sequências acabaram).
        if (n_current_beam == 0 && best_gap_node != NULL) {
             new_beam[n_current_beam++] = best_gap_node;
        }

        // 4c. Copia o feixe selecionado para o próximo ciclo.
        for(int i=0; i<n_current_beam; i++) {
            current_beam[i] = new_beam[i];
        }

        // Se, mesmo com a nova lógica, não selecionamos ninguém, paramos.
        if (n_current_beam == 0) {
             printf("Fim da arvore na coluna %d (sem candidatos viaveis).\n", col);
             col_final = col;
             break;
        }
        
        col_final = col + 1; // Atualiza a última coluna processada
    }

    // --- Resultado ---
    // O melhor nó final estará no feixe atual, provavelmente o primeiro (já que está ordenado)
    No* melhor_no = NULL;
    if (n_current_beam > 0) {
        melhor_no = current_beam[0]; // Pega o primeiro (maior score)
    }
    
    // Procura o de maior score explicitamente, por segurança
    double max_score_final = -DBL_MAX;
     for(int i = 0; i < n_current_beam; i++) {
        if(current_beam[i]->score > max_score_final) {
            max_score_final = current_beam[i]->score;
            melhor_no = current_beam[i];
        }
    }


    if (melhor_no && melhor_no != raiz) {
        char alinhamento_final[MAX_SEQS][MAX_LEN_ALN];
        reconstruirAlinhamento(melhor_no, alinhamento_final, n_sequencias, col_final); 
        printf("\nMelhor Alinhamento Encontrado (Score: %.2f):\n", melhor_no->score);
        imprimirAlinhamento(alinhamento_final, n_sequencias);
    } else {
        printf("Nao foi possivel encontrar um alinhamento.\n");
    }

    // --- Limpeza ---
    printf("\nLiberando memoria...\n");
    liberaArvore(raiz); // Libera a árvore INTEIRA
    printf("Memoria liberada.\n");
    printf("\nPrograma finalizado.\n");

    return 0;
}