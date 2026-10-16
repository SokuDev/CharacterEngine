#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<{{APPLY_ALLOC_SIZE}}> applyMemory;
StackedMemory<{{INTERNAL_ALLOC_SIZE}}> internalMemory;
Allocator applyAllocator{applyMemory};
PatchListAllocator patchListAllocator{internalMemory};
TrampolineAllocator trampolineAllocator{internalMemory};

#include "Soku2Patches.inl"


{{PATCH_BYTECODE}}

{{PATCH_SKELETONS}}

{{PATCH_SKELETONS_INDICES}}


std::vector<Patch, TrampolineAllocator> compiledPatches{trampolineAllocator};
std::vector<Patch *, PatchListAllocator> objectUpdate_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> objectInitializeAction_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> update_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> initializeAction_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> initialize_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> handleInputs_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> VUnknown58_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> VUnknown5C_patches{patchListAllocator};
std::vector<Patch *, PatchListAllocator> VUnknown60_patches{patchListAllocator};

static Patch compilePatch(const PatchSkeleton &skeleton)
{
	Patch result{skeleton};

	result.trampoline = internalMemory.alloc(skeleton.byteCode.byteCodeLength + 5);
	memcpy(result.trampoline, skeleton.byteCode.byteCode, skeleton.byteCode.byteCodeLength);
	for (int i = 0; i < skeleton.byteCode.jumpsSize; i++) {
		JumpTarget &jump = skeleton.byteCode.jumps[i];
		auto location = (unsigned *)&result.trampoline[jump.patchOffset];
		unsigned relative = jump.targetAddress - ((unsigned)location + 4);

		*location = relative;
	}
	SokuLib::TamperNearJmp((DWORD)&result.trampoline[skeleton.byteCode.byteCodeLength], (DWORD)result.location + result.patchSize);
	return result;
}

static void allocateTrampolines(std::vector<Patch, TrampolineAllocator> &result)
{
	result.reserve(std::size(patchList));
	for (auto &patch : patchList)
		result.push_back(compilePatch(patch));
}

template<typename T>
static void getPatchList(const T &skeletons, std::vector<Patch *, PatchListAllocator> &result)
{
	result.reserve(std::size(skeletons));
	for (auto &skeleton : skeletons)
		result.push_back(&compiledPatches[skeleton]);
}

void initPatches()
{
	DWORD old;

	allocateTrampolines(compiledPatches);
	getPatchList(skeletonsForObjectUpdate, objectUpdate_patches);
	getPatchList(skeletonsForObjectInitializeAction, objectInitializeAction_patches);
	getPatchList(skeletonsForUpdate, update_patches);
	getPatchList(skeletonsForInitializeAction, initializeAction_patches);
	getPatchList(skeletonsForInitialize, initialize_patches);
	getPatchList(skeletonsForHandleInputs, handleInputs_patches);
	getPatchList(skeletonsForVUnknown58, VUnknown58_patches);
	getPatchList(skeletonsForVUnknown5C, VUnknown5C_patches);
	getPatchList(skeletonsForVUnknown60, VUnknown60_patches);
	printf("Internal memory used %zu/%zu\n", internalMemory.getIndex(), internalMemory.size());

	VirtualProtect(internalMemory.getBufferStart(), internalMemory.size(), PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect(applyMemory.getBufferStart(), applyMemory.size(), PAGE_EXECUTE_READWRITE, &old);
}

void clearPatches()
{
	compiledPatches.clear();
	objectUpdate_patches.clear();
	objectInitializeAction_patches.clear();
	update_patches.clear();
	initializeAction_patches.clear();
	initialize_patches.clear();
	handleInputs_patches.clear();
	VUnknown58_patches.clear();
	VUnknown5C_patches.clear();
	VUnknown60_patches.clear();
}
