/*
Строка называется палиндромом, 
если она одинаково читается как слева направо, так и справа налево. 
Например, «abba» – палиндром, а «roman» – нет.

Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать 
число пар (i, j), 1 ≤ i < j ≤ n, 
таких что подстрока s[i..j] является палиндромом.
*/

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cin;
using std::min;
using std::cout;

int64_t CountPalindromes (const string& s) {
  int64_t n = s.size();
  int64_t palindromes_count = 0;
  int64_t palindrome_count;
  int64_t reflection;
  for (char even = 0; even < 2; ++even) {
    vector<int64_t> palindromes(n, 0);
    int64_t l = 0;
    int64_t r = -1;

    for (int64_t i = 0; i < n; ++i) {
      palindrome_count = 0;
      if (i < r) {
        reflection = r - i + l + even;
        palindrome_count = min(palindromes[reflection], r - i + even);
      }
      while (0 <= i - palindrome_count - 1 && i + palindrome_count + !even < n &&
             s[i - palindrome_count - 1] == s[i + palindrome_count + !even]) {
        palindrome_count++;
      }
      palindromes[i] = palindrome_count;
      palindromes_count += palindromes[i];
      if (r < i + palindrome_count - even) {
        l = i - palindrome_count;
        r = i + palindrome_count - even;
      }
    }
  }
  return palindromes_count;
}

int main() {
  string s;
  cin >> s;
  cout << CountPalindromes(s);
}