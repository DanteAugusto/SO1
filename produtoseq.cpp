#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Função para multiplicar duas matrizes
vector<vector<int>> multiplicarMatrizes(const vector<vector<int>>& M1, const vector<vector<int>>& M2) {
    int rows1 = M1.size();
    int cols1 = M1[0].size();
    int cols2 = M2[0].size();

    vector<vector<int>> resultado(rows1, vector<int>(cols2, 0));

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                resultado[i][j] += M1[i][k] * M2[k][j];
            }
        }
    }

    return resultado;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Uso: " << argv[0] << " arquivo_M1.txt arquivo_M2.txt arquivo_saida.txt" << endl;
        return 1;
    }

    // Abrir e ler os arquivos de entrada
    ifstream arquivoM1(argv[1]);
    ifstream arquivoM2(argv[2]);

    if (!arquivoM1.is_open() || !arquivoM2.is_open()) {
        cout << "Erro ao abrir um dos arquivos de entrada." << endl;
        return 2;
    }

    int rows1, cols1, rows2, cols2;

    arquivoM1 >> rows1 >> cols1;
    arquivoM2 >> rows2 >> cols2;

    if (cols1 != rows2) {
        cout << "As matrizes não podem ser multiplicadas. O número de colunas de M1 deve ser igual ao número de linhas de M2." << endl;
        return 3;
    }

    vector<vector<int>> M1(rows1, vector<int>(cols1));
    vector<vector<int>> M2(rows2, vector<int>(cols2));

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

    // Realizar a multiplicação das matrizes e medir o tempo
    auto start = high_resolution_clock::now();
    vector<vector<int>> resultado = multiplicarMatrizes(M1, M2);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    // Salvar o resultado em um arquivo de saída
    ofstream arquivoSaida(argv[3]);

    if (!arquivoSaida.is_open()) {
        cout << "Erro ao abrir o arquivo de saída." << endl;
        return 4;
    }

    arquivoSaida << rows1 << " " << cols2 << endl;

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            arquivoSaida << "c" << i+1 << j+1 << " " << resultado[i][j] << endl;
        }
    }

    arquivoSaida << "Tempo de execução (ms): " << duration.count() << endl;

    arquivoSaida.close();

    cout << "Multiplicação de matrizes concluída com sucesso." << endl;

    return 0;
}
