#ifndef CHARACTERENGINE_SEKIBANKI_CODE_PATCHES_HPP
#define CHARACTERENGINE_SEKIBANKI_CODE_PATCHES_HPP


#include <vector>
#include "Soku2Patches.hpp"

typedef StackedAllocator<AppliedPatch, 17928> Allocator;
typedef StackedAllocator<Patch, 11710> TrampolineAllocator;
typedef StackedAllocator<Patch *, 11710> PatchListAllocator;
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


#endif //CHARACTERENGINE_SEKIBANKI_CODE_PATCHES_HPP
