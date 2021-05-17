// EE3980 HW08 Minimum-Cost Spanning Tree
// 106061151, 劉安得
// 2021/05/08

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sEdge {                      // struct for edges
    int u, v;                               // two vertices
    double weight;                          // the weight of the edge
} Edge;

int *parent;                                // represent the disjoint sets

double GetTime(void);                       // get local time in seconds
double Kruskal(Edge **edge, int V, int E, Edge **T); // min cost spanning tree
void Heapify(Edge **edge, int i, int n);    // Maintain heap property
int SetFind(int i);                         // Find the set that element i is in
void WeightedUnion(int i, int j);           // union using the weighting rule.

int main(void)
{
    int i, j;                               // loop index
    double t;                               // for CPU time tracking
    int V, E;                               // the number of nodes and edges
    Edge **edge;                            // variable to record edge
    double mincost;                         // minimum cost
    Edge **T;                               // Minimum-Cost Spanning Tree

    scanf("%d", &V);                        // Read V
    scanf("%d", &E);                        // Read E
    edge = malloc(E * sizeof(Edge*));
    T = malloc((V - 1) * sizeof(Edge*));
    parent = malloc(V * sizeof(int));
    for (i = 0; i < V - 1; i++) {
        T[i] = malloc(sizeof(Edge));
    }
    for (i = 0; i < E; i++) {               // Read edge
        edge[i] = malloc(sizeof(Edge));
        scanf("%d", &edge[i]->u);
        scanf("%d", &edge[i]->v);
        scanf("%lf", &edge[i]->weight);
        edge[i]->u -= 1;                    // to store 1~|V| into 0~|V|-1
        edge[i]->v -= 1;
    }

    t = GetTime();                          // initialize time counter
    mincost = Kruskal(edge, V, E, T);       // find the min cost spanning tree
    t = (GetTime() - t);                    // calculate CPU time

    // print out output
    printf("Minimum-cost spanning tree:\n");
    for (i = 0; i < V - 1; i++) {
        printf("  %d: <%d %d> %.2lf\n", i + 1, T[i]->u + 1, T[i]->v + 1, 
                                        T[i]->weight);
    }
    printf("|V| = %d |E| = %d\n", V, E);
    printf("Minimum cost: %.2lf\n", mincost);
    printf("CPU time: %g seconds\n", t);

    return 0;
}

double GetTime(void)                        // get local time in seconds
{
    struct timeval tv;                      // variable to record the time

    gettimeofday(&tv, NULL);                // get the time
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

double Kruskal(Edge **edge, int V, int E, Edge **T) // min cost spanning tree
{
    int i;                                  // loop index
    int j, k;                               // the parent of u & v
    int i_heap;                             // loop index for heap
    double mincost;                         // minimum cost
    int u, v;                               // the vertices of minimum cost edge
    double w;                               // the weight of minimum cost edge                  

    for (i = E / 2; i > 0; i--) {           // Construct a min heap
        Heapify(edge, i, E);
    }
    for (i = 0; i < V; i++) {               // Enable cycle checking
        parent[i] = -1;
    }
    i = 0;
    mincost = 0;
    i_heap = E;
    while (i < V - 1 && i_heap > 1) {
        // delete a minimum cost edge (u, v) from the heap
        u = edge[0]->u;
        v = edge[0]->v;
        w = edge[0]->weight;
        edge[0] = edge[i_heap - 1];

        Heapify(edge, 1, i_heap - 1);       // Adjust the heap
        i_heap -= 1;

        j = SetFind(u);                     // using parent array
        k = SetFind(v);
        if (j != k) {
            T[i]->u = u;
            T[i]->v = v;
            T[i]->weight = w;
            i += 1;
            mincost += w;
            WeightedUnion(j, k);            // modify parent array
        }
    }

    return mincost;
}

void Heapify(Edge **edge, int i, int n)     // Maintain heap property
{
    int j;                                  // the index of smaller child
    Edge *item;                             // temp var. to store list[i]
    int done;                               // 1: done, 0: undone

    j = 2 * i;                              // A[j] is the lchild
    item = edge[i - 1];
    done = 0;
    while (j <= n && done == 0) {           // A[j + 1] is the rchild.
        if (j < n && edge[j - 1]->weight > edge[j]->weight) {
            j += 1;                         // A[j] is the smaller child.
        }
        // If smaller than children, done
        if (item->weight < edge[j - 1]->weight) {
            done = 1;
        }
        else {                              // Otherwise, continue.
            edge[j / 2 - 1] = edge[j - 1];
            j *= 2;
        }
    }
    edge[j / 2 - 1] = item;
}

int SetFind(int i)                          // Find the set that element i is in
{
    while (parent[i] >= 0) {                // find the set by following the ptr
        i = parent[i];
    }
    
    return i;
}

void WeightedUnion(int i, int j)            // union using the weighting rule.
{
    int temp;                               // temporary variable

    temp = parent[i] + parent[j];           // Note that temp < 0.
    if (parent[i] > parent[j]) {            // i has fewer elements.
        parent[i] = j;
        parent[j] = temp;
    }
    else {                                  // j has fewer elements.
        parent[j] = i;
        parent[i] = temp;
    }
}
