// EE3980 HW06 Stock Short Selling Revisited
// 106061151, 劉安得
// 2021/04/17

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sSTKprice {              // the structure to store srock price
    int year, month, day;               // time
    double price;                       // price
} STKprice;

double GetTime(void);                   // get local time in seconds
double MinSubArrayBF(STKprice* A, int n, int *low, int *high);
                                        // Brute-Force Approach
double MinSubArray(STKprice* A, int n, int *low, int *high);
                                        // Dynamic Programming

int main(void)
{
    int i;                              // loop index
    double t;                           // for CPU time tracking
    int R = 5000;                       // number of repetitions
    int N;                              // input size
    STKprice *data;                     // stock price entries
    int low, high;                      // the index of minimum subarray 
    double min;                         // the sum of minimum subarray

    // Read a network from stdin
    scanf("%d", &N);                    // Read N
    data = malloc(N * sizeof(STKprice));
    for (i = 0; i < N; i++) {           // Read stock price entries
        scanf("%d", &data[i].year);
        scanf("%d", &data[i].month);
        scanf("%d", &data[i].day);
        scanf("%lf", &data[i].price);
    }
    printf("N = %d\n", N);              // print out N = ?
    t = GetTime();                      // initialize time counter
    for (i = 0; i < R; i++) {
        min = MinSubArrayBF(data, N, &low, &high);
                                        // execute brute-force approach
    }
    t = (GetTime() - t) / R;            // calculate CPU time per iteration

    // print out output
    printf("Brute-force approach: CPU time %g s\n", t);
    printf("  Sell: %d/%d/%d at %.4lf\n", data[low].year,
        data[low].month, data[low].day, data[low].price);
    printf("  Buy: %d/%d/%d at %.4lf\n", data[high].year,
        data[high].month, data[high].day, data[high].price);
    printf("  Earning: %.4lf per share.\n", -min);

    t = GetTime();                      // initialize time counter
    for (i = 0; i < R; i++) {
        min = MinSubArray(data, N, &low, &high);
                                        // execute Dynamic Programming
    }
    t = (GetTime() - t) / R;            // calculate CPU time per iteration

    // print out output
    printf("Dynamic Programming: CPU time %g s\n", t);
    printf("  Sell: %d/%d/%d at %.4lf\n", data[low].year,
        data[low].month, data[low].day, data[low].price);
    printf("  Buy: %d/%d/%d at %.4lf\n", data[high].year,
        data[high].month, data[high].day, data[high].price);
    printf("  Earning: %.4lf per share.\n", -min);
 
    return 0;
}

double GetTime(void)                    // get local time in seconds
{
    struct timeval tv;                  // variable to record the time

    gettimeofday(&tv, NULL);            // get the time
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

// Brute-Force Approach
double MinSubArrayBF(STKprice* A, int n, int *low, int *high)
{
    int i, j;                           // loop index
    double min, sum;                    // mininum and sum of A[j]-A[i]

    min = 0;                            // Initialize
    *low = 0;
    *high = n - 1;
    for (i = 0; i < n; i++) {           // Try all possible ranges: A[i : j].
        for (j = i; j < n; j++) {
            sum = A[j].price - A[i].price;  // A[j]-A[i]
            if (sum < min) {            // Record the minimum  value and range.
                min = sum;
                *low = i;
                *high = j;
            }
        }
    }
    return min;
}

// Dynamic Programming
double MinSubArray(STKprice* A, int n, int *low, int *high)
{
    int i;                              // loop index
    double min, sum;                    // mininum and sum of A[i]-A[local_low]
    int local_low;                      // j which min. A[i]-A[j] for all j <= i

    min = 0;                            // Initialize
    *low = 0;
    *high = n - 1;
    local_low = 0;
    for (i = 0; i < n; i++) {           // loop all A[i].
        sum = A[i].price - A[local_low].price;
        if (sum < min) {                // Record the minimum value and range.
            min = sum;
            *low = local_low;
            *high = i;
        }
        else if (sum > 0) {             // if sum > 0, for i+1, local_low = i
            local_low = i;
        }
    }
    return min;
}
