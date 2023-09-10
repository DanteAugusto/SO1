import subprocess
# Dimensão das matrizes quadradas (começa com 100)
N = 100

# Para calcular os tempos
time = 0
timethr = 0
timeproc = 0

# Para armazenar os tempos e os tamanhos das matrizes
avarage_time_per_dimension = []
# Compila o gerador de matrizes
comando = "g++ -std=c++11 auxx.cpp -o generator"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes sequencial
comando = "g++ -std=c++11 produtoseq.cpp -o prdseq"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com threads
comando = "g++ -std=c++11 produtothr.cpp -o prdthr -pthread"
saida = subprocess.check_output(comando, shell=True)

# Compila o produto de matrizes com processos
comando = "g++ -std=c++11 produtoproc.cpp -o prdproc"
saida = subprocess.check_output(comando, shell=True)

while(time < 120000):
    # Gera as matrizes
    comando = "./generator "+str(N)+ " "+str(N)+ " "+str(N)+ " "+str(N)
    saida = subprocess.check_output(comando, shell=True)
    mediaseq = 0
    mediathr = 0
    mediaproc = 0
    # Realiza o produto 10 vezes
    for i in range(10):
        #Produto sequencial
        comando = "./prdseq M1.txt M2.txt out.txt"
        saida = subprocess.check_output(comando, shell=True) 
        mediaseq = mediaseq + int(saida.strip())

        #Produto com threads
        threads = int((N*N)/8)
        threads = int((N*N)/threads)
        if(threads * 8 < N*N):
            threads = threads+1
        comando = "./prdthr M1.txt M2.txt"+ str(threads)+" out.txt"
        saida = subprocess.check_output(comando, shell=True) 
        max_time_threads = 0
        for i in range(threads):
            nome_arquivo = "threads_out"+str(i)+".txt"
            try:
                with open(nome_arquivo, "r") as arquivo:
                    linhas = arquivo.readlines()
                    if linhas:
                        # Obtém a última linha (índice -1)
                        ultima_linha = linhas[-1]

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
        comando = "./prdproc M1.txt M2.txt"+ str(int((N*N)/8))+" out.txt"
        saida = subprocess.check_output(comando, shell=True) 
        procs = int((N*N)/8)
        procs = int((N*N)/procs)
        if(procs * 8 < N*N):
            procs = procs+1
        max_time_procs = 0
        for i in range(procs):
            nome_arquivo = "procs_out"+str(i)+".txt"
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
    avarage_time_per_dimension.append([N,mediaseq,mediathr,mediaproc])
    N = 2*N

# Executa o comando e captura a saída

# Imprime a saída
print(saida.decode("utf-8"))