#include <stdbool.h>
#include <stdio.h>

#define MAX_VAL 10000

int nodes = 5;
int matrix[5][5] = {{0, 1, MAX_VAL, 2, MAX_VAL},
                    {1, 0, 3, MAX_VAL, 6},
                    {MAX_VAL, 3, 0, 3, 2},
                    {2, MAX_VAL, 3, 0, MAX_VAL},
                    {MAX_VAL, 6, 2, MAX_VAL, 0}};

int distance_table[5][5][2];

int min(int a, int b) { return a < b ? a : b; }

void init_distance_table() {
  for (int i = 0; i < nodes; i++) {
    for (int j = 0; j < nodes; j++) {
      distance_table[i][j][0] = matrix[i][j];
      distance_table[i][j][1] = (matrix[i][j] < MAX_VAL ? j : -1);
    }
  }
}

void display_distance_table(int node) {
  printf("\nDistance Table of node: %d\n", node);
  printf("--------------------------\n");
  printf("Distance\tNext Hop\n");
  printf("--------------------------\n");
  for (int j = 0; j < nodes; j++) {
    printf("%d\t\t%d\n",
           (distance_table[node][j][0] < MAX_VAL ? distance_table[node][j][0]
                                                 : -1),
           distance_table[node][j][1]);
  }
  printf("\n");
}

bool check_convergence() {
  for (int u = 0; u < nodes; u++) {
    for (int v = 0; v < nodes; v++) {
      // no edge exists
      if (matrix[u][v] != MAX_VAL) continue;

      for (int i = 0; i < nodes; i++) {
        if (distance_table[u][v][0] + distance_table[v][i][0] <
            distance_table[u][i][0]) {
          return false;
        }
      }
    }
  }
  return true;
}

void dvr(int iter) {
  printf("\n ~~~~|| ITERATION: %d ||~~~~\n", iter);
  for (int u = 0; u < nodes; u++) {
    int flag = 0;
    for (int v = 0; v < nodes; v++) {
      // no edge exists
      if (matrix[u][v] == MAX_VAL) continue;

      for (int i = 0; i < nodes; i++) {
        if (distance_table[u][v][0] + distance_table[v][i][0] <
            distance_table[u][i][0]) {
          distance_table[u][i][0] =
              distance_table[u][v][0] + distance_table[v][i][0];
          distance_table[u][i][1] = v;

          flag++;
        }
      }
    }

    if (flag > 0) {
      display_distance_table(u);
    }
  }
}

int main() {
  init_distance_table();
  for (int i = 0; i < nodes; i++) {
    display_distance_table(i);
  }

  printf("Enter simulation duration: ");
  int duration;
  scanf("%d", &duration);

  // implementing bellman ford
  for (int iter = 0; iter < min(duration, nodes - 1); iter++) {
    dvr(iter);
  }

  if (check_convergence()) {
    printf("\nConvergence reached\n");
  } else {
    printf("\n!! Convergence not yet reached\nExtending simulation time...\n");

    for (int iter = duration; iter < nodes - 1; iter++) {
      dvr(iter);
    }
  }

  printf("\n\nFinal Results:\n");
  printf("========================================\n");
  printf("Src\tDest\tDistance\tNext Hop\n");
  printf("========================================\n");
  for (int i = 0; i < nodes; i++) {
    for (int j = 0; j < nodes; j++) {
      printf("%d\t%d\t", i, j);
      if (distance_table[i][j][0] < MAX_VAL) {
        printf("%d\t\t%d\n", distance_table[i][j][0], distance_table[i][j][1]);
      } else {
        printf("INF\t\t-1\n");
      }
    }
  }

  return 0;
}
