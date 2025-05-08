#pragma once

#include "substitution.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace global_value_hide {

/// @brief Concrete substitution implementing additive-mask transformation
/// @details Implements the formula:
/// `constA - ((-(key + constA)) + ((key + constA) ^ constB) + constC) ^ constD)
/// Provides anti-optimization measures through volatile operations and random
/// instruction sequencing.
class Sub1 : public AlgebraicSubstitutionBase<Sub1> {
public:
  /// @brief Applies algebraic substitution with anti-optimization measures
  /// @inheritDoc AlgebraicSubstitutionInterface::substitution
  /// @note Generates non-deterministic code patterns to hinder static analysis
  llvm::Value *substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                            llvm::Constant *key,
                            llvm::LLVMContext &ctx) override;
};

} // namespace global_value_hide