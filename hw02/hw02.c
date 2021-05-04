// EE3980 HW02 Random Data Searches
// 106061151, 劉安得
// 2021/03/20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

double GetTime(void);                           // get local time in seconds
int Search(char *word, char **list, int n);     // Linear Search
int Search2(char *word, char **list, int n);    // Search 2 Algorithm
int OEsearch(char *word, char **list, int n);   // odd-even Search
int ROEsearch(char *word, char **list, int n);  // randomized Odd-Even Search

int main(void)
{
    int i, j;                                   // loop index
    double t;                                   // for CPU time tracking
    int N;                                      // input size
    char **data;                                // input data
    char *word;                                 // the word to be searchd
    int R1 = 500;                               // number of average repetitions
    int R2 = 5000;                          // number of worst-case repetitions

    scanf("%d", &N);                            // Read n
    data = malloc(N * sizeof(char *));

    // Read a list of n words and store into A array, using Dynamic Store
    for (i = 0; i < N; i++) {
        data[i] = malloc(100 * sizeof(char));
        scanf("%s", data[i]);
    }

    srand((unsigned) time(NULL));               // set random seed
    printf("n: %d\n", N);                       // print out n: ?
    t = GetTime();                              // initialize time counter
    for (i = 0; i < R1; i++) {
        word = data[rand() % N];                // randomly choose a word
        Search(word, data, N);                  // execute linear search
    }
    t = (GetTime() - t) / R1;               // calculate CPU time per iteration
    printf("Linear search average CPU time: %g\n", t);
                                                // print out average CPU time

    // repeat for other searchs
    t = GetTime();
    for (i = 0; i < R1; i++) {
        word = data[rand() % N];
        Search2(word, data, N);
    }
    t = (GetTime() - t) / R1;
    printf("Search 2 algorithm average CPU time: %g\n", t);
    t = GetTime();
    for (i = 0; i < R1; i++) {
        word = data[rand() % N];
        OEsearch(word, data, N);
    }
    t = (GetTime() - t) / R1;
    printf("Odd-even search average CPU time: %g\n", t);
    t = GetTime();
    for (i = 0; i < R1; i++) {
        word = data[rand() % N];
        ROEsearch(word, data, N);
    }
    t = (GetTime() - t) / R1;
    printf("Random odd-even search average CPU time: %g\n", t);

    word = data[N - 1];                     // choose data[N-1] as worst-case
    t = GetTime();                              // initialize time counter
    for (i = 0; i < R2; i++) {
        Search(word, data, N);                  // execute linear search
    }
    t = (GetTime() - t) / R2;               // calculate CPU time per iteration
    printf("Linear search worst-case CPU time: %g\n", t);
                                                // print out worst-case CPU time

    // repeat for other searchs
    word = data[N - 1];                     // choose data[N-1] as worst-case
    t = GetTime();
    for (i = 0; i < R2; i++) {
        Search2(word, data, N);
    }
    t = (GetTime() - t) / R2;
    printf("Search 2 algorithm worst-case CPU time: %g\n", t);
    // choose worst-case according to whether N is even or odd
    if (N % 2 == 0) {
        word = data[N - 1];
    }
    else {
        word = data[N - 2];
    }
    t = GetTime();
    for (i = 0; i < R2; i++) {
        OEsearch(word, data, N);
    }
    t = (GetTime() - t) / R2;
    printf("Odd-even search worst-case CPU time: %g\n", t);
    // choose worst-case according to whether N is even or odd
    if (N % 2 == 0) {
        word = data[N - 1];
    }
    else {
        word = data[N - 2];
    }
    t = GetTime();
    for (i = 0; i < R2; i++) {
        ROEsearch(word, data, N);
    }
    t = (GetTime() - t) / R2;
    printf("Random odd-even search worst-case CPU time: %g\n", t);

    return 0;
}

double GetTime(void)                            // get local time in seconds
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int Search(char *word, char **list, int n)      // Linear Search
{
    int i;                                      // loop index

    for (i = 0; i < n; i++) {                   // compare all possible entries
        if (strcmp(list[i], word) == 0) {
            return i;
        }
    }
    return -1;                                  // unsuccessful search
}

int Search2(char *word, char **list, int n)     // odd-even Search
{
    int i;                                      // loop index

    for (i = 0; i < n; i += 2) {                // compare all possible entries
        if (strcmp(list[i], word) == 0) {
            return i;
        }
        if (i + 1 < n && strcmp(list[i + 1], word) == 0) {
            return i + 1;
        }
    }
    return -1;                                  // unsuccessful search
}

int OEsearch(char *word, char **list, int n)    // randomized Odd-Even Search
{
    int i;                                      // loop index

    for (i = 0; i < n; i += 2) {                // compare odd entries
        if (strcmp(list[i], word) == 0) {
            return i;
        }
    }
    for (i = 1; i < n; i += 2) {                // compare even entries
        if (strcmp(list[i], word) == 0) {
            return i;
        }
    }
    return -1;                                  // unsuccessful search
}

int ROEsearch(char *word, char **list, int n)
{
    int i, j;                                   // loop index and random result

    j = rand() % 2;
    if (j == 1) {
        for (i = 0; i < n; i += 2) {            // compare odd entries
            if (strcmp(list[i], word) == 0) {
                return i;
            }
        }
        for (i = 1; i < n; i += 2) {            // compare even entries
            if (strcmp(list[i], word) == 0) {
                return i;
            }
        }
    }
    else {
        for (i = 1; i < n; i += 2) {            // compare even entries
            if (strcmp(list[i], word) == 0) {
                return i;
            }
        }
        for (i = 0; i < n; i += 2) {            // compare odd entries
            if (strcmp(list[i], word) == 0) {
                return i;
            }
        }
    }
    return -1;                                  // unsuccessful search
}
