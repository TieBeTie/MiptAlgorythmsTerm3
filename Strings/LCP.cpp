/*
Дана строка длины n. 
Найти количество ее различных подстрок. 
Используйте суффиксный массив.

Построение суффиксного массива выполняйте за O(n log n). 
Вычисление количества различных подстрок выполняйте за O(n).
*/

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::vector;

const int ALPHABET = 256;

void ZeroPhase(const string& s, vector<uint64_t>& permutation,
               vector<vector<uint64_t>>& classes,
               int64_t & classes_count, vector<uint64_t> char_count) {
    //сортировка подсчётом
    for (char i : s) {
        ++char_count[i];
    }
    for (uint64_t i = 1; i < ALPHABET; ++i) {
        char_count[i] += char_count[i - 1];
    }
    for (uint64_t i = 0; i < s.size(); ++i) {
        permutation[--char_count[s[i]]] = i;
    }
    //подсчёт классов
    classes_count = 1;
    classes[0][permutation[0]] = 0;
    for (int64_t i = 1; i < s.size(); ++i) {
        if (s[permutation[i]] != s[permutation[i - 1]]) ++classes_count;
        classes[0][permutation[i]] = classes_count - 1;
    }
}

void SuffixArray(const string& s, vector<uint64_t>& permutation,
                 vector<vector<uint64_t>>& classes,
                 int64_t & classes_count,
                 vector<uint64_t> char_count,
                 int64_t& substring_length,
                 vector<int64_t>& new_permutation) {
    for (; (1 << substring_length) < s.size(); substring_length++) {
        //сдвиги
        for (int64_t i = 0; i < s.size(); ++i) {
            new_permutation[i] = permutation[i] - (1 << substring_length);
            if (new_permutation[i] < 0) {
                new_permutation[i] += s.size();
            }
        }
        // сортируем пары (classes[i] , classes[i + 2^substring_length])
        // с учётом того, что младший разряд уже осортирован
        char_count.assign(classes_count, 0);
        for (uint64_t i : new_permutation) {
            ++char_count[classes[substring_length][i]];
        }
        for (uint64_t i = 1; i < classes_count; ++i) {
            char_count[i] += char_count[i - 1];
        }
        //так как на ZeroPhase заполнялся  с конца, аналогично
        for (int64_t i = s.size() - 1; i >= 0; i--) {
            permutation[--char_count[classes[substring_length][new_permutation[i]]]] =
                    new_permutation[i];
        }

        // Аналогично, считаем новые классы эквивалентности
        classes.emplace_back(s.size());
        classes[substring_length + 1][permutation[0]] = 0;
        classes_count = 1;
        for (int64_t i = 1; i < s.size(); ++i) {
            int64_t first_mid = (permutation[i] + (1 << substring_length))
                    % s.size();
            int64_t second_mid = (permutation[i - 1] + (1 << substring_length))
                    % s.size();

            if (classes[substring_length][permutation[i]] !=
                classes[substring_length][permutation[i - 1]]
                ||
                classes[substring_length][first_mid] !=
                classes[substring_length][second_mid]) {
                classes_count++;
            }
            classes[substring_length + 1][permutation[i]] = classes_count - 1;
        }
    }
}

vector<int64_t> LCP(string& s, vector<uint64_t>& permutation,
                   vector<vector<uint64_t>>& classes, int64_t substring_length) {
    vector<int64_t> lcp(s.size() - 2, 0);
    for (int64_t i = 1; i < s.size() - 1; ++i) {
        int64_t head = permutation[i];
        int64_t tale = permutation[i + 1];
        for (int64_t sub = substring_length - 1; sub >= 0; --sub)
            if (classes[sub][head] == classes[sub][tale]) {
                lcp[i - 1] += 1 << sub;
                head += 1 << sub;
                tale += 1 << sub;
            }
    }
    return lcp;
}

uint64_t SubstringsCount(std::string s) {
    s.push_back('$');
    vector<uint64_t> permutation(s.size());
    vector<vector<uint64_t>> classes(1, vector<uint64_t>(s.size()));
    vector<uint64_t> char_count (ALPHABET, 0);
    int64_t classes_count;

    ZeroPhase(s, permutation, classes, classes_count, char_count);
    vector<int64_t> new_permutation(s.size());
    int64_t substring_length = 0;
    SuffixArray(s, permutation, classes, classes_count,
                char_count, substring_length, new_permutation);

    vector<int64_t> lcp = LCP(s, permutation, classes, substring_length);
    // считаем подстроки, используя lcp
    int64_t answer = 0;
    for (uint64_t i = 1; i < s.size(); ++i) {
        answer += (s.size() - 1) - permutation[i];
    }
    //отнимаем количество общих подстрок
    for (uint64_t i = 0; i < s.size() - 2; ++i) {
        answer -= lcp[i];
    }
    return answer;
}

int main() {
    std::string str;
    std::cin >> str;
    std::cout << SubstringsCount(str);
}
