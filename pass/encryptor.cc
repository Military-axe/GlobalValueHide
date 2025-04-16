#include "encryptor.h"
#include "prelude.h"

namespace global_value_hide {

void GlobalValueEncryptor::enc(GlobalValues &gv, Funcs &func) {
  gv_ = Encryptor<llvm::GlobalVariable>(M_, gv);
  func_ = Encryptor<llvm::Function>(M_, func);
}

} // namespace global_value_hide