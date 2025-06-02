#pragma once

#include "pass/algebraic_substitution/substitution.h"

namespace global_value_hide {

class Sub2 : public AlgebraicSubstitutionBase<Sub2> {
public:
  llvm::Value *substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                            llvm::Constant *key,
                            llvm::LLVMContext &ctx) override;                          
};

} // namespace global_value_hide