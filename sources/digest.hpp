#pragma once

#include <array>
#include <fstream>

#include <openssl/evp.h>

namespace BigWord {
class Digest {
 public:
  static constexpr size_t MaximumSize = EVP_MAX_MD_SIZE;

 private:
  unsigned int length = 0;
  std::array<unsigned char, MaximumSize> digest{};

 public:
  Digest() = default;

  /**
   * Constructs a Digest by digesting the contents of the specified file.
   */
  explicit Digest(const std::string &filename);

  bool operator==(const Digest &other) const noexcept;

  friend std::ostream &operator<<(std::ostream &os, const Digest &digest);

  friend std::istream &operator>>(std::istream &is, Digest &digest);
};
}  // namespace BigWord