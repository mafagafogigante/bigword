#ifndef LINESTREAM_HPP
#define LINESTREAM_HPP

#include <fstream>
#include <streambuf>
#include "linenumber.hpp"

class LineStream : public std::streambuf {
 private:
  std::streambuf *source;
  std::istream *owner = nullptr;
  LineNumber line_number = 1;

  char buffer = '\0';

 protected:
  int underflow() {
    // Read the next character.
    const int character = source->sbumpc();
    if (character != EOF) {
      if (buffer == '\n') {
        line_number++;
      }
      buffer = character;
      // Set the read area pointers.
      setg(&buffer, &buffer, &buffer + 1);
    }
    return character;
  }

 public:
  LineStream(std::streambuf *source) : source(source) {}

  LineStream(std::istream &stream_owner) {
    // Set the owner buffer to this object.
    owner = &stream_owner;
    // Keep track of the previous buffer so we can restore it in the end.
    source = owner->rdbuf(this);
  }

  ~LineStream() {
    if (owner != nullptr) {
      // Restore the previous buffer.
      owner->rdbuf(source);
    }
  }

  LineNumber get_line_number() const;
};

#endif
