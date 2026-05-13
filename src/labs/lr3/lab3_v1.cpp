#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <chrono>

#define EPS 0.0001      // Минимально допустимая точность поиска минимума
#define N 10000         // Количество повторов измерения времени для усреднения
#define BEG -1000       // Левая граница интервала поиска
#define END 1000        // Правая граница интервала поиска

using namespace std;

// Универсальная функция записи данных в файл
template<typename... T>
void writefile(ofstream& file, const char* filename, bool append,
               const string& sep, const string& end, T&&... args)
{
    if (!file.is_open()) {
        file.open(filename, append ? ios::out | ios::app : ios::out);
    }

    int dummy[sizeof...(T)] = { (file << args << sep, 0)... };
    file << end;

    if (!append) {
        file.close();
    }
}

// Целевая функция для минимизации: f(x) = 3x² + 2x - 2
double f(double x)
{
    return 3 * x * x + 2 * x - 2;
}

// Метод дихотомии для поиска минимума на [a, b]
double dichotomy(double eps, double a = BEG, double b = END)
{
    double x1, x2, f1, f2;

    while (eps < b - a) {
        x1 = (a + b - 0.75 * eps) / 2;
        x2 = (a + b + 0.75 * eps) / 2;

        f1 = f(x1);
        f2 = f(x2);

        if (f1 <= f2) {
            b = x2;
        } else {
            a = x1;
        }
    }

    return (a + b) / 2;
}

// Метод полного перебора
double bruteForce(double eps, double a = BEG, double b = END)
{
    double temp, mn = f(a);
    long n = ceil((b - a) / eps);

    for (long i = 0; i <= n; i++) {
        temp = f(a + i * eps);
        if (temp < mn) {
            mn = temp;
        }
    }

    return mn;
}

// Вывод красивой таблицы результатов
void printTableHeader()
{
    cout << setw(12) << left << "Epsilon"
         << setw(18) << "Brute Force (ns)"
         << setw(18) << "Dichotomy (ns)"
         << setw(12) << "Ускорение"
         << endl;
    cout << string(60, '-') << endl;
}

void printTableRow(double eps, double bf_time, double dich_time)
{
    double speedup = bf_time / dich_time;
    cout << fixed << setprecision(6);
    cout << setw(12) << left << eps
         << setw(18) << bf_time
         << setw(18) << dich_time
         << setw(12) << speedup << "x"
         << endl;
}

int main()
{
    ofstream file;
    chrono::nanoseconds avgTime[2] = { chrono::nanoseconds(0), chrono::nanoseconds(0) };

    // Создание CSV с заголовками
    writefile(file, "optimization.csv", false, ";", "\n",
              "Epsilon", "Brute Force", "Dichotomy", "Fibonacci");

    // Заголовок таблицы в консоли
    printTableHeader();

    // Тестирование при разных epsilon
    for (double i = 1; i >= EPS; i /= 2) {
        avgTime[0] = chrono::nanoseconds(0);
        avgTime[1] = chrono::nanoseconds(0);

        auto begin = chrono::system_clock::now();
        auto end = chrono::system_clock::now();

        // N повторов для усреднения
        for (int j = 0; j < N; j++) {
            // Brute Force
            begin = chrono::system_clock::now();
            bruteForce(i);
            end = chrono::system_clock::now();
            avgTime[0] += chrono::duration_cast<chrono::nanoseconds>(end - begin);

            // Dichotomy
            begin = chrono::system_clock::now();
            dichotomy(i);
            end = chrono::system_clock::now();
            avgTime[1] += chrono::duration_cast<chrono::nanoseconds>(end - begin);
        }

        // Средние времена
        double bf_avg = avgTime[0].count() / N;
        double dich_avg = avgTime[1].count() / N;

        // Вывод строки таблицы
        printTableRow(i, bf_avg, dich_avg);

        // Запись в CSV
        writefile(file, "optimization.csv", true, ";", "\n",
                  i, bf_avg, dich_avg);
    }

    file.close();
    cout << string(60, '-') << endl;
    cout << "Результаты также сохранены в optimization.csv" << endl;

    return 0;
}
