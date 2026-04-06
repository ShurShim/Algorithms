#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

#define ALGO_APPLIES 10000

// "Тяжелый" Brute Force - делает максимальное количество сравнений
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

// Алгоритм Z-функции
vector<int> z_function(const string& s) {
    int n = s.length();
    vector<int> z(n);
    int l = 0, r = 0;

    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

vector<int> z_search_all(const string& source, const string& pattern) {
    vector<int> occurrences;
    if (pattern.length() > source.length()) return occurrences;
    
    string concat = pattern + "$" + source;
    vector<int> z = z_function(concat);

    for (int i = pattern.length() + 1; i < z.size(); i++) {
        if (z[i] == pattern.length()) {
            occurrences.push_back(i - (pattern.length() + 1));
        }
    }
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

// Функция тестирования
void testAlgos(int n, const string& key) {
    double t1_sum = 0, t2_sum = 0, t3_sum = 0;

    for (int testIteration = 0; testIteration < ALGO_APPLIES; testIteration++) {
        string source = generateString(n, key);
        
        double t1_start = clock();
        vector<int> result1 = full_numerate_search_all(source, key);
        double t1_end = clock();
        t1_sum += (t1_end - t1_start);

        double t2_start = clock();
        vector<int> result2 = kmp_search_all(source, key);
        double t2_end = clock();
        t2_sum += (t2_end - t2_start);

        double t3_start = clock();
        vector<int> result3 = z_search_all(source, key);
        double t3_end = clock();
        t3_sum += (t3_end - t3_start);
    }

    double t1_sec = t1_sum / CLOCKS_PER_SEC;
    double t2_sec = t2_sum / CLOCKS_PER_SEC;
    double t3_sec = t3_sum / CLOCKS_PER_SEC;
    
    cout << "| " << setw(10) << n 
         << " | " << setw(15) << fixed << setprecision(6) << t1_sec 
         << " | " << setw(15) << t2_sec 
         << " | " << setw(15) << t3_sec << " |" << endl;
}

int main() {
    srand(time(0));
    
    string key = "abcdefghijklm"; // 13 букв
    
    cout << "=========================================================================" << endl;
    cout << "String Search Algorithms Performance Test" << endl;
    cout << "=========================================================================" << endl;
    cout << "Alphabet: 7 letters (a-g)" << endl;
    cout << "Search key: \"" << key << "\" (13 letters)" << endl;
    cout << "String structure: random string with ~1% key occurrences" << endl;
    cout << "Iterations per size: " << ALGO_APPLIES << endl;
    cout << "=========================================================================" << endl;
    
    cout << "|" << setw(12) << "n (length)" 
         << " | " << setw(15) << "Brute Force (s)" 
         << " | " << setw(15) << "KMP (s)" 
         << " | " << setw(15) << "Z-function (s)" << " |" << endl;
    cout << "|------------|-----------------|-----------------|-----------------|" << endl;
    
    vector<int> testSizes = {1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 
                             2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000};
    
    for (int n : testSizes) {
        testAlgos(n, key);
    }
    
    cout << "=========================================================================" << endl;
    cout << "Results show total time for " << ALGO_APPLIES << " iterations" << endl;
    cout << "=========================================================================" << endl;

    return 0;
}
