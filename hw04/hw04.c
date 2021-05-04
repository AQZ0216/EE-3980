// EE3980 HW04 Stock Short Selling
// 106061151, 劉安得
// 2021/04/03

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sSTKprice {              // the structure to store srock price
    int year, month, day;               // time
    double price, change;               // price and change
} STKprice;

double GetTime(void);                   // get local time in seconds
double MinSubArrayBF(STKprice* A, int n, int *low, int *high);
                                        // Brute-Force Approach
double MinSubArray(STKprice* A, int begin, int end, int *low, int *high);
                                        // Divide and Conquer
double MinSubArrayXB(STKprice* A,
    int begin, int mid, int end, int *low, int *high); // cross boundary

int main(void)
{
    int i;                              // loop index
    double t;                           // for CPU time tracking
    int R = 1000;                       // number of repetitions
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

        // calculate the change of price
        if (i == 0) {
            data[i].change = 0;
        }
        else {
            data[i].change = data[i].price - data[i - 1].price;
        }
    }
    printf("N = %d\n", N);              // print out N = ?
    t = GetTime();                      // initialize time counter
    min = MinSubArrayBF(data, N, &low, &high); // execute brute-force approach
    t = GetTime() - t;                  // calculate CPU time per iteration

    // print out output
    printf("Brute-force approach: CPU time %g s\n", t);
    printf("  Sell: %d/%d/%d at %.4lf\n", data[low - 1].year,
        data[low - 1].month, data[low - 1].day, data[low - 1].price);
    printf("  Buy: %d/%d/%d at %.4lf\n", data[high].year,
        data[high].month, data[high].day, data[high].price);
    printf("  Earning: %.4lf per share.\n", -min);

    t = GetTime();                      // initialize time counter
    for (i = 0; i < R; i++) {
        min = MinSubArray(data, 0, N - 1, &low, &high);
                                        // execute Divide and Conquer
    }
    t = (GetTime() - t) / R;            // calculate CPU time per iteration

    // print out output
    printf("Divide and Conquer: CPU time %g s\n", t);
    printf("  Sell: %d/%d/%d at %.4lf\n", data[low - 1].year,
        data[low - 1].month, data[low - 1].day, data[low - 1].price);
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
    int i, j, k;                        // loop index
    double min, sum;                    // mininum and sum of A[i:j]

    min = 0;                            // Initialize
    *low = 0;
    *high = n - 1;
    for (i = 0; i < n; i++) {           // Try all possible ranges: A[i : j].
        for (j = i; j < n; j++) {
            sum = 0;
            for (k = i; k <= j; k++) {  // Summation for A[i : j]
                sum += A[k].change;
            }
            if (sum < min) {            // Record the minimum  value and range.
                min = sum;
                *low = i;
                *high = j;
            } 
        }
    }
    return min;
}

// Divide and Conquer
double MinSubArray(STKprice* A, int begin, int end, int *low, int *high)
{
    int mid;                            // the middle index
    double lsum, rsum, xsum;            // different sum
    int llow, lhigh, rlow, rhigh, xlow, xhigh;  // different low and high 
    
    if (begin == end) {                 // termination condition.
        *low = begin;
        *high = end;
        return A[begin].change;
    }
    mid = (begin + end) / 2;
    lsum = MinSubArray(A, begin, mid, &llow, &lhigh);   // left region
    rsum = MinSubArray(A, mid + 1, end, &rlow, &rhigh); // right region
    xsum = MinSubArrayXB(A, begin, mid, end, &xlow, &xhigh);
    if (lsum <= rsum && lsum <= xsum) { // lsum is the smallest
        *low = llow;
        *high = lhigh;
        return lsum;
    }
    else if (rsum <= lsum && rsum <= xsum) {    // rsum is the smallest
        *low = rlow;
        *high = rhigh;
        return rsum;
    }
    *low = xlow;
    *high = xhigh;
    return xsum;                        // cross-boundary is the smallest
}

double MinSubArrayXB(STKprice* A,
    int begin, int mid, int end, int *low, int *high) // cross boundary
{
    int i;                              // loop index
    double lsum, rsum, sum;             // left/right sum and sum of A[:]

    lsum = 0;                           // Initialize for lower half.
    *low = mid;
    sum = 0;
    for (i = mid; i >= begin; i--) {    // find low to minimize ΣA[low : mid]
        sum += A[i].change;             // continue to add
        if (sum < lsum) {
            lsum = sum;
            *low = i;
        }
    }
    rsum = 0;                           // Initialize for higher half.
    *high = mid + 1;
    sum = 0;
    for (i = mid + 1; i <= end; i++) {  // find high to minimize ΣA[mid+1:high]
        sum += A[i].change;             // continue to add
        if (sum < rsum) {               // record if smaller.
            rsum = sum;
            *high = i;
        }
    }
    return lsum + rsum;                 // Overall sum.
}
