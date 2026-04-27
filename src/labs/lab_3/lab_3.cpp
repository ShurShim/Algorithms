#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <chrono>

#define EPS 0.0001       // Минимально допустимая точность поиска минимума
#define N 10000           // Количество повторов измерения времени для усреднения
#define BEG -1000        // Левая граница интервала поиска
#define END 1000         // Правая граница интервала поиска

using namespace std;

// Функция для записи данных в файл
// Параметры:
//  file - поток для записи
//  filename - имя файла
//  append - флаг для добавления к файлу (true) или перезаписи (false)
//  sep - разделитель между элементами
//  end - окончание строки (например, "\n")
//  args - переменное число аргументов для записи
template<typename ...T>
void writefile(ofstream& file,
    const char* filename,
    bool append,
    const string& sep,
    const string& end,
    T&& ...args) {
    // Если файл не открыт - открыть в нужном режиме
    if (!file.is_open())
        file.open(filename, append ? ios::out | ios::app : ios::out);
    // Запись аргументов с разделителем, используя инициализацию массива (fold expression)
    int dummy[sizeof...(T)] = { (file << args << sep, 0)... };
    file << end; // Запись окончания строки
    if (!append)
        file.close(); // Закрыть файл, если не требуется добавление
}

// Целевая функция для минимизации
double f(double x) {
    return 3 * x * x + 2 * x - 2;
}

// Метод дихотомии для поиска минимума функции на интервале [a, b]
// eps - желаемая точность
// Возвращает приблизительное значение аргумента минимума функции
double dichotomy(double eps, double a = BEG, double b = END) {
    double x1, x2;
    double f1, f2;
    // Пока длина интервала больше epsilon
    while (eps < b - a) {
        // Выбираем две точки близкие к середине интервала, с небольшим смещением 
        // Использован коэффициент 0.75 * eps, чтобы избежать равенства точек
        x1 = (a + b - 0.75 * eps) / 2;
        x2 = (a + b + 0.75 * eps) / 2;

        f1 = f(x1);
        f2 = f(x2);

        // Сравниваем значения функции и сужаем интервал
        if (f1 <= f2)
            b = x2;
        else
            a = x1;
    }
    // Возвращаем средину интервала как приближение минимума
    return (a + b) / 2;
}

// Метод перебора для поиска минимума функции на отрезке [a, b]
// eps - шаг перебора
// Возвращает минимальное значение функции (не аргумент)
double bruteForce(double eps, double a = BEG, double b = END) {
    double temp;
    long n = ceil((b - a) / eps);  // Количество шагов перебора
    double mn = f(a);               // Текущее минимальное значение функции

    // Перебираем значения функции с шагом eps
    for (long i = 0; i <= n; i++) {
        temp = f(a + i * eps);
        if (temp < mn) {
            mn = temp;  // Обновляем минимум
        }
    }
    return mn; // Возвращаем найденный минимум функции
}

int main() {
    ofstream file;

    // Массив для накопления суммарного времени двух методов (в наносекундах)
    // 0 — bruteForce, 1 — dichotomy
    chrono::nanoseconds avgTime[2] = { chrono::nanoseconds(0), chrono::nanoseconds(0) };

    // Время начала и конца замера интервала
    auto begin = chrono::system_clock::now();
    auto end = chrono::system_clock::now();

    // Открываем CSV файл для записи результатов (перезапись)
    file.open("optimization.csv", ios::out);

    // Записываем заголовки столбцов в файл
    writefile(file, "optimization.csv", false, ";", "\n", "Epsilon", "Brute Force", "Dichotomy", "Fibonacci");

    // Цикл по различным значениям epsilon, уменьшаем шаг от 1 до EPS в 2 раза на каждом шаге
    for (double i = 1; i >= EPS; i /= 2) {

        // Сброс времени для новых измерений
        avgTime[0] = chrono::nanoseconds(0);
        avgTime[1] = chrono::nanoseconds(0);

        // N повторов для получения средних значений времени работы алгоритмов
        for (int j = 0; j < N; j++) {
            // Замер времени работы метода перебора
            begin = chrono::system_clock::now();
            bruteForce(i);
            end = chrono::system_clock::now();
            avgTime[0] += chrono::duration_cast<chrono::nanoseconds>(end - begin);

            // Замер времени работы метода дихотомии
            begin = chrono::system_clock::now();
            dichotomy(i);
            end = chrono::system_clock::now();
            avgTime[1] += chrono::duration_cast<chrono::nanoseconds>(end - begin);
        }

        cout << "test" << endl; // Сообщение о прохождении очередного шага

        // Запись в файл: значение epsilon, среднее время bruteForce и dichotomy
        writefile(file, "optimization.csv", true, ";", "\n",
            i, avgTime[0].count() / N, avgTime[1].count() / N);
    }

    // Закрываем файл после записи всех данных
    file.close();

    cout << "Программа завершена" << endl;
    return 0;
}
