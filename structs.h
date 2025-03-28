#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Mapeamento de caracteres para valores numéricos
typedef struct {
    char letra;
    int valor;
} Mapeamento;

Mapeamento tabela[] = {
    {'q', 1}, {'w', 6}, {'e', 7}, {'r', 6}, {'t', 5}, {'y', 2},
    {'u', 3}, {'i', 8}, {'o', 9}, {'p', 4}, {'á', 3}, {'ã', 4},
    {'a', 2}, {'s', 5}, {'d', 8}, {'f', 7}, {'g', 4}, {'h', 1},
    {'j', 4}, {'k', 7}, {'l', 8}, {'ç', 5}, {'é', 2}, {'í', 3},
    {'z', 3}, {'x', 4}, {'c', 9}, {'v', 8}, {'b', 3}, {'n', 2},
    {'m', 5}, {'ó', 6}, {'õ', 7}, {'ô', 6}, {'â', 1}, {'ê', 2}
}; //Mapemamento Letra para Número

typedef struct {
    char req[3][8];
    int  qtd_reqs; // quantidade de pré-requisitos
}Requisito;

typedef struct {
    char nome[50];
    char codigo[8];
    int carga_horaria;
    char horario[10];
    int pre_req; // 0 se não houver pré-requisito, 1 se houver
    Requisito req;
}Disciplina;

Disciplina quadro[] = {
    {"PROGRAMACAO 1", "COMP359", 72, "06M3456", 0}, {"LOGICA PARA COMPUTACAO", "COMP360", 72, "24M34", 0}, {"COMPUTACAO SOCIEDADE E ETICA", "COMP361", 72, "24M56", 0}, {"MATEMATICA DISCRETA", "COMP362", 72, "24M12", 0}, {"CALCULO DIFERENCIAL E INTEGRAL", "COMP363", 144, "356M234", 0}, {"ESTRUTURA DE DADOS", "COMP364", 72, "24T34", 1, "COMP359", "*", "*", 1}, {"BANCO DE DADOS", "COMP365", 72, "35T12", 0}, {"ORGANIZACAO E ARQUITETURA DE COMPUTADORES", "COMP366", 72, "24T12", 0}, {"GEOMETRIA ANALITICA", "COMP367", 72, "24M34", 0}, {"REDES DE COMPUTADORES", "COMP368", 72, "34M56", 1, "COMP359", "*", "*", 1}, {"TEORIA DOS GRAFOS", "COMP369", 72, "25M34", 1, "COMP364", "COMP362", "*", 2}, {"PROBABILIDADE E ESTATISTICA", "COMP370", 72, "24T12", 1, "COMP363", "*", "*", 1}, {"ALGEBRA LINEAR", "COMP371", 72, "24T34", 1, "COMP367", "*", "*", 1}, {"PROGRAMACAO 2", "COMP372", 72, "35T12", 1, "COMP364", "COMP365", "COMP368", 3}, {"PROGRAMACAO 3", "COMP373", 72, "34T34", 1, "COMP364", "COMP365", "COMP368", 3}, {"PROJETO E ANALISE DE ALGORITMOS", "COMP374", 72, "25M56", 1, "COMP364", "COMP369", "*", 2}, {"TEORIA DA COMPUTACAO", "COMP376", 72, "24T12", 0}, {"SISTEMAS OPERACIONAIS", "COMP378", 72, "24M34", 1, "COMP366", "*", "*", 1}, {"COMPILADORES", "COMP379", 72, "24M56", 1, "COMP364", "COMP376", "*", 2}, {"INTELIGENCIA ARTIFICIAL", "COMP380", 72, "35M34", 1, "COMP360", "COMP364", "*", 2}, {"COMPUTACAO GRAFICA", "COMP381", 72, "36M56", 0}, {"PROJETO E DESENVOLVIMENTO DE SISTEMAS", "COMP382", 288, "2356T1234", 1, "COMP364", "COMP369", "COMP374", 3}, {"METODOLOGIA DE PESQUISA E TRABALHO INDIVIDUAL", "COMP386", 72, "36M34", 0}, {"NOCOES DE DIREITO", "COMP387", 72, "06T1234", 0}, {"CALCULO 3", "COMP404", 72, "35T12", 1, "COMP363", "*", "*", 1}, {"CONCEITOS DE LINGUAGEM DE PROGRAMACAO", "COMP389", 72, "35T34", 0}, {"APRENDIZAGEM DE MAQUINA", "COMP390", 72, "24T56", 1, "COMP404", "*", "*", 1}, {"SISTEMAS DIGITAIS", "COMP391", 72, "56M34", 1, "COMP404", "*", "*", 1}, {"SISTEMAS DISTRIBUIDOS", "COMP392", 72, "06T1234", 0}, {"REDES NEURAIS E APRENDIZADO PROFUNDO", "COMP393", 72, "35T34", 0}, {"INTERACAO HOMEM-MAQUINA", "COMP395", 72, "24T34", 1, "COMP373", "*", "*", 1}, {"PROCESSAMENTO DIGITAL DE IMAGENS", "COMP396", 72, "35T12", 1, "COMP381", "*", "*", 1}, {"COMPUTACAO EVOLUCIONARIA", "COMP397", 72, "24M12", 0}, {"SISTEMAS EMBARCADOS", "COMP398", 72, "35T56", 0}, {"GERENCIA DE PROJETO", "COMP399", 72, "35T56", 1, "COMP382", "*", "*", 1}, {"CIENCIA DE DADOS", "COMP401", 72, "24M34", 1, "COMP370", "*", "*", 1}, {"MICROCONTROLADORES E APLICACOES", "COMP402", 72, "24M56", 0}, {"SEGURANCA DE SISTEMAS COMPUTACIONAIS", "COMP403", 72, "46M34", 1, "COMP368", "*", "*", 1}
}; // Faltou a disciplina "FPGA" que tá no ppc mas não tava no horário, "Visão Computacional" também não foi ofertada