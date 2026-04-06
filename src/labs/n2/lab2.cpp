#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

using namespace std;

#define REPEAT_COUNT 10000

// "Тяжелый" Basic Search - делает максимальное количество сравнений
vector<int> full_numerate_search_all(const string& source, const string& pattern) {
    vector<int> occurrences;
    if (pattern.length() > source.length()) return occurrences;

    for (int i = 0; i <= source.length() - pattern.length(); i++) {
        bool found = true;
        // Принудительно проверяем все символы, даже после несовпадения
        for (int j = 0; j < pattern.length(); j++) {
            if (source[i + j] != pattern[j]) {
                found = false;
                // НЕ делаем break, продолжаем проверку (замедляет)
            }
        }
        if (found) {
            occurrences.push_back(i);
        }
    }
    return occurrences;
}

// Алгоритм Кнута-Морриса-Пратта
vector<int> computeLPSArray(const string& pattern) {
    int len = 0;
    vector<int> lps(pattern.length(), 0);
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else {
            if (len != 0) {
                len = lps[len - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

vector<int> kmp_search_all(const string& source, const string& pattern) {
    vector<int> occurrences;
    if (pattern.length() > source.length()) return occurrences;

    vector<int> lps = computeLPSArray(pattern);
    int i = 0;
    int j = 0;

    while (i < source.length()) {
        if (pattern[j] == source[i]) {
            i++;
            j++;
        }

        if (j == pattern.length()) {
            occurrences.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < source.length() && pattern[j] != source[i]) {
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i++;
            }
        }
    }
    return occurrences;
}

// Boyer-Moore алгоритм (хеш плохого символа)
int* computeBadCharHeuristic(const string& sub, int m) {
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

// Поиск всех вхождений (а не первого)
vector<int> boyer_moore_search_all(const string& source, const string& pattern) {
    vector<int> occurrences;
    int n = source.length();
    int m = pattern.length();

    if (m == 0) return occurrences;
    if (n < m) return occurrences;

    int* badChar = computeBadCharHeuristic(pattern, m);
    if (badChar == NULL) return occurrences;

    int shift = 0;

    while (shift <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == source[shift + j]) {
            j--;
        }

        if (j < 0) {
            occurrences.push_back(shift);
            // Сдвигаемся на 1 (или можно использовать правило хорошего суффикса)
            shift++;
        } else {
            int move = j - badChar[(unsigned char)source[shift + j]];
            if (move < 1) move = 1;
            shift += move;
        }
    }

    free(badChar);
    return occurrences;
}

// Генерация случайного символа из алфавита из 7 букв (a-g)
char getRandomChar7() {
    return char('a' + rand() % 7);
}

// Генерация строки с случайными символами (без гарантированных вхождений)
string generateString(int n, const string& key) {
    string result;

    // Генерируем полностью случайную строку
    for (int i = 0; i < n; i++) {
        result += getRandomChar7();
    }

    // Добавляем ключ в несколько случайных позиций
    int numKeys = max(1, n / 100); // 1% вхождений ключа
    for (int i = 0; i < numKeys; i++) {
        int pos = rand() % (n - key.length());
        for (int j = 0; j < key.length(); j++) {
            result[pos + j] = key[j];
        }
    }

    return result;
}

// Функция тестирования с сохранением в файлы
void testAlgosWithFileOutput(int n, const string& key, ofstream& outSizes, ofstream& outBasic, ofstream& outKMP, ofstream& outBM) {
    double t1_sum = 0, t2_sum = 0, t3_sum = 0;
    long long t1_count = 0, t2_count = 0, t3_count = 0;

    for (int testIteration = 0; testIteration < REPEAT_COUNT; testIteration++) {
        string source = generateString(n, key);

        clock_t start, end;

        // Basic Search
        start = clock();
        vector<int> result1 = full_numerate_search_all(source, key);
        end = clock();
        if (start != (clock_t)-1 && end != (clock_t)-1) {
            t1_sum += (end - start);
            t1_count++;
        }

        // KMP
        start = clock();
        vector<int> result2 = kmp_search_all(source, key);
        end = clock();
        if (start != (clock_t)-1 && end != (clock_t)-1) {
            t2_sum += (end - start);
            t2_count++;
        }

        // Boyer-Moore
        start = clock();
        vector<int> result3 = boyer_moore_search_all(source, key);
        end = clock();
        if (start != (clock_t)-1 && end != (clock_t)-1) {
            t3_sum += (end - start);
            t3_count++;
        }

        // Проверка корректности (каждые 100 итераций)
        if (testIteration % 100 == 0 && (result1.size() != result2.size() || result1.size() != result3.size())) {
            cout << "Warning: Mismatch in occurrences count at n=" << n
                 << " (BF:" << result1.size()
                 << ", KMP:" << result2.size()
                 << ", BM:" << result3.size() << ")" << endl;
        }
    }

    // Вычисляем среднее время в миллисекундах
    double t1_sec = (t1_sum / CLOCKS_PER_SEC) * 1000.0 / t1_count;
    double t2_sec = (t2_sum / CLOCKS_PER_SEC) * 1000.0 / t2_count;
    double t3_sec = (t3_sum / CLOCKS_PER_SEC) * 1000.0 / t3_count;

    // Выводим в консоль
    cout << "| " << setw(10) << n
         << " | " << setw(15) << fixed << setprecision(6) << t1_sec
         << " | " << setw(15) << t2_sec
         << " | " << setw(15) << t3_sec << " |" << endl;

    // Сохраняем в файлы
    outSizes << n << "\n";
    outBasic << t1_sec << "\n";
    outKMP << t2_sec << "\n";
    outBM << t3_sec << "\n";
}

int main() {
    srand(time(0));

    // Создаем директорию для данных (если её нет)
    system("mkdir -p src/data");

    // Открываем файлы для записи
    ofstream outSizes("src/data/sizes.txt");
    ofstream outBasic("src/data/basic_search.txt");
    ofstream outKMP("src/data/kmp.txt");
    ofstream outBM("src/data/boyer_moore.txt");

    if (!outSizes.is_open() || !outBasic.is_open() || !outKMP.is_open() || !outBM.is_open()) {
        cout << "Error opening files for writing!" << endl;
        cout << "Make sure directory 'src/data' exists" << endl;
        return 1;
    }

    string key = "abcdefghijklm"; // 13 букв

    cout << "=========================================================================" << endl;
    cout << "String Search Algorithms Performance Test" << endl;
    cout << "=========================================================================" << endl;
    cout << "Alphabet: 7 letters (a-g)" << endl;
    cout << "Search key: \"" << key << "\" (13 letters)" << endl;
    cout << "String structure: random string with ~1% key occurrences" << endl;
    cout << "Iterations per size: " << REPEAT_COUNT << endl;
    cout << "=========================================================================" << endl;

    cout << "|" << setw(11) << "n (length)"
         << " | " << setw(13) << "Basic Search (ms)"
         << " | " << setw(15) << "KMP (ms)"
         << " | " << setw(15) << "Boyer-Moore (ms)" << " |" << endl;
    cout << "|------------|-----------------|-----------------|-----------------|" << endl;

    vector<int> testSizes = {
    100, 200, 300, 400, 500,
    600, 700, 800, 900, 1000,
    1100, 1200, 1300, 1400, 1500,
    1600, 1700, 1800, 1900, 2000,
    2100, 2200, 2300, 2400, 2500,
    2600, 2700, 2800, 2900, 3000
    };

    for (int n : testSizes) {
        testAlgosWithFileOutput(n, key, outSizes, outBasic, outKMP, outBM);
    }

    cout << "=========================================================================" << endl;
    cout << "Results show average time per search in milliseconds" << endl;
    cout << "=========================================================================" << endl;

    // Закрываем файлы
    outSizes.close();
    outBasic.close();
    outKMP.close();
    outBM.close();

    cout << "\n✓ Results saved to src/data/ directory" << endl;
    cout << "Files created:" << endl;
    cout << "  - src/data/sizes.txt (x-axis: string lengths)" << endl;
    cout << "  - src/data/brute_force.txt (y1: Basic Search times)" << endl;
    cout << "  - src/data/kmp.txt (y2: KMP times)" << endl;
    cout << "  - src/data/boyer_moore.txt (y3: Boyer-Moore times)" << endl;

    return 0;
}
