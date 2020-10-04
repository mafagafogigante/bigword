#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "analyzer.hpp"
#include "linestream.hpp"
#include "rules.hpp"

namespace BigWord {
std::string get_words_filename();

class LetterCount {
 private:
  size_t letter_count = 0;
  std::array<uint8_t, AlphabetSize> counters{};

 public:
  LetterCount();

  void initialize(const std::string &word);

  bool operator==(const LetterCount &other) const;

  bool operator<(const LetterCount &other) const;

  bool is_contained(const LetterCount &o, const Analysis *an) const;
};

class Word {
 private:
  std::string word;
  LineNumber line_number;  // Zero indicates an undefined line.
  LetterCount count;

 public:
  explicit Word(const std::string &string = "", LineNumber line = 0);

  bool operator==(const Word &other) const;

  bool operator<(const Word &other) const;

  LetterCount get_count();

  std::string to_string() const;

  LineNumber get_line_number() const;

  static bool is_shorter(const Word &a, const Word &b);

  static bool is_shorter_and_smaller(const Word &a, const Word &b);

  static bool is_contained(Word &a, Word &b, const Analysis *an);
};

std::ostream &operator<<(std::ostream &os, const Word &word);

void dump_word_to_store(std::ostream &os, const Word &word);

Word read_word_from_store(std::istream &is);
}  // namespace BigWord
