# Alinhamento Múltiplo de Sequências de DNA

Este é um projeto desenvolvido para a disciplina de Estrutura de Dados da Universidade Federal de Alagoas (UFAL).

**Universidade:** Universidade Federal de Alagoas (UFAL).
**Disciplina:** Estrutura de Dados.
**Professora:** Roberta Vilhena Vieira Lopes.
**Alunos:**
* Leonardo Barbosa Barros
* José Herberty de Oliveira Freire

## Sobre o Projeto

O objetivo deste projeto é implementar um algoritmo para realizar o Alinhamento Múltiplo de Sequências (Multiple Sequence Alignment - MSA) de DNA. O MSA é uma ferramenta fundamental em bioinformática, usada para identificar regiões conservadas, prever funções de genes e proteínas, e estudar relações evolutivas.

Este projeto utiliza uma abordagem gulosa (greedy) com uma Árvore N-ária para encontrar um alinhamento com boa pontuação. Dada um conjunto de sequências de DNA lidas de um arquivo, o programa constrói iterativamente um alinhamento, coluna por coluna, escolhendo a melhor opção em cada passo com base em um sistema de pontuação.

## Implementação

O algoritmo funciona da seguinte maneira:

1.  **Leitura e Validação:** As sequências de DNA são lidas de um arquivo de texto. Cada linha representa uma sequência. O programa valida se as sequências contêm apenas caracteres válidos (A, C, T, G) e se não excedem o limite de 100 caracteres. Sequências inválidas ou muito longas são ignoradas.
2.  **Árvore N-ária:** Uma árvore N-ária é utilizada para explorar as possíveis colunas do alinhamento. Cada nó na árvore representa uma possível coluna de alinhamento (`varSeq`), e seus filhos (`primogenito`) representam as possíveis colunas seguintes.
3.  **Geração de Candidatos:** Em cada passo, o algoritmo gera um conjunto de colunas candidatas. Estas incluem uma coluna onde se tenta usar o próximo caractere de cada sequência e colunas onde se introduz um *gap* ('-') em uma das sequências.
4.  **Sistema de Pontuação (Sum-of-Pairs - SP):** Cada coluna candidata é avaliada com base em um sistema de pontuação *Sum-of-Pairs*. A pontuação é calculada somando os scores de todos os pares de caracteres na coluna, usando os seguintes valores:
    * **ALPHA (Match):** +1 (Bases idênticas)
    * **BETA (Mismatch):** 0 (Bases diferentes)
    * **DELTA (Base-Gap):** -2 (Uma base e um gap)
    * **GAP_GAP (Gap-Gap):** 0 (Dois gaps)
5.  **Abordagem Gulosa:** O algoritmo escolhe a coluna candidata com o maior score (`devolveMelhorFilho`) para adicionar ao alinhamento. A árvore é então "podada", mantendo apenas o caminho correspondente à escolha gulosa, e o processo continua a partir deste novo nó. Uma lógica é implementada para forçar o progresso caso a melhor escolha seja uma coluna apenas com gaps.
6.  **Reconstrução:** Após processar todas as sequências, o alinhamento final é reconstruído percorrendo o caminho escolhido na árvore (`reconstroiAlinhamento`).
7.  **Cálculo de Score Final:** O score total do alinhamento final é calculado (`calcular_score`) e exibido.

## Como Compilar e Executar

Certifique-se de ter um compilador C (como o GCC) instalado.

1.  **Navegue até a pasta do projeto:**
    ```bash
    cd caminho/para/o/projeto
    ```
2.  **Compile os arquivos:**
    ```bash
    gcc main.c functions.c -o main -Wall
    ```
3.  **Execute o programa:**
    ```bash
    ./main
    ```
4.  **Informe o nome do arquivo:** O programa solicitará que você digite o nome do arquivo `.txt` contendo as sequências (ex: `data.txt`). Os arquivos de dados devem estar localizados na subpasta `data/`.

## Estrutura de Arquivos

* `main.c`: Contém a função principal (`main`) que orquestra a execução do programa, incluindo a leitura do arquivo, chamada das funções de alinhamento e exibição dos resultados.
* `functions.h`: Arquivo de cabeçalho que define as constantes (como os scores ALPHA, BETA, DELTA), as estruturas (`No`), e os protótipos de todas as funções utilizadas no projeto.
* `functions.c`: Contém a implementação de todas as funções, incluindo a manipulação da árvore N-ária, a lógica do alinhamento guloso, o cálculo de scores, a leitura e validação de arquivos.
* `structs.h`: Define a estrutura `No` utilizada para construir a árvore N-ária.
* `data/`: Pasta contendo os arquivos de entrada (`.txt`) com as sequências de DNA. Inclui `data.txt` e vários arquivos `teste*.txt` para fins de avaliação.
* `README.md`: Este arquivo.

## Arquivos de Entrada

Os arquivos de entrada devem:

* Estar no formato `.txt`.
* Estar localizados na pasta `data/`.
* Conter uma sequência de DNA por linha.
* Usar apenas os caracteres 'A', 'C', 'T', 'G' (maiúsculas ou minúsculas).
* Ter sequências com no máximo 100 caracteres.