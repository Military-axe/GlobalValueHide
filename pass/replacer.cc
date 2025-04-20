#include "replacer.h"
#include "prelude.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/User.h>

using namespace llvm;

namespace global_value_hide {

void GlobalValueReplacer::replace(const EncGvsInfo &gvs,
                                  const EncFunsInfo &funcs) {
  auto &sub = substitution_.get()->choose();

  for (const auto &gv : gvs) {
    ReplaceTrait<GlobalVariable>::replace(ctx_, gv, sub);
  }

  for (const auto &func : funcs) {
    ReplaceTrait<Function>::replace(ctx_, func, sub);
  }
}

} // namespace global_value_hide