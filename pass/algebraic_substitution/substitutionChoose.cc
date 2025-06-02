#include "substitutionChoose.h"
#include "sub1/sub1.h"
#include "sub2/sub2.h"
#include "substitution.h"
#include "utils.h"
#include <memory>

namespace global_value_hide {

AlgSubList AlgebraicSubstitutionChoose::collectorSubstitution() {
  // Add new substitution here
  AlgSubList subs;
  subs.emplace_back(std::make_unique<Sub1>());
  subs.emplace_back(std::make_unique<Sub2>());

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