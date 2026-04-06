#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TESTS 10000
#define START_SIZE 100
#define END_SIZE 2000
#define STEP 100

int randomNumber(int min, int max);
char randomChar();
char* generateString(int n);
int basicSearch(const char* s, const char* sub);
int* computeLPS(const char* pattern, int m);
int KMPSearch(const char* s, const char* sub);
int* computeBadCharHeuristic(const char* sub, int m);
int boyerMooreSearch(const char* s, const char* sub);

int main() {
    srand((unsigned int)time(NULL));

    FILE* outBasic = fopen("output/basic_search.txt", "w");
    FILE* outKMP = fopen("output/kmp_search.txt", "w");
    FILE* outBM = fopen("output/boyer_moore_search.txt", "w");
    FILE* outSizes = fopen("output/sizes.txt", "w");

    if (outBasic == NULL || outKMP == NULL || outBM == NULL || outSizes == NULL) {
        printf("Error opening files!\n");
        return 1;
    }

    printf("%-15s | %-10s | %-8s | %-15s\n",
           "String Length", "Basic(ms)", "KMP(ms)", "Boyer-Moore(ms)");
    printf("---------------|------------|----------|-----------------\n");

    for (int size = START_SIZE; size <= END_SIZE; size += STEP) {
        double totalBasicTime = 0;
        double totalKMPTime = 0;
        double totalBoyerMooreTime = 0;

        for (int test = 0; test < NUM_TESTS; test++) {
            char* s = generateString(size);
            char* sub = generateString(randomNumber(10, 15));

            if (s == NULL || sub == NULL) {
                printf("Memory allocation failed!\n");
                if (s) free(s);
                if (sub) free(sub);
                fclose(outBasic);
                fclose(outKMP);
                fclose(outBM);
                fclose(outSizes);
                return 1;
            }

            clock_t start = clock();
            basicSearch(s, sub);
            clock_t end = clock();
            if (start != (clock_t)-1 && end != (clock_t)-1) {
                totalBasicTime += (double)(end - start) / CLOCKS_PER_SEC * 1000000.0;
            }

            start = clock();
            KMPSearch(s, sub);
            end = clock();
            if (start != (clock_t)-1 && end != (clock_t)-1) {
                totalKMPTime += (double)(end - start) / CLOCKS_PER_SEC * 1000000.0;
            }

            start = clock();
            boyerMooreSearch(s, sub);
            end = clock();
            if (start != (clock_t)-1 && end != (clock_t)-1) {
                totalBoyerMooreTime += (double)(end - start) / CLOCKS_PER_SEC * 1000000.0;
            }

            free(s);
            free(sub);
        }

        double avgBasic = totalBasicTime / NUM_TESTS / 1000.0;
        double avgKMP = totalKMPTime / NUM_TESTS / 1000.0;
        double avgBoyerMoore = totalBoyerMooreTime / NUM_TESTS / 1000.0;

        printf("%-15d | %-10.3f | %-8.3f | %-15.3f\n",
               size, avgBasic, avgKMP, avgBoyerMoore);

        fprintf(outSizes, "%d\n", size);
        fprintf(outBasic, "%.6f\n", avgBasic);
        fprintf(outKMP, "%.6f\n", avgKMP);
        fprintf(outBM, "%.6f\n", avgBoyerMoore);
    }

    fclose(outBasic);
    fclose(outKMP);
    fclose(outBM);
    fclose(outSizes);

    printf("\nResults saved to output/ directory\n");
    printf("Files created:\n");
    printf("  - output/sizes.txt (x-axis data)\n");
    printf("  - output/basic_search.txt (y1 - Basic Search)\n");
    printf("  - output/kmp_search.txt (y2 - KMP Search)\n");
    printf("  - output/boyer_moore_search.txt (y3 - Boyer-Moore Search)\n");

    return 0;
}

int randomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

char randomChar() {
    return (char)randomNumber((int)'a', (int)'z');
}

char* generateString(int n) {
    char* temp = (char*)malloc((n + 1) * sizeof(char));
    if (temp == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        temp[i] = randomChar();
    }
    temp[n] = '\0';
    return temp;
}

int basicSearch(const char* s, const char* sub) {
    int n = strlen(s);
    int m = strlen(sub);

    if (m == 0) return 0;
    if (n < m) return -1;

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (s[i + j] != sub[j]) break;
        }
        if (j == m) return i;
    }
    return -1;
}

int* computeLPS(const char* pattern, int m) {
    int* lps = (int*)calloc(m, sizeof(int));
    if (lps == NULL) return NULL;

    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int KMPSearch(const char* s, const char* sub) {
    int n = strlen(s);
    int m = strlen(sub);

    if (m == 0) return 0;
    if (n < m) return -1;

    int* lps = computeLPS(sub, m);
    if (lps == NULL) return -1;

    int i = 0;
    int j = 0;

    while (i < n) {
        if (sub[j] == s[i]) {
            i++;
            j++;
        }

        if (j == m) {
            free(lps);
            return i - j;
        } else if (i < n && sub[j] != s[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return -1;
}

int* computeBadCharHeuristic(const char* sub, int m) {
    int* badChar = (int*)malloc(256 * sizeof(int));
    if (badChar == NULL) return NULL;

    for (int i = 0; i < 256; i++) {
        badChar[i] = -1;
    }

    for (int i = 0; i < m; i++) {
        badChar[(unsigned char)sub[i]] = i;
    }

    return badChar;
}

int boyerMooreSearch(const char* s, const char* sub) {
    int n = strlen(s);
    int m = strlen(sub);

    if (m == 0) return 0;
    if (n < m) return -1;

    int* badChar = computeBadCharHeuristic(sub, m);
    if (badChar == NULL) return -1;

    int shift = 0;

    while (shift <= n - m) {
        int j = m - 1;

        while (j >= 0 && sub[j] == s[shift + j]) {
            j--;
        }

        if (j < 0) {
            free(badChar);
            return shift;
        } else {
            int move = j - badChar[(unsigned char)s[shift + j]];
            if (move < 1) move = 1;
            shift += move;
        }
    }

    free(badChar);
    return -1;
}
