# Program 2 – Distance Vector Routing Algorithm

Implements the **Bellman-Ford** distributed routing algorithm. Each router iteratively updates its routing table based on neighbour costs until no further improvements are possible.

---

## Files

| File | Role |
|------|------|
| `dv.c` | Single-file simulation of N routers exchanging distance vectors |

---

## Compile & Run

```bash
gcc dv.c -o dv
./dv
```

---

## How It Works

1. User enters number of nodes and the cost matrix (use `999` for ∞ / no link).
2. The algorithm relaxes routes using Bellman-Ford until stable.
3. Prints each router's final routing table: destination, next-hop, and total distance.

---

## dv.c

```c
/* Program 2: Distance Vector Routing Algorithm
   Implements Bellman-Ford shortest path for N routers */

#include <stdio.h>

struct node {
    unsigned front[20];
    unsigned dist[20];
} t[10];

int main()
{
    int costmat[20][20];
    int nodes, i, j, k, count = 0;

    printf("\nEnter the no of nodes: ");
    scanf("%d", &nodes);
    printf("Enter the cost matrix:\n");

    for (i = 0; i < nodes; i++) {
        for (j = 0; j < nodes; j++) {
            scanf("%d", &costmat[i][j]);
            costmat[i][i]  = 0;
            t[i].dist[j]   = costmat[i][j];
            t[i].front[j]  = j;
        }
    }

    /* Bellman-Ford relaxation */
    do {
        count = 0;
        for (i = 0; i < nodes; i++)
            for (j = 0; j < nodes; j++)
                for (k = 0; k < nodes; k++)
                    if (t[i].dist[j] > costmat[i][k] + t[k].dist[j]) {
                        t[i].dist[j]  = costmat[i][k] + t[k].dist[j];
                        t[i].front[j] = k;
                        count++;
                    }
    } while (count != 0);

    for (i = 0; i < nodes; i++) {
        printf("\nFor router %d", i + 1);
        for (j = 0; j < nodes; j++)
            printf("\n  For node %d via %d distance is %d",
                   j + 1, t[i].front[j] + 1, t[i].dist[j]);
    }
    printf("\n\n");
    return 0;
}
```
