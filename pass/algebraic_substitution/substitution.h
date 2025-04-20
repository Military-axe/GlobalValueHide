#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>

namespace global_value_hide {

class AlgebraicSubstitutionInterface {
public:
  virtual llvm::Value *substitution(llvm::IRBuilder<> &IRB,
                                    llvm::Value *encrypted, llvm::Constant *key,
                                    llvm::LLVMContext &ctx) = 0;
  virtual ~AlgebraicSubstitutionInterface() = default;
};

template <class T>
class AlgebraicSubstitutionBase : public AlgebraicSubstitutionInterface {
public:
  llvm::Value *substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                            llvm::Constant *key,
                            llvm::LLVMContext &ctx) override {
    return static_cast<T *>(this)->substitution(IRB, encrypted, key, ctx);
  }
};

} // namespace global_value_hide