import random
import os

nome_base_do_arquivo = "teste.txt"
pasta_de_saida = "data"

numero_de_sequencias = 20
comprimento_minimo = 2
comprimento_maximo = 50
bases_dna = ['A', 'T', 'C', 'G']

caminho_completo_arquivo = os.path.join(pasta_de_saida, nome_base_do_arquivo)

try:
    if not os.path.exists(pasta_de_saida):
        os.makedirs(pasta_de_saida)
        print(f"Pasta '{pasta_de_saida}' criada.")

    with open(caminho_completo_arquivo, 'w') as arquivo:
        for _ in range(numero_de_sequencias):
            comprimento_da_sequencia = random.randint(comprimento_minimo, comprimento_maximo)
            sequencia_dna = "".join(random.choices(bases_dna, k=comprimento_da_sequencia))
            arquivo.write(sequencia_dna + '\n')
            
    print(f"Arquivo '{caminho_completo_arquivo}' gerado com sucesso contendo {numero_de_sequencias} sequências.")
    print(f"Cada sequência tem um comprimento entre {comprimento_minimo} e {comprimento_maximo} caracteres.")

except IOError:
    print(f"Erro: Nao foi possivel escrever no arquivo '{caminho_completo_arquivo}'. Verifique as permissoes.")
except OSError as e:
    print(f"Erro do sistema operacional ao tentar criar a pasta '{pasta_de_saida}': {e}")
except Exception as e:
    print(f"Ocorreu um erro inesperado: {e}")