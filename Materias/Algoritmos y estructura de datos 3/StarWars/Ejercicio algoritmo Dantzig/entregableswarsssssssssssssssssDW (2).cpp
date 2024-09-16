//usuario: d_waisman


#include <algorithm>
#include <iostream>
#include <vector>
#include <limits> 

using namespace std;

const long long int INF = numeric_limits<long long int>::max();

class Grafo {
    long long int V;
    vector<vector<long long int>> dist; // Matriz de distancias

public:
    Grafo(long long int V);
    void addArista(long long int u, long long int v, long long int w);
    long long int dantizg(); 
    vector<vector<long long int>> getDistancias();
    void setDist(const vector<vector<long long int>>& newDist);
};

Grafo::Grafo(long long int V) {
    this->V = V;
    dist.resize(V, vector<long long int>(V, INF));
    for (long long int i = 0; i < V; ++i) {
        dist[i][i] = 0; 
    }
}

void Grafo::addArista(long long int u, long long int v, long long int w) {
    dist[u][v] = w;
    dist[v][u] = w;
}

void Grafo::setDist(const vector<vector<long long int>>& newDist) {
    dist = newDist;
}

long long int Grafo::dantizg() { 
    long long int suma = 0;
    long long int n = V;
    for (long long int k = 0; k < n-1; k++) {
        for (long long int i = 0; i <= k; i++) {
            long long int minc = INF;
            long long int minf = INF;
            for (long long int j = 0; j <= k; j++) {
                if (dist[i][j] + dist[j][k+1] < minc) {
                    minc = dist[i][j] + dist[j][k+1];
                }
                if (dist[k+1][j] + dist[j][i] < minf) {
                    minf = dist[k+1][j] + dist[j][i];
                }
            }
            dist[i][k+1] = minc;
            dist[k+1][i] = minf;
        }
        for (long long int i = 0; i <= k; i++) {
            for (long long int j = 0; j <= k; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k+1] + dist[k+1][j]);
            }
        }
        long long int sum = 0;
        for (long long int i = 0; i <= k+1; ++i) {
            for (long long int j = 0; j <= k+1; ++j) {
                sum += dist[i][j];
            }
        }
        suma += sum;
    }
    return suma;
}

vector<vector<long long int>> Grafo::getDistancias() {
    return dist;
}

int main() {
    long long int t;
    cin >> t; 

    for (long long int test_case = 0; test_case < t; ++test_case) {
        long long int n;
        cin >> n;

        vector<vector<long long int>> costo_energia(n, vector<long long int>(n)); // Básicamente el peso de las aristas entre cada torre
        for (long long int i = 0; i < n; ++i) {
            for (long long int j = 0; j < n; ++j) {
                cin >> costo_energia[i][j];
            }
        }

        vector<long long int> destructionOrder(n);
        for (long long int i = 0; i < n; ++i) {
            cin >> destructionOrder[i];
        }

        
        reverse(destructionOrder.begin(), destructionOrder.end());
        //aprovecho el invariante de dantzig para en cada paso sumar usando los primeros k nodos, por eso ordeno de la siguiente manera
        //la matriz

        
        vector<vector<long long int>> matriz_ordenada(n, vector<long long int>(n));
        for (long long int i = 0; i < n; ++i) {
            for (long long int j = 0; j < n; ++j) {
                matriz_ordenada[i][j] = costo_energia[destructionOrder[i]][destructionOrder[j]];
            }
        }

        Grafo g(n);
        g.setDist(matriz_ordenada);
        long long int energiatotal = g.dantizg(); 

        cout << energiatotal << endl; 
    }

    return 0;
}
