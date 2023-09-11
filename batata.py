import subprocess
import matplotlib.pyplot as plt
import numpy as np
# Dimensão das matrizes quadradas (começa com 100)
N = 100

# Para calcular os tempos
time = 0
timethr = 0
timeproc = 0

# Para armazenar os tempos e os tamanhos das matrizes
avarage_time_per_dimension = []
# Compila o gerador de matrizes
comando = "g++ -std=c++11 auxx.cpp -o exec/generator"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes sequencial
comando = "g++ -std=c++11 produtoseq.cpp -o exec/prdseq"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com threads
comando = "g++ -std=c++11 produtothr.cpp -o exec/prdthr -pthread"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com processos
comando = "g++ -std=c++11 produtoproc.cpp -o exec/prdproc"
saida = subprocess.check_output(comando, shell=True)

while(time < 1000):
    # Gera as matrizes
    comando = "./exec/generator "+str(N)+ " "+str(N)+ " "+str(N)+ " "+str(N)
    saida = subprocess.check_output(comando, shell=True)
    mediaseq = 0
    mediathr = 0
    mediaproc = 0
    # Realiza o produto 10 vezes
    for i in range(10):
        #Produto sequencial
        comando = "./exec/prdseq outs/M1.txt outs/M2.txt outs/out.txt"
        saida = subprocess.check_output(comando, shell=True) 
        mediaseq = mediaseq + int(saida.strip())

        #Produto com threads
        p = int((N*N)/8)
        comando = "./exec/prdthr outs/M1.txt outs/M2.txt "+ str(p)
        saida = subprocess.check_output(comando, shell=True) 
        threads = int((N*N)/p)
        if(threads * p < N*N):
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
                        print(ultima_linha)

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
        p = int((N*N)/8)
        comando = "./exec/prdproc outs/M1.txt outs/M2.txt "+ str(p)
        saida = subprocess.check_output(comando, shell=True) 
        procs = int((N*N)/p)
        if(procs * p < N*N):
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
    mediaseq = mediaseq/10
    mediathr = mediathr/10
    mediaproc = mediaproc/10
    time = mediaseq
    print(str(time))
    avarage_time_per_dimension.append([N,mediaseq,mediathr,mediaproc])
    N = 2*N

# Executa o comando e captura a saída

# Imprime a saída
print(saida.decode("utf-8"))

# Arrays com os resultados
matriz_sizes = avarage_time_per_dimension[0]
seq_time = avarage_time_per_dimension[1]
thread_time = avarage_time_per_dimension[2]
proc_time = avarage_time_per_dimension[3]

# Crie um gráfico de linha para cada função e adicione legendas
plt.scatter(matriz_sizes, seq_time, label='Tempo do codigo sequencial')
plt.scatter(matriz_sizes, thread_time, label='Tempo do codigo de threads paralelas')
plt.scatter(matriz_sizes, proc_time, label='Tempo do código de processos paralelos')

# Adicione rótulos aos eixos x e y
plt.xlabel('Tamanho da matriz')
plt.ylabel('Tempo')

# Adicione um título ao gráfico
plt.title('Tempo X elementos')

# Exiba o gráfico
plt.savefig('grafico.png')