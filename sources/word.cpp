#include "word.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include "data.hpp"
#include "linestream.hpp"
#include "rules.hpp"

static void report_unsupported_letter(const char letter) {
  std::cerr << "Found unsupported letter:";
  std::cerr << ' ';
  std::cerr << '"';
  std::cerr << letter;
  std::cerr << '"';
  std::cerr << '\n';
}

std::string get_words_filename() {
  return "words.txt";
}

LetterCount::LetterCount() {}

void LetterCount::initialize(const std::string &word) {
  if (letter_count > 0) {
    return;
  }
  for (const char letter : word) {
    if (is_valid_letter(letter)) {
      counters[get_letter_index(letter)]++;
      letter_count++;
    } else {
      report_unsupported_letter(letter);
    }
  }
}

bool LetterCount::operator==(const LetterCount &other) const {
  return letter_count == other.letter_count && counters == other.counters;
}

bool LetterCount::operator<(const LetterCount &other) const {
  return letter_count < other.letter_count;
}

bool LetterCount::is_contained(const LetterCount &o, const Analysis *an) const {
  if (letter_count > o.letter_count) {
    return false;
  }
  size_t remaining = o.letter_count - letter_count;
  for (size_t i = 0; i < alphabet_size; i++) {
    const size_t x = an == nullptr ? i : an->best_index(i);
    if (counters[x] > o.counters[x]) {
      return false;
    }
    // By catching excessive differences we may fail early.
    const size_t difference = o.counters[x] - counters[x];
    if (difference > remaining) {
      return false;
    }
    remaining -= difference;
  }
  return true;
}

Word::Word(const std::string &string, const LineNumber line) {
  word = string;
  line_number = line;
}

Word::Word(std::istream &is) {
  is.read(reinterpret_cast<char *>(&line_number), sizeof(LineNumber));
  word = read_safe_string(is);
  is.ignore(1);  // Ignore the newline.
}

bool Word::operator==(const Word &other) const {
  return word == other.word && count == other.count;
}

bool Word::operator<(const Word &other) const {
  return word < other.word;
}

LetterCount Word::get_count() {
  count.initialize(word);
  return count;
}

std::string Word::to_string() const {
  return word;
}

LineNumber Word::get_line_number() const {
  return line_number;
}

bool Word::is_shorter(const Word &a, const Word &b) {
  return a.word.size() < b.word.size();
}

bool Word::is_shorter_and_smaller(const Word &a, const Word &b) {
  if (Word::is_shorter(a, b)) {
    return true;
  } else if (Word::is_shorter(b, a)) {
    return false;
  } else {
    return a.word < b.word;
  }
}

bool Word::is_contained(Word &a, Word &b, const Analysis *an) {
  return a.get_count().is_contained(b.get_count(), an);
}

void Word::dump(std::ostream &os) const {
  os.write(reinterpret_cast<const char *>(&line_number), sizeof(LineNumber));
  dump_safe_string(os, word);
}

std::ostream &operator<<(std::ostream &os, const Word &word) {
  os << word.to_string();
  return os;
}
