#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<24224> applyMemory;
StackedMemory<12230> internalMemory;
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


// flandreFLDZ declared in flandre.asm line 1
static unsigned char flandreFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode flandreFLDZ_patchByteCode = { flandreFLDZ_byteCode, 2, nullptr, 0 };
// flandreFLD1 declared in flandre.asm line 5
static unsigned char flandreFLD1_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 6
};
static PatchByteCode flandreFLD1_patchByteCode = { flandreFLD1_byteCode, 2, nullptr, 0 };
// flandreNOP declared in flandre.asm line 9
static unsigned char flandreNOP_byteCode[] = {
	0x90, // NOP ; line 10
};
static PatchByteCode flandreNOP_patchByteCode = { flandreNOP_byteCode, 1, nullptr, 0 };
// flandreID_FIX declared in flandre.asm line 13
static unsigned char flandreID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000018 ; line 14
	0x66, 0x39, 0xAE, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],BP ; line 16
};
static PatchByteCode flandreID_FIX_patchByteCode = { flandreID_FIX_byteCode, 17, nullptr, 0 };
// flandreSLICERSC_SPD declared in flandre.asm line 25
static unsigned char flandreSLICERSC_SPD_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBE4] ; line 26
};
static PatchByteCode flandreSLICERSC_SPD_patchByteCode = { flandreSLICERSC_SPD_byteCode, 6, nullptr, 0 };
// flandreSLICERSC_LOOP declared in flandre.asm line 32
static unsigned char flandreSLICERSC_LOOP_byteCode[] = {
	0x66, 0x81, 0xF8, 0x08, 0x00,       // CMP AX,0x0008 ; line 33
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF, // JNL 0x596755 ; line 34
};
static JumpTarget flandreSLICERSC_LOOP_jumpData[] = {
	{ 0x007, 0x596755 },
};
static PatchByteCode flandreSLICERSC_LOOP_patchByteCode = { flandreSLICERSC_LOOP_byteCode, 11, flandreSLICERSC_LOOP_jumpData, 1 };
// flandreA236SWIPE_PARTICLE declared in flandre.asm line 40
static unsigned char flandreA236SWIPE_PARTICLE_byteCode[] = {
	0x89, 0xF1,                               // MOV ECX,ESI ; line 43
	0x6A, 0x78,                               // PUSH 0x78 ; line 44
	0x68, 0xC8, 0x00, 0x00, 0x00,             // PUSH 0x000000C8 ; line 45
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x47A9E0 ; line 46
	0x89, 0xF1,                               // MOV ECX,ESI ; line 49
	0x6A, 0x32,                               // PUSH 0x32 ; line 50
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487870 ; line 51
	0x89, 0xF1,                               // MOV ECX,ESI ; line 53
	0x6A, 0x03,                               // PUSH 0x03 ; line 55
	0x8D, 0x94, 0x24, 0x2C, 0x05, 0x00, 0x00, // LEA EDX,[ESP+0x52C] ; line 57
	0x52,                                     // PUSH EDX ; line 58
	0xD9, 0xEE,                               // FLDZ ; line 60
	0xD9, 0x9C, 0x24, 0x30, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x530] ; line 61
	0x6A, 0x01,                               // PUSH 0x01 ; line 63
	0xD9, 0xEE,                               // FLDZ ; line 65
	0xD9, 0x9C, 0x24, 0x38, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x538] ; line 66
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00,       // FLD DWORD PTR [0x85B870] ; line 68
	0xD9, 0x9C, 0x24, 0x3C, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x53C] ; line 69
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00, // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 71
	0x50,                                     // PUSH EAX ; line 72
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,       // SUB ESP,0x08 ; line 73
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 75
	0xD9, 0x5C, 0x24, 0x04,                   // FSTP DWORD PTR [ESP+0x04] ; line 76
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 78
	0xD9, 0x1C, 0x24,                         // FSTP DWORD PTR [ESP] ; line 79
	0x68, 0x50, 0x03, 0x00, 0x00,             // PUSH 0x350 ; line 80
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x46EB30 ; line 81
	0xD9, 0x05, 0x30, 0xB8, 0x85, 0x00,       // FLD DWORD PTR [0x85B830] ; line 83
};
static JumpTarget flandreA236SWIPE_PARTICLE_jumpData[] = {
	{ 0x00A, 0x47A9E0 },
	{ 0x013, 0x487870 },
	{ 0x06B, 0x46EB30 },
};
static PatchByteCode flandreA236SWIPE_PARTICLE_patchByteCode = { flandreA236SWIPE_PARTICLE_byteCode, 117, flandreA236SWIPE_PARTICLE_jumpData, 3 };
// flandreA236SWIPE_PARTICLE_C declared in flandre.asm line 89
static unsigned char flandreA236SWIPE_PARTICLE_C_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x464980 ; line 90
	0x89, 0xF1,                               // MOV ECX,ESI ; line 93
	0x6A, 0x78,                               // PUSH 0x78 ; line 94
	0x68, 0xC8, 0x00, 0x00, 0x00,             // PUSH 0x000000C8 ; line 95
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x47A9E0 ; line 96
	0x89, 0xF1,                               // MOV ECX,ESI ; line 99
	0x6A, 0x32,                               // PUSH 0x32 ; line 100
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487870 ; line 101
	0x89, 0xF1,                               // MOV ECX,ESI ; line 103
	0x6A, 0x03,                               // PUSH 0x03 ; line 105
	0x8D, 0x94, 0x24, 0x2C, 0x05, 0x00, 0x00, // LEA EDX,[ESP+0x52C] ; line 107
	0x52,                                     // PUSH EDX ; line 108
	0xD9, 0xEE,                               // FLDZ ; line 110
	0xD9, 0x9C, 0x24, 0x30, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x530] ; line 111
	0x6A, 0x01,                               // PUSH 0x01 ; line 113
	0xD9, 0xEE,                               // FLDZ ; line 115
	0xD9, 0x9C, 0x24, 0x38, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x538] ; line 116
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00,       // FLD DWORD PTR [0x85B870] ; line 118
	0xD9, 0x9C, 0x24, 0x3C, 0x05, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x53C] ; line 119
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00, // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 121
	0x50,                                     // PUSH EAX ; line 122
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,       // SUB ESP,0x08 ; line 123
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 125
	0xD9, 0x5C, 0x24, 0x04,                   // FSTP DWORD PTR [ESP+0x04] ; line 126
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 128
	0xD9, 0x1C, 0x24,                         // FSTP DWORD PTR [ESP] ; line 129
	0x68, 0x50, 0x03, 0x00, 0x00,             // PUSH 0x350 ; line 130
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x46EB30 ; line 131
};
static JumpTarget flandreA236SWIPE_PARTICLE_C_jumpData[] = {
	{ 0x001, 0x464980 },
	{ 0x00F, 0x47A9E0 },
	{ 0x018, 0x487870 },
	{ 0x070, 0x46EB30 },
};
static PatchByteCode flandreA236SWIPE_PARTICLE_C_patchByteCode = { flandreA236SWIPE_PARTICLE_C_byteCode, 116, flandreA236SWIPE_PARTICLE_C_jumpData, 4 };
// flandreA236SWIPE_CANCEL declared in flandre.asm line 137
static unsigned char flandreA236SWIPE_CANCEL_byteCode[] = {
	0x90, // NOP ; line 138
};
static PatchByteCode flandreA236SWIPE_CANCEL_patchByteCode = { flandreA236SWIPE_CANCEL_byteCode, 1, nullptr, 0 };
// flandreA236SWIPE declared in flandre.asm line 142
static unsigned char flandreA236SWIPE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x05, 0x00,       // CMP WORD PTR [ESI+0x13E],0x05 ; line 143
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE LABEL05 ; line 144
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00,       // CMP WORD PTR [ESI+0x13E],0x06 ; line 145
	0x0F, 0x84, 0x4B, 0x00, 0x00, 0x00,                         // JE LABEL06 ; line 146
	0xE9, 0x8A, 0x00, 0x00, 0x00,                               // JMP END ; line 147
	0x6A, 0x00,                                                 // PUSH 0x0 ; line 150
	0x6A, 0x00,                                                 // PUSH 0x0 ; line 151
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CF60 ; line 152
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 153
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0xFE, 0x01,       // CMP WORD PTR [EAX+0x13C],0x1FE ; line 154
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 155
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0xFF, 0x01,       // CMP WORD PTR [EAX+0x13C],0x1FF ; line 156
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 157
	0xE9, 0x46, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 158
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x13E],0x00 ; line 160
	0x0F, 0x84, 0x37, 0x00, 0x00, 0x00,                         // JE LABELEND ; line 161
	0xE9, 0x44, 0x00, 0x00, 0x00,                               // JMP END ; line 162
	0x6A, 0x00,                                                 // PUSH 0x0 ; line 165
	0x6A, 0x00,                                                 // PUSH 0x0 ; line 166
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CF60 ; line 167
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 168
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0xFF, 0x01,       // CMP WORD PTR [EAX+0x13C],0x1FF ; line 169
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 170
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x13E],0x00 ; line 171
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABELEND ; line 172
	0xE9, 0x12, 0x00, 0x00, 0x00,                               // JMP END ; line 173
	0x81, 0x86, 0x4C, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // ADD DWORD PTR [ESI+0x34C],0xFFFFFFFF ; line 176
	0x5F,                                                       // POP EDI ; line 177
	0x5E,                                                       // POP ESI ; line 178
	0x5D,                                                       // POP EBP ; line 179
	0x5B,                                                       // POP EBX ; line 180
	0x89, 0xEC,                                                 // MOV ESP, EBP ; line 181
	0x5D,                                                       // POP EBP ; line 182
	0xC3,                                                       // RET ; line 183
	0x0F, 0x87, 0xDE, 0xAD, 0xBE, 0xEF,                         // JA 0x5A0DDA ; line 186
};
static JumpTarget flandreA236SWIPE_jumpData[] = {
	{ 0x028, 0x48CF60 },
	{ 0x06E, 0x48CF60 },
	{ 0x0AF, 0x5A0DDA },
};
static PatchByteCode flandreA236SWIPE_patchByteCode = { flandreA236SWIPE_byteCode, 179, flandreA236SWIPE_jumpData, 3 };
// flandreSTRETCH_X declared in flandre.asm line 190
static unsigned char flandreSTRETCH_X_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 191
};
static PatchByteCode flandreSTRETCH_X_patchByteCode = { flandreSTRETCH_X_byteCode, 6, nullptr, 0 };
// flandre6ANEW_1 declared in flandre.asm line 200
static unsigned char flandre6ANEW_1_byteCode[] = {
	0xD9, 0xEE,                                           // FLDZ ; line 201
	0x52,                                                 // PUSH EDX ; line 202
	0xD9, 0x9C, 0x24, 0x30, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x530] ; line 203
	0xD9, 0xEE,                                           // FLDZ ; line 204
	0x6A, 0x01,                                           // PUSH 0x01 ; line 205
	0xD9, 0x9C, 0x24, 0x38, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x538] ; line 206
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x2E, 0x01, // CMP WORD PTR [ESI+0x13C],0x12E ; line 207
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE JSIXA ; line 208
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00,                   // FLD DWORD PTR [0x871538] ; line 209
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP END ; line 210
	0xD9, 0xEE,                                           // FLDZ ; line 212
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x58A6C3 ; line 214
};
static JumpTarget flandre6ANEW_1_jumpData[] = {
	{ 0x032, 0x58A6C3 },
};
static PatchByteCode flandre6ANEW_1_patchByteCode = { flandre6ANEW_1_byteCode, 54, flandre6ANEW_1_jumpData, 1 };
// flandre6ACNEW_1 declared in flandre.asm line 218
static unsigned char flandre6ACNEW_1_byteCode[] = {
	0xD9, 0x9C, 0x24, 0x0C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x50C] ; line 219
	0xD9, 0xEE,                                           // FLDZ ; line 220
	0x50,                                                 // PUSH EAX ; line 221
	0xD9, 0x9C, 0x24, 0x14, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x514] ; line 222
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x2E, 0x01, // CMP WORD PTR [ESI+0x13C],0x12E ; line 223
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE JSIXA ; line 224
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00,                   // FLD DWORD PTR [0x871538] ; line 225
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP END ; line 226
	0xD9, 0xEE,                                           // FLDZ ; line 228
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x58A7AB ; line 230
};
static JumpTarget flandre6ACNEW_1_jumpData[] = {
	{ 0x02E, 0x58A7AB },
};
static PatchByteCode flandre6ACNEW_1_patchByteCode = { flandre6ACNEW_1_byteCode, 50, flandre6ACNEW_1_jumpData, 1 };
// flandre6ANEW declared in flandre.asm line 234
static unsigned char flandre6ANEW_byteCode[] = {
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x2E, 0x01, // CMP WORD PTR [EAX+0x13C],0x12E ; line 235
};
static PatchByteCode flandre6ANEW_patchByteCode = { flandre6ANEW_byteCode, 9, nullptr, 0 };
// flandreBATFORM_PROJ declared in flandre.asm line 239
static unsigned char flandreBATFORM_PROJ_byteCode[] = {
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 240
	0x66, 0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x144],0x00 ; line 242
	0x0F, 0x85, 0x36, 0x00, 0x00, 0x00,                         // JNE HITBOX ; line 243
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR[ ESI+0x142],0x00 ; line 244
	0x0F, 0x85, 0x27, 0x00, 0x00, 0x00,                         // JNE HITBOX ; line 245
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 246
	0x0F, 0x85, 0x18, 0x00, 0x00, 0x00,                         // JNE HITBOX ; line 247
	0xC6, 0x86, 0x8C, 0x01, 0x00, 0x00, 0x04,                   // MOV BYTE PTR [ESI+0x18C],0x04 ; line 248
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 250
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 251
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xEC] ; line 254
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 255
	0xD9, 0x81, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xF0] ; line 256
	0xD8, 0x05, 0x40, 0x15, 0x87, 0x00,                         // FADD DWORD PTR [0x871540] ; line 257
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 258
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0xFA, 0x01,       // CMP WORD PTR [ECX+0x13C],0x1FA ; line 259
	0x0F, 0x84, 0x1F, 0x00, 0x00, 0x00,                         // JE SHORT CVER ; line 260
	0xD9, 0xEE,                                                 // FLDZ ; line 261
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00,                         // FSUB DWORD PTR [0x85AE94] ; line 262
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00,                         // FSUB DWORD PTR [0x85AE94] ; line 263
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00,                         // FSUB DWORD PTR [0x85AE94] ; line 264
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 265
	0xE9, 0x0E, 0x00, 0x00, 0x00,                               // JMP END ; line 266
	0xD9, 0xEE,                                                 // FLDZ ; line 268
	0xD8, 0x25, 0x00, 0xC5, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C500] ; line 269
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 270
	0x66, 0x81, 0xB9, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ECX+0x13E],0x02 ; line 273
	0x0F, 0x85, 0x3F, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 274
	0x66, 0x81, 0xB9, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ECX+0x144],0x00 ; line 275
	0x0F, 0x85, 0x30, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 276
	0x66, 0x81, 0xB9, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ECX+0x142],0x00 ; line 277
	0x0F, 0x85, 0x21, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 278
	0x66, 0x81, 0xB9, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ECX+0x140],0x00 ; line 279
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 280
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x142],0xFF ; line 281
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x80, 0x00,       // MOV WORD PTR [ESI+0x140],0x80 ; line 282
};
static PatchByteCode flandreBATFORM_PROJ_patchByteCode = { flandreBATFORM_PROJ_byteCode, 243, nullptr, 0 };
// flandreBATFORM_PROJSTICK declared in flandre.asm line 288
static unsigned char flandreBATFORM_PROJSTICK_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x59E7EB ; line 289
};
static JumpTarget flandreBATFORM_PROJSTICK_jumpData[] = {
	{ 0x001, 0x59E7EB },
};
static PatchByteCode flandreBATFORM_PROJSTICK_patchByteCode = { flandreBATFORM_PROJSTICK_byteCode, 5, flandreBATFORM_PROJSTICK_jumpData, 1 };
// flandreBATFORM declared in flandre.asm line 293
static unsigned char flandreBATFORM_byteCode[] = {
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF4] ; line 294
};
static PatchByteCode flandreBATFORM_patchByteCode = { flandreBATFORM_byteCode, 6, nullptr, 0 };
// flandreBATFORMB declared in flandre.asm line 301
static unsigned char flandreBATFORMB_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 303
	0x0F, 0x85, 0xFF, 0x00, 0x00, 0x00,                         // JNE OBJSKIP ; line 304
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 305
	0x0F, 0x85, 0xF0, 0x00, 0x00, 0x00,                         // JNE OBJSKIP ; line 306
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x03 ; line 307
	0x0F, 0x85, 0xE1, 0x00, 0x00, 0x00,                         // JNE OBJSKIP ; line 308
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 312
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 313
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 314
	0xD9, 0xEE,                                                 // FLDZ ; line 317
	0xD9, 0x9C, 0x24, 0x90, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x390] ; line 318
	0x6A, 0x03,                                                 // PUSH 0x03 ; line 319
	0xD9, 0xEE,                                                 // FLDZ ; line 320
	0xD9, 0x9C, 0x24, 0x98, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x398] ; line 321
	0x8D, 0x8C, 0x24, 0x94, 0x03, 0x00, 0x00,                   // LEA ECX,[ESP+0x394] ; line 322
	0xD9, 0xEE,                                                 // FLDZ ; line 324
	0x80, 0xBE, 0xA8, 0x06, 0x00, 0x00, 0x04,                   // CMP BYTE PTR [ESI+0x6A8],0x04 ; line 325
	0x0F, 0x8C, 0x0B, 0x00, 0x00, 0x00,                         // JL SHORT LOWLV1 ; line 326
	0xD8, 0x05, 0xF0, 0x89, 0x85, 0x00,                         // FADD DWORD PTR [0x8589F0] ; line 327
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP FINLV1 ; line 328
	0xD9, 0x9C, 0x24, 0x9C, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x39C] ; line 331
	0x51,                                                       // PUSH ECX ; line 332
	0x0F, 0xB6, 0x96, 0x04, 0x01, 0x00, 0x00,                   // MOVZX EDX,BYTE PTR [ESI+0x104] ; line 333
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 334
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 335
	0x52,                                                       // PUSH EDX ; line 336
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                         // SUB ESP,0x08 ; line 337
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 338
	0xD9, 0x5C, 0x24, 0x04,                                     // FSTP DWORD PTR [ESP+0x04] ; line 339
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 340
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 341
	0x68, 0x2B, 0x03, 0x00, 0x00,                               // PUSH 0x32B ; line 342
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x46EB30 ; line 343
	0xD9, 0xEE,                                                 // FLDZ ; line 347
	0xD9, 0x9C, 0x24, 0x90, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x390] ; line 348
	0x6A, 0x03,                                                 // PUSH 0x03 ; line 349
	0xD9, 0xEE,                                                 // FLDZ ; line 350
	0xD9, 0x9C, 0x24, 0x98, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x398] ; line 351
	0x8D, 0x8C, 0x24, 0x94, 0x03, 0x00, 0x00,                   // LEA ECX,[ESP+0x394] ; line 352
	0xD9, 0xE8,                                                 // FLD1 ; line 354
	0x80, 0xBE, 0xA8, 0x06, 0x00, 0x00, 0x04,                   // CMP BYTE PTR [ESI+0x6A8],0x04 ; line 355
	0x0F, 0x8C, 0x0B, 0x00, 0x00, 0x00,                         // JL SHORT LOWLV2 ; line 356
	0xD8, 0x05, 0xF0, 0x89, 0x85, 0x00,                         // FADD DWORD PTR [0x8589F0] ; line 357
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP FINLV2 ; line 358
	0xD9, 0x9C, 0x24, 0x9C, 0x03, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x39C] ; line 361
	0x51,                                                       // PUSH ECX ; line 362
	0x0F, 0xB6, 0x96, 0x04, 0x01, 0x00, 0x00,                   // MOVZX EDX,BYTE PTR [ESI+0x104] ; line 363
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 364
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 365
	0x52,                                                       // PUSH EDX ; line 366
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                         // SUB ESP,0x08 ; line 367
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 368
	0xD9, 0x5C, 0x24, 0x04,                                     // FSTP DWORD PTR [ESP+0x04] ; line 369
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 370
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 371
	0x68, 0x2B, 0x03, 0x00, 0x00,                               // PUSH 0x32B ; line 372
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x46EB30 ; line 373
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0xFA, 0x01,       // CMP WORD PTR [ESI+0x13C],0x1FA ; line 377
	0x0F, 0x84, 0xDE, 0x00, 0x00, 0x00,                         // JE CMMT ; line 378
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 381
	0x0F, 0x84, 0x22, 0x00, 0x00, 0x00,                         // JE MMT1 ; line 382
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 383
	0x0F, 0x84, 0x3C, 0x00, 0x00, 0x00,                         // JE MMT2 ; line 384
	0xD9, 0xEE,                                                 // FLDZ ; line 386
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 387
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 388
	0xE9, 0x8B, 0x01, 0x00, 0x00,                               // JMP AMMT ; line 389
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                         // FLD DWORD PTR [0x85B3B0] ; line 392
	0xD8, 0x35, 0x54, 0xB9, 0x85, 0x00,                         // FDIV DWORD PTR [0x85B954] ; line 393
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 394
	0xD9, 0x05, 0x30, 0xB8, 0x85, 0x00,                         // FLD DWORD PTR [0x85B830] ; line 395
	0xD8, 0x35, 0x54, 0xB9, 0x85, 0x00,                         // FDIV DWORD PTR [0x85B954] ; line 396
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 397
	0xE9, 0x62, 0x01, 0x00, 0x00,                               // JMP AMMT ; line 398
	0xD9, 0xE8,                                                 // FLD1 ; line 402
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 403
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 405
	0x0F, 0x85, 0x55, 0x00, 0x00, 0x00,                         // JNE MMT2A ; line 406
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 407
	0x0F, 0x85, 0x46, 0x00, 0x00, 0x00,                         // JNE MMT2A ; line 408
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 409
	0x0F, 0x85, 0x37, 0x00, 0x00, 0x00,                         // JNE MMT2A ; line 410
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 413
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 414
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 415
	0xD9, 0x05, 0x7C, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC7C] ; line 417
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 418
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00,                         // FLD DWORD PTR [0x8677A8] ; line 419
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 420
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 421
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 422
	0xE9, 0x12, 0x00, 0x00, 0x00,                               // JMP MMT2B ; line 423
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 426
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0x100] ; line 427
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 428
	0xE9, 0xDE, 0x00, 0x00, 0x00,                               // JMP AMMT ; line 431
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 436
	0x0F, 0x84, 0x22, 0x00, 0x00, 0x00,                         // JE CMMT1 ; line 437
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 438
	0x0F, 0x84, 0x3C, 0x00, 0x00, 0x00,                         // JE CMMT2 ; line 439
	0xD9, 0xEE,                                                 // FLDZ ; line 441
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 442
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 443
	0xE9, 0xAD, 0x00, 0x00, 0x00,                               // JMP AMMT ; line 444
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00,                         // FLD DWORD PTR [0x85BBE4] ; line 447
	0xD8, 0x35, 0x54, 0xB9, 0x85, 0x00,                         // FDIV DWORD PTR [0x85B954] ; line 448
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 449
	0xD9, 0x05, 0x7C, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC7C] ; line 450
	0xD8, 0x35, 0x54, 0xB9, 0x85, 0x00,                         // FDIV DWORD PTR [0x85B954] ; line 451
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 452
	0xE9, 0x84, 0x00, 0x00, 0x00,                               // JMP AMMT ; line 453
	0xD9, 0xE8,                                                 // FLD1 ; line 457
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 458
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 460
	0x0F, 0x85, 0x55, 0x00, 0x00, 0x00,                         // JNE CMMT2A ; line 461
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 462
	0x0F, 0x85, 0x46, 0x00, 0x00, 0x00,                         // JNE CMMT2A ; line 463
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 464
	0x0F, 0x85, 0x37, 0x00, 0x00, 0x00,                         // JNE CMMT2A ; line 465
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 468
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 469
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 470
	0xD9, 0x05, 0x7C, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC7C] ; line 472
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 473
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00,                         // FLD DWORD PTR [0x8677A8] ; line 474
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 475
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 476
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 477
	0xE9, 0x12, 0x00, 0x00, 0x00,                               // JMP CMMT2B ; line 478
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 481
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0x100] ; line 482
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 483
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP AMMT ; line 486
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 489
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABEL01 ; line 490
	0xE9, 0x0C, 0x00, 0x00, 0x00,                               // JMP LABEL02 ; line 491
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 493
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487CA0 ; line 494
	0xE9, 0xA6, 0x00, 0x00, 0x00,                               // JMP LABEL03 ; line 495
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 497
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487EA0 ; line 498
	0x84, 0xC0,                                                 // TEST AL,AL ; line 499
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE END ; line 500
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 503
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x438C60 ; line 504
	0x84, 0xC0,                                                 // TEST AL,AL ; line 505
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 506
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 508
	0x8B, 0x42, 0x08,                                           // MOV EAX,[EDX+0x08] ; line 509
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 510
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 511
	0xFF, 0xD0,                                                 // CALL EAX ; line 512
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x58E2ED ; line 513
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 516
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x58E2ED ; line 518
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 519
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x58E2ED ; line 521
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 522
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x58E2ED ; line 524
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 526
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                         // JNE SS2 ; line 527
	0x6A, 0x78,                                                 // PUSH 0x78 ; line 529
	0x68, 0xC8, 0x00, 0x00, 0x00,                               // PUSH 0xC8 ; line 530
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 531
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x47A9E0 ; line 532
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 533
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 534
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487870 ; line 535
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x58E2ED ; line 539
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x13E],0x03 ; line 542
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x58E2ED ; line 544
	0x8B, 0x06,                                                 // MOV EAX,[ESI] ; line 545
	0x8B, 0x50, 0x08,                                           // MOV EDX,[EAX+0x08] ; line 546
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 547
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 548
	0xFF, 0xD2,                                                 // CALL EDX ; line 549
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x58E2ED ; line 551
	0xE9, 0x64, 0xFF, 0xFF, 0xFF,                               // JMP BATFORM1 ; line 555
};
static JumpTarget flandreBATFORMB_jumpData[] = {
	{ 0x032, 0x464980 },
	{ 0x09E, 0x46EB30 },
	{ 0x10A, 0x46EB30 },
	{ 0x1B2, 0x464980 },
	{ 0x290, 0x464980 },
	{ 0x2F0, 0x487CA0 },
	{ 0x2FC, 0x487EA0 },
	{ 0x30B, 0x438C60 },
	{ 0x323, 0x58E2ED },
	{ 0x333, 0x58E2ED },
	{ 0x342, 0x58E2ED },
	{ 0x351, 0x58E2ED },
	{ 0x36E, 0x47A9E0 },
	{ 0x377, 0x487870 },
	{ 0x37C, 0x58E2ED },
	{ 0x38B, 0x58E2ED },
	{ 0x39B, 0x58E2ED },
};
static PatchByteCode flandreBATFORMB_patchByteCode = { flandreBATFORMB_byteCode, 932, flandreBATFORMB_jumpData, 17 };
// flandreBATFORMC declared in flandre.asm line 559
static unsigned char flandreBATFORMC_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58E2F5 ; line 560
};
static JumpTarget flandreBATFORMC_jumpData[] = {
	{ 0x001, 0x58E2F5 },
};
static PatchByteCode flandreBATFORMC_patchByteCode = { flandreBATFORMC_byteCode, 5, flandreBATFORMC_jumpData, 1 };
// flandreJ2A_REC2 declared in flandre.asm line 567
static unsigned char flandreJ2A_REC2_byteCode[] = {
	0x0F, 0x84, 0x5B, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 568
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 569
	0x0F, 0x85, 0x4C, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 570
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x140],0x02 ; line 571
	0x0F, 0x8F, 0x3D, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 572
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x142],0x01 ; line 573
	0x0F, 0x85, 0x2E, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 574
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000001 ; line 575
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 576
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000007 ; line 579
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 580
	0xE9, 0x09, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 581
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x03, 0x00,       // MOV WORD PTR [ESI+0x142],0x03 ; line 583
	0x66, 0x39, 0xAE, 0x3E, 0x01, 0x00, 0x00,                   // CMP [ESI+0x13E],BP ; line 585
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x599FB0 ; line 586
};
static JumpTarget flandreJ2A_REC2_jumpData[] = {
	{ 0x06A, 0x599FB0 },
};
static PatchByteCode flandreJ2A_REC2_patchByteCode = { flandreJ2A_REC2_byteCode, 110, flandreJ2A_REC2_jumpData, 1 };
// flandreJ2A_REC declared in flandre.asm line 591
static unsigned char flandreJ2A_REC_byteCode[] = {
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 592
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 593
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000001 ; line 594
	0x0F, 0x84, 0x25, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 595
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000002 ; line 596
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 597
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000007 ; line 598
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 599
	0xE9, 0x0A, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 600
	0x8B, 0x42, 0x14,                                           // MOV EAX,[EDX+0x14] ; line 602
	0xFF, 0xD0,                                                 // CALL EAX ; line 603
	0xE9, 0x07, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 604
	0x8B, 0x42, 0x08,                                           // MOV EAX,[EDX+0x08] ; line 607
	0x6A, 0x0A,                                                 // PUSH 0x0A ; line 608
	0xFF, 0xD0,                                                 // CALL EAX ; line 609
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x58AC63 ; line 612
};
static JumpTarget flandreJ2A_REC_jumpData[] = {
	{ 0x04B, 0x58AC63 },
};
static PatchByteCode flandreJ2A_REC_patchByteCode = { flandreJ2A_REC_byteCode, 79, flandreJ2A_REC_jumpData, 1 };
// flandre5B_QUANT declared in flandre.asm line 616
static unsigned char flandre5B_QUANT_byteCode[] = {
	0x01, 0xDF,                         // ADD EDI,EBX ; line 617
	0x81, 0xFF, 0x01, 0x00, 0x00, 0x00, // CMP EDI,0x01 ; line 618
};
static PatchByteCode flandre5B_QUANT_patchByteCode = { flandre5B_QUANT_byteCode, 8, nullptr, 0 };
// flandre5B_QUANT1 declared in flandre.asm line 620
static unsigned char flandre5B_QUANT1_byteCode[] = {
	0x81, 0xC7, 0x01, 0x00, 0x00, 0x00, // ADD EDI,0x01 ; line 621
	0x81, 0xFF, 0x01, 0x00, 0x00, 0x00, // CMP EDI,0x01 ; line 622
};
static PatchByteCode flandre5B_QUANT1_patchByteCode = { flandre5B_QUANT1_byteCode, 12, nullptr, 0 };
// flandre5B_NEW0 declared in flandre.asm line 625
static unsigned char flandre5B_NEW0_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 626
};
static PatchByteCode flandre5B_NEW0_patchByteCode = { flandre5B_NEW0_byteCode, 6, nullptr, 0 };
// flandre5B_NEW0_CHARGE declared in flandre.asm line 628
static unsigned char flandre5B_NEW0_CHARGE_byteCode[] = {
	0xD9, 0x05, 0x0C, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x87150C] ; line 629
};
static PatchByteCode flandre5B_NEW0_CHARGE_patchByteCode = { flandre5B_NEW0_CHARGE_byteCode, 6, nullptr, 0 };
// flandre5B_NEW1 declared in flandre.asm line 632
static unsigned char flandre5B_NEW1_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 633
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x4099F0 ; line 634
};
static JumpTarget flandre5B_NEW1_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode flandre5B_NEW1_patchByteCode = { flandre5B_NEW1_byteCode, 7, flandre5B_NEW1_jumpData, 1 };
// flandre5B_NEWSYS declared in flandre.asm line 690
static unsigned char flandre5B_NEWSYS_byteCode[] = {
	0x6A, 0x0A,                                                 // PUSH 0x0A ; line 693
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 694
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CDE0 ; line 695
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 696
	0x66, 0x9C,                                                 // PUSHF ; line 698
	0x60,                                                       // PUSHAD ; line 699
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0F ; line 700
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                         // JNE SHORT LABELX ; line 701
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 702
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE SHORT LABELX ; line 703
	0x6A, 0x18,                                                 // PUSH 0x18 ; line 704
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 705
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 706
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x42 ; line 709
	0x0F, 0x8F, 0x72, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 710
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 711
	0xD8, 0x8E, 0xF4, 0x00, 0x00, 0x00,                         // FMUL DWORD PTR [ESI+0xF4] ; line 712
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 713
	0xD8, 0x8E, 0xF8, 0x00, 0x00, 0x00,                         // FMUL DWORD PTR [ESI+0xF8] ; line 714
	0xDE, 0xC1,                                                 // FADDP ; line 715
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00,                         // FLD DWORD PTR [0x8589F0] ; line 716
	0xDE, 0xD9,                                                 // FCOMPP ; line 717
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 718
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 719
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 720
	0xE9, 0x2E, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 721
	0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x5A, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x140],0x5A ; line 723
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 726
	0xDC, 0x0D, 0x60, 0xB8, 0x85, 0x00,                         // FMUL QWORD PTR [0x85B860] ; line 727
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 728
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 730
	0xDC, 0x0D, 0x60, 0xB8, 0x85, 0x00,                         // FMUL QWORD PTR [0x85B860] ; line 731
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 732
	0x61,                                                       // POPAD ; line 735
	0x66, 0x9D,                                                 // POPF ; line 736
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000005 ; line 737
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x59DFEB ; line 738
	0x61,                                                       // POPAD ; line 741
	0x66, 0x9D,                                                 // POPF ; line 742
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000005 ; line 743
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x59E2D8 ; line 744
};
static JumpTarget flandre5B_NEWSYS_jumpData[] = {
	{ 0x005, 0x48CDE0 },
	{ 0x036, 0x464980 },
	{ 0x0B8, 0x59DFEB },
	{ 0x0CA, 0x59E2D8 },
};
static PatchByteCode flandre5B_NEWSYS_patchByteCode = { flandre5B_NEWSYS_byteCode, 206, flandre5B_NEWSYS_jumpData, 4 };
// flandre5B_ANIM declared in flandre.asm line 752
static unsigned char flandre5B_ANIM_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x59E1F2 ; line 753
};
static JumpTarget flandre5B_ANIM_jumpData[] = {
	{ 0x001, 0x59E1F2 },
};
static PatchByteCode flandre5B_ANIM_patchByteCode = { flandre5B_ANIM_byteCode, 5, flandre5B_ANIM_jumpData, 1 };
// flandreC5C_NEW_GRAZERES declared in flandre.asm line 757
static unsigned char flandreC5C_NEW_GRAZERES_byteCode[] = {
	0x6A, 0x0F,                   // PUSH 0x0F ; line 758
	0x89, 0xF1,                   // MOV ECX,ESI ; line 759
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CDE0 ; line 760
};
static JumpTarget flandreC5C_NEW_GRAZERES_jumpData[] = {
	{ 0x005, 0x48CDE0 },
};
static PatchByteCode flandreC5C_NEW_GRAZERES_patchByteCode = { flandreC5C_NEW_GRAZERES_byteCode, 9, flandreC5C_NEW_GRAZERES_jumpData, 1 };
// flandreC5C_NEW_DENSITY declared in flandre.asm line 764
static unsigned char flandreC5C_NEW_DENSITY_byteCode[] = {
	0x6A, 0x04,                   // PUSH 0x04 ; line 765
	0x89, 0xF1,                   // MOV ECX,ESI ; line 766
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CE30 ; line 767
};
static JumpTarget flandreC5C_NEW_DENSITY_jumpData[] = {
	{ 0x005, 0x48CE30 },
};
static PatchByteCode flandreC5C_NEW_DENSITY_patchByteCode = { flandreC5C_NEW_DENSITY_byteCode, 9, flandreC5C_NEW_DENSITY_jumpData, 1 };
// flandreC5C_ONCE declared in flandre.asm line 771
static unsigned char flandreC5C_ONCE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x99, 0x00, // CMP WORD PTR [ESI+0x140],0x99 ; line 772
};
static PatchByteCode flandreC5C_ONCE_patchByteCode = { flandreC5C_ONCE_byteCode, 9, nullptr, 0 };
// flandre5C_HITCOUNT declared in flandre.asm line 775
static unsigned char flandre5C_HITCOUNT_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 776
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x07,                   // CMP BYTE PTR [ESI+0x194],0x07 ; line 777
	0x0F, 0x84, 0x12, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 778
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x0A,                   // CMP BYTE PTR [ESI+0x194],0x0A ; line 779
	0x0F, 0x84, 0x0C, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 780
	0xE9, 0x0E, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 781
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x03,                   // MOV BYTE PTR [ESI+0x194],0x03 ; line 783
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x03,                   // MOV BYTE PTR [ESI+0x194],0x03 ; line 785
	0x66, 0x9D,                                                 // POPF ; line 787
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 788
};
static PatchByteCode flandre5C_HITCOUNT_patchByteCode = { flandre5C_HITCOUNT_byteCode, 59, nullptr, 0 };
// flandreJ2C_ONEBULLET declared in flandre.asm line 806
static unsigned char flandreJ2C_ONEBULLET_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58DA82 ; line 807
};
static JumpTarget flandreJ2C_ONEBULLET_jumpData[] = {
	{ 0x001, 0x58DA82 },
};
static PatchByteCode flandreJ2C_ONEBULLET_patchByteCode = { flandreJ2C_ONEBULLET_byteCode, 5, flandreJ2C_ONEBULLET_jumpData, 1 };
// flandre3A_REC declared in flandre.asm line 812
static unsigned char flandre3A_REC_byteCode[] = {
	0x6A, 0x00, // PUSH 0x00 ; line 813
	0x89, 0xF1, // MOV ECX,ESI ; line 814
	0xFF, 0xD0, // CALL EAX ; line 815
};
static PatchByteCode flandre3A_REC_patchByteCode = { flandre3A_REC_byteCode, 6, nullptr, 0 };
// flandre3A_SPD declared in flandre.asm line 817
static unsigned char flandre3A_SPD_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBE4] ; line 818
};
static PatchByteCode flandre3A_SPD_patchByteCode = { flandre3A_SPD_byteCode, 6, nullptr, 0 };
// flandre3A_DESPD declared in flandre.asm line 820
static unsigned char flandre3A_DESPD_byteCode[] = {
	0xD8, 0x25, 0xB8, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBB8] ; line 821
};
static PatchByteCode flandre3A_DESPD_patchByteCode = { flandre3A_DESPD_byteCode, 6, nullptr, 0 };
// flandreJ6A_0 declared in flandre.asm line 833
static unsigned char flandreJ6A_0_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 834
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x59AD90 ; line 835
};
static JumpTarget flandreJ6A_0_jumpData[] = {
	{ 0x008, 0x59AD90 },
};
static PatchByteCode flandreJ6A_0_patchByteCode = { flandreJ6A_0_byteCode, 12, flandreJ6A_0_jumpData, 1 };
// flandreJ6A_1 declared in flandre.asm line 839
static unsigned char flandreJ6A_1_byteCode[] = {
	0x81, 0xF8, 0x07, 0x00, 0x00, 0x00, // CMP EAX,0x07 ; line 840
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00, // CMP EAX,0x02 ; line 841
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x58AA86 ; line 842
};
static JumpTarget flandreJ6A_1_jumpData[] = {
	{ 0x00D, 0x58AA86 },
};
static PatchByteCode flandreJ6A_1_patchByteCode = { flandreJ6A_1_byteCode, 17, flandreJ6A_1_jumpData, 1 };
// flandreJ6ABOUNCE_X declared in flandre.asm line 847
static unsigned char flandreJ6ABOUNCE_X_byteCode[] = {
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867860] ; line 848
};
static PatchByteCode flandreJ6ABOUNCE_X_patchByteCode = { flandreJ6ABOUNCE_X_byteCode, 6, nullptr, 0 };
// flandre0 declared in flandre.asm line 859
static unsigned char flandre0_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 860
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF4] ; line 861
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x58AB04 ; line 862
};
static JumpTarget flandre0_jumpData[] = {
	{ 0x009, 0x58AB04 },
};
static PatchByteCode flandre0_patchByteCode = { flandre0_byteCode, 13, flandre0_jumpData, 1 };
// flandre1A declared in flandre.asm line 868
static unsigned char flandre1A_byteCode[] = {
	0x89, 0xF1,                   // MOV ECX,ESI ; line 869
	0x6A, 0x03,                   // PUSH 0x03 ; line 870
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58A6A9 ; line 872
};
static JumpTarget flandre1A_jumpData[] = {
	{ 0x005, 0x58A6A9 },
};
static PatchByteCode flandre1A_patchByteCode = { flandre1A_byteCode, 9, flandre1A_jumpData, 1 };
// flandre1B declared in flandre.asm line 877
static unsigned char flandre1B_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x33, 0x01, // CMP WORD PTR [ESI+0x13C],0x0133 ; line 879
	0x0F, 0x84, 0x0C, 0x00, 0x00, 0x00,                   // JE SHORT LABELA ; line 880
	0x66, 0x39, 0xAE, 0x42, 0x01, 0x00, 0x00,             // CMP [ESI+0x142],BP ; line 881
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x58A6F9 ; line 882
	0x6A, 0x1D,                                           // PUSH 0x1D ; line 885
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 886
	0xD9, 0x05, 0xBC, 0xC1, 0x85, 0x00,                   // FLD DWORD PTR [0x85C1BC] ; line 887
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 888
	0xD9, 0xEB,                                           // FLDPI ; line 889
	0xD8, 0xC8,                                           // FMUL ST0,ST0 ; line 890
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 891
	0xD9, 0x05, 0x6C, 0xBC, 0x85, 0x00,                   // FLD DWORD PTR [0x85BC6C] ; line 892
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x100] ; line 893
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x439490 ; line 894
	0x66, 0x39, 0xAE, 0x42, 0x01, 0x00, 0x00,             // CMP [ESI+0x142],BP ; line 895
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 896
	0x66, 0x39, 0x9E, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BX ; line 897
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 898
	0xD9, 0x05, 0xBC, 0xC1, 0x85, 0x00,                   // FLD DWORD PTR [0x85C1BC] ; line 899
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 900
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x58ABEF ; line 902
};
static JumpTarget flandre1B_jumpData[] = {
	{ 0x017, 0x58A6F9 },
	{ 0x042, 0x439490 },
	{ 0x06D, 0x58ABEF },
};
static PatchByteCode flandre1B_patchByteCode = { flandre1B_byteCode, 113, flandre1B_jumpData, 3 };
// flandre2 declared in flandre.asm line 907
static unsigned char flandre2_byteCode[] = {
	0x66, 0x81, 0xF9, 0x33, 0x01,       // CMP CX,0x0133 ; line 908
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x5A0D3F ; line 909
	0x66, 0x81, 0xF9, 0x33, 0x01,       // CMP CX,0x0133 ; line 910
};
static JumpTarget flandre2_jumpData[] = {
	{ 0x007, 0x5A0D3F },
};
static PatchByteCode flandre2_patchByteCode = { flandre2_byteCode, 16, flandre2_jumpData, 1 };
// flandre3 declared in flandre.asm line 918
static unsigned char flandre3_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x03 ; line 919
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 920
	0xE9, 0x2B, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL3 ; line 921
	0x66, 0x39, 0xAE, 0x42, 0x01, 0x00, 0x00,             // CMP [ESI+0x142],BP ; line 924
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 925
	0x66, 0x39, 0x9E, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BX ; line 926
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 927
	0xD9, 0x05, 0xBC, 0xC1, 0x85, 0x00,                   // FLD DWORD PTR [0x85C1BC] ; line 928
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 929
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x58ABEF ; line 931
	0x90,                                                 // NOP ; line 934
};
static JumpTarget flandre3_jumpData[] = {
	{ 0x03B, 0x58ABEF },
};
static PatchByteCode flandre3_patchByteCode = { flandre3_byteCode, 64, flandre3_jumpData, 1 };
// flandre4 declared in flandre.asm line 938
static unsigned char flandre4_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x03 ; line 939
};
static PatchByteCode flandre4_patchByteCode = { flandre4_byteCode, 9, nullptr, 0 };
// flandreA22CEILING declared in flandre.asm line 947
static unsigned char flandreA22CEILING_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 948
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x35, 0x02, // CMP WORD PTR [ESI+0x13C],0x0235 ; line 949
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 950
	0xD9, 0xEE,                                           // FLDZ ; line 951
	0xE9, 0x0E, 0x00, 0x00, 0x00,                         // JMP LABEL2 ; line 953
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 955
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 957
	0xD9, 0xEE,                                           // FLDZ ; line 958
	0x66, 0x9D,                                           // POPF ; line 961
	0x90,                                                 // NOP ; line 962
};
static PatchByteCode flandreA22CEILING_patchByteCode = { flandreA22CEILING_byteCode, 41, nullptr, 0 };
// flandreA22CEILING_XB declared in flandre.asm line 964
static unsigned char flandreA22CEILING_XB_byteCode[] = {
	0xD9, 0x05, 0x90, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C590] ; line 965
};
static PatchByteCode flandreA22CEILING_XB_patchByteCode = { flandreA22CEILING_XB_byteCode, 6, nullptr, 0 };
// flandreA22CEILING_XC declared in flandre.asm line 967
static unsigned char flandreA22CEILING_XC_byteCode[] = {
	0xD9, 0x05, 0x90, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C590] ; line 968
};
static PatchByteCode flandreA22CEILING_XC_patchByteCode = { flandreA22CEILING_XC_byteCode, 6, nullptr, 0 };
// flandreA22CEILING_SKIP declared in flandre.asm line 970
static unsigned char flandreA22CEILING_SKIP_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 971
	0x60,                                                 // PUSHAD ; line 972
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x0003 ; line 973
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                   // JNE SHORT LABELN ; line 974
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 975
	0xD8, 0x25, 0x60, 0x78, 0x86, 0x00,                   // FSUB DWORD PTR [0x867860] ; line 976
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 977
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 980
	0x0F, 0x85, 0x52, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 981
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 982
	0xD8, 0x35, 0x94, 0xC5, 0x85, 0x00,                   // FDIV DWORD PTR [0x85C594] ; line 983
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF0] ; line 984
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x0000 ; line 985
	0x0F, 0x85, 0x31, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 986
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 987
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 988
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 990
	0xD9, 0x05, 0x44, 0x15, 0x87, 0x00,                   // FLD DWORD PTR [0x871544] ; line 991
	0xDE, 0xD9,                                           // FCOMPP ; line 992
	0xDF, 0xE0,                                           // FNSTSW AX ; line 993
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 994
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 995
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x19, 0x00, // MOV WORD PTR [ESI+0x142],0x0019 ; line 996
	0x61,                                                 // POPAD ; line 999
	0x66, 0x9D,                                           // POPF ; line 1000
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x13E],0x06 ; line 1001
};
static PatchByteCode flandreA22CEILING_SKIP_patchByteCode = { flandreA22CEILING_SKIP_byteCode, 145, nullptr, 0 };
// flandreA22CEILING_BOUNCEX declared in flandre.asm line 1008
static unsigned char flandreA22CEILING_BOUNCEX_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 1009
};
static PatchByteCode flandreA22CEILING_BOUNCEX_patchByteCode = { flandreA22CEILING_BOUNCEX_byteCode, 6, nullptr, 0 };
// flandreA22CEILING_BOUNCEX1 declared in flandre.asm line 1011
static unsigned char flandreA22CEILING_BOUNCEX1_byteCode[] = {
	0xDC, 0x25, 0x80, 0xA2, 0x85, 0x00, // FSUB QWORD PTR [0x85A280] ; line 1012
};
static PatchByteCode flandreA22CEILING_BOUNCEX1_patchByteCode = { flandreA22CEILING_BOUNCEX1_byteCode, 6, nullptr, 0 };
// flandreA22CEILING_BOUNCEX2B declared in flandre.asm line 1014
static unsigned char flandreA22CEILING_BOUNCEX2B_byteCode[] = {
	0xF6, 0xC4, 0x41,             // TEST AH,0x41 ; line 1015
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x593105 ; line 1016
};
static JumpTarget flandreA22CEILING_BOUNCEX2B_jumpData[] = {
	{ 0x004, 0x593105 },
};
static PatchByteCode flandreA22CEILING_BOUNCEX2B_patchByteCode = { flandreA22CEILING_BOUNCEX2B_byteCode, 8, flandreA22CEILING_BOUNCEX2B_jumpData, 1 };
// flandreA22CEILING_BOUNCEX2C declared in flandre.asm line 1018
static unsigned char flandreA22CEILING_BOUNCEX2C_byteCode[] = {
	0xF6, 0xC4, 0x41,             // TEST AH,0x41 ; line 1019
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x5935BE ; line 1020
};
static JumpTarget flandreA22CEILING_BOUNCEX2C_jumpData[] = {
	{ 0x004, 0x5935BE },
};
static PatchByteCode flandreA22CEILING_BOUNCEX2C_patchByteCode = { flandreA22CEILING_BOUNCEX2C_byteCode, 8, flandreA22CEILING_BOUNCEX2C_jumpData, 1 };
// flandreROCKETKICK declared in flandre.asm line 1041
static unsigned char flandreROCKETKICK_byteCode[] = {
	0xD9, 0x05, 0x90, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B990] ; line 1042
};
static PatchByteCode flandreROCKETKICK_patchByteCode = { flandreROCKETKICK_byteCode, 6, nullptr, 0 };
// flandreROCKETKICK_B declared in flandre.asm line 1044
static unsigned char flandreROCKETKICK_B_byteCode[] = {
	0xDD, 0x05, 0x10, 0xCE, 0x85, 0x00, // FLD QWORD PTR [0x85CE10] ; line 1045
};
static PatchByteCode flandreROCKETKICK_B_patchByteCode = { flandreROCKETKICK_B_byteCode, 6, nullptr, 0 };
// flandreROCKETKICK_STARTUPB declared in flandre.asm line 1047
static unsigned char flandreROCKETKICK_STARTUPB_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 1048
	0x60,                                                 // PUSHAD ; line 1049
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1050
	0x0F, 0x85, 0x18, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1051
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1052
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1053
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x20, 0x00, // MOV WORD PTR [ESI+0x142],0x20 ; line 1054
	0x61,                                                 // POPAD ; line 1056
	0x66, 0x9D,                                           // POPF ; line 1057
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x487CA0 ; line 1058
};
static JumpTarget flandreROCKETKICK_STARTUPB_jumpData[] = {
	{ 0x02E, 0x487CA0 },
};
static PatchByteCode flandreROCKETKICK_STARTUPB_patchByteCode = { flandreROCKETKICK_STARTUPB_byteCode, 50, flandreROCKETKICK_STARTUPB_jumpData, 1 };
// flandreROCKETKICK_END declared in flandre.asm line 1071
static unsigned char flandreROCKETKICK_END_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1072
	0x60,                                                       // PUSHAD ; line 1073
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 1075
	0x0F, 0x85, 0x3A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 1076
	0xD9, 0xEE,                                                 // FLDZ ; line 1078
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1079
	0xDE, 0xD9,                                                 // FCOMPP ; line 1080
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1081
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1082
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1083
	0xE9, 0x20, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1084
	0xD9, 0xEE,                                                 // FLDZ ; line 1087
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 1088
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1089
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x50, 0x00,       // MOV WORD PTR [ESI+0x140],0x50 ; line 1090
	0x66, 0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0x50, 0x00,       // MOV WORD PTR [ESI+0x144],0x50 ; line 1091
	0x61,                                                       // POPAD ; line 1094
	0x66, 0x9D,                                                 // POPF ; line 1095
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 1097
};
static PatchByteCode flandreROCKETKICK_END_patchByteCode = { flandreROCKETKICK_END_byteCode, 90, nullptr, 0 };
// flandreROCKETKICK_END1 declared in flandre.asm line 1120
static unsigned char flandreROCKETKICK_END1_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1121
	0x60,                                                       // PUSHAD ; line 1122
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x59 ; line 1123
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 1124
	0xE9, 0x0E, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1126
	0x61,                                                       // POPAD ; line 1128
	0x66, 0x9D,                                                 // POPF ; line 1129
	0x0F, 0x8B, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNP 0x59E2D8 ; line 1130
	0xE9, 0x08, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 1131
	0x61,                                                       // POPAD ; line 1133
	0x66, 0x9D,                                                 // POPF ; line 1134
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x59E2D8 ; line 1135
	0x90,                                                       // NOP ; line 1137
};
static JumpTarget flandreROCKETKICK_END1_jumpData[] = {
	{ 0x01D, 0x59E2D8 },
	{ 0x02A, 0x59E2D8 },
};
static PatchByteCode flandreROCKETKICK_END1_patchByteCode = { flandreROCKETKICK_END1_byteCode, 47, flandreROCKETKICK_END1_jumpData, 2 };
// flandreD623_INVB declared in flandre.asm line 1144
static unsigned char flandreD623_INVB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x5915FB ; line 1145
};
static JumpTarget flandreD623_INVB_jumpData[] = {
	{ 0x001, 0x5915FB },
};
static PatchByteCode flandreD623_INVB_patchByteCode = { flandreD623_INVB_byteCode, 5, flandreD623_INVB_jumpData, 1 };
// flandreD623_INVC declared in flandre.asm line 1147
static unsigned char flandreD623_INVC_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x591760 ; line 1148
};
static JumpTarget flandreD623_INVC_jumpData[] = {
	{ 0x001, 0x591760 },
};
static PatchByteCode flandreD623_INVC_patchByteCode = { flandreD623_INVC_byteCode, 5, flandreD623_INVC_jumpData, 1 };
// flandreD623_INVLV declared in flandre.asm line 1150
static unsigned char flandreD623_INVLV_byteCode[] = {
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x05, // CMP BYTE PTR [ESI+0x6A6],0x05 ; line 1151
};
static PatchByteCode flandreD623_INVLV_patchByteCode = { flandreD623_INVLV_byteCode, 7, nullptr, 0 };
// flandreA22STIG0 declared in flandre.asm line 1161
static unsigned char flandreA22STIG0_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 1162
};
static PatchByteCode flandreA22STIG0_patchByteCode = { flandreA22STIG0_byteCode, 6, nullptr, 0 };
// flandreA22STIG1 declared in flandre.asm line 1164
static unsigned char flandreA22STIG1_byteCode[] = {
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BA68] ; line 1165
};
static PatchByteCode flandreA22STIG1_patchByteCode = { flandreA22STIG1_byteCode, 6, nullptr, 0 };
// flandreA22STIG_SPD declared in flandre.asm line 1171
static unsigned char flandreA22STIG_SPD_byteCode[] = {
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00, // FLD DWORD PTR [0x8589F0] ; line 1172
};
static PatchByteCode flandreA22STIG_SPD_patchByteCode = { flandreA22STIG_SPD_byteCode, 6, nullptr, 0 };
// flandreA22STIG_ROT declared in flandre.asm line 1176
static unsigned char flandreA22STIG_ROT_byteCode[] = {
	0xDC, 0x05, 0x80, 0x7F, 0x85, 0x00, // FADD QWORD PTR [0x857F80] ; line 1177
};
static PatchByteCode flandreA22STIG_ROT_patchByteCode = { flandreA22STIG_ROT_byteCode, 6, nullptr, 0 };
// flandreBLS_FREEZE declared in flandre.asm line 1181
static unsigned char flandreBLS_FREEZE_byteCode[] = {
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,       // MOV EAX,[ESI+0x398] ; line 1182
	0x66, 0x8B, 0x80, 0x96, 0x01, 0x00, 0x00, // MOV AX,[EAX+0x196] ; line 1183
	0x66, 0x81, 0xF8, 0x00, 0x00,             // CMP AX,0x00 ; line 1184
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,       // JNE 0x5A1871 ; line 1185
	0x0F, 0xBF, 0x86, 0x3E, 0x01, 0x00, 0x00, // MOVSX EAX,WORD PTR [ESI+0x13E] ; line 1186
};
static JumpTarget flandreBLS_FREEZE_jumpData[] = {
	{ 0x014, 0x5A1871 },
};
static PatchByteCode flandreBLS_FREEZE_patchByteCode = { flandreBLS_FREEZE_byteCode, 31, flandreBLS_FREEZE_jumpData, 1 };
// flandreBLS_FLASH declared in flandre.asm line 1193
static unsigned char flandreBLS_FLASH_byteCode[] = {
	0xBD, 0x03, 0x00, 0x00, 0x00, // MOV EBP,0x00000003 ; line 1194
	0x66, 0x39, 0xE8,             // CMP AX,BP ; line 1195
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x594CBA ; line 1196
};
static JumpTarget flandreBLS_FLASH_jumpData[] = {
	{ 0x009, 0x594CBA },
};
static PatchByteCode flandreBLS_FLASH_patchByteCode = { flandreBLS_FLASH_byteCode, 13, flandreBLS_FLASH_jumpData, 1 };
// flandreBLS_HEIGHT declared in flandre.asm line 1199
static unsigned char flandreBLS_HEIGHT_byteCode[] = {
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1200
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1202
	0x0F, 0x85, 0x38, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1203
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x140],0x0A ; line 1204
	0x0F, 0x85, 0x29, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1205
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 1206
	0x0F, 0x85, 0x1A, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1207
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1208
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00 ; line 1209
	0x6A, 0x0D,                                                 // PUSH 0x0D ; line 1210
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1211
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1212
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x10, 0x00,       // CMP WORD PTR [ESI+0x140],0x10 ; line 1215
	0x0F, 0x8F, 0x1D, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 1216
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1217
	0xD9, 0xEE,                                                 // FLDZ ; line 1218
	0xDE, 0xD9,                                                 // FCOMPP ; line 1219
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1220
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1221
	0x0F, 0x85, 0x08, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1222
	0xD9, 0xEE,                                                 // FLDZ ; line 1223
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1224
};
static JumpTarget flandreBLS_HEIGHT_jumpData[] = {
	{ 0x049, 0x464980 },
};
static PatchByteCode flandreBLS_HEIGHT_patchByteCode = { flandreBLS_HEIGHT_byteCode, 121, flandreBLS_HEIGHT_jumpData, 1 };
// flandreBLS_FLASH1 declared in flandre.asm line 1229
static unsigned char flandreBLS_FLASH1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 1230
	0x0F, 0x85, 0x23, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1231
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1232
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1233
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x01 ; line 1234
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1235
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x594EE1 ; line 1236
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1238
};
static JumpTarget flandreBLS_FLASH1_jumpData[] = {
	{ 0x02E, 0x594EE1 },
};
static PatchByteCode flandreBLS_FLASH1_patchByteCode = { flandreBLS_FLASH1_byteCode, 60, flandreBLS_FLASH1_jumpData, 1 };
// flandreBLS_RISE declared in flandre.asm line 1243
static unsigned char flandreBLS_RISE_byteCode[] = {
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 1244
	0xD9, 0xE8,                         // FLD1 ; line 1245
	0xDE, 0xD9,                         // FCOMPP ; line 1246
	0xDF, 0xE0,                         // FNSTSW AX ; line 1247
	0xF6, 0xC4, 0x41,                   // TEST AH,0x41 ; line 1248
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00, // JNE SHORT CMPFALSE ; line 1249
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B9D4] ; line 1250
	0xE9, 0x06, 0x00, 0x00, 0x00,       // JMP END ; line 1252
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867810] ; line 1254
};
static PatchByteCode flandreBLS_RISE_patchByteCode = { flandreBLS_RISE_byteCode, 38, nullptr, 0 };
// flandreBLS_RISEDEC declared in flandre.asm line 1257
static unsigned char flandreBLS_RISEDEC_byteCode[] = {
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00, // FLD DWORD PTR [0x8677A8] ; line 1259
};
static PatchByteCode flandreBLS_RISEDEC_patchByteCode = { flandreBLS_RISEDEC_byteCode, 6, nullptr, 0 };
// flandreBLS_PARTICLE declared in flandre.asm line 1299
static unsigned char flandreBLS_PARTICLE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x02 ; line 1300
};
static PatchByteCode flandreBLS_PARTICLE_patchByteCode = { flandreBLS_PARTICLE_byteCode, 9, nullptr, 0 };
// flandreBLS_SKIPMOMENTUM declared in flandre.asm line 1304
static unsigned char flandreBLS_SKIPMOMENTUM_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x594DB1 ; line 1305
};
static JumpTarget flandreBLS_SKIPMOMENTUM_jumpData[] = {
	{ 0x001, 0x594DB1 },
};
static PatchByteCode flandreBLS_SKIPMOMENTUM_patchByteCode = { flandreBLS_SKIPMOMENTUM_byteCode, 5, flandreBLS_SKIPMOMENTUM_jumpData, 1 };
// flandreBLS_UNSTICK declared in flandre.asm line 1309
static unsigned char flandreBLS_UNSTICK_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x594E8E ; line 1310
};
static JumpTarget flandreBLS_UNSTICK_jumpData[] = {
	{ 0x001, 0x594E8E },
};
static PatchByteCode flandreBLS_UNSTICK_patchByteCode = { flandreBLS_UNSTICK_byteCode, 5, flandreBLS_UNSTICK_jumpData, 1 };
// flandreBLS_EFF declared in flandre.asm line 1314
static unsigned char flandreBLS_EFF_byteCode[] = {
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF, // JGE 0x59E2D8 ; line 1315
};
static JumpTarget flandreBLS_EFF_jumpData[] = {
	{ 0x002, 0x59E2D8 },
};
static PatchByteCode flandreBLS_EFF_patchByteCode = { flandreBLS_EFF_byteCode, 6, flandreBLS_EFF_jumpData, 1 };
// flandreD22_B_RISE declared in flandre.asm line 1335
static unsigned char flandreD22_B_RISE_byteCode[] = {
	0x6A, 0x32,                               // PUSH 0x32 ; line 1336
	0x89, 0xF1,                               // MOV ECX,ESI ; line 1337
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487870 ; line 1338
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 1339
	0xDD, 0x05, 0x68, 0xB9, 0x85, 0x00,       // FLD QWORD PTR [0x85B968] ; line 1340
	0xDE, 0xD9,                               // FCOMPP ; line 1341
	0xDF, 0xE0,                               // FNSTSW AX ; line 1342
	0xF6, 0xC4, 0x41,                         // TEST AH,0x41 ; line 1343
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,       // JNE SHORT CMPFALSE ; line 1344
	0xD9, 0xEE,                               // FLDZ ; line 1345
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF4] ; line 1346
	0xD9, 0xEE,                               // FLDZ ; line 1348
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x02, // MOV BYTE PTR [ESI+0x194],0x02 ; line 1349
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP END ; line 1350
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x867810] ; line 1352
};
static JumpTarget flandreD22_B_RISE_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode flandreD22_B_RISE_patchByteCode = { flandreD22_B_RISE_byteCode, 62, flandreD22_B_RISE_jumpData, 1 };
// flandreD22_C_RISE declared in flandre.asm line 1355
static unsigned char flandreD22_C_RISE_byteCode[] = {
	0x6A, 0x32,                               // PUSH 0x32 ; line 1356
	0x89, 0xF1,                               // MOV ECX,ESI ; line 1357
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487870 ; line 1358
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 1359
	0xDD, 0x05, 0x68, 0xB9, 0x85, 0x00,       // FLD QWORD PTR [0x85B968] ; line 1360
	0xDE, 0xD9,                               // FCOMPP ; line 1361
	0xDF, 0xE0,                               // FNSTSW AX ; line 1362
	0xF6, 0xC4, 0x41,                         // TEST AH,0x41 ; line 1363
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,       // JNE SHORT CMPFALSE ; line 1364
	0xD9, 0xEE,                               // FLDZ ; line 1365
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF4] ; line 1366
	0xD9, 0xEE,                               // FLDZ ; line 1368
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x02, // MOV BYTE PTR [ESI+0x194],0x02 ; line 1369
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP END ; line 1370
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B9D4] ; line 1372
};
static JumpTarget flandreD22_C_RISE_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode flandreD22_C_RISE_patchByteCode = { flandreD22_C_RISE_byteCode, 62, flandreD22_C_RISE_jumpData, 1 };
// flandreD22_RISEDEC declared in flandre.asm line 1377
static unsigned char flandreD22_RISEDEC_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 1379
};
static PatchByteCode flandreD22_RISEDEC_patchByteCode = { flandreD22_RISEDEC_byteCode, 2, nullptr, 0 };
// flandreD22S_PARTICLE declared in flandre.asm line 1395
static unsigned char flandreD22S_PARTICLE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x01 ; line 1396
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x5927B5 ; line 1397
};
static JumpTarget flandreD22S_PARTICLE_jumpData[] = {
	{ 0x00B, 0x5927B5 },
};
static PatchByteCode flandreD22S_PARTICLE_patchByteCode = { flandreD22S_PARTICLE_byteCode, 15, flandreD22S_PARTICLE_jumpData, 1 };
// flandreD22S_RESPIN declared in flandre.asm line 1402
static unsigned char flandreD22S_RESPIN_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x31, 0x02,       // CMP WORD PTR [ESI+0x13C],0x231 ; line 1404
	0x0F, 0x85, 0x7C, 0x00, 0x00, 0x00,                         // JNE LABELR ; line 1405
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 1406
	0x0F, 0x85, 0x6D, 0x00, 0x00, 0x00,                         // JNE LABELR ; line 1407
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0F, 0x00,       // CMP WORD PTR [ESI+0x140],0x0F ; line 1408
	0x0F, 0x84, 0x5E, 0x00, 0x00, 0x00,                         // JE LABELR ; line 1409
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00,       // CMP WORD PTR [ESI+0x140],0x07 ; line 1410
	0x0F, 0x8D, 0x47, 0x00, 0x00, 0x00,                         // JGE LABELR2 ; line 1411
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x03 ; line 1412
	0x0F, 0x8C, 0x40, 0x00, 0x00, 0x00,                         // JL LABELR ; line 1413
	0x0F, 0x85, 0x1B, 0x00, 0x00, 0x00,                         // JNE LABELR1 ; line 1414
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1415
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                         // JNE LABELR1 ; line 1416
	0xD9, 0x05, 0x2C, 0xC2, 0x85, 0x00,                         // FLD DWORD PTR [0x85C22C] ; line 1417
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1418
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1420
	0xD8, 0x25, 0xA8, 0x77, 0x86, 0x00,                         // FSUB DWORD PTR [0x8677A8] ; line 1421
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1422
	0xE9, 0x08, 0x00, 0x00, 0x00,                               // JMP LABELR ; line 1423
	0xD9, 0xEE,                                                 // FLDZ ; line 1425
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1426
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x5E, 0x02,       // CMP WORD PTR [ESI+0x13C],0x025E ; line 1430
	0x0F, 0x84, 0xC2, 0x00, 0x00, 0x00,                         // JE LABELQ ; line 1431
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 1432
	0x0F, 0x85, 0xB3, 0x00, 0x00, 0x00,                         // JNE LABELQ ; line 1433
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x08, 0x00,       // CMP WORD PTR [ESI+0x140],0x08 ; line 1434
	0x0F, 0x8C, 0xA4, 0x00, 0x00, 0x00,                         // JL LABELQ ; line 1435
	0x66, 0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x760],0x0A ; line 1437
	0x0F, 0x8F, 0x0F, 0x00, 0x00, 0x00,                         // JG LABELP1 ; line 1438
	0x66, 0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x760],0x00 ; line 1439
	0x0F, 0x8F, 0x5F, 0x00, 0x00, 0x00,                         // JG LABELP ; line 1440
	0x66, 0x81, 0xBE, 0x80, 0x07, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x780],0x0A ; line 1442
	0x0F, 0x8F, 0x0F, 0x00, 0x00, 0x00,                         // JG LABELP2 ; line 1443
	0x66, 0x81, 0xBE, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x780],0x00 ; line 1444
	0x0F, 0x8F, 0x41, 0x00, 0x00, 0x00,                         // JG LABELP ; line 1445
	0x66, 0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x764],0x0A ; line 1447
	0x0F, 0x8F, 0x0F, 0x00, 0x00, 0x00,                         // JG LABELP3 ; line 1448
	0x66, 0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x764],0x00 ; line 1449
	0x0F, 0x8F, 0x23, 0x00, 0x00, 0x00,                         // JG LABELP ; line 1450
	0x66, 0x81, 0xBE, 0x84, 0x07, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [ESI+0x784],0x0A ; line 1452
	0x0F, 0x8F, 0x0F, 0x00, 0x00, 0x00,                         // JG LABELP4 ; line 1453
	0x66, 0x81, 0xBE, 0x84, 0x07, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x784],0x00 ; line 1454
	0x0F, 0x8F, 0x05, 0x00, 0x00, 0x00,                         // JG LABELP ; line 1455
	0xE9, 0x27, 0x00, 0x00, 0x00,                               // JMP LABELQ ; line 1457
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x142],0xFF ; line 1459
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x0F, 0x00,       // MOV WORD PTR [ESI+0x140],0x0F ; line 1460
	0xD9, 0x05, 0xB8, 0xC5, 0x85, 0x00,                         // FLD DWORD PTR [0x85C5B8] ; line 1463
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1464
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 1466
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1467
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1468
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 1472
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                         // JNE LABELO ; line 1473
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0E, 0x00,       // CMP WORD PTR [ESI+0x140],0x0E ; line 1474
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE LABELO ; line 1475
	0x8B, 0x0E,                                                 // MOV ECX,[ESI] ; line 1477
	0x8B, 0x41, 0x04,                                           // MOV EAX,[ECX+0x04] ; line 1478
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1479
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1480
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1481
	0xFF, 0xD0,                                                 // CALL EAX ; line 1482
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 1486
	0x0F, 0x85, 0x43, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 1487
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 1488
	0x0F, 0x85, 0x34, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 1489
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1490
	0x0F, 0x85, 0x25, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 1491
	0x66, 0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x144],0x00 ; line 1492
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 1493
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x86782C] ; line 1494
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1495
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00 ; line 1496
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 1499
	0x0F, 0x8C, 0x1E, 0x00, 0x00, 0x00,                         // JL LABELN ; line 1500
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00,                         // FLD DWORD PTR [0x8677A8] ; line 1501
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 1502
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1503
	0xDC, 0x0D, 0x20, 0xB9, 0x85, 0x00,                         // FMUL QWORD PTR [0x85B920] ; line 1504
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1505
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x02,                   // CMP BYTE PTR [ESI+0x194],0x02 ; line 1508
	0x0F, 0x8D, 0x53, 0x00, 0x00, 0x00,                         // JGE LABEL1 ; line 1509
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x01 ; line 1510
	0x0F, 0x85, 0x44, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1511
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 1512
	0x0F, 0x85, 0x35, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1513
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x5E, 0x02,       // CMP WORD PTR [ESI+0x13C],0x025E ; line 1514
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 1515
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1516
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1517
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x142],0xFF ; line 1519
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x0F, 0x00,       // MOV WORD PTR [ESI+0x140],0x0F ; line 1520
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP END ; line 1521
	0x39, 0xAE, 0x44, 0x01, 0x00, 0x00,                         // CMP [ESI+0x144],EBP ; line 1524
};
static JumpTarget flandreD22S_RESPIN_jumpData[] = {
	{ 0x158, 0x464980 },
};
static PatchByteCode flandreD22S_RESPIN_patchByteCode = { flandreD22S_RESPIN_byteCode, 620, flandreD22S_RESPIN_jumpData, 1 };
// flandreD22S_B_SKIPMOMENTUM declared in flandre.asm line 1529
static unsigned char flandreD22S_B_SKIPMOMENTUM_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x59275A ; line 1530
};
static JumpTarget flandreD22S_B_SKIPMOMENTUM_jumpData[] = {
	{ 0x001, 0x59275A },
};
static PatchByteCode flandreD22S_B_SKIPMOMENTUM_patchByteCode = { flandreD22S_B_SKIPMOMENTUM_byteCode, 5, flandreD22S_B_SKIPMOMENTUM_jumpData, 1 };
// flandreD22S_C_SKIPMOMENTUM declared in flandre.asm line 1532
static unsigned char flandreD22S_C_SKIPMOMENTUM_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x592ADD ; line 1533
};
static JumpTarget flandreD22S_C_SKIPMOMENTUM_jumpData[] = {
	{ 0x001, 0x592ADD },
};
static PatchByteCode flandreD22S_C_SKIPMOMENTUM_patchByteCode = { flandreD22S_C_SKIPMOMENTUM_byteCode, 5, flandreD22S_C_SKIPMOMENTUM_jumpData, 1 };
// flandreD22S_B_XFIX declared in flandre.asm line 1539
static unsigned char flandreD22S_B_XFIX_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF4] ; line 1540
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x592613 ; line 1541
};
static JumpTarget flandreD22S_B_XFIX_jumpData[] = {
	{ 0x007, 0x592613 },
};
static PatchByteCode flandreD22S_B_XFIX_patchByteCode = { flandreD22S_B_XFIX_byteCode, 11, flandreD22S_B_XFIX_jumpData, 1 };
// flandreD22S_C_XFIX declared in flandre.asm line 1543
static unsigned char flandreD22S_C_XFIX_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF4] ; line 1544
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x592996 ; line 1545
};
static JumpTarget flandreD22S_C_XFIX_jumpData[] = {
	{ 0x007, 0x592996 },
};
static PatchByteCode flandreD22S_C_XFIX_patchByteCode = { flandreD22S_C_XFIX_byteCode, 11, flandreD22S_C_XFIX_jumpData, 1 };
// flandreD22S_B_UNSTICK declared in flandre.asm line 1550
static unsigned char flandreD22S_B_UNSTICK_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x592817 ; line 1551
};
static JumpTarget flandreD22S_B_UNSTICK_jumpData[] = {
	{ 0x001, 0x592817 },
};
static PatchByteCode flandreD22S_B_UNSTICK_patchByteCode = { flandreD22S_B_UNSTICK_byteCode, 5, flandreD22S_B_UNSTICK_jumpData, 1 };
// flandreD22S_C_UNSTICK declared in flandre.asm line 1553
static unsigned char flandreD22S_C_UNSTICK_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x592BA8 ; line 1554
};
static JumpTarget flandreD22S_C_UNSTICK_jumpData[] = {
	{ 0x001, 0x592BA8 },
};
static PatchByteCode flandreD22S_C_UNSTICK_patchByteCode = { flandreD22S_C_UNSTICK_byteCode, 5, flandreD22S_C_UNSTICK_jumpData, 1 };
// flandreD22S_DRAWORDER declared in flandre.asm line 1559
static unsigned char flandreD22S_DRAWORDER_byteCode[] = {
	0x6A, 0x01,                         // PUSH 0x01 ; line 1560
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 1561
};
static PatchByteCode flandreD22S_DRAWORDER_patchByteCode = { flandreD22S_DRAWORDER_byteCode, 8, nullptr, 0 };
// flandreD22S_EFF declared in flandre.asm line 1566
static unsigned char flandreD22S_EFF_byteCode[] = {
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF, // JGE 0x59E2D8 ; line 1567
};
static JumpTarget flandreD22S_EFF_jumpData[] = {
	{ 0x002, 0x59E2D8 },
};
static PatchByteCode flandreD22S_EFF_patchByteCode = { flandreD22S_EFF_byteCode, 6, flandreD22S_EFF_jumpData, 1 };
// flandreD22S_EFF2 declared in flandre.asm line 1571
static unsigned char flandreD22S_EFF2_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x48CF60 ; line 1572
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 1573
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ECX+0xEC] ; line 1574
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xEC] ; line 1575
	0xD9, 0x81, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ECX+0xF0] ; line 1576
	0xD8, 0x05, 0xD8, 0x7D, 0x85, 0x00,                   // FADD DWORD PTR [0x857DD8] ; line 1577
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF0] ; line 1578
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0x5E, 0x02, // CMP WORD PTR [ECX+0x13C],0x025E ; line 1579
	0x0F, 0x84, 0x3F, 0x00, 0x00, 0x00,                   // JE LABELN ; line 1580
	0x66, 0x81, 0xB9, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ECX+0x13E],0x02 ; line 1581
	0x0F, 0x85, 0x30, 0x00, 0x00, 0x00,                   // JNE LABELN ; line 1582
	0x66, 0x81, 0xB9, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ECX+0x140],0x00 ; line 1583
	0x0F, 0x85, 0x21, 0x00, 0x00, 0x00,                   // JNE LABELN ; line 1584
	0x66, 0x81, 0xB9, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ECX+0x142],0x00 ; line 1585
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                   // JNE LABELN ; line 1586
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x20, 0x00, // MOV WORD PTR [ESI+0x140],0x20 ; line 1587
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xC7, 0x00, // MOV WORD PTR [ESI+0x142],0xC7 ; line 1588
};
static JumpTarget flandreD22S_EFF2_jumpData[] = {
	{ 0x001, 0x48CF60 },
};
static PatchByteCode flandreD22S_EFF2_patchByteCode = { flandreD22S_EFF2_byteCode, 119, flandreD22S_EFF2_jumpData, 1 };
// flandreD22S_NOBOUNCE declared in flandre.asm line 1593
static unsigned char flandreD22S_NOBOUNCE_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x592CAF ; line 1594
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x190] ; line 1595
	0x39, 0xF8,                         // CMP EAX,EDI ; line 1596
	0x81, 0xF8, 0x07, 0x00, 0x00, 0x00, // CMP EAX,0x07 ; line 1597
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x5928AA ; line 1598
};
static JumpTarget flandreD22S_NOBOUNCE_jumpData[] = {
	{ 0x001, 0x592CAF },
	{ 0x014, 0x5928AA },
};
static PatchByteCode flandreD22S_NOBOUNCE_patchByteCode = { flandreD22S_NOBOUNCE_byteCode, 24, flandreD22S_NOBOUNCE_jumpData, 2 };
// flandreD22S_LV declared in flandre.asm line 1603
static unsigned char flandreD22S_LV_byteCode[] = {
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1604
	0x0F, 0x85, 0x2F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1605
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1606
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1607
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x01 ; line 1608
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1609
	0x0F, 0xB6, 0x9E, 0xA7, 0x06, 0x00, 0x00,             // MOVZX EBX,BYTE PTR [ESI+0x6A7] ; line 1610
	0x6B, 0xDB, 0x0C,                                     // IMUL BX,BX,0x0C ; line 1611
	0x66, 0x89, 0x9E, 0x42, 0x01, 0x00, 0x00,             // MOV [ESI+0x142],BX ; line 1612
	0xBB, 0x05, 0x00, 0x00, 0x00,                         // MOV EBX,0x00000005 ; line 1614
};
static PatchByteCode flandreD22S_LV_patchByteCode = { flandreD22S_LV_byteCode, 67, nullptr, 0 };
// flandreA214FORK_LV1 declared in flandre.asm line 1619
static unsigned char flandreA214FORK_LV1_byteCode[] = {
	0x0F, 0xBE, 0x96, 0xAD, 0x06, 0x00, 0x00, // MOVSX EDX,BYTE PTR [ESI+0x6AD] ; line 1620
	0x80, 0xBE, 0xAD, 0x06, 0x00, 0x00, 0x03, // CMP BYTE PTR [ESI+0x6AD],0x03 ; line 1621
	0x0F, 0x8C, 0x06, 0x00, 0x00, 0x00,       // JL LABEL1 ; line 1622
	0x81, 0xEA, 0x01, 0x00, 0x00, 0x00,       // SUB EDX,0x01 ; line 1623
};
static PatchByteCode flandreA214FORK_LV1_patchByteCode = { flandreA214FORK_LV1_byteCode, 26, nullptr, 0 };
// flandreA214FORK_LV2 declared in flandre.asm line 1629
static unsigned char flandreA214FORK_LV2_byteCode[] = {
	0x0F, 0xBE, 0x86, 0xAD, 0x06, 0x00, 0x00, // MOVSX EAX,BYTE PTR [ESI+0x6AD] ; line 1630
	0x80, 0xBE, 0xAD, 0x06, 0x00, 0x00, 0x03, // CMP BYTE PTR [ESI+0x6AD],0x03 ; line 1631
	0x0F, 0x8C, 0x06, 0x00, 0x00, 0x00,       // JL LABEL1 ; line 1632
	0x81, 0xE8, 0x01, 0x00, 0x00, 0x00,       // SUB EAX,0x01 ; line 1633
};
static PatchByteCode flandreA214FORK_LV2_patchByteCode = { flandreA214FORK_LV2_byteCode, 26, nullptr, 0 };
// flandreA214FORK_SOUND declared in flandre.asm line 1639
static unsigned char flandreA214FORK_SOUND_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 1640
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x140],0x06 ; line 1641
	0x0F, 0x85, 0x27, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1642
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1643
	0x0F, 0x85, 0x18, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1644
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 1645
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1646
	0x6A, 0x16,                                           // PUSH 0x16 ; line 1647
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1648
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 1649
	0x66, 0x9D,                                           // POPF ; line 1651
	0xC6, 0x86, 0x8C, 0x01, 0x00, 0x00, 0x09,             // MOV BYTE PTR [ESI+0x18C],0x09 ; line 1652
};
static JumpTarget flandreA214FORK_SOUND_jumpData[] = {
	{ 0x034, 0x464980 },
};
static PatchByteCode flandreA214FORK_SOUND_patchByteCode = { flandreA214FORK_SOUND_byteCode, 65, flandreA214FORK_SOUND_jumpData, 1 };
// flandreA214FORK_POSX declared in flandre.asm line 1685
static unsigned char flandreA214FORK_POSX_byteCode[] = {
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xEC] ; line 1686
};
static PatchByteCode flandreA214FORK_POSX_patchByteCode = { flandreA214FORK_POSX_byteCode, 6, nullptr, 0 };
// flandreA214FORK_POSY declared in flandre.asm line 1689
static unsigned char flandreA214FORK_POSY_byteCode[] = {
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B984] ; line 1690
};
static PatchByteCode flandreA214FORK_POSY_patchByteCode = { flandreA214FORK_POSY_byteCode, 6, nullptr, 0 };
// flandreA214FORK declared in flandre.asm line 1704
static unsigned char flandreA214FORK_byteCode[] = {
	0x60,                               // PUSHAD ; line 1705
	0x8B, 0x86, 0x70, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x170] ; line 1706
	0xD8, 0x80, 0xEC, 0x00, 0x00, 0x00, // FADD DWORD PTR [EAX+0xEC] ; line 1707
	0x61,                               // POPAD ; line 1708
};
static PatchByteCode flandreA214FORK_patchByteCode = { flandreA214FORK_byteCode, 14, nullptr, 0 };
// flandreA214FORK_0 declared in flandre.asm line 1720
static unsigned char flandreA214FORK_0_byteCode[] = {
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x59E2D8 ; line 1721
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x59FC80 ; line 1722
};
static JumpTarget flandreA214FORK_0_jumpData[] = {
	{ 0x002, 0x59E2D8 },
	{ 0x007, 0x59FC80 },
};
static PatchByteCode flandreA214FORK_0_patchByteCode = { flandreA214FORK_0_byteCode, 11, flandreA214FORK_0_jumpData, 2 };
// flandre3SCCROSS_HEIGHT declared in flandre.asm line 1726
static unsigned char flandre3SCCROSS_HEIGHT_byteCode[] = {
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF0] ; line 1727
	0xD9, 0xEE,                         // FLDZ ; line 1728
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x593CA2 ; line 1729
};
static JumpTarget flandre3SCCROSS_HEIGHT_jumpData[] = {
	{ 0x009, 0x593CA2 },
};
static PatchByteCode flandre3SCCROSS_HEIGHT_patchByteCode = { flandre3SCCROSS_HEIGHT_byteCode, 13, flandre3SCCROSS_HEIGHT_jumpData, 1 };
// flandre5SCCROSS_HEIGHT declared in flandre.asm line 1731
static unsigned char flandre5SCCROSS_HEIGHT_byteCode[] = {
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF0] ; line 1732
	0xD9, 0xEE,                         // FLDZ ; line 1733
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x594419 ; line 1734
};
static JumpTarget flandre5SCCROSS_HEIGHT_jumpData[] = {
	{ 0x009, 0x594419 },
};
static PatchByteCode flandre5SCCROSS_HEIGHT_patchByteCode = { flandre5SCCROSS_HEIGHT_byteCode, 13, flandre5SCCROSS_HEIGHT_jumpData, 1 };
// flandre5SCCRADLE_DUR declared in flandre.asm line 1746
static unsigned char flandre5SCCRADLE_DUR_byteCode[] = {
	0x0F, 0xB7, 0x86, 0xD0, 0x07, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x7D0] ; line 1747
	0x66, 0x81, 0xF8, 0x17, 0x00,             // CMP AX,0x0017 ; line 1748
};
static PatchByteCode flandre5SCCRADLE_DUR_patchByteCode = { flandre5SCCRADLE_DUR_byteCode, 12, nullptr, 0 };
// flandre5SCCRADLE_0 declared in flandre.asm line 1755
static unsigned char flandre5SCCRADLE_0_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 1756
};
static PatchByteCode flandre5SCCRADLE_0_patchByteCode = { flandre5SCCRADLE_0_byteCode, 6, nullptr, 0 };
// flandre5SCCRADLE_1 declared in flandre.asm line 1760
static unsigned char flandre5SCCRADLE_1_byteCode[] = {
	0xDD, 0x05, 0xA0, 0x8E, 0x85, 0x00, // FLD QWORD PTR [0x858EA0] ; line 1761
};
static PatchByteCode flandre5SCCRADLE_1_patchByteCode = { flandre5SCCRADLE_1_byteCode, 6, nullptr, 0 };
// flandre2SC5SC declared in flandre.asm line 1770
static unsigned char flandre2SC5SC_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x5A, 0x02, // CMP WORD PTR [ESI+0x13C],0x025A ; line 1771
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00,                   // FLD DWORD PTR [0x8677A8] ; line 1772
	0x0F, 0x84, 0x04, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1773
	0xD9, 0xE9,                                           // FLDL2T ; line 1774
	0xDE, 0xF9,                                           // FDIVP ST1 ; line 1775
	0x90,                                                 // NOP ; line 1777
};
static PatchByteCode flandre2SC5SC_patchByteCode = { flandre2SC5SC_byteCode, 26, nullptr, 0 };
// flandreD236_UP declared in flandre.asm line 1783
static unsigned char flandreD236_UP_byteCode[] = {
	0xD9, 0x05, 0xBC, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBBC] ; line 1784
};
static PatchByteCode flandreD236_UP_patchByteCode = { flandreD236_UP_byteCode, 6, nullptr, 0 };
// flandreD236_GRAV declared in flandre.asm line 1788
static unsigned char flandreD236_GRAV_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1789
};
static PatchByteCode flandreD236_GRAV_patchByteCode = { flandreD236_GRAV_byteCode, 2, nullptr, 0 };
// flandreD236_CANCEL declared in flandre.asm line 1794
static unsigned char flandreD236_CANCEL_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1795
	0x60,                                                       // PUSHAD ; line 1796
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 1797
	0x0F, 0x85, 0x1A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1798
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1799
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1800
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 1801
	0x61,                                                       // POPAD ; line 1803
	0x66, 0x9D,                                                 // POPF ; line 1804
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,                   // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 1805
};
static PatchByteCode flandreD236_CANCEL_patchByteCode = { flandreD236_CANCEL_byteCode, 54, nullptr, 0 };
// flandreD236 declared in flandre.asm line 1811
static unsigned char flandreD236_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0xF5, 0x01, // CMP WORD PTR [ESI+0x13C],0x01F5 ; line 1812
	0x0F, 0x84, 0x2B, 0x00, 0x00, 0x00,                   // JE SHORT LABELC ; line 1813
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FLD QWORD PTR [0x867830] ; line 1815
	0x60,                                                 // PUSHAD ; line 1816
	0x0F, 0xB6, 0x86, 0xA4, 0x06, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x6A4] ; line 1817
	0x66, 0x81, 0xF8, 0x00, 0x00,                         // CMP AX,0x0000 ; line 1819
	0x0F, 0x8E, 0x0D, 0x00, 0x00, 0x00,                   // JLE SHORT LABEL1 ; line 1820
	0x66, 0x48,                                           // DEC AX ; line 1821
	0xD8, 0x05, 0xF0, 0x89, 0x85, 0x00,                   // FADD DWORD PTR [0x8589F0] ; line 1822
	0xE9, 0xE8, 0xFF, 0xFF, 0xFF,                         // JMP SHORT LABEL2 ; line 1823
	0xE9, 0x26, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL3 ; line 1825
	0xDD, 0x05, 0x90, 0x89, 0x85, 0x00,                   // FLD QWORD PTR [0x858990] ; line 1828
	0x60,                                                 // PUSHAD ; line 1829
	0x0F, 0xB6, 0x86, 0xA4, 0x06, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x6A4] ; line 1830
	0x66, 0x81, 0xF8, 0x00, 0x00,                         // CMP AX,0x0000 ; line 1832
	0x0F, 0x8E, 0x0D, 0x00, 0x00, 0x00,                   // JLE SHORT LABEL3 ; line 1833
	0x66, 0x48,                                           // DEC AX ; line 1834
	0xD8, 0x05, 0x60, 0x78, 0x86, 0x00,                   // FADD DWORD PTR [0x867860] ; line 1835
	0xE9, 0xE8, 0xFF, 0xFF, 0xFF,                         // JMP SHORT LABEL4 ; line 1836
	0x61,                                                 // POPAD ; line 1838
	0x90,                                                 // NOP ; line 1839
};
static PatchByteCode flandreD236_patchByteCode = { flandreD236_byteCode, 98, nullptr, 0 };
// flandreD236C declared in flandre.asm line 1843
static unsigned char flandreD236C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58DB6D ; line 1844
};
static JumpTarget flandreD236C_jumpData[] = {
	{ 0x001, 0x58DB6D },
};
static PatchByteCode flandreD236C_patchByteCode = { flandreD236C_byteCode, 5, flandreD236C_jumpData, 1 };
// flandreJ8A_GRAVITY declared in flandre.asm line 1848
static unsigned char flandreJ8A_GRAVITY_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1850
};
static PatchByteCode flandreJ8A_GRAVITY_patchByteCode = { flandreJ8A_GRAVITY_byteCode, 2, nullptr, 0 };
// flandreA214CHAIN_ANG declared in flandre.asm line 1857
static unsigned char flandreA214CHAIN_ANG_byteCode[] = {
	0xD9, 0x5C, 0x24, 0x28,       // FSTP DWORD PTR [ESP+0x28] ; line 1858
	0xD9, 0xEE,                   // FLDZ ; line 1859
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x59F3F4 ; line 1860
};
static JumpTarget flandreA214CHAIN_ANG_jumpData[] = {
	{ 0x007, 0x59F3F4 },
};
static PatchByteCode flandreA214CHAIN_ANG_patchByteCode = { flandreA214CHAIN_ANG_byteCode, 11, flandreA214CHAIN_ANG_jumpData, 1 };
// flandreA214CHAIN_ANGSTART1 declared in flandre.asm line 1864
static unsigned char flandreA214CHAIN_ANGSTART1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02, // CMP WORD PTR [ESI+0x13C],0x020D ; line 1865
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1866
	0xD9, 0x05, 0x54, 0xC1, 0x85, 0x00,                   // FLD DWORD PTR [0x85C154] ; line 1867
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 1868
	0xD9, 0x05, 0x08, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B908] ; line 1870
	0xD9, 0x9E, 0xE4, 0x07, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x7E4] ; line 1872
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x590607 ; line 1873
};
static JumpTarget flandreA214CHAIN_ANGSTART1_jumpData[] = {
	{ 0x027, 0x590607 },
};
static PatchByteCode flandreA214CHAIN_ANGSTART1_patchByteCode = { flandreA214CHAIN_ANGSTART1_byteCode, 43, flandreA214CHAIN_ANGSTART1_jumpData, 1 };
// flandreA214CHAIN_ANGSTART2 declared in flandre.asm line 1875
static unsigned char flandreA214CHAIN_ANGSTART2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0F, 0x02, // CMP WORD PTR [ESI+0x13C],0x020F ; line 1876
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1877
	0xD9, 0x05, 0xD8, 0x7D, 0x85, 0x00,                   // FLD DWORD PTR [0x857DD8] ; line 1878
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 1879
	0xD9, 0xEE,                                           // FLDZ ; line 1881
	0xD9, 0x9E, 0xE4, 0x07, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x7E4] ; line 1883
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x590B64 ; line 1884
};
static JumpTarget flandreA214CHAIN_ANGSTART2_jumpData[] = {
	{ 0x023, 0x590B64 },
};
static PatchByteCode flandreA214CHAIN_ANGSTART2_patchByteCode = { flandreA214CHAIN_ANGSTART2_byteCode, 39, flandreA214CHAIN_ANGSTART2_jumpData, 1 };
// flandreA214CHAIN0 declared in flandre.asm line 1890
static unsigned char flandreA214CHAIN0_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 1891
	0x0F, 0x85, 0x31, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1892
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 1893
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1894
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1895
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1896
	0x60,                                                 // PUSHAD ; line 1897
	0x0F, 0xB6, 0x86, 0xA9, 0x06, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x6A9] ; line 1898
	0x6B, 0xC0, 0x02,                                     // IMUL EAX,EAX,0x02 ; line 1899
	0x66, 0x89, 0x86, 0x42, 0x01, 0x00, 0x00,             // MOV WORD PTR [ESI+0x142],AX ; line 1900
	0x61,                                                 // POPAD ; line 1901
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0E, 0x02, // CMP WORD PTR [ESI+0x13C],0x020E ; line 1903
	0x0F, 0x8F, 0x0E, 0x00, 0x00, 0x00,                   // JG SHORT LABEL2 ; line 1904
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 1905
	0xE9, 0x07, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL3 ; line 1906
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 1908
	0x90,                                                 // NOP ; line 1910
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x5904EF ; line 1911
};
static JumpTarget flandreA214CHAIN0_jumpData[] = {
	{ 0x066, 0x5904EF },
};
static PatchByteCode flandreA214CHAIN0_patchByteCode = { flandreA214CHAIN0_byteCode, 106, flandreA214CHAIN0_jumpData, 1 };
// flandreA214CHAIN1 declared in flandre.asm line 1913
static unsigned char flandreA214CHAIN1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 1914
	0x0F, 0x85, 0x31, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1915
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 1916
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1917
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1918
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1919
	0x60,                                                 // PUSHAD ; line 1920
	0x0F, 0xB6, 0x86, 0xA9, 0x06, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x6A9] ; line 1921
	0x6B, 0xC0, 0x02,                                     // IMUL EAX,EAX,0x02 ; line 1922
	0x66, 0x89, 0x86, 0x42, 0x01, 0x00, 0x00,             // MOV WORD PTR [ESI+0x142],AX ; line 1923
	0x61,                                                 // POPAD ; line 1924
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0E, 0x02, // CMP WORD PTR [ESI+0x13C],0x020E ; line 1926
	0x0F, 0x8F, 0x0E, 0x00, 0x00, 0x00,                   // JG SHORT LABEL2 ; line 1927
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 1928
	0xE9, 0x07, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL3 ; line 1929
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 1931
	0x90,                                                 // NOP ; line 1933
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x59098C ; line 1934
};
static JumpTarget flandreA214CHAIN1_jumpData[] = {
	{ 0x066, 0x59098C },
};
static PatchByteCode flandreA214CHAIN1_patchByteCode = { flandreA214CHAIN1_byteCode, 106, flandreA214CHAIN1_jumpData, 1 };
// flandreA214CHAIN declared in flandre.asm line 1940
static unsigned char flandreA214CHAIN_byteCode[] = {
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x190] ; line 1942
	0x81, 0xF8, 0x00, 0x00, 0x00, 0x00,                         // CMP EAX,0x00 ; line 1944
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,                         // JE SHORT LABELC1 ; line 1945
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                         // CMP EAX,0x01 ; line 1946
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,                         // JE SHORT LABELC2 ; line 1947
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00,                         // CMP EAX,0x02 ; line 1948
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                         // JE SHORT LABELC2 ; line 1949
	0x81, 0xF8, 0x07, 0x00, 0x00, 0x00,                         // CMP EAX,0x07 ; line 1950
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABELC2 ; line 1951
	0xE9, 0x05, 0x00, 0x00, 0x00,                               // JMP SHORT LABELC3 ; line 1953
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x59F266 ; line 1955
	0x60,                                                       // PUSHAD ; line 1958
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 1959
	0xD9, 0xEE,                                                 // FLDZ ; line 1960
	0xDE, 0xD9,                                                 // FCOMPP ; line 1961
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1962
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1963
	0x0F, 0x84, 0x39, 0x00, 0x00, 0x00,                         // JE SHORT LABELC4 ; line 1964
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1965
	0xD9, 0xEE,                                                 // FLDZ ; line 1966
	0xDE, 0xD9,                                                 // FCOMPP ; line 1967
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1968
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1969
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,                         // JE SHORT LABELC4 ; line 1970
	0xE9, 0x6D, 0x00, 0x00, 0x00,                               // JMP LABELY ; line 1971
	0xD9, 0x05, 0xF4, 0xB8, 0x85, 0x00,                         // FLD DWORD PTR [0x85B8F4] ; line 1974
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1975
	0xDE, 0xD9,                                                 // FCOMPP ; line 1976
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1977
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1978
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                         // JE SHORT LABELC4 ; line 1979
	0x61,                                                       // POPAD ; line 1981
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x59F282 ; line 1982
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 1985
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 1986
	0x60,                                                       // PUSHAD ; line 1987
	0x8B, 0xB6, 0x98, 0x03, 0x00, 0x00,                         // MOV ESI,[ESI+0x398] ; line 1988
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02,       // CMP WORD PTR [ESI+0x13C],0x020D ; line 1989
	0x0F, 0x8C, 0x21, 0x00, 0x00, 0x00,                         // JL SHORT LABELC5 ; line 1990
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x11, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0211 ; line 1991
	0x0F, 0x8D, 0x12, 0x00, 0x00, 0x00,                         // JNL SHORT LABELC5 ; line 1992
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // MOV WORD PTR [ESI+0x140],0x0003 ; line 1993
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x0A, 0x00,       // MOV WORD PTR [ESI+0x142],0x000A ; line 1994
	0x61,                                                       // POPAD ; line 1996
	0xE9, 0xAC, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABELC6 ; line 1997
	0xD9, 0x05, 0x00, 0xBD, 0x85, 0x00,                         // FLD DWORD PTR [0x85BD00] ; line 2000
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 2001
	0xDE, 0xD9,                                                 // FCOMPP ; line 2002
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 2003
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 2004
	0x0F, 0x84, 0x99, 0xFF, 0xFF, 0xFF,                         // JE LABELC4 ; line 2005
	0xE9, 0x75, 0xFF, 0xFF, 0xFF,                               // JMP LABELZ ; line 2006
};
static JumpTarget flandreA214CHAIN_jumpData[] = {
	{ 0x03C, 0x59F266 },
	{ 0x08B, 0x59F282 },
};
static PatchByteCode flandreA214CHAIN_patchByteCode = { flandreA214CHAIN_byteCode, 251, flandreA214CHAIN_jumpData, 2 };
// flandreA214CHAIN_IDK0 declared in flandre.asm line 2010
static unsigned char flandreA214CHAIN_IDK0_byteCode[] = {
	0x66, 0x81, 0xFA, 0x0D, 0x02,       // CMP DX,0x020D ; line 2011
	0x0F, 0x8C, 0x0A, 0x00, 0x00, 0x00, // JL SHORT LABELB1 ; line 2012
	0x66, 0x81, 0xFA, 0x11, 0x02,       // CMP DX,0x0211 ; line 2013
	0xE9, 0x0A, 0x00, 0x00, 0x00,       // JMP SHORT LABELB2 ; line 2014
	0x66, 0x81, 0xFA, 0x00, 0x00,       // CMP DX,0x0000 ; line 2016
	0xE9, 0x00, 0x00, 0x00, 0x00,       // JMP SHORT LABELB2 ; line 2017
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF, // JL 0x59F35E ; line 2020
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x59F2A5 ; line 2021
};
static JumpTarget flandreA214CHAIN_IDK0_jumpData[] = {
	{ 0x021, 0x59F35E },
	{ 0x026, 0x59F2A5 },
};
static PatchByteCode flandreA214CHAIN_IDK0_patchByteCode = { flandreA214CHAIN_IDK0_byteCode, 42, flandreA214CHAIN_IDK0_jumpData, 2 };
// flandreA214CHAIN_IDK1 declared in flandre.asm line 2023
static unsigned char flandreA214CHAIN_IDK1_byteCode[] = {
	0x66, 0x81, 0xF8, 0x0D, 0x02,       // CMP AX,0x020D ; line 2024
	0x0F, 0x8C, 0x0A, 0x00, 0x00, 0x00, // JL SHORT LABELA1 ; line 2025
	0x66, 0x81, 0xF8, 0x11, 0x02,       // CMP AX,0x0211 ; line 2026
	0xE9, 0x0A, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2027
	0x66, 0x81, 0xF8, 0x00, 0x00,       // CMP AX,0x0000 ; line 2029
	0xE9, 0x00, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2030
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF, // JL 0x59F54A ; line 2033
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x59F56D ; line 2034
};
static JumpTarget flandreA214CHAIN_IDK1_jumpData[] = {
	{ 0x021, 0x59F54A },
	{ 0x026, 0x59F56D },
};
static PatchByteCode flandreA214CHAIN_IDK1_patchByteCode = { flandreA214CHAIN_IDK1_byteCode, 42, flandreA214CHAIN_IDK1_jumpData, 2 };
// flandreA214CHAIN_IDK3 declared in flandre.asm line 2036
static unsigned char flandreA214CHAIN_IDK3_byteCode[] = {
	0x66, 0x81, 0xF8, 0x0D, 0x02,       // CMP AX,0x020D ; line 2037
	0x0F, 0x8C, 0x0A, 0x00, 0x00, 0x00, // JL SHORT LABELA1 ; line 2038
	0x66, 0x81, 0xF8, 0x11, 0x02,       // CMP AX,0x0211 ; line 2039
	0xE9, 0x0A, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2040
	0x66, 0x81, 0xF8, 0x00, 0x00,       // CMP AX,0x0000 ; line 2042
	0xE9, 0x00, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2043
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF, // JL 0x59FB53 ; line 2046
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x59FABD ; line 2047
};
static JumpTarget flandreA214CHAIN_IDK3_jumpData[] = {
	{ 0x021, 0x59FB53 },
	{ 0x026, 0x59FABD },
};
static PatchByteCode flandreA214CHAIN_IDK3_patchByteCode = { flandreA214CHAIN_IDK3_byteCode, 42, flandreA214CHAIN_IDK3_jumpData, 2 };
// flandreA214CHAIN_IDK4 declared in flandre.asm line 2049
static unsigned char flandreA214CHAIN_IDK4_byteCode[] = {
	0x66, 0x81, 0xF8, 0x0D, 0x02,       // CMP AX,0x020D ; line 2050
	0x0F, 0x8C, 0x0A, 0x00, 0x00, 0x00, // JL SHORT LABELA1 ; line 2051
	0x66, 0x81, 0xF8, 0x11, 0x02,       // CMP AX,0x0211 ; line 2052
	0xE9, 0x0A, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2053
	0x66, 0x81, 0xF8, 0x00, 0x00,       // CMP AX,0x0000 ; line 2055
	0xE9, 0x00, 0x00, 0x00, 0x00,       // JMP SHORT LABELA2 ; line 2056
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF, // JL 0x59F8FB ; line 2059
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x59F84D ; line 2060
};
static JumpTarget flandreA214CHAIN_IDK4_jumpData[] = {
	{ 0x021, 0x59F8FB },
	{ 0x026, 0x59F84D },
};
static PatchByteCode flandreA214CHAIN_IDK4_patchByteCode = { flandreA214CHAIN_IDK4_byteCode, 42, flandreA214CHAIN_IDK4_jumpData, 2 };
// flandreA214CHAIN_LEVEL declared in flandre.asm line 2072
static unsigned char flandreA214CHAIN_LEVEL_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 2073
	0x0F, 0x85, 0x31, 0x00, 0x00, 0x00,                   // JNE SHORT LABELC7 ; line 2074
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 2075
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                   // JNE SHORT LABELC7 ; line 2076
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 2077
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                   // JNE SHORT LABELC7 ; line 2078
	0x60,                                                 // PUSHAD ; line 2079
	0x0F, 0xB6, 0x86, 0xA9, 0x06, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x6A9] ; line 2080
	0x6B, 0xC0, 0x02,                                     // IMUL EAX,EAX,0x02 ; line 2081
	0x66, 0x89, 0x86, 0x42, 0x01, 0x00, 0x00,             // MOV [ESI+0x142],AX ; line 2082
	0x61,                                                 // POPAD ; line 2083
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0E, 0x02, // CMP WORD PTR [ESI+0x13C],0x020E ; line 2085
	0x0F, 0x8F, 0x0E, 0x00, 0x00, 0x00,                   // JG SHORT LABELC8 ; line 2086
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 2087
	0xE9, 0x07, 0x00, 0x00, 0x00,                         // JMP SHORT LABELC9 ; line 2088
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 2090
	0x90,                                                 // NOP ; line 2092
};
static PatchByteCode flandreA214CHAIN_LEVEL_patchByteCode = { flandreA214CHAIN_LEVEL_byteCode, 101, nullptr, 0 };
// flandreA214CHAIN_SKIP0 declared in flandre.asm line 2098
static unsigned char flandreA214CHAIN_SKIP0_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 2099
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x59F95D ; line 2100
};
static JumpTarget flandreA214CHAIN_SKIP0_jumpData[] = {
	{ 0x00A, 0x59F95D },
};
static PatchByteCode flandreA214CHAIN_SKIP0_patchByteCode = { flandreA214CHAIN_SKIP0_byteCode, 14, flandreA214CHAIN_SKIP0_jumpData, 1 };
// flandreA214CHAIN_WHAT0 declared in flandre.asm line 2103
static unsigned char flandreA214CHAIN_WHAT0_byteCode[] = {
	0xD8, 0x0D, 0x30, 0xB8, 0x85, 0x00, // FMUL DWORD PTR [0x85B830] ; line 2104
};
static PatchByteCode flandreA214CHAIN_WHAT0_patchByteCode = { flandreA214CHAIN_WHAT0_byteCode, 6, nullptr, 0 };
// flandreA214CHAIN_NODUPE declared in flandre.asm line 2110
static unsigned char flandreA214CHAIN_NODUPE_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 2111
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 2112
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02,       // CMP WORD PTR [EAX+0x13C],0x020D ; line 2113
	0x0F, 0x8C, 0x41, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 2114
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x11, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0211 ; line 2115
	0x0F, 0x8D, 0x32, 0x00, 0x00, 0x00,                         // JNL SHORT LABEL1 ; line 2116
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x13E],0x00 ; line 2117
	0x0F, 0x85, 0x23, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 2118
	0x66, 0x81, 0xB8, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x140],0x00 ; line 2119
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 2120
	0x66, 0x81, 0xB8, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [EAX+0x144],0x01 ; line 2121
	0x0F, 0x8F, 0x05, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 2122
	0xE9, 0x0B, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 2123
	0x61,                                                       // POPAD ; line 2125
	0xBB, 0x02, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000002 ; line 2126
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 2127
	0x81, 0x86, 0x4C, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // ADD DWORD PTR [ESI+0x34C],0xFFFFFFFF ; line 2129
	0xE9, 0xE6, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL1 ; line 2130
	0x90,                                                       // NOP ; line 2132
};
static PatchByteCode flandreA214CHAIN_NODUPE_patchByteCode = { flandreA214CHAIN_NODUPE_byteCode, 114, nullptr, 0 };
// flandreJ6DD declared in flandre.asm line 2136
static unsigned char flandreJ6DD_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 2137
};
static PatchByteCode flandreJ6DD_patchByteCode = { flandreJ6DD_byteCode, 6, nullptr, 0 };
// flandreJ4DD declared in flandre.asm line 2140
static unsigned char flandreJ4DD_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2141
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 2142
};
static PatchByteCode flandreJ4DD_patchByteCode = { flandreJ4DD_byteCode, 8, nullptr, 0 };
// flandreA236TRICK_SWAP0 declared in flandre.asm line 2147
static unsigned char flandreA236TRICK_SWAP0_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487CA0 ; line 2148
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58F1F5 ; line 2149
};
static JumpTarget flandreA236TRICK_SWAP0_jumpData[] = {
	{ 0x001, 0x487CA0 },
	{ 0x006, 0x58F1F5 },
};
static PatchByteCode flandreA236TRICK_SWAP0_patchByteCode = { flandreA236TRICK_SWAP0_byteCode, 10, flandreA236TRICK_SWAP0_jumpData, 2 };
// flandreA236TRICK_SWAP1 declared in flandre.asm line 2151
static unsigned char flandreA236TRICK_SWAP1_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487CA0 ; line 2152
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x58EB0E ; line 2153
};
static JumpTarget flandreA236TRICK_SWAP1_jumpData[] = {
	{ 0x001, 0x487CA0 },
	{ 0x006, 0x58EB0E },
};
static PatchByteCode flandreA236TRICK_SWAP1_patchByteCode = { flandreA236TRICK_SWAP1_byteCode, 10, flandreA236TRICK_SWAP1_jumpData, 2 };
// flandre2SCBOOM declared in flandre.asm line 2158
static unsigned char flandre2SCBOOM_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBE4] ; line 2159
};
static PatchByteCode flandre2SCBOOM_patchByteCode = { flandre2SCBOOM_byteCode, 6, nullptr, 0 };
// flandre4SCBOOM declared in flandre.asm line 2162
static unsigned char flandre4SCBOOM_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 2163
};
static PatchByteCode flandre4SCBOOM_patchByteCode = { flandre4SCBOOM_byteCode, 6, nullptr, 0 };


// flandre.asm
std::array<PatchSkeleton, 206> patchList = {
	PatchSkeleton{ flandreID_FIX_patchByteCode,               (void *)0x587B8B,  7}, // Declared line 18. Patch on CMP [ESI+0x196], BP
	PatchSkeleton{ flandreSLICERSC_SPD_patchByteCode,         (void *)0x595685,  6}, // Declared line 28. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x595674,  6}, // Declared line 30. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ flandreSLICERSC_LOOP_patchByteCode,        (void *)0x595472, 10}, // Declared line 36. Patch on CMP AX, 0x3C
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_patchByteCode,   (void *)0x58F214,  6}, // Declared line 85. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_patchByteCode,   (void *)0x58F4E0,  6}, // Declared line 86. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_patchByteCode,   (void *)0x58F6DB,  6}, // Declared line 87. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_C_patchByteCode, (void *)0x58EB42,  5}, // Declared line 133. Patch on CALL 0x00464980
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_C_patchByteCode, (void *)0x58EE15,  5}, // Declared line 134. Patch on CALL 0x00464980
	PatchSkeleton{ flandreA236SWIPE_PARTICLE_C_patchByteCode, (void *)0x58EFD9,  5}, // Declared line 135. Patch on CALL 0x00464980
	PatchSkeleton{ flandreA236SWIPE_CANCEL_patchByteCode,     (void *)0x58F61D, 17}, // Declared line 140. Patch on MOV DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ flandreA236SWIPE_patchByteCode,            (void *)0x5A0D0A,  6}, // Declared line 188. Patch on JA 0x005A0DDA
	PatchSkeleton{ flandreSTRETCH_X_patchByteCode,            (void *)0x59564E,  6}, // Declared line 193. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59FCAC,  7}, // Declared line 196. Patch on ADD BYTE PTR [ESI+0x194], 1
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59FCB3, 10}, // Declared line 197. Patch on MOV DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ flandre6ANEW_1_patchByteCode,              (void *)0x58A6B0, 10}, // Declared line 216. Patch on FLDZ
	PatchSkeleton{ flandre6ACNEW_1_patchByteCode,             (void *)0x58A79C,  7}, // Declared line 232. Patch on FST DWORD PTR [ESP+0x50C], ST
	PatchSkeleton{ flandre6ANEW_patchByteCode,                (void *)0x5A0D94,  9}, // Declared line 237. Patch on CMP WORD PTR [EAX+0x13C], 0x12F
	PatchSkeleton{ flandreBATFORM_PROJ_patchByteCode,         (void *)0x59E784,  6}, // Declared line 286. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ flandreBATFORM_PROJSTICK_patchByteCode,    (void *)0x59E7E0,  6}, // Declared line 291. Patch on PUSH 0
	PatchSkeleton{ flandreBATFORM_patchByteCode,              (void *)0x58E4DC,  6}, // Declared line 296. Patch on FSTP DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ flandreBATFORMB_patchByteCode,             (void *)0x58E2F5,  7}, // Declared line 557. Patch on MOV ECX, ESI
	PatchSkeleton{ flandreBATFORMC_patchByteCode,             (void *)0x58E974,  7}, // Declared line 562. Patch on MOV ECX, ESI
	PatchSkeleton{ flandreJ2A_REC2_patchByteCode,             (void *)0x58ACC9,  6}, // Declared line 588. Patch on JNE 0x00599FB0
	PatchSkeleton{ flandreJ2A_REC_patchByteCode,              (void *)0x58AC5A,  5}, // Declared line 614. Patch on MOV EDX, [ESI]
	PatchSkeleton{ flandreFLD1_patchByteCode,                 (void *)0x58B5F2,  6}, // Declared line 639. Patch on FLD ST, DWORD PTR [0x85BCA4]
	PatchSkeleton{ flandre5B_NEW0_patchByteCode,              (void *)0x58B57E, 10}, // Declared line 641. Patch on FLD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58B564,  6}, // Declared line 643. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58B540,  7}, // Declared line 645. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT_patchByteCode,             (void *)0x58B797,  5}, // Declared line 648. Patch on ADD EDI, EBX
	PatchSkeleton{ flandre5B_NEW0_CHARGE_patchByteCode,       (void *)0x58B731, 10}, // Declared line 649. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58B717,  6}, // Declared line 650. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58B6F3,  7}, // Declared line 651. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT_patchByteCode,             (void *)0x58B9FD,  5}, // Declared line 654. Patch on ADD EDI, EBX
	PatchSkeleton{ flandre5B_NEW0_patchByteCode,              (void *)0x58B98D, 10}, // Declared line 655. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58B975,  6}, // Declared line 656. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58B951,  7}, // Declared line 657. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT_patchByteCode,             (void *)0x58BBAC,  5}, // Declared line 659. Patch on ADD EDI, EBX
	PatchSkeleton{ flandre5B_NEW0_CHARGE_patchByteCode,       (void *)0x58BB3E, 10}, // Declared line 660. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58BB24,  6}, // Declared line 661. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58BB00,  7}, // Declared line 662. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT_patchByteCode,             (void *)0x58BD6A,  5}, // Declared line 664. Patch on ADD EDI, EBX
	PatchSkeleton{ flandre5B_NEW0_patchByteCode,              (void *)0x58BCFC, 10}, // Declared line 665. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58BCE4,  6}, // Declared line 666. Patch on FSUB ST, QWORD PTR [0x8579B0]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58BCC0,  7}, // Declared line 667. Patch on PUSH 0x1E
	PatchSkeleton{ flandre5B_QUANT1_patchByteCode,            (void *)0x58C055,  6}, // Declared line 669. Patch on ADD EDI, 1
	PatchSkeleton{ flandre5B_NEW0_patchByteCode,              (void *)0x58BFEF, 10}, // Declared line 670. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58BFD4,  6}, // Declared line 671. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58BFB0,  7}, // Declared line 672. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT1_patchByteCode,            (void *)0x58C225,  6}, // Declared line 674. Patch on ADD EDI, 1
	PatchSkeleton{ flandre5B_NEW0_CHARGE_patchByteCode,       (void *)0x58C1BF, 10}, // Declared line 675. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58C1A4,  6}, // Declared line 676. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58C180,  7}, // Declared line 677. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT1_patchByteCode,            (void *)0x58C535,  6}, // Declared line 679. Patch on ADD EDI, 1
	PatchSkeleton{ flandre5B_NEW0_patchByteCode,              (void *)0x58C4CF, 10}, // Declared line 680. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58C4B4,  6}, // Declared line 681. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58C490,  7}, // Declared line 682. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_QUANT1_patchByteCode,            (void *)0x58C705,  6}, // Declared line 684. Patch on ADD EDI, 1
	PatchSkeleton{ flandre5B_NEW0_CHARGE_patchByteCode,       (void *)0x58C69F, 10}, // Declared line 685. Patch on FILD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x58C684,  6}, // Declared line 686. Patch on FSUB ST, QWORD PTR [0x871520]
	PatchSkeleton{ flandre5B_NEW1_patchByteCode,              (void *)0x58C660,  7}, // Declared line 687. Patch on PUSH 0x14
	PatchSkeleton{ flandre5B_NEWSYS_patchByteCode,            (void *)0x59DFE4,  7}, // Declared line 747. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59E002,  6}, // Declared line 748. Patch on JNP 0x0059E2D8
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59E01D,  6}, // Declared line 749. Patch on JE 0x0059E2D6
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59E034,  6}, // Declared line 750. Patch on JNP 0x0059E2D6
	PatchSkeleton{ flandre5B_ANIM_patchByteCode,              (void *)0x59E07F,  6}, // Declared line 755. Patch on JE 0x0059E1F2
	PatchSkeleton{ flandreC5C_NEW_GRAZERES_patchByteCode,     (void *)0x59E520,  9}, // Declared line 762. Patch on PUSH 4
	PatchSkeleton{ flandreC5C_NEW_DENSITY_patchByteCode,      (void *)0x59E529,  8}, // Declared line 769. Patch on PUSH EBP
	PatchSkeleton{ flandreC5C_ONCE_patchByteCode,             (void *)0x58CAC8,  7}, // Declared line 791. Patch on CMP [ESI+0x140], BX
	PatchSkeleton{ flandreC5C_ONCE_patchByteCode,             (void *)0x58CB6F,  8}, // Declared line 792. Patch on CMP WORD PTR [ESI+0x140], 7
	PatchSkeleton{ flandreC5C_ONCE_patchByteCode,             (void *)0x58D0C1,  7}, // Declared line 794. Patch on CMP [ESI+0x140], BX
	PatchSkeleton{ flandreC5C_ONCE_patchByteCode,             (void *)0x58D168,  8}, // Declared line 795. Patch on CMP WORD PTR [ESI+0x140], 7
	PatchSkeleton{ flandre5C_HITCOUNT_patchByteCode,          (void *)0x59E2E7,  7}, // Declared line 797. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ flandreJ2C_ONEBULLET_patchByteCode,        (void *)0x58D976,  6}, // Declared line 809. Patch on FLD ST, DWORD PTR [0x857DD8]
	PatchSkeleton{ flandre3A_REC_patchByteCode,               (void *)0x58A96B,  6}, // Declared line 824. Patch on PUSH 2
	PatchSkeleton{ flandre3A_SPD_patchByteCode,               (void *)0x58A98C,  6}, // Declared line 825. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ flandre3A_DESPD_patchByteCode,             (void *)0x58A938,  6}, // Declared line 826. Patch on FSUB ST, QWORD PTR [0x85B818]
	PatchSkeleton{ flandreJ6A_0_patchByteCode,                (void *)0x59AC35,  5}, // Declared line 837. Patch on CALL 0x004636B0
	PatchSkeleton{ flandreJ6A_1_patchByteCode,                (void *)0x58AA7C, 10}, // Declared line 844. Patch on CMP EAX, 7
	PatchSkeleton{ flandreJ6ABOUNCE_X_patchByteCode,          (void *)0x58AAB3,  6}, // Declared line 850. Patch on FLD ST, DWORD PTR [0x85B908]
	PatchSkeleton{ flandre0_patchByteCode,                    (void *)0x58AACC,  7}, // Declared line 864. Patch on MOV ECX, ESI
	PatchSkeleton{ flandre1A_patchByteCode,                   (void *)0x58AB98,  6}, // Declared line 874. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ flandre1B_patchByteCode,                   (void *)0x58A6F2,  7}, // Declared line 905. Patch on CMP [ESI+0x142], BP
	PatchSkeleton{ flandre2_patchByteCode,                    (void *)0x5A0D2D, 12}, // Declared line 912. Patch on CMP CX, 0x12E
	PatchSkeleton{ flandre3_patchByteCode,                    (void *)0x58AB8E, 10}, // Declared line 936. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ flandre4_patchByteCode,                    (void *)0x58AA35,  8}, // Declared line 941. Patch on CMP WORD PTR [ESI+0x140], 6
	PatchSkeleton{ flandreA22CEILING_BOUNCEX_patchByteCode,   (void *)0x592E79,  6}, // Declared line 1022. Patch on FLD ST, DWORD PTR [0x85B9E8]
	PatchSkeleton{ flandreA22CEILING_BOUNCEX1_patchByteCode,  (void *)0x5930E0,  6}, // Declared line 1023. Patch on FADD ST, QWORD PTR [0x85A280]
	PatchSkeleton{ flandreA22CEILING_BOUNCEX2B_patchByteCode, (void *)0x5930F8,  5}, // Declared line 1024. Patch on TEST AH, 0x41
	PatchSkeleton{ flandreA22CEILING_BOUNCEX_patchByteCode,   (void *)0x593330,  6}, // Declared line 1025. Patch on FLD ST, DWORD PTR [0x85B9E8]
	PatchSkeleton{ flandreA22CEILING_BOUNCEX1_patchByteCode,  (void *)0x593599,  6}, // Declared line 1026. Patch on FADD ST, QWORD PTR [0x85A280]
	PatchSkeleton{ flandreA22CEILING_BOUNCEX2C_patchByteCode, (void *)0x5935B1,  5}, // Declared line 1027. Patch on TEST AH, 0x41
	PatchSkeleton{ flandreA22CEILING_patchByteCode,           (void *)0x593737,  6}, // Declared line 1029. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ flandreA22CEILING_XB_patchByteCode,        (void *)0x5931A7,  6}, // Declared line 1031. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ flandreA22CEILING_XC_patchByteCode,        (void *)0x593660,  6}, // Declared line 1032. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ flandreA22CEILING_SKIP_patchByteCode,      (void *)0x592D9F,  8}, // Declared line 1034. Patch on CMP WORD PTR [ESI+0x13E], 6
	PatchSkeleton{ flandreA22CEILING_SKIP_patchByteCode,      (void *)0x593256,  8}, // Declared line 1035. Patch on CMP WORD PTR [ESI+0x13E], 6
	PatchSkeleton{ flandreROCKETKICK_patchByteCode,           (void *)0x59197D,  6}, // Declared line 1061. Patch on FLD ST, DWORD PTR [0x85BC00]
	PatchSkeleton{ flandreROCKETKICK_B_patchByteCode,         (void *)0x591990,  6}, // Declared line 1063. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ flandreROCKETKICK_patchByteCode,           (void *)0x591AF4,  6}, // Declared line 1065. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x591B07,  6}, // Declared line 1067. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ flandreROCKETKICK_STARTUPB_patchByteCode,  (void *)0x59191D,  5}, // Declared line 1069. Patch on CALL 0x00487CA0
	PatchSkeleton{ flandreROCKETKICK_END_patchByteCode,       (void *)0x59FEB5,  7}, // Declared line 1099. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ flandreROCKETKICK_END1_patchByteCode,      (void *)0x5A03BC,  6}, // Declared line 1139. Patch on JNP 0x0059E2D8
	PatchSkeleton{ flandreD623_INVB_patchByteCode,            (void *)0x5915E1,  7}, // Declared line 1153. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ flandreD623_INVLV_patchByteCode,           (void *)0x59AFE6,  7}, // Declared line 1154. Patch on CMP BYTE PTR [ESI+0x6A6], 1
	PatchSkeleton{ flandreD623_INVC_patchByteCode,            (void *)0x591746,  7}, // Declared line 1155. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ flandreA22STIG0_patchByteCode,             (void *)0x593809,  6}, // Declared line 1167. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ flandreA22STIG1_patchByteCode,             (void *)0x593A26,  6}, // Declared line 1168. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ flandreA22STIG_SPD_patchByteCode,          (void *)0x5A0AC6,  6}, // Declared line 1174. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ flandreA22STIG_ROT_patchByteCode,          (void *)0x5A0B49,  6}, // Declared line 1179. Patch on FADD ST, QWORD PTR [0x8579B0]
	PatchSkeleton{ flandreBLS_FREEZE_patchByteCode,           (void *)0x5A18F6,  7}, // Declared line 1188. Patch on MOVSX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x594D1B,  6}, // Declared line 1191. Patch on FLD ST, DWORD PTR [0x85B9E4]
	PatchSkeleton{ flandreBLS_HEIGHT_patchByteCode,           (void *)0x594E4E,  6}, // Declared line 1227. Patch on FSTP DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ flandreBLS_FLASH_patchByteCode,            (void *)0x594CA5,  5}, // Declared line 1240. Patch on MOV EBP, 3
	PatchSkeleton{ flandreBLS_FLASH1_patchByteCode,           (void *)0x594CF0,  7}, // Declared line 1241. Patch on CMP DWORD PTR [ESI+0x144], 0
	PatchSkeleton{ flandreBLS_RISE_patchByteCode,             (void *)0x594D30,  6}, // Declared line 1262. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ flandreBLS_RISEDEC_patchByteCode,          (void *)0x594D3E,  6}, // Declared line 1264. Patch on FLD ST, DWORD PTR [0x85AAE8]
	PatchSkeleton{ flandreBLS_PARTICLE_patchByteCode,         (void *)0x594D7D,  7}, // Declared line 1302. Patch on CMP [ESI+0x13E], DI
	PatchSkeleton{ flandreBLS_SKIPMOMENTUM_patchByteCode,     (void *)0x594D8A,  6}, // Declared line 1307. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ flandreBLS_UNSTICK_patchByteCode,          (void *)0x594E69,  6}, // Declared line 1312. Patch on JE SHORT 0x00594E8E
	PatchSkeleton{ flandreBLS_EFF_patchByteCode,              (void *)0x5A1965,  6}, // Declared line 1317. Patch on JNE 0x0059E2D8
	PatchSkeleton{ flandreD22S_EFF2_patchByteCode,            (void *)0x5A196F,  5}, // Declared line 1319. Patch on CALL 0x0048CF60
	PatchSkeleton{ flandreD22_B_RISE_patchByteCode,           (void *)0x592619,  6}, // Declared line 1375. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ flandreD22_RISEDEC_patchByteCode,          (void *)0x592625,  6}, // Declared line 1381. Patch on FLD ST, DWORD PTR [0x85AAE8]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x5926ED,  6}, // Declared line 1383. Patch on FLD ST, DWORD PTR [0x85C500]
	PatchSkeleton{ flandreD22_C_RISE_patchByteCode,           (void *)0x59299C,  6}, // Declared line 1386. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ flandreD22_RISEDEC_patchByteCode,          (void *)0x5929A8,  6}, // Declared line 1388. Patch on FLD ST, DWORD PTR [0x85AAE8]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x592A70,  6}, // Declared line 1390. Patch on FLD ST, DWORD PTR [0x85C500]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x59B096,  5}, // Declared line 1393. Patch on CALL 0x004636B0
	PatchSkeleton{ flandreD22S_PARTICLE_patchByteCode,        (void *)0x592664,  7}, // Declared line 1399. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ flandreD22S_PARTICLE_patchByteCode,        (void *)0x5929E7,  7}, // Declared line 1400. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ flandreD22S_RESPIN_patchByteCode,          (void *)0x5925B7,  6}, // Declared line 1526. Patch on CMP [ESI+0x144], EBP
	PatchSkeleton{ flandreD22S_RESPIN_patchByteCode,          (void *)0x59293A,  6}, // Declared line 1527. Patch on CMP [ESI+0x144], EBP
	PatchSkeleton{ flandreD22S_B_SKIPMOMENTUM_patchByteCode,  (void *)0x592671,  6}, // Declared line 1535. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ flandreD22S_C_SKIPMOMENTUM_patchByteCode,  (void *)0x5929F4,  6}, // Declared line 1536. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ flandreD22S_B_XFIX_patchByteCode,          (void *)0x592603, 10}, // Declared line 1547. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ flandreD22S_C_XFIX_patchByteCode,          (void *)0x592986, 10}, // Declared line 1548. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ flandreD22S_B_UNSTICK_patchByteCode,       (void *)0x5927F2,  6}, // Declared line 1556. Patch on JE SHORT 0x00592817
	PatchSkeleton{ flandreD22S_C_UNSTICK_patchByteCode,       (void *)0x592B75, 10}, // Declared line 1557. Patch on JE SHORT 0x00592BA8
	PatchSkeleton{ flandreD22S_DRAWORDER_patchByteCode,       (void *)0x592781,  8}, // Declared line 1563. Patch on PUSH 0xFFFFFFFF
	PatchSkeleton{ flandreD22S_DRAWORDER_patchByteCode,       (void *)0x592B04,  8}, // Declared line 1564. Patch on PUSH 0xFFFFFFFF
	PatchSkeleton{ flandreD22S_EFF_patchByteCode,             (void *)0x5A0409,  6}, // Declared line 1569. Patch on JNE 0x0059E2D8
	PatchSkeleton{ flandreD22S_EFF2_patchByteCode,            (void *)0x5A0415,  5}, // Declared line 1591. Patch on CALL 0x0048CF60
	PatchSkeleton{ flandreD22S_NOBOUNCE_patchByteCode,        (void *)0x592899,  8}, // Declared line 1600. Patch on JNE SHORT 0x005928DF
	PatchSkeleton{ flandreD22S_NOBOUNCE_patchByteCode,        (void *)0x592C2B,  8}, // Declared line 1601. Patch on JNE SHORT 0x00592C6D
	PatchSkeleton{ flandreD22S_LV_patchByteCode,              (void *)0x592577,  5}, // Declared line 1616. Patch on MOV EBX, 5
	PatchSkeleton{ flandreD22S_LV_patchByteCode,              (void *)0x5928FA,  5}, // Declared line 1617. Patch on MOV EBX, 5
	PatchSkeleton{ flandreA214FORK_LV1_patchByteCode,         (void *)0x590E16,  7}, // Declared line 1626. Patch on MOVSX EDX, BYTE PTR [ESI+0x6AD]
	PatchSkeleton{ flandreA214FORK_LV1_patchByteCode,         (void *)0x5912C0,  7}, // Declared line 1627. Patch on MOVSX EDX, BYTE PTR [ESI+0x6AD]
	PatchSkeleton{ flandreA214FORK_LV2_patchByteCode,         (void *)0x590F0A,  7}, // Declared line 1636. Patch on MOVSX EAX, BYTE PTR [ESI+0x6AD]
	PatchSkeleton{ flandreA214FORK_LV2_patchByteCode,         (void *)0x5913BA,  7}, // Declared line 1637. Patch on MOVSX EAX, BYTE PTR [ESI+0x6AD]
	PatchSkeleton{ flandreA214FORK_SOUND_patchByteCode,       (void *)0x590C80,  7}, // Declared line 1654. Patch on MOV BYTE PTR [ESI+0x18C], 9
	PatchSkeleton{ flandreA214FORK_SOUND_patchByteCode,       (void *)0x591112,  7}, // Declared line 1655. Patch on MOV BYTE PTR [ESI+0x18C], 9
	PatchSkeleton{ flandreA214FORK_POSX_patchByteCode,        (void *)0x590D94,  6}, // Declared line 1693. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ flandreA214FORK_POSX_patchByteCode,        (void *)0x590F90,  6}, // Declared line 1694. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ flandreA214FORK_POSX_patchByteCode,        (void *)0x59123E,  6}, // Declared line 1695. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ flandreA214FORK_POSX_patchByteCode,        (void *)0x591440,  6}, // Declared line 1696. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ flandreA214FORK_POSY_patchByteCode,        (void *)0x590DB9,  6}, // Declared line 1698. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ flandreA214FORK_POSY_patchByteCode,        (void *)0x590FB5,  6}, // Declared line 1699. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ flandreA214FORK_POSY_patchByteCode,        (void *)0x591263,  6}, // Declared line 1700. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ flandreA214FORK_POSY_patchByteCode,        (void *)0x591465,  6}, // Declared line 1701. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ flandreA214FORK_patchByteCode,             (void *)0x590EE5,  6}, // Declared line 1711. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ flandreA214FORK_patchByteCode,             (void *)0x5910BF,  6}, // Declared line 1713. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ flandreA214FORK_patchByteCode,             (void *)0x591395,  6}, // Declared line 1715. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ flandreA214FORK_patchByteCode,             (void *)0x591575,  6}, // Declared line 1717. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ flandreA214FORK_0_patchByteCode,           (void *)0x59FC1E,  6}, // Declared line 1724. Patch on JE 0x0059E2D8
	PatchSkeleton{ flandre3SCCROSS_HEIGHT_patchByteCode,      (void *)0x593C96,  6}, // Declared line 1739. Patch on FADD ST, QWORD PTR [0x85B8F8]
	PatchSkeleton{ flandre5SCCROSS_HEIGHT_patchByteCode,      (void *)0x59440D,  6}, // Declared line 1741. Patch on FADD ST, QWORD PTR [0x8581D0]
	PatchSkeleton{ flandre5SCCRADLE_DUR_patchByteCode,        (void *)0x594976, 11}, // Declared line 1750. Patch on MOVZX EAX, WORD PTR [ESI+0x7D0]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x594BFF,  6}, // Declared line 1752. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ flandre5SCCRADLE_0_patchByteCode,          (void *)0x594C0F,  6}, // Declared line 1758. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ flandre5SCCRADLE_1_patchByteCode,          (void *)0x59499E,  6}, // Declared line 1763. Patch on FLD ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ flandreFLDZ_patchByteCode,                 (void *)0x594A66,  6}, // Declared line 1765. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ flandreNOP_patchByteCode,                  (void *)0x594B17,  6}, // Declared line 1767. Patch on MOV [ESI+0x190], EBP
	PatchSkeleton{ flandre2SC5SC_patchByteCode,               (void *)0x594985,  6}, // Declared line 1781. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ flandreD236_UP_patchByteCode,              (void *)0x58DD54,  6}, // Declared line 1786. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ flandreD236_UP_patchByteCode,              (void *)0x58E048,  6}, // Declared line 1787. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ flandreD236_GRAV_patchByteCode,            (void *)0x58DD62,  6}, // Declared line 1791. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ flandreD236_GRAV_patchByteCode,            (void *)0x58E056,  6}, // Declared line 1792. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ flandreD236_CANCEL_patchByteCode,          (void *)0x58DB6D,  7}, // Declared line 1807. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ flandreD236_patchByteCode,                 (void *)0x58DD41,  6}, // Declared line 1841. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ flandreD236C_patchByteCode,                (void *)0x58DE4C,  7}, // Declared line 1846. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ flandreJ8A_GRAVITY_patchByteCode,          (void *)0x58AE3B,  6}, // Declared line 1852. Patch on FLD ST, DWORD PTR [0x85B954]
	PatchSkeleton{ flandreA214CHAIN_ANG_patchByteCode,        (void *)0x59F3E8,  6}, // Declared line 1862. Patch on FMUL ST, QWORD PTR [0x8581B8]
	PatchSkeleton{ flandreA214CHAIN_ANGSTART1_patchByteCode,  (void *)0x5905CF,  6}, // Declared line 1886. Patch on FCOMP ST, QWORD PTR [0x85C450]
	PatchSkeleton{ flandreA214CHAIN_ANGSTART2_patchByteCode,  (void *)0x590B2C,  6}, // Declared line 1887. Patch on FCOMP ST, QWORD PTR [0x85C450]
	PatchSkeleton{ flandreA214CHAIN0_patchByteCode,           (void *)0x59037A,  7}, // Declared line 1936. Patch on MOV ECX, ESI
	PatchSkeleton{ flandreA214CHAIN1_patchByteCode,           (void *)0x590758,  5}, // Declared line 1937. Patch on MOV EDI, 4
	PatchSkeleton{ flandreA214CHAIN_IDK0_patchByteCode,       (void *)0x59F28F,  5}, // Declared line 2062. Patch on CMP DX, 0x20E
	PatchSkeleton{ flandreA214CHAIN_IDK1_patchByteCode,       (void *)0x59F53E,  6}, // Declared line 2063. Patch on CMP AX, 0x20E
	PatchSkeleton{ flandreA214CHAIN_IDK4_patchByteCode,       (void *)0x59F839, 10}, // Declared line 2064. Patch on CMP AX, 0x20E
	PatchSkeleton{ flandreA214CHAIN_IDK3_patchByteCode,       (void *)0x59FAA9, 10}, // Declared line 2065. Patch on CMP AX, 0x20E
	PatchSkeleton{ flandreA214CHAIN_patchByteCode,            (void *)0x59F258,  6}, // Declared line 2069. Patch on MOV EAX, [ESI+0x190]
	PatchSkeleton{ flandreA214CHAIN_LEVEL_patchByteCode,      (void *)0x5904E7,  8}, // Declared line 2094. Patch on CMP WORD PTR [ESI+0x13E], 3
	PatchSkeleton{ flandreA214CHAIN_LEVEL_patchByteCode,      (void *)0x590985,  7}, // Declared line 2095. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ flandreA214CHAIN_SKIP0_patchByteCode,      (void *)0x59F953,  8}, // Declared line 2102. Patch on CMP WORD PTR [ESI+0x140], 0
	PatchSkeleton{ flandreA214CHAIN_WHAT0_patchByteCode,      (void *)0x59F970,  6}, // Declared line 2106. Patch on FMUL ST, QWORD PTR [0x8579B0]
	PatchSkeleton{ flandreA214CHAIN_WHAT0_patchByteCode,      (void *)0x59F99C,  6}, // Declared line 2107. Patch on FMUL ST, QWORD PTR [0x8579B0]
	PatchSkeleton{ flandreA214CHAIN_NODUPE_patchByteCode,     (void *)0x59F22E,  5}, // Declared line 2134. Patch on MOV EBX, 2
	PatchSkeleton{ flandreJ6DD_patchByteCode,                 (void *)0x58A28B,  6}, // Declared line 2139. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ flandreJ4DD_patchByteCode,                 (void *)0x58A0DC,  6}, // Declared line 2144. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ flandreA236TRICK_SWAP0_patchByteCode,      (void *)0x58EB09,  5}, // Declared line 2155. Patch on CALL 0x00487CA0
	PatchSkeleton{ flandreA236TRICK_SWAP1_patchByteCode,      (void *)0x58F1F0,  5}, // Declared line 2156. Patch on CALL 0x00487CA0
	PatchSkeleton{ flandre2SCBOOM_patchByteCode,              (void *)0x593F0E,  6}, // Declared line 2161. Patch on FLD ST, DWORD PTR [0x871540]
	PatchSkeleton{ flandre4SCBOOM_patchByteCode,              (void *)0x5947F2,  6}, // Declared line 2165. Patch on FLD ST, DWORD PTR [0x871544]
};


static std::array<unsigned, 168> skeletonsForUpdate = {
	   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  12,  15,  16,  20,  21,
	  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,
	  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,
	  54,  55,  56,  57,  58,  59,  60,  68,  69,  70,  71,  73,  74,  75,  76,  78,
	  79,  80,  81,  82,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	  96,  97,  98,  99, 100, 101, 104, 106, 107, 108, 112, 113, 114, 115, 116, 117,
	 118, 119, 120, 123, 124, 125, 126, 127, 128, 130, 131, 132, 133, 134, 135, 136,
	 137, 138, 139, 140, 141, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
	 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 167, 168, 169, 170, 171,
	 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 185, 186, 187, 188,
	 194, 195, 200, 201, 202, 203, 204, 205,
};
static std::array<unsigned, 3> skeletonsForInitializeAction = { 77, 105, 129 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 35> skeletonsForHandleInputs = {
	  11,  13,  14,  17,  18,  19,  61,  62,  63,  64,  65,  66,  67,  72,  83, 102,
	 103, 109, 110, 111, 121, 122, 142, 143, 166, 184, 189, 190, 191, 192, 193, 196,
	 197, 198, 199,
};
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 35> skeletonsForObjectUpdate = {
	  11,  13,  14,  17,  18,  19,  61,  62,  63,  64,  65,  66,  67,  72,  83, 102,
	 103, 109, 110, 111, 121, 122, 142, 143, 166, 184, 189, 190, 191, 192, 193, 196,
	 197, 198, 199,
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
	DWORD old;

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
	printf("Internal memory used %zu/12230\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 12230);

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
