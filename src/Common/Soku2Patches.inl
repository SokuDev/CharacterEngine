//
// Created by PinkySmile on 05/06/2026.
//

#include "Soku2Patches.hpp"
#include "SokuLib.hpp"

inline Patch::Patch(const PatchSkeleton &skeleton) :
	location(skeleton.location),
	patchSize(skeleton.patchSize),
	trampoline(nullptr)
{}
inline Patch::Patch(Patch &o)
{
	std::swap(this->location, o.location);
	std::swap(this->patchSize, o.patchSize);
	std::swap(this->trampoline, o.trampoline);
}
inline Patch::Patch(Patch &&o) noexcept
{
	std::swap(this->location, o.location);
	std::swap(this->patchSize, o.patchSize);
	std::swap(this->trampoline, o.trampoline);
}
inline Patch &Patch::operator=(Patch &&o) noexcept
{
	std::swap(this->location, o.location);
	std::swap(this->patchSize, o.patchSize);
	std::swap(this->trampoline, o.trampoline);
	return *this;
}

AppliedPatch::AppliedPatch(const Patch *patch) :
	_patch(patch),
	_oldData(applyMemory.alloc(patch->patchSize))
{
	memcpy(this->_oldData, patch->location, patch->patchSize);
	memset(patch->location, 0x90, patch->patchSize);
	SokuLib::TamperNearJmp((DWORD)patch->location, patch->trampoline);
}
AppliedPatch::~AppliedPatch()
{
	DWORD old;

	memcpy(this->_patch->location, this->_oldData, this->_patch->patchSize);
	applyMemory.dealloc(this->_oldData);
}
AppliedPatch::AppliedPatch(AppliedPatch &o)
{
	std::swap(this->_patch, o._patch);
	std::swap(this->_oldData, o._oldData);
}
AppliedPatch::AppliedPatch(AppliedPatch &&o) noexcept
{
	std::swap(this->_patch, o._patch);
	std::swap(this->_oldData, o._oldData);
}
AppliedPatch &AppliedPatch::operator=(AppliedPatch &&o) noexcept
{
	std::swap(this->_patch, o._patch);
	std::swap(this->_oldData, o._oldData);
	return *this;
}