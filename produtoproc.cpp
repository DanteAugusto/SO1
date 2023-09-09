#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <chrono>

// Função para multiplicar uma parte das matrizes
void multiplyMatrices(const std::vector<std::vector<int>>& matrixA,
                      const std::vector<std::vector<int>>& matrixB,
                      std::vector<std::vector<int>>& result,
                      int start, int end, int p, int rows1, int rows2, int cols2) {
    // std::cout << "Começo: " << start << " e fim: " << end << std::endl;
    // Realizar a multiplicação das matrizes e medir o tempo
    for (int i = start; i < end; ++i){
        int row = i/cols2;
        int col = i%cols2;
        // printf("Elemento [%d][%d]\n", row,col);
        for (int k = 0; k < rows2; ++k) {
            // printf("Elemento [%d][%d] += matrixA[%d][%d] * matrixB[%d][%d]\n", row,col,row,k,k,col);
            result[row][col] += matrixA[row][k] * matrixB[k][col];
        }
    }

    std::string out = "threads_out";
    out.push_back(p + '1');
    std::ofstream arquivoSaida(out+".txt");

    if (!arquivoSaida.is_open()) {
        std::cout << "Erro ao abrir o arquivo de saída." << std::endl;
        return ;
    }
    arquivoSaida << rows1 << " " << cols2 << std::endl;
    for (int i = start; i < end; ++i){
        int row = i/cols2;
        int col = i%cols2;
        arquivoSaida << "c" << row+1 << col+1 << " " << result[row][col] << std::endl;
    }    

    arquivoSaida.close();

    std::cout << "Thread " << p+1 << " concluída." << std::endl;
}

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        std::cout << "Uso: " << argv[0] << " arquivo_M1.txt arquivo_M2.txt elementos_por_thread" << std::endl;
        return 1;
    }

    // Abrir e ler os arquivos de entrada
    std::ifstream arquivoM1(argv[1]);
    std::ifstream arquivoM2(argv[2]);

    if (!arquivoM1.is_open() || !arquivoM2.is_open()) {
        std::cout << "Erro ao abrir um dos arquivos de entrada." << std::endl;
        return 2;
    }

    int rows1, cols1, rows2, cols2, p;
    p = *argv[3] - '0';

    arquivoM1 >> rows1 >> cols1;
    arquivoM2 >> rows2 >> cols2;

    if (cols1 != rows2) {
        std::cout << "As matrizes não podem ser multiplicadas. O número de colunas de M1 deve ser igual ao número de linhas de M2." << std::endl;
        return 3;
    }

    std::vector<std::vector<int>> M1(rows1, std::vector<int>(cols1));
    std::vector<std::vector<int>> M2(rows2, std::vector<int>(cols2));
    std::vector<std::vector<int>> result(rows1, std::vector<int>(cols2));

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            arquivoM1 >> M1[i][j];
        }
    }

    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            arquivoM2 >> M2[i][j];
        }
    }

    arquivoM1.close();
    arquivoM2.close();

    int procs = (rows1*cols2)/p;
    int start = 0;
    int end = 0;
    pid_t pid;
    for (int i = 0; i < procs; ++i) {
        start = i * p;
        end = (i == procs - 1) ? rows1*cols2 : start + p;
        if(i != procs - 1){
            pid = fork();
            if (pid < 0) { /* ocorrência de erro*/
                fprintf(stderr, "Criação Falhou");
                exit(-1);
            }
            else if (pid == 0) { /* processo filho*/
                break;
            }
        }
        threads.emplace_back(multiplyMatrices, std::ref(M1), std::ref(M2), std::ref(result), start, end, i, rows1, rows2, cols2);
        // std::cout << "batata\n" << std::endl;
    }

    multiplyMatrices(std::ref(M1), std::ref(M2), std::ref(result), start, end, i, rows1, rows2, cols2);
    return 0;
}