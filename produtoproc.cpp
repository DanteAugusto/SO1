#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <map>
#include <fstream>
#include <vector>
#include <chrono>

// Função para multiplicar uma parte das matrizes
void multiplyMatrices(const std::vector<std::vector<int>>& matrix1,
                      const std::vector<std::vector<int>>& matrix2,
                      std::vector<std::vector<int>>& result,
                      int start, int end, int p, int rows1, int rows2, int cols2) {
    // std::cout << "Começo: " << start << " e fim: " << end << std::endl;
    // Realizar a multiplicação das matrizes e medir o tempo
    for (int i = start; i < end; ++i){
        int row = i/cols2;
        int col = i%cols2;
        // printf("Elemento [%d][%d]\n", row,col);
        for (int k = 0; k < rows2; ++k) {
            // printf("Elemento [%d][%d] += matrix1[%d][%d] * matrix2[%d][%d]\n", row,col,row,k,k,col);
            result[row][col] += matrix1[row][k] * matrix2[k][col];
        }
    }

    std::string out = "outs/procs_out";
    std::ofstream arquivoSaida(out+std::to_string(p)+".txt");

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

    std::cout << "Processo " << p+1 << " concluído." << std::endl;
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
    p = std::stoi(argv[3]);

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
    // std::cout << procs << std::endl;
    if(p*procs < rows1*cols2)procs++;
    // std::cout << procs << std::endl;
    int start = 0;
    int end = 0;
    int x = 0;
    std::vector<pid_t> pids(procs);
    std::map<pid_t, int> map_index;
    std::map<pid_t, std::chrono::steady_clock::time_point> map_beginsTime;


    for(int i{0}; i<procs; ++i){
        start = i * p;
        end = (i == procs - 1) ? rows1*cols2 : start + p;
        auto begin = std::chrono::steady_clock::now();
        pids[i] = fork();        
        if(pids[i] == 0){
            multiplyMatrices(M1, M2, result, start, end, i, rows1, rows2, cols2);
            exit(0);
        }
        else{
            map_index[pids[i]] = i;
            map_beginsTime[pids[i]] = begin;
        }

    }

    std::string out = "outs/procs_out";
    pid_t pid_filho;
    for(int i{0}; i < procs; ++i){
        pid_filho = wait(NULL);
        auto stopTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - map_beginsTime[pid_filho]);
        std::ofstream arquivoSaida(out+std::to_string(map_index[pid_filho])+".txt", std::ios::app);
        if (!arquivoSaida.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo." << std::endl;
            return 1; // Saia do programa ou trate o erro de outra forma
        }
        arquivoSaida << duration.count() << std::endl;
        arquivoSaida.close();

    }
    
    auto stopTime = std::chrono::steady_clock::now();
    
    return 0;
}