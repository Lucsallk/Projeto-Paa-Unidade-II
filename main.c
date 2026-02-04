#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MATCH 0
#define MISMATCH 1
#define GAP 2

int score(char a, char b) {
    return (a == b) ? MATCH : MISMATCH;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

void space_efficient_alignment(const char *X, const char *Y, int *result) {
    int n = (int)strlen(X);
    int m = (int)strlen(Y);

    // vetores indexados por i (0..n): queremos custos para todos i numa coluna final
    int *prev = (int*)malloc((n + 1) * sizeof(int));
    int *curr = (int*)malloc((n + 1) * sizeof(int));

    // dp[i][0] = i * GAP
    for (int i = 0; i <= n; i++) prev[i] = i * GAP;

    for (int j = 1; j <= m; j++) {
        curr[0] = j * GAP; // dp[0][j] = j * GAP
        for (int i = 1; i <= n; i++) {
            int diag = prev[i - 1] + score(X[i - 1], Y[j - 1]); // dp[i-1][j-1]
            int up   = curr[i - 1] + GAP;                      // dp[i-1][j]
            int left = prev[i] + GAP;                          // dp[i][j-1]
            curr[i] = min3(diag, up, left);
        }
        int *tmp = prev; prev = curr; curr = tmp;
    }

    // Agora prev[i] = dp[i][m] para i=0..n  (última coluna)
    for (int i = 0; i <= n; i++) result[i] = prev[i];

    free(prev);
    free(curr);
}

void backward_space_efficient_alignment(const char *X, const char *Y, int *result) {
    int n = (int)strlen(X);
    int m = (int)strlen(Y);

    char *X_rev = (char*)malloc((n + 1) * sizeof(char));
    char *Y_rev = (char*)malloc((m + 1) * sizeof(char));
    reverse_string(X, X_rev);
    reverse_string(Y, Y_rev);

    // retorna custos dp_rev[i][m] em função de i (0..n), ou seja, vetor tamanho n+1
    space_efficient_alignment(X_rev, Y_rev, result);

    free(X_rev);
    free(Y_rev);
}


void reverse_string(const char *src, char *dst) {
    int len = strlen(src);
    for (int i = 0; i < len; i++)
        dst[i] = src[len - 1 - i];
    dst[len] = '\0';
}


void needleman_wunsch(const char *X, const char *Y, char **outX, char **outY) {
    int n = strlen(X);
    int m = strlen(Y);
    int **dp = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
        dp[i] = malloc((m + 1) * sizeof(int));

    for (int i = 0; i <= n; i++) dp[i][0] = i * GAP;
    for (int j = 0; j <= m; j++) dp[0][j] = j * GAP;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int diag = dp[i - 1][j - 1] + score(X[i - 1], Y[j - 1]);
            int up = dp[i - 1][j] + GAP;
            int left = dp[i][j - 1] + GAP;
            dp[i][j] = min3(diag, up, left);
        }
    }

    char *aX = malloc(n + m + 1);
    char *aY = malloc(n + m + 1);
    int i = n, j = m, k = 0;

while (i > 0 || j > 0) {
    int current = dp[i][j];
    int diag = (i > 0 && j > 0) ? dp[i - 1][j - 1] + score(X[i - 1], Y[j - 1]) : INT_MAX;
    int left = (j > 0) ? dp[i][j - 1] + GAP : INT_MAX;
    int up   = (i > 0) ? dp[i - 1][j] + GAP : INT_MAX;

    if (i > 0 && j > 0 && current == diag) {
        aX[k] = X[i - 1];
        aY[k] = Y[j - 1];
        i--; j--;
    } else if (j > 0 && current == left) {
        aX[k] = '-';
        aY[k] = Y[j - 1];
        j--;
    } else if (i > 0 && current == up) {
        aX[k] = X[i - 1];
        aY[k] = '-';
        i--;
    }

    k++;
}



    aX[k] = '\0'; aY[k] = '\0';

    // inverter os alinhamentos
    *outX = malloc(k + 1);
    *outY = malloc(k + 1);
    for (int t = 0; t < k; t++) {
        (*outX)[t] = aX[k - 1 - t];
        (*outY)[t] = aY[k - 1 - t];
    }
    (*outX)[k] = '\0';
    (*outY)[k] = '\0';

    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    free(aX);
    free(aY);
}

void divide_and_conquer(const char *X, const char *Y, char **outX, char **outY) {
    int n = strlen(X);
    int m = strlen(Y);

    if (n == 0) {
        *outX = malloc(m + 1);
        *outY = strdup(Y);
        for (int i = 0; i < m; i++) (*outX)[i] = '-';
        (*outX)[m] = '\0';
        return;
    }
    if (m == 0) {
        *outX = strdup(X);
        *outY = malloc(n + 1);
        for (int i = 0; i < n; i++) (*outY)[i] = '-';
        (*outY)[n] = '\0';
        return;
    }
    if (n <= 2 || m <= 2) {
        needleman_wunsch(X, Y, outX, outY);
        return;
    }

    int mid = m / 2;

    int *L = malloc((n + 1) * sizeof(int));
    int *R = malloc((n + 1) * sizeof(int));

    char *Y_left = strndup(Y, mid);
    space_efficient_alignment(X, Y_left, L);

    backward_space_efficient_alignment(X, Y + mid, R);

    int best = L[0] + R[n];
    int q = 0;
    for (int i = 1; i <= n; i++) {
        int cost = L[i] + R[n - i];
        if (cost < best) {
            best = cost;
            q = i;
        }
    }


    char *leftX, *leftY, *rightX, *rightY;

    char *Xl = malloc(q + 1);
    memcpy(Xl, X, q); Xl[q] = '\0';

    char *Xr = strdup(X + q);

    char *Yl = malloc(mid + 1);
    memcpy(Yl, Y, mid); Yl[mid] = '\0';

    char *Yr = strdup(Y + mid);
    divide_and_conquer(Xl, Yl, &leftX, &leftY);
    divide_and_conquer(Xr, Yr, &rightX, &rightY);

    *outX = malloc(strlen(leftX) + strlen(rightX) + 1);
    *outY = malloc(strlen(leftY) + strlen(rightY) + 1);
    strcpy(*outX, leftX); strcat(*outX, rightX);
    strcpy(*outY, leftY); strcat(*outY, rightY);

    free(L); free(R);
    free(Xl); free(Xr); free(Yl); free(Yr);
    free(leftX); free(leftY); free(rightX); free(rightY);
}

int compute_cost(const char *A, const char *B) {
    int cost = 0;
    for (int i = 0; A[i]; i++) {
        if (A[i] == '-' || B[i] == '-') cost += GAP;
        else cost += score(A[i], B[i]);
    }
    return cost;
}

int main() {
    char X[10001], Y[10001];
    scanf("%s", X);
    scanf("%s", Y);

    char *alignedX, *alignedY;
    divide_and_conquer(X, Y, &alignedX, &alignedY);

    int total_cost = compute_cost(alignedX, alignedY);
    printf("%d\n%s\n%s\n", total_cost, alignedX, alignedY);

    free(alignedX);
    free(alignedY);
    return 0;
}
