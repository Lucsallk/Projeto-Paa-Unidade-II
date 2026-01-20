
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"
#include "foward_backward.h"

void classic_alignment(const char *X, const char *Y, int *result) {
    // modificação de space_efficient_alignment
    // TODO: tentar incorporar ambos
    // TODO: função de dividir e conquistar
    int n = strlen(X);
    int m = strlen(Y);

    int dp[n + 1][m + 1];

    dp[0][0] = 0;
    for (int j = 1; j <= m; j++)
        dp[0][j] = dp[0][j - 1] + GAP;
    for (int i = 1; i <= n; i++)
        dp[i][0] = dp[i - 1][0] + GAP;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int match = dp[i - 1][j - 1] + score(X[i - 1], Y[j - 1]);
            int del = dp[i - 1][j] + GAP;
            int ins = dp[i][j - 1] + GAP;

            int max = match;
            if (del > max) max = del;
            if (ins > max) max = ins;

            dp[i][j] = max;
        }
    }

    for (int j = 0; j <= m; j++)
        result[j] = dp[n][j];
}