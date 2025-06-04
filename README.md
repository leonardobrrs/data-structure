# Alinhamento Múltiplo de Sequências de DNA

Este projeto foi desenvolvido para a disciplina de Estrutura de Dados da Universidade Federal de Alagoas (UFAL). O objetivo é implementar um programa que realiza a operação de alinhamento múltiplo de sequências de DNA.

**Universidade:**
* Universidade Federal de Alagoas (UFAL)

**Disciplina:**
* Estrutura de Dados

**Professora:**
* Roberta Vilhena Vieira Lopes

**Alunos:**
* Leonardo Barbosa Barros
* José Herberty de Oliveira Freire

---

## Sobre o Projeto

O alinhamento múltiplo de sequências (MSA) é um processo fundamental em bioinformática que consiste em alinhar três ou mais sequências biológicas (como DNA, RNA ou proteínas) para identificar regiões de similaridade que podem indicar relações funcionais, estruturais ou evolutivas entre as sequências.

Este projeto visa implementar um programa para realizar o alinhamento múltiplo de sequências de DNA. A entrada é um conjunto de sequências de DNA ($S_{input} = \{s_1, s_2, ..., s_i, ...\}$) com tamanhos variando até um máximo de **100 caracteres**. O resultado ($S_{output} = \{r_1, r_2, ..., r_i, ...\}$) é um conjunto de sequências, todas com o mesmo comprimento (m), construídas pela inserção de *gaps* (buracos, representados por "-") nas sequências originais.

O objetivo é que, para toda posição $j$ (variando de 1 a $m$), o valor da fórmula de pontuação (*score*) seja maximizado. A fórmula de *score* é:
$score = \alpha x + \beta y + \delta z$

Onde:
* $\alpha$: Peso para $x$ pares de bases nitrogenadas iguais na mesma posição.
* $\beta$: Peso para $y$ pares de bases nitrogenadas diferentes na mesma posição.
* $\delta$: Peso para $z$ pares formados por um *gap* e uma base nitrogenada na mesma posição.

Além disso, para cada sequência $s_k$ em $S_{input}$, deve existir uma única sequência $r_k$ em $S_{output}$ tal que a remoção dos *gaps* de $r_k$ reproduza $s_k$.

1.  As sequências são lidas de um arquivo de entrada.
2.  A sequência mais longa é escolhida como referência inicial.
3.  As demais sequências são alinhadas progressivamente a esta referência (que pode ser atualizada/expandida) usando o algoritmo de Needleman-Wunsch para alinhamentos par a par.

---

## Cálculo dos Pesos e Preferência de Gaps (Baseado em "José Freire")

Os pesos $\alpha$, $\beta$, e $\delta$, bem como a preferência de posicionamento de *gaps*, são determinados com base no nome de um dos membros da equipe: **José Herberty de Oliveira Freire** (utilizando "José" como primeiro nome e "Freire" como último nome para os cálculos).

### Cálculo dos Pesos (α, β, δ) - Nome: "José"

1.  **Substituição das letras por números (conforme tabela do projeto):**
    * J = 11
    * O = 9
    * S = 9
    * É = 3
    Soma = $11 + 9 + 9 + 3 = 32$

2.  **Divisão por 3 e verificação do resto:**
    * $32 \div 3 = 10$ com resto $2$.

3.  **Determinação dos pesos com base no resto:**
    * Resto 0: $\alpha= +1, \beta= 0, \delta= -1$
    * Resto 1: $\alpha= +2, \beta= 0, \delta= -1$
    * **Resto 2: $\alpha= +1, \beta= 0, \delta= -2$**

Portanto, os pesos utilizados são:
* $\alpha = +1$
* $\beta = 0$
* $\delta = -2$

Esses valores estão definidos no arquivo `structs.h` como `ALPHA`, `BETA`, e `DELTA` respectivamente. O arquivo também define `GAP_GAP = 0` para o alinhamento entre dois *gaps*.

### Preferência de Gaps - Sobrenome: "Freire"

1.  **Substituição das letras por números (conforme tabela do projeto):**
    * F = 5
    * R = 4
    * E = 3
    * I = 8
    * R = 4
    * E = 3
    Soma = $5 + 4 + 3 + 8 + 4 + 3 = 27$

2.  **Divisão por 3 e verificação do resto:**
    * $27 \div 3 = 9$ com resto $0$.

3.  **Determinação da preferência de *gaps*:**
    * **Resto 0: *Gaps* devem ocorrer preferencialmente no início da sequência.**
    * Resto 1: *Gaps* devem ocorrer preferencialmente no final da sequência.
    * Resto 2: *Gaps* devem ocorrer preferencialmente no meio da sequência.

---

## Funcionalidades Implementadas

* **Leitura de Sequências:** O programa lê sequências de DNA de um arquivo `.txt`. As sequências devem conter apenas os caracteres 'A', 'C', 'T', 'G' (case-insensitive) e ter no máximo 100 caracteres (definido por `MAX_LEN` em `structs.h` considerando uma margem, a especificação do problema é 100). Linhas vazias ou sequências inválidas/muito longas são ignoradas.
* **Validação de Caracteres:** Verifica se as sequências contêm apenas caracteres válidos (A, C, T, G).
* **Alinhamento Progressivo:**
    * As sequências de entrada são ordenadas por tamanho em ordem decrescente.
    * A sequência mais longa é usada como referência inicial para o Alinhamento Múltiplo de Sequências (MSA).
    * Cada sequência restante é alinhada à referência atual usando o algoritmo de Needleman-Wunsch. A referência é a primeira sequência (`seq1`) na chamada de `needleman_wunsch`, e a flag `evitar_gaps_em_seq1` é ativada para penalizar a introdução de gaps na referência.
    * O MSA final é construído progressivamente.
* **Cálculo de Score (Sum-of-Pairs):** Após o alinhamento, o *score* SP (Sum-of-Pairs) é calculado para o conjunto final de sequências alinhadas. A função `calcular_score` itera por cada coluna do alinhamento e soma as pontuações para todos os pares de bases (ou base-*gap*) nessa coluna.
    * Match (ex: A/A): `ALPHA` (+1)
    * Mismatch (ex: A/C): `BETA` (0)
    * Base/Gap (ex: A/-): `DELTA` (-2)
    * Gap/Gap (-/-): `GAP_GAP` (0)
* **Impressão do Resultado:** As sequências alinhadas e o *score* final são impressos no console.

---

## Estrutura do Projeto e Arquivos

* `main.c`: Contém a função `main`, que orquestra a leitura das sequências, o processo de alinhamento e a impressão dos resultados.
* `functions.h`: Arquivo de cabeçalho com as declarações das funções utilizadas para o alinhamento, cálculo de *score*, leitura de sequências e outras utilidades.
* `functions.c`: Implementação das funções declaradas em `functions.h`.
* `structs.h`: Define estruturas de dados (como `DetalheSequencia`) e constantes globais (como `MAX_SEQ`, `MAX_LEN`, `MAX_FINAL_LEN`, e os pesos `ALPHA`, `BETA`, `DELTA`, `GAP_GAP`).
* `gerador.py`: Um script Python para gerar arquivos de teste (`.txt`) com sequências de DNA aleatórias.
* `data/`: Diretório destinado a armazenar os arquivos de entrada `.txt` contendo as sequências de DNA. O programa espera que os arquivos de entrada estejam nesta pasta (ex: `data/teste.txt`).
* `README.md`: O arquivo de descrição do projeto (similar a este).

---

## Requisitos do Projeto (ABN1 e ABN2)

* As sequências podem ter até **100 caracteres**.
* **ABN1:** Alinhar 2 sequências de DNA.
* **ABN2:** Alinhar até 50 sequências de DNA (o `MAX_SEQ` está definido como 50 em `structs.h`).

---

## Como Compilar e Executar

1.  **Compilação:**
    Você precisará de um compilador C (como GCC). Navegue até o diretório raiz do projeto e compile os arquivos `.c`:
    ```bash
    gcc main.c functions.c -o msa_program
    ```

2.  **Execução:**
    Após a compilação bem-sucedida, execute o programa:
    ```bash
    ./msa_program
    ```
    O programa solicitará o nome do arquivo `.txt` contendo as sequências. Certifique-se de que este arquivo esteja localizado no diretório `data/`.
    Por exemplo, se o seu arquivo for `teste.txt` dentro da pasta `data`, digite `teste.txt` quando solicitado.

    **Exemplo de conteúdo de arquivo de entrada (`data/meu_arquivo.txt`):**
    ```
    ATGCGT
    AGTC
    ATGT
    ```

---
