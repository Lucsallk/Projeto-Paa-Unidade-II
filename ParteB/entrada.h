#define MATCH 0
#define MISMATCH 1
#define GAP 2

int score(char a, char b) {
    return (a == b) ? MATCH : MISMATCH;
}