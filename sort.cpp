#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define ALGO_APPLIES 10000

int randomNumber(int min, int max){
    return rand()%(max-min-1) + min;
}

char randomChar(){
    return char(randomNumber(int('a'), int('z')));
}

string generateString(int n){
    string temp = "";
    for(int i = 0;i < n;i++){
        temp += randomChar();
    }
    return temp;
}

int basicSearch(string s, string sub){
    for(int i = 0; i <= s.length()-sub.length();i++){
        if(s[i] == sub[0]){
            for(int j = 1;j < sub.length();j++){
                if(s[i+j] != sub[j]) break;
                if(j == sub.length()-1) return i;
            }
        }
    }
    return -1;
}

// Algorithm 2: KMP
vector<int> computeLPS(string pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
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

int KMPSearch(string s, string sub) {
    int n = s.length();
    int m = sub.length();
    
    if (m == 0) return 0;
    if (n < m) return -1;
    
    vector<int> lps = computeLPS(sub);
    int i = 0;
    int j = 0;
    
    while (i < n) {
        if (sub[j] == s[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            return i - j;
        } else if (i < n && sub[j] != s[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1;
}

// Algorithm 3: Boyer-Moore
vector<int> computeBadCharHeuristic(string sub) {
    vector<int> badChar(256, -1);
    for (int i = 0; i < sub.length(); i++) {
        badChar[sub[i]] = i;
    }
    return badChar;
}

int boyerMooreSearch(string s, string sub) {
    int n = s.length();
    int m = sub.length();
    
    if (m == 0) return 0;
    if (n < m) return -1;
    
    vector<int> badChar = computeBadCharHeuristic(sub);
    int shift = 0;
    
    while (shift <= n - m) {
        int j = m - 1;
        
        while (j >= 0 && sub[j] == s[shift + j]) {
            j--;
        }
        
        if (j < 0) {
            return shift;
        } else {
            int move = j - badChar[s[shift + j]];
            if (move < 1) move = 1;
            shift += move;
        }
    }
    return -1;
}

int main() {
    srand(time(0));
    
    const int NUM_TESTS = 10000;
    const int START_SIZE = 100;
    const int END_SIZE = 2000;
    const int STEP = 100;
    
    ofstream outFile("search_results.txt");
    
    // Вывод заголовка таблицы в консоль
    cout << fixed << setprecision(7);
    cout << "| String Length | Basic(ms) | KMP(ms) | Boyer-Moore(ms) |" << endl;
    cout << "|---------------|-----------|---------|-----------------|" << endl;
    
    // Запись заголовка в файл
    outFile << "String Length | Basic(ms) | KMP(ms) | Boyer-Moore(ms) |" << endl;
    
    for (int size = START_SIZE; size <= END_SIZE; size += STEP) {
        double totalBasicTime = 0;
        double totalKMPTime = 0;
        double totalBoyerMooreTime = 0;
        
        for (int test = 0; test < NUM_TESTS; test++) {
            string s = generateString(size);
            string sub = generateString(randomNumber(10,15));
            
            auto start = high_resolution_clock::now();
            basicSearch(s, sub);
            auto end = high_resolution_clock::now();
            totalBasicTime += duration_cast<microseconds>(end - start).count();
            
            start = high_resolution_clock::now();
            KMPSearch(s, sub);
            end = high_resolution_clock::now();
            totalKMPTime += duration_cast<microseconds>(end - start).count();
            
            start = high_resolution_clock::now();
            boyerMooreSearch(s, sub);
            end = high_resolution_clock::now();
            totalBoyerMooreTime += duration_cast<microseconds>(end - start).count();
        }
        
        double avgBasic = (totalBasicTime / NUM_TESTS) / 1000.0;
        double avgKMP = (totalKMPTime / NUM_TESTS) / 1000.0;
        double avgBoyerMoore = (totalBoyerMooreTime / NUM_TESTS) / 1000.0;
        
        // Вывод в консоль в формате таблицы
        cout << "| " << setw(13) << size << " | " 
             << setw(9) << avgBasic << " | " 
             << setw(7) << avgKMP << " | " 
             << setw(15) << avgBoyerMoore << " |" << endl;
        
        // Запись в файл в формате таблицы
        outFile << "| " << size << " | " 
                << avgBasic << " | " 
                << avgKMP << " | " 
                << avgBoyerMoore << " |" << endl;
    }
    
    outFile.close();
    
    cout << "\nResults saved to search_results.txt" << endl;
    
    return 0;
}