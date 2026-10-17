#ifndef CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP
#define CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP


#include <vector>
#include "Soku2Patches.hpp"

typedef StackedAllocator<AppliedPatch, {{APPLY_ALLOC_SIZE}}> Allocator;
typedef StackedAllocator<Patch, {{INTERNAL_ALLOC_SIZE}}> TrampolineAllocator;
typedef StackedAllocator<Patch *, {{INTERNAL_ALLOC_SIZE}}> PatchListAllocator;
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


#endif //CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP
