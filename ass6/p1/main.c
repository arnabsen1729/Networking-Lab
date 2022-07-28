#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int n = 7;   // number of nodes
int m = 12;  // number of edges
int adj[][7] = {{0, 4, 5, 3, 0, 0, 0}, {4, 0, 2, 0, 0, 3, 4},
                {5, 2, 0, 6, 4, 4, 0}, {3, 0, 6, 0, 3, 0, 0},
                {0, 0, 4, 3, 0, 2, 0}, {0, 3, 4, 0, 2, 0, 5},
                {0, 4, 0, 0, 0, 5, 0}};

void dijkstra(int src) {
  int dist[n];
  int parent[n];

  bool visited[n];

  for (int i = 0; i < n; i++) dist[i] = INT_MAX, visited[i] = false;

  dist[src] = 0;
  parent[src] = -1;

  int search = 0;

  for (int count = 0; count < n - 1; count++) {
    int u;

    int min_distance = INT_MAX;

    for (int v = 0; v < n; v++)
      if (visited[v] == false && dist[v] <= min_distance)
        min_distance = dist[v], u = v;

    visited[u] = true;

    for (int v = 0; v < n; v++) {
      if (!visited[v] && adj[u][v] && dist[u] != INT_MAX &&
          dist[u] + adj[u][v] < dist[v]) {
        dist[v] = dist[u] + adj[u][v];
        parent[v] = u;
      }

      search++;
    }
  }

  printf("-------------------------------------\n");
  printf("Vertex   Distance from Source  Parent\n");
  printf("-------------------------------------\n");

  for (int i = 0; i < n; i++) {
    printf("%c \t\t %d \t\t %c \n", i + 'A', dist[i], 'A' + parent[i]);
  }

  printf("\n\n");

  printf("Shortest Path Tree: \n");
  for (int i = 0; i < n; i++) {
    if (i == src) continue;
    printf("%c <-> %c \n", 'A' + i, 'A' + parent[i]);
  }

  printf("\n\n");

  printf("Search Count: %d \n", search);
}

int main() {
  dijkstra(0);
  return 0;
}
