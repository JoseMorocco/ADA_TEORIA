#include <iostream>
#include <vector>
#include <algorithm>
#include <random> 
#include <ctime> 
using namespace std;


// Genera una matriz cuadrada de orden MxM con valores aleatorios.

vector<vector<int>> generarMatrizAleatoria(int M) {
    mt19937 generador(time(0));
    uniform_int_distribution<> distribucion(0, 9); 

    vector<vector<int>> matriz(M, vector<int>(M));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            matriz[i][j] = distribucion(generador);
        }
    }
    return matriz;
}

// Imprime una matriz de MxM.
void imprimirMatriz(const vector<vector<int>>& matriz, int M, const string& titulo) {
    cout << titulo << ":" << endl;
    for (int i = 0; i < M; ++i) {
        cout << "[ ";
        for (int j = 0; j < M; ++j) {
            cout.width(2); 
            cout << matriz[i][j] << (j == M - 1 ? " " : ", ");
        }
        cout << "]" << endl;
    }
}

void resolverCaminoMaximo(const vector<vector<int>>& dinero, int M) {
    vector<vector<int>> dp(M, vector<int>(M, 0));

    // Caso Base (0,0)
    dp[0][0] = dinero[0][0];

    // Caso Base: Primera Fila
    for (int j = 1; j < M; ++j) {
        dp[0][j] = dinero[0][j] + dp[0][j-1];
    }

    // Caso Base: Primera Columna
    for (int i = 1; i < M; ++i) {
        dp[i][0] = dinero[i][0] + dp[i-1][0];
    }

    // Aplicar la Relación de Recurrencia
    for (int i = 1; i < M; ++i) {
        for (int j = 1; j < M; ++j) {
            dp[i][j] = dinero[i][j] + max(dp[i-1][j], dp[i][j-1]);
        }
    }

    imprimirMatriz(dinero, M, "Tablero de Dinero");
    cout << "\n";
    imprimirMatriz(dp, M, "Matriz DP (Ganancias Maximas)");
    cout << "\n";
    
    int sumaMaxima = dp[M - 1][M - 1];
    cout << "=================================================" << endl;
    cout << "Suma Total de Dinero: " << sumaMaxima << endl;

    vector<pair<int, int>> camino;
    int i = M - 1;
    int j = M - 1;

    while (i >= 0 && j >= 0) {
        camino.push_back({i, j});

        // Caso de parada: llegamos al inicio
        if (i == 0 && j == 0) {
            break;
        }

        // ¿Llegamos desde arriba? (Borde izquierdo)
        if (j == 0) {
            i--;
        }
        // ¿Llegamos desde la izquierda? (Borde superior)
        else if (i == 0) {
            j--;
        }
        // Caso general: comparar con la recurrencia
        else {
            if (dp[i-1][j] > dp[i][j-1]) {
                i--; // vinimos desde arriba.
            }
            else {
                j--; // vinimos desde izquierda.
            }
        }
    }

    cout << "Secuencia de Movimientos (Camino):" << endl;
    string separador = "";
    vector<string> movimientos;

    for (int k = camino.size() - 1; k > 0; k--) {
        cout << separador << "(" << camino[k].first << "," << camino[k].second << ")";
        separador = " -> ";

        if (camino[k-1].first > camino[k].first) {
            movimientos.push_back("Abajo");
        } else {
            movimientos.push_back("Derecha");
        }
    }
    cout << separador << "(" << camino[0].first << "," << camino[0].second << ")" << endl;

    cout << "Movimientos: ";
    separador = "";
    for(const string& mov : movimientos) {
        cout << separador << mov;
        separador = ", ";
    }
    cout << endl;
    cout << "=================================================" << endl;
}

int main(){
    vector<vector<int>> dinero4X4 = {
        {0, 3, 1, 5},
        {2, 2, 4, 1},
        {5, 2, 2, 2},
        {1, 4, 1, 0}};
    resolverCaminoMaximo(dinero4X4, 4);

    int M;
    cout << "Ingrese la dimension de la matriz (M): ";
    cin >> M;

    if (M <= 0) {
        cout << "La dimension debe ser un numero positivo." << endl;
        return 1;
    }

    vector<vector<int>> dinero = generarMatrizAleatoria(M);

    resolverCaminoMaximo(dinero, M);

    return 0;
}