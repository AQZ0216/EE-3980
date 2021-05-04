// EE3980 HW05 Better Sorts
// 106061151, 劉安得
// 2021/04/10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

char **B;                                       // temporary storage

void copyArray(char **data, char **A, int n);   // copy data to array A
double GetTime(void);                           // get local time in seconds
void HeapSort(char **list, int n);              // use max heap to sort
void MergeSort(char **list, int low, int high); // merge sort recur fn
void QuickSort(char **list, int low, int high); // quick sort recur fn.
void Heapify(char **list, int i, int n);        // Maintain heap property
void Merge(char **list, int low, int mid, int high);    // merge two lists
int Partition(char **list, int low, int high);  // Partition A by a pivot
void Swap(char **list, int i, int j);           // swap list[i] and list[j]

int main()
{
    int i, j;                                   // loop index
    double t;                                   // for CPU time tracking
    int N;                                      // input size
    char **data;                                // input data
    char **A;                                   // array to be sorted
    int R = 5000;                               // number of repetitions

    scanf("%d", &N);                            // Read n
    data = malloc(N * sizeof(char *));
    A = malloc(N * sizeof(char *));
    B = malloc(N * sizeof(char *));

    // Read a list of n words and store into A array
    for (i = 0; i < N; i++) {
        data[i] = malloc(100 * sizeof(char));
        scanf("%s", data[i]);
    }

    printf("N = %d\n", N);                      // print out N = ?
    t = GetTime();                              // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);                  // initialize array for sorting
        HeapSort(A, N);                         // execute heap sort
    }
    t = (GetTime() - t) / R;                // calculate CPU time per iteration
    printf("HeapSort CPU time: %g s\n", t);     // print out CPU time

    // repeat for other sorts
    t = GetTime();
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);
        MergeSort(A, 0, N - 1);
    }
    t = (GetTime() - t) / R;
    printf("MergeSort CPU time: %g s\n", t);
    t = GetTime();
    for (i = 0; i < R; i++) {
        copyArray(data, A, N);
        QuickSort(A, 0, N - 1);
    }
    t = (GetTime() - t) / R;
    printf("QuickSort CPU time: %g s\n", t);

    for (i = 0; i < N; i++) {
        printf("%d %s\n", i + 1, A[i]);             // print sorted results
    }

    return 0;
}

void copyArray(char **data, char **A, int n)    // copy data to array A
{
    int i;                                      // loop index
    
    for (i = 0; i < n; i++) {
        A[i] = malloc(sizeof(char) * strlen(data[i]));  // dynamic allocate A[i]
        A[i] = data[i];                         // copy data to A
    }
}

double GetTime(void)                            // get local time in seconds
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void HeapSort(char **list, int n)               // use max heap to sort
{
    int i;                                      // loop index
    char *temp;                                 // temporary variable for swap

    for (i = n / 2; i > 0; i--) {               // Init. A[1:n] to be a max heap
        Heapify(list, i, n);
    }
    for (i = n; i > 1; i--) {                   // Repeat n − 1 times
        // Move maximum to the end.
        temp = list[i - 1];
        list[i - 1] = list[0];
        list[0] = temp;

        Heapify(list, 1, i - 1);                // Then make A[1:i-1] a max heap
    }
}

void Heapify(char **list, int i, int n)         // Maintain heap property
{
    int j;                                      // the index of larger child
    char *item;                                 // temp var. to store list[i]
    int done;                                   // 1: done, 0: undone

    j = 2 * i;                                  // A[j] is the lchild
    item = list[i - 1];
    done = 0;
    while (j <= n && done == 0) {               // A[j + 1] is the rchild.
        if (j < n && strcmp(list[j - 1], list[j]) < 0) {
            j += 1;                             // A[j] is the larger child.
        }
        if (strcmp(item, list[j - 1]) > 0) {    // If larger than children, done
            done = 1;
        }
        else {                                  // Otherwise, continue.
            list[j / 2 - 1] = list[j - 1];
            j *= 2;
        }
    }
    list[j / 2 - 1] = item;
}

void MergeSort(char **list, int low, int high)  // merge sort recur fn
{
    int mid;                                    // the middle index
    
    if (low < high) {                           // if n > 1
        mid = (low + high) / 2;
        MergeSort(list, low, mid);              // sort list[low : mid]
        MergeSort(list, mid + 1, high);         // sort list[mid+1 : high]
        Merge(list, low, mid, high);            // merge two lists
    }
}

void Merge(char **list, int low, int mid, int high) // merge two lists
{
    int i, j, k, l;                             // loop index

    // Initialize looping indices.
    i = low;
    j = low;
    k = mid + 1;

    while (j <= mid && k <= high) {             // Store smaller one to B[i].
        if (strcmp(list[j], list[k]) <= 0) {    // A[h] is smaller.
            B[i] = list[j];
            j++;
        }
        else {                                  // A[j] is smaller.
            B[i] = list[k];
            k++;
        }
        i++;
    }
    if (j > mid) {                              // A[j : high] remaining.
        for (l = k; l <= high; l++) {
            B[i] = list[l];
            i++;
        }
    }
    else {                                      // A[h : mid] remaining.
        for (l = j; l <= mid; l++) {
            B[i] = list[l];
            i++;
        }
    }
    for (l = low; l <= high; l++) {             // Copy B to A.
        list[l] = B[l];
    }
}

void QuickSort(char **list, int low, int high)  // quick sort recur. fn.
{
    int mid;                                    // the pivot index

    if (low < high) {                           // if n > 1
        mid = Partition(list, low, high + 1);       // Partition A by a pivot
        QuickSort(list, low, mid - 1);          // sort list[low : mid-1]
        QuickSort(list, mid + 1, high);         // sort list[mid+1 : high]
    }
}

int Partition(char **list, int low, int high)   // Partition A by a pivot
{
    char *v;                                    // temp var. to store list[low]
    int i, j;                                   // index for swaping

    // Initialize
    v = list[low];
    i = low;
    j = high;

    do {                                        // Check for all elements.
        do {                                    // Find i such that A[i] >= v.
            i++;
        } while (i < high && strcmp(list[i], v) < 0);
        do {                                    // Find j such that A[j] <= v.
            j--;
        } while (strcmp(list[j], v) > 0); 
        if (i < j) {
            Swap(list, i, j);                   // Exchange A[i] and A[j].
        }
    } while (i < j);

    // Move v to the right position.
    list[low] = list[j];
    list[j] = v;
    
    return j;
}

void Swap(char **list, int i, int j)            // swap list[i] and list[j]
{
    char *temp;                                 // temporary variable for swap

    temp = list[i];
    list[i] = list[j];
    list[j] = temp;    
}
