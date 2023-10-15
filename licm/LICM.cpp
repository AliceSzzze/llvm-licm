#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/SmallVector.h"
using namespace llvm;

namespace {
struct LICMPass : public PassInfoMixin<LICMPass> {
    PreservedAnalyses run (Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U) {
        DominatorTree& DT = AR.DT;
        errs() << "I saw a loop with loop depth " << L.getLoopDepth () << "!\n Potentially hoistable instructions:";
        for (auto &B : L.blocks()) {
            for (auto &I: *B) {
                // TODO: hasLoopInvariantOperands only checks that all reaching defintions of arguments
                // are outside of the loop and is too conservative. We also want to cover the second criterion "exactly one definition, and it is already marked as
                // loop invariant"

                if (!L.hasLoopInvariantOperands(&I)) {
                    continue;
                }

                // check that def dominates all uses
                if (!all_of(I.uses(), [&DT, &I](Use &U) { return DT.dominates(&I, U); })) continue;

                // get loop exits
                SmallVector<BasicBlock*> exitBlocks;
                L.getExitBlocks(exitBlocks);

                // TODO: if instruction does not dominate exits, we still want to try to move it to the preheader
                // because most instructions don't. See notes for how to relax this restriction
                // we can use isSafeToSpeculativelyExecute(&I) to check that there are no side effects

                // check that instruction dominates all loop exits
                if (!all_of(exitBlocks, [&DT, &I](BasicBlock* B) { return DT.dominates(&I, B); })) continue;
                
                // TODO: if all criteria are satisfied, we can move the instruction to the preheader 
                // (we can get the preheader via L.getLoopPreheader())

                errs() << I << "\n";
            }
        }
        
        
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
