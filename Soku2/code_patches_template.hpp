#ifndef CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP
#define CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP


#include <vector>

struct JumpTarget {
	size_t patchOffset;
	size_t targetAddress;
};

struct PatchByteCode {
	unsigned char *byteCode;
	size_t byteCodeLength;
	JumpTarget *jumps;
	size_t jumpsSize;
};

struct PatchSkeleton {
	PatchByteCode &byteCode;
	void *location;
	size_t patchSize;
};

struct Patch {
	void *location = nullptr;
	size_t patchSize = 0;
	unsigned char *trampoline = nullptr;

	Patch(const PatchSkeleton &skeleton);
	~Patch();

	Patch(Patch &o);
	Patch(Patch &&o) noexcept;
	Patch &operator=(Patch &&o) noexcept;
};

class AppliedPatch {
private:
	const Patch *_patch = nullptr;
	unsigned char *_oldData = nullptr;

public:
	AppliedPatch(const Patch *patch);
	~AppliedPatch();

	AppliedPatch(AppliedPatch &o);
	AppliedPatch(AppliedPatch &&o) noexcept;
	AppliedPatch &operator=(AppliedPatch &&o) noexcept;
};

extern std::vector<Patch> compiledPatches;
extern std::vector<Patch *> objectUpdate_patches;
extern std::vector<Patch *> objectInitializeAction_patches;
extern std::vector<Patch *> update_patches;
extern std::vector<Patch *> initializeAction_patches;
extern std::vector<Patch *> initialize_patches;
extern std::vector<Patch *> handleInputs_patches;
extern std::vector<Patch *> VUnknown58_patches;
extern std::vector<Patch *> VUnknown5C_patches;
extern std::vector<Patch *> VUnknown60_patches;

void initPatches();
void clearPatches();


#endif //CHARACTERENGINE_{{CLASS_NAME}}_CODE_PATCHES_HPP
