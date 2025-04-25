#include "pass.h"
#include "gv_hide.h"
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/SymbolTableListTraits.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>

using namespace llvm;

PreservedAnalyses GlobalValueHidePass::run(Module &M,
                                           ModuleAnalysisManager &MAM) {
  global_value_hide::GlobalValHideManager manager(M);
  manager.run();
  return PreservedAnalyses::all();
}

PassPluginLibraryInfo getGlobalValueHidePluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "GvHide", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            // Use this to support clang usage
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                  MPM.addPass(GlobalValueHidePass());
                });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "global-value-hide") {
                    MPM.addPass(GlobalValueHidePass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getGlobalValueHidePluginInfo();
}