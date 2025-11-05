#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

// --- Variables Globales ---
const int N = 5;
const int M = 3;
// Optimización M0 según el artículo (N < 2M)
const int M0 = N - M; // M0 = 2

// Almacena los costos de transición T(g_k) para cada subconjunto (máscara)
map<int, double> T;

// Tabla DP: dp[k][mask] = W_k*(z)
double dp[M0 + 1][1 << N];
// Tabla para reconstruir la política (los clústeres elegidos)
string path[M0 + 1][1 << N];

// Límites de estado (min/max) para N=5, M=3, M0=2
// (Movido al ámbito global para ser accesible por main())
int min_entities[M0 + 1] = {0, 2, 4};
int max_entities[M0 + 1] = {0, 3, 4};

// Distancias al cuadrado (d_ij^2) del Apéndice B del artículo
// Índices 0-4 para entidades 1-5
map<pair<int, int>, double> distSq;

/**
 * Función auxiliar para contar los bits (entidades) en una máscara.
 * Representa n_k (tamaño del clúster).
 */
int countBits(int mask) {
    int count = 0;
    for (int i = 0; i < N; ++i) {
        if ((mask >> i) & 1) {
            count++;
        }
    }
    return count;
}

/**
 * Función auxiliar para convertir una máscara a un string (ej. {1, 5}).
 */
string maskToString(int mask) {
    string s = "{";
    bool first = true;
    for (int i = 0; i < N; ++i) {
        if ((mask >> i) & 1) {
            if (!first) s += ", ";
            s += to_string(i + 1);
            first = false;
        }
    }
    s += "}";
    return s;
}

/**
 * Pre-calcula los costos de transición T(g_k) para todos los 31 subconjuntos.
 * Usa la fórmula T(g_k) = (1/n_k) * sum(d_ij^2).
 */
void precalcularCostosT() {
    // Datos del Apéndice B, Tabla IV
    distSq[{0, 1}] = 13; distSq[{0, 2}] = 32; distSq[{0, 3}] = 18; distSq[{0, 4}] = 1;
    distSq[{1, 2}] = 5;  distSq[{1, 3}] = 1;  distSq[{1, 4}] = 8;
    distSq[{2, 3}] = 2;  distSq[{2, 4}] = 25;
    distSq[{3, 4}] = 13;

    // Iterar sobre todos los 31 posibles subconjuntos (máscaras)
    for (int mask = 1; mask < (1 << N); ++mask) {
        int n_k = countBits(mask);

        // Clústeres de una entidad (singletons) tienen costo 0
        if (n_k <= 1) {
            T[mask] = 0.0;
            continue;
        }

        double sum_distSq = 0;
        vector<int> entities;
        for (int i = 0; i < N; ++i) {
            if ((mask >> i) & 1) {
                entities.push_back(i);
            }
        }

        // Sumar d_ij^2 para todos los pares i < j en el clúster
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                int u = entities[i];
                int v = entities[j];
                // Asegurar orden (u < v) para el map
                if (u > v) swap(u, v); 
                sum_distSq += distSq[{u, v}];
            }
        }

        T[mask] = sum_distSq / (double)n_k;
    }
}

/**
 * Implementa la recursión de Programación Dinámica W_K*(z).
 */
void resolverDP() {
    // 1. Inicializar DP
    for (int k = 0; k <= M0; ++k) {
        for (int mask = 0; mask < (1 << N); ++mask) {
            dp[k][mask] = numeric_limits<double>::infinity();
        }
    }

    // 2. Caso Base: W_0*(0) = 0
    dp[0][0] = 0;
    path[0][0] = "Inicio";

    // 3. Iterar por etapas (K)
    for (int k = 1; k <= M0; ++k) {
        cout << "--- Calculando Etapa K = " << k << " ---" << endl;

        // 4. Iterar por todos los estados 'z' (z_mask)
        for (int z_mask = 1; z_mask < (1 << N); ++z_mask) {
            
            // Validar si el estado 'z' es alcanzable en la etapa 'k'
            int n_z = countBits(z_mask);
            if (n_z < min_entities[k] || n_z > max_entities[k]) {
                continue;
            }

            // 5. Iterar por todos los estados 'y' (y_mask) que son subconjuntos de 'z'
            // Esto implementa la parte min_y[...]
            for (int y_mask = 0; y_mask < z_mask; ++y_mask) {
                
                // Si 'y' es un subconjunto propio de 'z'
                if ((z_mask & y_mask) == y_mask) {
                    
                    // Validar si el estado 'y' era válido en la etapa k-1
                    int n_y = countBits(y_mask);
                    if (k > 1 && (n_y < min_entities[k-1] || n_y > max_entities[k-1])) {
                        continue;
                    }

                    // El nuevo clúster es (z - y)
                    int cluster_nuevo_mask = z_mask ^ y_mask; // XOR = z - y

                    // Ecuación de Recurrencia: T(z-y) + W_{K-1}*(y)
                    double costo_actual = T[cluster_nuevo_mask] + dp[k-1][y_mask];

                    // Si encontramos un costo menor, actualizamos la tabla DP
                    if (costo_actual < dp[k][z_mask]) {
                        dp[k][z_mask] = costo_actual;
                        // Guardar la política (el camino)
                        path[k][z_mask] = path[k-1][y_mask] + " -> " + maskToString(cluster_nuevo_mask);
                    }
                }
            }
        }
    }
}

int main() {
    precalcularCostosT();
    resolverDP();

    cout << "\n--- Resultados Finales (Etapa M0 = " << M0 << ") ---" << endl;

    double min_costo_final = numeric_limits<double>::infinity();
    string politica_optima = "";
    int estado_optimo = -1;

    // Buscar el costo mínimo en la última etapa (K=M0=2)
    // Los estados válidos en esta etapa tienen 4 entidades (max_entities[2]=4)
    for (int z_mask = 1; z_mask < (1 << N); ++z_mask) {
        if (countBits(z_mask) == max_entities[M0]) { // 4 entidades
            cout << "W_2*(" << maskToString(z_mask) << ") = " << dp[M0][z_mask] 
                 << "  (Política: " << path[M0][z_mask] << ")" << endl;
            
            if (dp[M0][z_mask] < min_costo_final) {
                min_costo_final = dp[M0][z_mask];
                politica_optima = path[M0][z_mask];
                estado_optimo = z_mask;
            }
        }
    }
    
    // Determinar la entidad singleton restante
    int singleton_mask = ((1 << N) - 1) ^ estado_optimo;

    cout << "\n==============================================" << endl;
    cout << "Costo W minimo global: " << min_costo_final << endl;
    cout << "Particion Optima (M=3 clústeres):" << endl;
    cout << "  Clústeres de DP (M0=2): " << politica_optima.substr(8) << endl;
    cout << "  Clúster Singleton: " << maskToString(singleton_mask) << endl;
    cout << "==============================================" << endl;

    return 0;
}