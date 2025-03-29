#include "functions.h"

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    char nome_completo[200];
    printf("Digite o nome completo do aluno: ");
    fgets(nome_completo, 200, stdin);
    nome_completo[strcspn(nome_completo, "\n")] = 0;
    printf("\n");

    converter_para_minusculo(nome_completo); // Converte para minúsculas
    
    planejamento_academico(nome_completo);


    FILE *arquivo = fopen("data/dados.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    Historico historico;
    historico.numeroPeriodos = -1; // Inicializa com valor inválido
    historico.quantidadeDisciplinas = 0; // Inicializa contador de disciplinas
    char linha[MAX_LINHA];

    // Capturar a primeira linha para obter o número de períodos
    if (fgets(linha, MAX_LINHA, arquivo)) {
        char *token = strtok(linha, ";\n");
        if (token) {
            token = strtok(NULL, ";\n"); // Captura o número de períodos
            if (token) {
                historico.numeroPeriodos = atoi(token); // Converte para inteiro
            }
        }
    }

    // Processamento das disciplinas
    while (fgets(linha, MAX_LINHA, arquivo) && historico.quantidadeDisciplinas < MAX_DISCIPLINAS) {
        char *token = strtok(linha, ";\n");
        if (!token) continue;

        strcpy(historico.disciplinas[historico.quantidadeDisciplinas].codigo, token);
        
        token = strtok(NULL, ";\n");
        if (token) strcpy(historico.disciplinas[historico.quantidadeDisciplinas].nome, token);

        token = strtok(NULL, ";\n");
        if (token) {
            substituirVirgulaPorPonto(token);  // Trocar ',' por '.' antes da conversão
            historico.disciplinas[historico.quantidadeDisciplinas].nota = atof(token);  // Convertendo string para float
        } else {
            historico.disciplinas[historico.quantidadeDisciplinas].nota = 0;
        }

        historico.quantidadeDisciplinas++; // Incrementa o contador de disciplinas
    }

    fclose(arquivo);

    printf("\n");
    // Exibir os dados armazenados
    printf("Número de períodos cursados: %d\n", historico.numeroPeriodos);
    printf("Quantidade de disciplinas: %d\n", historico.quantidadeDisciplinas);
    printf("Histórico:\n");
    for (int i = 0; i < historico.quantidadeDisciplinas; i++) {
        printf("Código: %s | Nome: %s | Nota: %.2f\n", 
               historico.disciplinas[i].codigo, 
               historico.disciplinas[i].nome, 
               historico.disciplinas[i].nota);
    }

    return 0;
}
