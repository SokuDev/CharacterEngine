#ifndef CHARACTERENGINE_MURASA_CODE_PATCHES_HPP
#define CHARACTERENGINE_MURASA_CODE_PATCHES_HPP


#include <vector>
#include "Soku2Patches.hpp"

typedef StackedAllocator<AppliedPatch, 12216> Allocator;
typedef StackedAllocator<Patch, 16755> TrampolineAllocator;
typedef StackedAllocator<Patch *, 16755> PatchListAllocator;
extern Allocator applyAllocator;
extern std::vector<Patch, TrampolineAllocator> compiledPatches;
extern std::vector<Patch *, PatchListAllocator> objectUpdate_patches;
extern std::vector<Patch *, PatchListAllocator> objectInitializeAction_patches;
extern std::vector<Patch *, PatchListAllocator> update_patches;
extern std::vector<Patch *, PatchListAllocator> initializeAction_patches;
extern std::vector<Patch *, PatchListAllocator> initialize_patches;
extern std::vector<Patch *, PatchListAllocator> handleInputs_patches;
extern std::vector<Patch *, PatchListAllocator> VUnknown58_patches;
extern std::vector<Patch *, PatchListAllocator> VUnknown5C_patches;
extern std::vector<Patch *, PatchListAllocator> VUnknown60_patches;

void initPatches();
void clearPatches();


#endif //CHARACTERENGINE_MURASA_CODE_PATCHES_HPP
