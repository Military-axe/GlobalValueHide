#include "utils.h"
#include <cstdint>
#include <random>

namespace utils {

uint8_t RandomEngine::getUint8() { return getRange(0, UINT8_MAX); }
uint16_t RandomEngine::getUint16() { return getRange(0, UINT16_MAX); }
uint32_t RandomEngine::getUint32() { return getRange(0, UINT32_MAX); }
uint64_t RandomEngine::getUint64() { return getRange(0, UINT64_MAX); }
uint64_t RandomEngine::getRange(uint64_t min_val, uint64_t max_val) {
  dynamic_dist.param(decltype(dynamic_dist)::param_type(min_val, max_val));
  return dynamic_dist(engine);
}

} // namespace utils