//usuario: d_waisman


#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cmath>
#include <tuple>

using namespace std;

typedef tuple<int, int, int> arista; // (peso, u, v)

class Grafo {
    int V;
    vector<arista> aristas;

public:
    Grafo(int V);
    void addArista(int u, int v, int w);
    int kruskal();
};

Grafo::Grafo(int V) {
    this->V = V;
}

void Grafo::addArista(int u, int v, int w) {
    aristas.push_back(make_tuple(w, u, v));
}

// Find function for disjoint-set (union-find)
int find(vector<int>& parent, int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]);
}

// Union function for disjoint-set (union-find)
void unionSets(vector<int>& parent, vector<int>& rank, int u, int v) {
    int root_u = find(parent, u);
    int root_v = find(parent, v);

    if (rank[root_u] < rank[root_v]) {
        parent[root_u] = root_v;
    } else if (rank[root_u] > rank[root_v]) {
        parent[root_v] = root_u;
    } else {
        parent[root_v] = root_u;
        rank[root_u]++;
    }
}

int Grafo::kruskal() {
    sort(aristas.begin(), aristas.end()); // Ordenar aristas por peso

    vector<int> parent(V);
    vector<int> rank(V, 0);

    for (int i = 0; i < V; ++i)
        parent[i] = i;

    int costoTotal = 0;
    for (const auto& [peso, u, v] : aristas) {
        if (find(parent, u) != find(parent, v)) {
            costoTotal += peso;
            unionSets(parent, rank, u, v);
        }
    }

    return costoTotal;
}

int distanciaclaves(const string& clave1, const string& clave2) {
    int distancia = 0;
    for (int i = 0; i < 4; i++) {
        int d1 = clave1[i] - '0';
        int d2 = clave2[i] - '0';
        int dif = abs(d1 - d2);
        if (dif < 10 - dif) {
            distancia += dif;
        } else {
            distancia += 10 - dif;
        }
    }
    return distancia;
}

int main() {
    int T;
    cin >> T;

    for (int i = 0; i < T; ++i) {
        int N;
        cin >> N;

        vector<string> casos(N);

        for (int j = 0; j < N; ++j) {
            cin >> casos[j];
        }

        Grafo g(N);
        int costoInicial = INT_MAX;

        for (int l = 0; l < N; l++) {
            int distancia = distanciaclaves("0000", casos[l]);
            if (distancia < costoInicial) {
                costoInicial = distancia;
            }
        }

        for (int u = 0; u < N; u++) {
            for (int v = u + 1; v < N; v++) {
                int dist = distanciaclaves(casos[u], casos[v]);
                g.addArista(u, v, dist);
            }
        }

        int res = costoInicial + g.kruskal();
        cout << res << endl;
    }

    return 0;
}
