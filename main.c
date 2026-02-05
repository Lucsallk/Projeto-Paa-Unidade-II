#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

constexpr int MATCH = 0;
constexpr int MISMATCH = 1;
constexpr int GAP = 2;

std::string alignedX, alignedY;

int score(char a, char b) {
    return (a == b) ? MATCH : MISMATCH;
}

int min3(int a, int b, int c) {
    return std::min({a, b, c});
}

std::vector<int> space_efficient_alignment(const std::string& X, const std::string& Y) {
    int n = (int)X.size();
    int m = (int)Y.size();

    std::vector<int> col(n + 1);
    for (int i = 0; i <= n; i++) col[i] = i * GAP;

    for (int j = 1; j <= m; j++) {
        int diag = col[0];
        col[0] = j * GAP;

        for (int i = 1; i <= n; i++) {
            int tmp = col[i];
            col[i] = min3(
                col[i - 1] + GAP,
                col[i] + GAP,
                diag + score(X[i - 1], Y[j - 1])
            );
            diag = tmp;
        }
    }

    return col;
}

void needleman_wunsch(const std::string& X, const std::string& Y) {
    int n = (int)X.size();
    int m = (int)Y.size();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1));

    for (int i = 0; i <= n; i++) dp[i][0] = i * GAP;
    for (int j = 0; j <= m; j++) dp[0][j] = j * GAP;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = min3(
                dp[i - 1][j] + GAP,
                dp[i][j - 1] + GAP,
                dp[i - 1][j - 1] + score(X[i - 1], Y[j - 1])
            );
        }
    }

    int i = n, j = m;
    std::string tempX, tempY;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + score(X[i - 1], Y[j - 1])) {
            tempX += X[i - 1];
            tempY += Y[j - 1];
            i--; j--;
        } else if (i > 0 && dp[i][j] == dp[i - 1][j] + GAP) {
            tempX += X[i - 1];
            tempY += '-';
            i--;
        } else {
            tempX += '-';
            tempY += Y[j - 1];
            j--;
        }
    }

    std::reverse(tempX.begin(), tempX.end());
    std::reverse(tempY.begin(), tempY.end());
    alignedX += tempX;
    alignedY += tempY;
}

void divide_and_conquer(const std::string& X, const std::string& Y) {
    int n = (int)X.size();
    int m = (int)Y.size();

    if (n == 0) {
        alignedX += std::string(m, '-');
        alignedY += Y;
        return;
    }

    if (m == 0) {
        alignedX += X;
        alignedY += std::string(n, '-');
        return;
    }

    if (n == 1 || m == 1) {
        needleman_wunsch(X, Y);
        return;
    }

    int mid = m / 2;

    std::vector<int> L = space_efficient_alignment(X, Y.substr(0, mid));

    std::string Xr = X;
    std::string Yr = Y.substr(mid);
    std::reverse(Xr.begin(), Xr.end());
    std::reverse(Yr.begin(), Yr.end());
    std::vector<int> R = space_efficient_alignment(Xr, Yr);

    int cut = 0;
    int best = INT_MAX;

    for (int i = 0; i <= n; i++) {
        int val = L[i] + R[n - i];
        if (val < best) {
            best = val;
            cut = i;
        }
    }

    divide_and_conquer(X.substr(0, cut), Y.substr(0, mid));
    divide_and_conquer(X.substr(cut), Y.substr(mid));
}

int compute_cost(const std::string& A, const std::string& B) {
    int cost = 0;
    for (size_t i = 0; i < A.size(); i++) {
        if (A[i] == '-' || B[i] == '-') cost += GAP;
        else cost += score(A[i], B[i]);
    }
    return cost;
}

int main() {
    std::string X, Y;
    std::getline(std::cin, X);
    std::getline(std::cin, Y);

    alignedX.clear();
    alignedY.clear();

    divide_and_conquer(X, Y);

    int total_cost = compute_cost(alignedX, alignedY);

    std::cout << total_cost << '\n';
    std::cout << alignedX << '\n';
    std::cout << alignedY << '\n';

    return 0;
}
