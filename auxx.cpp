#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Função para gerar uma matriz aleatória
void gerarMatrizAleatoria(int n1, int m1, const char* nomeArquivo1, int n2, int m2, const char* nomeArquivo2) {
    ofstream arquivo1(nomeArquivo1);

    if (!arquivo1) {
        cerr << "Erro ao abrir o arquivo 1 " << nomeArquivo1 << endl;
        exit(1);
    }

    // Seed para a geração de números aleatórios
    srand(time(nullptr));

    arquivo1 << n1 << " " << m1 << endl; // Escreve as dimensões no arquivo

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            // Gera um valor aleatório entre 1 e 100 para cada elemento
            int valor = rand() % 100 + 1;
            arquivo1 << valor << " ";
        }
        arquivo1 << endl;
    }

    arquivo1.close();

    ofstream arquivo2(nomeArquivo2);

    if (!arquivo2) {
        cerr << "Erro ao abrir o arquivo 2 " << nomeArquivo2 << endl;
        exit(1);
    }

    arquivo2 << n2 << " " << m2 << endl; // Escreve as dimensões no arquivo

    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            // Gera um valor aleatório entre 1 e 100 para cada elemento
            int valor = rand() % 100 + 1;
            arquivo2 << valor << " ";
        }
        arquivo2 << endl;
    }

    arquivo2.close();
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

    gerarMatrizAleatoria(n1, m1, "M1.txt", n2, m2, "M2.txt");

    cout << "Matrizes geradas e salvas em M1.txt e M2.txt." << endl;

    return 0;
}
