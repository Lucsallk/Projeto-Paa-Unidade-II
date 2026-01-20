#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void space_efficient_alignment(const char *X, const char *Y, int *result) {
    int n = strlen(X);
    int m = strlen(Y);

    int *prev = malloc((m + 1) * sizeof(int));
    int *curr = malloc((m + 1) * sizeof(int));

    prev[0] = 0;
    for (int j = 1; j <= m; j++) {
        prev[j] = prev[j - 1] + GAP;
    }

    for (int i = 1; i <= n; i++) {
        curr[0] = prev[0] + GAP;

        for (int j = 1; j <= m; j++) {
            int match = prev[j - 1] + score(X[i - 1], Y[j - 1]);
            int delete = prev[j] + GAP;
            int insert = curr[j - 1] + GAP;

            int max = match;
            if (delete > max) max = delete;
            if (insert > max) max = insert;

            curr[j] = max;
        }

        int *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    for (int j = 0; j <= m; j++) {
        result[j] = prev[j];
    }

    free(prev);
    free(curr);
}

void reverse_string(const char *src, char *dst) {
    int len = strlen(src);
    for (int i = 0; i < len; i++) {
        dst[i] = src[len - 1 - i];
    }
    dst[len] = '\0';
}

void backward_space_efficient_alignment(const char *X, const char *Y, int *result) {
    int n = strlen(X);
    int m = strlen(Y);

    char *X_rev = malloc((n + 1) * sizeof(char));
    char *Y_rev = malloc((m + 1) * sizeof(char));

    reverse_string(X, X_rev);
    reverse_string(Y, Y_rev);

    space_efficient_alignment(X_rev, Y_rev, result);

    free(X_rev);
    free(Y_rev);
}