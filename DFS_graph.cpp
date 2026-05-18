#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

class Graph {

    int V;
    vector<vector<int>> adj;

public:

    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge in undirected graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Parallel DFS Utility
    void parallelDFSUtil(int node, vector<bool>& visited) {

        bool shouldVisit = false;

        // Critical section for safe access
        #pragma omp critical
        {
            if (!visited[node]) {
                visited[node] = true;
                shouldVisit = true;

                cout << node << " ";
            }
        }

        // If already visited, return
        if (!shouldVisit)
            return;

        // Parallel traversal of neighbors
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {

            int neighbor = adj[node][i];

            bool alreadyVisited;

            // Safe read of visited array
            #pragma omp critical
            {
                alreadyVisited = visited[neighbor];
            }

            if (!alreadyVisited) {
                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    // DFS Function
    void parallelDFS(int start) {

        vector<bool> visited(V, false);

        cout << "Parallel DFS Traversal: ";

        parallelDFSUtil(start, visited);

        cout << endl;
    }
};

int main() {

    Graph g(7);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    double start = omp_get_wtime();

    g.parallelDFS(0);

    double end = omp_get_wtime();

    cout << "Execution Time: "
         << (end - start) * 1000000
         << " microseconds" << endl;

    return 0;
}