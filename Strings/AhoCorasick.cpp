/*
Шаблон поиска задан строкой длины m, 
в которой кроме обычных символов могут встречаться символы “?”. 
Найти позиции всех вхождений шаблона в тексте длины n. 
Каждое вхождение шаблона предполагает,
что все обычные символы совпадают с соответствующими из текста, 
а вместо символа “?” в тексте встречается произвольный символ. 
Гарантируется, что сам “?” в тексте не встречается.

Время работы - O(n + m + Z), 
где Z - общее число вхождений 
подстрок шаблона “между вопросиками” в исходном тексте. 
m ≤ 5000, n ≤ 2000000.
*/

#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;
using std::string;
using std::pair;


const int ALPHABET = 256;

class ACTree {
public:
  ACTree(const string &pattern) :
          bor_(1, Vertex(0, -1)),
          pattern_text_(pattern) {
    bor_[0].suffix_link = 0;
    SubPatternPositionsSearch();
    for (int i = 0; i < sub_pattern_positions_.size(); ++i) {
      AddPattern(sub_pattern_positions_[i], i);
    }
  }

  vector<int> MatchesSearch(const string &text) {
    vector<int> entries(text.length());
    int n = text.length();
    int v = 0;
    bool flag = true;

    for (int i = 0; i < n; ++i) {
      v = GetLink(v, text[i]);
      int u = v;
      int first_ind;
      int second_ind;
      while (u != 0) {
        if (bor_[u].is_pattern || flag) {
          for (int index = 0; index < bor_[u].ind_pattern.size(); index++) {

            int startIndex = i - sub_pattern_positions_[bor_[u].ind_pattern[index]].second
                             + sub_pattern_positions_[bor_[u].ind_pattern[index]].first;

            if ((startIndex -
                 sub_pattern_positions_[bor_[u].ind_pattern[index]].first >= 0) &&
                (startIndex - sub_pattern_positions_[bor_[u].ind_pattern[index]].first +
                 pattern_text_.length() - 1 < text.length())) {
              entries[startIndex -
                      sub_pattern_positions_[bor_[u].ind_pattern[index]].first]++;
            }
          }
        }
        flag = true;
        u = GetSuffixLink(u);
      }
    }
    vector<int> result;
    for (int i = 0; i < entries.size(); ++i) {
      if (entries[i] == sub_pattern_positions_.size()) {
        result.push_back(i);
      }
    }
    return result;
  }

private:
  struct Vertex {
    int edges[ALPHABET];
    int short_link[ALPHABET];
    int suffix_link;
    int parent;
    char parrent_char;
    bool is_pattern;
    vector<int> ind_pattern;

    Vertex(int parent, char parrent_char) {
      this->parrent_char = parrent_char;
      this->parent = parent;
      this->suffix_link = -1;
      this->is_pattern = false;

      for (int i = 0; i < ALPHABET; ++i) {
        edges[i] = -1;
        short_link[i] = -1;
      }
    }
  };

  void AddPattern(const pair<int, int> &pattern_position, int pattIndex) {
    int node = 0;
    for (int i = pattern_position.first; i <= pattern_position.second; ++i) {
      char character = pattern_text_[i];
      if (bor_[node].edges[character] == -1) {
        bor_.emplace_back(node, character);
        bor_[node].edges[character] = bor_.size() - 1;
      }
      node = bor_[node].edges[character];
    }
    bor_[node].is_pattern = true;
    bor_[node].ind_pattern.push_back(pattIndex);
  }

  void SubPatternPositionsSearch() {
    pair<int, int> pattern_position;
    int n = pattern_text_.length();

    if (isalpha(pattern_text_[0])) {
      pattern_position.first = 0;
    }
    if (pattern_text_[1] == '?' && isalpha(pattern_text_[0])) {
      pattern_position.second = 0;
      sub_pattern_positions_.push_back(pattern_position);
    }

    for (int i = 1; i < n - 1; ++i) {
      if (pattern_text_[i - 1] == '?' && isalpha(pattern_text_[i])) {
        pattern_position.first = i;                                     //сканирующая прямая
      }
      if (pattern_text_[i + 1] == '?' && isalpha(pattern_text_[i])) {
        pattern_position.second = i;
        sub_pattern_positions_.push_back(pattern_position);
      }
    }

    if (pattern_text_[n - 2] == '?' && isalpha(pattern_text_[n - 1])) {
      pattern_position.first = n - 1;
    }
    if (isalpha(pattern_text_[n - 1])) {
      pattern_position.second = n - 1;
      sub_pattern_positions_.push_back(pattern_position);
    }
  }

  int GetSuffixLink(int index) {
    if (bor_[index].suffix_link == -1) {
      if (bor_[index].parent == 0) {
        bor_[index].suffix_link = 0;
      } else {
        bor_[index].suffix_link =
                GetLink(GetSuffixLink(bor_[index].parent), bor_[index].parrent_char);
      }
    }
    return bor_[index].suffix_link;
  }

  int GetLink(int index, char character) {
    if (bor_[index].short_link[character] == -1) {
      if (bor_[index].edges[character] != -1) {
        bor_[index].short_link[character] = bor_[index].edges[character];
      } else if (index == 0) {
        bor_[index].short_link[character] = 0;
      } else {
        bor_[index].short_link[character] = GetLink(GetSuffixLink(index), character);
      }
    }
    return bor_[index].short_link[character];
  }

  string pattern_text_;
  vector<Vertex> bor_;
  vector<pair<int, int>> sub_pattern_positions_;
};

int main() {
  string pattern, text;
  cin >> pattern >> text;

  bool exp = true;
  for (signed char letter : pattern) {
    if (letter != '?') {
      exp = false;
    }
  }
  if (exp) {
    for (int i = 0; i < text.size() - pattern.size() + 1; ++i) {
      cout << i << ' ';
    }
    return 0;
  }
  ACTree actree(pattern);

  vector<int> ind_full_pattern = actree.MatchesSearch(text);
  for (int i = 0; i < ind_full_pattern.size(); ++i) {
    cout << ind_full_pattern[i] << " ";
  }
}