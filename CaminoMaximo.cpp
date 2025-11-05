#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Dimensiones del tablero
const int FILAS = 4;
const int COLS = 4;

/**
 * Imprime una matriz de 4x4.
 */
void imprimirMatriz(const vector<vector<int>>& matriz, const string& titulo) {
    cout << titulo << ":" << endl;
    for (int i = 0; i < FILAS; ++i) {
        cout << "[ ";
        for (int j = 0; j < COLS; ++j) {
            cout.width(2); // Formato para alinear
            cout << matriz[i][j] << (j == COLS - 1 ? " " : ", ");
        }
        cout << "]" << endl;
    }
}

void resolverCaminoMaximo(const vector<vector<int>>& dinero) {
    
    vector<vector<int>> dp(FILAS, vector<int>(COLS, 0));

    
    // Caso Base (0,0)
    dp[0][0] = dinero[0][0];

    // Caso Base: Primera Fila
    for (int j = 1; j < COLS; ++j) {
        dp[0][j] = dinero[0][j] + dp[0][j-1];
    }

    // Caso Base: Primera Columna
    for (int i = 1; i < FILAS; ++i) {
        dp[i][0] = dinero[i][0] + dp[i-1][0];
    }

    // Aplicar la Relación de Recurrencia
    for (int i = 1; i < FILAS; ++i) {
        for (int j = 1; j < COLS; ++j) {
            // dp[i][j] = dinero[i][j] + max(dp[i-1][j], dp[i][j-1])
            dp[i][j] = dinero[i][j] + max(dp[i-1][j], dp[i][j-1]);
        }
    }

    imprimirMatriz(dinero, "Tablero de Dinero");
    cout << "\n";
    imprimirMatriz(dp, "Matriz DP (Ganancias Maximas)");
    cout << "\n";
    
    int sumaMaxima = dp[FILAS - 1][COLS - 1];
    cout << "=================================================" << endl;
    cout << "Suma Total de Dinero: " << sumaMaxima << endl;

    vector<pair<int, int>> camino;
    int i = FILAS - 1;
    int j = COLS - 1;

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
            // Si el valor de arriba (dp[i-1][j]) es mayor que el de la izquierda (dp[i][j-1]),
            // significa que vinimos desde arriba.
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


int main() {
    vector<vector<int>> dinero = {
        {0, 3, 1, 5},
        {2, 2, 4, 1},
        {5, 2, 2, 2},
        {1, 4, 1, 0}
    };

    resolverCaminoMaximo(dinero);

    return 0;
}