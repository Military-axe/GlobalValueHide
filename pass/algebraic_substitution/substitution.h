#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>

namespace global_value_hide {

/// @brief Interface for algebraic substitution strategies
/// @details Defines the common interface for all algebraic substitution
/// implementations used in global value hiding transformations.
class AlgebraicSubstitutionInterface {
public:
  /// @brief Performs algebraic substitution on encrypted value
  /// @param IRB LLVM IR builder for instruction insertion
  /// @param encrypted The encrypted value to transform
  /// @param key Constant key used in substitution
  /// @param ctx LLVM context for type creation
  /// @return Transformed LLVM IR value representing substituted result
  virtual llvm::Value *substitution(llvm::IRBuilder<> &IRB,
                                    llvm::Value *encrypted, llvm::Constant *key,
                                    llvm::LLVMContext &ctx) = 0;

  /// @brief Virtual destructor for proper polymorphic cleanup
  virtual ~AlgebraicSubstitutionInterface() = default;
};

/// @brief CRTP base class for algebraic substitution implementations
/// @tparam T Concrete substitution implementation type
/// @details Provides boilerplate for static polymorphism using the Curiously
/// Recurring Template Pattern (CRTP). Derived classes must implement the
/// substitution method.
template <class T>
class AlgebraicSubstitutionBase : public AlgebraicSubstitutionInterface {
public:
  /// @brief Dispatches substitution to concrete implementation
  /// @inheritDoc AlgebraicSubstitutionInterface::substitution
  /// @note Uses static_cast to forward call to derived class implementation
  llvm::Value *substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                            llvm::Constant *key,
                            llvm::LLVMContext &ctx) override {
    return static_cast<T *>(this)->substitution(IRB, encrypted, key, ctx);
  }
};

} // namespace global_value_hide