//usuario: d_waisman

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cmath>
#include <tuple>
#include <string>
#include <sstream>
#include <set>
#include <map>

using namespace std;

typedef tuple<int, int> vertice; // (piso, ascensor)
typedef tuple<vertice, int> arista; 

class Grafo {
    int V;
    map<vertice, vector<arista>> adj;

public:
    Grafo(int V);
    void addArista(vertice u, vertice v, int w);
    map<vertice, int> dijkstra(vertice origen);
};

Grafo::Grafo(int V) {
    this->V = V;
}

void Grafo::addArista(vertice u, vertice v, int w) {
    adj[u].push_back(make_tuple(v, w));
    adj[v].push_back(make_tuple(u, w)); 
}

map<vertice, int> Grafo::dijkstra(vertice origen) {
    map<vertice, int> dist; 
    for (const auto& p : adj) {
        dist[p.first] = INT_MAX;
    }
    set<vertice> S;
    priority_queue<pair<int, vertice>, vector<pair<int, vertice>>, greater<pair<int, vertice>>> pq; 

    dist[origen] = 0;
    pq.emplace(0, origen); 

    while (!pq.empty()) {
        int d = pq.top().first;
        vertice u = pq.top().second;
        pq.pop();

        if (S.find(u) != S.end()) continue; 
        S.insert(u);

        for (const auto& edge : adj[u]) {
            vertice v = get<0>(edge);
            int peso = get<1>(edge);

            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pq.emplace(dist[v], v);
            }
        }
    }

    return dist;
}

int main() {
    int n, target;

    while (cin >> n >> target) {
        vector<int> tiempos_a(n);

        for (int i = 0; i < n; ++i) {
            cin >> tiempos_a[i];
        }

        cin.ignore(); 

        vector<vector<int>> pisos_ascensor(n);
        for (int i = 0; i < n; ++i) {
            string line;
            getline(cin, line);
            stringstream ss(line);
            int piso;
            while (ss >> piso) {
                pisos_ascensor[i].push_back(piso);
            }
        }

        Grafo g(100 * n + 2); 

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < pisos_ascensor[i].size() - 1; j++) {
                vertice u = make_tuple(pisos_ascensor[i][j], i);
                vertice v = make_tuple(pisos_ascensor[i][j + 1], i);
                int tiempo = abs(pisos_ascensor[i][j + 1] - pisos_ascensor[i][j]) * tiempos_a[i];
                g.addArista(u, v, tiempo);
            }
        }


        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                set<int> set1(pisos_ascensor[i].begin(), pisos_ascensor[i].end());
                set<int> set2(pisos_ascensor[j].begin(), pisos_ascensor[j].end());

                for (int piso : set1) {
                    if (set2.count(piso)) {
                        vertice u = make_tuple(piso, i);
                        vertice v = make_tuple(piso, j);
                        g.addArista(u, v, 60); 
                    }
                }
            }
        }

        
        vertice origen_ficticio = make_tuple(0, -1);
        bool hay_ascensor_en_0 = false;

        for (int i = 0; i < n; ++i) {
            if (find(pisos_ascensor[i].begin(), pisos_ascensor[i].end(), 0) != pisos_ascensor[i].end()) {
                vertice ascensor_origen = make_tuple(0, i);
                g.addArista(origen_ficticio, ascensor_origen, 0);
                hay_ascensor_en_0 = true;
            }
        }

        if (!hay_ascensor_en_0) {
            cout << "IMPOSSIBLE" << endl;
            continue;
        }

        
        vertice destino_ficticio = make_tuple(target, -1);
        bool hay_ascensor_en_target = false;

        for (int i = 0; i < n; ++i) {
            if (find(pisos_ascensor[i].begin(), pisos_ascensor[i].end(), target) != pisos_ascensor[i].end()) {
                vertice ascensor_destino = make_tuple(target, i);
                g.addArista(destino_ficticio, ascensor_destino, 0);
                hay_ascensor_en_target = true;
            }
        }

        if (!hay_ascensor_en_target) {
            cout << "IMPOSSIBLE" << endl;
            continue;
        }

        auto dist = g.dijkstra(origen_ficticio);

        if (dist.find(destino_ficticio) != dist.end() && dist[destino_ficticio] != INT_MAX) {
            cout << dist[destino_ficticio] << endl;
        } else {
            cout << "IMPOSSIBLE" << endl;
        }
    }

    return 0;
}
