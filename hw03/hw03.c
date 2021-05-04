// EE3980 HW03 Network Connectivity Problem
// 106061151, 劉安得
// 2021/03/27

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int V, E;                               // the number of nodes and edges
int NS;                                 // Number of disjoint sets
int *edge;                              // variable to record edge
int *p;                                 // represent the disjoint sets
int *R;                                 // the array of the root of the set.

double GetTime(void);                   // get local time in seconds
void Connect1(void);                    // SetFind & SetUnion
void Connect2(void);                    // SetFind & WeightedUnion
void Connect3(void);                    // CollapsingFind & WeightedUnion
int SetFind(int i);                     // Find the set that element i is in.
void SetUnion(int i, int j);            // union two sets with roots, i and j.
void WeightedUnion(int i, int j);       // union using the weighting rule.
int CollapsingFind(int i);              // Find using collapsing rule

int main(void)
{
    int i;                              // loop index
    double t0, t1, t2, t3;              // for CPU time tracking
    int NS1, NS2, NS3;                  // number of sets found by functions
    int N_REPEAT = 100;                 // number of repetitions

    // Read a network from stdin
    scanf("%d", &V);                    // Read V
    scanf("%d", &E);                    // Read E
    edge = malloc(2 * E * sizeof(int));
    p = malloc(V * sizeof(int));
    R = malloc(V * sizeof(int));
    for (i = 0; i < E; i++) {           // Read edge
        scanf("%d", &edge[2 * i]);
        scanf("%d", &edge[2 * i + 1]);
        edge[2 * i] -= 1;               // to store 1~100 into 0~99
        edge[2 * i + 1] -= 1;
    }

    t0 = GetTime();                     // Record time
    for (i = 0; i < N_REPEAT; i++) {
        Connect1();                     // execute Connect1
    }
    t1 = GetTime();                     // Record time and number of sets found.
    NS1 = NS;
    for (i = 0; i < N_REPEAT; i++) {
        Connect2();                     // execute Connect2
    }
    t2 = GetTime();                     // Record time and number of sets found.
    NS2 = NS;
    for (i = 0; i < N_REPEAT; i++) {
        Connect3();                     // execute Connect3
    }
    t3 = GetTime();                     // Record time and number of sets found.
    NS3 = NS;

    // print out output
    printf("|V| = %d, |E| = %d\n", V, E);
    printf("Connect1 CPU time: %g, ", (t1 - t0) / N_REPEAT);
    printf("Disjoint sets: %d\n", NS1);
    printf("Connect2 CPU time: %g, ", (t2 - t1) / N_REPEAT);
    printf("Disjoint sets: %d\n", NS2);
    printf("Connect3 CPU time: %g, ", (t3 - t2) / N_REPEAT);
    printf("Disjoint sets: %d\n", NS3); 
    return 0;
}

double GetTime(void)                    // get local time in seconds
{
    struct timeval tv;                  // variable to record the time

    gettimeofday(&tv, NULL);            // get the time
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void Connect1(void)                     // SetFind & SetUnion
{
    int i;                              // loop index
    int si, sj;

    for (i = 0; i < V; i++) {           // One element for each set.
        p[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {           // Connected vertices
        si = SetFind(edge[2 * i]);
        sj = SetFind(edge[2 * i + 1]);
        if (si != sj) {                 // Unite two sets.
            NS -= 1;                    // # of disjoint sets decreases by 1.
            SetUnion(si, sj);
        }
    }
    for (i = 0; i < V; i++) {           // Record root to R table.
        R[i] = SetFind(i);
    }
}

void Connect2(void)                     // SetFind & WeightedUnion
{
    int i;                              // loop index
    int si, sj;

    for (i = 0; i < V; i++) {           // One element for each set.
        p[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {           // Connected vertices
        si = SetFind(edge[2 * i]);
        sj = SetFind(edge[2 * i + 1]);
        if (si != sj) {                 // Unite two sets.
            NS -= 1;                    // # of disjoint sets decreases by 1.
            WeightedUnion(si, sj);
        }
    }
    for (i = 0; i < V; i++) {           // Record root to R table.
        R[i] = SetFind(i);
    }
}

void Connect3(void)                     // CollapsingFind & WeightedUnion
{
    int i;                              // loop index
    int si, sj;

    for (i = 0; i < V; i++) {           // One element for each set.
        p[i] = -1;
    }
    NS = V;
    for (i = 0; i < E; i++) {           // Connected vertices
        si = CollapsingFind(edge[2 * i]);
        sj = CollapsingFind(edge[2 * i + 1]);
        if (si != sj) {                 // Unite two sets.
            NS -= 1;                    // # of disjoint sets decreases by 1.
            WeightedUnion(si, sj);
        }
    }
    for (i = 0; i < V; i++) {           // Record root to R table.
        R[i] = SetFind(i);
    }
}

int SetFind(int i)                      // Find the set that element i is in.
{
    while (p[i] >= 0) {                 // find the set  by following the ptr
        i = p[i];
    }
    
    return i;
}

void SetUnion(int i, int j)             // union two sets with roots, i and j.
{
    p[i] = j;                           // setting j to be the parent of i
}

void WeightedUnion(int i, int j)        // union using the weighting rule.
{
    int temp;                           // temporary variable

    temp = p[i] + p[j];                 // Note that temp < 0.
    if (p[i] > p[j]) {                  // i has fewer elements.
        p[i] = j;
        p[j] = temp;
    }
    else {                              // j has fewer elements.
        p[j] = i;
        p[i] = temp;
    }
}

int CollapsingFind(int i)               // Find using collapsing rule
{
    int r;                              // root
    int s;                              // temp variable to record the parent
    
    r = i;                              // Initialized r to i.
    while (p[r] >= 0) {                 // Find the root.
        r = p[r];
    }
    while (i != r) {                    // Collapse the elements on the path.
        s = p[i];
        p[i] = r;
        i = s;
    }
    
    return r;
}
