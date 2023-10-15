#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct LICMPass : public PassInfoMixin<LICMPass> {
    PreservedAnalyses run (Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U) {
        errs() << "I saw a loop!\n";
        
        return PreservedAnalyses::all();
    };

    static bool isRequired() { return true; }
};
};


extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = LLVM_VERSION_STRING,
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerLateLoopOptimizationsEPCallback(
                [](llvm::LoopPassManager &LPM, OptimizationLevel Level) {
                  LPM.addPass(LICMPass());
                });
        }
    };
}
