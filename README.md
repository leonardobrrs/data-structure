# Alinhamento Múltiplo de Sequências de DNA com Abordagem Progressiva

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

Embora o problema mencione uma "abordagem de computação evolucionária", a implementação atual utiliza uma **abordagem de alinhamento progressivo**:
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

**Nota sobre a implementação da preferência de gaps:**
A implementação atual do algoritmo de Needleman-Wunsch (`needleman_wunsch` em `functions.c`) inclui um parâmetro `evitar_gaps_em_seq1`. Quando a função `alinharComReferenciaNW` alinha sequências à sequência de referência, ela define `evitar_gaps_em_seq1 = 1`. Isso significa que o algoritmo tenta evitar introduzir *gaps* na sequência de referência durante o alinhamento par a par. A diretriz de que "gaps devem ocorrer preferencialmente no início da sequência" pode exigir modificações mais específicas nas penalidades de abertura ou extensão de gaps no início das sequências durante o alinhamento, o que não é explicitamente implementado de forma diferenciada na versão atual do código.

---

## Funcionalidades Implementadas

* **Leitura de Sequências:** O programa lê sequências de DNA de um arquivo `.txt`. As sequências devem conter apenas os caracteres 'A', 'C', 'T', 'G' (case-insensitive) [cite: uploaded
