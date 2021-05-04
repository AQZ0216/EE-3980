// EE3980 HW01 Quadratic Sorts
// 106061151, 劉安得
// 2021/03/13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void copyArray(char **data, char **A, int n);   // copy data to array A
double GetTime(void);                           // get local time in seconds
void SelectionSort(char **list, int n);         // in-place selection sort
void InsertionSort(char **list, int n);         // in-place insertion sort
void BubbleSort(char **list, int n);            // in-place bubble sort
void OddEvenSort(char **list, int n);           // in-place shaker sort

int main(void)
{
    int i, j;                                   // loop index
    double t;                                   // for CPU time tracking
    int N;                                      // input size
    char **data;                                // input data
    char **A;                                   // array to be sorted
    int R = 500;                                // number of repetitions

    scanf("%d", &N);                            // Read n
    data = malloc(N * sizeof(char *));
    A = malloc(N * sizeof(char *));

    // Read a list of n words and store into A array, using Dynamic Store
    for (i = 0; i < N; i++) {
        data[i] = malloc(100 * sizeof(char));
        scanf("%s", data[i]);
    }

    t = GetTime();                              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);                  // initialize array for sorting
        SelectionSort(A, N);                    // execute selection sort
    }
    t = (GetTime() - t) / R;                // calculate CPU time per iteration
    printf("Selection sort: N = %d CPU = %g seconds\n", N, t);
                                                // print out CPU time

    // repeat for other sorts
    t = GetTime();
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);
        InsertionSort(A, N);
    }
    t = (GetTime() - t) / R;
    printf("Insertion sort: N = %d CPU = %g seconds\n", N, t);
    t = GetTime();
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);
        BubbleSort(A, N);
    }
    t = (GetTime() - t) / R;
    printf("Bubble sort: N = %d CPU = %g seconds\n", N, t);
    t = GetTime();
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);
        OddEvenSort(A, N);
    }
    t = (GetTime() - t) / R;
    printf("OddEven sort: N = %d CPU = %g seconds\n", N, t);

    for (i = 0; i < N; i++) {
        printf("%d %s\n", i + 1, A[i]);             // print sorted results
    }

    return 0;
}

void copyArray(char **data, char **A, int n)
{
    int i;                                              // loop index
    
    for (i = 0; i < n; i++) {
        A[i] = malloc(sizeof(char) * strlen(data[i]));  // dynamic allocate A[i]
        A[i] = data[i];                          // copy data to A
    }
}

double GetTime(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void SelectionSort(char **list, int n)
{
    int i, j;                                   // loop index
    int min;                        // to record the index of smallest element
    char *temp;                                 // temporary variable for swap

    for (i = 0; i < n-1; i++) {
        min = i;
        for (j = i+1; j < n; j++) {     // Search for the smallest in A[i+1:n]
            if (strcmp(list[j], list[min]) < 0) {
                min = j;                        // Found, remember it in min
            }
        }
        // Swap A[i] and A[min].
        temp = list[i];
        list[i] = list[min];
        list[min] = temp;
    }
}

void InsertionSort(char **list, int n)
{
    int i, j;                               // loop index
    char *item;                             // item to save list[i] temporary

    for (i = 1; i < n; i++) {               // Assume A[1:i-1] already sorted
        item = list[i];
        // Find j such that A[i] < A[j].
        for (j = i-1; j >= 0 && strcmp(item, list[j]) < 0; j--) {
            list[j+1] = list[j];            // Move A[j] up by one position
        }
        list[j+1] = item;                   // Move A[i] to A[i+1].
    }    
}

void BubbleSort(char **list, int n)
{
    int i, j;                               // loop index
    char *temp;                             // temporary variable for swap

    for (i = 0; i < n-1; i++) {             // Find the smallest item for A[i]
        for (j = n-1; j > i; j--) {
            if (strcmp(list[j-1], list[j]) > 0) {
                // Swap A[j] and A[j-1].
                temp = list[j];
                list[j] = list[j-1];
                list[j-1] = temp;
            }
        }

    }
}

void OddEvenSort(char **list, int n)
{
    int done = 0;                       // the state flag, 1: done, 0: undone
    
    int i;                              // loop index
    char *temp;                         // temporary variable for swap
    
    while (done != 1) {
        done = 1;                       // set the flag as done
        for (i = 1; i < n-1; i+=2) {
            if (strcmp(list[i], list[i+1]) > 0) {
                done = 0;               // set the flag as undone
                // Swap A[i] and A[i-1].
                temp = list[i];
                list[i] = list[i+1];
                list[i+1] = temp;
            }
        }
        for (i = 0; i < n-1; i+=2) {
            if (strcmp(list[i], list[i+1]) > 0) {
                done = 0;                // set the flag as undone
                // Swap A[i] and A[i-1].
                temp = list[i];
                list[i] = list[i+1];
                list[i+1] = temp;
            }
        }
    }
}
