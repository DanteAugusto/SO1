#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Função para gerar uma matriz aleatória
void gerarMatrizAleatoria(int n, int m, const char* nomeArquivo) {
    ofstream arquivo(nomeArquivo);

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        exit(1);
    }

    // Seed para a geração de números aleatórios
    srand(time(nullptr));

    arquivo << n << " " << m << endl; // Escreve as dimensões no arquivo

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // Gera um valor aleatório entre 1 e 100 para cada elemento
            int valor = rand() % 100 + 1;
            arquivo << valor << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Uso: " << argv[0] << " n1 m1 n2 m2" << endl;
        return 1;
    }

    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    gerarMatrizAleatoria(n1, m1, "M1.txt");
    gerarMatrizAleatoria(n2, m2, "M2.txt");

    cout << "Matrizes geradas e salvas em M1.txt e M2.txt." << endl;

    return 0;
}
