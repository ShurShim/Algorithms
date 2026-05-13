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

// Универсальная функция записи данных в файл (одна строка)
template<typename... T>
void writefile(ofstream& file, const string& sep, const string& end, T&&... args)
{
    int dummy[sizeof...(T)] = { (file << args << sep, 0)... };
    file << end;
}

// Целевая функция: f(x) = 3x² + 2x - 2
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

// Вывод красивой таблицы в консоль
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
    // Открываем четыре текстовых файла для записи
    ofstream bfFile("src/data/brute.txt");
    ofstream dichFile("src/data/dichotomy.txt");
    ofstream speedFile("src/data/speedup.txt");
    ofstream epsFile("src/data/sizes.txt");

    if (!bfFile.is_open() || !dichFile.is_open() || !speedFile.is_open() || !epsFile.is_open()) {
        cerr << "Ошибка открытия файлов!" << endl;
        return 1;
    }


    // Заголовок таблицы в консоли
    printTableHeader();

    // Тестирование при разных epsilon (от 1 до EPS, каждый раз деля на 2)
    for (double eps = 1; eps >= EPS; eps /= 2) {
        chrono::nanoseconds totalBF(0), totalDich(0);

        // N повторов для усреднения
        for (int j = 0; j < N; ++j) {
            // Измеряем время полного перебора
            auto begin = chrono::system_clock::now();
            bruteForce(eps);
            auto end = chrono::system_clock::now();
            totalBF += chrono::duration_cast<chrono::nanoseconds>(end - begin);

            // Измеряем время дихотомии
            begin = chrono::system_clock::now();
            dichotomy(eps);
            end = chrono::system_clock::now();
            totalDich += chrono::duration_cast<chrono::nanoseconds>(end - begin);
        }

        // Средние времена
        double bf_avg = totalBF.count() / (double)N;
        double dich_avg = totalDich.count() / (double)N;
        double speedup = bf_avg / dich_avg;

        // Вывод в консоль
        printTableRow(eps, bf_avg, dich_avg);

        // Запись в файлы
        writefile(bfFile, "\t", "\n", bf_avg);
        writefile(dichFile, "\t", "\n", dich_avg);
        writefile(speedFile, "\t", "\n", speedup);
        writefile(epsFile, "\n", "\n", eps);
    }

    // Закрываем файлы
    bfFile.close();
    dichFile.close();
    speedFile.close();
    epsFile.close();

    cout << string(60, '-') << endl;
    cout << "Результаты сохранены в файлы: brute.txt, dichotomy.txt, speedup.txt" << endl;

    return 0;
}
