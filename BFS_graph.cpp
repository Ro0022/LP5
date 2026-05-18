#include <iostream>
#include <vector>
#include <queue>
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

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS Traversal: ";

        while (!q.empty()) {

            int levelSize = q.size();

            vector<int> currentLevel;

            for (int i = 0; i < levelSize; i++) {
                int node = q.front();
                q.pop();

                currentLevel.push_back(node);
                cout << node << " ";
            }

            vector<int> nextLevel;

            #pragma omp parallel
            {
                vector<int> localNext;

                #pragma omp for nowait
                for (int i = 0; i < currentLevel.size(); i++) {

                    int u = currentLevel[i];

                    for (int v : adj[u]) {

                        bool shouldVisit = false;

                        #pragma omp critical
                        {
                            if (!visited[v]) {
                                visited[v] = true;
                                shouldVisit = true;
                            }
                        }

                        if (shouldVisit) {
                            localNext.push_back(v);
                        }
                    }
                }

                #pragma omp critical
                {
                    nextLevel.insert(nextLevel.end(),
                                     localNext.begin(),
                                     localNext.end());
                }
            }

            for (int node : nextLevel) {
                q.push(node);
            }
        }

        cout << endl;
    }
};

int main() {

    int vertices = 7;

    Graph g(vertices);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    g.parallelBFS(0);

    return 0;
}