#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

namespace {
    using namespace llvm;
    struct HelloWorld : PassInfoMixin<HelloWorld> {
        PreservedAnalyses run(Function &F,FunctionAnalysisManager &) {
            errs() << "Hello from: "<< F.getName() << "\n";
            errs() << "number of arguments: " << F.arg_size() << "\n";
            return PreservedAnalyses::all();
        }
        static bool isRequired() { 
            return true; 
        }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
      LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<llvm::PassBuilder::PipelineElement>) -> bool {
                    if(Name.equals("HelloWorld")) {
                        FPM.addPass(HelloWorld());
                        return true;
                    }
                    return false;
                });
          }
    };
}