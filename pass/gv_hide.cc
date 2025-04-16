#include "gv_hide.h"
#include "collector.h"
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/User.h>
#include <llvm/Support/Casting.h>

using namespace llvm;

namespace global_value_hide {

void GlobalValHideManager::run() {
  collector_->collect();
  encryptor_->enc(collector_->gvs_, collector_->funcs_);
  replacer_->replace(encryptor_->gv_, encryptor_->func_);
}

} // namespace global_value_hide