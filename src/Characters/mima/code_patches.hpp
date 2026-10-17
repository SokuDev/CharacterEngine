#ifndef CHARACTERENGINE_MIMA_CODE_PATCHES_HPP
#define CHARACTERENGINE_MIMA_CODE_PATCHES_HPP


#include <vector>
#include "Soku2Patches.hpp"

typedef StackedAllocator<AppliedPatch, 5256> Allocator;
typedef StackedAllocator<Patch, 1344> TrampolineAllocator;
typedef StackedAllocator<Patch *, 1344> PatchListAllocator;
extern Allocator applyAllocator;
extern std::vector<Patch, TrampolineAllocator> compiledPatches;
extern std::vector<Patch *, PatchListAllocator> objectUpdate_patches;
extern std::vector<Patch *, PatchListAllocator> objectInitializeAction_patches;
extern std::vector<Patch *, PatchListAllocator> update_patches;
extern std::vector<Patch *, PatchListAllocator> initializeAction_patches;
extern std::vector<Patch *, PatchListAllocator> initialize_patches;
extern std::vector<Patch *, PatchListAllocator> handleInputs_patches;
extern std::vector<Patch *, PatchListAllocator> computerInputs_patches;
extern std::vector<Patch *, PatchListAllocator> updateStory_patches;
extern std::vector<Patch *, PatchListAllocator> setCustomScenarioAction_patches;

void initPatches();
void clearPatches();


#endif //CHARACTERENGINE_MIMA_CODE_PATCHES_HPP
