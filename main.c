#include "functions.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    char nome_completo[200];
    printf("Digite o nome completo do aluno: ");
    fgets(nome_completo, 200, stdin);
    nome_completo[strcspn(nome_completo, "\n")] = 0;

    converter_para_minusculo(nome_completo); // Converte para minúsculas
    
    planejamento_academico(nome_completo);
    return 0;
}
