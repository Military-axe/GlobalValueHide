#pragma once

#include "substitution.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace global_value_hide {

  // constA - ((-(key + constA)) + ((key + constA) ^ constB) + constC) ^ constD)
class Sub1 : public AlgebraicSubstitutionBase<Sub1> {
public:
  llvm::Value *substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                      llvm::Constant *key, llvm::LLVMContext &ctx) override;
};

} // namespace global_value_hide