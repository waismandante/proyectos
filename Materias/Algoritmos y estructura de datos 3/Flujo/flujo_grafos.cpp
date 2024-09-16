//usuario: d_waisman
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <cstring>
#include <climits>

using namespace std;

class Graph {
public:
    Graph(long long int V);
    void addEdge(long long int u, long long int v, long long int w);
    long long int findMaxFlow(long long int s, long long int t);

private:
    long long int V;
    vector<vector<long long int>> capacity;
    vector<vector<long long int>> adj;

    bool bfs(long long int s, long long int t, vector<long long int>& parent);
    long long int sendFlow(long long int s, long long int flow, long long int t, vector<long long int>& parent);
};

Graph::Graph(long long int V) {
    this->V = V;
    capacity.assign(V, vector<long long int>(V, 0));
    adj.assign(V, vector<long long int>());
}

void Graph::addEdge(long long int u, long long int v, long long int w) {
    capacity[u][v] = w;
    adj[u].push_back(v);
    adj[v].push_back(u);
}

bool Graph::bfs(long long int s, long long int t, vector<long long int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = s;
    queue<pair<long long int, long long int>> q;
    q.push({s, LLONG_MAX});

    while (!q.empty()) {
        long long int u = q.front().first;
        long long int flow = q.front().second;
        q.pop();

        for (long long int v : adj[u]) {
            if (parent[v] == -1 && capacity[u][v]) {
                parent[v] = u;
                long long int new_flow = min(flow, capacity[u][v]);
                if (v == t)
                    return new_flow;
                q.push({v, new_flow});
            }
        }
    }

    return 0;
}

long long int Graph::sendFlow(long long int s, long long int flow, long long int t, vector<long long int>& parent) {
    long long int current = t;
    while (current != s) {
        long long int prev = parent[current];
        capacity[prev][current] -= flow;
        capacity[current][prev] += flow;
        current = prev;
    }
    return flow;
}

long long int Graph::findMaxFlow(long long int s, long long int t) {
    long long int max_flow = 0;
    vector<long long int> parent(V);
    long long int flow;

    while (flow = bfs(s, t, parent)) {
        max_flow += sendFlow(s, flow, t, parent);
    }

    return max_flow;
}

int main() {
    long long int t;
    cin >> t; 
    for (long long int test_case = 0; test_case < t; ++test_case) {
        long long int N;
        cin >> N;
        long long int M;
        cin >> M; 

        
        if (N < 1 || N > 36 || N % 6 != 0 || M < 1 || M > 30 || N < M) {
            cout << "NO" << endl;
            continue;
        }

        vector<vector<string>> restricciones(M, vector<string>(2));

        for (long long int i = 0; i < M; ++i) {
            for (long long int j = 0; j < 2; ++j) {
                cin >> restricciones[i][j];
            }
        }

        long long int cantNodos = 1 + 6 + M + 1; 
        Graph g(cantNodos);
        long long int source = 0; 
        long long int sink = 6 + M + 1;

        vector<string> talles = {"XXL", "XL", "L", "M", "S", "XS"};
        
        
        for (long long int i = 1; i < 7; ++i) {
            g.addEdge(source, i, N / 6);
        }

       
        map<string, long long int> tallaMap;
        for (long long int i = 0; i < talles.size(); ++i) {
            tallaMap[talles[i]] = i + 1;
        }

        for (long long int i = 0; i < M; ++i) {
            long long int volunteerNode = i + 7; 
            long long int talla1 = tallaMap[restricciones[i][0]];
            long long int talla2 = tallaMap[restricciones[i][1]];

            g.addEdge(talla1, volunteerNode, 1);
            g.addEdge(talla2, volunteerNode, 1);
            g.addEdge(volunteerNode, sink, 1);
        }

        long long int res = g.findMaxFlow(source, sink);

        if (res == M) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}
