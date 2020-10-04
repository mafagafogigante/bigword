#include "store.hpp"

#include <execution>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include <openssl/evp.h>

#include "data.hpp"
#include "linestream.hpp"
#include "rules.hpp"

namespace BigWord {
void WordStore::compile() {
  std::sort(std::execution::par_unseq, words.begin(), words.end(),
            Word::is_shorter_and_smaller);
  analysis.compile();
}

static std::ostream &operator<<(std::ostream &os, const WordStore &store) {
  dump_unsafe_string(os, store.store_name);
  os << store.source_digest << '\n';
  os << store.analysis << '\n';
  os << store.words.size() << '\n';
  for (const Word &word : store.words) {
    dump_word_to_store(os, word);
  }
  return os;
}

static std::istream &operator>>(std::istream &is, WordStore &store) {
  size_t word_count = 0;
  store.store_name = read_unsafe_string(is);
  is >> store.source_digest;
  is >> store.analysis;
  is >> word_count;
  store.words.reserve(word_count);
  for (size_t i = 0; i < word_count; i++) {
    store.words.push_back(read_word_from_store(is));
  }
  return is;
}

static std::string derive_store_name(const std::string &filename) {
  return filename + ".store";
}

static void dump_word_store(const WordStore &store) {
  std::ofstream ofs(store.store_name);
  ofs << store;
}

// Helper static function: we can't take the address of a constructor.
static Digest digest_file(const std::string &filename) {
  return Digest(filename);
}

static WordStore make_word_store(const std::string &filename) {
  auto future_digest = std::async(std::launch::async, digest_file, filename);
  WordStore store;
  store.store_name = derive_store_name(filename);
  std::ifstream ifs(filename);
  LineStream line_stream(ifs);
  std::istream is(&line_stream);
  while (is) {
    std::string string;
    is >> string;
    if (!is_valid_word(string)) {
      continue;
    }
    const LineNumber line_number = line_stream.get_line_number();
    store.words.emplace_back(string, line_number);
    store.analysis.analyze(string);
  }
  store.source_digest = future_digest.get();
  store.compile();
  return store;
}

/**
 * Loads the WordStore for the specified text file.
 */
WordStore load_word_store(const std::string &filename) {
  const std::string store_filename = derive_store_name(filename);
  std::ifstream input_stream;
  input_stream.open(store_filename);
  if (input_stream.is_open()) {
    auto future_digest = std::async(std::launch::async, digest_file, filename);
    WordStore store;
    input_stream >> store;
    if (store.source_digest == future_digest.get()) {
      return store;
    }
  }
  WordStore store = make_word_store(filename);
  dump_word_store(store);
  return store;
}
}  // namespace BigWord