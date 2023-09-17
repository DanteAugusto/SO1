#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <chrono>

std::vector<std::vector<int>> M1;
std::vector<std::vector<int>> M2;
std::vector<std::vector<int>> result;
int rows1, cols1, rows2, cols2, p, numthread;

// Função para multiplicar uma parte das matrizes
void* multiplyMatrices(void* a) {
    // std::cout << "Começo: " << start << " e fim: " << end << std::endl;
    int id = (int)(size_t)a;
    int start = id*p;
    int end = std::min(start+p, rows1*cols2);

    std::string out = "outs/threads_out";
    std::string idx = std::to_string(id);
    std::ofstream arquivoSaida(out+idx+".txt");

    if (!arquivoSaida.is_open()) {
        std::cout << "Erro ao abrir o arquivo de saída." << std::endl;
    }
    arquivoSaida << rows1 << " " << cols2 << std::endl;
    for (int i = start; i < end; ++i){
        int row = i/cols2;
        int col = i%cols2;
        // printf("Elemento [%d][%d]\n", row,col);
        for (int k = 0; k < rows2; ++k) {
            // printf("Elemento [%d][%d] += matrixA[%d][%d] * matrixB[%d][%d]\n", row,col,row,k,k,col);
            result[row][col] += M1[row][k] * M2[k][col];
        }
        arquivoSaida << "c" << row+1 << col+1 << " " << result[row][col] << std::endl;
    }    

    arquivoSaida.close();

    pthread_exit(NULL);
    // std::cout << "Thread " << p+1 << " concluída." << std::endl;
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
    
    p = std::stoi(argv[3]);

    arquivoM1 >> rows1 >> cols1;
    arquivoM2 >> rows2 >> cols2;

    if (cols1 != rows2) {
        std::cout << "As matrizes não podem ser multiplicadas. O número de colunas de M1 deve ser igual ao número de linhas de M2." << std::endl;
        return 3;
    }

    M1 = std::vector<std::vector<int>>(rows1, std::vector<int>(cols1));
    M2 = std::vector<std::vector<int>>(rows2, std::vector<int>(cols2));
    result = std::vector<std::vector<int>>(rows1, std::vector<int>(cols2));

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


    numthread = (rows1*cols2)/p;
    if(p*numthread < rows1*cols2)numthread++;
    pthread_t threads[numthread];
    int status;

    std::vector<std::chrono::steady_clock::time_point>begins(numthread);
    std::vector<std::chrono::steady_clock::time_point>ends(numthread);
    for (int i = 0; i < numthread; ++i) {
        begins[i] = std::chrono::steady_clock::now();            
        status = pthread_create(&threads[i], NULL, multiplyMatrices, (void *)(size_t)i);
    }

    int x = 0;
    std::string out;
    out = "outs/threads_out";
    // Aguarda todas as threads concluírem
    for (int i = 0; i < numthread; ++i) {
        status = pthread_join(threads[i], NULL);
        ends[i] = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ends[i] - begins[i]);
        std::ofstream arquivoSaida(out+std::to_string(x)+".txt", std::ios::app);
        if (!arquivoSaida.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo." << std::endl;
            return 1; // Saia do programa ou trate o erro de outra forma
        }
        arquivoSaida << duration.count() << std::endl;
        arquivoSaida.close();
        x++;
    }

    // Imprime a matriz resultante
    // for (int i = 0; i < rows1; ++i) {
    //     for (int j = 0; j < cols2; ++j) {
    //         std::cout << result[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    
    return 0;
}
