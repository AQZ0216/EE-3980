// EE3980 HW07 Grouping Friends
// 106061151, 劉安得
// 2021/05/01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct sNode {                      // linked list node
    int data;                               // for data storage
    struct sNode *link;                     // pointer to the next node
} Node;

int N, M;                                   // input size
int time;                                   // Global variable to track time.
int N_group;                                // variable to record the # of trees
int *visited, *f, *v;                       // visited, finish time, order of V

double GetTime(void);                       // get local time in seconds
void DFS_Call(Node **adj_list);             // Init. and recur DFS function call
void DFS(Node **adj_list, int v, int i);    // Depth First Search

int main(void)
{
    int i, j;                               // loop index
    double t;                               // for CPU time tracking
    char **name;                            // variable to record the names
    char sent[13], receive[13];             // variable to read the records
    int sent_idx, receive_idx;              // the index of records
    Node *p;                                // temporary node
    Node **G, **GT;                         // graph of edges and transpose

    // Read a communication list from stdin
    scanf("%d", &N);                        // Read N
    scanf("%d", &M);                        // Read M
    name = malloc(N * sizeof(char *));
    G = malloc(N * sizeof(Node*));
    GT = malloc(N * sizeof(Node*));
    visited = malloc(N * sizeof(int));
    f = malloc(N * sizeof(int));
    v = malloc(N * sizeof(int));
    for (i = 0; i < N; i++) {               // Read names
        name[i] = malloc(13 * sizeof(char));
        scanf("%s", name[i]);
        G[i] = malloc(sizeof(Node));
        GT[i] = malloc(sizeof(Node));
        G[i]->link = NULL;
        GT[i]->link = NULL;
        v[i] = i;
    }
    for (i = 0; i < M; i++) {               // Read records
        scanf("%s", sent);
        scanf("%s");
        scanf("%s", receive);
        for (j = 0; j < N; j++) {
            if (strcmp(name[j], sent) == 0) {
                sent_idx = j;
            }
            else if (strcmp(name[j], receive) == 0) {
                receive_idx = j;
            }
        }

        // Construct the graph G
        p = G[sent_idx];
        while (p->link != NULL) {
            p = p->link;
        }
        p->link = malloc(sizeof(Node));
        p->link->data = receive_idx;
        p->link->link = NULL;

        // Construct the transpose graph GT
        p = GT[receive_idx];
        while (p->link != NULL) {
            p = p->link;
        }
        p->link = malloc(sizeof(Node));
        p->link->data = sent_idx;
        p->link->link = NULL;
    }

    // GCC()
    t = GetTime();                          // initialize time counter
    DFS_Call(G);                            // Perform DFS to get array f[1:n].
    for (i = 0; i < N; i++) {               // Sort V of GT in order of f[v].
        v[N - f[i]] = i;
    }
    DFS_Call(GT);                           // Perform DFS on GT.
    t = (GetTime() - t);                    // calculate CPU time

    // print out output
    printf("N = %d M = %d Subgroup = %d CPU time = %g\n", N, M, N_group, t);
    printf("Number of subgroups: %d\n", N_group);
    for (i = 1; i <= N_group; i++) {
        printf("  Subgroup %d:", i);
        for (j = 0; j < N; j++) {
            if (visited[j] == i) {
                printf(" %s", name[j]);
            }
        }
        printf("\n");
    }
 
    return 0;
}

double GetTime(void)                        // get local time in seconds
{
    struct timeval tv;                      // variable to record the time

    gettimeofday(&tv, NULL);                // get the time
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void DFS_Call(Node **adj_list)              // Init. and recur DFS function call
{
    int i;                                  // loop index

    for (i = 0; i < N; i++) {               // Init. to not visited
        visited[i] = 0;
        f[i] = 0;
    }
    time = 0;                               // Global variable to track time.
    N_group = 0;                            // variable to record the # of trees
    for (i = 0; i < N; i++) {               // To handle forest case.
        if (visited[v[i]] == 0) {
            N_group++;
            DFS(adj_list, v[i], N_group);
        }
    }
}

void DFS(Node **adj_list, int v, int i)     // Depth First Search
{
    Node *p;                                // temporary node
    int w;                                  // the index to recur DFS
    
    visited[v] = i;                         // record the index of trees
    p = adj_list[v];
    while (p->link != NULL) {               // for each vertex w adjacent to v
        p = p->link;
        w = p->data;
        if (visited[w] == 0) {              // if not visited
            DFS(adj_list, w, i);            // keep DFS
        }
    }
    time += 1;
    f[v] = time;
}
