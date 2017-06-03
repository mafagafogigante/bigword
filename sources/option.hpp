#ifndef OPTION_HPP
#define OPTION_HPP

#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "clock.hpp"
#include "word.hpp"

class OptionInfo {
  const std::string name;
  const std::string info;

 public:
  OptionInfo(const std::string &name, const std::string &info)
      : name(name), info(info) {}

  bool operator==(const OptionInfo &other) const {
    return name == other.name && info == other.info;
  }

  std::string get_name() const;
  std::string get_info() const;
};

struct OptionValue {
  static const OptionValue negative;
  static const OptionValue positive;

  std::string text;
  int64_t integer;

  OptionValue(const std::string &text, const int64_t integer)
      : text(text), integer(integer) {}

  bool operator==(const OptionValue &other) const {
    return text == other.text && integer == other.integer;
  }

  bool is_boolean() const {
    return *this == negative || *this == positive;
  }

  bool to_boolean() const {
    return integer != 0;
  }
};

struct Option {
  OptionInfo info;
  OptionValue value;

  Option(const OptionInfo info, const OptionValue value)
      : info(info), value(value) {}
};

class OptionList {
 private:
  std::map<std::string, Option> map;
  void add_option(Option option);
  OptionValue get_value(const std::string &string) const;

 public:
  OptionList();
  void parse(const std::string &string);
  bool needs_query() const;
  bool is_timing() const;
  bool is_printing_line_numbers() const;
  bool is_printing_configuration() const;
  bool is_printing_copyright() const;
  bool is_printing_version() const;
  std::string get_source_file() const;
  void print_options() const;
  void print_configuration() const;
};

#endif
