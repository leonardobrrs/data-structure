// main.c
#include "functions.h" // Inclui os protótipos de todas as funções, definições e outras bibliotecas necessárias

int main() {
    // Declaração das matrizes para armazenar as sequências:
    // - sequencias: armazena as sequências originais lidas do arquivo.
    // - sequencias_inicial: armazena uma cópia para o alinhamento inicial com gaps no começo.
    // - melhor_alinhamento: armazena o resultado do alinhamento final.
    // MAX_FINAL_LEN permite que as sequências cresçam com a inserção de gaps.
    char sequencias[MAX_SEQ][MAX_FINAL_LEN];
    char sequencias_inicial[MAX_SEQ][MAX_FINAL_LEN];
    char melhor_alinhamento[MAX_SEQ][MAX_FINAL_LEN];
    
    // --- INICIALIZAÇÃO DAS MATRIZES ---
    // Preenche as matrizes com zeros (caractere nulo '\0') para evitar lixo de memória.
    // Isso garante que as strings estejam devidamente terminadas antes de qualquer uso.
    memset(sequencias, 0, sizeof(sequencias));
    memset(sequencias_inicial, 0, sizeof(sequencias_inicial));
    memset(melhor_alinhamento, 0, sizeof(melhor_alinhamento));
    // ---------------------------------

    // Variáveis para controle e informações das sequências:
    int sequencias_count = 0;     // Número de sequências lidas do arquivo.
    int max_string_size = 0;    // Comprimento da maior sequência original.
    int final_len = 0;          // Comprimento do alinhamento final (após inserção de gaps).
    int lengths[MAX_SEQ];       // Array para armazenar os comprimentos pré-calculados de cada sequência original.

    char nome_arquivo_base[100];        // Buffer para armazenar o nome do arquivo digitado pelo usuário.
    char caminho_completo_arquivo[150]; // Buffer para construir o caminho completo (ex: "data/arquivo.txt").

    limparTela(); // Função para limpar a tela do console (específica do sistema).
    printf("\n\t******** Alinhamento Multiplo de DNA - Nova Versao ********\n\n");

    // --- Leitura do Arquivo ---
    printf("Digite o nome do arquivo .txt com as sequencias (ex: data.txt): ");
    scanf("%99s", nome_arquivo_base); // Lê o nome base do arquivo (até 99 caracteres para segurança).
    
    // Constrói o caminho completo, assumindo que os arquivos de dados estão em uma subpasta "data".
    sprintf(caminho_completo_arquivo, "data/%s", nome_arquivo_base);

    // Chama a função para ler as sequências do arquivo especificado.
    // Retorna o número de sequências lidas e atualiza max_string_size.
    sequencias_count = lerSequencias(caminho_completo_arquivo, sequencias, &max_string_size);

    // Verifica se a leitura foi bem-sucedida e se há sequências suficientes.
    if (sequencias_count <= 0) {
        printf("Erro ao ler o arquivo '%s' ou nenhuma sequencia valida encontrada.\n", caminho_completo_arquivo); 
        return 1; // Termina o programa se houver erro na leitura.
    }
    if (sequencias_count < 2) {
         printf("Sao necessarias pelo menos 2 sequencias para o alinhamento.\n");
         return 1; // Termina se não houver pelo menos duas sequências.
    }

    printf("\nLidas %d sequencias. Tamanho maximo: %d\n", sequencias_count, max_string_size);

    // Pré-calcula e armazena o comprimento de cada sequência original.
    // Isso evita chamadas repetidas a strlen() dentro dos algoritmos.
    for(int i = 0; i < sequencias_count; i++) {
        lengths[i] = strlen(sequencias[i]);
    }

    // Copia as sequências originais para 'sequencias_inicial'.
    // 'sequencias_inicial' será modificada para mostrar o alinhamento com gaps no início.
    for(int i = 0; i < sequencias_count; i++) {
        strcpy(sequencias_inicial[i], sequencias[i]);
    }

    // --- Alinhamento e Score Inicial ---
    printf("\nAlinhamento com gaps no inicio:\n\n");
    // Preenche o início das sequências em 'sequencias_inicial' com gaps
    // para que todas tenham o mesmo comprimento (max_string_size).
    preencheGapInicial(sequencias_inicial, sequencias_count, max_string_size);
    // Imprime o alinhamento inicial.
    imprimirSequencia(sequencias_inicial, sequencias_count, max_string_size);
    
    printf("\nSCORE INICIAL:\n");
    // Calcula e imprime o score do alinhamento inicial.
    calcular_score(sequencias_inicial, sequencias_count, max_string_size);

    // --- Melhor Alinhamento ---
    printf("\nCalculando melhor alinhamento...\n");
    // Chama a função principal de alinhamento, que usa a árvore N-ária e a lógica gulosa.
    // As sequências originais e seus comprimentos pré-calculados são passados.
    // O resultado é armazenado em 'melhor_alinhamento' e seu comprimento em 'final_len'.
    alinhaComArvore(sequencias, lengths, sequencias_count, melhor_alinhamento, &final_len);

    // --- Resultado Final ---
    printf("\nMelhor alinhamento encontrado:\n\n");
    // Imprime o melhor alinhamento obtido.
    imprimirSequencia(melhor_alinhamento, sequencias_count, final_len);
    
    printf("\nSCORE FINAL:\n");
    // Calcula e imprime o score do melhor alinhamento.
    calcular_score(melhor_alinhamento, sequencias_count, final_len);

    printf("\n\n"); // Adiciona espaço extra no final da saída.

    return 0; // Indica que o programa terminou com sucesso.
}