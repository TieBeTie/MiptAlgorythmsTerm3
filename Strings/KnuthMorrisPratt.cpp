/*
Найдите все вхождения шаблона в строку. 
Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
p ≤ 30000, n ≤ 300000.

Использовать один из методов:
Вариант 1. С помощью префикс-функции;
Вариант 2. С помощью z-функции.
*/

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::vector;
using std::getline;

vector <size_t> PrefixFunction(const string& s) {
    vector <size_t> result(s.length(), 0);
    size_t prev_pf = 0;

    for (size_t i = 1; i < s.length(); ++i) {
        prev_pf = result[i - 1];

        while (prev_pf > 0 && s[i] != s[prev_pf]) {
            prev_pf = result[prev_pf - 1];
        }
        if (s[i] == s[prev_pf]) {
            result[i] = ++prev_pf;
        }
    }

    return result;
}

void PrintKnuthMorrisPratt(const string& pattern, const string& text) {
    vector <size_t> prefix_pattern = PrefixFunction(pattern);
    size_t prev_prefix_in_text = 0;

    for (size_t i = 0; i < text.length(); ++i) {

        while (pattern.size() == prev_prefix_in_text ||
               prev_prefix_in_text > 0 && text[i] != pattern[prev_prefix_in_text]) {

            prev_prefix_in_text = prefix_pattern[prev_prefix_in_text - 1];
        }
        if (text[i] == pattern[prev_prefix_in_text]) {
            ++prev_prefix_in_text;
        }
        if (prev_prefix_in_text == pattern.size()) {
            cout << i - pattern.size() + 1 << ' ';
        }
    }
}

int main() {
    string pattern, text;
    getline(cin, pattern);
    getline(cin, text);
    PrintKnuthMorrisPratt(pattern, text);
}
