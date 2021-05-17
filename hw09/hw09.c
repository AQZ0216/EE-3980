// EE3980 HW08 Coin Set Design
// 106061151, 劉安得
// 2021/05/15

#include <stdio.h>
#include <stdlib.h>

int NCoinGreedy(int D, int NCoin, int Coins[]); // greedy algorithm

int main(void)
{
    int i, j, k;                                // loop index
    int Coins[] = {1, 5, 10, 50};               // Coin set
    int num;                                    // number of coins
    double aver;                                // the average number of coins
    double min;                                 // minimum average
    int coin_2, coin_3;                         // the values of coins replacing

    // Coin set {1, 5, 10, 50}
    num = 0;
    for (i = 1; i <= 99; i++) {                 // try $1 to $99
        num += NCoinGreedy(i, 4, Coins);
    }
    aver = (double) num / 99;                   // calculate the average
    printf("For coin set {1, 5, 10, 50} the average is %.5lf\n", aver);

    // Coin set {1, 5, 10, dd}
    min = 0;
    for (i = 11; i <= 99; i++) {
        Coins[3] = i;
        num = 0;
        for (j = 1; j <= 99; j++) {
            num += NCoinGreedy(j, 4, Coins);
        }
        aver = (double) num / 99;
        if (min > aver || min == 0) {           // Find the minimum average
            min = aver;
            coin_3 = i;
        }
    }
    printf("Coin set {1, 5, 10, %d} has the minimum average of %.5lf\n",
                                                                coin_3, min);

    // Coin set {1, 5, 10, dd}
    Coins[3] = 50;
    min = 0;
    for (i = 6; i <= 49; i++) {
        Coins[2] = i;
        num = 0;
        for (j = 1; j <= 99; j++) {
            num += NCoinGreedy(j, 4, Coins);
        }
        aver = (double) num / 99;
        if (min > aver || min == 0) {           // Find the minimum average
            min = aver;
            coin_2 = i;
        }
    }
    printf("Coin set {1, 5, %d, 50} has the minimum average of %.5lf\n",
                                                                coin_2, min);

    // Coin set {1, 5, dd, dd}
    min = 0;
    for (i = 6; i <= 98; i++) {
        Coins[2] = i;
        for (j = i; j <= 99; j++) {
            Coins[3] = j;
            num = 0;
            for (k = 1; k <= 99; k++) {
                num += NCoinGreedy(k, 4, Coins);
            }
            aver = (double) num / 99;
            if (min > aver || min == 0) {       // Find the minimum average
                min = aver;
                coin_2 = i;
                coin_3 = j;
            }
        }
    }
    printf("Coin set {1, 5, %d, %d} has the minimum average of %.5lf\n",
                                                        coin_2, coin_3, min);

    return 0;
}

int NCoinGreedy(int D, int NCoin, int Coins[])  // greedy algorithm
{
    int i;                                      // loop index
    int num;                                    // number of coins

    num = 0;
    for (i = NCoin - 1; i >= 0; i--) {          // select max value of coins
        num += D / Coins[i];
	    D = D % Coins[i];
    }

    return num;
}
