#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"
#include "llvm/IRPrinter/IRPrintingPasses.h"
#include "llvm/IR/Instruction.h"
#include <unordered_set>

using namespace llvm;
namespace
{
    struct LICMPass : public PassInfoMixin<LICMPass>
    {
        std::unordered_set<Value *> loopInvariants;

        bool isLIAndSafeToHoist(Loop &L, Instruction &I)
        {
            if (!isSafeToSpeculativelyExecute(&I) || I.mayReadOrWriteMemory())
                return false;

            if (all_of(I.operands(), [&L, this](Value *V)
                       // do we need the second condition?
                       { return L.isLoopInvariant(V) || loopInvariants.find(V) != loopInvariants.end(); }))
            {
                // errs() << "LOOP INV " << I << "\n";
                loopInvariants.insert(&I);
                return true;
            }

            return false;
        }

        PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM, LoopStandardAnalysisResults &AR, LPMUpdater &U)
        {

            bool changing;
            do
            {
                changing = false;
                for (auto &B : L.blocks())
                {
                    for (auto &I : *B)
                    {
                        if (!isLIAndSafeToHoist(L, I))
                        {
                            continue;
                        }

                        BasicBlock *preheader = L.getLoopPreheader();
                        auto *PI = &I;
                        Instruction *term = preheader->getTerminator();
                        I.moveBefore(term);
                        I.dropUnknownNonDebugMetadata();
                        changing = true;

                        // errs() << "Hoisted instruction: " << *PI << "\n";
                        break;
                    }
                }
            } while (changing);

            return PreservedAnalyses::none();
        };
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
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level)
                {
                    MPM.addPass(createModuleToFunctionPassAdaptor(PromotePass()));
                    // MPM.addPass(createModuleToFunctionPassAdaptor(LoopSimplifyPass()));
                    // MPM.addPass(PrintModulePass());
                    MPM.addPass(createModuleToFunctionPassAdaptor(createFunctionToLoopPassAdaptor(LICMPass())));
                    MPM.addPass(PrintModulePass());
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