#include "option.hpp"

#include <iostream>
#include <vector>

#include "configuration.hpp"

namespace BigWord {
const OptionValue OptionValue::negative = OptionValue("Negative", 0);
const OptionValue OptionValue::positive = OptionValue("Positive", 1);

static const std::string option_name_values = "--configuration";
static const std::string option_info_values = "Output the option values.";

static const std::string option_name_version = "--version";
static const std::string option_info_version = "Output the program version.";

static const std::string option_name_time = "--time";
static const std::string option_info_time = "Output computation times.";

static const std::string option_name_line_number = "--line-number";
static const std::string option_info_line_number = "Output line numbers.";

static const std::string option_name_source = "--source";
static const std::string option_info_source = "Set the source file.";

std::string OptionInfo::get_name() const {
  return name;
}

std::string OptionInfo::get_info() const {
  return info;
}

OptionValue::OptionValue(const std::string &text, const int64_t integer)
    : text(text), integer(integer) {}

bool OptionValue::operator==(const OptionValue &other) const {
  return text == other.text && integer == other.integer;
}

bool OptionValue::is_boolean() const {
  return *this == negative || *this == positive;
}

bool OptionValue::to_boolean() const {
  return integer != 0;
}

OptionList::OptionList() {
  OptionInfo line_number_info(option_name_line_number, option_info_line_number);
  add_option(Option(line_number_info, OptionValue::negative));
  OptionInfo line_number_version(option_name_version, option_info_version);
  add_option(Option(line_number_version, OptionValue::negative));

  OptionInfo time_info(option_name_time, option_info_time);
  add_option(Option(time_info, OptionValue::negative));
  OptionInfo values_info(option_name_values, option_info_values);
  add_option(Option(values_info, OptionValue::negative));

  OptionInfo source_info(option_name_source, option_info_source);
  add_option(Option(source_info, OptionValue(default_source, 0)));
}

void OptionList::add_option(Option option) {
  map.insert({option.info.get_name(), option});
}

static std::string get_option_name(const std::string &string) {
  const auto end = std::find(string.begin(), string.end(), '=');
  return std::string(string.begin(), end);
}

static std::vector<std::string> get_arguments(const std::string &string) {
  std::vector<std::string> arguments;
  auto it = std::find(string.begin(), string.end(), '=');
  if (it == string.end()) {
    return arguments;
  }
  it = std::next(it);
  while (it != string.end()) {
    const std::string::const_iterator end = std::find(it, string.end(), ',');
    arguments.emplace_back(it, end);
    it = end;
  }
  return arguments;
}

void OptionList::parse(const std::string &string) {
  const auto option_name = get_option_name(string);
  const auto arguments = get_arguments(string);
  try {
    auto &option = map.at(option_name);
    if (option.value.is_boolean()) {
      option.value = OptionValue::positive;
      return;
    }
    if (arguments.empty()) {
      const auto message = "Option " + option_name + " requires an argument.";
      throw std::runtime_error(message);
    }
    option.value = OptionValue(arguments[0], 0);
  } catch (const std::out_of_range &exception) {
    // Not an option, just ignore it.
  }
}

OptionValue OptionList::get_value(const std::string &string) const {
  const auto iter = map.find(string);
  if (iter == map.end()) {
    return OptionValue::negative;
  }
  return iter->second.value;
}

bool OptionList::is_timing() const {
  return get_value(option_name_time).to_boolean();
}

bool OptionList::is_printing_line_numbers() const {
  return get_value(option_name_line_number).to_boolean();
}

bool OptionList::is_printing_configuration() const {
  return get_value(option_name_values).to_boolean();
}

bool OptionList::is_printing_version() const {
  return get_value(option_name_version).to_boolean();
}

bool OptionList::needs_query() const {
  return !(is_printing_configuration() || is_printing_version());
}

std::string OptionList::get_source_file() const {
  return get_value(option_name_source).text;
}

void OptionList::print_options() const {
  for (const auto &pair : map) {
    std::cout << std::setw(4) << ' ';
    std::cout << std::setw(16) << std::left;
    std::cout << pair.second.info.get_name();
    std::cout << ' ';
    std::cout << pair.second.info.get_info();
    std::cout << '\n';
  }
}

void OptionList::print_configuration() const {
  for (const auto &pair : map) {
    std::cout << std::setw(4) << ' ';
    std::cout << std::setw(16) << std::left;
    std::cout << pair.second.info.get_name();
    std::cout << ' ';
    std::cout << pair.second.value.text;
    std::cout << '\n';
  }
}
}  // namespace BigWord