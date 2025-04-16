#pragma once

#include <cstdint>
#include <random>
#include <vector>

namespace utils {

/// @brief Predefined constant indicating element not found in search operations
constexpr int FIND_NOT_FOUND = -1;

/// @brief Find the index of target element in a vector
/// @tparam T Type of elements in the vector
/// @param vec Container to search in
/// @param target Element to locate
/// @return Index of the element if found, FIND_NOT_FOUND otherwise
template <typename T>
int findElementIndex(const std::vector<T> &vec, const T &target) {
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i] == target) {
      return i;
    }
  }
  return FIND_NOT_FOUND;
}

/// @brief Random number generator utility class
/// @details Provides thread-safe generation of various unsigned integer types
/// using C++11 random library facilities. Supports 8/16/32/64-bit integers
/// and custom ranges.
class RandomEngine {
private:
  std::mt19937_64 engine;
  mutable std::uniform_int_distribution<uint64_t> dynamic_dist;

public:
  /// @brief Construct a new RandomEngine with proper initialization
  RandomEngine() : engine(std::random_device{}()) {}

  /// @brief Generate random 8-bit unsigned integer
  /// @return Random value in [0, 255] range
  uint8_t getUint8();

  /// @brief Generate random 16-bit unsigned integer
  /// @return Random value in [0, 65535] range
  uint16_t getUint16();

  /// @brief Generate random 32-bit unsigned integer
  /// @return Random value in [0, 4294967295] range
  uint32_t getUint32();

  /// @brief Generate random 64-bit unsigned integer
  /// @return Random value in [0, 18446744073709551615] range
  uint64_t getUint64();

  /// @brief Generate random number in specified range
  /// @param minVal Inclusive lower bound of the range
  /// @param maxVal Inclusive upper bound of the range
  /// @return Random value in [begin, end] range
  uint64_t getRange(uint64_t minVal, uint64_t maxVal);
};
} // namespace utils