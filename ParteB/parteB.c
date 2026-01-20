#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "divide_conquer.h"

int main() {
    const char *X = "GATTACA";
    const char *Y = "GCATGCU";

    char *AX, *AY;

    int m = strlen(Y);
    int *forward = malloc((m + 1) * sizeof(int));
    int *backward = malloc((m + 1) * sizeof(int));

    space_efficient_alignment(X, Y, forward);
    backward_space_efficient_alignment(X, Y, backward);

    printf("Forward last column:\n");
    for (int j = 0; j <= m; j++) {
        printf("%d ", forward[j]);
    }
    printf("\n");

    printf("Backward last column:\n");
    for (int j = 0; j <= m; j++) {
        printf("%d ", backward[j]);
    }
    printf("\n");

    //divide_and_conquer_alignment(X, Y, &AX, &AY);

    free(forward);
    free(backward);

    return 0;
}