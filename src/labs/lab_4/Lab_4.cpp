#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;
using namespace chrono;

const int INF = 1e9;
const int iterations = 1000; // Можно увеличить до 100 для точности

class Graph {
public:
    int size;
    vector<vector<int>> graph;

    Graph(int n) : size(n), graph(n, vector<int>(n, INF)) {
        generateGraph();
    }

    void generateGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> weightDist(1, 100);

        for (int i = 0; i < size; ++i) {
            graph[i][i] = 0;
            int connections = static_cast<int>(size * 1.2 + rand() % static_cast<int>(size * 0.1 + 1));
            for (int j = 0; j < connections; ++j) {
                int randomNode = rand() % size;
                if (randomNode != i) {
                    graph[i][randomNode] = weightDist(gen);
                }
            }
        }
    }
};

// Оптимизированный Floyd-Warshall
void floydWarshallOptimized(vector<vector<int>>& dist) {
    int V = dist.size();
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] == INF) continue;
            for (int j = 0; j < V; ++j) {
                if (dist[k][j] == INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

// Bellman-Ford
vector<int> bellmanFord(const vector<vector<int>>& graph, int src) {
    int V = graph.size();
    vector<int> dist(V, INF);
    dist[src] = 0;

    for (int i = 1; i < V; ++i)
        for (int u = 0; u < V; ++u)
            for (int v = 0; v < V; ++v)
                if (graph[u][v] < INF && dist[u] < INF && dist[u] + graph[u][v] < dist[v])
                    dist[v] = dist[u] + graph[u][v];

    return dist;
}

// Dijkstra
vector<int> dijkstra(const vector<vector<int>>& graph, int src) {
    int V = graph.size();
    vector<int> dist(V, INF);
    vector<bool> visited(V, false);
    dist[src] = 0;

    for (int count = 0; count < V - 1; ++count) {
        int u = -1;
        for (int i = 0; i < V; ++i)
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;

        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;
        for (int v = 0; v < V; ++v)
            if (!visited[v] && graph[u][v] < INF && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    return dist;
}

// Measure time
template<typename Func>
double measureTime(Func f) {
    auto start = high_resolution_clock::now();
    volatile auto result = f(); // Чтобы результат не оптимизировался
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

int main() {
    vector<int> sizes = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1500, 2000 };

    cout << left << setw(10) << "Size"
        << setw(20) << "FloydWarshall (ms)"
        << setw(20) << "BellmanFord (ms)"
        << setw(20) << "Dijkstra (ms)" << endl;

    for (int size : sizes) {
        double timeFW = 0, timeBF = 0, timeDJK = 0;

        for (int i = 0; i < iterations; ++i) {
            Graph g(size);
            const vector<vector<int>>& baseGraph = g.graph;

            vector<vector<int>> fwGraph = baseGraph; // Копия
            timeFW += measureTime([&]() {
                floydWarshallOptimized(fwGraph);
                return 0;
                });

            timeBF += measureTime([&]() {
                return bellmanFord(baseGraph, 0);
                });

            timeDJK += measureTime([&]() {
                return dijkstra(baseGraph, 0);
                });
        }

        cout << left << setw(10) << size
            << setw(20) << fixed << setprecision(4) << (timeFW / iterations)
            << setw(20) << fixed << setprecision(4) << (timeBF / iterations)
            << setw(20) << fixed << setprecision(4) << (timeDJK / iterations)
            << endl;
    }

    return 0;
}
