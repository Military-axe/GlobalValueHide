#pragma once

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {

class GlobalValueHidePass : public PassInfoMixin<GlobalValueHidePass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM);
};

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo();

} // namespace llvm