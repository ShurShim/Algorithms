#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int n);
void mergeSort(int a[], int lb, int ub);
void quickSort(int arr[], int low, int high);
void fillRandom(int arr[], int n);
void copyArray(int source[], int dest[], int n);
void saveIntArrayToFile(int arr[], int size, const char *filename);
void saveDoubleArrayToFile(double arr[], int size, const char *filename);

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
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    const int NUM_REPEATS = 10000;
    double resultArray[3][numSizes];   // теперь double и индексация по idx

    for (int idx = 0; idx < numSizes; idx++) {
        int n = sizes[idx];
        printf("Размер массива: %d 🏁\n", sizes[idx]);

        int* original = (int*) malloc(n * sizeof(int));
        int* work1 = (int*) malloc(n * sizeof(int));
        int* work2 = (int*) malloc(n * sizeof(int));
        int* work3 = (int*) malloc(n * sizeof(int));

        if (!original || !work1 || !work2 || !work3) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(1);
        }

        fillRandom(original, n);
        copyArray(original, work1, n);
        copyArray(original, work2, n);
        copyArray(original, work3, n);

        double time_bubble = 0.0, time_merge = 0.0, time_quick = 0.0;

        for (int repeat = 0; repeat < NUM_REPEATS; repeat++) {
            copyArray(original, work1, n);
            copyArray(original, work2, n);
            copyArray(original, work3, n);

            clock_t start, end;

            // Bubble sort
            start = clock();
            bubbleSort(work1, n);
            end = clock();
            time_bubble += ((double)(end - start)) / CLOCKS_PER_SEC;

            // Merge sort
            start = clock();
            mergeSort(work2, 0, n - 1);
            end = clock();
            time_merge += ((double)(end - start)) / CLOCKS_PER_SEC;

            // Quick sort
            start = clock();
            quickSort(work3, 0, n - 1);
            end = clock();
            time_quick += ((double)(end - start)) / CLOCKS_PER_SEC;
        }

        resultArray[0][idx] = time_bubble / NUM_REPEATS;
        resultArray[1][idx] = time_merge / NUM_REPEATS;
        resultArray[2][idx] = time_quick / NUM_REPEATS;

        free(original);
        free(work1);
        free(work2);
        free(work3);
    }

    // Сохраняем результаты в файлы
    saveIntArrayToFile(sizes, numSizes, "output/sizes.txt");
    saveDoubleArrayToFile(resultArray[0], numSizes, "output/bubble_sort.txt");
    saveDoubleArrayToFile(resultArray[1], numSizes, "output/merge_sort.txt");
    saveDoubleArrayToFile(resultArray[2], numSizes, "output/quick_sort.txt");

    return 0;
}

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
}

void merge(int a[], int lb, int split, int ub) {
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

// Сортировка слиянием.
// a - массив | lb - левая граница | ub - правая граница
void mergeSort(int a[], int lb, int ub) {
    int split;
    if (lb < ub) {
        split = (lb + ub) / 2;          // находим середину
        mergeSort(a, lb, split);        // сортируем левую половину
        mergeSort(a, split + 1, ub);    // сортируем правую половину
        merge(a, lb, split, ub);        // сливаем две отсортированные половины
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];      // опорный элемент (берём последний)
    int i = (low - 1);          // индекс для элементов меньше pivot
    int temp;

    // Проходим по всем элементам, кроме опорного
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {   // если текущий элемент меньше опорного
            i++;                // расширяем границу "меньших" элементов
            // меняем местами arr[i] и arr[j]
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Ставим опорный элемент на правильное место (после всех меньших)
    temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);             // возвращаем индекс опорного элемента
}

// Быстрая сортировка
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // делим массив
        quickSort(arr, low, pi - 1);         // сортируем левую часть
        quickSort(arr, pi + 1, high);        // сортируем правую часть
    }
}

// Заполняет массив arr, длинной n значениями rand() % 10000
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

// Сохранение целочисленного массива в файл
void saveIntArrayToFile(int arr[], int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", arr[i]);
    }

    fclose(file);
    printf("Массив сохранён в %s\n", filename);
}

// Сохранение вещественного массива в файл
void saveDoubleArrayToFile(double arr[], int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%f\n", arr[i]);
    }

    fclose(file);
    printf("Массив сохранён в %s\n", filename);
}
