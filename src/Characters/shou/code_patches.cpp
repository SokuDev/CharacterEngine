#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<29960> applyMemory;
StackedMemory<11602> internalMemory;
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


// shouFLDZ declared in shou.asm line 9
static unsigned char shouFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 10
};
static PatchByteCode shouFLDZ_patchByteCode = { shouFLDZ_byteCode, 2, nullptr, 0 };
// shouNOP declared in shou.asm line 12
static unsigned char shouNOP_byteCode[] = {
	0x90, // NOP ; line 13
};
static PatchByteCode shouNOP_patchByteCode = { shouNOP_byteCode, 1, nullptr, 0 };
// shouID_FIX declared in shou.asm line 16
static unsigned char shouID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x0000001E ; line 17
	0x66, 0x39, 0xBE, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],DI ; line 19
};
static PatchByteCode shouID_FIX_patchByteCode = { shouID_FIX_byteCode, 17, nullptr, 0 };
// shouWALK declared in shou.asm line 24
static unsigned char shouWALK_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 25
};
static PatchByteCode shouWALK_patchByteCode = { shouWALK_byteCode, 6, nullptr, 0 };
// shouHJ8Y declared in shou.asm line 35
static unsigned char shouHJ8Y_byteCode[] = {
	0xDD, 0x05, 0xA0, 0xBB, 0x85, 0x00, // FLD QWORD PTR [0x85BBA0] ; line 36
};
static PatchByteCode shouHJ8Y_patchByteCode = { shouHJ8Y_byteCode, 6, nullptr, 0 };
// shouHJ9Y declared in shou.asm line 40
static unsigned char shouHJ9Y_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 41
};
static PatchByteCode shouHJ9Y_patchByteCode = { shouHJ9Y_byteCode, 6, nullptr, 0 };
// shouHJ8GRAV declared in shou.asm line 46
static unsigned char shouHJ8GRAV_byteCode[] = {
	0xD9, 0x05, 0xEC, 0xAC, 0x85, 0x00, // FLD DWORD PTR [0x85ACEC] ; line 47
};
static PatchByteCode shouHJ8GRAV_patchByteCode = { shouHJ8GRAV_byteCode, 6, nullptr, 0 };
// shouHJMAXFALL declared in shou.asm line 51
static unsigned char shouHJMAXFALL_byteCode[] = {
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 52
};
static PatchByteCode shouHJMAXFALL_patchByteCode = { shouHJMAXFALL_byteCode, 6, nullptr, 0 };
// shouFLIGHT_INIT declared in shou.asm line 65
static unsigned char shouFLIGHT_INIT_byteCode[] = {
	0xDD, 0x05, 0x90, 0x89, 0x85, 0x00, // FLD QWORD PTR [0x858990] ; line 66
};
static PatchByteCode shouFLIGHT_INIT_patchByteCode = { shouFLIGHT_INIT_byteCode, 6, nullptr, 0 };
// shouFLIGHT_ACC declared in shou.asm line 70
static unsigned char shouFLIGHT_ACC_byteCode[] = {
	0xD8, 0x25, 0xA4, 0xBC, 0x85, 0x00, // FSUB DWORD PTR [0x85BCA4] ; line 71
};
static PatchByteCode shouFLIGHT_ACC_patchByteCode = { shouFLIGHT_ACC_byteCode, 6, nullptr, 0 };
// shouFLIGHT_FIN declared in shou.asm line 75
static unsigned char shouFLIGHT_FIN_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 76
};
static PatchByteCode shouFLIGHT_FIN_patchByteCode = { shouFLIGHT_FIN_byteCode, 6, nullptr, 0 };
// shouFLIGHT_ETC declared in shou.asm line 80
static unsigned char shouFLIGHT_ETC_byteCode[] = {
	0xF6, 0xC4, 0x05,                   // TEST AH,0x05 ; line 81
	0x0F, 0x8B, 0xDE, 0xAD, 0xBE, 0xEF, // JNP 0x6AD4BB ; line 82
};
static JumpTarget shouFLIGHT_ETC_jumpData[] = {
	{ 0x005, 0x6AD4BB },
};
static PatchByteCode shouFLIGHT_ETC_patchByteCode = { shouFLIGHT_ETC_byteCode, 9, shouFLIGHT_ETC_jumpData, 1 };
// shouGROUNDDASH_SPD declared in shou.asm line 87
static unsigned char shouGROUNDDASH_SPD_byteCode[] = {
	0xDD, 0x05, 0x90, 0x89, 0x85, 0x00, // FLD QWORD PTR [0x858990] ; line 88
};
static PatchByteCode shouGROUNDDASH_SPD_patchByteCode = { shouGROUNDDASH_SPD_byteCode, 6, nullptr, 0 };
// shouGROUNDDASH_ACC declared in shou.asm line 92
static unsigned char shouGROUNDDASH_ACC_byteCode[] = {
	0xD8, 0x25, 0xA4, 0xBC, 0x85, 0x00, // FSUB DWORD PTR [0x85BCA4] ; line 93
};
static PatchByteCode shouGROUNDDASH_ACC_patchByteCode = { shouGROUNDDASH_ACC_byteCode, 6, nullptr, 0 };
// shouGROUNDDASH_DUR declared in shou.asm line 97
static unsigned char shouGROUNDDASH_DUR_byteCode[] = {
	0x66, 0x81, 0xF8, 0x01, 0x00,       // CMP AX,0x0001 ; line 98
	0x0F, 0x8E, 0xDE, 0xAD, 0xBE, 0xEF, // JLE 0x6AC539 ; line 99
};
static JumpTarget shouGROUNDDASH_DUR_jumpData[] = {
	{ 0x007, 0x6AC539 },
};
static PatchByteCode shouGROUNDDASH_DUR_patchByteCode = { shouGROUNDDASH_DUR_byteCode, 11, shouGROUNDDASH_DUR_jumpData, 1 };
// shouBACKDASH_SPD declared in shou.asm line 104
static unsigned char shouBACKDASH_SPD_byteCode[] = {
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 105
};
static PatchByteCode shouBACKDASH_SPD_patchByteCode = { shouBACKDASH_SPD_byteCode, 6, nullptr, 0 };
// shouBACKDASH_ACC declared in shou.asm line 109
static unsigned char shouBACKDASH_ACC_byteCode[] = {
	0xD8, 0x05, 0x60, 0x78, 0x86, 0x00, // FADD DWORD PTR [0x867860] ; line 110
};
static PatchByteCode shouBACKDASH_ACC_patchByteCode = { shouBACKDASH_ACC_byteCode, 6, nullptr, 0 };
// shouBACKDASH_Y declared in shou.asm line 114
static unsigned char shouBACKDASH_Y_byteCode[] = {
	0xD9, 0x05, 0x5C, 0xF6, 0x85, 0x00, // FLD DWORD PTR [0x85F65C] ; line 115
};
static PatchByteCode shouBACKDASH_Y_patchByteCode = { shouBACKDASH_Y_byteCode, 6, nullptr, 0 };
// shou6A_X declared in shou.asm line 120
static unsigned char shou6A_X_byteCode[] = {
	0xDD, 0x05, 0x00, 0x9B, 0x85, 0x00, // FLD QWORD PTR [0x859B00] ; line 121
};
static PatchByteCode shou6A_X_patchByteCode = { shou6A_X_byteCode, 6, nullptr, 0 };
// shou6A_X1 declared in shou.asm line 124
static unsigned char shou6A_X1_byteCode[] = {
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00, // FSUB DWORD PTR [0x8589F0] ; line 125
};
static PatchByteCode shou6A_X1_patchByteCode = { shou6A_X1_byteCode, 6, nullptr, 0 };
// shouC6A_X declared in shou.asm line 129
static unsigned char shouC6A_X_byteCode[] = {
	0xDD, 0x05, 0x00, 0x9B, 0x85, 0x00, // FLD QWORD PTR [0x859B00] ; line 130
};
static PatchByteCode shouC6A_X_patchByteCode = { shouC6A_X_byteCode, 6, nullptr, 0 };
// shouC6A_X1 declared in shou.asm line 133
static unsigned char shouC6A_X1_byteCode[] = {
	0xDC, 0x25, 0xB8, 0x81, 0x85, 0x00, // FSUB QWORD PTR [0x8581B8] ; line 134
};
static PatchByteCode shouC6A_X1_patchByteCode = { shouC6A_X1_byteCode, 6, nullptr, 0 };
// shouC6A_POINT declared in shou.asm line 139
static unsigned char shouC6A_POINT_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 140
};
static PatchByteCode shouC6A_POINT_patchByteCode = { shouC6A_POINT_byteCode, 9, nullptr, 0 };
// shou66A_X declared in shou.asm line 145
static unsigned char shou66A_X_byteCode[] = {
	0xD9, 0x05, 0x00, 0x9F, 0x85, 0x00, // FLD DWORD PTR [0x859F00] ; line 146
};
static PatchByteCode shou66A_X_patchByteCode = { shou66A_X_byteCode, 6, nullptr, 0 };
// shou66A_1 declared in shou.asm line 149
static unsigned char shou66A_1_byteCode[] = {
	0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x140],0x03 ; line 150
};
static PatchByteCode shou66A_1_patchByteCode = { shou66A_1_byteCode, 10, nullptr, 0 };
// shou66B declared in shou.asm line 156
static unsigned char shou66B_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 157
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x6B08C5 ; line 158
};
static JumpTarget shou66B_jumpData[] = {
	{ 0x00B, 0x6B08C5 },
};
static PatchByteCode shou66B_patchByteCode = { shou66B_byteCode, 15, shou66B_jumpData, 1 };
// shou66C_X declared in shou.asm line 163
static unsigned char shou66C_X_byteCode[] = {
	0xDD, 0x05, 0x00, 0x9B, 0x85, 0x00, // FLD QWORD PTR [0x859B00] ; line 164
};
static PatchByteCode shou66C_X_patchByteCode = { shou66C_X_byteCode, 6, nullptr, 0 };
// shou66C_ACC declared in shou.asm line 167
static unsigned char shou66C_ACC_byteCode[] = {
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00, // FSUB DWORD PTR [0x8589F0] ; line 168
};
static PatchByteCode shou66C_ACC_patchByteCode = { shou66C_ACC_byteCode, 6, nullptr, 0 };
// shou66C_1 declared in shou.asm line 171
static unsigned char shou66C_1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 172
};
static PatchByteCode shou66C_1_patchByteCode = { shou66C_1_byteCode, 9, nullptr, 0 };
// shou3A_X declared in shou.asm line 177
static unsigned char shou3A_X_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 178
};
static PatchByteCode shou3A_X_patchByteCode = { shou3A_X_byteCode, 6, nullptr, 0 };
// shou3A_X1 declared in shou.asm line 181
static unsigned char shou3A_X1_byteCode[] = {
	0xD8, 0x25, 0xA8, 0x77, 0x86, 0x00, // FSUB DWORD PTR [0x8677A8] ; line 182
};
static PatchByteCode shou3A_X1_patchByteCode = { shou3A_X1_byteCode, 6, nullptr, 0 };
// shouC3A_DISABLE declared in shou.asm line 186
static unsigned char shouC3A_DISABLE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x22, 0x00, // CMP WORD PTR [ESI+0x140],0x22 ; line 187
};
static PatchByteCode shouC3A_DISABLE_patchByteCode = { shouC3A_DISABLE_byteCode, 9, nullptr, 0 };
// shouJ6A_RECOVGRAV declared in shou.asm line 192
static unsigned char shouJ6A_RECOVGRAV_byteCode[] = {
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 193
};
static PatchByteCode shouJ6A_RECOVGRAV_patchByteCode = { shouJ6A_RECOVGRAV_byteCode, 6, nullptr, 0 };
// shouJ6A_SOUND declared in shou.asm line 197
static unsigned char shouJ6A_SOUND_byteCode[] = {
	0xB9, 0xFF, 0x00, 0x00, 0x00, // MOV ECX,0x000000FF ; line 198
};
static PatchByteCode shouJ6A_SOUND_patchByteCode = { shouJ6A_SOUND_byteCode, 5, nullptr, 0 };
// shouJ8A_KEY declared in shou.asm line 203
static unsigned char shouJ8A_KEY_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x05 ; line 204
};
static PatchByteCode shouJ8A_KEY_patchByteCode = { shouJ8A_KEY_byteCode, 9, nullptr, 0 };
// shou5B_ANG1 declared in shou.asm line 211
static unsigned char shou5B_ANG1_byteCode[] = {
	0xD8, 0x1D, 0x18, 0xBA, 0x85, 0x00, // FCOMP DWORD PTR [0x85BA18] ; line 212
};
static PatchByteCode shou5B_ANG1_patchByteCode = { shou5B_ANG1_byteCode, 6, nullptr, 0 };
// shou5B_ANG2 declared in shou.asm line 214
static unsigned char shou5B_ANG2_byteCode[] = {
	0xD9, 0x05, 0x18, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BA18] ; line 215
};
static PatchByteCode shou5B_ANG2_patchByteCode = { shou5B_ANG2_byteCode, 6, nullptr, 0 };
// shou5B_ANG3 declared in shou.asm line 217
static unsigned char shou5B_ANG3_byteCode[] = {
	0xD8, 0x1D, 0x3C, 0x15, 0x87, 0x00, // FCOMP DWORD PTR [0x87153C] ; line 218
};
static PatchByteCode shou5B_ANG3_patchByteCode = { shou5B_ANG3_byteCode, 6, nullptr, 0 };
// shou5B_ANG4 declared in shou.asm line 220
static unsigned char shou5B_ANG4_byteCode[] = {
	0xD9, 0x05, 0x3C, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x87153C] ; line 221
};
static PatchByteCode shou5B_ANG4_patchByteCode = { shou5B_ANG4_byteCode, 6, nullptr, 0 };
// shou5B_PROJSPD declared in shou.asm line 223
static unsigned char shou5B_PROJSPD_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 224
};
static PatchByteCode shou5B_PROJSPD_patchByteCode = { shou5B_PROJSPD_byteCode, 6, nullptr, 0 };
// shou2B_BULLETTYPE declared in shou.asm line 235
static unsigned char shou2B_BULLETTYPE_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 236
};
static PatchByteCode shou2B_BULLETTYPE_patchByteCode = { shou2B_BULLETTYPE_byteCode, 6, nullptr, 0 };
// shou6B_SPD declared in shou.asm line 241
static unsigned char shou6B_SPD_byteCode[] = {
	0xDD, 0x05, 0x90, 0x89, 0x85, 0x00, // FLD QWORD PTR [0x858990] ; line 242
};
static PatchByteCode shou6B_SPD_patchByteCode = { shou6B_SPD_byteCode, 6, nullptr, 0 };
// shou6B_SPD1 declared in shou.asm line 244
static unsigned char shou6B_SPD1_byteCode[] = {
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00, // FADD DWORD PTR [0x86782C] ; line 245
};
static PatchByteCode shou6B_SPD1_patchByteCode = { shou6B_SPD1_byteCode, 6, nullptr, 0 };
// shouJ2B_SPD declared in shou.asm line 264
static unsigned char shouJ2B_SPD_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 265
};
static PatchByteCode shouJ2B_SPD_patchByteCode = { shouJ2B_SPD_byteCode, 6, nullptr, 0 };
// shouJ2B_ANG1 declared in shou.asm line 270
static unsigned char shouJ2B_ANG1_byteCode[] = {
	0xD9, 0x05, 0x3C, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x87153C] ; line 271
};
static PatchByteCode shouJ2B_ANG1_patchByteCode = { shouJ2B_ANG1_byteCode, 6, nullptr, 0 };
// shouJ2B_ANG2 declared in shou.asm line 273
static unsigned char shouJ2B_ANG2_byteCode[] = {
	0xD8, 0x1D, 0x3C, 0x15, 0x87, 0x00, // FCOMP DWORD PTR [0x87153C] ; line 274
};
static PatchByteCode shouJ2B_ANG2_patchByteCode = { shouJ2B_ANG2_byteCode, 6, nullptr, 0 };
// shouJ2B_BULLETTYPE declared in shou.asm line 280
static unsigned char shouJ2B_BULLETTYPE_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 281
};
static PatchByteCode shouJ2B_BULLETTYPE_patchByteCode = { shouJ2B_BULLETTYPE_byteCode, 6, nullptr, 0 };
// shouJ2B_GRAVPOINT declared in shou.asm line 285
static unsigned char shouJ2B_GRAVPOINT_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0D, 0x00, // CMP WORD PTR [ESI+0x140],0x0D ; line 286
};
static PatchByteCode shouJ2B_GRAVPOINT_patchByteCode = { shouJ2B_GRAVPOINT_byteCode, 9, nullptr, 0 };
// shouJC2B_KEY declared in shou.asm line 294
static unsigned char shouJC2B_KEY_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x33, 0x00, // CMP WORD PTR [ESI+0x140],0x33 ; line 295
};
static PatchByteCode shouJC2B_KEY_patchByteCode = { shouJC2B_KEY_byteCode, 9, nullptr, 0 };
// shou5C_MUL declared in shou.asm line 335
static unsigned char shou5C_MUL_byteCode[] = {
	0xD8, 0x0D, 0x30, 0x88, 0x85, 0x00, // FMUL DWORD PTR [0x858830] ; line 336
};
static PatchByteCode shou5C_MUL_patchByteCode = { shou5C_MUL_byteCode, 6, nullptr, 0 };
// shou5C_SKIPDEATH declared in shou.asm line 340
static unsigned char shou5C_SKIPDEATH_byteCode[] = {
	0xDD, 0x05, 0x80, 0x7F, 0x85, 0x00, // FLD QWORD PTR [0x857F80] ; line 341
};
static PatchByteCode shou5C_SKIPDEATH_patchByteCode = { shou5C_SKIPDEATH_byteCode, 6, nullptr, 0 };
// shouj2A_MOMENTUM declared in shou.asm line 346
static unsigned char shouj2A_MOMENTUM_byteCode[] = {
	0xD9, 0x05, 0x30, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB30] ; line 347
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF4] ; line 348
};
static PatchByteCode shouj2A_MOMENTUM_patchByteCode = { shouj2A_MOMENTUM_byteCode, 12, nullptr, 0 };
// shouD236_1 declared in shou.asm line 353
static unsigned char shouD236_1_byteCode[] = {
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BA68] ; line 354
};
static PatchByteCode shouD236_1_patchByteCode = { shouD236_1_byteCode, 6, nullptr, 0 };
// shouD236_2 declared in shou.asm line 369
static unsigned char shouD236_2_byteCode[] = {
	0xD9, 0x5C, 0x24, 0x14, // FSTP DWORD PTR [ESP+0x14] ; line 370
	0xD9, 0xEE,             // FLDZ ; line 371
};
static PatchByteCode shouD236_2_patchByteCode = { shouD236_2_byteCode, 6, nullptr, 0 };
// shouD236_3 declared in shou.asm line 377
static unsigned char shouD236_3_byteCode[] = {
	0xD9, 0x9C, 0x24, 0xA4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESP+0xA4] ; line 378
	0xD9, 0xEE,                               // FLDZ ; line 379
};
static PatchByteCode shouD236_3_patchByteCode = { shouD236_3_byteCode, 9, nullptr, 0 };
// shouD236_COLOR1 declared in shou.asm line 394
static unsigned char shouD236_COLOR1_byteCode[] = {
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x120] ; line 395
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6C8EE4 ; line 396
};
static JumpTarget shouD236_COLOR1_jumpData[] = {
	{ 0x007, 0x6C8EE4 },
};
static PatchByteCode shouD236_COLOR1_patchByteCode = { shouD236_COLOR1_byteCode, 11, shouD236_COLOR1_jumpData, 1 };
// shouD236_COLOR2 declared in shou.asm line 398
static unsigned char shouD236_COLOR2_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x6CB1E5 ; line 399
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6C8E21 ; line 400
};
static JumpTarget shouD236_COLOR2_jumpData[] = {
	{ 0x002, 0x6CB1E5 },
	{ 0x007, 0x6C8E21 },
};
static PatchByteCode shouD236_COLOR2_patchByteCode = { shouD236_COLOR2_byteCode, 11, shouD236_COLOR2_jumpData, 2 };
// shouD236_ONHIT declared in shou.asm line 405
static unsigned char shouD236_ONHIT_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 406
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 407
	0x8B, 0x80, 0x3C, 0x01, 0x00, 0x00,                         // MOV EAX,[EAX+0x13C] ; line 408
	0x66, 0x81, 0xF8, 0x1C, 0x02,                               // CMP AX,0x021C ; line 409
	0x0F, 0x8C, 0x11, 0x00, 0x00, 0x00,                         // JL SHORT LABEL2 ; line 410
	0x66, 0x81, 0xF8, 0x20, 0x02,                               // CMP AX,0x0220 ; line 411
	0x0F, 0x8D, 0x06, 0x00, 0x00, 0x00,                         // JNL SHORT LABEL2 ; line 412
	0x61,                                                       // POPAD ; line 415
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP SHORT LABELX ; line 416
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000004 ; line 418
	0xE9, 0xEB, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL1 ; line 419
	0x81, 0xBE, 0x9C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x39C],0x00 ; line 422
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C8D29 ; line 423
};
static JumpTarget shouD236_ONHIT_jumpData[] = {
	{ 0x043, 0x6C8D29 },
};
static PatchByteCode shouD236_ONHIT_patchByteCode = { shouD236_ONHIT_byteCode, 71, shouD236_ONHIT_jumpData, 1 };
// shouD623_KEY declared in shou.asm line 431
static unsigned char shouD623_KEY_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x17, 0x00, // CMP WORD PTR [ESI+0x140],0x17 ; line 432
};
static PatchByteCode shouD623_KEY_patchByteCode = { shouD623_KEY_byteCode, 9, nullptr, 0 };
// shouD623 declared in shou.asm line 438
static unsigned char shouD623_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 439
	0x0F, 0x85, 0x99, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 440
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x0003 ; line 441
	0x0F, 0x8C, 0x8A, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 442
	0xE9, 0x6A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 443
	0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x760],0x00000000 ; line 445
	0x0F, 0x85, 0x75, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 446
	0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x764],0x00000000 ; line 447
	0x0F, 0x85, 0x65, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 448
	0x60,                                                       // PUSHAD ; line 451
	0x66, 0x8B, 0x86, 0x40, 0x01, 0x00, 0x00,                   // MOV AX,[ESI+0x140] ; line 452
	0x0F, 0xB6, 0x8E, 0xA7, 0x06, 0x00, 0x00,                   // MOVZX ECX, BYTE PTR [ESI+0x6A7] ; line 453
	0x00, 0xC9,                                                 // ADD CL,CL ; line 454
	0x66, 0x0F, 0xB6, 0xC9,                                     // MOVZX CX,CL ; line 455
	0x66, 0x01, 0xC8,                                           // ADD AX,CX ; line 456
	0x66, 0x81, 0xC0, 0x06, 0x00,                               // ADD AX,0x0006 ; line 457
	0xDD, 0x05, 0xB8, 0x81, 0x85, 0x00,                         // FLD QWORD PTR [0x8581B8] ; line 458
	0x66, 0x50,                                                 // PUSH AX ; line 459
	0xDF, 0x04, 0x24,                                           // FILD WORD PTR [ESP] ; line 460
	0xDE, 0xC9,                                                 // FMULP ST1 ; line 461
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 462
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x16, 0x00,       // MOV WORD PTR [ESI+0x140],0x0016 ; line 463
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x142],0x0001 ; line 464
	0x66, 0x58,                                                 // POP AX ; line 465
	0x61,                                                       // POPAD ; line 466
	0xE9, 0x1B, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 467
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x15, 0x00,       // CMP WORD PTR [ESI+0x140],0x0015 ; line 470
	0x0F, 0x8C, 0x87, 0xFF, 0xFF, 0xFF,                         // JL SHORT LABEL3 ; line 471
	0x0F, 0x84, 0xA1, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 472
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                         // JG 0x6B4611 ; line 473
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B4611 ; line 476
};
static JumpTarget shouD623_jumpData[] = {
	{ 0x0A4, 0x6B4611 },
	{ 0x0A9, 0x6B4611 },
};
static PatchByteCode shouD623_patchByteCode = { shouD623_byteCode, 173, shouD623_jumpData, 2 };
// shouD623_SKIP1 declared in shou.asm line 481
static unsigned char shouD623_SKIP1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B447E ; line 482
};
static JumpTarget shouD623_SKIP1_jumpData[] = {
	{ 0x001, 0x6B447E },
};
static PatchByteCode shouD623_SKIP1_patchByteCode = { shouD623_SKIP1_byteCode, 5, shouD623_SKIP1_jumpData, 1 };
// shouD623_SKIP2 declared in shou.asm line 484
static unsigned char shouD623_SKIP2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B48DE ; line 485
};
static JumpTarget shouD623_SKIP2_jumpData[] = {
	{ 0x001, 0x6B48DE },
};
static PatchByteCode shouD623_SKIP2_patchByteCode = { shouD623_SKIP2_byteCode, 5, shouD623_SKIP2_jumpData, 1 };
// shouD623_DECEL declared in shou.asm line 489
static unsigned char shouD623_DECEL_byteCode[] = {
	0xD8, 0x25, 0xA4, 0xBC, 0x85, 0x00, // FSUB DWORD PTR [0x85BCA4] ; line 490
};
static PatchByteCode shouD623_DECEL_patchByteCode = { shouD623_DECEL_byteCode, 6, nullptr, 0 };
// shou5C_GRAZERES declared in shou.asm line 507
static unsigned char shou5C_GRAZERES_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 508
	0x89, 0xF1,                   // MOV ECX,ESI ; line 509
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CDE0 ; line 510
	0x6A, 0x03,                   // PUSH 0x03 ; line 513
	0x89, 0xF1,                   // MOV ECX,ESI ; line 514
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CE30 ; line 515
};
static JumpTarget shou5C_GRAZERES_jumpData[] = {
	{ 0x005, 0x48CDE0 },
	{ 0x00E, 0x48CE30 },
};
static PatchByteCode shou5C_GRAZERES_patchByteCode = { shou5C_GRAZERES_byteCode, 18, shou5C_GRAZERES_jumpData, 2 };
// shou2C_ANG1 declared in shou.asm line 520
static unsigned char shou2C_ANG1_byteCode[] = {
	0xD8, 0x0D, 0x8C, 0xB9, 0x85, 0x00, // FMUL DWORD PTR [0x85B98C] ; line 521
};
static PatchByteCode shou2C_ANG1_patchByteCode = { shou2C_ANG1_byteCode, 6, nullptr, 0 };
// shou2C_ANG2 declared in shou.asm line 523
static unsigned char shou2C_ANG2_byteCode[] = {
	0xD8, 0x05, 0x30, 0xBB, 0x85, 0x00, // FADD DWORD PTR [0x85BB30] ; line 524
};
static PatchByteCode shou2C_ANG2_patchByteCode = { shou2C_ANG2_byteCode, 6, nullptr, 0 };
// shou2C_ANG3 declared in shou.asm line 526
static unsigned char shou2C_ANG3_byteCode[] = {
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867810] ; line 527
};
static PatchByteCode shou2C_ANG3_patchByteCode = { shou2C_ANG3_byteCode, 6, nullptr, 0 };
// shouD22_LV declared in shou.asm line 540
static unsigned char shouD22_LV_byteCode[] = {
	0x80, 0xB8, 0xA5, 0x06, 0x00, 0x00, 0x06, // CMP BYTE PTR [EAX+0x6A5],0x06 ; line 541
};
static PatchByteCode shouD22_LV_patchByteCode = { shouD22_LV_byteCode, 7, nullptr, 0 };
// shouD22_1 declared in shou.asm line 545
static unsigned char shouD22_1_byteCode[] = {
	0xD9, 0xEE,                               // FLDZ ; line 546
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 547
	0x60,                                     // PUSHAD ; line 548
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x144] ; line 549
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,       // MOV ECX,[ESI+0x398] ; line 550
	0x0F, 0xB6, 0x89, 0xA5, 0x06, 0x00, 0x00, // MOVZX ECX,BYTE PTR [ECX+0x6A5] ; line 551
	0x81, 0xC1, 0x02, 0x00, 0x00, 0x00,       // ADD ECX,0x02 ; line 552
	0x6B, 0xC9, 0x60,                         // IMUL ECX,ECX,0x60 ; line 553
	0x39, 0xC8,                               // CMP EAX,ECX ; line 554
	0x61,                                     // POPAD ; line 555
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,       // JG 0x6C5A53 ; line 556
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6C5D28 ; line 557
};
static JumpTarget shouD22_1_jumpData[] = {
	{ 0x02A, 0x6C5A53 },
	{ 0x02F, 0x6C5D28 },
};
static PatchByteCode shouD22_1_patchByteCode = { shouD22_1_byteCode, 51, shouD22_1_jumpData, 2 };
// shouD22_2 declared in shou.asm line 563
static unsigned char shouD22_2_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 564
	0x60,                                                 // PUSHAD ; line 565
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                   // MOV EBX,[ESI+0x398] ; line 566
	0xD9, 0x83, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xF0] ; line 567
	0xD9, 0xE8,                                           // FLD1 ; line 568
	0xDE, 0xD9,                                           // FCOMPP ; line 569
	0xDF, 0xE0,                                           // FNSTSW AX ; line 570
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 571
	0x0F, 0x85, 0x4F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 572
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xEC] ; line 573
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 574
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FADD QWORD PTR [0x867830] ; line 575
	0xDE, 0xD9,                                           // FCOMPP ; line 576
	0xDF, 0xE0,                                           // FNSTSW AX ; line 577
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 578
	0x0F, 0x85, 0x30, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 579
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 580
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xEC] ; line 581
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FADD QWORD PTR [0x867830] ; line 582
	0xDE, 0xD9,                                           // FCOMPP ; line 583
	0xDF, 0xE0,                                           // FNSTSW AX ; line 584
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 585
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 586
	0x66, 0xC7, 0x83, 0xE4, 0x05, 0x00, 0x00, 0xF4, 0x01, // MOV WORD PTR [EBX+0x5E4],0x01F4 ; line 587
	0x61,                                                 // POPAD ; line 588
	0x66, 0x9D,                                           // POPF ; line 589
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6C5A53 ; line 590
	0x61,                                                 // POPAD ; line 592
	0x66, 0x9D,                                           // POPF ; line 593
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6C5B94 ; line 594
};
static JumpTarget shouD22_2_jumpData[] = {
	{ 0x069, 0x6C5A53 },
	{ 0x071, 0x6C5B94 },
};
static PatchByteCode shouD22_2_patchByteCode = { shouD22_2_byteCode, 117, shouD22_2_jumpData, 2 };
// shouD22_BROCKBACK declared in shou.asm line 602
static unsigned char shouD22_BROCKBACK_byteCode[] = {
	0x69, 0xD2, 0x80, 0x02, 0x00, 0x00, // IMUL EDX,EDX,0x00000280 ; line 603
};
static PatchByteCode shouD22_BROCKBACK_patchByteCode = { shouD22_BROCKBACK_byteCode, 6, nullptr, 0 };
// shouD22_CCAST declared in shou.asm line 605
static unsigned char shouD22_CCAST_byteCode[] = {
	0x69, 0xD2, 0x20, 0x09, 0x00, 0x00, // IMUL EDX,EDX,0x00000920 ; line 606
};
static PatchByteCode shouD22_CCAST_patchByteCode = { shouD22_CCAST_byteCode, 6, nullptr, 0 };
// shouD22_CCASTIDK declared in shou.asm line 608
static unsigned char shouD22_CCASTIDK_byteCode[] = {
	0x69, 0xD2, 0xFA, 0x00, 0x00, 0x00, // IMUL EDX,EDX,0x000000FA ; line 609
};
static PatchByteCode shouD22_CCASTIDK_patchByteCode = { shouD22_CCASTIDK_byteCode, 6, nullptr, 0 };
// shouD214_DUR declared in shou.asm line 630
static unsigned char shouD214_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x3C ; line 631
};
static PatchByteCode shouD214_DUR_patchByteCode = { shouD214_DUR_byteCode, 10, nullptr, 0 };
// shouD214_ANG declared in shou.asm line 635
static unsigned char shouD214_ANG_byteCode[] = {
	0xD8, 0x25, 0x84, 0xB9, 0x85, 0x00, // FSUB DWORD PTR [0x85B984] ; line 636
};
static PatchByteCode shouD214_ANG_patchByteCode = { shouD214_ANG_byteCode, 6, nullptr, 0 };
// shouD214_PERIOD declared in shou.asm line 640
static unsigned char shouD214_PERIOD_byteCode[] = {
	0xB9, 0x0A, 0x00, 0x00, 0x00, // MOV ECX,0x0000000A ; line 641
};
static PatchByteCode shouD214_PERIOD_patchByteCode = { shouD214_PERIOD_byteCode, 5, nullptr, 0 };
// shouD214_DURLV declared in shou.asm line 646
static unsigned char shouD214_DURLV_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 647
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6C4D41 ; line 648
	0x6A, 0x14,                                                 // PUSH 0x14 ; line 649
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x4099F0 ; line 650
	0x81, 0xC4, 0x04, 0x00, 0x00, 0x00,                         // ADD ESP,0x04 ; line 652
	0x85, 0xC0,                                                 // TEST EAX,EAX ; line 653
	0x89, 0x44, 0x24, 0x10,                                     // MOV [ESP+0x10],EAX ; line 654
	0xD9, 0xEE,                                                 // FLDZ ; line 655
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C4AA5 ; line 656
};
static JumpTarget shouD214_DURLV_jumpData[] = {
	{ 0x00C, 0x6C4D41 },
	{ 0x013, 0x4099F0 },
	{ 0x026, 0x6C4AA5 },
};
static PatchByteCode shouD214_DURLV_patchByteCode = { shouD214_DURLV_byteCode, 42, shouD214_DURLV_jumpData, 3 };
// shouD214_LV1 declared in shou.asm line 660
static unsigned char shouD214_LV1_byteCode[] = {
	0xB9, 0x19, 0x00, 0x00, 0x00, // MOV ECX,0x00000019 ; line 661
};
static PatchByteCode shouD214_LV1_patchByteCode = { shouD214_LV1_byteCode, 5, nullptr, 0 };
// shouD214_LV2 declared in shou.asm line 663
static unsigned char shouD214_LV2_byteCode[] = {
	0xB9, 0x23, 0x00, 0x00, 0x00, // MOV ECX,0x00000023 ; line 664
};
static PatchByteCode shouD214_LV2_patchByteCode = { shouD214_LV2_byteCode, 5, nullptr, 0 };
// shouA214BOOM_SKIP1 declared in shou.asm line 672
static unsigned char shouA214BOOM_SKIP1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C5951 ; line 673
};
static JumpTarget shouA214BOOM_SKIP1_jumpData[] = {
	{ 0x001, 0x6C5951 },
};
static PatchByteCode shouA214BOOM_SKIP1_patchByteCode = { shouA214BOOM_SKIP1_byteCode, 5, shouA214BOOM_SKIP1_jumpData, 1 };
// shouA214BOOM_BOX declared in shou.asm line 677
static unsigned char shouA214BOOM_BOX_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 678
	0x0F, 0x85, 0x21, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 679
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 680
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 681
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 682
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 683
	0x39, 0xBE, 0x44, 0x01, 0x00, 0x00,                         // CMP [ESI+0x144],EDI ; line 685
};
static PatchByteCode shouA214BOOM_BOX_patchByteCode = { shouA214BOOM_BOX_byteCode, 54, nullptr, 0 };
// shou3SCWEATHER_1 declared in shou.asm line 693
static unsigned char shou3SCWEATHER_1_byteCode[] = {
	0xDF, 0x86, 0x44, 0x01, 0x00, 0x00, // FILD WORD PTR [ESI+0x144] ; line 694
	0xD8, 0xC8,                         // FMUL ST0,ST0 ; line 695
	0xDD, 0x05, 0x70, 0xEB, 0x85, 0x00, // FLD QWORD PTR [0x85EB70] ; line 696
	0xD8, 0xC8,                         // FMUL ST0,ST0 ; line 697
	0xDC, 0xC9,                         // FMUL ST1,ST0 ; line 698
	0xDE, 0xC9,                         // FMULP ST1 ; line 699
	0xDE, 0xC9,                         // FMULP ST1 ; line 700
	0xD8, 0x86, 0xEC, 0x00, 0x00, 0x00, // FADD DWORD PTR [ESI+0xEC] ; line 701
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xEC] ; line 702
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 703
	0xD9, 0x54, 0x24, 0x18,             // FST DWORD PTR [ESP+0x18] ; line 704
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6CD928 ; line 705
};
static JumpTarget shou3SCWEATHER_1_jumpData[] = {
	{ 0x02D, 0x6CD928 },
};
static PatchByteCode shou3SCWEATHER_1_patchByteCode = { shou3SCWEATHER_1_byteCode, 49, shou3SCWEATHER_1_jumpData, 1 };
// shou3SCWEATHER_2 declared in shou.asm line 712
static unsigned char shou3SCWEATHER_2_byteCode[] = {
	0xD9, 0x86, 0x2C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x12C] ; line 713
	0xD8, 0x25, 0x78, 0xBC, 0x85, 0x00,                         // FSUB DWORD PTR [0x85BC78] ; line 714
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 715
	0xD9, 0xE8,                                                 // FLD1 ; line 716
	0xD9, 0x96, 0x1C, 0x01, 0x00, 0x00,                         // FST DWORD PTR [ESI+0x11C] ; line 717
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 718
	0x66, 0x9C,                                                 // PUSHF ; line 719
	0x60,                                                       // PUSHAD ; line 720
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x144] ; line 721
	0xBA, 0x00, 0x00, 0x00, 0x00,                               // MOV EDX,0x00000000 ; line 722
	0xBB, 0x04, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000004 ; line 723
	0xF7, 0xF3,                                                 // DIV EBX ; line 724
	0x81, 0xFA, 0x00, 0x00, 0x00, 0x00,                         // CMP EDX,0x00000000 ; line 725
	0x0F, 0x85, 0x1F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 726
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000000 ; line 727
	0x0F, 0x84, 0x0F, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 728
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 730
	0xE9, 0x17, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 731
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 733
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 734
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x1E,                   // MOV BYTE PTR [ESI+0x194],0x1E ; line 735
	0x61,                                                       // POPAD ; line 738
	0x66, 0x9D,                                                 // POPF ; line 739
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x67, 0x01,       // CMP WORD PTR [ESI+0x140],0x0167 ; line 740
	0x0F, 0x8C, 0x09, 0x00, 0x00, 0x00,                         // JL SHORT LABEL3 ; line 741
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x77, 0x00,       // MOV WORD PTR [ESI+0x140],0x0077 ; line 742
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 744
};
static PatchByteCode shou3SCWEATHER_2_patchByteCode = { shou3SCWEATHER_2_byteCode, 153, nullptr, 0 };
// shou3SCWEATHER_3 declared in shou.asm line 747
static unsigned char shou3SCWEATHER_3_byteCode[] = {
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x142],0x0001 ; line 748
};
static PatchByteCode shou3SCWEATHER_3_patchByteCode = { shou3SCWEATHER_3_byteCode, 9, nullptr, 0 };
// shou3SCWEATHER_ONHIT declared in shou.asm line 757
static unsigned char shou3SCWEATHER_ONHIT_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6CD7CC ; line 758
};
static JumpTarget shou3SCWEATHER_ONHIT_jumpData[] = {
	{ 0x001, 0x6CD7CC },
};
static PatchByteCode shou3SCWEATHER_ONHIT_patchByteCode = { shou3SCWEATHER_ONHIT_byteCode, 5, shou3SCWEATHER_ONHIT_jumpData, 1 };
// shou3SCWEATHER_BLOCK declared in shou.asm line 761
static unsigned char shou3SCWEATHER_BLOCK_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6CD804 ; line 762
};
static JumpTarget shou3SCWEATHER_BLOCK_jumpData[] = {
	{ 0x001, 0x6CD804 },
};
static PatchByteCode shou3SCWEATHER_BLOCK_patchByteCode = { shou3SCWEATHER_BLOCK_byteCode, 5, shou3SCWEATHER_BLOCK_jumpData, 1 };
// shou3SCWEATHER_4 declared in shou.asm line 766
static unsigned char shou3SCWEATHER_4_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x6CD8AE ; line 767
};
static JumpTarget shou3SCWEATHER_4_jumpData[] = {
	{ 0x002, 0x6CD8AE },
};
static PatchByteCode shou3SCWEATHER_4_patchByteCode = { shou3SCWEATHER_4_byteCode, 6, shou3SCWEATHER_4_jumpData, 1 };
// shouA236REKKA declared in shou.asm line 772
static unsigned char shouA236REKKA_byteCode[] = {
	0xE9, 0xBF, 0x00, 0x00, 0x00,                               // JMP LABEL0 ; line 773
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x26, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0226 ; line 776
	0x0F, 0x85, 0x26, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 777
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 778
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x6B3CDE ; line 780
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 781
	0x0F, 0x85, 0xEA, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL2 ; line 782
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 784
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B3CD6 ; line 785
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x27, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0227 ; line 787
	0x0F, 0x85, 0x47, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 788
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 789
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x6B4017 ; line 791
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 792
	0x0F, 0x85, 0xEA, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 793
	0xE9, 0x30, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 794
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x09, 0x00,       // CMP WORD PTR [ESI+0x140],0x0009 ; line 796
	0x0F, 0x8C, 0x43, 0x00, 0x00, 0x00,                         // JL SHORT LABEL6 ; line 797
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0002 ; line 798
	0x0F, 0x8D, 0x34, 0x00, 0x00, 0x00,                         // JNL SHORT LABEL6 ; line 799
	0xE9, 0x5F, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL7 ; line 800
	0x84, 0xC0,                                                 // TEST AL,AL ; line 802
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6B3CDE ; line 803
	0xE9, 0x96, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL8 ; line 804
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 806
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B400F ; line 807
	0xC6, 0x86, 0xF4, 0x07, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x7F4],0x01 ; line 809
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x26, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0226 ; line 810
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6B3CDE ; line 811
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B4017 ; line 812
	0xE9, 0x41, 0xFF, 0xFF, 0xFF,                               // JMP LABELS ; line 814
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 816
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL9 ; line 817
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 818
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL9 ; line 819
	0xC6, 0x86, 0xF4, 0x07, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x7F4],0x01 ; line 820
	0xE9, 0x7E, 0xFF, 0xFF, 0xFF,                               // JMP LABEL10 ; line 822
	0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x760],0x0000000A ; line 825
	0x0F, 0x8F, 0x10, 0x00, 0x00, 0x00,                         // JG SHORT LABEL11 ; line 826
	0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x760],0x00000000 ; line 827
	0x0F, 0x8F, 0x65, 0x00, 0x00, 0x00,                         // JG SHORT LABEL12 ; line 829
	0x81, 0xBE, 0x80, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x780],0x0000000A ; line 831
	0x0F, 0x8F, 0x10, 0x00, 0x00, 0x00,                         // JG SHORT LABEL13 ; line 832
	0x81, 0xBE, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x780],0x00000000 ; line 833
	0x0F, 0x8F, 0xDA, 0xFF, 0xFF, 0xFF,                         // JG SHORT LABEL14 ; line 834
	0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x764],0x0000000A ; line 836
	0x0F, 0x8F, 0x10, 0x00, 0x00, 0x00,                         // JG SHORT LABEL15 ; line 837
	0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x764],0x00000000 ; line 838
	0x0F, 0x8F, 0x33, 0x00, 0x00, 0x00,                         // JG SHORT LABEL16 ; line 839
	0x81, 0xBE, 0x84, 0x07, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x784],0x0000000A ; line 841
	0x0F, 0x8F, 0x10, 0x00, 0x00, 0x00,                         // JG SHORT LABEL17 ; line 842
	0x81, 0xBE, 0x84, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x784],0x00000000 ; line 843
	0x0F, 0x8F, 0x13, 0x00, 0x00, 0x00,                         // JG SHORT LABEL16 ; line 844
	0xE9, 0x4B, 0xFF, 0xFF, 0xFF,                               // JMP LABEL6 ; line 846
	0x66, 0xC7, 0x86, 0x3C, 0x01, 0x00, 0x00, 0x26, 0x02,       // MOV WORD PTR [ESI+0x13C],0x0226 ; line 849
	0xE9, 0x42, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL18 ; line 850
	0x66, 0xC7, 0x86, 0x3C, 0x01, 0x00, 0x00, 0x27, 0x02,       // MOV WORD PTR [ESI+0x13C],0x0227 ; line 852
	0xE9, 0x34, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL18 ; line 853
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 855
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00,                         // FSUB DWORD PTR [0x8589F0] ; line 856
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 857
	0xD9, 0xEE,                                                 // FLDZ ; line 858
	0x66, 0x9C,                                                 // PUSHF ; line 859
	0x60,                                                       // PUSHAD ; line 860
	0xDE, 0xD9,                                                 // FCOMPP ; line 861
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 862
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 863
	0x0F, 0x85, 0x08, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL19 ; line 864
	0xD9, 0xEE,                                                 // FLDZ ; line 865
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 866
	0x61,                                                       // POPAD ; line 868
	0x66, 0x9D,                                                 // POPF ; line 869
	0xE9, 0x16, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL20 ; line 870
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 872
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 873
	0xE9, 0x35, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL21 ; line 874
	0x66, 0x39, 0xBE, 0x42, 0x01, 0x00, 0x00,                   // CMP [ESI+0x142],DI ; line 876
	0x0F, 0x85, 0x27, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL22 ; line 877
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x0001 ; line 878
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL23 ; line 879
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 880
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 881
	0x66, 0x39, 0xBE, 0x42, 0x01, 0x00, 0x00,                   // CMP [ESI+0x142],DI ; line 883
	0xC3,                                                       // RET ; line 885
	0x8B, 0x06,                                                 // MOV EAX,[ESI] ; line 888
	0x8B, 0x50, 0x04,                                           // MOV EDX,[EAX+0x04] ; line 889
	0x8B, 0x8E, 0x3E, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x13E] ; line 890
	0x41,                                                       // INC ECX ; line 891
	0x89, 0x8E, 0x3E, 0x01, 0x00, 0x00,                         // MOV [ESI+0x13E],ECX ; line 892
	0xFF, 0xB6, 0x3E, 0x01, 0x00, 0x00,                         // PUSH DWORD PTR [ESI+0x13E] ; line 893
	0xFF, 0xB6, 0x3C, 0x01, 0x00, 0x00,                         // PUSH DWORD PTR [ESI+0x13C] ; line 894
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 895
	0xFF, 0xD2,                                                 // CALL EDX ; line 896
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x140],0x0001 ; line 897
	0xE9, 0x65, 0xFE, 0xFF, 0xFF,                               // JMP LABELX ; line 898
};
static JumpTarget shouA236REKKA_jumpData[] = {
	{ 0x01F, 0x6B3CDE },
	{ 0x036, 0x6B3CD6 },
	{ 0x054, 0x6B4017 },
	{ 0x094, 0x6B3CDE },
	{ 0x0A0, 0x6B400F },
	{ 0x0B6, 0x6B3CDE },
	{ 0x0BB, 0x6B4017 },
};
static PatchByteCode shouA236REKKA_patchByteCode = { shouA236REKKA_byteCode, 575, shouA236REKKA_jumpData, 7 };
// shouA236REKKA_LUL declared in shou.asm line 901
static unsigned char shouA236REKKA_LUL_byteCode[] = {
	0xE8, 0x20, 0x00, 0x00, 0x00,                               // CALL LABELZZ ; line 902
	0xE9, 0xCA, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 904
	0xC6, 0x86, 0xF4, 0x07, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x7F4],0x01 ; line 907
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x26, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0226 ; line 908
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6B3CDE ; line 909
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B4017 ; line 910
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 913
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00,                         // FSUB DWORD PTR [0x8589F0] ; line 914
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 915
	0xD9, 0xEE,                                                 // FLDZ ; line 916
	0x66, 0x9C,                                                 // PUSHF ; line 917
	0x60,                                                       // PUSHAD ; line 918
	0xDE, 0xD9,                                                 // FCOMPP ; line 919
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 920
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 921
	0x0F, 0x85, 0x08, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL19 ; line 922
	0xD9, 0xEE,                                                 // FLDZ ; line 923
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 924
	0x61,                                                       // POPAD ; line 926
	0x66, 0x9D,                                                 // POPF ; line 927
	0xE9, 0x16, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL20 ; line 928
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 930
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 931
	0xE9, 0x35, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL21 ; line 932
	0x66, 0x39, 0xBE, 0x42, 0x01, 0x00, 0x00,                   // CMP [ESI+0x142],DI ; line 934
	0x0F, 0x85, 0x27, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL22 ; line 935
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x0001 ; line 936
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL23 ; line 937
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 938
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 939
	0x66, 0x39, 0xBE, 0x42, 0x01, 0x00, 0x00,                   // CMP [ESI+0x142],DI ; line 941
	0xC3,                                                       // RET ; line 943
	0x8B, 0x06,                                                 // MOV EAX,[ESI] ; line 946
	0x8B, 0x50, 0x04,                                           // MOV EDX,[EAX+0x04] ; line 947
	0x8B, 0x8E, 0x3E, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x13E] ; line 948
	0x41,                                                       // INC ECX ; line 949
	0x89, 0x8E, 0x3E, 0x01, 0x00, 0x00,                         // MOV [ESI+0x13E],ECX ; line 950
	0xFF, 0xB6, 0x3E, 0x01, 0x00, 0x00,                         // PUSH DWORD PTR [ESI+0x13E] ; line 951
	0xFF, 0xB6, 0x3C, 0x01, 0x00, 0x00,                         // PUSH DWORD PTR [ESI+0x13C] ; line 952
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 953
	0xFF, 0xD2,                                                 // CALL EDX ; line 954
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x140],0x0001 ; line 955
	0xE9, 0x36, 0xFF, 0xFF, 0xFF,                               // JMP LABELX ; line 956
	0x90,                                                       // NOP ; line 959
};
static JumpTarget shouA236REKKA_LUL_jumpData[] = {
	{ 0x01C, 0x6B3CDE },
	{ 0x021, 0x6B4017 },
};
static PatchByteCode shouA236REKKA_LUL_patchByteCode = { shouA236REKKA_LUL_byteCode, 213, shouA236REKKA_LUL_jumpData, 2 };
// shouA236REKKA_LETSNOT declared in shou.asm line 966
static unsigned char shouA236REKKA_LETSNOT_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B3CD0 ; line 967
};
static JumpTarget shouA236REKKA_LETSNOT_jumpData[] = {
	{ 0x001, 0x6B3CD0 },
};
static PatchByteCode shouA236REKKA_LETSNOT_patchByteCode = { shouA236REKKA_LETSNOT_byteCode, 5, shouA236REKKA_LETSNOT_jumpData, 1 };
// shouA236REKKA_AREYOUKIDDINGME1 declared in shou.asm line 974
static unsigned char shouA236REKKA_AREYOUKIDDINGME1_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6BA7FF ; line 977
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 978
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6B3DA2 ; line 979
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00,                   // FLD DWORD PTR [0x85AE94] ; line 980
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 981
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x0000 ; line 982
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 983
	0x6A, 0x78,                                           // PUSH 0x78 ; line 984
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 985
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 986
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 987
	0x6A, 0x32,                                           // PUSH 0x32 ; line 988
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 989
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x487870 ; line 990
	0x6A, 0x1D,                                           // PUSH 0x1D ; line 991
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 992
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x439490 ; line 993
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6B3D32 ; line 995
};
static JumpTarget shouA236REKKA_AREYOUKIDDINGME1_jumpData[] = {
	{ 0x002, 0x6BA7FF },
	{ 0x011, 0x6B3DA2 },
	{ 0x03A, 0x47A9E0 },
	{ 0x043, 0x487870 },
	{ 0x04C, 0x439490 },
	{ 0x051, 0x6B3D32 },
};
static PatchByteCode shouA236REKKA_AREYOUKIDDINGME1_patchByteCode = { shouA236REKKA_AREYOUKIDDINGME1_byteCode, 85, shouA236REKKA_AREYOUKIDDINGME1_jumpData, 6 };
// shouA236REKKA_AREYOUKIDDINGME2 declared in shou.asm line 998
static unsigned char shouA236REKKA_AREYOUKIDDINGME2_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6BA7FF ; line 1001
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 1002
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6B40DB ; line 1003
	0xDD, 0x05, 0x00, 0x9B, 0x85, 0x00,                   // FLD QWORD PTR [0x859B00] ; line 1004
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 1005
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x0000 ; line 1006
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1007
	0x6A, 0x78,                                           // PUSH 0x78 ; line 1008
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1009
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1010
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1011
	0x6A, 0x32,                                           // PUSH 0x32 ; line 1012
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1013
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x487870 ; line 1014
	0x6A, 0x1D,                                           // PUSH 0x1D ; line 1015
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1016
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x439490 ; line 1017
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6B406B ; line 1019
};
static JumpTarget shouA236REKKA_AREYOUKIDDINGME2_jumpData[] = {
	{ 0x002, 0x6BA7FF },
	{ 0x011, 0x6B40DB },
	{ 0x03A, 0x47A9E0 },
	{ 0x043, 0x487870 },
	{ 0x04C, 0x439490 },
	{ 0x051, 0x6B406B },
};
static PatchByteCode shouA236REKKA_AREYOUKIDDINGME2_patchByteCode = { shouA236REKKA_AREYOUKIDDINGME2_byteCode, 85, shouA236REKKA_AREYOUKIDDINGME2_jumpData, 6 };
// shouA236REKKA_EFFECTS declared in shou.asm line 1042
static unsigned char shouA236REKKA_EFFECTS_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 1043
};
static PatchByteCode shouA236REKKA_EFFECTS_patchByteCode = { shouA236REKKA_EFFECTS_byteCode, 9, nullptr, 0 };
// shouA236REKKA_EFFECTS2 declared in shou.asm line 1048
static unsigned char shouA236REKKA_EFFECTS2_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 1049
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1050
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x464980 ; line 1051
};
static JumpTarget shouA236REKKA_EFFECTS2_jumpData[] = {
	{ 0x005, 0x464980 },
};
static PatchByteCode shouA236REKKA_EFFECTS2_patchByteCode = { shouA236REKKA_EFFECTS2_byteCode, 9, shouA236REKKA_EFFECTS2_jumpData, 1 };
// shouA236REKKA_SFX declared in shou.asm line 1056
static unsigned char shouA236REKKA_SFX_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6CA3C6 ; line 1057
};
static JumpTarget shouA236REKKA_SFX_jumpData[] = {
	{ 0x001, 0x6CA3C6 },
};
static PatchByteCode shouA236REKKA_SFX_patchByteCode = { shouA236REKKA_SFX_byteCode, 5, shouA236REKKA_SFX_jumpData, 1 };
// shouA623SLASHY1 declared in shou.asm line 1068
static unsigned char shouA623SLASHY1_byteCode[] = {
	0xBB, 0x08, 0x00, 0x00, 0x00, // MOV EBX,0x00000008 ; line 1069
};
static PatchByteCode shouA623SLASHY1_patchByteCode = { shouA623SLASHY1_byteCode, 5, nullptr, 0 };
// shouIDK0 declared in shou.asm line 1080
static unsigned char shouIDK0_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B5289 ; line 1081
};
static JumpTarget shouIDK0_jumpData[] = {
	{ 0x001, 0x6B5289 },
};
static PatchByteCode shouIDK0_patchByteCode = { shouIDK0_byteCode, 5, shouIDK0_jumpData, 1 };
// shouIDK1 declared in shou.asm line 1086
static unsigned char shouIDK1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6BA7FF ; line 1087
};
static JumpTarget shouIDK1_jumpData[] = {
	{ 0x001, 0x6BA7FF },
};
static PatchByteCode shouIDK1_patchByteCode = { shouIDK1_byteCode, 5, shouIDK1_jumpData, 1 };
// shouA22_1 declared in shou.asm line 1098
static unsigned char shouA22_1_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 1099
};
static PatchByteCode shouA22_1_patchByteCode = { shouA22_1_byteCode, 6, nullptr, 0 };
// shouA22_IDK1 declared in shou.asm line 1106
static unsigned char shouA22_IDK1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C7805 ; line 1107
};
static JumpTarget shouA22_IDK1_jumpData[] = {
	{ 0x001, 0x6C7805 },
};
static PatchByteCode shouA22_IDK1_patchByteCode = { shouA22_IDK1_byteCode, 5, shouA22_IDK1_jumpData, 1 };
// shouA22 declared in shou.asm line 1112
static unsigned char shouA22_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1113
	0x60,                                                       // PUSHAD ; line 1114
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x190] ; line 1115
	0x81, 0xF8, 0x04, 0x00, 0x00, 0x00,                         // CMP EAX,0x04 ; line 1116
	0x0F, 0x84, 0x1C, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1117
	0x81, 0xF8, 0x08, 0x00, 0x00, 0x00,                         // CMP EAX,0x08 ; line 1118
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1119
	0x61,                                                       // POPAD ; line 1120
	0x66, 0x9D,                                                 // POPF ; line 1121
	0xD9, 0xEE,                                                 // FLDZ ; line 1122
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 1123
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C7937 ; line 1124
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x140],0x00FF ; line 1126
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x000000FF ; line 1127
	0x61,                                                       // POPAD ; line 1128
	0x66, 0x9D,                                                 // POPF ; line 1129
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C7811 ; line 1130
};
static JumpTarget shouA22_jumpData[] = {
	{ 0x02D, 0x6C7937 },
	{ 0x048, 0x6C7811 },
};
static PatchByteCode shouA22_patchByteCode = { shouA22_byteCode, 76, shouA22_jumpData, 2 };
// shouA22_TOTAL declared in shou.asm line 1134
static unsigned char shouA22_TOTAL_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 1135
};
static PatchByteCode shouA22_TOTAL_patchByteCode = { shouA22_TOTAL_byteCode, 6, nullptr, 0 };
// shouIDK1A declared in shou.asm line 1145
static unsigned char shouIDK1A_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 1146
	0x0F, 0x85, 0x33, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1147
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 1148
	0x0F, 0x85, 0x23, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1149
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1150
	0x66, 0xC7, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x194],0x0001 ; line 1151
	0xD9, 0xE8,                                                 // FLD1 ; line 1152
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x11C] ; line 1153
	0xD9, 0xE8,                                                 // FLD1 ; line 1154
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 1155
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 1157
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C8A92 ; line 1158
};
static JumpTarget shouIDK1A_jumpData[] = {
	{ 0x049, 0x6C8A92 },
};
static PatchByteCode shouIDK1A_patchByteCode = { shouIDK1A_byteCode, 77, shouIDK1A_jumpData, 1 };
// shouIDK1_DUR declared in shou.asm line 1164
static unsigned char shouIDK1_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000076 ; line 1165
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                         // JG 0x6C3E6F ; line 1166
	0x60,                                                       // PUSHAD ; line 1168
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x190] ; line 1169
	0x81, 0xF8, 0x04, 0x00, 0x00, 0x00,                         // CMP EAX,0x04 ; line 1170
	0x0F, 0x84, 0x42, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1171
	0x81, 0xF8, 0x08, 0x00, 0x00, 0x00,                         // CMP EAX,0x08 ; line 1172
	0x0F, 0x84, 0x36, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1173
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                         // CMP EAX,0x01 ; line 1174
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1175
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00,                         // CMP EAX,0x02 ; line 1176
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1177
	0x81, 0xF8, 0x06, 0x00, 0x00, 0x00,                         // CMP EAX,0x06 ; line 1178
	0x0F, 0x84, 0x12, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1179
	0x81, 0xF8, 0x07, 0x00, 0x00, 0x00,                         // CMP EAX,0x07 ; line 1180
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1181
	0x61,                                                       // POPAD ; line 1182
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C8ABC ; line 1183
	0x61,                                                       // POPAD ; line 1185
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C3E6F ; line 1186
};
static JumpTarget shouIDK1_DUR_jumpData[] = {
	{ 0x00C, 0x6C3E6F },
	{ 0x061, 0x6C8ABC },
	{ 0x067, 0x6C3E6F },
};
static PatchByteCode shouIDK1_DUR_patchByteCode = { shouIDK1_DUR_byteCode, 107, shouIDK1_DUR_jumpData, 3 };
// shouIDK1_SPD declared in shou.asm line 1191
static unsigned char shouIDK1_SPD_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C8AEB ; line 1192
};
static JumpTarget shouIDK1_SPD_jumpData[] = {
	{ 0x001, 0x6C8AEB },
};
static PatchByteCode shouIDK1_SPD_patchByteCode = { shouIDK1_SPD_byteCode, 5, shouIDK1_SPD_jumpData, 1 };
// shouIDK1_POS declared in shou.asm line 1201
static unsigned char shouIDK1_POS_byteCode[] = {
	0xD8, 0x80, 0xEC, 0x00, 0x00, 0x00,       // FADD DWORD PTR [EAX+0xEC] ; line 1202
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0xFF, // CMP BYTE PTR [ESI+0x104],0xFF ; line 1203
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 1204
	0xD8, 0x05, 0xA8, 0x75, 0x85, 0x00,       // FADD DWORD PTR [0x8575A8] ; line 1205
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 1206
	0xD8, 0x25, 0xA8, 0x75, 0x85, 0x00,       // FSUB DWORD PTR [0x8575A8] ; line 1208
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6C6D19 ; line 1210
};
static JumpTarget shouIDK1_POS_jumpData[] = {
	{ 0x025, 0x6C6D19 },
};
static PatchByteCode shouIDK1_POS_patchByteCode = { shouIDK1_POS_byteCode, 41, shouIDK1_POS_jumpData, 1 };
// shouIDK1_IDK declared in shou.asm line 1218
static unsigned char shouIDK1_IDK_byteCode[] = {
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 1219
};
static PatchByteCode shouIDK1_IDK_patchByteCode = { shouIDK1_IDK_byteCode, 6, nullptr, 0 };
// shouA623CHARGE_BX declared in shou.asm line 1233
static unsigned char shouA623CHARGE_BX_byteCode[] = {
	0xDD, 0x05, 0x00, 0x9B, 0x85, 0x00, // FLD QWORD PTR [0x859B00] ; line 1234
};
static PatchByteCode shouA623CHARGE_BX_patchByteCode = { shouA623CHARGE_BX_byteCode, 6, nullptr, 0 };
// shouA623CHARGE_BY declared in shou.asm line 1236
static unsigned char shouA623CHARGE_BY_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 1237
};
static PatchByteCode shouA623CHARGE_BY_patchByteCode = { shouA623CHARGE_BY_byteCode, 6, nullptr, 0 };
// shouA623CHARGE_DEC1 declared in shou.asm line 1239
static unsigned char shouA623CHARGE_DEC1_byteCode[] = {
	0xDD, 0x05, 0xB8, 0x81, 0x85, 0x00, // FLD QWORD PTR [0x8581B8] ; line 1240
};
static PatchByteCode shouA623CHARGE_DEC1_patchByteCode = { shouA623CHARGE_DEC1_byteCode, 6, nullptr, 0 };
// shouA623CHARGE_DEC2 declared in shou.asm line 1242
static unsigned char shouA623CHARGE_DEC2_byteCode[] = {
	0xD8, 0x25, 0x60, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x867860] ; line 1243
};
static PatchByteCode shouA623CHARGE_DEC2_patchByteCode = { shouA623CHARGE_DEC2_byteCode, 6, nullptr, 0 };
// shou3SCCHARGE_1 declared in shou.asm line 1271
static unsigned char shou3SCCHARGE_1_byteCode[] = {
	0xDD, 0x05, 0x58, 0x79, 0x85, 0x00, // FLD QWORD PTR [0x857958] ; line 1272
};
static PatchByteCode shou3SCCHARGE_1_patchByteCode = { shou3SCCHARGE_1_byteCode, 6, nullptr, 0 };
// shou3SCCHARGE_2 declared in shou.asm line 1274
static unsigned char shou3SCCHARGE_2_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 1275
};
static PatchByteCode shou3SCCHARGE_2_patchByteCode = { shou3SCCHARGE_2_byteCode, 6, nullptr, 0 };
// shou5SCCHARGE_X declared in shou.asm line 1283
static unsigned char shou5SCCHARGE_X_byteCode[] = {
	0xD9, 0x05, 0xD8, 0x7D, 0x85, 0x00, // FLD DWORD PTR [0x857DD8] ; line 1284
};
static PatchByteCode shou5SCCHARGE_X_patchByteCode = { shou5SCCHARGE_X_byteCode, 6, nullptr, 0 };
// shou5SCCHARGE_Y declared in shou.asm line 1286
static unsigned char shou5SCCHARGE_Y_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 1287
};
static PatchByteCode shou5SCCHARGE_Y_patchByteCode = { shou5SCCHARGE_Y_byteCode, 6, nullptr, 0 };
// shou5SCCHARGE declared in shou.asm line 1295
static unsigned char shou5SCCHARGE_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 1297
	0x8B, 0x8E, 0x70, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x170] ; line 1298
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xEC] ; line 1299
	0xD9, 0x05, 0xB8, 0x96, 0x85, 0x00,                         // FLD DWORD PTR [0x8596B8] ; line 1300
	0xDE, 0xD9,                                                 // FCOMPP ; line 1301
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1302
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1303
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1304
	0xD9, 0x05, 0xB8, 0x96, 0x85, 0x00,                         // FLD DWORD PTR [0x8596B8] ; line 1305
	0xE9, 0x2A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1306
	0xD9, 0x05, 0x5C, 0xA2, 0x85, 0x00,                         // FLD DWORD PTR [0x85A25C] ; line 1308
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xEC] ; line 1309
	0xDE, 0xD9,                                                 // FCOMPP ; line 1310
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1311
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1312
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 1313
	0xD9, 0x05, 0x5C, 0xA2, 0x85, 0x00,                         // FLD DWORD PTR [0x85A25C] ; line 1315
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1316
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xEC] ; line 1318
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1320
	0x61,                                                       // POPAD ; line 1321
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1E ; line 1323
	0xD9, 0x05, 0x4C, 0x15, 0x87, 0x00,                         // FLD DWORD PTR [0x87154C] ; line 1324
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 1325
	0xD9, 0xEE,                                                 // FLDZ ; line 1326
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1327
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6B7438 ; line 1328
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1E ; line 1330
	0x0F, 0x85, 0xCD, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 1331
	0xE9, 0x6C, 0xFF, 0xFF, 0xFF,                               // JMP LABELS ; line 1332
	0x90,                                                       // NOP ; line 1333
};
static JumpTarget shou5SCCHARGE_jumpData[] = {
	{ 0x07B, 0x6B7438 },
};
static PatchByteCode shou5SCCHARGE_patchByteCode = { shou5SCCHARGE_byteCode, 149, shou5SCCHARGE_jumpData, 1 };
// shou2SCQUAKE_SPD declared in shou.asm line 1341
static unsigned char shou2SCQUAKE_SPD_byteCode[] = {
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 1342
};
static PatchByteCode shou2SCQUAKE_SPD_patchByteCode = { shou2SCQUAKE_SPD_byteCode, 6, nullptr, 0 };
// shou2SCQUAKE declared in shou.asm line 1347
static unsigned char shou2SCQUAKE_byteCode[] = {
	0xE9, 0x77, 0x00, 0x00, 0x00,                               // JMP LABELS ; line 1348
	0x60,                                                       // PUSHAD ; line 1351
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                         // MOV EBX,[ESI+0x398] ; line 1352
	0xD9, 0x83, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EBX+0xF0] ; line 1353
	0xD9, 0xE8,                                                 // FLD1 ; line 1354
	0xDE, 0xD9,                                                 // FCOMPP ; line 1355
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1356
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1357
	0x0F, 0x85, 0x55, 0x00, 0x00, 0x00,                         // JNE SHORT LABELS2 ; line 1358
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EBX+0xEC] ; line 1359
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1360
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                         // FADD QWORD PTR [0x867830] ; line 1361
	0xDE, 0xD9,                                                 // FCOMPP ; line 1362
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1363
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1364
	0x0F, 0x85, 0x36, 0x00, 0x00, 0x00,                         // JNE SHORT LABELS2 ; line 1365
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1366
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EBX+0xEC] ; line 1367
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                         // FADD QWORD PTR [0x867830] ; line 1368
	0xDE, 0xD9,                                                 // FCOMPP ; line 1369
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1370
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1371
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,                         // JNE SHORT LABELS2 ; line 1372
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x78, 0x00,       // MOV WORD PTR [ESI+0x140],0x0078 ; line 1373
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x09, 0x00,       // MOV WORD PTR [ESI+0x142],0x0009 ; line 1374
	0xE9, 0x29, 0x00, 0x00, 0x00,                               // JMP SHORT LABELS3 ; line 1375
	0x61,                                                       // POPAD ; line 1378
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6CB189 ; line 1379
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x78, 0x00,       // CMP WORD PTR [ESI+0x140],0x0078 ; line 1382
	0x0F, 0x8C, 0x7A, 0xFF, 0xFF, 0xFF,                         // JL LABELS1 ; line 1383
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x82, 0x00,       // CMP WORD PTR [ESI+0x140],0x0082 ; line 1384
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                         // JL 0x6CB189 ; line 1385
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6CB178 ; line 1386
	0x8B, 0x8B, 0xE4, 0x05, 0x00, 0x00,                         // MOV ECX,[EBX+0x5E4] ; line 1388
	0x66, 0x81, 0xC1, 0xFA, 0x00,                               // ADD CX,0x00FA ; line 1389
	0x66, 0x81, 0xF9, 0xF4, 0x01,                               // CMP CX,0x01F4 ; line 1390
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 1391
	0x66, 0xB9, 0xF4, 0x01,                                     // MOV CX,0x01F4 ; line 1392
	0x66, 0x89, 0x8B, 0xE4, 0x05, 0x00, 0x00,                   // MOV [EBX+0x5E4],CX ; line 1394
	0x61,                                                       // POPAD ; line 1395
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6CB189 ; line 1396
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1398
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 1399
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1400
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1401
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1403
};
static JumpTarget shou2SCQUAKE_jumpData[] = {
	{ 0x078, 0x6CB189 },
	{ 0x096, 0x6CB189 },
	{ 0x09B, 0x6CB178 },
	{ 0x0C2, 0x6CB189 },
};
static PatchByteCode shou2SCQUAKE_patchByteCode = { shou2SCQUAKE_byteCode, 237, shou2SCQUAKE_jumpData, 4 };
// shou2SCQUAKE_ANG declared in shou.asm line 1408
static unsigned char shou2SCQUAKE_ANG_byteCode[] = {
	0x89, 0x44, 0x24, 0x10, // MOV [ESP+0x10],EAX ; line 1409
	0xD9, 0xEE,             // FLDZ ; line 1410
};
static PatchByteCode shou2SCQUAKE_ANG_patchByteCode = { shou2SCQUAKE_ANG_byteCode, 6, nullptr, 0 };
// shou2SCQUAKE_ANG1 declared in shou.asm line 1413
static unsigned char shou2SCQUAKE_ANG1_byteCode[] = {
	0xD8, 0x25, 0x84, 0xB9, 0x85, 0x00, // FSUB DWORD PTR [0x85B984] ; line 1414
};
static PatchByteCode shou2SCQUAKE_ANG1_patchByteCode = { shou2SCQUAKE_ANG1_byteCode, 6, nullptr, 0 };
// shou2SCQUAKE_SPAWNRATE declared in shou.asm line 1419
static unsigned char shou2SCQUAKE_SPAWNRATE_byteCode[] = {
	0xB9, 0x06, 0x00, 0x00, 0x00, // MOV ECX,0x00000006 ; line 1420
};
static PatchByteCode shou2SCQUAKE_SPAWNRATE_patchByteCode = { shou2SCQUAKE_SPAWNRATE_byteCode, 5, nullptr, 0 };
// shou2SCQUAKE_BOX declared in shou.asm line 1428
static unsigned char shou2SCQUAKE_BOX_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1429
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1430
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1431
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1432
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1434
};
static PatchByteCode shou2SCQUAKE_BOX_patchByteCode = { shou2SCQUAKE_BOX_byteCode, 39, nullptr, 0 };
// shou2SCQUAKE_BOUNDS declared in shou.asm line 1439
static unsigned char shou2SCQUAKE_BOUNDS_byteCode[] = {
	0xD9, 0x05, 0xC0, 0xE7, 0x85, 0x00, // FLD DWORD PTR [0x85E7C0] ; line 1440
};
static PatchByteCode shou2SCQUAKE_BOUNDS_patchByteCode = { shou2SCQUAKE_BOUNDS_byteCode, 6, nullptr, 0 };
// shou2SCQUAKE_BOUNDS1 declared in shou.asm line 1442
static unsigned char shou2SCQUAKE_BOUNDS1_byteCode[] = {
	0xD9, 0x05, 0x80, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB80] ; line 1443
};
static PatchByteCode shou2SCQUAKE_BOUNDS1_patchByteCode = { shou2SCQUAKE_BOUNDS1_byteCode, 6, nullptr, 0 };
// shouSPACE2 declared in shou.asm line 1450
static unsigned char shouSPACE2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C7937 ; line 1451
};
static JumpTarget shouSPACE2_jumpData[] = {
	{ 0x001, 0x6C7937 },
};
static PatchByteCode shouSPACE2_patchByteCode = { shouSPACE2_byteCode, 5, shouSPACE2_jumpData, 1 };
// shou3SCWEATHER_OFFSCREEN declared in shou.asm line 1466
static unsigned char shou3SCWEATHER_OFFSCREEN_byteCode[] = {
	0x0F, 0x8B, 0x05, 0x00, 0x00, 0x00,       // JNP SHORT LABEL1 ; line 1467
	0xE9, 0x0E, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 1468
	0xDD, 0xD8,                               // FSTP ST0 ; line 1470
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00, // MOV BYTE PTR [ESI+0x194],0x00 ; line 1471
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6CD972 ; line 1472
};
static JumpTarget shou3SCWEATHER_OFFSCREEN_jumpData[] = {
	{ 0x015, 0x6CD972 },
};
static PatchByteCode shou3SCWEATHER_OFFSCREEN_patchByteCode = { shou3SCWEATHER_OFFSCREEN_byteCode, 25, shou3SCWEATHER_OFFSCREEN_jumpData, 1 };
// shou3SCWEATHER_OFFSCREEN2 declared in shou.asm line 1475
static unsigned char shou3SCWEATHER_OFFSCREEN2_byteCode[] = {
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 1476
	0xE9, 0x0E, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 1477
	0xDD, 0xD8,                               // FSTP ST0 ; line 1479
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00, // MOV BYTE PTR [ESI+0x194],0x00 ; line 1480
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6CD972 ; line 1481
};
static JumpTarget shou3SCWEATHER_OFFSCREEN2_jumpData[] = {
	{ 0x015, 0x6CD972 },
};
static PatchByteCode shou3SCWEATHER_OFFSCREEN2_patchByteCode = { shou3SCWEATHER_OFFSCREEN2_byteCode, 25, shou3SCWEATHER_OFFSCREEN2_jumpData, 1 };
// shou5SCWEATHER_DUR declared in shou.asm line 1519
static unsigned char shou5SCWEATHER_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1F ; line 1520
};
static PatchByteCode shou5SCWEATHER_DUR_patchByteCode = { shou5SCWEATHER_DUR_byteCode, 10, nullptr, 0 };
// shou4SCWEATHER_IDK declared in shou.asm line 1539
static unsigned char shou4SCWEATHER_IDK_byteCode[] = {
	0x6A, 0x0A,                               // PUSH 0x0A ; line 1540
	0x89, 0xF1,                               // MOV ECX,ESI ; line 1541
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x02, // MOV BYTE PTR [ESI+0x194],0x02 ; line 1542
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6CC2F8 ; line 1543
};
static JumpTarget shou4SCWEATHER_IDK_jumpData[] = {
	{ 0x00C, 0x6CC2F8 },
};
static PatchByteCode shou4SCWEATHER_IDK_patchByteCode = { shou4SCWEATHER_IDK_byteCode, 16, shou4SCWEATHER_IDK_jumpData, 1 };
// shou4SCWEATHER_Y declared in shou.asm line 1547
static unsigned char shou4SCWEATHER_Y_byteCode[] = {
	0xD8, 0x05, 0xBC, 0xCB, 0x85, 0x00, // FADD DWORD PTR [0x85CBBC] ; line 1548
};
static PatchByteCode shou4SCWEATHER_Y_patchByteCode = { shou4SCWEATHER_Y_byteCode, 6, nullptr, 0 };
// shou3SCSLASHY_X declared in shou.asm line 1554
static unsigned char shou3SCSLASHY_X_byteCode[] = {
	0xD9, 0x05, 0x40, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871540] ; line 1555
};
static PatchByteCode shou3SCSLASHY_X_patchByteCode = { shou3SCSLASHY_X_byteCode, 6, nullptr, 0 };
// shou3SCSLASHY_ACC declared in shou.asm line 1557
static unsigned char shou3SCSLASHY_ACC_byteCode[] = {
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00, // FSUB DWORD PTR [0x8589F0] ; line 1558
};
static PatchByteCode shou3SCSLASHY_ACC_patchByteCode = { shou3SCSLASHY_ACC_byteCode, 6, nullptr, 0 };
// shou4SCARMOR_DUR declared in shou.asm line 1570
static unsigned char shou4SCARMOR_DUR_byteCode[] = {
	0xC7, 0x86, 0x24, 0x09, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, // MOV DWORD PTR [ESI+0x924],0x0000012C ; line 1571
};
static PatchByteCode shou4SCARMOR_DUR_patchByteCode = { shou4SCARMOR_DUR_byteCode, 10, nullptr, 0 };
// shou4SCARMOR_FLASH declared in shou.asm line 1575
static unsigned char shou4SCARMOR_FLASH_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x140],0x06 ; line 1576
};
static PatchByteCode shou4SCARMOR_FLASH_patchByteCode = { shou4SCARMOR_FLASH_byteCode, 9, nullptr, 0 };
// shou4SCARMOR_IDK declared in shou.asm line 1580
static unsigned char shou4SCARMOR_IDK_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x02 ; line 1581
};
static PatchByteCode shou4SCARMOR_IDK_patchByteCode = { shou4SCARMOR_IDK_byteCode, 10, nullptr, 0 };
// shou4SCARMOR declared in shou.asm line 1585
static unsigned char shou4SCARMOR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1586
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1587
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0000 ; line 1588
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1589
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1590
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1591
	0xBB, 0x01, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000001 ; line 1593
};
static PatchByteCode shou4SCARMOR_patchByteCode = { shou4SCARMOR_byteCode, 53, nullptr, 0 };
// shou2SCSLASH_TARG declared in shou.asm line 1600
static unsigned char shou2SCSLASH_TARG_byteCode[] = {
	0x68, 0x53, 0x03, 0x00, 0x00, // PUSH 0x00000353 ; line 1601
};
static PatchByteCode shou2SCSLASH_TARG_patchByteCode = { shou2SCSLASH_TARG_byteCode, 5, nullptr, 0 };
// shou2SCSLASH_MOVE declared in shou.asm line 1606
static unsigned char shou2SCSLASH_MOVE_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B642B ; line 1607
};
static JumpTarget shou2SCSLASH_MOVE_jumpData[] = {
	{ 0x001, 0x6B642B },
};
static PatchByteCode shou2SCSLASH_MOVE_patchByteCode = { shou2SCSLASH_MOVE_byteCode, 5, shou2SCSLASH_MOVE_jumpData, 1 };
// shou2SCSLASH_IDK declared in shou.asm line 1612
static unsigned char shou2SCSLASH_IDK_byteCode[] = {
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x142],0x01 ; line 1613
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1614
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1615
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1616
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x4F, 0x00,       // CMP WORD PTR [ESI+0x140],0x4F ; line 1618
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                         // JG SHORT LABEL2 ; line 1619
	0xB0, 0xFF,                                                 // MOV AL,0xFF ; line 1620
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV [ESI+0x113],AL ; line 1621
	0xD9, 0xE8,                                                 // FLD1 ; line 1623
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x11C] ; line 1624
	0xD9, 0xE8,                                                 // FLD1 ; line 1625
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 1626
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6CA7D8 ; line 1627
};
static JumpTarget shou2SCSLASH_IDK_jumpData[] = {
	{ 0x048, 0x6CA7D8 },
};
static PatchByteCode shou2SCSLASH_IDK_patchByteCode = { shou2SCSLASH_IDK_byteCode, 76, shou2SCSLASH_IDK_jumpData, 1 };
// shouA214QUAKE declared in shou.asm line 1637
static unsigned char shouA214QUAKE_byteCode[] = {
	0xE9, 0xC2, 0x00, 0x00, 0x00,                               // JMP LABELS ; line 1638
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1641
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1642
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x08,                   // MOV BYTE PTR [ESI+0x194],0x08 ; line 1643
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x190] ; line 1645
	0x81, 0xF8, 0x00, 0x00, 0x00, 0x00,                         // CMP EAX,0x00 ; line 1646
	0x0F, 0x84, 0x6B, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 1647
	0x81, 0xF8, 0x04, 0x00, 0x00, 0x00,                         // CMP EAX,0x04 ; line 1648
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 1649
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 1650
	0x6A, 0x0F,                                                 // PUSH 0x0F ; line 1652
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1653
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CDE0 ; line 1654
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x06 ; line 1655
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 1656
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 1657
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 1659
	0x0F, 0x8E, 0x3A, 0x00, 0x00, 0x00,                         // JLE SHORT LABEL5 ; line 1660
	0x66, 0x81, 0xBE, 0x6C, 0x03, 0x00, 0x00, 0x05, 0x00,       // CMP WORD PTR [ESI+0x36C],0x0005 ; line 1661
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL6 ; line 1662
	0x66, 0xC7, 0x86, 0x6C, 0x03, 0x00, 0x00, 0x00, 0x00,       // MOV WORD PTR [ESI+0x36C],0x0000 ; line 1663
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1664
	0x66, 0x81, 0x86, 0x6C, 0x03, 0x00, 0x00, 0x01, 0x00,       // ADD WORD PTR [ESI+0x36C],0x01 ; line 1666
	0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x140],0x78 ; line 1668
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C5133 ; line 1669
	0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x6D, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x140],0x6D ; line 1671
	0x0F, 0x8D, 0xE1, 0xFF, 0xFF, 0xFF,                         // JNL SHORT LABEL2 ; line 1672
	0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x140],0x0000006E ; line 1673
	0xE9, 0xD2, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL2 ; line 1674
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x2B, 0x03,       // CMP WORD PTR [ESI+0x13C],0x032B ; line 1677
	0x0F, 0x85, 0x32, 0x00, 0x00, 0x00,                         // JNE SHORT LABELX ; line 1678
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 1679
	0x66, 0x8B, 0x80, 0x3C, 0x01, 0x00, 0x00,                   // MOV AX,[EAX+0x13C] ; line 1680
	0x66, 0x81, 0xF8, 0x32, 0x00,                               // CMP AX,0x0032 ; line 1681
	0x0F, 0x8C, 0x15, 0x00, 0x00, 0x00,                         // JL SHORT LABELZ ; line 1682
	0x66, 0x81, 0xF8, 0x60, 0x00,                               // CMP AX,0x0060 ; line 1683
	0x0F, 0x8F, 0x0A, 0x00, 0x00, 0x00,                         // JG SHORT LABELZ ; line 1684
	0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x140],0x00000078 ; line 1685
	0xE9, 0xFD, 0xFE, 0xFF, 0xFF,                               // JMP LABELY ; line 1687
	0xD9, 0xEE,                                                 // FLDZ ; line 1689
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 1690
	0xE9, 0xEE, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABELZ ; line 1691
};
static JumpTarget shouA214QUAKE_jumpData[] = {
	{ 0x046, 0x48CDE0 },
	{ 0x0A4, 0x6C5133 },
};
static PatchByteCode shouA214QUAKE_patchByteCode = { shouA214QUAKE_byteCode, 277, shouA214QUAKE_jumpData, 2 };
// shouA214QUAKE_LEAP declared in shou.asm line 1697
static unsigned char shouA214QUAKE_LEAP_byteCode[] = {
	0xF6, 0xC4, 0x01, // TEST AH,0x01 ; line 1698
};
static PatchByteCode shouA214QUAKE_LEAP_patchByteCode = { shouA214QUAKE_LEAP_byteCode, 3, nullptr, 0 };
// shouA214QUAKE_BPOS declared in shou.asm line 1703
static unsigned char shouA214QUAKE_BPOS_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1704
	0xD9, 0x5C, 0x24, 0x04,             // FSTP DWORD PTR [ESP+0x04] ; line 1705
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xEC] ; line 1706
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6B1A48 ; line 1707
};
static JumpTarget shouA214QUAKE_BPOS_jumpData[] = {
	{ 0x00D, 0x6B1A48 },
};
static PatchByteCode shouA214QUAKE_BPOS_patchByteCode = { shouA214QUAKE_BPOS_byteCode, 17, shouA214QUAKE_BPOS_jumpData, 1 };
// shouA214QUAKE_CPOS declared in shou.asm line 1714
static unsigned char shouA214QUAKE_CPOS_byteCode[] = {
	0xD9, 0x05, 0xA4, 0x75, 0x85, 0x00, // FLD DWORD PTR [0x8575A4] ; line 1715
};
static PatchByteCode shouA214QUAKE_CPOS_patchByteCode = { shouA214QUAKE_CPOS_byteCode, 6, nullptr, 0 };
// shouA214QUAKE_CSTART declared in shou.asm line 1719
static unsigned char shouA214QUAKE_CSTART_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1720
};
static PatchByteCode shouA214QUAKE_CSTART_patchByteCode = { shouA214QUAKE_CSTART_byteCode, 10, nullptr, 0 };
// shou5SCQUAKE_DUR1 declared in shou.asm line 1728
static unsigned char shou5SCQUAKE_DUR1_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 1729
};
static PatchByteCode shou5SCQUAKE_DUR1_patchByteCode = { shou5SCQUAKE_DUR1_byteCode, 10, nullptr, 0 };
// shou5SCQUAKE_DUR2 declared in shou.asm line 1731
static unsigned char shou5SCQUAKE_DUR2_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x02 ; line 1732
};
static PatchByteCode shou5SCQUAKE_DUR2_patchByteCode = { shou5SCQUAKE_DUR2_byteCode, 10, nullptr, 0 };
// shou5SCQUAKE_DUR3 declared in shou.asm line 1734
static unsigned char shou5SCQUAKE_DUR3_byteCode[] = {
	0xBB, 0x03, 0x00, 0x00, 0x00, // MOV EBX,0x00000003 ; line 1735
};
static PatchByteCode shou5SCQUAKE_DUR3_patchByteCode = { shou5SCQUAKE_DUR3_byteCode, 5, nullptr, 0 };
// shou5SCQUAKE_DUR4 declared in shou.asm line 1737
static unsigned char shou5SCQUAKE_DUR4_byteCode[] = {
	0x81, 0xFB, 0x78, 0x00, 0x00, 0x00, // CMP EBX,0x00000078 ; line 1738
};
static PatchByteCode shou5SCQUAKE_DUR4_patchByteCode = { shou5SCQUAKE_DUR4_byteCode, 6, nullptr, 0 };
// shou5SCQUAKE_DUR5 declared in shou.asm line 1740
static unsigned char shou5SCQUAKE_DUR5_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000078 ; line 1741
};
static PatchByteCode shou5SCQUAKE_DUR5_patchByteCode = { shou5SCQUAKE_DUR5_byteCode, 10, nullptr, 0 };
// shou5SCQUAKE_PERIOD declared in shou.asm line 1756
static unsigned char shou5SCQUAKE_PERIOD_byteCode[] = {
	0x81, 0xC3, 0x1A, 0x00, 0x00, 0x00, // ADD EBX,0x1A ; line 1757
	0x81, 0xC5, 0x01, 0x00, 0x00, 0x00, // ADD EBP,0x01 ; line 1758
};
static PatchByteCode shou5SCQUAKE_PERIOD_patchByteCode = { shou5SCQUAKE_PERIOD_byteCode, 12, nullptr, 0 };
// shou5SCQUAKE_LOCPERIOD1 declared in shou.asm line 1762
static unsigned char shou5SCQUAKE_LOCPERIOD1_byteCode[] = {
	0xDC, 0x0D, 0x10, 0xBB, 0x85, 0x00, // FMUL QWORD PTR [0x85BB10] ; line 1763
};
static PatchByteCode shou5SCQUAKE_LOCPERIOD1_patchByteCode = { shou5SCQUAKE_LOCPERIOD1_byteCode, 6, nullptr, 0 };
// shou5SCQUAKE_LOCPERIOD2 declared in shou.asm line 1765
static unsigned char shou5SCQUAKE_LOCPERIOD2_byteCode[] = {
	0xD8, 0x0D, 0x84, 0xF1, 0x85, 0x00, // FMUL DWORD PTR [0x85F184] ; line 1766
};
static PatchByteCode shou5SCQUAKE_LOCPERIOD2_patchByteCode = { shou5SCQUAKE_LOCPERIOD2_byteCode, 6, nullptr, 0 };
// shouIDK4 declared in shou.asm line 1787
static unsigned char shouIDK4_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6CBAD7 ; line 1788
};
static JumpTarget shouIDK4_jumpData[] = {
	{ 0x001, 0x6CBAD7 },
};
static PatchByteCode shouIDK4_patchByteCode = { shouIDK4_byteCode, 5, shouIDK4_jumpData, 1 };
// shouIDK5 declared in shou.asm line 1792
static unsigned char shouIDK5_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6CC163 ; line 1793
};
static JumpTarget shouIDK5_jumpData[] = {
	{ 0x001, 0x6CC163 },
};
static PatchByteCode shouIDK5_patchByteCode = { shouIDK5_byteCode, 5, shouIDK5_jumpData, 1 };
// shouA236BORGAR_SKIP declared in shou.asm line 1803
static unsigned char shouA236BORGAR_SKIP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C44E4 ; line 1804
};
static JumpTarget shouA236BORGAR_SKIP_jumpData[] = {
	{ 0x001, 0x6C44E4 },
};
static PatchByteCode shouA236BORGAR_SKIP_patchByteCode = { shouA236BORGAR_SKIP_byteCode, 5, shouA236BORGAR_SKIP_jumpData, 1 };
// shouA236BORGAR_1 declared in shou.asm line 1808
static unsigned char shouA236BORGAR_1_byteCode[] = {
	0x66, 0x81, 0xF8, 0x9A, 0x01,       // CMP AX,0x019A ; line 1809
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x6C451F ; line 1811
	0x66, 0x81, 0xF8, 0x9B, 0x01,       // CMP AX,0x019B ; line 1812
	0x0F, 0x84, 0xEF, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1813
	0x66, 0x81, 0xF8, 0x9E, 0x01,       // CMP AX,0x019E ; line 1814
	0x0F, 0x84, 0xE4, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1815
	0x66, 0x81, 0xF8, 0x9F, 0x01,       // CMP AX,0x019F ; line 1816
	0x0F, 0x84, 0xD9, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1817
	0x66, 0x81, 0xF8, 0x21, 0x02,       // CMP AX,0x0221 ; line 1818
	0x0F, 0x84, 0xCE, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1819
	0x66, 0x81, 0xF8, 0x22, 0x02,       // CMP AX,0x0222 ; line 1820
	0x0F, 0x84, 0xC3, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1821
	0x66, 0x81, 0xF8, 0x23, 0x02,       // CMP AX,0x0223 ; line 1822
	0x0F, 0x84, 0xB8, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1823
	0x66, 0x81, 0xF8, 0x24, 0x02,       // CMP AX,0x0224 ; line 1824
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x6C90C5 ; line 1825
	0xE9, 0xA8, 0xFF, 0xFF, 0xFF,       // JMP SHORT LABEL1 ; line 1826
};
static JumpTarget shouA236BORGAR_1_jumpData[] = {
	{ 0x007, 0x6C451F },
	{ 0x054, 0x6C90C5 },
};
static PatchByteCode shouA236BORGAR_1_patchByteCode = { shouA236BORGAR_1_byteCode, 93, shouA236BORGAR_1_jumpData, 2 };
// shouA236BORGAR_2 declared in shou.asm line 1830
static unsigned char shouA236BORGAR_2_byteCode[] = {
	0x66, 0x81, 0xF8, 0x9A, 0x01,       // CMP AX,0x019A ; line 1831
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x6C451F ; line 1833
	0x66, 0x81, 0xF8, 0x9B, 0x01,       // CMP AX,0x019B ; line 1834
	0x0F, 0x84, 0xEF, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1835
	0x66, 0x81, 0xF8, 0x9E, 0x01,       // CMP AX,0x019E ; line 1836
	0x0F, 0x84, 0xE4, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1837
	0x66, 0x81, 0xF8, 0x9F, 0x01,       // CMP AX,0x019F ; line 1838
	0x0F, 0x84, 0xD9, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1839
	0x66, 0x81, 0xF8, 0x21, 0x02,       // CMP AX,0x0221 ; line 1840
	0x0F, 0x84, 0xCE, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1841
	0x66, 0x81, 0xF8, 0x22, 0x02,       // CMP AX,0x0222 ; line 1842
	0x0F, 0x84, 0xC3, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1843
	0x66, 0x81, 0xF8, 0x23, 0x02,       // CMP AX,0x0223 ; line 1844
	0x0F, 0x84, 0xB8, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1845
	0x66, 0x81, 0xF8, 0x24, 0x02,       // CMP AX,0x0224 ; line 1846
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6C4930 ; line 1847
};
static JumpTarget shouA236BORGAR_2_jumpData[] = {
	{ 0x007, 0x6C451F },
	{ 0x053, 0x6C4930 },
};
static PatchByteCode shouA236BORGAR_2_patchByteCode = { shouA236BORGAR_2_byteCode, 87, shouA236BORGAR_2_jumpData, 2 };
// shouA236BORGAR_3 declared in shou.asm line 1851
static unsigned char shouA236BORGAR_3_byteCode[] = {
	0x66, 0x81, 0xF8, 0x9A, 0x01,       // CMP AX,0x019A ; line 1852
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x6C4994 ; line 1854
	0x66, 0x81, 0xF8, 0x9B, 0x01,       // CMP AX,0x019B ; line 1855
	0x0F, 0x84, 0xEF, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1856
	0x66, 0x81, 0xF8, 0x9E, 0x01,       // CMP AX,0x019E ; line 1857
	0x0F, 0x84, 0xE4, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1858
	0x66, 0x81, 0xF8, 0x9F, 0x01,       // CMP AX,0x019F ; line 1859
	0x0F, 0x84, 0xD9, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1860
	0x66, 0x81, 0xF8, 0x21, 0x02,       // CMP AX,0x0221 ; line 1861
	0x0F, 0x84, 0xCE, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1862
	0x66, 0x81, 0xF8, 0x22, 0x02,       // CMP AX,0x0222 ; line 1863
	0x0F, 0x84, 0xC3, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1864
	0x66, 0x81, 0xF8, 0x23, 0x02,       // CMP AX,0x0223 ; line 1865
	0x0F, 0x84, 0xB8, 0xFF, 0xFF, 0xFF, // JE SHORT LABEL1 ; line 1866
	0x66, 0x81, 0xF8, 0x24, 0x02,       // CMP AX,0x0224 ; line 1867
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x6C49A5 ; line 1868
	0xE9, 0xA8, 0xFF, 0xFF, 0xFF,       // JMP SHORT LABEL1 ; line 1869
};
static JumpTarget shouA236BORGAR_3_jumpData[] = {
	{ 0x007, 0x6C4994 },
	{ 0x054, 0x6C49A5 },
};
static PatchByteCode shouA236BORGAR_3_patchByteCode = { shouA236BORGAR_3_byteCode, 93, shouA236BORGAR_3_jumpData, 2 };
// shouA236BORGAR_4A declared in shou.asm line 1873
static unsigned char shouA236BORGAR_4A_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x21, 0x03, // CMP WORD PTR [ESI+0x13C],0x0321 ; line 1874
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1875
	0x68, 0x21, 0x03, 0x00, 0x00,                         // PUSH 0x00000321 ; line 1876
	0xE9, 0x05, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 1877
	0x68, 0x35, 0x03, 0x00, 0x00,                         // PUSH 0x00000335 ; line 1879
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6C4691 ; line 1881
};
static JumpTarget shouA236BORGAR_4A_jumpData[] = {
	{ 0x01F, 0x6C4691 },
};
static PatchByteCode shouA236BORGAR_4A_patchByteCode = { shouA236BORGAR_4A_byteCode, 35, shouA236BORGAR_4A_jumpData, 1 };
// shouA236BORGAR_4B declared in shou.asm line 1884
static unsigned char shouA236BORGAR_4B_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x21, 0x03, // CMP WORD PTR [ESI+0x13C],0x0321 ; line 1885
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1886
	0x68, 0x21, 0x03, 0x00, 0x00,                         // PUSH 0x00000321 ; line 1887
	0xE9, 0x05, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 1888
	0x68, 0x35, 0x03, 0x00, 0x00,                         // PUSH 0x00000335 ; line 1890
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6C4716 ; line 1892
};
static JumpTarget shouA236BORGAR_4B_jumpData[] = {
	{ 0x01F, 0x6C4716 },
};
static PatchByteCode shouA236BORGAR_4B_patchByteCode = { shouA236BORGAR_4B_byteCode, 35, shouA236BORGAR_4B_jumpData, 1 };
// shouA236BORGAR_5 declared in shou.asm line 1895
static unsigned char shouA236BORGAR_5_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0000 ; line 1896
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1897
	0xE9, 0x35, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1898
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 1901
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C9450 ; line 1902
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1904
	0x0F, 0x85, 0xCC, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 1905
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1906
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1907
	0xE9, 0x38, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 1908
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0002 ; line 1911
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6C44E4 ; line 1913
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0003 ; line 1914
	0x0F, 0x84, 0xEB, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 1915
	0xE9, 0x2B, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 1916
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 1918
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x6C3E6F ; line 1919
	0xE9, 0x39, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL6 ; line 1920
	0xE9, 0x57, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL7 ; line 1923
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNL 0x6C3E6F ; line 1924
	0x6A, 0x04,                                                 // PUSH 0x04 ; line 1926
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 1927
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1928
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C39B5 ; line 1929
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x13E],0x000A ; line 1931
	0x0F, 0x84, 0xAC, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 1932
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0B, 0x00,       // CMP WORD PTR [ESI+0x13E],0x000B ; line 1933
	0x0F, 0x84, 0x9D, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 1934
	0xE9, 0x5A, 0xFF, 0xFF, 0xFF,                               // JMP LABEL8 ; line 1935
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0xC7, 0x00,       // CMP WORD PTR [ESI+0x140],0x00C7 ; line 1939
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6C3E6F ; line 1941
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x99, 0x01,       // CMP WORD PTR [ESI+0x140],0x0199 ; line 1942
	0x0F, 0x84, 0xEB, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL12 ; line 1943
	0xE9, 0xAF, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL13 ; line 1944
	0x60,                                                       // PUSHAD ; line 1948
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x35C] ; line 1949
	0xD9, 0x01,                                                 // FLD DWORD PTR [ECX] ; line 1950
	0xD9, 0xEE,                                                 // FLDZ ; line 1951
	0xDE, 0xD9,                                                 // FCOMPP ; line 1952
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1953
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1954
	0x61,                                                       // POPAD ; line 1955
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                         // JNE SHORT LABELEND ; line 1956
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0xD2, 0x00,       // MOV WORD PTR [ESI+0x140],0x00D2 ; line 1957
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C39AF ; line 1959
};
static JumpTarget shouA236BORGAR_5_jumpData[] = {
	{ 0x01F, 0x6C9450 },
	{ 0x054, 0x6C44E4 },
	{ 0x078, 0x6C3E6F },
	{ 0x088, 0x6C3E6F },
	{ 0x093, 0x6C39B5 },
	{ 0x0C5, 0x6C3E6F },
	{ 0x100, 0x6C39AF },
};
static PatchByteCode shouA236BORGAR_5_patchByteCode = { shouA236BORGAR_5_byteCode, 260, shouA236BORGAR_5_jumpData, 7 };
// shouA236BORGAR_5A declared in shou.asm line 1962
static unsigned char shouA236BORGAR_5A_byteCode[] = {
	0xE9, 0x6C, 0x00, 0x00, 0x00,                               // JMP LABELEX ; line 1964
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0000 ; line 1966
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1967
	0xE9, 0x35, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1968
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 1971
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C9450 ; line 1972
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1974
	0x0F, 0x85, 0xCC, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 1975
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1976
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1977
	0xE9, 0x38, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 1978
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0002 ; line 1981
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6C44E4 ; line 1983
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0003 ; line 1984
	0x0F, 0x84, 0xEB, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 1985
	0xE9, 0x2B, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 1986
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 1989
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x6C3E6F ; line 1990
	0xE9, 0x39, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL6 ; line 1991
	0xE9, 0x57, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL7 ; line 1994
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNL 0x6C3E6F ; line 1995
	0x6A, 0x04,                                                 // PUSH 0x04 ; line 1997
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 1998
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1999
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C39B5 ; line 2000
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x13E],0x000A ; line 2002
	0x0F, 0x84, 0xAC, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 2003
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0B, 0x00,       // CMP WORD PTR [ESI+0x13E],0x000B ; line 2004
	0x0F, 0x84, 0x9D, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL4 ; line 2005
	0xE9, 0x5A, 0xFF, 0xFF, 0xFF,                               // JMP LABEL8 ; line 2006
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0xC7, 0x00,       // CMP WORD PTR [ESI+0x140],0x00C7 ; line 2010
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x6C3E6F ; line 2012
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x99, 0x01,       // CMP WORD PTR [ESI+0x140],0x0199 ; line 2013
	0x0F, 0x84, 0xEB, 0xFF, 0xFF, 0xFF,                         // JE SHORT LABEL12 ; line 2014
	0xE9, 0xAF, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL13 ; line 2015
	0x60,                                                       // PUSHAD ; line 2019
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x35C] ; line 2020
	0xD9, 0x01,                                                 // FLD DWORD PTR [ECX] ; line 2021
	0xD9, 0xEE,                                                 // FLDZ ; line 2022
	0xDE, 0xD9,                                                 // FCOMPP ; line 2023
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 2024
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 2025
	0x61,                                                       // POPAD ; line 2026
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                         // JNE SHORT LABELEND ; line 2027
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0xD2, 0x00,       // MOV WORD PTR [ESI+0x140],0x00D2 ; line 2028
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6C39AF ; line 2030
};
static JumpTarget shouA236BORGAR_5A_jumpData[] = {
	{ 0x024, 0x6C9450 },
	{ 0x059, 0x6C44E4 },
	{ 0x07D, 0x6C3E6F },
	{ 0x08D, 0x6C3E6F },
	{ 0x098, 0x6C39B5 },
	{ 0x0CA, 0x6C3E6F },
	{ 0x105, 0x6C39AF },
};
static PatchByteCode shouA236BORGAR_5A_patchByteCode = { shouA236BORGAR_5A_byteCode, 265, shouA236BORGAR_5A_jumpData, 7 };
// shouA236BORGAR_6 declared in shou.asm line 2046
static unsigned char shouA236BORGAR_6_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x35, 0x03, // CMP WORD PTR [ESI+0x13C],0x0335 ; line 2048
	0x0F, 0x85, 0x2E, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL9 ; line 2049
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 2050
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                   // JE SHORT LABEL10 ; line 2051
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0B, 0x00, // CMP WORD PTR [ESI+0x13E],0x0B ; line 2052
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                   // JE SHORT LABEL10 ; line 2053
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL9 ; line 2054
	0xD8, 0x0D, 0x60, 0xB9, 0x85, 0x00,                   // FMUL DWORD PTR [0x85B960] ; line 2056
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL11 ; line 2057
	0xD8, 0x0D, 0x30, 0x88, 0x85, 0x00,                   // FMUL DWORD PTR [0x858830] ; line 2059
	0x90,                                                 // NOP ; line 2061
};
static PatchByteCode shouA236BORGAR_6_patchByteCode = { shouA236BORGAR_6_byteCode, 68, nullptr, 0 };
// shouA236BORGAR_IDK1 declared in shou.asm line 2069
static unsigned char shouA236BORGAR_IDK1_byteCode[] = {
	0xDD, 0x05, 0x90, 0x89, 0x85, 0x00, // FLD QWORD PTR [0x858990] ; line 2070
};
static PatchByteCode shouA236BORGAR_IDK1_patchByteCode = { shouA236BORGAR_IDK1_byteCode, 6, nullptr, 0 };
// shouA236BORGAR_IDK2 declared in shou.asm line 2072
static unsigned char shouA236BORGAR_IDK2_byteCode[] = {
	0xD9, 0x05, 0xD0, 0x50, 0x89, 0x00, // FLD DWORD PTR [0x8950D0] ; line 2073
};
static PatchByteCode shouA236BORGAR_IDK2_patchByteCode = { shouA236BORGAR_IDK2_byteCode, 6, nullptr, 0 };
// shouD236STARTUP_IDK declared in shou.asm line 2083
static unsigned char shouD236STARTUP_IDK_byteCode[] = {
	0xDC, 0x05, 0x10, 0xCE, 0x85, 0x00, // FADD QWORD PTR [0x85CE10] ; line 2084
};
static PatchByteCode shouD236STARTUP_IDK_patchByteCode = { shouD236STARTUP_IDK_byteCode, 6, nullptr, 0 };
// shouA22GUARD_DISABLEAIR1 declared in shou.asm line 2096
static unsigned char shouA22GUARD_DISABLEAIR1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C2CD0 ; line 2097
};
static JumpTarget shouA22GUARD_DISABLEAIR1_jumpData[] = {
	{ 0x001, 0x6C2CD0 },
};
static PatchByteCode shouA22GUARD_DISABLEAIR1_patchByteCode = { shouA22GUARD_DISABLEAIR1_byteCode, 5, shouA22GUARD_DISABLEAIR1_jumpData, 1 };
// shouA22GUARD_DISABLEAIR2 declared in shou.asm line 2099
static unsigned char shouA22GUARD_DISABLEAIR2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C2DC0 ; line 2100
};
static JumpTarget shouA22GUARD_DISABLEAIR2_jumpData[] = {
	{ 0x001, 0x6C2DC0 },
};
static PatchByteCode shouA22GUARD_DISABLEAIR2_patchByteCode = { shouA22GUARD_DISABLEAIR2_byteCode, 5, shouA22GUARD_DISABLEAIR2_jumpData, 1 };
// shouA22GUARD_SPOT declared in shou.asm line 2105
static unsigned char shouA22GUARD_SPOT_byteCode[] = {
	0xD9, 0x05, 0xA8, 0x75, 0x85, 0x00, // FLD DWORD PTR [0x8575A8] ; line 2106
};
static PatchByteCode shouA22GUARD_SPOT_patchByteCode = { shouA22GUARD_SPOT_byteCode, 6, nullptr, 0 };
// shouA22GUARD_MOVE1 declared in shou.asm line 2110
static unsigned char shouA22GUARD_MOVE1_byteCode[] = {
	0xD8, 0xD9, // FCOMP ST1 ; line 2111
};
static PatchByteCode shouA22GUARD_MOVE1_patchByteCode = { shouA22GUARD_MOVE1_byteCode, 2, nullptr, 0 };
// shouA22GUARD_MOVE2 declared in shou.asm line 2113
static unsigned char shouA22GUARD_MOVE2_byteCode[] = {
	0xD8, 0xD9,                         // FCOMP ST1 ; line 2114
	0xD9, 0xEE,                         // FLDZ ; line 2115
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF0] ; line 2116
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6C61F8 ; line 2117
};
static JumpTarget shouA22GUARD_MOVE2_jumpData[] = {
	{ 0x00B, 0x6C61F8 },
};
static PatchByteCode shouA22GUARD_MOVE2_patchByteCode = { shouA22GUARD_MOVE2_byteCode, 15, shouA22GUARD_MOVE2_jumpData, 1 };
// shouA22GUARD_COUNT declared in shou.asm line 2122
static unsigned char shouA22GUARD_COUNT_byteCode[] = {
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871538] ; line 2123
};
static PatchByteCode shouA22GUARD_COUNT_patchByteCode = { shouA22GUARD_COUNT_byteCode, 6, nullptr, 0 };
// shouA22GUARD_IDKTBH0 declared in shou.asm line 2127
static unsigned char shouA22GUARD_IDKTBH0_byteCode[] = {
	0x0F, 0x87, 0x09, 0x00, 0x00, 0x00,                   // JA SHORT LABELY ; line 2128
	0x80, 0xC0, 0x0A,                                     // ADD AL,0x0A ; line 2129
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00,                   // MOV [ESI+0x113],AL ; line 2130
	0x39, 0xAE, 0x90, 0x01, 0x00, 0x00,                   // CMP [ESI+0x190],EBP ; line 2132
	0x0F, 0x84, 0x0F, 0x00, 0x00, 0x00,                   // JE SHORT LABELZ ; line 2133
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x96, 0x00, // MOV WORD PTR [ESI+0x140],0x0096 ; line 2134
	0x89, 0xAE, 0x90, 0x01, 0x00, 0x00,                   // MOV [ESI+0x190],EBP ; line 2135
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x95, 0x00, // CMP WORD PTR [ESI+0x140],0x0095 ; line 2139
	0x0F, 0x8F, 0xAF, 0x00, 0x00, 0x00,                   // JG LABEL2 ; line 2140
	0x60,                                                 // PUSHAD ; line 2141
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                   // MOV EBX,[ESI+0x398] ; line 2142
	0x8B, 0x9B, 0x70, 0x01, 0x00, 0x00,                   // MOV EBX,[EBX+0x170] ; line 2143
	0xD9, 0x83, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xF0] ; line 2144
	0xDD, 0x05, 0x20, 0x7B, 0x85, 0x00,                   // FLD QWORD PTR [0x857B20] ; line 2145
	0xDE, 0xD9,                                           // FCOMPP ; line 2146
	0xDF, 0xE0,                                           // FNSTSW AX ; line 2147
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 2148
	0x0F, 0x85, 0x88, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2149
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xEC] ; line 2150
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 2151
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FADD QWORD PTR [0x867830] ; line 2152
	0xDE, 0xD9,                                           // FCOMPP ; line 2153
	0xDF, 0xE0,                                           // FNSTSW AX ; line 2154
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 2155
	0x0F, 0x85, 0x69, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2156
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 2157
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xEC] ; line 2158
	0xDC, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FADD QWORD PTR [0x867830] ; line 2159
	0xDE, 0xD9,                                           // FCOMPP ; line 2160
	0xDF, 0xE0,                                           // FNSTSW AX ; line 2161
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 2162
	0x0F, 0x85, 0x4A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2163
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x96, 0x00, // MOV WORD PTR [ESI+0x140],0x0096 ; line 2164
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x09, 0x00, // MOV WORD PTR [ESI+0x142],0x0009 ; line 2165
	0x66, 0x8B, 0x8B, 0x9E, 0x04, 0x00, 0x00,             // MOV CX,[EBX+0x49E] ; line 2168
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x398] ; line 2169
	0x8A, 0x80, 0xA9, 0x06, 0x00, 0x00,                   // MOV AL,[EAX+0x6A9] ; line 2170
	0x80, 0xC0, 0x03,                                     // ADD AL,0x03 ; line 2171
	0x0F, 0xB6, 0xC0,                                     // MOVZX EAX,AL ; line 2172
	0x6B, 0xC0, 0x4B,                                     // IMUL EAX,EAX,0x4B ; line 2173
	0x66, 0x39, 0xC1,                                     // CMP CX,AX ; line 2174
	0x0F, 0x8F, 0x09, 0x00, 0x00, 0x00,                   // JG SHORT LABEL3 ; line 2175
	0x66, 0xB9, 0x00, 0x00,                               // MOV CX,0x0000 ; line 2176
	0xE9, 0x03, 0x00, 0x00, 0x00,                         // JMP LABEL4 ; line 2177
	0x66, 0x29, 0xC1,                                     // SUB CX,AX ; line 2179
	0x66, 0x89, 0x8B, 0x9E, 0x04, 0x00, 0x00,             // MOV [EBX+0x49E],CX ; line 2181
	0x61,                                                 // POPAD ; line 2184
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6C6181 ; line 2186
};
static JumpTarget shouA22GUARD_IDKTBH0_jumpData[] = {
	{ 0x0E9, 0x6C6181 },
};
static PatchByteCode shouA22GUARD_IDKTBH0_patchByteCode = { shouA22GUARD_IDKTBH0_byteCode, 237, shouA22GUARD_IDKTBH0_jumpData, 1 };
// shouA22GUARD_DISABLEAIRLV declared in shou.asm line 2192
static unsigned char shouA22GUARD_DISABLEAIRLV_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6C27BE ; line 2193
};
static JumpTarget shouA22GUARD_DISABLEAIRLV_jumpData[] = {
	{ 0x001, 0x6C27BE },
};
static PatchByteCode shouA22GUARD_DISABLEAIRLV_patchByteCode = { shouA22GUARD_DISABLEAIRLV_byteCode, 5, shouA22GUARD_DISABLEAIRLV_jumpData, 1 };
// shouA22GUARD_REDIR declared in shou.asm line 2198
static unsigned char shouA22GUARD_REDIR_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6B2AA6 ; line 2199
};
static JumpTarget shouA22GUARD_REDIR_jumpData[] = {
	{ 0x001, 0x6B2AA6 },
};
static PatchByteCode shouA22GUARD_REDIR_patchByteCode = { shouA22GUARD_REDIR_byteCode, 5, shouA22GUARD_REDIR_jumpData, 1 };
// shouA22GUARD_DUR declared in shou.asm line 2204
static unsigned char shouA22GUARD_DUR_byteCode[] = {
	0x6B, 0xC9, 0x28,             // IMUL CX,CX,0x28 ; line 2205
	0x66, 0x81, 0xC1, 0x96, 0x00, // ADD CX,0x0096 ; line 2206
};
static PatchByteCode shouA22GUARD_DUR_patchByteCode = { shouA22GUARD_DUR_byteCode, 8, nullptr, 0 };
// shouA22GUARD_MULTI declared in shou.asm line 2210
static unsigned char shouA22GUARD_MULTI_byteCode[] = {
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 2211
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02,       // CMP WORD PTR [ECX+0x13C],0x020D ; line 2212
	0x0F, 0x8C, 0x31, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 2213
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0x11, 0x02,       // CMP WORD PTR [ECX+0x13C],0x0211 ; line 2214
	0x0F, 0x8F, 0x22, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 2215
	0x81, 0xB9, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x144],0x00 ; line 2216
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 2217
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x96, 0x00,       // MOV WORD PTR [ESI+0x140],0x0096 ; line 2218
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x09, 0x00,       // MOV WORD PTR [ESI+0x142],0x0009 ; line 2219
	0x90,                                                       // NOP ; line 2221
};
static PatchByteCode shouA22GUARD_MULTI_patchByteCode = { shouA22GUARD_MULTI_byteCode, 71, nullptr, 0 };


// shou.asm
std::array<PatchSkeleton, 260> patchList = {
	PatchSkeleton{ shouID_FIX_patchByteCode,                     (void *)0x6AB6EB,  7}, // Declared line 21. Patch on CMP [ESI+0x196], DI
	PatchSkeleton{ shouWALK_patchByteCode,                       (void *)0x6BFB2D,  6}, // Declared line 27. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ shouWALK_patchByteCode,                       (void *)0x6AB9D4,  6}, // Declared line 29. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ shouHJ8Y_patchByteCode,                       (void *)0x6ACDEE,  6}, // Declared line 38. Patch on FLD ST, DWORD PTR [0x85BB30]
	PatchSkeleton{ shouHJ9Y_patchByteCode,                       (void *)0x6ACF34,  6}, // Declared line 43. Patch on FLD ST, DWORD PTR [0x85BA68]
	PatchSkeleton{ shouHJ8GRAV_patchByteCode,                    (void *)0x6ACDFA,  6}, // Declared line 49. Patch on FLD ST, DWORD PTR [0x85C228]
	PatchSkeleton{ shouHJMAXFALL_patchByteCode,                  (void *)0x6ACD60,  6}, // Declared line 54. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ shouHJMAXFALL_patchByteCode,                  (void *)0x6ACEA2,  6}, // Declared line 56. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ shouFLIGHT_INIT_patchByteCode,                (void *)0x6BFB9A,  6}, // Declared line 68. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shouFLIGHT_ACC_patchByteCode,                 (void *)0x6AD48E,  6}, // Declared line 73. Patch on FADD ST, QWORD PTR [0x85A280]
	PatchSkeleton{ shouFLIGHT_FIN_patchByteCode,                 (void *)0x6AD4A2,  6}, // Declared line 78. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shouFLIGHT_ETC_patchByteCode,                 (void *)0x6AD4AE,  5}, // Declared line 84. Patch on TEST AH, 5
	PatchSkeleton{ shouGROUNDDASH_SPD_patchByteCode,             (void *)0x6AC560,  6}, // Declared line 90. Patch on FLD ST, DWORD PTR [0x85C22C]
	PatchSkeleton{ shouGROUNDDASH_ACC_patchByteCode,             (void *)0x6ACC80,  6}, // Declared line 95. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ shouGROUNDDASH_DUR_patchByteCode,             (void *)0x6AC51D,  6}, // Declared line 101. Patch on CMP AX, 0x2D
	PatchSkeleton{ shouBACKDASH_SPD_patchByteCode,               (void *)0x6AC7B1,  6}, // Declared line 107. Patch on FLD ST, DWORD PTR [0x85B960]
	PatchSkeleton{ shouBACKDASH_ACC_patchByteCode,               (void *)0x6AC634,  6}, // Declared line 112. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ shouBACKDASH_Y_patchByteCode,                 (void *)0x6AC7C2,  6}, // Declared line 117. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ shou6A_X_patchByteCode,                       (void *)0x6AEB0F,  6}, // Declared line 123. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shou6A_X1_patchByteCode,                      (void *)0x6AEAD2,  6}, // Declared line 127. Patch on FSUB ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ shouC6A_X_patchByteCode,                      (void *)0x6AEBA1,  6}, // Declared line 132. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ shouC6A_X1_patchByteCode,                     (void *)0x6AEB5B,  6}, // Declared line 136. Patch on FSUB ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ shouC6A_POINT_patchByteCode,                  (void *)0x6AEA49,  8}, // Declared line 142. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ shou66A_X_patchByteCode,                      (void *)0x6BFC12,  6}, // Declared line 148. Patch on FLD ST, DWORD PTR [0x85C5B8]
	PatchSkeleton{ shou66A_1_patchByteCode,                      (void *)0x6AEE91,  7}, // Declared line 153. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ shou66B_patchByteCode,                        (void *)0x6B08A9, 10}, // Declared line 160. Patch on CMP WORD PTR [ESI+0x13E], 3
	PatchSkeleton{ shou66C_X_patchByteCode,                      (void *)0x6C0048,  6}, // Declared line 166. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shou66C_ACC_patchByteCode,                    (void *)0x6B1343,  6}, // Declared line 170. Patch on FSUB ST, QWORD PTR [0x858EA8]
	PatchSkeleton{ shou66C_1_patchByteCode,                      (void *)0x6B1333,  8}, // Declared line 174. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ shou3A_X_patchByteCode,                       (void *)0x6AED51,  6}, // Declared line 180. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ shou3A_X1_patchByteCode,                      (void *)0x6AEC98,  6}, // Declared line 184. Patch on FSUB ST, QWORD PTR [0x859910]
	PatchSkeleton{ shouC3A_DISABLE_patchByteCode,                (void *)0x6AED66,  8}, // Declared line 189. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ shouJ6A_RECOVGRAV_patchByteCode,              (void *)0x6AF030,  6}, // Declared line 195. Patch on FLD ST, DWORD PTR [0x85B85C]
	PatchSkeleton{ shouJ6A_SOUND_patchByteCode,                  (void *)0x6AF009,  5}, // Declared line 200. Patch on MOV ECX, 0xA
	PatchSkeleton{ shouJ8A_KEY_patchByteCode,                    (void *)0x6AF3D6,  8}, // Declared line 206. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ shou5B_ANG1_patchByteCode,                    (void *)0x6AF754,  6}, // Declared line 226. Patch on FCOMP ST, QWORD PTR [0x85C450]
	PatchSkeleton{ shou5B_ANG2_patchByteCode,                    (void *)0x6AF761,  6}, // Declared line 227. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ shou5B_ANG3_patchByteCode,                    (void *)0x6AF773,  6}, // Declared line 229. Patch on FCOMP ST, QWORD PTR [0x867830]
	PatchSkeleton{ shou5B_ANG4_patchByteCode,                    (void *)0x6AF780,  6}, // Declared line 230. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ shou5B_PROJSPD_patchByteCode,                 (void *)0x6AF7A2,  6}, // Declared line 232. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shou2B_BULLETTYPE_patchByteCode,              (void *)0x6AFCFF,  6}, // Declared line 238. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ shou6B_SPD_patchByteCode,                     (void *)0x6C3C91,  6}, // Declared line 247. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ shou6B_SPD_patchByteCode,                     (void *)0x6C3EC4,  6}, // Declared line 248. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ shou6B_SPD1_patchByteCode,                    (void *)0x6C3EB3,  6}, // Declared line 249. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ shou6B_SPD1_patchByteCode,                    (void *)0x6C3C80,  6}, // Declared line 250. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6AF9B9,  6}, // Declared line 252. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6AF9D4,  6}, // Declared line 253. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6AFB2D,  6}, // Declared line 255. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6AFB48,  6}, // Declared line 256. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B01D8,  6}, // Declared line 258. Patch on FLD ST, DWORD PTR [0x85B908]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B01F3,  6}, // Declared line 259. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B0367,  6}, // Declared line 260. Patch on FLD ST, DWORD PTR [0x85B908]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B0382,  6}, // Declared line 261. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shouJ2B_SPD_patchByteCode,                    (void *)0x6B06AA,  6}, // Declared line 267. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ shouJ2B_ANG1_patchByteCode,                   (void *)0x6B0689,  6}, // Declared line 276. Patch on FLD ST, DWORD PTR [0x871540]
	PatchSkeleton{ shouJ2B_ANG2_patchByteCode,                   (void *)0x6B067C,  6}, // Declared line 277. Patch on FCOMP ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ shouJ2B_BULLETTYPE_patchByteCode,             (void *)0x6B06B9,  6}, // Declared line 283. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ shouJ2B_GRAVPOINT_patchByteCode,              (void *)0x6B0747,  8}, // Declared line 288. Patch on CMP WORD PTR [ESI+0x140], 0xF
	PatchSkeleton{ shouJC2B_KEY_patchByteCode,                   (void *)0x6B0566,  8}, // Declared line 297. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ shou5B_ANG1_patchByteCode,                    (void *)0x6AFEEF,  6}, // Declared line 300. Patch on FCOMP ST, QWORD PTR [0x85C450]
	PatchSkeleton{ shou5B_ANG2_patchByteCode,                    (void *)0x6AFEFC,  6}, // Declared line 301. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ shou5B_ANG3_patchByteCode,                    (void *)0x6AFF0E,  6}, // Declared line 303. Patch on FCOMP ST, QWORD PTR [0x867830]
	PatchSkeleton{ shou5B_ANG4_patchByteCode,                    (void *)0x6AFF1B,  6}, // Declared line 304. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ shou5B_PROJSPD_patchByteCode,                 (void *)0x6AFF3D,  6}, // Declared line 306. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B0C53,  6}, // Declared line 309. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B118C,  6}, // Declared line 311. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C45CE,  6}, // Declared line 315. Patch on FADD ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ shou5C_MUL_patchByteCode,                     (void *)0x6C458A,  6}, // Declared line 338. Patch on FMUL ST, QWORD PTR [0x85C068]
	PatchSkeleton{ shou5C_SKIPDEATH_patchByteCode,               (void *)0x6C42CF,  6}, // Declared line 343. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ shouj2A_MOMENTUM_patchByteCode,               (void *)0x6AF369,  8}, // Declared line 350. Patch on FLDZ
	PatchSkeleton{ shouD236_1_patchByteCode,                     (void *)0x6C9077,  6}, // Declared line 356. Patch on FLD ST, DWORD PTR [0x87153C]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C908E,  6}, // Declared line 358. Patch on FLD ST, DWORD PTR [0x85BA68]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C923C,  6}, // Declared line 360. Patch on FADD ST, QWORD PTR [0x871510]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C90A1,  6}, // Declared line 364. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C9193,  6}, // Declared line 366. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C9209,  6}, // Declared line 367. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C91DC,  6}, // Declared line 368. Patch on FMUL ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ shouD236_2_patchByteCode,                     (void *)0x6C9242,  8}, // Declared line 373. Patch on FSTP DWORD PTR [ESP+0x14], ST
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C9166,  6}, // Declared line 374. Patch on FMUL ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C912C,  6}, // Declared line 375. Patch on FLD ST, DWORD PTR [0x85B984]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C911F,  6}, // Declared line 376. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ shouD236_3_patchByteCode,                     (void *)0x6C9100,  9}, // Declared line 381. Patch on FSTP DWORD PTR [ESP+0xA4], ST
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C8C49,  6}, // Declared line 383. Patch on ADD [ESI+0x111], AL
	PatchSkeleton{ shouD236_COLOR1_patchByteCode,                (void *)0x6C8EB9,  6}, // Declared line 402. Patch on FSTP DWORD PTR [ESI+0x120], ST
	PatchSkeleton{ shouD236_COLOR2_patchByteCode,                (void *)0x6C8DF9,  6}, // Declared line 403. Patch on JNE 0x006CB1E5
	PatchSkeleton{ shouD236_ONHIT_patchByteCode,                 (void *)0x6C8D22,  7}, // Declared line 425. Patch on CMP DWORD PTR [ESI+0x39C], 0
	PatchSkeleton{ shouD623_KEY_patchByteCode,                   (void *)0x6B4428,  8}, // Declared line 434. Patch on CMP WORD PTR [ESI+0x140], 1
	PatchSkeleton{ shouD623_patchByteCode,                       (void *)0x6B44B0,  8}, // Declared line 479. Patch on CMP WORD PTR [ESI+0x13E], 1
	PatchSkeleton{ shouD623_SKIP1_patchByteCode,                 (void *)0x6B4449,  6}, // Declared line 487. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shouD623_DECEL_patchByteCode,                 (void *)0x6B4357,  6}, // Declared line 492. Patch on FSUB ST, QWORD PTR [0x858820]
	PatchSkeleton{ shouD623_KEY_patchByteCode,                   (void *)0x6B4885,  7}, // Declared line 496. Patch on CMP [ESI+0x140], DI
	PatchSkeleton{ shouD623_patchByteCode,                       (void *)0x6B4917,  7}, // Declared line 498. Patch on CMP [ESI+0x13E], DI
	PatchSkeleton{ shouD623_SKIP2_patchByteCode,                 (void *)0x6B48A9,  6}, // Declared line 501. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shouD623_DECEL_patchByteCode,                 (void *)0x6B47C4,  6}, // Declared line 503. Patch on FSUB ST, QWORD PTR [0x858820]
	PatchSkeleton{ shou5C_GRAZERES_patchByteCode,                (void *)0x6C4268,  9}, // Declared line 517. Patch on PUSH 5
	PatchSkeleton{ shou2C_ANG1_patchByteCode,                    (void *)0x6B0C24,  6}, // Declared line 529. Patch on FMUL ST, QWORD PTR [0x85C068]
	PatchSkeleton{ shou2C_ANG2_patchByteCode,                    (void *)0x6B0C31,  6}, // Declared line 530. Patch on FADD ST, QWORD PTR [0x859630]
	PatchSkeleton{ shou2C_ANG3_patchByteCode,                    (void *)0x6B0D7A,  6}, // Declared line 531. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shou2C_ANG1_patchByteCode,                    (void *)0x6B1157,  6}, // Declared line 533. Patch on FMUL ST, QWORD PTR [0x85C068]
	PatchSkeleton{ shou2C_ANG2_patchByteCode,                    (void *)0x6B1167,  6}, // Declared line 534. Patch on FADD ST, QWORD PTR [0x859630]
	PatchSkeleton{ shou2C_ANG3_patchByteCode,                    (void *)0x6B12BC,  6}, // Declared line 535. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shouD22_LV_patchByteCode,                     (void *)0x6C5A78,  7}, // Declared line 543. Patch on CMP BYTE PTR [EAX+0x6A5], 1
	PatchSkeleton{ shouD22_1_patchByteCode,                      (void *)0x6C5A23,  6}, // Declared line 560. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ shouD22_2_patchByteCode,                      (void *)0x6C5D28,  6}, // Declared line 596. Patch on MOV ECX, [ESI+0x154]
	PatchSkeleton{ shouD22_BROCKBACK_patchByteCode,              (void *)0x6B25BB,  6}, // Declared line 611. Patch on IMUL EDX, 0x96
	PatchSkeleton{ shouD22_BROCKBACK_patchByteCode,              (void *)0x6B260C,  6}, // Declared line 613. Patch on IMUL EDX, 0x96
	PatchSkeleton{ shouD22_CCAST_patchByteCode,                  (void *)0x6B2702,  6}, // Declared line 615. Patch on IMUL EDX, 0x12C
	PatchSkeleton{ shouD22_CCASTIDK_patchByteCode,               (void *)0x6B2753,  6}, // Declared line 616. Patch on IMUL EDX, 0x12C
	PatchSkeleton{ shouD22_BROCKBACK_patchByteCode,              (void *)0x6B2852,  6}, // Declared line 618. Patch on IMUL EDX, 0x96
	PatchSkeleton{ shouD22_BROCKBACK_patchByteCode,              (void *)0x6B28A2,  6}, // Declared line 619. Patch on IMUL EDX, 0x96
	PatchSkeleton{ shouD22_CCAST_patchByteCode,                  (void *)0x6B2A31,  6}, // Declared line 621. Patch on IMUL EDX, 0x12C
	PatchSkeleton{ shouD22_CCASTIDK_patchByteCode,               (void *)0x6B2A81,  6}, // Declared line 622. Patch on IMUL EDX, 0x12C
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B26E3,  6}, // Declared line 624. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B2A12,  6}, // Declared line 626. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ shouD214_DUR_patchByteCode,                   (void *)0x6C4EEA,  7}, // Declared line 633. Patch on CMP DWORD PTR [ESI+0x144], 3
	PatchSkeleton{ shouD214_ANG_patchByteCode,                   (void *)0x6C4AAD,  6}, // Declared line 638. Patch on FSUB ST, QWORD PTR [0x85BBD8]
	PatchSkeleton{ shouD214_PERIOD_patchByteCode,                (void *)0x6C4A82,  5}, // Declared line 643. Patch on MOV ECX, 5
	PatchSkeleton{ shouD214_DURLV_patchByteCode,                 (void *)0x6C4A91,  7}, // Declared line 658. Patch on PUSH 0x14
	PatchSkeleton{ shouD214_LV1_patchByteCode,                   (void *)0x6C4A65,  5}, // Declared line 666. Patch on MOV ECX, 0x19
	PatchSkeleton{ shouD214_LV2_patchByteCode,                   (void *)0x6C4A6E,  5}, // Declared line 667. Patch on MOV ECX, 0x23
	PatchSkeleton{ shouA214BOOM_SKIP1_patchByteCode,             (void *)0x6C56ED,  5}, // Declared line 675. Patch on PUSH 5
	PatchSkeleton{ shouA214BOOM_BOX_patchByteCode,               (void *)0x6B1FBE,  6}, // Declared line 688. Patch on CMP [ESI+0x144], EDI
	PatchSkeleton{ shouA214BOOM_BOX_patchByteCode,               (void *)0x6B21F5,  6}, // Declared line 690. Patch on CMP [ESI+0x144], EDI
	PatchSkeleton{ shou3SCWEATHER_1_patchByteCode,               (void *)0x6CD8FC,  6}, // Declared line 707. Patch on FMUL ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ shou3SCWEATHER_3_patchByteCode,               (void *)0x6B8384, 12}, // Declared line 750. Patch on AND EAX, 0x80000001
	PatchSkeleton{ shou3SCWEATHER_3_patchByteCode,               (void *)0x6B8635, 12}, // Declared line 752. Patch on AND EAX, 0x80000001
	PatchSkeleton{ shou3SCWEATHER_2_patchByteCode,               (void *)0x6CD804,  7}, // Declared line 755. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ shou3SCWEATHER_ONHIT_patchByteCode,           (void *)0x6CD745,  6}, // Declared line 760. Patch on JNE 0x006CD7CC
	PatchSkeleton{ shou3SCWEATHER_BLOCK_patchByteCode,           (void *)0x6CD7D7,  9}, // Declared line 764. Patch on JNE SHORT 0x006CD804
	PatchSkeleton{ shou3SCWEATHER_4_patchByteCode,               (void *)0x6CD80B,  6}, // Declared line 769. Patch on JE 0x006CD8AE
	PatchSkeleton{ shouA236REKKA_patchByteCode,                  (void *)0x6B3CD0,  6}, // Declared line 964. Patch on TEST AL, AL
	PatchSkeleton{ shouA236REKKA_LETSNOT_patchByteCode,          (void *)0x6B4009,  6}, // Declared line 969. Patch on TEST AL, AL
	PatchSkeleton{ shouA236REKKA_LUL_patchByteCode,              (void *)0x6B3CDE,  7}, // Declared line 1022. Patch on CMP [ESI+0x13E], DI
	PatchSkeleton{ shouA236REKKA_AREYOUKIDDINGME1_patchByteCode, (void *)0x6B3CEB,  7}, // Declared line 1023. Patch on CMP [ESI+0x142], DI
	PatchSkeleton{ shouA236REKKA_LUL_patchByteCode,              (void *)0x6B4017,  7}, // Declared line 1024. Patch on CMP [ESI+0x13E], DI
	PatchSkeleton{ shouA236REKKA_AREYOUKIDDINGME2_patchByteCode, (void *)0x6B4024,  7}, // Declared line 1025. Patch on CMP [ESI+0x142], DI
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B3E2F, 10}, // Declared line 1033. Patch on MOV DWORD PTR [ESI+0x190], 1
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B4168, 10}, // Declared line 1036. Patch on MOV DWORD PTR [ESI+0x190], 1
	PatchSkeleton{ shouA236REKKA_EFFECTS_patchByteCode,          (void *)0x6B3E1F,  8}, // Declared line 1045. Patch on CMP WORD PTR [ESI+0x140], 8
	PatchSkeleton{ shouA236REKKA_EFFECTS_patchByteCode,          (void *)0x6B4158,  8}, // Declared line 1046. Patch on CMP WORD PTR [ESI+0x140], 8
	PatchSkeleton{ shouA236REKKA_EFFECTS2_patchByteCode,         (void *)0x6B3FE3,  9}, // Declared line 1053. Patch on PUSH 0x11
	PatchSkeleton{ shouA236REKKA_EFFECTS2_patchByteCode,         (void *)0x6B4315,  9}, // Declared line 1054. Patch on PUSH 0x11
	PatchSkeleton{ shouA236REKKA_SFX_patchByteCode,              (void *)0x6C9F56,  6}, // Declared line 1059. Patch on JG 0x006CA3C6
	PatchSkeleton{ shouA623SLASHY1_patchByteCode,                (void *)0x6B4E5E,  5}, // Declared line 1071. Patch on MOV EBX, 7
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B4F52, 12}, // Declared line 1073. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ shouIDK0_patchByteCode,                       (void *)0x6B513B,  6}, // Declared line 1083. Patch on JNE 0x006B5289
	PatchSkeleton{ shouIDK1_patchByteCode,                       (void *)0x6B52AB,  6}, // Declared line 1089. Patch on JL 0x006BA7FF
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C70FA,  6}, // Declared line 1096. Patch on JNE 0x006C729E
	PatchSkeleton{ shouA22_1_patchByteCode,                      (void *)0x6C71FC,  6}, // Declared line 1101. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C7601,  6}, // Declared line 1104. Patch on JNE 0x006C7937
	PatchSkeleton{ shouA22_IDK1_patchByteCode,                   (void *)0x6C7615,  6}, // Declared line 1109. Patch on JNE 0x006C7805
	PatchSkeleton{ shouA22_patchByteCode,                        (void *)0x6C7805,  6}, // Declared line 1132. Patch on CMP [ESI+0x144], EBX
	PatchSkeleton{ shouA22_TOTAL_patchByteCode,                  (void *)0x6C7A63,  6}, // Declared line 1137. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ shouIDK1A_patchByteCode,                      (void *)0x6C8A86,  6}, // Declared line 1160. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C8A8C,  6}, // Declared line 1162. Patch on FADD ST, QWORD PTR [0x85C070]
	PatchSkeleton{ shouIDK1_DUR_patchByteCode,                   (void *)0x6C8AA6,  6}, // Declared line 1188. Patch on MOV AL, [ESI+0x113]
	PatchSkeleton{ shouIDK1_SPD_patchByteCode,                   (void *)0x6C8ABC,  6}, // Declared line 1194. Patch on MOV EAX, [ESI+0x35C]
	PatchSkeleton{ shouIDK1_POS_patchByteCode,                   (void *)0x6C6D13,  6}, // Declared line 1212. Patch on FADD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ shouIDK1_IDK_patchByteCode,                   (void *)0x6C7107,  6}, // Declared line 1221. Patch on FSTP DWORD PTR [ESI+0xF0], ST
	PatchSkeleton{ shouA623CHARGE_BX_patchByteCode,              (void *)0x6B5784,  6}, // Declared line 1245. Patch on FLD ST, DWORD PTR [0x85882C]
	PatchSkeleton{ shouA623CHARGE_BY_patchByteCode,              (void *)0x6B579D,  6}, // Declared line 1247. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shouA623CHARGE_DEC1_patchByteCode,            (void *)0x6B56A7,  6}, // Declared line 1249. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ shouA623CHARGE_DEC2_patchByteCode,            (void *)0x6B56B9,  6}, // Declared line 1250. Patch on FSUB ST, QWORD PTR [0x867840]
	PatchSkeleton{ shouA623CHARGE_BX_patchByteCode,              (void *)0x6B5A0A,  6}, // Declared line 1253. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ shouA623CHARGE_BY_patchByteCode,              (void *)0x6B5A23,  6}, // Declared line 1254. Patch on FLD ST, DWORD PTR [0x85C230]
	PatchSkeleton{ shouA623CHARGE_DEC1_patchByteCode,            (void *)0x6B592D,  6}, // Declared line 1255. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ shouA623CHARGE_DEC2_patchByteCode,            (void *)0x6B593F,  6}, // Declared line 1256. Patch on FSUB ST, QWORD PTR [0x867840]
	PatchSkeleton{ shouA623CHARGE_BX_patchByteCode,              (void *)0x6B5C59,  6}, // Declared line 1259. Patch on FLD ST, DWORD PTR [0x85882C]
	PatchSkeleton{ shouA623CHARGE_BY_patchByteCode,              (void *)0x6B5C79,  6}, // Declared line 1260. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ shouA623CHARGE_DEC1_patchByteCode,            (void *)0x6B5B8C,  6}, // Declared line 1261. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ shouA623CHARGE_DEC2_patchByteCode,            (void *)0x6B5B9E,  6}, // Declared line 1262. Patch on FSUB ST, QWORD PTR [0x867840]
	PatchSkeleton{ shouA623CHARGE_BX_patchByteCode,              (void *)0x6B5EB4,  6}, // Declared line 1265. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ shouA623CHARGE_BY_patchByteCode,              (void *)0x6B5ED4,  6}, // Declared line 1266. Patch on FLD ST, DWORD PTR [0x85C230]
	PatchSkeleton{ shouA623CHARGE_DEC1_patchByteCode,            (void *)0x6B5DE7,  6}, // Declared line 1267. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ shouA623CHARGE_DEC2_patchByteCode,            (void *)0x6B5DF9,  6}, // Declared line 1268. Patch on FSUB ST, QWORD PTR [0x867840]
	PatchSkeleton{ shou3SCCHARGE_1_patchByteCode,                (void *)0x6B6B2B,  6}, // Declared line 1277. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shou3SCCHARGE_2_patchByteCode,                (void *)0x6B6B3B,  6}, // Declared line 1278. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ shouA623CHARGE_DEC1_patchByteCode,            (void *)0x6B68DD,  6}, // Declared line 1279. Patch on FLD ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ shou5SCCHARGE_X_patchByteCode,                (void *)0x6B78D0,  6}, // Declared line 1289. Patch on FLD ST, DWORD PTR [0x85882C]
	PatchSkeleton{ shou5SCCHARGE_Y_patchByteCode,                (void *)0x6B78E0,  6}, // Declared line 1290. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B74E1,  6}, // Declared line 1292. Patch on FADD ST, QWORD PTR [0x85BA58]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B751F,  6}, // Declared line 1293. Patch on FADD ST, QWORD PTR [0x85BA58]
	PatchSkeleton{ shou5SCCHARGE_patchByteCode,                  (void *)0x6B7431,  7}, // Declared line 1335. Patch on CMP DWORD PTR [ESI+0x144], 0x1E
	PatchSkeleton{ shou2SCQUAKE_SPD_patchByteCode,               (void *)0x6B6296,  6}, // Declared line 1344. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ shou2SCQUAKE_patchByteCode,                   (void *)0x6CB16F,  7}, // Declared line 1406. Patch on CMP DWORD PTR [ESI+0x144], 3
	PatchSkeleton{ shou2SCQUAKE_ANG_patchByteCode,               (void *)0x6CAC3F,  8}, // Declared line 1412. Patch on MOV [ESP+0x10], EAX
	PatchSkeleton{ shou2SCQUAKE_ANG1_patchByteCode,              (void *)0x6CAC4F,  6}, // Declared line 1416. Patch on FSUB ST, QWORD PTR [0x85BBD8]
	PatchSkeleton{ shou2SCQUAKE_SPAWNRATE_patchByteCode,         (void *)0x6CAC25,  5}, // Declared line 1422. Patch on MOV ECX, 0x14
	PatchSkeleton{ shou2SCQUAKE_BOX_patchByteCode,               (void *)0x6CABF8,  6}, // Declared line 1436. Patch on MOV EAX, [ESI+0x35C]
	PatchSkeleton{ shou2SCQUAKE_BOUNDS_patchByteCode,            (void *)0x6CAF28,  6}, // Declared line 1445. Patch on FLD ST, DWORD PTR [0x85A25C]
	PatchSkeleton{ shou2SCQUAKE_BOUNDS1_patchByteCode,           (void *)0x6CAF45,  6}, // Declared line 1446. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ shouSPACE2_patchByteCode,                     (void *)0x6C7842,  7}, // Declared line 1453. Patch on PUSH 0x14
	PatchSkeleton{ shou3SCWEATHER_OFFSCREEN_patchByteCode,       (void *)0x6CD939,  6}, // Declared line 1484. Patch on JNP 0x006C679B
	PatchSkeleton{ shou3SCWEATHER_OFFSCREEN_patchByteCode,       (void *)0x6CD950,  6}, // Declared line 1485. Patch on JNP 0x006C679B
	PatchSkeleton{ shou3SCWEATHER_OFFSCREEN2_patchByteCode,      (void *)0x6CD961,  6}, // Declared line 1486. Patch on JE 0x006C679B
	PatchSkeleton{ shou5SCWEATHER_DUR_patchByteCode,             (void *)0x6B7C52,  7}, // Declared line 1522. Patch on CMP DWORD PTR [ESI+0x144], 0x78
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B7CD8,  6}, // Declared line 1524. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shou5SCWEATHER_DUR_patchByteCode,             (void *)0x6B7E26,  7}, // Declared line 1526. Patch on CMP DWORD PTR [ESI+0x144], 0x78
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B7EFE,  6}, // Declared line 1528. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6B6FCF,  6}, // Declared line 1532. Patch on JNE 0x006B705A
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6B6FE7,  6}, // Declared line 1534. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ shou4SCWEATHER_IDK_patchByteCode,             (void *)0x6CC2E2,  6}, // Declared line 1545. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ shou4SCWEATHER_Y_patchByteCode,               (void *)0x6B700D,  6}, // Declared line 1550. Patch on FADD ST, QWORD PTR [0x858E98]
	PatchSkeleton{ shou3SCSLASHY_X_patchByteCode,                (void *)0x6B82D7,  6}, // Declared line 1560. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ shou3SCSLASHY_ACC_patchByteCode,              (void *)0x6B8345,  6}, // Declared line 1562. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ shou4SCARMOR_DUR_patchByteCode,               (void *)0x6B7C15, 10}, // Declared line 1573. Patch on MOV DWORD PTR [ESI+0x924], 0x384
	PatchSkeleton{ shou4SCARMOR_FLASH_patchByteCode,             (void *)0x6B7B81,  8}, // Declared line 1578. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ shou4SCARMOR_IDK_patchByteCode,               (void *)0x6B7971,  7}, // Declared line 1583. Patch on CMP DWORD PTR [ESI+0x144], 0x24
	PatchSkeleton{ shou4SCARMOR_patchByteCode,                   (void *)0x6B7963,  5}, // Declared line 1595. Patch on MOV EBX, 1
	PatchSkeleton{ shou2SCSLASH_TARG_patchByteCode,              (void *)0x6B6620,  5}, // Declared line 1603. Patch on PUSH 0x350
	PatchSkeleton{ shou2SCSLASH_MOVE_patchByteCode,              (void *)0x6B6392,  6}, // Declared line 1609. Patch on JNE 0x006B642B
	PatchSkeleton{ shou2SCSLASH_IDK_patchByteCode,               (void *)0x6CB4FA,  6}, // Declared line 1629. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ shouA214QUAKE_patchByteCode,                  (void *)0x6C512C,  7}, // Declared line 1693. Patch on CMP DWORD PTR [ESI+0x144], 3
	PatchSkeleton{ shouA214QUAKE_LEAP_patchByteCode,             (void *)0x6B1A03,  5}, // Declared line 1700. Patch on TEST AH, 1
	PatchSkeleton{ shouA214QUAKE_BPOS_patchByteCode,             (void *)0x6B1A38,  6}, // Declared line 1709. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ shouA214QUAKE_LEAP_patchByteCode,             (void *)0x6C51B0,  5}, // Declared line 1711. Patch on TEST AH, 1
	PatchSkeleton{ shouFLDZ_patchByteCode,                       (void *)0x6C51E0,  6}, // Declared line 1713. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ shouA214QUAKE_CPOS_patchByteCode,             (void *)0x6C51F2,  6}, // Declared line 1717. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ shouA214QUAKE_CSTART_patchByteCode,           (void *)0x6C5186,  7}, // Declared line 1722. Patch on CMP DWORD PTR [ESI+0x144], 0x3C
	PatchSkeleton{ shouA214QUAKE_patchByteCode,                  (void *)0x6CC194,  6}, // Declared line 1725. Patch on FLD ST, DWORD PTR [ESI+0x37C]
	PatchSkeleton{ shou5SCQUAKE_DUR1_patchByteCode,              (void *)0x6CB7E5,  7}, // Declared line 1743. Patch on CMP DWORD PTR [ESI+0x144], 0x78
	PatchSkeleton{ shou5SCQUAKE_DUR2_patchByteCode,              (void *)0x6CB7FD, 10}, // Declared line 1744. Patch on CMP DWORD PTR [ESI+0x144], 0xF0
	PatchSkeleton{ shou5SCQUAKE_DUR3_patchByteCode,              (void *)0x6CB82B,  5}, // Declared line 1745. Patch on MOV EBX, 0x1A4
	PatchSkeleton{ shou5SCQUAKE_DUR4_patchByteCode,              (void *)0x6CBCFE,  6}, // Declared line 1746. Patch on CMP EBX, 0x208
	PatchSkeleton{ shou5SCQUAKE_DUR5_patchByteCode,              (void *)0x6CBD0A, 10}, // Declared line 1747. Patch on CMP DWORD PTR [ESI+0x144], 0x208
	PatchSkeleton{ shou5SCQUAKE_PERIOD_patchByteCode,            (void *)0x6CBCF8,  6}, // Declared line 1760. Patch on ADD EBX, 0xA
	PatchSkeleton{ shou5SCQUAKE_LOCPERIOD1_patchByteCode,        (void *)0x6CBA98,  6}, // Declared line 1768. Patch on FMUL ST, QWORD PTR [0x858E98]
	PatchSkeleton{ shou5SCQUAKE_LOCPERIOD2_patchByteCode,        (void *)0x6CBC97,  6}, // Declared line 1769. Patch on FMUL ST, QWORD PTR [0x857F98]
	PatchSkeleton{ shou5SCQUAKE_LOCPERIOD1_patchByteCode,        (void *)0x6CBF24,  6}, // Declared line 1770. Patch on FMUL ST, QWORD PTR [0x858E98]
	PatchSkeleton{ shou5SCQUAKE_LOCPERIOD2_patchByteCode,        (void *)0x6CC124,  6}, // Declared line 1771. Patch on FMUL ST, QWORD PTR [0x857F98]
	PatchSkeleton{ shouIDK4_patchByteCode,                       (void *)0x6CB9DB,  6}, // Declared line 1790. Patch on FLDZ
	PatchSkeleton{ shouIDK5_patchByteCode,                       (void *)0x6CC063,  6}, // Declared line 1795. Patch on FLDZ
	PatchSkeleton{ shouA236BORGAR_1_patchByteCode,               (void *)0x6C4503,  6}, // Declared line 1801. Patch on CMP AX, 0x19A
	PatchSkeleton{ shouA236BORGAR_SKIP_patchByteCode,            (void *)0x6C9819,  6}, // Declared line 1806. Patch on PUSH 5
	PatchSkeleton{ shouA236BORGAR_2_patchByteCode,               (void *)0x6C4914,  5}, // Declared line 2033. Patch on CMP CX, 0x19A
	PatchSkeleton{ shouA236BORGAR_3_patchByteCode,               (void *)0x6C4978,  5}, // Declared line 2034. Patch on CMP CX, 0x19A
	PatchSkeleton{ shouA236BORGAR_4A_patchByteCode,              (void *)0x6C468C,  5}, // Declared line 2038. Patch on PUSH 0x321
	PatchSkeleton{ shouA236BORGAR_4B_patchByteCode,              (void *)0x6C4711,  5}, // Declared line 2039. Patch on PUSH 0x321
	PatchSkeleton{ shouA236BORGAR_5_patchByteCode,               (void *)0x6C9449,  7}, // Declared line 2042. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ shouA236BORGAR_5A_patchByteCode,              (void *)0x6C39AF,  6}, // Declared line 2044. Patch on PUSH 4
	PatchSkeleton{ shouA236BORGAR_6_patchByteCode,               (void *)0x6C45AD,  6}, // Declared line 2063. Patch on FMUL ST, QWORD PTR [0x871520]
	PatchSkeleton{ shouA236BORGAR_IDK1_patchByteCode,            (void *)0x6C94B9,  6}, // Declared line 2075. Patch on FLD ST, QWORD PTR [0x857B20]
	PatchSkeleton{ shouA236BORGAR_IDK2_patchByteCode,            (void *)0x6B36AD,  6}, // Declared line 2076. Patch on FLD ST, DWORD PTR [0x87150C]
	PatchSkeleton{ shouA236BORGAR_IDK2_patchByteCode,            (void *)0x6B3837,  6}, // Declared line 2077. Patch on FLD ST, DWORD PTR [0x87150C]
	PatchSkeleton{ shouA236BORGAR_IDK2_patchByteCode,            (void *)0x6B3A27,  6}, // Declared line 2078. Patch on FLD ST, DWORD PTR [0x87150C]
	PatchSkeleton{ shouA236BORGAR_IDK2_patchByteCode,            (void *)0x6B3C1D,  6}, // Declared line 2079. Patch on FLD ST, DWORD PTR [0x87150C]
	PatchSkeleton{ shouD236STARTUP_IDK_patchByteCode,            (void *)0x6C8D6E,  6}, // Declared line 2086. Patch on FADD ST, QWORD PTR [0x867840]
	PatchSkeleton{ shouA22GUARD_DISABLEAIR1_patchByteCode,       (void *)0x6C2C5B,  8}, // Declared line 2102. Patch on CMP WORD PTR [ESI+0x91A], 0
	PatchSkeleton{ shouA22GUARD_DISABLEAIR2_patchByteCode,       (void *)0x6C2D4B,  8}, // Declared line 2103. Patch on CMP WORD PTR [ESI+0x91A], 0
	PatchSkeleton{ shouA22GUARD_SPOT_patchByteCode,              (void *)0x6B2B1A,  6}, // Declared line 2108. Patch on FLD ST, DWORD PTR [0x871544]
	PatchSkeleton{ shouA22GUARD_MOVE1_patchByteCode,             (void *)0x6C61B6,  6}, // Declared line 2119. Patch on FSTP DWORD PTR [ESI+0xEC], ST
	PatchSkeleton{ shouA22GUARD_MOVE2_patchByteCode,             (void *)0x6C61EC,  6}, // Declared line 2120. Patch on FADD ST, DWORD PTR [ECX+0xF0]
	PatchSkeleton{ shouA22GUARD_COUNT_patchByteCode,             (void *)0x6B2BC5,  6}, // Declared line 2125. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ shouA22GUARD_IDKTBH0_patchByteCode,           (void *)0x6C6159, 10}, // Declared line 2188. Patch on JA SHORT 0x006C6165
	PatchSkeleton{ shouA22GUARD_DISABLEAIRLV_patchByteCode,      (void *)0x6C26F4,  6}, // Declared line 2195. Patch on JE 0x006C27BE
	PatchSkeleton{ shouA22GUARD_REDIR_patchByteCode,             (void *)0x6B2C44,  7}, // Declared line 2201. Patch on MOV ECX, ESI
	PatchSkeleton{ shouA22GUARD_REDIR_patchByteCode,             (void *)0x6B2CB6,  7}, // Declared line 2202. Patch on MOV ECX, ESI
	PatchSkeleton{ shouA22GUARD_DUR_patchByteCode,               (void *)0x6B2C2C,  9}, // Declared line 2208. Patch on IMUL CX, 0x5A
	PatchSkeleton{ shouA22GUARD_MULTI_patchByteCode,             (void *)0x6C60FD,  6}, // Declared line 2223. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ shouNOP_patchByteCode,                        (void *)0x6C620F,  7}, // Declared line 2225. Patch on MOV BYTE PTR [ESI+0x358], 0xFF
};


static std::array<unsigned, 154> skeletonsForUpdate = {
	   0,   2,   3,   4,   5,   6,   7,   9,  10,  11,  12,  13,  14,  15,  16,  17,
	  18,  19,  20,  21,  22,  24,  25,  27,  28,  29,  30,  31,  32,  33,  34,  35,
	  36,  37,  38,  39,  40,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,
	  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  69,  86,  87,  88,  89,  90,
	  91,  92,  93,  95,  96,  97,  98,  99, 100, 104, 105, 106, 107, 108, 109, 110,
	 111, 112, 113, 121, 122, 124, 125, 130, 131, 132, 133, 134, 135, 136, 137, 138,
	 139, 140, 141, 143, 144, 145, 146, 159, 160, 161, 162, 163, 164, 165, 166, 167,
	 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
	 195, 196, 197, 198, 199, 200, 202, 203, 204, 205, 206, 207, 208, 209, 210, 213,
	 214, 242, 243, 244, 245, 249, 252, 255, 256, 257,
};
static std::array<unsigned, 4> skeletonsForInitializeAction = { 1, 8, 23, 26 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 3> skeletonsForHandleInputs = { 247, 248, 254 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 99> skeletonsForObjectUpdate = {
	  41,  42,  43,  44,  66,  67,  68,  70,  71,  72,  73,  74,  75,  76,  77,  78,
	  79,  80,  81,  82,  83,  84,  85,  94, 101, 102, 103, 114, 115, 116, 117, 118,
	 119, 120, 123, 126, 127, 128, 129, 142, 147, 148, 149, 150, 151, 152, 153, 154,
	 155, 156, 157, 158, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 201,
	 211, 212, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
	 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 246, 250, 251,
	 253, 258, 259,
};
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
	printf("Internal memory used %zu/11602\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 11602);
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
