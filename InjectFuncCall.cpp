#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>

namespace {
    using namespace llvm;
    struct InjectFuncCall : PassInfoMixin<InjectFuncCall> {
        PreservedAnalyses run(Module &M,ModuleAnalysisManager &) {
            LLVMContext& context = M.getContext();
            PointerType *returnType = IntegerType::getInt8PtrTy(context);
            PointerType *paramType = IntegerType::getInt8PtrTy(context);
            FunctionType *funcType = FunctionType::get(returnType,paramType,false);
            FunctionCallee funcCallee = M.getOrInsertFunction("inject_func", funcType);
            for(auto &F : M) {
                if(F.isDeclaration() || F.getName().equals("inject_func")) {
                    continue;
                }
                IRBuilder<> builder(&*F.getEntryBlock().getFirstInsertionPt());
                auto funcName = builder.CreateGlobalStringPtr(F.getName());
                builder.CreateCall(funcCallee,funcName);
            }
            return PreservedAnalyses::none();
        }
        static bool isRequired() { 
            return true;
        }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
      LLVM_PLUGIN_API_VERSION, "InjectFuncCall", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerOptimizerEarlyEPCallback([](ModulePassManager &MPM, OptimizationLevel level) {
                MPM.addPass(InjectFuncCall());
            });
          }
    };
}