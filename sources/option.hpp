#pragma once

#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "clock.hpp"
#include "word.hpp"

namespace BigWord {
class OptionInfo {
  const std::string name;
  const std::string info;

 public:
  OptionInfo(const std::string &name, const std::string &info)
      : name(name), info(info) {}

  bool operator==(const OptionInfo &other) const {
    return name == other.name && info == other.info;
  }

  [[nodiscard]] std::string get_name() const;
  [[nodiscard]] std::string get_info() const;
};

struct OptionValue {
  std::string text;
  int64_t integer;

  OptionValue(std::string text, int64_t integer);

  bool operator==(const OptionValue &other) const noexcept;

  [[nodiscard]] bool is_boolean() const;

  [[nodiscard]] bool to_boolean() const;
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
  void add_option(const Option &option);
  OptionValue get_value(const std::string &string) const;

 public:
  OptionList();
  void parse(const std::string &string);
  bool needs_query() const;
  bool is_timing() const;
  bool is_printing_line_numbers() const;
  bool is_printing_configuration() const;
  bool is_printing_version() const;
  std::string get_source_file() const;
  void print_options() const;
  void print_configuration() const;
};
}  // namespace BigWord