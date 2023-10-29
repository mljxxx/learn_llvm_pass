#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

static llvm::cl::opt<bool> enable_add_to_sub("enable-add-to-sub", llvm::cl::desc("pass variable"), llvm::cl::value_desc("pass variable"));

namespace {
    using namespace llvm;
    struct ReplaceAdd : PassInfoMixin<ReplaceAdd> {
        PreservedAnalyses run(Function &F,FunctionAnalysisManager &) {
            if(enable_add_to_sub) {
                for(auto &BB : F) {
                    for(BasicBlock::iterator iterator = BB.begin();iterator!=BB.end();) {
                        Instruction& ins = *iterator;
                        errs() << ins.getOpcodeName() << "\n";
                        if (ins.isBinaryOp() && ins.getOpcode() == llvm::Instruction::Add) {
                            BinaryOperator *op = BinaryOperator::Create(llvm::Instruction::Sub, ins.getOperand(0), ins.getOperand(1),Twine(),&ins);
                            op->setHasNoSignedWrap(ins.hasNoUnsignedWrap());
                            op->setHasNoUnsignedWrap(ins.hasNoUnsignedWrap());
                            ins.replaceAllUsesWith(op);
                            iterator = ins.eraseFromParent();
                            continue;
                        }
                        iterator++;
                    }
                }
                return PreservedAnalyses::none();
            } else {
                return PreservedAnalyses::all();
            }
        }
        static bool isRequired() { 
            return true; 
        }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
      LLVM_PLUGIN_API_VERSION, "ReplaceAdd", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerOptimizerEarlyEPCallback([](ModulePassManager &MPM, OptimizationLevel level) {
                MPM.addPass(createModuleToFunctionPassAdaptor(ReplaceAdd()));
            });
            // PB.registerPipelineParsingCallback(
            //     [](StringRef Name, FunctionPassManager &FPM,
            //         ArrayRef<llvm::PassBuilder::PipelineElement>) -> bool {
            //         if(Name.equals("ReplaceAdd")) {
            //             FPM.addPass(ReplaceAdd());
            //             return true;
            //         }
            //         return false;
            // });
          }
    };
}