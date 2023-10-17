#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include <queue>

using namespace llvm;
namespace
{
    struct LICMPass : public PassInfoMixin<LICMPass>
    {
        PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U)
        {
            DominatorTree &DT = AR.DT;
            for (auto &B : L.blocks())
            {
                std::queue<std::pair<Instruction *, BasicBlock *>> toBeHoisted;
                for (auto &I : *B)
                {
                    // TODO: hasLoopInvariantOperands only checks that all reaching defintions of arguments
                    // are outside of the loop and is too conservative. We also want to cover the second criterion "exactly one definition, and it is already marked as
                    // loop invariant"
                    if (!L.hasLoopInvariantOperands(&I))
                    {
                        continue;
                    }
                    if (!all_of(I.uses(), [&DT, &I](Use &U)
                                { return DT.dominates(&I, U); }))
                        continue;
                    SmallVector<BasicBlock *> exitBlocks;
                    L.getExitBlocks(exitBlocks);
                    if (!all_of(exitBlocks, [&DT, &I](BasicBlock *B)
                                { return DT.dominates(&I, B); }))
                        continue;

                    if (isSafeToSpeculativelyExecute(&I))
                    {
                        BasicBlock *preheader = L.getLoopPreheader();
                        auto *PI = &I;
                        toBeHoisted.push({PI, preheader});

                        errs() << "Hoisted instruction: " << *PI << "\n";
                    }
                }
                while (!toBeHoisted.empty())
                {
                    const auto &[I, preheader] = toBeHoisted.front();
                    toBeHoisted.pop();
                    I->removeFromParent();
                    I->insertInto(preheader, preheader->end());
                }
            }

            return PreservedAnalyses::none();
        };
        static bool isRequired() { return true; }
    };
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = LLVM_VERSION_STRING,
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB)
        {
            PB.registerScalarOptimizerLateEPCallback(
                [](FunctionPassManager &FPM, OptimizationLevel Level) {
                    FPM.addPass(PromotePass());
                    FPM.addPass(createFunctionToLoopPassAdaptor(LICMPass()));
                });

            // use the following code if you want to run the passes with opt    
 
            // PB.registerPipelineParsingCallback(
            //     [](StringRef Name, LoopPassManager &LPM,
            //        ArrayRef<PassBuilder::PipelineElement>)
            //     {
            //         if (Name == "licm-pass") {
            //             LPM.addPass(createFunctionToLoopPassAdaptor(LICMPass()));
            //             return true;
            //         }
            //         return false;
                    
            //     });
        }};
}