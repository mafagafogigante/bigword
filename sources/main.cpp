#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include "configuration.hpp"
#include "license.hpp"
#include "option.hpp"
#include "store.hpp"
#include "word.hpp"

typedef std::vector<Word> WordVector;

static void print_usage(const std::string &program_name, OptionList options) {
  std::cout << "Usage: " << program_name;
  std::cout << ' ' << "[OPTIONS]";
  std::cout << ' ' << "LETTERS";
  std::cout << '\n';
  options.print_options();
}

/**
 * Reads the command line options.
 */
static OptionList read_options(int argc, char *argv[]) {
  OptionList options;
  for (int i = 1; i != argc; i++) {
    options.parse(std::string(argv[i]));
  }
  return options;
}

/**
 * Reads the input letters and make a word from it.
 *
 * Returns the empty word if there were no good candidates.
 */
static WordVector read_input(int argc, char *argv[]) {
  WordVector words;
  for (int i = 1; i != argc; i++) {
    const std::string argument(argv[i]);
    if (is_valid_word(argument)) {
      words.push_back(Word(argument));
    }
  }
  return words;
}

static WordVector find_matches(const WordStore &store, Word input) {
  WordVector matches;
  const WordVector &words = store.words;
  const Analysis *analysis = &store.analysis;
  const auto begin = words.begin();
  const auto end = words.end();
  auto iter = std::upper_bound(begin, end, input, Word::is_shorter);
  while (iter != begin) {
    iter--;
    Word word = *iter;
    if (!matches.empty()) {
      // Stop if we reached smaller words.
      if (Word::is_shorter(word, *matches.begin())) {
        break;
      }
    }
    if (Word::is_contained(word, input, analysis)) {
      matches.push_back(word);
    }
  }
  std::reverse(matches.begin(), matches.end());
  return matches;
}

static void write_escape() {
  std::cout << "--";
  std::cout << ' ';
}

static void write_duration(const std::string &action, Duration duration) {
  write_escape();
  std::cout << "Finished";
  std::cout << ' ';
  std::cout << action;
  std::cout << ' ';
  std::cout << "after";
  std::cout << ' ';
  write_time(duration.to_nanoseconds());
  std::cout << '.';
  std::cout << '\n';
}

int main(int argc, char *argv[]) {
  std::ios_base::sync_with_stdio(false);
  const TimePoint loading_start;
  OptionList options = read_options(argc, argv);
  if (argc < 2) {
    // Fail if there are too few arguments.
    print_usage(std::string(argv[0]), options);
    return 0;
  }
  if (options.is_printing_configuration()) {
    options.print_configuration();
  }
  if (options.is_printing_copyright()) {
    print_license();
  }
  if (options.is_printing_version()) {
    std::cout << "BigWord version " << version << '\n';
  }
  const WordVector words = read_input(argc, argv);
  if (words.empty()) {
    // Fail if there is no query and one is needed.
    if (options.needs_query()) {
      print_usage(std::string(argv[0]), options);
    }
    // Prevent loading the WordStore.
    return 0;
  }
  WordStore store = load_word_store(options.get_source_file());
  if (options.is_timing()) {
    write_duration("loading", Duration(loading_start, TimePoint()));
  }
  for (size_t query = 0; query != words.size(); query++) {
    const TimePoint query_start;
    const WordVector matches = find_matches(store, words[query]);
    write_escape();
    std::cout << "Query" << ' ' << query + 1 << ' ';
    if (matches.empty()) {
      std::cout << "did not match any words";
    } else {
      std::cout << "matched" << ' ' << matches.size() << ' ';
      std::cout << (matches.size() == 1 ? "word" : "words");
    }
    if (options.is_timing()) {
      std::cout << ' ';
      std::cout << "after";
      std::cout << ' ';
      write_time(Duration(query_start, TimePoint()).to_nanoseconds());
    }
    std::cout << '.' << '\n';
    for (Word match : matches) {
      if (options.is_printing_line_numbers()) {
        std::cout << match.get_line_number() << ':';
      }
      std::cout << match;
      std::cout << '\n';
    }
  }
  return 0;
}
