// structs.h

// Include Guard: Evita que o conteúdo deste arquivo seja incluído múltiplas vezes
// se o arquivo for chamado em diferentes partes do projeto.
#ifndef STRUCTS_H  // Se STRUCTS_H não foi definido ainda...
#define STRUCTS_H  // ...define STRUCTS_H agora.

#include <stdlib.h> // Inclui a biblioteca padrão para funções como malloc (alocação de memória) e NULL.
#include <string.h> // Inclui a biblioteca padrão para funções de manipulação de strings, como strcpy.
#include "functions.h" // Inclui o arquivo de cabeçalho "functions.h".
                       // Presumivelmente, para ter acesso a definições como MAX_SEQ,
                       // embora MAX_SEQ não seja diretamente usado na definição de MAX_SEQ_STR_LEN abaixo,
                       // o tamanho da coluna (varSeq) depende do número de sequências.

// Define o tamanho máximo da string que armazena uma coluna na árvore.
// Se temos até 50 sequências (MAX_SEQ), e cada caractere na coluna representa
// uma base de uma sequência, então a string precisa de 50 caracteres + 1 para o terminador nulo '\0'.
#define MAX_SEQ_STR_LEN (50 + 1)

// Typedef para a estrutura 'no' (nó da árvore N-ária).
// Esta estrutura representa um nó na árvore que será usada para
// construir e avaliar os alinhamentos.
typedef struct no {
    // Campo para armazenar a representação da coluna de alinhamento deste nó.
    // Por exemplo, se há 3 sequências, poderia ser "A-C".
    char varSeq[MAX_SEQ_STR_LEN];

    // Ponteiro para o primeiro filho deste nó na árvore.
    // Se este nó tem derivações (próximas colunas possíveis), 'primogenito' aponta para a primeira delas.
    struct no *primogenito;

    // Ponteiro para o próximo nó irmão.
    // Se o nó pai tem múltiplos filhos (múltiplas variações para a próxima coluna),
    // este ponteiro liga um filho ao seu próximo irmão no mesmo nível.
    struct no *irmao;

    // Campo para armazenar o score associado à coluna representada por 'varSeq'.
    // Utilizado para decidir o "melhor" caminho na abordagem gulosa.
    int score;

} No; // 'No' agora é um tipo que pode ser usado para declarar variáveis desta estrutura.

#endif // STRUCTS_H // Fim do bloco do include guard.