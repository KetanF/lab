#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph
{
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Add an edge to the graph
    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
    }

    // Parallel Breadth-First Search
    void parallelBFS(int startVertex)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        double start_time = omp_get_wtime(); // Start time

        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            cout << v << " ";

#pragma omp parallel for
            for (int i = 0; i < adj[v].size(); ++i)
            {
                int n = adj[v][i];
                if (!visited[n])
                {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }

        double end_time = omp_get_wtime(); // End time
        cout << "\nParallel BFS Execution Time: " << end_time - start_time << " seconds\n";
    }

    // Sequential Breadth-First Search
    void sequentialBFS(int startVertex)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        double start_time = omp_get_wtime(); // Start time

        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            cout << v << " ";

            for (int i = 0; i < adj[v].size(); ++i)
            {
                int n = adj[v][i];
                if (!visited[n])
                {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }

        double end_time = omp_get_wtime(); // End time
        cout << "\nSequential BFS Execution Time: " << end_time - start_time << " seconds\n";
    }
};

int main()
{
    // Create a graph
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(2, 3);
    g.addEdge(2, 6);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    cout << "Sequential BFS: ";
    g.sequentialBFS(0);
    cout << endl;

    cout << "Parallel BFS: ";
    g.parallelBFS(0);
    cout << endl;

    return 0;
}
