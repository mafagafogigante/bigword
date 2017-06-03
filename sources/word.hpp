#ifndef WORD_HPP
#define WORD_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "analyzer.hpp"
#include "linestream.hpp"
#include "rules.hpp"

std::string get_words_filename();

class LetterCount {
 private:
  size_t letter_count = 0;
  uint8_t counters[alphabet_size] = {0};

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
  // Zero indicates an undefined line.
  LineNumber line_number;
  LetterCount count;

 public:
  // Zero indicates an undefined line.
  Word(const std::string &string, const LineNumber line = 0);
  Word(std::istream &is);

  bool operator==(const Word &other) const;

  bool operator<(const Word &other) const;

  LetterCount get_count();

  std::string to_string() const;

  LineNumber get_line_number() const;

  static bool is_shorter(const Word &a, const Word &b);

  static bool is_shorter_and_smaller(const Word &a, const Word &b);

  static bool is_contained(Word &a, Word &b, const Analysis *an);

  void dump(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const Word &word);

#endif
