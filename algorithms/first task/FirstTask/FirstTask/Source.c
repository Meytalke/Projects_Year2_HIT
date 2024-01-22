#include <stdio.h>

#define N 6 
#define ROWS 5
#define COLS 5
int max(int a, int b) {
    return (a > b) ? a : b;
}
int MaxSum(int i, int j, int A[N][N]){
    if (i == 0) return A[i][j];
    if (j == 0) return A[i][j] + MaxSum(i - 1, j, A);
    if(i == N-1 && j== N-1) return A[i][j] + MaxSum(i-1, j-1, A);
    return A[i][j] + max(MaxSum(i - 1, j, A), MaxSum(i - 1, j - 1, A));
}
int MaxSum2(int i, int j, int A[N][N]) {
    if (i<0 || i>N-1 || j<0 || j>N-1)
        return 0;
    if (i == N - 1)
        return A[i][j];
    return A[i][j] + max(MaxSum2(i + 1, j, A), MaxSum2(i + 1, j + 1, A));
}

int checkPaths(int A[N][N]) {
    int maxSumValue = 0;
    for (int j = N - 1; j >= 0; j--) {
        int result = MaxSum(N - 1, j, A);
        if (result > maxSumValue) {
            maxSumValue = result;
        }
    }
    return maxSumValue;
}

int MaxSumDynamic(int A[N][N])
{
    //int* res= (int*)malloc(N * sizeof(int));
    int res[N];
    res[0] = A[0][0];
    for (int i = 1; i < N; i++) {
        int prev = res[0];  
        res[0] = A[i][0] + res[0];  
        for (int j = 1; j < i; j++) {
            int current = max(prev, res[j]);  
            prev = res[j]; 
            res[j] = A[i][j] + current;  
        }
        res[i] = A[i][i] + prev;  
    }
    int maxSum = 0;
    for (int i = 0; i < N; i++) {
        if (res[i] > maxSum)
            maxSum = res[i];
    }
    //free(res);
    return maxSum;
}
void findMaxPath(int A[N][N]) {
    /*int** S = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++)
        S[i] = (int*)malloc(N * sizeof(int));*/
    int S[N][N];
    for (int j = 0; j < N; j++)
        S[N - 1][j] = A[N - 1][j];

    int currentCol = 0;
    for (int i = N - 2; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (S[i + 1][j] > S[i + 1][j + 1]) {
                S[i][j] = A[i][j] + S[i + 1][j];
            }
            else {
                S[i][j] = A[i][j] + S[i + 1][j + 1];
            }
        }

        if (S[i][currentCol] < S[i][currentCol + 1]) {
            currentCol++;
        }
    }

    printf("Max Path: ");
    int i;
    printf("[0, 0] -> ");
    for (i = 1; i < N - 1; i++) {
        printf("[%d, %d] -> ", i, currentCol);
        if (currentCol < i + 1 && S[i + 1][currentCol] < S[i + 1][currentCol + 1]) {
            currentCol++;
        }
    }
    printf("[%d, %d]\n", i, currentCol);

    /*for (int i = 0; i < N; i++)
        free(S[i]);
    free(S);*/
}
int find_longest_snake_path(int A[ROWS][COLS], int i, int j) {
    if (i == 0 && j == 0)
        return 1;
    int up_length = 0, left_length = 0;
    if (i >= 0 && j >= 0 && abs(A[i][j] - A[i - 1][j]) > 1 && abs(A[i][j] - A[i][j - 1]) > 1)
        return 1;  
    if (i > 0 && abs(A[i][j] - A[i - 1][j]) <= 1)
        up_length = find_longest_snake_path(A, i - 1, j);
    if (j > 0 && abs(A[i][j] - A[i][j - 1]) <= 1)
        left_length = find_longest_snake_path(A, i, j - 1);
    return max(up_length, left_length) + 1;
}

int find_longest_snake_path_dynamic(int A[N][N]) {
    /*int* path = (int*)malloc(N * sizeof(int));*/
    int path[N],max_length = 0,temp = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 && j > 0) {
                if (abs(A[i][j] - A[i][j - 1]) <= 1)
                    temp = path[j - 1] + 1;
                else
                    temp = 1;
            }
            else if (j == 0 && i > 0) {
                if (abs(A[i][j] - A[i - 1][j]) <= 1)
                    temp = path[j] + 1;
                else
                    temp = 1;
            }
            else if (i > 0 && j > 0 && abs(A[i][j] - A[i - 1][j]) <= 1 && abs(A[i][j] - A[i][j - 1]) <= 1)
                temp = max(path[j], temp) + 1;
            else if (i > 0 && abs(A[i][j] - A[i - 1][j]) <= 1)
                temp = path[j] + 1;
            else if (j > 0 && abs(A[i][j] - A[i][j - 1]) <= 1)
                temp = temp + 1;
            else
                temp = 1;
            path[j] = temp;
            if (path[j] > max_length)
                max_length = path[j];
        }
    }
    //free(path);
    return max_length;
}

// A function that returns the length of the longest common subsequence of three sequences
int LCS(char* X, char* Y, char* Z, int i, int j, int k) {
    // Base case: if any of the indices is zero, return zero
    if (i == 0 || j == 0 || k == 0) {
        return 0;
    }
    // If the last characters of all sequences are equal, add one to the length and recurse on the previous characters
    if (X[i - 1] == Y[j - 1] && Y[j - 1] == Z[k - 1]) {
        return LCS(X, Y, Z, i - 1, j - 1, k - 1) + 1;
    }
    // Otherwise, find the maximum length by ignoring the last character of one of the sequences
    int a = LCS(X, Y, Z, i - 1, j, k); // ignore X[i-1]
    int b = LCS(X, Y, Z, i, j - 1, k); // ignore Y[j-1]
    int c = LCS(X, Y, Z, i, j, k - 1); // ignore Z[k-1]
    return max(a, max(b, c)); // return the maximum of the three
}

// A test function that prints the length of the longest common subsequence of three sequences
void test(char* X, char* Y, char* Z,char t) 
{
    if (t == 'R')
    {
        int n = strlen(X); // assume all sequences have the same length
        int length = LCS(X, Y, Z, n, n, n); // call the LCS function
        printf("REC- The length of the longest common subsequence of %s, %s, and %s is %d\n", X, Y, Z, length); // print the result
    }
    else
    {
        int n = strlen(X); // assume all sequences have the same length
        int length = lcs3(X, Y, Z,n,n,n); // call the LCS function
        printf("DYN - The length of the longest common subsequence of %s, %s, and %s is %d\n", X, Y, Z, length); // print the result
    }
}

int lcs3(char* X, char* Y, char* Z) {
    int n = max(strlen(X), max(strlen(Y), strlen(Z)));
    
    int** L = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; ++i)
        L[i] = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= n; k++) {
                if (i == 0 || j == 0 || k == 0)
                    L[j][k] = 0;
                else if (X[i - 1] == Y[j - 1] && Y[j - 1] == Z[k - 1])
                    L[j][k] = L[j - 1][k - 1] + 1;
                else
                    L[j][k] = max(L[j - 1][k], max(L[j][k - 1], L[j][k]));
            }
        }
    }
    int num = L[n][n];
    for (int i = 0; i <= n; ++i) {
        free(L[i]);
    }
    free(L);
    return num;
}
int main() {
    int A[N][N] = 
    {
     {4, 0, 0, 0, 0, 0},
    {22, 14, 0, 0, 0, 0},
    {12, 18, 7, 0, 0, 0},
    {16, 28, 9, 35, 0, 0},
    {55, 60, 40, 75, 20, 0},
    {85, 50, 35, 60, 70, 45}
    };
    printf("A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n\n\n");
    int res = checkPaths(A);
    printf("\nBest Result REC1: %d\n", res);
    int j = MaxSum2(0, 0, A);
    printf("\nBest Result REC2: %d\n", j);
    int res1[N][N] = { 0 };
    int res3= MaxSumDynamic(A);
    printf("\ndynamic - Best Result : %d\n", res3);
    findMaxPath(A);
    int B[ROWS][COLS] = {
       {1, 10, 2, 40, 50},
       {2, 2, 1, 20, 60},
       {40, 50, 2, 90, 50},
       {50, 60, 3, 2, 100},
       {30, 20, 10, 7, 6}
    };
    int longest_snake_length = find_longest_snake_path(B, 3, 3);
    printf("Length of the longest snake path: %d\n", longest_snake_length);
    int longest_snake_lengthdynamic = find_longest_snake_path_dynamic(B);
    printf("dynamic-Length of the longest snake path: %d\n", longest_snake_lengthdynamic);
    test("ABC", "BCD", "CDE",'R'); // output: 1
    test("ABC", "BCD", "CDE", 'D'); // output: 1
    test("ABCD", "ABCD", "ABCD", 'R'); // output: 4
    test("ABCD", "ABCD", "ABCD", 'D'); // output: 4
    test("XYZ", "PQR", "LMN", 'R'); // output: 0
    test("XYZ", "PQR", "LMN", 'D'); // output: 0
    test("ABCBDAB", "BDCABA", "BADACB", 'R'); // output: 4
    test("ABCBDAB", "BDCABAX", "BADACBJ", 'D'); // output: 4

    test("AGGTBAQ", "GXTXAYB", "GTBPLM", 'R'); // output: 3
    test("AGGTBAQ", "GXTXAYB", "GTBPLM", 'D'); // output: 3

    test("ABCBD", "BDCAB", "BADCB", 'R'); // output: 3
    test("ABCBD", "BDCAB", "BADCB", 'D'); // output: 3

    test("ARCRAA", "AEBCAA", "AABXAC", 'R'); // output: 3
    test("ABCBD", "BDCAB", "BADCB", 'D'); // output: 3
    return 0;
}
