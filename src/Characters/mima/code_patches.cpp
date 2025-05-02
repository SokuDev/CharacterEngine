#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<5256> applyMemory;
StackedMemory<1428> internalMemory;
Allocator applyAllocator{applyMemory};
PatchListAllocator patchListAllocator{internalMemory};
TrampolineAllocator trampolineAllocator{internalMemory};

Patch::Patch(const PatchSkeleton &skeleton) :
	location(skeleton.location),
	patchSize(skeleton.patchSize),
	trampoline(nullptr)
{}

Patch::Patch(Patch &o)
{
	std::swap(this->location, o.location);
	std::swap(this->patchSize, o.patchSize);
	std::swap(this->trampoline, o.trampoline);
}
Patch::Patch(Patch &&o) noexcept
{
	std::swap(this->location, o.location);
	std::swap(this->patchSize, o.patchSize);
	std::swap(this->trampoline, o.trampoline);
}
Patch &Patch::operator=(Patch &&o) noexcept
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
	DWORD old;

	memcpy(this->_oldData, patch->location, patch->patchSize);
	VirtualProtect(patch->location, patch->patchSize, PAGE_EXECUTE_WRITECOPY, &old);
	memset(patch->location, 0x90, patch->patchSize);
	SokuLib::TamperNearJmp((DWORD)patch->location, patch->trampoline);
	VirtualProtect(patch->location, patch->patchSize, old, &old);
}

AppliedPatch::~AppliedPatch()
{
	DWORD old;

	VirtualProtect(this->_patch->location, this->_patch->patchSize, PAGE_EXECUTE_WRITECOPY, &old);
	memcpy(this->_patch->location, this->_oldData, this->_patch->patchSize);
	VirtualProtect(this->_patch->location, this->_patch->patchSize, old, &old);
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


// mimaFLDZ declared in mima.asm line 5
static unsigned char mimaFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 6
};
static PatchByteCode mimaFLDZ_patchByteCode = { mimaFLDZ_byteCode, 2, nullptr, 0 };
// mimaID_FIX declared in mima.asm line 10
static unsigned char mimaID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x0000001D ; line 11
	0x66, 0x39, 0x9E, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],BX ; line 13
};
static PatchByteCode mimaID_FIX_patchByteCode = { mimaID_FIX_byteCode, 17, nullptr, 0 };
// mimaD214_SOUND_ANDHIT declared in mima.asm line 17
static unsigned char mimaD214_SOUND_ANDHIT_byteCode[] = {
	0x6A, 0x05,                                                 // PUSH 0x05 ; line 21
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 22
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 23
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 24
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CE90 ; line 25
	0x84, 0xC0,                                                 // TEST AL,AL ; line 26
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x69EBDB ; line 27
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0xB4 ; line 43
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 44
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 45
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 46
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 47
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 50
};
static JumpTarget mimaD214_SOUND_ANDHIT_jumpData[] = {
	{ 0x009, 0x48CE90 },
	{ 0x011, 0x69EBDB },
	{ 0x02E, 0x464980 },
};
static PatchByteCode mimaD214_SOUND_ANDHIT_patchByteCode = { mimaD214_SOUND_ANDHIT_byteCode, 56, mimaD214_SOUND_ANDHIT_jumpData, 3 };
// mimaDIAL1 declared in mima.asm line 54
static unsigned char mimaDIAL1_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 55
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 56
	0x0F, 0x85, 0x1A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 57
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 58
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 59
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00,                   // FLD DWORD PTR [0x85B870] ; line 60
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 61
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF4] ; line 63
	0x66, 0x9D,                                           // POPF ; line 65
};
static PatchByteCode mimaDIAL1_patchByteCode = { mimaDIAL1_byteCode, 51, nullptr, 0 };
// mimaDIAL2 declared in mima.asm line 68
static unsigned char mimaDIAL2_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 69
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 70
	0x0F, 0x85, 0x1A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 71
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 72
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 73
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00,                   // FLD DWORD PTR [0x85B870] ; line 74
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 75
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF4] ; line 77
	0x66, 0x9D,                                           // POPF ; line 79
};
static PatchByteCode mimaDIAL2_patchByteCode = { mimaDIAL2_byteCode, 51, nullptr, 0 };
// mima5B_0A declared in mima.asm line 84
static unsigned char mima5B_0A_byteCode[] = {
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x86782C] ; line 85
};
static PatchByteCode mima5B_0A_patchByteCode = { mima5B_0A_byteCode, 6, nullptr, 0 };
// mima5B_0B declared in mima.asm line 87
static unsigned char mima5B_0B_byteCode[] = {
	0xDC, 0x25, 0x20, 0xBC, 0x85, 0x00, // FSUB QWORD PTR [0x85BC20] ; line 88
};
static PatchByteCode mima5B_0B_patchByteCode = { mima5B_0B_byteCode, 6, nullptr, 0 };
// mima5B_0C declared in mima.asm line 90
static unsigned char mima5B_0C_byteCode[] = {
	0xDC, 0x2D, 0x90, 0x89, 0x85, 0x00, // FSUBR QWORD PTR [0x858990] ; line 91
};
static PatchByteCode mima5B_0C_patchByteCode = { mima5B_0C_byteCode, 6, nullptr, 0 };
// mima5B_1A declared in mima.asm line 93
static unsigned char mima5B_1A_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 94
};
static PatchByteCode mima5B_1A_patchByteCode = { mima5B_1A_byteCode, 6, nullptr, 0 };
// mima5B_1B declared in mima.asm line 96
static unsigned char mima5B_1B_byteCode[] = {
	0xDD, 0x05, 0xF8, 0x81, 0x85, 0x00, // FLD QWORD PTR [0x8581F8] ; line 97
};
static PatchByteCode mima5B_1B_patchByteCode = { mima5B_1B_byteCode, 6, nullptr, 0 };
// mima5B_2A declared in mima.asm line 99
static unsigned char mima5B_2A_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x46EB30 ; line 100
	0x81, 0xC7, 0x04, 0x00, 0x00, 0x00, // ADD EDI,0x04 ; line 101
};
static JumpTarget mima5B_2A_jumpData[] = {
	{ 0x001, 0x46EB30 },
};
static PatchByteCode mima5B_2A_patchByteCode = { mima5B_2A_byteCode, 11, mima5B_2A_jumpData, 1 };
// mima5B_2B declared in mima.asm line 103
static unsigned char mima5B_2B_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x46EB30 ; line 104
	0x81, 0xC7, 0x03, 0x00, 0x00, 0x00, // ADD EDI,0x03 ; line 105
};
static JumpTarget mima5B_2B_jumpData[] = {
	{ 0x001, 0x46EB30 },
};
static PatchByteCode mima5B_2B_patchByteCode = { mima5B_2B_byteCode, 11, mima5B_2B_jumpData, 1 };
// mima5B_3A declared in mima.asm line 107
static unsigned char mima5B_3A_byteCode[] = {
	0x81, 0xFF, 0x18, 0x00, 0x00, 0x00, // CMP EDI,0x18 ; line 108
	0x89, 0x7C, 0x24, 0x18,             // MOV [ESP+0x18],EDI ; line 109
};
static PatchByteCode mima5B_3A_patchByteCode = { mima5B_3A_byteCode, 10, nullptr, 0 };
// mima5B_3B declared in mima.asm line 111
static unsigned char mima5B_3B_byteCode[] = {
	0x81, 0xFF, 0x18, 0x00, 0x00, 0x00, // CMP EDI,0x00000018 ; line 112
};
static PatchByteCode mima5B_3B_patchByteCode = { mima5B_3B_byteCode, 6, nullptr, 0 };
// mima6C declared in mima.asm line 186
static unsigned char mima6C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x68D85A ; line 187
};
static JumpTarget mima6C_jumpData[] = {
	{ 0x001, 0x68D85A },
};
static PatchByteCode mima6C_patchByteCode = { mima6C_byteCode, 5, mima6C_jumpData, 1 };
// mimaJ6C declared in mima.asm line 191
static unsigned char mimaJ6C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x68E34B ; line 192
};
static JumpTarget mimaJ6C_jumpData[] = {
	{ 0x001, 0x68E34B },
};
static PatchByteCode mimaJ6C_patchByteCode = { mimaJ6C_byteCode, 5, mimaJ6C_jumpData, 1 };
// mima3SCSPARK_DUR declared in mima.asm line 200
static unsigned char mima3SCSPARK_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000081 ; line 201
};
static PatchByteCode mima3SCSPARK_DUR_patchByteCode = { mima3SCSPARK_DUR_byteCode, 10, nullptr, 0 };


// mima.asm
std::array<PatchSkeleton, 45> patchList = {
	PatchSkeleton{ mimaID_FIX_patchByteCode,            (void *)0x687C14,  7}, // Declared line 15. Patch on CMP [ESI+0x196], BX
	PatchSkeleton{ mimaD214_SOUND_ANDHIT_patchByteCode, (void *)0x6A0249,  6}, // Declared line 52. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ mimaDIAL1_patchByteCode,             (void *)0x68C111,  6}, // Declared line 81. Patch on FLD ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ mimaDIAL2_patchByteCode,             (void *)0x68C1C0,  6}, // Declared line 82. Patch on FLD ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68C4CD,  6}, // Declared line 114. Patch on FSUBR ST, QWORD PTR [0x858990]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68C52D,  8}, // Declared line 115. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68C535,  7}, // Declared line 116. Patch on CMP EDI, 0x64
	PatchSkeleton{ mima5B_1A_patchByteCode,             (void *)0x68C49D,  6}, // Declared line 117. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68C5BB,  6}, // Declared line 120. Patch on FSUBR ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68C61B,  8}, // Declared line 121. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3B_patchByteCode,             (void *)0x68C623,  6}, // Declared line 122. Patch on CMP EDI, 0x87
	PatchSkeleton{ mima5B_1B_patchByteCode,             (void *)0x68C591,  6}, // Declared line 123. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68CA41,  6}, // Declared line 126. Patch on FSUBR ST, QWORD PTR [0x858010]
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68CAA1,  7}, // Declared line 128. Patch on CMP EDI, 0x24
	PatchSkeleton{ mima5B_1A_patchByteCode,             (void *)0x68CA17,  6}, // Declared line 129. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68CB2A,  6}, // Declared line 132. Patch on FSUBR ST, QWORD PTR [0x871520]
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68CB89,  7}, // Declared line 134. Patch on CMP EDI, 0x32
	PatchSkeleton{ mima5B_1B_patchByteCode,             (void *)0x68CAFD,  6}, // Declared line 135. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68CD4D,  6}, // Declared line 138. Patch on FSUBR ST, QWORD PTR [0x858990]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68CDA5,  8}, // Declared line 139. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68CDAD,  7}, // Declared line 140. Patch on CMP EDI, 0x64
	PatchSkeleton{ mima5B_1A_patchByteCode,             (void *)0x68CD23,  6}, // Declared line 141. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0A_patchByteCode,             (void *)0x68CE54,  6}, // Declared line 144. Patch on FSUBR ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ mima5B_2B_patchByteCode,             (void *)0x68CEAC,  8}, // Declared line 145. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3B_patchByteCode,             (void *)0x68CEB4,  6}, // Declared line 146. Patch on CMP EDI, 0x87
	PatchSkeleton{ mima5B_1B_patchByteCode,             (void *)0x68CE2A,  6}, // Declared line 147. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0B_patchByteCode,             (void *)0x68C77F,  6}, // Declared line 150. Patch on FSUBR ST, QWORD PTR [0x858990]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68C7DF,  8}, // Declared line 152. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68C7E7,  7}, // Declared line 154. Patch on CMP EDI, 0x64
	PatchSkeleton{ mima5B_1A_patchByteCode,             (void *)0x68C755,  6}, // Declared line 156. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0B_patchByteCode,             (void *)0x68C86D,  6}, // Declared line 159. Patch on FSUBR ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68C8CD,  8}, // Declared line 161. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3B_patchByteCode,             (void *)0x68C8D5,  6}, // Declared line 163. Patch on CMP EDI, 0x87
	PatchSkeleton{ mima5B_1B_patchByteCode,             (void *)0x68C843,  6}, // Declared line 165. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68D0F0,  8}, // Declared line 170. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3A_patchByteCode,             (void *)0x68D0F8,  7}, // Declared line 172. Patch on CMP EDI, 0x64
	PatchSkeleton{ mima5B_1A_patchByteCode,             (void *)0x68D068,  6}, // Declared line 174. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima5B_0C_patchByteCode,             (void *)0x68D19F,  6}, // Declared line 177. Patch on FSUBR ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ mima5B_2A_patchByteCode,             (void *)0x68D1F7,  8}, // Declared line 179. Patch on CALL 0x0046EB30
	PatchSkeleton{ mima5B_3B_patchByteCode,             (void *)0x68D1FF,  6}, // Declared line 181. Patch on CMP EDI, 0x87
	PatchSkeleton{ mima5B_1B_patchByteCode,             (void *)0x68D16B,  6}, // Declared line 183. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ mima6C_patchByteCode,                (void *)0x68D7F5,  6}, // Declared line 189. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ mimaJ6C_patchByteCode,               (void *)0x68E2E5,  6}, // Declared line 194. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ mimaFLDZ_patchByteCode,              (void *)0x6966DD,  6}, // Declared line 197. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ mima3SCSPARK_DUR_patchByteCode,      (void *)0x6A3DE7, 10}, // Declared line 203. Patch on CMP DWORD PTR [ESI+0x144], 0x12C
};


static std::array<unsigned, 43> skeletonsForUpdate = {
	   0,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
	  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
	  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
};
static std::vector<unsigned> skeletonsForInitializeAction;
static std::vector<unsigned> skeletonsForInitialize;
static std::vector<unsigned> skeletonsForHandleInputs;
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 2> skeletonsForObjectUpdate = { 1, 44 };
static std::vector<unsigned> skeletonsForObjectInitializeAction;



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
	assert(trampolineAllocator.getIndex() == 0);
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
	printf("Internal memory used %zu/1428\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 1428);
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
