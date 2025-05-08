#include "substitutionChoose.h"
#include "sub1.h"
#include "utils.h"
#include <memory>

namespace global_value_hide {

AlgSubList AlgebraicSubstitutionChoose::collectorSubstitution() {
  // Add new substitution here
  AlgSubList subs;
  subs.emplace_back(std::make_unique<Sub1>());

  return subs;
}

AlgebraicSubstitutionChoose::AlgebraicSubstitutionChoose() {
  subs_ = collectorSubstitution();
}

AlgebraicSubstitutionInterface &AlgebraicSubstitutionChoose::choose() {
  // choose random substitution
  static utils::RandomEngine rand;
  auto &ref = rand.getRandomRef(subs_);
  return *ref;
}

} // namespace global_value_hide