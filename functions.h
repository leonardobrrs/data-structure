#include "structs.h"

// Função para converter vírgula para ponto em números decimais
void substituirVirgulaPorPonto(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
}

void converter_para_minusculo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int calcular_peso(const char *nome) {
    int soma = 0;
    for (int i = 0; i < strlen(nome); i++) {
        for (int j = 0; j < TAM_TABELA; j++) {
            if (nome[i] == tabela[j].letra) {
                soma += tabela[j].valor;
                break;
            }
        }
    }
    return soma;
}

int determinar_max_disciplinas(int peso) {
    int resto = peso % 3;
    return resto;
}

int determinar_tempo_conclusao(int peso) {
    int resto = peso % 3;
    return resto;
}

int determinar_enfase(int peso) {
    int resto = peso % 3;
    return resto;
}

int determinar_distribuicao_disciplinas(int peso) {
    int resto = peso % 3;
    return resto;
}

void planejamento_academico(char *nome_completo) {
    char nome[10][50];  // Suporta até 10 nomes
    int num_nomes = 0;
    char *token = strtok(nome_completo, " ");
    
    while (token != NULL && num_nomes < 10) {
        strcpy(nome[num_nomes], token);
        num_nomes++;
        token = strtok(NULL, " ");
    }
    
    if (num_nomes == 0) return; // Caso de entrada vazia

    int peso1 = calcular_peso(nome[0]);
    int max_disciplinas = determinar_max_disciplinas(peso1);
    int peso2 = calcular_peso(nome[1 % num_nomes]);
    int tempo_conclusao = determinar_tempo_conclusao(peso2);
    int peso3 = calcular_peso(nome[2 % num_nomes]);
    int enfase = determinar_enfase(peso3);
    int peso4 = calcular_peso(nome[3 % num_nomes]);
    int distribuicao_disciplinas = determinar_distribuicao_disciplinas(peso4);


    if(max_disciplinas == 0){
        printf("Peso máximo de disciplinas: %d, Resto: %d (Limite máximo: 10)\n", peso1, max_disciplinas);
    } else if( max_disciplinas == 1){
        printf("Peso máximo de disciplinas: %d, Resto: %d (Limite máximo: 8)\n", peso1, max_disciplinas);
    } else{
        printf("Peso máximo de disciplinas: %d, Resto: %d (Limite máximo: 6)\n", peso1, max_disciplinas);
    }

    if (tempo_conclusao == 0){
        printf("Peso do tempo de conclusão: %d, Resto: %d (No menor tempo possível)\n", peso2, tempo_conclusao);
    } else if (tempo_conclusao == 1){
        printf("Peso do tempo de conclusão: %d, Resto: %d (No maior tempo possível)\n", peso2, tempo_conclusao);
    } else {
        printf("Peso do tempo de conclusão: %d, Resto %d (Tempo medio possível)\n", peso2, tempo_conclusao);
    }

    if (enfase == 0){
        printf("Peso da ênfase: %d, Resto: %d (Ênfase desejada pelo aluno)\n", peso3, enfase);
    } else if (enfase == 1){
        printf("Peso da ênfase: %d, Resto: %d (Ênfase que diminui o tempo de conclusão do curso)\n", peso3, enfase);
    } else {
        printf("Peso da ênfase: %d, Resto: %d (Nenhuma ênfase)\n", peso3, enfase);
    }
    
    if (distribuicao_disciplinas == 0){
        printf("Peso da distribuição das disciplinas: %d, Resto: %d (As disciplinas devem ocupar o menor número de dias possíveis)\n", peso4, distribuicao_disciplinas);
    } else if (distribuicao_disciplinas == 1){
        printf("Peso da distribuição das disciplinas: %d, Resto %d (as disciplinas devem pertencer a um mesmo turno)\n", peso4, distribuicao_disciplinas);
    } else {
        printf("Peso da distribuição das disciplinas: %d, Resto: %d (o aluno não deve extrapolar o limite de 3 disciplinas no máximo por dia, mas deve ir todos os dias a ufal)\n", peso4, distribuicao_disciplinas);
    }
}