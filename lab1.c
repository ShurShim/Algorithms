#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void shakerSort(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    int i, temp;

    while (left <= right) {
        for (i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
        right--;

        for (i = right; i > left; i--) {
            if (arr[i - 1] > arr[i]) {
                temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
            }
        }
        left++;
    }
}

void fillRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

void copyArray(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

int isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    #ifdef _WIN32
        system("chcp 1251 > nul");
    #endif

    srand(time(NULL));

    int sizes[] = {900, 3000, 8500};
    int numSizes = 3;
    const int NUM_REPEATS = 1000; // Число повторений для точности (как NN в методичке)

    printf("===========================================================\n");
    printf("Лабораторная работа №1: Методы сортировки\n");
    printf("Вариант 21: Шейкерная сортировка\n");
    printf("===========================================================\n\n");


    for (int idx = 0; idx < numSizes; idx++) {
        int n = sizes[idx];

        int* original = (int*)malloc(n * sizeof(int));
        int* work = (int*)malloc(n * sizeof(int));

        fillRandom(original, n);

        clock_t t1, t2;
        double copy_time = 0.0, sort_time = 0.0;

        t1 = clock();
        for (int repeat = 0; repeat < NUM_REPEATS; repeat++) {
            copyArray(original, work, n);
        }
        t2 = clock();
        copy_time = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

        // Замер времени сортировки (с учетом копирования)
        t1 = clock();
        for (int repeat = 0; repeat < NUM_REPEATS; repeat++) {
            copyArray(original, work, n);
            shakerSort(work, n);
        }
        t2 = clock();
        sort_time = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

        // Вычитаем время копирования
        double pure_sort_time = (sort_time - copy_time) / NUM_REPEATS;

        // Проверка корректности последней сортировки
        if (!isSorted(work, n)) {
            printf("Ошибка сортировки!\n");
        }

        printf("Размер массива: %d\n", n);
        printf("  Общее время %d сортировок: %.6f с\n", NUM_REPEATS, sort_time);
        printf("  Время копирования: %.6f с\n", copy_time);
        printf("  Чистое время одной сортировки: %.10f с\n", pure_sort_time);
        printf("  Теоретическая O(n^2): ~%.6f с (относительно n=%d)\n\n",
               pure_sort_time * pow(n/900.0, 2), n);

        free(original);
        free(work);
    }

    return 0;
}
