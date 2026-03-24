#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Сортировка "Пузырьком"
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
}

void merge(int a[], int lb, int split, int ub){
    int pos1 = lb;          // указатель на начало левой половины
    int pos2 = split + 1;   // указатель на начало правой половины
    int pos3 = 0;           // указатель во временном массиве
    int *temp = (int *) malloc((ub - lb + 1) * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }

    // Слияние: сравниваем элементы и кладём меньший во временный массив
    while (pos1 <= split && pos2 <= ub) {
        if (a[pos1] < a[pos2])
            temp[pos3++] = a[pos1++];
        else
            temp[pos3++] = a[pos2++];
    }

    // Копируем остатки из правой половины (если остались)
    while (pos2 <= ub)
        temp[pos3++] = a[pos2++];

    // Копируем остатки из левой половины (если остались)
    while (pos1 <= split)
        temp[pos3++] = a[pos1++];

    // Возвращаем элементы из временного массива обратно в исходный
    for (pos3 = 0; pos3 < ub - lb + 1; pos3++)
        a[lb + pos3] = temp[pos3];

    free(temp);  // освобождаем память
}

// Сортировка слиянием
void mergeSort(int a[], int lb, int ub){
    int split;
    if (lb < ub) {
        split = (lb + ub) / 2;          // находим середину
        mergeSort(a, lb, split);        // сортируем левую половину
        mergeSort(a, split + 1, ub);    // сортируем правую половину
        merge(a, lb, split, ub);        // сливаем две отсортированные половины
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    int temp;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

// Быстрая сортировка
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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


// Лабораторная работа №1: Методы сортировки
// Вариант 21: Шейкерная сортировка
int main() {
    #ifdef _WIN32
        system("chcp 1251 > nul");
    #endif

    srand(time(NULL));

    int sizes[] = {
        100, 200, 300, 400, 500,
        600, 700, 800, 900, 1000,
        1100, 1200, 1300, 1400, 1500,
        1600, 1700, 1800, 1900, 2000
    };
    int count = sizeof(sizes) / sizeof(sizes[0]);
    const int NUM_REPEATS = 10000;

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
