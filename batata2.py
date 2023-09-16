import subprocess
import matplotlib.pyplot as plt
import numpy as np
import csv
# Dimensão das matrizes quadradas (começa com 100)
P = 80000

# Para calcular os tempos
# time = 0

# Para armazenar os tempos e os tamanhos das matrizes
avarage_time_per_p = []
# Compila o gerador de matrizes
# comando = "g++ auxx.cpp -o exec/generator"
# saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes sequencial
# comando = "g++ produtoseq.cpp -o exec/prdseq"
# saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com threads
comando = "g++ produtothr.cpp -o exec/prdthr -pthread"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com processos
comando = "g++ produtoproc.cpp -o exec/prdproc"
saida = subprocess.check_output(comando, shell=True)

while(P < 1280000):
    mediathr = 0
    mediaproc = 0
    # Realiza o produto 10 vezes
    for i in range(10):
        #Produto com threads
        comando = "./exec/prdthr outs/M1.txt outs/M2.txt "+ str(P)
        saida = subprocess.check_output(comando, shell=True) 
        threads = int((1600*1600)/p)
        if(threads * P < 1600*1600):
            threads = threads+1
        max_time_threads = 0
        for i in range(threads):
            nome_arquivo = "outs/threads_out"+str(i)+".txt"
            try:
                with open(nome_arquivo, "r") as arquivo:
                    linhas = arquivo.readlines()
                    if linhas:
                        # Obtém a última linha (índice -1)
                        ultima_linha = linhas[-1]
                        # print(ultima_linha)

                        # Tente converter a última linha em um número
                        try:
                            numero = int(ultima_linha.strip())
                            if(numero > max_time_threads):
                                max_time_threads = numero
                        except ValueError:
                            print("A última linha não contém um número válido.")
                    else:
                        print("O arquivo está vazio.")
            except FileNotFoundError:
                print(f"O arquivo {nome_arquivo} não foi encontrado.")
            except Exception as e:
                print(f"Ocorreu um erro ao ler o arquivo' {nome_arquivo}: {e}")
        mediathr = mediathr + max_time_threads

        #Produto com processos
        comando = "./exec/prdproc outs/M1.txt outs/M2.txt "+ str(P)
        saida = subprocess.check_output(comando, shell=True) 
        procs = int((1600*1600)/P)
        if(procs * P < 1600*1600):
            procs = procs+1
        max_time_procs = 0
        for i in range(procs):
            nome_arquivo = "outs/procs_out"+str(i)+".txt"
            try:
                with open(nome_arquivo, "r") as arquivo:
                    linhas = arquivo.readlines()
                    if linhas:
                        # Obtém a última linha (índice -1)
                        ultima_linha = linhas[-1]

                        # Tente converter a última linha em um número
                        try:
                            numero = int(ultima_linha.strip())
                            if(numero > max_time_procs):
                                max_time_procs = numero
                        except ValueError:
                            print("A última linha não contém um número válido.")
                    else:
                        print("O arquivo está vazio.")
            except FileNotFoundError:
                print(f"O arquivo {nome_arquivo} não foi encontrado.")
            except Exception as e:
                print(f"Ocorreu um erro ao ler o arquivo' {nome_arquivo}: {e}")
        mediaproc = mediaproc + max_time_procs
    mediathr = mediathr/10
    mediaproc = mediaproc/10
    print("Teste feitos para P = "+str(P))
    avarage_time_per_p.append([P,mediathr,mediaproc])
    P = P + 240000

# Executa o comando e captura a saída

# Imprime a saída
# print(saida.decode("utf-8"))

# Arrays com os resultados
data = np.array(avarage_time_per_p)
Ps = data[:, 0]
thread_time = data[:, 1]
proc_time = data[:, 2]

# Crie um gráfico de linha para cada função e adicione legendas
plt.scatter(Ps, thread_time, label='Tempo do codigo de threads paralelas')
plt.scatter(Ps, proc_time, label='Tempo do código de processos paralelos')

# Adiciona a legenda
plt.legend(loc='upper right', frameon = False, fontsize = 6)

# Adicione rótulos aos eixos x e y
plt.xlabel('Diferentes valores de P,')
plt.ylabel('Tempo (ms)')

# Configure os eixos x e y em escala logarítmica
plt.xscale('log')
plt.yscale('log')

# Adicione um título ao gráfico
plt.title('Tempo X Ps')

# Exiba o gráfico
plt.savefig('grafico.png')


# Escrevendo no arquivo

nome_arquivo_saida = "outs/dados2.csv"

with open(nome_arquivo_saida, 'w', newline='') as arquivo_saida:
    # Crie um objeto escritor CSV
    escritor_csv = csv.writer(arquivo_saida)

    escritor_csv.writerow(["Tamanho da matriz", "tempo da execução sequencial", "tempo da execução multithread", "tempo da execução multiprocessos"])
    # Escreva os dados no arquivo
    for linha in avarage_time_per_p:
        escritor_csv.writerow(linha)