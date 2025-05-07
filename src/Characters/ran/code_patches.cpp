#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<20616> applyMemory;
StackedMemory<10453> internalMemory;
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


// ranFLDZ declared in ran.asm line 1
static unsigned char ranFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode ranFLDZ_patchByteCode = { ranFLDZ_byteCode, 2, nullptr, 0 };
// ranNOP declared in ran.asm line 5
static unsigned char ranNOP_byteCode[] = {
	0x90, // NOP ; line 6
};
static PatchByteCode ranNOP_patchByteCode = { ranNOP_byteCode, 1, nullptr, 0 };
// ranID_FIX declared in ran.asm line 11
static unsigned char ranID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000022 ; line 12
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x0000 ; line 14
};
static PatchByteCode ranID_FIX_patchByteCode = { ranID_FIX_byteCode, 19, nullptr, 0 };
// ran66B_HITCOUNT declared in ran.asm line 18
static unsigned char ran66B_HITCOUNT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 19
};
static PatchByteCode ran66B_HITCOUNT_patchByteCode = { ran66B_HITCOUNT_byteCode, 7, nullptr, 0 };
// ranD22_DIVE declared in ran.asm line 24
static unsigned char ranD22_DIVE_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 25
};
static PatchByteCode ranD22_DIVE_patchByteCode = { ranD22_DIVE_byteCode, 2, nullptr, 0 };
// ranD22_DIVE2 declared in ran.asm line 27
static unsigned char ranD22_DIVE2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x30, 0x02, // CMP WORD PTR [ESI+0x13C],0x0230 ; line 28
	0x0F, 0x84, 0x2D, 0x00, 0x00, 0x00,                   // JE LABELB ; line 29
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x31, 0x02, // CMP WORD PTR [ESI+0x13C],0x0231 ; line 30
	0x0F, 0x84, 0x49, 0x00, 0x00, 0x00,                   // JE LABELC ; line 31
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x32, 0x02, // CMP WORD PTR [ESI+0x13C],0x0232 ; line 32
	0x0F, 0x84, 0x5F, 0x00, 0x00, 0x00,                   // JE LABELAIRB ; line 33
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x33, 0x02, // CMP WORD PTR [ESI+0x13C],0x0233 ; line 34
	0x0F, 0x84, 0x7B, 0x00, 0x00, 0x00,                   // JE LABELAIRC ; line 35
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                   // FLD DWORD PTR [0x85B3B0] ; line 38
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00,                   // FADD DWORD PTR [0x85B3B0] ; line 39
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 40
	0xD9, 0xEE,                                           // FLDZ ; line 41
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 42
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 43
	0xD9, 0x05, 0x04, 0xAD, 0x85, 0x00,                   // FLD DWORD PTR [0x85AD04] ; line 44
	0xE9, 0x6C, 0x00, 0x00, 0x00,                         // JMP END ; line 45
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00,                   // FLD DWORD PTR [0x867860] ; line 48
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 49
	0xD9, 0xEE,                                           // FLDZ ; line 50
	0xD8, 0x25, 0x80, 0xC8, 0x85, 0x00,                   // FSUB DWORD PTR [0x85C880] ; line 51
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 52
	0xD9, 0x05, 0x04, 0xAD, 0x85, 0x00,                   // FLD DWORD PTR [0x85AD04] ; line 53
	0xE9, 0x47, 0x00, 0x00, 0x00,                         // JMP END ; line 54
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                   // FLD DWORD PTR [0x85B3B0] ; line 57
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 58
	0xD9, 0xEE,                                           // FLDZ ; line 59
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 60
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 61
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 62
	0xD9, 0x05, 0x04, 0xAD, 0x85, 0x00,                   // FLD DWORD PTR [0x85AD04] ; line 63
	0xE9, 0x1C, 0x00, 0x00, 0x00,                         // JMP END ; line 64
	0xD9, 0xEE,                                           // FLDZ ; line 67
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 68
	0xD9, 0xEE,                                           // FLDZ ; line 69
	0xD8, 0x25, 0x80, 0xC8, 0x85, 0x00,                   // FSUB DWORD PTR [0x85C880] ; line 70
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 71
	0xD9, 0x05, 0x04, 0xAD, 0x85, 0x00,                   // FLD DWORD PTR [0x85AD04] ; line 72
};
static PatchByteCode ranD22_DIVE2_patchByteCode = { ranD22_DIVE2_byteCode, 211, nullptr, 0 };
// ranD22_SUBSEQ2B declared in ran.asm line 77
static unsigned char ranD22_SUBSEQ2B_byteCode[] = {
	0x66, 0x81, 0xF8, 0x02, 0x00,                               // CMP AX,0x0002 ; line 78
	0x0F, 0x85, 0x3B, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 79
	0xD9, 0xE8,                                                 // FLD1 ; line 81
	0xD8, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FCOMP DWORD PTR [ESI+0xF0] ; line 82
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 83
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 84
	0x0F, 0x8B, 0x28, 0x00, 0x00, 0x00,                         // JNP LABEL3 ; line 85
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 87
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 88
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 90
	0x8B, 0x42, 0x14,                                           // MOV EAX,[EDX+0x14] ; line 91
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 92
	0xFF, 0xD0,                                                 // CALL EAX ; line 93
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 97
	0x6A, 0x12,                                                 // PUSH 0x12 ; line 98
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 99
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4F58AD ; line 101
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x0003 ; line 104
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x4F5860 ; line 105
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 106
};
static JumpTarget ranD22_SUBSEQ2B_jumpData[] = {
	{ 0x03D, 0x464980 },
	{ 0x042, 0x4F58AD },
	{ 0x04D, 0x4F5860 },
};
static PatchByteCode ranD22_SUBSEQ2B_patchByteCode = { ranD22_SUBSEQ2B_byteCode, 87, ranD22_SUBSEQ2B_jumpData, 3 };
// ranD22_SUBSEQ2C declared in ran.asm line 109
static unsigned char ranD22_SUBSEQ2C_byteCode[] = {
	0x66, 0x81, 0xF8, 0x02, 0x00,                               // CMP AX,0x0002 ; line 110
	0x0F, 0x85, 0x3B, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 111
	0xD9, 0xE8,                                                 // FLD1 ; line 113
	0xD8, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FCOMP DWORD PTR [ESI+0xF0] ; line 114
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 115
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 116
	0x0F, 0x8B, 0x28, 0x00, 0x00, 0x00,                         // JNP LABEL3 ; line 117
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 119
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 120
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 122
	0x8B, 0x42, 0x14,                                           // MOV EAX,[EDX+0x14] ; line 123
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 124
	0xFF, 0xD0,                                                 // CALL EAX ; line 125
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 129
	0x6A, 0x12,                                                 // PUSH 0x12 ; line 130
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 131
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4F5C92 ; line 133
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x0003 ; line 136
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x4F5C45 ; line 137
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 138
};
static JumpTarget ranD22_SUBSEQ2C_jumpData[] = {
	{ 0x03D, 0x464980 },
	{ 0x042, 0x4F5C92 },
	{ 0x04D, 0x4F5C45 },
};
static PatchByteCode ranD22_SUBSEQ2C_patchByteCode = { ranD22_SUBSEQ2C_byteCode, 87, ranD22_SUBSEQ2C_jumpData, 3 };
// ranD22_SUBSEQ2AIRB declared in ran.asm line 142
static unsigned char ranD22_SUBSEQ2AIRB_byteCode[] = {
	0x66, 0x81, 0xF8, 0x02, 0x00,                               // CMP AX,0x0002 ; line 143
	0x0F, 0x85, 0x3B, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 144
	0xD9, 0xE8,                                                 // FLD1 ; line 146
	0xD8, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FCOMP DWORD PTR [ESI+0xF0] ; line 147
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 148
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 149
	0x0F, 0x8B, 0x28, 0x00, 0x00, 0x00,                         // JNP LABEL3 ; line 150
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 152
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 153
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 155
	0x8B, 0x42, 0x14,                                           // MOV EAX,[EDX+0x14] ; line 156
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 157
	0xFF, 0xD0,                                                 // CALL EAX ; line 158
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 162
	0x6A, 0x12,                                                 // PUSH 0x12 ; line 163
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 164
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4F6063 ; line 166
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x0003 ; line 169
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x4F6016 ; line 170
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 171
};
static JumpTarget ranD22_SUBSEQ2AIRB_jumpData[] = {
	{ 0x03D, 0x464980 },
	{ 0x042, 0x4F6063 },
	{ 0x04D, 0x4F6016 },
};
static PatchByteCode ranD22_SUBSEQ2AIRB_patchByteCode = { ranD22_SUBSEQ2AIRB_byteCode, 87, ranD22_SUBSEQ2AIRB_jumpData, 3 };
// ranD22_SUBSEQ2AIRC declared in ran.asm line 174
static unsigned char ranD22_SUBSEQ2AIRC_byteCode[] = {
	0x66, 0x81, 0xF8, 0x02, 0x00,                               // CMP AX,0x0002 ; line 175
	0x0F, 0x85, 0x3B, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 176
	0xD9, 0xE8,                                                 // FLD1 ; line 178
	0xD8, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FCOMP DWORD PTR [ESI+0xF0] ; line 179
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 180
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 181
	0x0F, 0x8B, 0x28, 0x00, 0x00, 0x00,                         // JNP LABEL3 ; line 182
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 184
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 185
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 187
	0x8B, 0x42, 0x14,                                           // MOV EAX,[EDX+0x14] ; line 188
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 189
	0xFF, 0xD0,                                                 // CALL EAX ; line 190
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 194
	0x6A, 0x12,                                                 // PUSH 0x12 ; line 195
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 196
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4F6492 ; line 198
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x0003 ; line 201
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x4F6445 ; line 202
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 203
};
static JumpTarget ranD22_SUBSEQ2AIRC_jumpData[] = {
	{ 0x03D, 0x464980 },
	{ 0x042, 0x4F6492 },
	{ 0x04D, 0x4F6445 },
};
static PatchByteCode ranD22_SUBSEQ2AIRC_patchByteCode = { ranD22_SUBSEQ2AIRC_byteCode, 87, ranD22_SUBSEQ2AIRC_jumpData, 3 };
// ranD22_SPINB declared in ran.asm line 230
static unsigned char ranD22_SPINB_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 231
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 232
	0xD9, 0xEE,                                                 // FLDZ ; line 233
	0xD8, 0x25, 0x60, 0x78, 0x86, 0x00,                         // FSUB DWORD PTR [0x867860] ; line 234
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 235
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x867810] ; line 236
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 237
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x001A ; line 238
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                         // JL 0x4F5837 ; line 239
};
static JumpTarget ranD22_SPINB_jumpData[] = {
	{ 0x037, 0x4F5837 },
};
static PatchByteCode ranD22_SPINB_patchByteCode = { ranD22_SPINB_byteCode, 59, ranD22_SPINB_jumpData, 1 };
// ranD22_SPINAIRB declared in ran.asm line 242
static unsigned char ranD22_SPINAIRB_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 243
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 244
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x6A7],0x00 ; line 246
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,                         // JE LABEL0 ; line 247
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x6A7],0x01 ; line 248
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 249
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x02,                   // CMP BYTE PTR [ESI+0x6A7],0x02 ; line 250
	0x0F, 0x84, 0x30, 0x00, 0x00, 0x00,                         // JE LABEL2 ; line 251
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x03,                   // CMP BYTE PTR [ESI+0x6A7],0x03 ; line 252
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 253
	0xE9, 0x3C, 0x00, 0x00, 0x00,                               // JMP LABEL4 ; line 254
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000000D ; line 257
	0xE9, 0x3C, 0x00, 0x00, 0x00,                               // JMP END ; line 258
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000000A ; line 261
	0xE9, 0x2D, 0x00, 0x00, 0x00,                               // JMP END ; line 262
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000007 ; line 265
	0xE9, 0x1E, 0x00, 0x00, 0x00,                               // JMP END ; line 266
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000004 ; line 269
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP END ; line 270
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 273
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP END ; line 274
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                         // JL 0x4F5837 ; line 277
};
static JumpTarget ranD22_SPINAIRB_jumpData[] = {
	{ 0x097, 0x4F5837 },
};
static PatchByteCode ranD22_SPINAIRB_patchByteCode = { ranD22_SPINAIRB_byteCode, 155, ranD22_SPINAIRB_jumpData, 1 };
// ranD22_SPINC declared in ran.asm line 280
static unsigned char ranD22_SPINC_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 281
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 282
	0xD9, 0xEE,                                                 // FLDZ ; line 283
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 284
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,                         // FLD DWORD PTR [0x858830] ; line 285
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 286
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x001A ; line 287
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                         // JL 0x4F5837 ; line 288
};
static JumpTarget ranD22_SPINC_jumpData[] = {
	{ 0x031, 0x4F5837 },
};
static PatchByteCode ranD22_SPINC_patchByteCode = { ranD22_SPINC_byteCode, 53, ranD22_SPINC_jumpData, 1 };
// ranD22_SPINAIRC declared in ran.asm line 291
static unsigned char ranD22_SPINAIRC_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 292
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 293
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x6A7],0x00 ; line 295
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,                         // JE LABEL0 ; line 296
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x6A7],0x01 ; line 297
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 298
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x02,                   // CMP BYTE PTR [ESI+0x6A7],0x02 ; line 299
	0x0F, 0x84, 0x30, 0x00, 0x00, 0x00,                         // JE LABEL2 ; line 300
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x03,                   // CMP BYTE PTR [ESI+0x6A7],0x03 ; line 301
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 302
	0xE9, 0x3C, 0x00, 0x00, 0x00,                               // JMP LABEL4 ; line 303
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000000D ; line 306
	0xE9, 0x3C, 0x00, 0x00, 0x00,                               // JMP END ; line 307
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000000A ; line 310
	0xE9, 0x2D, 0x00, 0x00, 0x00,                               // JMP END ; line 311
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000007 ; line 314
	0xE9, 0x1E, 0x00, 0x00, 0x00,                               // JMP END ; line 315
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000004 ; line 318
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP END ; line 319
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 322
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP END ; line 323
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                         // JL 0x4F5837 ; line 326
};
static JumpTarget ranD22_SPINAIRC_jumpData[] = {
	{ 0x097, 0x4F5837 },
};
static PatchByteCode ranD22_SPINAIRC_patchByteCode = { ranD22_SPINAIRC_byteCode, 155, ranD22_SPINAIRC_jumpData, 1 };
// ranD22_AIRLV declared in ran.asm line 334
static unsigned char ranD22_AIRLV_byteCode[] = {
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x6A7],0x00 ; line 335
};
static PatchByteCode ranD22_AIRLV_patchByteCode = { ranD22_AIRLV_byteCode, 7, nullptr, 0 };
// ran5SC_SPIN_BULLET_NOBOUNCE declared in ran.asm line 340
static unsigned char ran5SC_SPIN_BULLET_NOBOUNCE_byteCode[] = {
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x144] ; line 341
	0x81, 0xF8, 0xB4, 0x00, 0x00, 0x00, // CMP EAX,0x000000B4 ; line 342
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF, // JGE 0x5083D6 ; line 343
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x5082CB ; line 344
};
static JumpTarget ran5SC_SPIN_BULLET_NOBOUNCE_jumpData[] = {
	{ 0x00E, 0x5083D6 },
	{ 0x013, 0x5082CB },
};
static PatchByteCode ran5SC_SPIN_BULLET_NOBOUNCE_patchByteCode = { ran5SC_SPIN_BULLET_NOBOUNCE_byteCode, 23, ran5SC_SPIN_BULLET_NOBOUNCE_jumpData, 2 };
// ran5SC_SPIN_LAUNCHSPD declared in ran.asm line 350
static unsigned char ran5SC_SPIN_LAUNCHSPD_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x85882C] ; line 351
};
static PatchByteCode ran5SC_SPIN_LAUNCHSPD_patchByteCode = { ran5SC_SPIN_LAUNCHSPD_byteCode, 6, nullptr, 0 };
// ran5SC_SPIN_INSTANTFIRE declared in ran.asm line 355
static unsigned char ran5SC_SPIN_INSTANTFIRE_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 356
};
static PatchByteCode ran5SC_SPIN_INSTANTFIRE_patchByteCode = { ran5SC_SPIN_INSTANTFIRE_byteCode, 10, nullptr, 0 };
// ran5SC_SPIN_BULLETCOUNT declared in ran.asm line 360
static unsigned char ran5SC_SPIN_BULLETCOUNT_byteCode[] = {
	0x81, 0xF9, 0x78, 0x00, 0x00, 0x00, // CMP ECX,0x78 ; line 361
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x4F85EF ; line 362
};
static JumpTarget ran5SC_SPIN_BULLETCOUNT_jumpData[] = {
	{ 0x008, 0x4F85EF },
};
static PatchByteCode ran5SC_SPIN_BULLETCOUNT_patchByteCode = { ran5SC_SPIN_BULLETCOUNT_byteCode, 12, ran5SC_SPIN_BULLETCOUNT_jumpData, 1 };
// ran5SC_SPIN_BULLETCOUNT_AIR declared in ran.asm line 366
static unsigned char ran5SC_SPIN_BULLETCOUNT_AIR_byteCode[] = {
	0x81, 0xF9, 0x78, 0x00, 0x00, 0x00, // CMP ECX,0x78 ; line 367
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x4F883D ; line 368
};
static JumpTarget ran5SC_SPIN_BULLETCOUNT_AIR_jumpData[] = {
	{ 0x008, 0x4F883D },
};
static PatchByteCode ran5SC_SPIN_BULLETCOUNT_AIR_patchByteCode = { ran5SC_SPIN_BULLETCOUNT_AIR_byteCode, 12, ran5SC_SPIN_BULLETCOUNT_AIR_jumpData, 1 };
// ran5SC_SPIN_GRAVITY declared in ran.asm line 372
static unsigned char ran5SC_SPIN_GRAVITY_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 373
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 374
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 375
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 376
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 377
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 378
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x13E],0x03 ; line 380
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 381
	0xD9, 0x05, 0x68, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC68] ; line 382
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 383
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x04, 0x00,       // CMP WORD PTR [ESI+0x13E],0x04 ; line 385
};
static PatchByteCode ran5SC_SPIN_GRAVITY_patchByteCode = { ran5SC_SPIN_GRAVITY_byteCode, 83, nullptr, 0 };
// ran5SC_SPIN_GROUNDHITCOUNT declared in ran.asm line 389
static unsigned char ran5SC_SPIN_GROUNDHITCOUNT_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 390
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 391
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 392
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 393
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 394
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 395
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 397
	0xDD, 0xD8,                                                 // FSTP ST0 ; line 398
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487CA0 ; line 399
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4F84A4 ; line 400
};
static JumpTarget ran5SC_SPIN_GROUNDHITCOUNT_jumpData[] = {
	{ 0x034, 0x487CA0 },
	{ 0x039, 0x4F84A4 },
};
static PatchByteCode ran5SC_SPIN_GROUNDHITCOUNT_patchByteCode = { ran5SC_SPIN_GROUNDHITCOUNT_byteCode, 61, ran5SC_SPIN_GROUNDHITCOUNT_jumpData, 2 };
// ran5SC_SPIN declared in ran.asm line 404
static unsigned char ran5SC_SPIN_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 405
};
static PatchByteCode ran5SC_SPIN_patchByteCode = { ran5SC_SPIN_byteCode, 2, nullptr, 0 };
// ran3SC_SPIN_ACTIONS declared in ran.asm line 409
static unsigned char ran3SC_SPIN_ACTIONS_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000001 ; line 410
	0x0F, 0x8C, 0x92, 0x02, 0x00, 0x00,                         // JL LABEL1 ; line 411
	0x0F, 0x8F, 0x26, 0x00, 0x00, 0x00,                         // JG LABEL2 ; line 412
	0xD9, 0x05, 0x30, 0xB8, 0x85, 0x00,                         // FLD DWORD PTR [0x85B830] ; line 413
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 414
	0xD9, 0xEE,                                                 // FLDZ ; line 415
	0xD9, 0x96, 0xF8, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF8] ; line 416
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 417
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x0A,                   // MOV BYTE PTR [ESI+0x194],0x0A ; line 418
	0xE9, 0x66, 0x02, 0x00, 0x00,                               // JMP LABEL1 ; line 419
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x104],0x01 ; line 424
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE LABELLEFT ; line 425
	0xD9, 0xE8,                                                 // FLD1 ; line 426
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP LABELNEXT ; line 427
	0xD9, 0x05, 0x0C, 0x15, 0x87, 0x00,                         // FLD DWORD PTR [0x87150C] ; line 430
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x90, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0290 ; line 434
	0x0F, 0x85, 0x3A, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 435
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0001 ; line 436
	0x0F, 0x85, 0x2B, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 437
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x0000 ; line 438
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                         // JNE LABELM ; line 439
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x0001 ; line 440
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x0003 ; line 442
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 444
	0x81, 0xBE, 0x58, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x758],0x00 ; line 447
	0x0F, 0x8F, 0xBA, 0x00, 0x00, 0x00,                         // JG LABELM_DOWN ; line 448
	0x81, 0xBE, 0x78, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x778],0x00 ; line 449
	0x0F, 0x8F, 0xAA, 0x00, 0x00, 0x00,                         // JG LABELM_DOWN ; line 450
	0x81, 0xBE, 0x58, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x758],0x00 ; line 452
	0x0F, 0x8C, 0x55, 0x00, 0x00, 0x00,                         // JL LABELM_UP ; line 453
	0x81, 0xBE, 0x78, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x778],0x00 ; line 454
	0x0F, 0x8C, 0x45, 0x00, 0x00, 0x00,                         // JL LABELM_UP ; line 455
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 457
	0x0F, 0x8F, 0x7B, 0x01, 0x00, 0x00,                         // JG LABELM_RIGHTONLY ; line 458
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 459
	0x0F, 0x8F, 0x6B, 0x01, 0x00, 0x00,                         // JG LABELM_RIGHTONLY ; line 460
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 462
	0x0F, 0x8C, 0x74, 0x01, 0x00, 0x00,                         // JL LABELM_LEFTONLY ; line 463
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 464
	0x0F, 0x8C, 0x64, 0x01, 0x00, 0x00,                         // JL LABELM_LEFTONLY ; line 465
	0xE9, 0x7E, 0x01, 0x00, 0x00,                               // JMP LABEL1 ; line 466
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 469
	0x0F, 0x8F, 0x7A, 0x00, 0x00, 0x00,                         // JG LABELM_UPRIGHT ; line 470
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 471
	0x0F, 0x8F, 0x6A, 0x00, 0x00, 0x00,                         // JG LABELM_UPRIGHT ; line 472
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 474
	0x0F, 0x8C, 0x96, 0x00, 0x00, 0x00,                         // JL LABELM_UPLEFT ; line 475
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 476
	0x0F, 0x8C, 0x86, 0x00, 0x00, 0x00,                         // JL LABELM_UPLEFT ; line 477
	0xE9, 0xC9, 0x00, 0x00, 0x00,                               // JMP LABELM_UPONLY ; line 478
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 481
	0x0F, 0x8F, 0x52, 0x00, 0x00, 0x00,                         // JG LABELM_DOWNRIGHT ; line 482
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 483
	0x0F, 0x8F, 0x42, 0x00, 0x00, 0x00,                         // JG LABELM_DOWNRIGHT ; line 484
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 486
	0x0F, 0x8C, 0x74, 0x00, 0x00, 0x00,                         // JL LABELM_DOWNLEFT ; line 487
	0x81, 0xBE, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x774],0x00 ; line 488
	0x0F, 0x8C, 0x64, 0x00, 0x00, 0x00,                         // JL LABELM_DOWNLEFT ; line 489
	0xE9, 0x9F, 0x00, 0x00, 0x00,                               // JMP LABELM_DOWNONLY ; line 490
	0xD8, 0x0D, 0x68, 0xBA, 0x85, 0x00,                         // FMUL DWORD PTR [0x85BA68] ; line 493
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 494
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00,                         // FLD DWORD PTR [0x85BA68] ; line 495
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 496
	0xE9, 0xD7, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 497
	0xD8, 0x0D, 0x68, 0xBA, 0x85, 0x00,                         // FMUL DWORD PTR [0x85BA68] ; line 499
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 500
	0xD9, 0xEE,                                                 // FLDZ ; line 501
	0xD8, 0x25, 0x68, 0xBA, 0x85, 0x00,                         // FSUB DWORD PTR [0x85BA68] ; line 502
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 503
	0xE9, 0xB8, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 504
	0xD8, 0x0D, 0x0C, 0x15, 0x87, 0x00,                         // FMUL DWORD PTR [0x87150C] ; line 506
	0xD8, 0x0D, 0x68, 0xBA, 0x85, 0x00,                         // FMUL DWORD PTR [0x85BA68] ; line 507
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 508
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00,                         // FLD DWORD PTR [0x85BA68] ; line 509
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 510
	0xE9, 0x95, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 511
	0xD8, 0x0D, 0x0C, 0x15, 0x87, 0x00,                         // FMUL DWORD PTR [0x87150C] ; line 513
	0xD8, 0x0D, 0x68, 0xBA, 0x85, 0x00,                         // FMUL DWORD PTR [0x85BA68] ; line 514
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 515
	0xD9, 0xEE,                                                 // FLDZ ; line 516
	0xD8, 0x25, 0x68, 0xBA, 0x85, 0x00,                         // FSUB DWORD PTR [0x85BA68] ; line 517
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 518
	0xE9, 0x70, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 519
	0xDD, 0xD8,                                                 // FSTP ST0 ; line 521
	0xD9, 0xEE,                                                 // FLDZ ; line 522
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 523
	0xD9, 0x05, 0x30, 0xB8, 0x85, 0x00,                         // FLD DWORD PTR [0x85B830] ; line 524
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 525
	0xE9, 0x55, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 526
	0xDD, 0xD8,                                                 // FSTP ST0 ; line 528
	0xD9, 0xEE,                                                 // FLDZ ; line 529
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 530
	0xD9, 0xEE,                                                 // FLDZ ; line 531
	0xD8, 0x25, 0x30, 0xB8, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B830] ; line 532
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 533
	0xE9, 0x38, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 534
	0xD8, 0x0D, 0x30, 0xB8, 0x85, 0x00,                         // FMUL DWORD PTR [0x85B830] ; line 536
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 537
	0xD9, 0xEE,                                                 // FLDZ ; line 538
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 539
	0xE9, 0x1F, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 540
	0xD8, 0x0D, 0x0C, 0x15, 0x87, 0x00,                         // FMUL DWORD PTR [0x87150C] ; line 542
	0xD8, 0x0D, 0x30, 0xB8, 0x85, 0x00,                         // FMUL DWORD PTR [0x85B830] ; line 543
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 544
	0xD9, 0xEE,                                                 // FLDZ ; line 545
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 546
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 547
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000003C ; line 550
};
static PatchByteCode ran3SC_SPIN_ACTIONS_patchByteCode = { ran3SC_SPIN_ACTIONS_byteCode, 684, nullptr, 0 };
// ran3SC_SPIN_TRAIL declared in ran.asm line 554
static unsigned char ran3SC_SPIN_TRAIL_byteCode[] = {
	0xD9, 0xEE,                                                 // FLDZ ; line 555
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00,                         // FST DWORD PTR [ESI+0xF4] ; line 556
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 557
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 558
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xEC] ; line 559
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 560
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xF0] ; line 561
	0xD8, 0x05, 0x84, 0xB9, 0x85, 0x00,                         // FADD DWORD PTR [0x85B984] ; line 562
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 563
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x5E, 0x02,       // CMP WORD PTR [EAX+0x13C],0x025E ; line 565
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 566
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x90, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0290 ; line 567
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABELNEXT ; line 568
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x508724 ; line 569
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [EAX+0x13E],0x0001 ; line 572
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                         // JG 0x508724 ; line 573
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 575
	0x0F, 0x85, 0x1E, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 576
	0xD9, 0x05, 0xB4, 0x96, 0x85, 0x00,                         // FLD DWORD PTR [0x8596B4] ; line 577
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 578
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 579
	0x6A, 0x40,                                                 // PUSH 0x40 ; line 580
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 581
	0x51,                                                       // PUSH ECX ; line 582
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 583
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 584
	0x68, 0xDE, 0x03, 0x00, 0x00,                               // PUSH 0x000003DE ; line 585
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x4B0F50 ; line 586
	0x8B, 0x8E, 0x90, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x190] ; line 589
	0x81, 0xF9, 0x05, 0x00, 0x00, 0x00,                         // CMP ECX,0x05 ; line 590
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x508676 ; line 591
};
static JumpTarget ran3SC_SPIN_TRAIL_jumpData[] = {
	{ 0x051, 0x508724 },
	{ 0x060, 0x508724 },
	{ 0x08E, 0x4B0F50 },
	{ 0x09F, 0x508676 },
};
static PatchByteCode ran3SC_SPIN_TRAIL_patchByteCode = { ran3SC_SPIN_TRAIL_byteCode, 163, ran3SC_SPIN_TRAIL_jumpData, 4 };
// ran3SC_SPIN_ONEBULLET declared in ran.asm line 595
static unsigned char ran3SC_SPIN_ONEBULLET_byteCode[] = {
	0x90, // NOP ; line 596
};
static PatchByteCode ran3SC_SPIN_ONEBULLET_patchByteCode = { ran3SC_SPIN_ONEBULLET_byteCode, 1, nullptr, 0 };
// ran3SC_SPIN_ONEBULLET_AIR declared in ran.asm line 600
static unsigned char ran3SC_SPIN_ONEBULLET_AIR_byteCode[] = {
	0x90, // NOP ; line 601
};
static PatchByteCode ran3SC_SPIN_ONEBULLET_AIR_patchByteCode = { ran3SC_SPIN_ONEBULLET_AIR_byteCode, 1, nullptr, 0 };
// ran3SC_SPIN_TOAIR declared in ran.asm line 605
static unsigned char ran3SC_SPIN_TOAIR_byteCode[] = {
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                   // JNE LABEL1 ; line 606
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F8E7A ; line 607
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x1D, 0x00, // CMP WORD PTR [ESI+0x140],0x001D ; line 610
	0x0F, 0x8C, 0x15, 0x00, 0x00, 0x00,                   // JL LABEL2 ; line 611
	0x8B, 0x16,                                           // MOV EDX,[ESI] ; line 613
	0x8B, 0x42, 0x04,                                     // MOV EAX,[EDX+0x04] ; line 614
	0x6A, 0x01,                                           // PUSH 0x00000001 ; line 615
	0x68, 0x90, 0x02, 0x00, 0x00,                         // PUSH 0x00000290 ; line 616
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 617
	0xFF, 0xD0,                                           // CALL EAX ; line 618
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4ED915 ; line 619
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4ED915 ; line 622
};
static JumpTarget ran3SC_SPIN_TOAIR_jumpData[] = {
	{ 0x007, 0x4F8E7A },
	{ 0x02B, 0x4ED915 },
	{ 0x030, 0x4ED915 },
};
static PatchByteCode ran3SC_SPIN_TOAIR_patchByteCode = { ran3SC_SPIN_TOAIR_byteCode, 52, ran3SC_SPIN_TOAIR_jumpData, 3 };
// ranSC_VAJRA_TRANSFORM declared in ran.asm line 628
static unsigned char ranSC_VAJRA_TRANSFORM_byteCode[] = {
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x12C] ; line 629
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x52, 0x03, // CMP WORD PTR [ESI+0x13C],0x0352 ; line 631
	0x0F, 0x85, 0x38, 0x00, 0x00, 0x00,                   // JNE LABEL1 ; line 632
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x0000 ; line 633
	0x0F, 0x85, 0x29, 0x00, 0x00, 0x00,                   // JNE LABEL1 ; line 634
	0xD9, 0x86, 0x2C, 0x01, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0x12C] ; line 636
	0xD9, 0x05, 0x98, 0x81, 0x85, 0x00,                   // FLD DWORD PTR [0x858198] ; line 637
	0xDE, 0xD9,                                           // FCOMPP ; line 638
	0xDF, 0xE0,                                           // FNSTSW AX ; line 639
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 640
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 641
	0xC6, 0x86, 0x12, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x112],0x00 ; line 642
	0x66, 0xC7, 0x86, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00, // MOV WORD PTR [ESI+0x13E],0x0006 ; line 643
};
static PatchByteCode ranSC_VAJRA_TRANSFORM_patchByteCode = { ranSC_VAJRA_TRANSFORM_byteCode, 77, nullptr, 0 };
// ranSC_VAJRA_SUBSEQREDIR declared in ran.asm line 650
static unsigned char ranSC_VAJRA_SUBSEQREDIR_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x13E],0x0006 ; line 651
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x50778C ; line 652
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x13E],0x0007 ; line 653
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x5077EE ; line 654
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x08, 0x00, // CMP WORD PTR [ESI+0x13E],0x0008 ; line 655
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x50796A ; line 656
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x0000 ; line 657
};
static JumpTarget ranSC_VAJRA_SUBSEQREDIR_jumpData[] = {
	{ 0x00B, 0x50778C },
	{ 0x01A, 0x5077EE },
	{ 0x029, 0x50796A },
};
static PatchByteCode ranSC_VAJRA_SUBSEQREDIR_patchByteCode = { ranSC_VAJRA_SUBSEQREDIR_byteCode, 54, ranSC_VAJRA_SUBSEQREDIR_jumpData, 3 };
// ranSC_VAJRA_ROTATE declared in ran.asm line 661
static unsigned char ranSC_VAJRA_ROTATE_byteCode[] = {
	0xDD, 0xD8,                                                 // FSTP ST0 ; line 662
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 664
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x52, 0x03,       // CMP WORD PTR [ESI+0x13C],0x0352 ; line 665
	0x0F, 0x85, 0x81, 0x00, 0x00, 0x00,                         // JNE LABEL3SC ; line 666
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xEC] ; line 667
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x07, 0x00,       // CMP WORD PTR [ESI+0x13E],0x07 ; line 669
	0x0F, 0x85, 0x2A, 0x00, 0x00, 0x00,                         // JNE LABELGOKI ; line 670
	0x80, 0xB8, 0x04, 0x01, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [EAX+0x104],0x01 ; line 672
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                         // JNE LABELZBACK ; line 673
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                         // FADD DWORD PTR [0x871544] ; line 674
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                         // FADD DWORD PTR [0x871544] ; line 675
	0xD8, 0x25, 0x44, 0x15, 0x87, 0x00,                         // FSUB DWORD PTR [0x871544] ; line 677
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 678
	0xE9, 0x25, 0x00, 0x00, 0x00,                               // JMP LABELZG ; line 679
	0x80, 0xB8, 0x04, 0x01, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [EAX+0x104],0x01 ; line 682
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                         // JNE LABELGBACK ; line 683
	0xD8, 0x25, 0x44, 0x15, 0x87, 0x00,                         // FSUB DWORD PTR [0x871544] ; line 684
	0xD8, 0x25, 0x44, 0x15, 0x87, 0x00,                         // FSUB DWORD PTR [0x871544] ; line 685
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                         // FADD DWORD PTR [0x871544] ; line 687
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 688
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xF0] ; line 691
	0xD8, 0x05, 0xC8, 0x7D, 0x85, 0x00,                         // FADD DWORD PTR [0x857DC8] ; line 692
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00,                         // FSUB DWORD PTR [0x86782C] ; line 693
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 694
	0xE9, 0xB2, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 696
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xEC] ; line 698
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 699
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [EAX+0xF0] ; line 700
	0xD8, 0x05, 0xC8, 0x7D, 0x85, 0x00,                         // FADD DWORD PTR [0x857DC8] ; line 701
	0xD8, 0x05, 0xC8, 0x7D, 0x85, 0x00,                         // FADD DWORD PTR [0x857DC8] ; line 702
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00,                         // FSUB DWORD PTR [0x85AE94] ; line 703
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 704
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x53, 0x03,       // CMP WORD PTR [ESI+0x13C],0x0353 ; line 706
	0x0F, 0x85, 0x64, 0x00, 0x00, 0x00,                         // JNE LABEL2B ; line 707
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000096 ; line 708
	0x0F, 0x8C, 0x54, 0x00, 0x00, 0x00,                         // JL LABEL2B ; line 709
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x000000A0 ; line 710
	0x0F, 0x8D, 0x2A, 0x00, 0x00, 0x00,                         // JGE LABEL2C ; line 711
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x144] ; line 712
	0x80, 0xE8, 0x96,                                           // SUB AL,0x96 ; line 713
	0x6B, 0xC0, 0x0A,                                           // IMUL EAX,EAX,0x0A ; line 714
	0xC6, 0x86, 0x10, 0x01, 0x00, 0x00, 0x7D,                   // MOV BYTE PTR [ESI+0x110],0x7D ; line 715
	0x28, 0x86, 0x10, 0x01, 0x00, 0x00,                         // SUB [ESI+0x110],AL ; line 716
	0x8A, 0x86, 0x10, 0x01, 0x00, 0x00,                         // MOV AL,[ESI+0x110] ; line 717
	0x88, 0x86, 0x11, 0x01, 0x00, 0x00,                         // MOV [ESI+0x111],AL ; line 718
	0xE9, 0x2F, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 719
	0xC6, 0x86, 0x10, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x110],0x00 ; line 721
	0xC6, 0x86, 0x11, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x111],0x00 ; line 722
	0xC6, 0x86, 0x12, 0x01, 0x00, 0x00, 0xFF,                   // MOV BYTE PTR [ESI+0x112],0xFF ; line 723
	0xE9, 0x15, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 724
	0xC6, 0x86, 0x10, 0x01, 0x00, 0x00, 0x7D,                   // MOV BYTE PTR [ESI+0x110],0x7D ; line 726
	0xC6, 0x86, 0x11, 0x01, 0x00, 0x00, 0x7D,                   // MOV BYTE PTR [ESI+0x111],0x7D ; line 727
	0xC6, 0x86, 0x12, 0x01, 0x00, 0x00, 0xFF,                   // MOV BYTE PTR [ESI+0x112],0xFF ; line 728
	0xDB, 0x86, 0x44, 0x01, 0x00, 0x00,                         // FILD DWORD PTR [ESI+0x144] ; line 731
	0xDC, 0x0D, 0xA0, 0x8E, 0x85, 0x00,                         // FMUL QWORD PTR [0x858EA0] ; line 732
	0xDC, 0x0D, 0xA0, 0x8E, 0x85, 0x00,                         // FMUL QWORD PTR [0x858EA0] ; line 733
	0xD8, 0x86, 0x2C, 0x01, 0x00, 0x00,                         // FADD DWORD PTR [ESI+0x12C] ; line 734
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 735
};
static PatchByteCode ranSC_VAJRA_ROTATE_patchByteCode = { ranSC_VAJRA_ROTATE_byteCode, 360, nullptr, 0 };
// ran2SC_VAJRA_SHOOT declared in ran.asm line 739
static unsigned char ran2SC_VAJRA_SHOOT_byteCode[] = {
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 740
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x000000FF ; line 741
	0x0F, 0x8D, 0x3C, 0x00, 0x00, 0x00,                         // JGE LABELFIRE ; line 742
	0x81, 0xB8, 0x70, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [EAX+0x770],0x00000000 ; line 743
	0x0F, 0x8F, 0xD4, 0x00, 0x00, 0x00,                         // JG LABELEND ; line 744
	0x81, 0xB8, 0x90, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [EAX+0x790],0x00000000 ; line 745
	0x0F, 0x8F, 0xC4, 0x00, 0x00, 0x00,                         // JG LABELEND ; line 746
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x52, 0x03,       // CMP WORD PTR [ESI+0x13C],0x0352 ; line 762
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE LABELFIRE ; line 763
	0x80, 0xBE, 0x12, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x112],0x00 ; line 764
	0x0F, 0x85, 0x8E, 0x00, 0x00, 0x00,                         // JNE LABELGOKI ; line 765
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x53, 0x03,       // CMP WORD PTR [ESI+0x13C],0x0353 ; line 768
	0x0F, 0x85, 0x7A, 0x00, 0x00, 0x00,                         // JNE LABELCOUNTEND ; line 769
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x02,                   // MOV BYTE PTR [ESI+0x194],0x02 ; line 770
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000020 ; line 771
	0x0F, 0x8C, 0x63, 0x00, 0x00, 0x00,                         // JL LABELCOUNTEND ; line 772
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x03,                   // MOV BYTE PTR [ESI+0x194],0x03 ; line 773
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000040 ; line 774
	0x0F, 0x8C, 0x4C, 0x00, 0x00, 0x00,                         // JL LABELCOUNTEND ; line 775
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x04,                   // MOV BYTE PTR [ESI+0x194],0x04 ; line 776
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000060 ; line 777
	0x0F, 0x8C, 0x35, 0x00, 0x00, 0x00,                         // JL LABELCOUNTEND ; line 778
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x05,                   // MOV BYTE PTR [ESI+0x194],0x05 ; line 779
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000080 ; line 780
	0x0F, 0x8C, 0x1E, 0x00, 0x00, 0x00,                         // JL LABELCOUNTEND ; line 781
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x06,                   // MOV BYTE PTR [ESI+0x194],0x06 ; line 782
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x000000A0 ; line 783
	0x0F, 0x8C, 0x07, 0x00, 0x00, 0x00,                         // JL LABELCOUNTEND ; line 784
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x07,                   // MOV BYTE PTR [ESI+0x194],0x07 ; line 785
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x507839 ; line 787
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xEB, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x000000EB ; line 790
	0x0F, 0x8F, 0x0A, 0x00, 0x00, 0x00,                         // JG LABELEND ; line 791
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0xEB, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x000000EB ; line 792
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x50795C ; line 795
};
static JumpTarget ran2SC_VAJRA_SHOOT_jumpData[] = {
	{ 0x0DC, 0x507839 },
	{ 0x0FB, 0x50795C },
};
static PatchByteCode ran2SC_VAJRA_SHOOT_patchByteCode = { ran2SC_VAJRA_SHOOT_byteCode, 255, ran2SC_VAJRA_SHOOT_jumpData, 2 };
// ran3SC_VAJRAB_JUMP declared in ran.asm line 799
static unsigned char ran3SC_VAJRAB_JUMP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x507771 ; line 800
};
static JumpTarget ran3SC_VAJRAB_JUMP_jumpData[] = {
	{ 0x001, 0x507771 },
};
static PatchByteCode ran3SC_VAJRAB_JUMP_patchByteCode = { ran3SC_VAJRAB_JUMP_byteCode, 5, ran3SC_VAJRAB_JUMP_jumpData, 1 };
// ran3SC_VAJRA_JUMP declared in ran.asm line 804
static unsigned char ran3SC_VAJRA_JUMP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F7DB4 ; line 805
};
static JumpTarget ran3SC_VAJRA_JUMP_jumpData[] = {
	{ 0x001, 0x4F7DB4 },
};
static PatchByteCode ran3SC_VAJRA_JUMP_patchByteCode = { ran3SC_VAJRA_JUMP_byteCode, 5, ran3SC_VAJRA_JUMP_jumpData, 1 };
// ran3SC_VAJRA_HITS declared in ran.asm line 811
static unsigned char ran3SC_VAJRA_HITS_byteCode[] = {
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 812
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000000 ; line 813
	0x66, 0x81, 0xBE, 0x6E, 0x03, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x36E],0x0000 ; line 814
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x53, 0x03,       // CMP WORD PTR [ESI+0x13C],0x0353 ; line 816
	0x0F, 0x85, 0xDC, 0x00, 0x00, 0x00,                         // JNE LABELNOT ; line 817
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000000 ; line 819
	0x0F, 0x84, 0xCC, 0x00, 0x00, 0x00,                         // JE LABELNOT ; line 820
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000001 ; line 821
	0x0F, 0x84, 0x55, 0x00, 0x00, 0x00,                         // JE LABELHIT ; line 822
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000002 ; line 823
	0x0F, 0x84, 0x45, 0x00, 0x00, 0x00,                         // JE LABELHIT ; line 824
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000004 ; line 825
	0x0F, 0x84, 0x88, 0x00, 0x00, 0x00,                         // JE LABELEND ; line 826
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000006 ; line 827
	0x0F, 0x84, 0x62, 0x00, 0x00, 0x00,                         // JE LABELGRAZE ; line 828
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000007 ; line 829
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                         // JE LABELHIT ; line 830
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00000008 ; line 831
	0x0F, 0x84, 0x42, 0x00, 0x00, 0x00,                         // JE LABELGRAZE ; line 832
	0xE9, 0x58, 0x00, 0x00, 0x00,                               // JMP LABELIDK ; line 833
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 836
	0x0F, 0x8E, 0x5A, 0x00, 0x00, 0x00,                         // JLE LABELNOT ; line 837
	0x66, 0x81, 0x86, 0x6E, 0x03, 0x00, 0x00, 0x01, 0x00,       // ADD WORD PTR [ESI+0x36E],0x01 ; line 838
	0x66, 0x81, 0xBE, 0x6E, 0x03, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x36E],0x0001 ; line 839
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                         // JNE LABELHIT2 ; line 840
	0x66, 0xC7, 0x86, 0x6E, 0x03, 0x00, 0x00, 0x00, 0x00,       // MOV WORD PTR [ESI+0x36E],0x0000 ; line 841
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 842
	0xE9, 0x2A, 0x00, 0x00, 0x00,                               // JMP LABELNOT ; line 845
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 848
	0x80, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // ADD BYTE PTR [ESI+0x194],0x01 ; line 849
	0xE9, 0x14, 0x00, 0x00, 0x00,                               // JMP LABELNOT ; line 850
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP LABELNOT ; line 854
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 857
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABELNOT ; line 858
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 861
};
static PatchByteCode ran3SC_VAJRA_HITS_patchByteCode = { ran3SC_VAJRA_HITS_byteCode, 267, nullptr, 0 };
// ran3SC_VAJRA_TO850 declared in ran.asm line 874
static unsigned char ran3SC_VAJRA_TO850_byteCode[] = {
	0x81, 0xF8, 0x33, 0x00, 0x00, 0x00,       // CMP EAX,0x00000033 ; line 875
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,       // JNE LABEL2 ; line 876
	0x0F, 0xB6, 0x80, 0xD0, 0xFB, 0x50, 0x00, // MOVZX EAX,BYTE PTR [EAX+0x50FBD0] ; line 877
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x50D32A ; line 878
	0x0F, 0xB6, 0x80, 0xD0, 0xFB, 0x50, 0x00, // MOVZX EAX,BYTE PTR [EAX+0x50FBD0] ; line 880
};
static JumpTarget ran3SC_VAJRA_TO850_jumpData[] = {
	{ 0x014, 0x50D32A },
};
static PatchByteCode ran3SC_VAJRA_TO850_patchByteCode = { ran3SC_VAJRA_TO850_byteCode, 31, ran3SC_VAJRA_TO850_jumpData, 1 };
// ran3SC_VAJRA_TO600 declared in ran.asm line 884
static unsigned char ran3SC_VAJRA_TO600_byteCode[] = {
	0x81, 0xF8, 0x64, 0x00, 0x00, 0x00, // CMP EAX,0x00000064 ; line 885
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x4FE7A3 ; line 886
	0x81, 0xF8, 0xEF, 0x00, 0x00, 0x00, // CMP EAX,0x000000EF ; line 887
};
static JumpTarget ran3SC_VAJRA_TO600_jumpData[] = {
	{ 0x008, 0x4FE7A3 },
};
static PatchByteCode ran3SC_VAJRA_TO600_patchByteCode = { ran3SC_VAJRA_TO600_byteCode, 18, ran3SC_VAJRA_TO600_jumpData, 1 };
// ran3SC_VAJRA_COUNTJUMP declared in ran.asm line 891
static unsigned char ran3SC_VAJRA_COUNTJUMP_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x58, 0x02, // CMP WORD PTR [ESI+0x13C],0x0258 ; line 892
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 893
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x144] ; line 894
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7ED5 ; line 895
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7F7B ; line 897
};
static JumpTarget ran3SC_VAJRA_COUNTJUMP_jumpData[] = {
	{ 0x016, 0x4F7ED5 },
	{ 0x01B, 0x4F7F7B },
};
static PatchByteCode ran3SC_VAJRA_COUNTJUMP_patchByteCode = { ran3SC_VAJRA_COUNTJUMP_byteCode, 31, ran3SC_VAJRA_COUNTJUMP_jumpData, 2 };
// ran2SC_2VAJRA_COUNT declared in ran.asm line 901
static unsigned char ran2SC_2VAJRA_COUNT_byteCode[] = {
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x144] ; line 902
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x58, 0x02, // CMP WORD PTR [ESI+0x13C],0x0258 ; line 903
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 904
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00,                   // CMP EAX,0x02 ; line 905
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7DE2 ; line 906
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 908
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7DE2 ; line 909
};
static JumpTarget ran2SC_2VAJRA_COUNT_jumpData[] = {
	{ 0x01C, 0x4F7DE2 },
	{ 0x027, 0x4F7DE2 },
};
static PatchByteCode ran2SC_2VAJRA_COUNT_patchByteCode = { ran2SC_2VAJRA_COUNT_byteCode, 43, ran2SC_2VAJRA_COUNT_jumpData, 2 };
// ran2SC_2VAJRA_ANG1 declared in ran.asm line 913
static unsigned char ran2SC_2VAJRA_ANG1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x58, 0x02, // CMP WORD PTR [ESI+0x13C],0x0258 ; line 914
	0x0F, 0x85, 0x2F, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 915
	0xD8, 0x05, 0x98, 0x81, 0x85, 0x00,                   // FADD DWORD PTR [0x858198] ; line 916
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 917
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 918
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 919
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 920
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 921
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 922
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7E25 ; line 923
	0xD8, 0x05, 0x98, 0x81, 0x85, 0x00,                   // FADD DWORD PTR [0x858198] ; line 925
};
static JumpTarget ran2SC_2VAJRA_ANG1_jumpData[] = {
	{ 0x03A, 0x4F7E25 },
};
static PatchByteCode ran2SC_2VAJRA_ANG1_patchByteCode = { ran2SC_2VAJRA_ANG1_byteCode, 68, ran2SC_2VAJRA_ANG1_jumpData, 1 };
// ran2SC_2VAJRA_ANG2 declared in ran.asm line 928
static unsigned char ran2SC_2VAJRA_ANG2_byteCode[] = {
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 929
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 930
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 931
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 932
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 933
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 934
	0xD8, 0x05, 0x98, 0x81, 0x85, 0x00, // FADD DWORD PTR [0x858198] ; line 935
};
static PatchByteCode ran2SC_2VAJRA_ANG2_patchByteCode = { ran2SC_2VAJRA_ANG2_byteCode, 42, nullptr, 0 };
// ran2SC_2VAJRA_STARTX declared in ran.asm line 940
static unsigned char ran2SC_2VAJRA_STARTX_byteCode[] = {
	0xD8, 0x0D, 0x54, 0xBB, 0x85, 0x00, // FMUL DWORD PTR [0x85BB54] ; line 941
};
static PatchByteCode ran2SC_2VAJRA_STARTX_patchByteCode = { ran2SC_2VAJRA_STARTX_byteCode, 6, nullptr, 0 };
// ranSC_VAJRAB_JUMP declared in ran.asm line 946
static unsigned char ranSC_VAJRAB_JUMP_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x52, 0x03, // CMP WORD PTR [ESI+0x13C],0x0352 ; line 947
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 948
	0x68, 0x52, 0x03, 0x00, 0x00,                         // PUSH 0x00000352 ; line 949
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x507904 ; line 950
	0x68, 0x53, 0x03, 0x00, 0x00,                         // PUSH 0x00000353 ; line 952
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x507904 ; line 953
};
static JumpTarget ranSC_VAJRAB_JUMP_jumpData[] = {
	{ 0x015, 0x507904 },
	{ 0x01F, 0x507904 },
};
static PatchByteCode ranSC_VAJRAB_JUMP_patchByteCode = { ranSC_VAJRAB_JUMP_byteCode, 35, ranSC_VAJRAB_JUMP_jumpData, 2 };
// ranSC_VAJRAB_JUMP2 declared in ran.asm line 957
static unsigned char ranSC_VAJRAB_JUMP2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x52, 0x03, // CMP WORD PTR [ESI+0x13C],0x0352 ; line 958
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 959
	0x68, 0x52, 0x03, 0x00, 0x00,                         // PUSH 0x00000352 ; line 960
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x507943 ; line 961
	0x68, 0x53, 0x03, 0x00, 0x00,                         // PUSH 0x00000353 ; line 963
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x507943 ; line 964
};
static JumpTarget ranSC_VAJRAB_JUMP2_jumpData[] = {
	{ 0x015, 0x507943 },
	{ 0x01F, 0x507943 },
};
static PatchByteCode ranSC_VAJRAB_JUMP2_patchByteCode = { ranSC_VAJRAB_JUMP2_byteCode, 35, ranSC_VAJRAB_JUMP2_jumpData, 2 };
// ranSC_VAJRA_SEQ declared in ran.asm line 968
static unsigned char ranSC_VAJRA_SEQ_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x58, 0x02, // CMP WORD PTR [ESI+0x13C],0x0258 ; line 969
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 970
	0x68, 0x52, 0x03, 0x00, 0x00,                         // PUSH 0x00000352 ; line 971
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7ECA ; line 972
	0x68, 0x53, 0x03, 0x00, 0x00,                         // PUSH 0x00000353 ; line 974
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7ECA ; line 975
};
static JumpTarget ranSC_VAJRA_SEQ_jumpData[] = {
	{ 0x015, 0x4F7ECA },
	{ 0x01F, 0x4F7ECA },
};
static PatchByteCode ranSC_VAJRA_SEQ_patchByteCode = { ranSC_VAJRA_SEQ_byteCode, 35, ranSC_VAJRA_SEQ_jumpData, 2 };
// ranSC_VAJRA_SEQ2 declared in ran.asm line 979
static unsigned char ranSC_VAJRA_SEQ2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x58, 0x02, // CMP WORD PTR [ESI+0x13C],0x0258 ; line 980
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                   // JNE LABEL3SC ; line 981
	0x68, 0x52, 0x03, 0x00, 0x00,                         // PUSH 0x00000352 ; line 982
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7F76 ; line 983
	0x68, 0x53, 0x03, 0x00, 0x00,                         // PUSH 0x00000353 ; line 985
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F7F76 ; line 986
};
static JumpTarget ranSC_VAJRA_SEQ2_jumpData[] = {
	{ 0x015, 0x4F7F76 },
	{ 0x01F, 0x4F7F76 },
};
static PatchByteCode ranSC_VAJRA_SEQ2_patchByteCode = { ranSC_VAJRA_SEQ2_byteCode, 35, ranSC_VAJRA_SEQ2_jumpData, 2 };
// ranD214C_OFFSET declared in ran.asm line 992
static unsigned char ranD214C_OFFSET_byteCode[] = {
	0xDD, 0xD8,                         // FSTP ST0 ; line 993
	0xD9, 0x05, 0x40, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871540] ; line 994
};
static PatchByteCode ranD214C_OFFSET_patchByteCode = { ranD214C_OFFSET_byteCode, 8, nullptr, 0 };
// ranD214_NOBOUNCE declared in ran.asm line 999
static unsigned char ranD214_NOBOUNCE_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x503BEF ; line 1000
};
static JumpTarget ranD214_NOBOUNCE_jumpData[] = {
	{ 0x001, 0x503BEF },
};
static PatchByteCode ranD214_NOBOUNCE_patchByteCode = { ranD214_NOBOUNCE_byteCode, 5, ranD214_NOBOUNCE_jumpData, 1 };
// ranD214_NOBOUNCE2 declared in ran.asm line 1004
static unsigned char ranD214_NOBOUNCE2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x503A67 ; line 1005
};
static JumpTarget ranD214_NOBOUNCE2_jumpData[] = {
	{ 0x001, 0x503A67 },
};
static PatchByteCode ranD214_NOBOUNCE2_patchByteCode = { ranD214_NOBOUNCE2_byteCode, 5, ranD214_NOBOUNCE2_jumpData, 1 };
// ranD214_ROTATELOOP declared in ran.asm line 1009
static unsigned char ranD214_ROTATELOOP_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 1010
	0x0F, 0x84, 0xE7, 0x00, 0x00, 0x00,                         // JE LABEL2A ; line 1011
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 1013
	0x0F, 0x85, 0xD2, 0x00, 0x00, 0x00,                         // JNE LABEL1A ; line 1014
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1018
	0xD9, 0xEE,                                                 // FLDZ ; line 1019
	0xD9, 0x18,                                                 // FSTP DWORD PTR [EAX] ; line 1020
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1021
	0x81, 0xEC, 0x0C, 0x00, 0x00, 0x00,                         // SUB ESP,0x0C ; line 1022
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1025
	0xD8, 0x8E, 0xF4, 0x00, 0x00, 0x00,                         // FMUL DWORD PTR [ESI+0xF4] ; line 1026
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1027
	0xD8, 0x8E, 0xF8, 0x00, 0x00, 0x00,                         // FMUL DWORD PTR [ESI+0xF8] ; line 1028
	0xDE, 0xC1,                                                 // FADDP ; line 1029
	0xDD, 0x05, 0xE0, 0x79, 0x85, 0x00,                         // FLD QWORD PTR [0x8579E0] ; line 1030
	0xDE, 0xD9,                                                 // FCOMPP ; line 1031
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1032
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1033
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABELB ; line 1034
	0xE9, 0x14, 0x00, 0x00, 0x00,                               // JMP SHORT LABELC ; line 1035
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1037
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00,                         // FLD DWORD PTR [0x85BBE4] ; line 1038
	0xD9, 0x58, 0x04,                                           // FSTP DWORD PTR [EAX+0x04] ; line 1039
	0xE9, 0x0F, 0x00, 0x00, 0x00,                               // JMP LABELNEXT ; line 1040
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1042
	0xDD, 0x05, 0xB0, 0x79, 0x85, 0x00,                         // FLD QWORD PTR [0x8579B0] ; line 1043
	0xD9, 0x58, 0x04,                                           // FSTP DWORD PTR [EAX+0x04] ; line 1044
	0xD9, 0x05, 0x44, 0x15, 0x87, 0x00,                         // FLD DWORD PTR [0x871544] ; line 1046
	0xD9, 0x5C, 0x24, 0x08,                                     // FSTP DWORD PTR [ESP+0x08] ; line 1047
	0xD9, 0x05, 0x3C, 0x15, 0x87, 0x00,                         // FLD DWORD PTR [0x87153C] ; line 1048
	0xD9, 0x5C, 0x24, 0x04,                                     // FSTP DWORD PTR [ESP+0x04] ; line 1049
	0xD9, 0xEE,                                                 // FLDZ ; line 1050
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 1051
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CCA0 ; line 1053
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 1054
	0xD9, 0x40, 0x04,                                           // FLD DWORD PTR [EAX+0x04] ; line 1055
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                         // SUB ESP,0x08 ; line 1056
	0xD9, 0x5C, 0x24, 0x04,                                     // FSTP DWORD PTR [ESP+0x04] ; line 1057
	0x8D, 0x4E, 0x04,                                           // LEA ECX,[ESI+0x04] ; line 1058
	0xD9, 0x00,                                                 // FLD DWORD PTR [EAX] ; line 1059
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 1060
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x438C00 ; line 1061
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x35C] ; line 1062
	0xD9, 0x01,                                                 // FLD DWORD PTR [ECX] ; line 1063
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD [ESI+0x12C] ; line 1064
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1066
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x2F ; line 1070
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,                         // JGE 0x503B18 ; line 1071
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x503B68 ; line 1072
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x503B68 ; line 1075
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x2F ; line 1078
	0x0F, 0x8E, 0x05, 0x00, 0x00, 0x00,                         // JLE LABEL2 ; line 1079
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x503B18 ; line 1080
	0xD9, 0x86, 0x2C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x12C] ; line 1083
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00,                         // FADD DWORD PTR [0x85B3B0] ; line 1084
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x12C] ; line 1085
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x503B68 ; line 1086
};
static JumpTarget ranD214_ROTATELOOP_jumpData[] = {
	{ 0x0A0, 0x48CCA0 },
	{ 0x0C0, 0x438C00 },
	{ 0x0E8, 0x503B18 },
	{ 0x0ED, 0x503B68 },
	{ 0x0F2, 0x503B68 },
	{ 0x107, 0x503B18 },
	{ 0x11E, 0x503B68 },
};
static PatchByteCode ranD214_ROTATELOOP_patchByteCode = { ranD214_ROTATELOOP_byteCode, 290, ranD214_ROTATELOOP_jumpData, 7 };
// ranD214_BULLETSPD declared in ran.asm line 1090
static unsigned char ranD214_BULLETSPD_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 1091
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EFEE1 ; line 1092
};
static JumpTarget ranD214_BULLETSPD_jumpData[] = {
	{ 0x003, 0x4EFEE1 },
};
static PatchByteCode ranD214_BULLETSPD_patchByteCode = { ranD214_BULLETSPD_byteCode, 7, ranD214_BULLETSPD_jumpData, 1 };
// ranD214_BULLETSPD_C declared in ran.asm line 1094
static unsigned char ranD214_BULLETSPD_C_byteCode[] = {
	0xD9, 0xE8,                   // FLD1 ; line 1095
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F030E ; line 1096
};
static JumpTarget ranD214_BULLETSPD_C_jumpData[] = {
	{ 0x003, 0x4F030E },
};
static PatchByteCode ranD214_BULLETSPD_C_patchByteCode = { ranD214_BULLETSPD_C_byteCode, 7, ranD214_BULLETSPD_C_jumpData, 1 };
// ranD214_BULLETSPD_CHARGE declared in ran.asm line 1101
static unsigned char ranD214_BULLETSPD_CHARGE_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 1102
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F00AD ; line 1103
};
static JumpTarget ranD214_BULLETSPD_CHARGE_jumpData[] = {
	{ 0x003, 0x4F00AD },
};
static PatchByteCode ranD214_BULLETSPD_CHARGE_patchByteCode = { ranD214_BULLETSPD_CHARGE_byteCode, 7, ranD214_BULLETSPD_CHARGE_jumpData, 1 };
// ranD214_BULLETSPD_CHARGE_C declared in ran.asm line 1105
static unsigned char ranD214_BULLETSPD_CHARGE_C_byteCode[] = {
	0xD9, 0xE8,                   // FLD1 ; line 1106
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F04A7 ; line 1107
};
static JumpTarget ranD214_BULLETSPD_CHARGE_C_jumpData[] = {
	{ 0x003, 0x4F04A7 },
};
static PatchByteCode ranD214_BULLETSPD_CHARGE_C_patchByteCode = { ranD214_BULLETSPD_CHARGE_C_byteCode, 7, ranD214_BULLETSPD_CHARGE_C_jumpData, 1 };
// ranD214_BULLETCOUNT declared in ran.asm line 1112
static unsigned char ranD214_BULLETCOUNT_byteCode[] = {
	0x80, 0xF8, 0x00,                         // CMP AL,0x00 ; line 1113
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,       // JE LABEL0 ; line 1114
	0x80, 0xF8, 0x01,                         // CMP AL,0x01 ; line 1115
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE LABEL1 ; line 1116
	0x80, 0xF8, 0x02,                         // CMP AL,0x02 ; line 1117
	0x0F, 0x84, 0x28, 0x00, 0x00, 0x00,       // JE LABEL2 ; line 1118
	0x80, 0xF8, 0x03,                         // CMP AL,0x03 ; line 1119
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,       // JE LABEL3 ; line 1120
	0x80, 0xF8, 0x04,                         // CMP AL,0x04 ; line 1121
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE LABEL4 ; line 1122
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FLD DWORD PTR [0x85BCA4] ; line 1125
	0xE9, 0x2C, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1126
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x867810] ; line 1129
	0xE9, 0x21, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1130
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x86782C] ; line 1133
	0xE9, 0x16, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1134
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1137
	0xE9, 0x0B, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1138
	0xDD, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FLD QWORD PTR [0x858EB0] ; line 1141
	0xE9, 0x00, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1142
	0xD9, 0x5C, 0x24, 0x14,                   // FSTP DWORD PTR [ESP+0x14] ; line 1145
	0x0F, 0xBF, 0x8E, 0xD2, 0x07, 0x00, 0x00, // MOVSX ECX,WORD PTR [ESI+0x7D2] ; line 1146
};
static PatchByteCode ranD214_BULLETCOUNT_patchByteCode = { ranD214_BULLETCOUNT_byteCode, 111, nullptr, 0 };
// ranD214_BULLETCOUNT_C declared in ran.asm line 1148
static unsigned char ranD214_BULLETCOUNT_C_byteCode[] = {
	0x80, 0xF8, 0x00,                         // CMP AL,0x00 ; line 1149
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,       // JE LABEL0 ; line 1150
	0x80, 0xF8, 0x01,                         // CMP AL,0x01 ; line 1151
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE LABEL1 ; line 1152
	0x80, 0xF8, 0x02,                         // CMP AL,0x02 ; line 1153
	0x0F, 0x84, 0x28, 0x00, 0x00, 0x00,       // JE LABEL2 ; line 1154
	0x80, 0xF8, 0x03,                         // CMP AL,0x03 ; line 1155
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,       // JE LABEL3 ; line 1156
	0x80, 0xF8, 0x04,                         // CMP AL,0x04 ; line 1157
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE LABEL4 ; line 1158
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FLD DWORD PTR [0x85BCA4] ; line 1161
	0xE9, 0x2C, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1162
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x867810] ; line 1165
	0xE9, 0x21, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1166
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x86782C] ; line 1169
	0xE9, 0x16, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1170
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1173
	0xE9, 0x0B, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1174
	0xDD, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FLD QWORD PTR [0x858EB0] ; line 1177
	0xE9, 0x00, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1178
	0xD9, 0x5C, 0x24, 0x14,                   // FSTP DWORD PTR [ESP+0x14] ; line 1181
	0x0F, 0xBF, 0x96, 0xD2, 0x07, 0x00, 0x00, // MOVSX EDX,WORD PTR [ESI+0x7D2] ; line 1182
};
static PatchByteCode ranD214_BULLETCOUNT_C_patchByteCode = { ranD214_BULLETCOUNT_C_byteCode, 111, nullptr, 0 };
// ranD214_BULLETCOUNT_CHARGE declared in ran.asm line 1187
static unsigned char ranD214_BULLETCOUNT_CHARGE_byteCode[] = {
	0x80, 0xF8, 0x00,                         // CMP AL,0x00 ; line 1188
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,       // JE LABEL0 ; line 1189
	0x80, 0xF8, 0x01,                         // CMP AL,0x01 ; line 1190
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE LABEL1 ; line 1191
	0x80, 0xF8, 0x02,                         // CMP AL,0x02 ; line 1192
	0x0F, 0x84, 0x34, 0x00, 0x00, 0x00,       // JE LABEL2 ; line 1193
	0x80, 0xF8, 0x03,                         // CMP AL,0x03 ; line 1194
	0x0F, 0x84, 0x3C, 0x00, 0x00, 0x00,       // JE LABEL3 ; line 1195
	0x80, 0xF8, 0x04,                         // CMP AL,0x04 ; line 1196
	0x0F, 0x84, 0x44, 0x00, 0x00, 0x00,       // JE LABEL4 ; line 1197
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FLD DWORD PTR [0x85BCA4] ; line 1200
	0xD8, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FADD DWORD PTR [0x85BCA4] ; line 1201
	0xE9, 0x44, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1202
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x867810] ; line 1205
	0xD8, 0x05, 0x10, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x867810] ; line 1206
	0xE9, 0x33, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1207
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x86782C] ; line 1210
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 1211
	0xE9, 0x22, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1212
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1215
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00,       // FADD DWORD PTR [0x858830] ; line 1216
	0xE9, 0x11, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1217
	0xDD, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FLD QWORD PTR [0x858EB0] ; line 1220
	0xDC, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FADD QWORD PTR [0x858EB0] ; line 1221
	0xE9, 0x00, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1222
	0xD9, 0x5C, 0x24, 0x14,                   // FSTP DWORD PTR [ESP+0x14] ; line 1225
	0x0F, 0xBF, 0x96, 0xD2, 0x07, 0x00, 0x00, // MOVSX EDX,WORD PTR [ESI+0x7D2] ; line 1226
};
static PatchByteCode ranD214_BULLETCOUNT_CHARGE_patchByteCode = { ranD214_BULLETCOUNT_CHARGE_byteCode, 141, nullptr, 0 };
// ranD214_BULLETCOUNT_CHARGE_C declared in ran.asm line 1228
static unsigned char ranD214_BULLETCOUNT_CHARGE_C_byteCode[] = {
	0x80, 0xF8, 0x00,                         // CMP AL,0x00 ; line 1229
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,       // JE LABEL0 ; line 1230
	0x80, 0xF8, 0x01,                         // CMP AL,0x01 ; line 1231
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE LABEL1 ; line 1232
	0x80, 0xF8, 0x02,                         // CMP AL,0x02 ; line 1233
	0x0F, 0x84, 0x34, 0x00, 0x00, 0x00,       // JE LABEL2 ; line 1234
	0x80, 0xF8, 0x03,                         // CMP AL,0x03 ; line 1235
	0x0F, 0x84, 0x3C, 0x00, 0x00, 0x00,       // JE LABEL3 ; line 1236
	0x80, 0xF8, 0x04,                         // CMP AL,0x04 ; line 1237
	0x0F, 0x84, 0x44, 0x00, 0x00, 0x00,       // JE LABEL4 ; line 1238
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FLD DWORD PTR [0x85BCA4] ; line 1241
	0xD8, 0x05, 0xA4, 0xBC, 0x85, 0x00,       // FADD DWORD PTR [0x85BCA4] ; line 1242
	0xE9, 0x44, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1243
	0xD9, 0x05, 0x10, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x867810] ; line 1246
	0xD8, 0x05, 0x10, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x867810] ; line 1247
	0xE9, 0x33, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1248
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FLD DWORD PTR [0x86782C] ; line 1251
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 1252
	0xE9, 0x22, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1253
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1256
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00,       // FADD DWORD PTR [0x858830] ; line 1257
	0xE9, 0x11, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1258
	0xDD, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FLD QWORD PTR [0x858EB0] ; line 1261
	0xDC, 0x05, 0xB0, 0x8E, 0x85, 0x00,       // FADD QWORD PTR [0x858EB0] ; line 1262
	0xE9, 0x00, 0x00, 0x00, 0x00,             // JMP LABELEND ; line 1263
	0xD9, 0x5C, 0x24, 0x14,                   // FSTP DWORD PTR [ESP+0x14] ; line 1266
	0x0F, 0xBF, 0x86, 0xD2, 0x07, 0x00, 0x00, // MOVSX EAX,WORD PTR [ESI+0x7D2] ; line 1267
};
static PatchByteCode ranD214_BULLETCOUNT_CHARGE_C_patchByteCode = { ranD214_BULLETCOUNT_CHARGE_C_byteCode, 141, nullptr, 0 };
// ranD214_LOOP declared in ran.asm line 1272
static unsigned char ranD214_LOOP_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1273
};
static PatchByteCode ranD214_LOOP_patchByteCode = { ranD214_LOOP_byteCode, 2, nullptr, 0 };
// ranD214_LOOP_CHARGE declared in ran.asm line 1278
static unsigned char ranD214_LOOP_CHARGE_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1279
};
static PatchByteCode ranD214_LOOP_CHARGE_patchByteCode = { ranD214_LOOP_CHARGE_byteCode, 2, nullptr, 0 };
// ranD623_SOUND declared in ran.asm line 1284
static unsigned char ranD623_SOUND_byteCode[] = {
	0x6A, 0x02,                   // PUSH 0x02 ; line 1285
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1286
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x464980 ; line 1287
};
static JumpTarget ranD623_SOUND_jumpData[] = {
	{ 0x005, 0x464980 },
};
static PatchByteCode ranD623_SOUND_patchByteCode = { ranD623_SOUND_byteCode, 9, ranD623_SOUND_jumpData, 1 };
// ranD623B_LVFIX declared in ran.asm line 1292
static unsigned char ranD623B_LVFIX_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EEC75 ; line 1293
};
static JumpTarget ranD623B_LVFIX_jumpData[] = {
	{ 0x001, 0x4EEC75 },
};
static PatchByteCode ranD623B_LVFIX_patchByteCode = { ranD623B_LVFIX_byteCode, 5, ranD623B_LVFIX_jumpData, 1 };
// ranD623B_LDUR declared in ran.asm line 1297
static unsigned char ranD623B_LDUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1B ; line 1298
};
static PatchByteCode ranD623B_LDUR_patchByteCode = { ranD623B_LDUR_byteCode, 10, nullptr, 0 };
// ranD623B declared in ran.asm line 1302
static unsigned char ranD623B_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 1303
};
static PatchByteCode ranD623B_patchByteCode = { ranD623B_byteCode, 2, nullptr, 0 };
// ranD623B_GRAV2 declared in ran.asm line 1314
static unsigned char ranD623B_GRAV2_byteCode[] = {
	0xD9, 0x9E, 0xDC, 0x07, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x7DC] ; line 1315
	0xD9, 0xEE,                         // FLDZ ; line 1316
	0xD9, 0x9E, 0xDC, 0x07, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x7DC] ; line 1317
};
static PatchByteCode ranD623B_GRAV2_patchByteCode = { ranD623B_GRAV2_byteCode, 14, nullptr, 0 };
// ranD623C_TOB declared in ran.asm line 1321
static unsigned char ranD623C_TOB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EE958 ; line 1322
};
static JumpTarget ranD623C_TOB_jumpData[] = {
	{ 0x001, 0x4EE958 },
};
static PatchByteCode ranD623C_TOB_patchByteCode = { ranD623C_TOB_byteCode, 5, ranD623C_TOB_jumpData, 1 };
// ranDJ623C_TOB declared in ran.asm line 1326
static unsigned char ranDJ623C_TOB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EF0C2 ; line 1327
};
static JumpTarget ranDJ623C_TOB_jumpData[] = {
	{ 0x001, 0x4EF0C2 },
};
static PatchByteCode ranDJ623C_TOB_patchByteCode = { ranDJ623C_TOB_byteCode, 5, ranDJ623C_TOB_jumpData, 1 };
// ranDJ623C_TOB2 declared in ran.asm line 1331
static unsigned char ranDJ623C_TOB2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4FE4A4 ; line 1332
};
static JumpTarget ranDJ623C_TOB2_jumpData[] = {
	{ 0x001, 0x4FE4A4 },
};
static PatchByteCode ranDJ623C_TOB2_patchByteCode = { ranDJ623C_TOB2_byteCode, 5, ranDJ623C_TOB2_jumpData, 1 };
// ranDJ623B_DUR declared in ran.asm line 1336
static unsigned char ranDJ623B_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x09 ; line 1337
};
static PatchByteCode ranDJ623B_DUR_patchByteCode = { ranDJ623B_DUR_byteCode, 10, nullptr, 0 };
// ranDJ623B_HITCOUNT declared in ran.asm line 1341
static unsigned char ranDJ623B_HITCOUNT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 1342
};
static PatchByteCode ranDJ623B_HITCOUNT_patchByteCode = { ranDJ623B_HITCOUNT_byteCode, 7, nullptr, 0 };
// ranDJ623C_HITCOUNT declared in ran.asm line 1346
static unsigned char ranDJ623C_HITCOUNT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 1347
};
static PatchByteCode ranDJ623C_HITCOUNT_patchByteCode = { ranDJ623C_HITCOUNT_byteCode, 7, nullptr, 0 };
// ranDJ623B_GRAVFIX declared in ran.asm line 1351
static unsigned char ranDJ623B_GRAVFIX_byteCode[] = {
	0xDD, 0xD9, // FSTP ST1 ; line 1352
};
static PatchByteCode ranDJ623B_GRAVFIX_patchByteCode = { ranDJ623B_GRAVFIX_byteCode, 2, nullptr, 0 };
// ranDJ623B_SKIPMOMENTUM declared in ran.asm line 1356
static unsigned char ranDJ623B_SKIPMOMENTUM_byteCode[] = {
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x0003 ; line 1357
	0x0F, 0x8D, 0x90, 0x00, 0x00, 0x00,                         // JGE NEXT ; line 1358
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 1360
	0x0F, 0x8D, 0x1E, 0x00, 0x00, 0x00,                         // JGE LABEL2 ; line 1361
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0000 ; line 1362
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1363
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1364
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 1365
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 1369
	0x0F, 0x8D, 0x52, 0x00, 0x00, 0x00,                         // JGE NEXT ; line 1370
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x00 ; line 1371
	0x0F, 0x8F, 0x3C, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 1372
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1373
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x104],0x0 ; line 1374
	0x0F, 0x8F, 0x0B, 0x00, 0x00, 0x00,                         // JG FACING ; line 1375
	0xD8, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FADD DWORD PTR [ESI+0xF4] ; line 1376
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP NEXT1 ; line 1377
	0xD8, 0xA6, 0xF4, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF4] ; line 1379
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1381
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 1382
	0xD8, 0xA6, 0xF8, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF8] ; line 1383
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 1384
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1386
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EF16A ; line 1387
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EF156 ; line 1389
};
static JumpTarget ranDJ623B_SKIPMOMENTUM_jumpData[] = {
	{ 0x097, 0x4EF16A },
	{ 0x09C, 0x4EF156 },
};
static PatchByteCode ranDJ623B_SKIPMOMENTUM_patchByteCode = { ranDJ623B_SKIPMOMENTUM_byteCode, 160, ranDJ623B_SKIPMOMENTUM_jumpData, 2 };
// ranFLD1 declared in ran.asm line 1393
static unsigned char ranFLD1_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1394
};
static PatchByteCode ranFLD1_patchByteCode = { ranFLD1_byteCode, 2, nullptr, 0 };
// ranJD236_MMNTM declared in ran.asm line 1398
static unsigned char ranJD236_MMNTM_byteCode[] = {
	0xDD, 0xD9, // FSTP ST1 ; line 1399
};
static PatchByteCode ranJD236_MMNTM_patchByteCode = { ranJD236_MMNTM_byteCode, 2, nullptr, 0 };
// ranD236_X2 declared in ran.asm line 1405
static unsigned char ranD236_X2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00,       // CMP WORD PTR [ESI+0x140],0x07 ; line 1406
	0x0F, 0x8C, 0x2F, 0x00, 0x00, 0x00,                         // JL LABEL1 ; line 1407
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1408
	0xD8, 0x05, 0xA8, 0x77, 0x86, 0x00,                         // FADD DWORD PTR [0x8677A8] ; line 1409
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1410
	0xD9, 0xEE,                                                 // FLDZ ; line 1412
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1413
	0xDE, 0xD9,                                                 // FCOMPP ; line 1414
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 1415
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 1416
	0x0F, 0x85, 0x08, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1417
	0xD9, 0xEE,                                                 // FLDZ ; line 1418
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1419
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1422
};
static PatchByteCode ranD236_X2_patchByteCode = { ranD236_X2_byteCode, 72, nullptr, 0 };
// ranD236C declared in ran.asm line 1426
static unsigned char ranD236C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F11C9 ; line 1427
};
static JumpTarget ranD236C_jumpData[] = {
	{ 0x001, 0x4F11C9 },
};
static PatchByteCode ranD236C_patchByteCode = { ranD236C_byteCode, 5, ranD236C_jumpData, 1 };
// ranJD236C declared in ran.asm line 1431
static unsigned char ranJD236C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4F1DAD ; line 1432
};
static JumpTarget ranJD236C_jumpData[] = {
	{ 0x001, 0x4F1DAD },
};
static PatchByteCode ranJD236C_patchByteCode = { ranJD236C_byteCode, 5, ranJD236C_jumpData, 1 };
// ranD236_X declared in ran.asm line 1436
static unsigned char ranD236_X_byteCode[] = {
	0xD9, 0xEE,                                           // FLDZ ; line 1437
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x1C, 0x02, // CMP WORD PTR [ESI+0x13C],0x021C ; line 1438
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE LABEL1 ; line 1439
	0xD8, 0x25, 0x10, 0xC6, 0x85, 0x00,                   // FSUB DWORD PTR [0x85C610] ; line 1440
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP LABEL2 ; line 1441
	0xD8, 0x25, 0x30, 0xC2, 0x85, 0x00,                   // FSUB DWORD PTR [0x85C230] ; line 1443
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 1445
	0xD9, 0xEE,                                           // FLDZ ; line 1446
	0x89, 0x9E, 0x90, 0x01, 0x00, 0x00,                   // MOV [ESI+0x190],EBX ; line 1447
};
static PatchByteCode ranD236_X_patchByteCode = { ranD236_X_byteCode, 48, nullptr, 0 };
// ranD236B_ANG1 declared in ran.asm line 1451
static unsigned char ranD236B_ANG1_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 1452
};
static PatchByteCode ranD236B_ANG1_patchByteCode = { ranD236B_ANG1_byteCode, 2, nullptr, 0 };
// ranD236B_ANG2 declared in ran.asm line 1454
static unsigned char ranD236B_ANG2_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1455
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1456
};
static PatchByteCode ranD236B_ANG2_patchByteCode = { ranD236B_ANG2_byteCode, 8, nullptr, 0 };
// ranD236B_ANG3 declared in ran.asm line 1458
static unsigned char ranD236B_ANG3_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1459
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1460
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1461
};
static PatchByteCode ranD236B_ANG3_patchByteCode = { ranD236B_ANG3_byteCode, 14, nullptr, 0 };
// ranD236B_ANG4 declared in ran.asm line 1463
static unsigned char ranD236B_ANG4_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1464
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1465
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1466
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 1467
};
static PatchByteCode ranD236B_ANG4_patchByteCode = { ranD236B_ANG4_byteCode, 20, nullptr, 0 };
// ranD236B_ANG5 declared in ran.asm line 1469
static unsigned char ranD236B_ANG5_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 1470
};
static PatchByteCode ranD236B_ANG5_patchByteCode = { ranD236B_ANG5_byteCode, 2, nullptr, 0 };
// ranD236B_ANG6 declared in ran.asm line 1472
static unsigned char ranD236B_ANG6_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1473
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1474
};
static PatchByteCode ranD236B_ANG6_patchByteCode = { ranD236B_ANG6_byteCode, 8, nullptr, 0 };
// ranD236B_ANG7 declared in ran.asm line 1476
static unsigned char ranD236B_ANG7_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1477
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1478
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1479
};
static PatchByteCode ranD236B_ANG7_patchByteCode = { ranD236B_ANG7_byteCode, 14, nullptr, 0 };
// ranD236B_ANG8 declared in ran.asm line 1481
static unsigned char ranD236B_ANG8_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1482
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1483
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1484
	0xD8, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FADD DWORD PTR [0x85B3B0] ; line 1485
};
static PatchByteCode ranD236B_ANG8_patchByteCode = { ranD236B_ANG8_byteCode, 20, nullptr, 0 };
// ranD236_LEVELS declared in ran.asm line 1516
static unsigned char ranD236_LEVELS_byteCode[] = {
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x06, // CMP BYTE PTR [ESI+0x6A6],0x06 ; line 1517
};
static PatchByteCode ranD236_LEVELS_patchByteCode = { ranD236_LEVELS_byteCode, 7, nullptr, 0 };
// ranD236B_SPEED declared in ran.asm line 1524
static unsigned char ranD236B_SPEED_byteCode[] = {
	0xD9, 0x05, 0x6C, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BC6C] ; line 1525
	0xD9, 0x9E, 0xDC, 0x07, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x7DC] ; line 1526
};
static PatchByteCode ranD236B_SPEED_patchByteCode = { ranD236B_SPEED_byteCode, 12, nullptr, 0 };
// ranD236B_SIZE declared in ran.asm line 1531
static unsigned char ranD236B_SIZE_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 1532
	0x0F, 0x8F, 0x0E, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 1533
	0xD9, 0xEE,                                                 // FLDZ ; line 1534
	0xD9, 0x96, 0x1C, 0x01, 0x00, 0x00,                         // FST DWORD PTR [ESI+0x11C] ; line 1535
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 1536
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x06 ; line 1538
	0x0F, 0x8F, 0x1D, 0x00, 0x00, 0x00,                         // JG LABEL2 ; line 1539
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 1540
	0xD8, 0x05, 0xE8, 0xBA, 0x85, 0x00,                         // FADD DWORD PTR [0x85BAE8] ; line 1541
	0xD9, 0x96, 0x1C, 0x01, 0x00, 0x00,                         // FST DWORD PTR [ESI+0x11C] ; line 1542
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 1543
	0xE9, 0x36, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 1544
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x104],0x01 ; line 1546
	0x0F, 0x84, 0x17, 0x00, 0x00, 0x00,                         // JE LABEL4 ; line 1547
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1548
	0xD8, 0x05, 0x10, 0xC6, 0x85, 0x00,                         // FADD DWORD PTR [0x85C610] ; line 1549
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1550
	0xE9, 0x12, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 1551
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1553
	0xD8, 0x25, 0x10, 0xC6, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C610] ; line 1554
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1555
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 1557
};
static PatchByteCode ranD236B_SIZE_patchByteCode = { ranD236B_SIZE_byteCode, 139, nullptr, 0 };
// ranD236B_TRAIL declared in ran.asm line 1561
static unsigned char ranD236B_TRAIL_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1562
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00, // FST DWORD PTR [ESI+0xF4] ; line 1563
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 1564
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x503DE6 ; line 1565
};
static JumpTarget ranD236B_TRAIL_jumpData[] = {
	{ 0x00F, 0x503DE6 },
};
static PatchByteCode ranD236B_TRAIL_patchByteCode = { ranD236B_TRAIL_byteCode, 19, ranD236B_TRAIL_jumpData, 1 };
// ranJ8A_Y declared in ran.asm line 1569
static unsigned char ranJ8A_Y_byteCode[] = {
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B9D4] ; line 1570
};
static PatchByteCode ranJ8A_Y_patchByteCode = { ranJ8A_Y_byteCode, 6, nullptr, 0 };
// ran3A_X declared in ran.asm line 1574
static unsigned char ran3A_X_byteCode[] = {
	0xD8, 0x25, 0x28, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x867828] ; line 1575
};
static PatchByteCode ran3A_X_patchByteCode = { ran3A_X_byteCode, 6, nullptr, 0 };
// ran3A_SOUND1 declared in ran.asm line 1583
static unsigned char ran3A_SOUND1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 1584
};
static PatchByteCode ran3A_SOUND1_patchByteCode = { ran3A_SOUND1_byteCode, 9, nullptr, 0 };
// ran3A_X2 declared in ran.asm line 1589
static unsigned char ran3A_X2_byteCode[] = {
	0xDC, 0x1D, 0xB0, 0x77, 0x86, 0x00, // FCOMP QWORD PTR [0x8677B0] ; line 1590
};
static PatchByteCode ran3A_X2_patchByteCode = { ran3A_X2_byteCode, 6, nullptr, 0 };
// ran3A_NOY1 declared in ran.asm line 1605
static unsigned char ran3A_NOY1_byteCode[] = {
	0x0F, 0x84, 0x19, 0x00, 0x00, 0x00, // JE LABELNEXT ; line 1606
	0x89, 0xF1,                         // MOV ECX,ESI ; line 1607
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x4397F0 ; line 1608
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF0] ; line 1609
	0x89, 0xF1,                         // MOV ECX,ESI ; line 1610
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x4636B0 ; line 1611
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x4EA966 ; line 1612
};
static JumpTarget ran3A_NOY1_jumpData[] = {
	{ 0x009, 0x4397F0 },
	{ 0x016, 0x4636B0 },
	{ 0x01B, 0x4EA966 },
};
static PatchByteCode ran3A_NOY1_patchByteCode = { ran3A_NOY1_byteCode, 31, ran3A_NOY1_jumpData, 3 };
// ran3A_NOY21 declared in ran.asm line 1617
static unsigned char ran3A_NOY21_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1618
	0xD9, 0x96, 0xF8, 0x00, 0x00, 0x00, // FST DWORD PTR [ESI+0xF8] ; line 1619
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x100] ; line 1620
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x4EAD19 ; line 1621
};
static JumpTarget ran3A_NOY21_jumpData[] = {
	{ 0x00F, 0x4EAD19 },
};
static PatchByteCode ran3A_NOY21_patchByteCode = { ran3A_NOY21_byteCode, 19, ran3A_NOY21_jumpData, 1 };
// ran3A_NOY22 declared in ran.asm line 1624
static unsigned char ran3A_NOY22_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1625
	0xD9, 0x96, 0xF8, 0x00, 0x00, 0x00, // FST DWORD PTR [ESI+0xF8] ; line 1626
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x100] ; line 1627
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x4EAD9A ; line 1628
};
static JumpTarget ran3A_NOY22_jumpData[] = {
	{ 0x00F, 0x4EAD9A },
};
static PatchByteCode ran3A_NOY22_patchByteCode = { ran3A_NOY22_byteCode, 19, ran3A_NOY22_jumpData, 1 };
// ranJ6A_SKIPMOMENTUM declared in ran.asm line 1634
static unsigned char ranJ6A_SKIPMOMENTUM_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 1635
	0x0F, 0x8D, 0x50, 0x00, 0x00, 0x00,                         // JGE NEXT ; line 1636
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x00 ; line 1637
	0x0F, 0x8F, 0x3C, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 1638
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1639
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x104],0x0 ; line 1640
	0x0F, 0x8F, 0x0B, 0x00, 0x00, 0x00,                         // JG FACING ; line 1641
	0xD8, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FADD DWORD PTR [ESI+0xF4] ; line 1642
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP NEXT1 ; line 1643
	0xD8, 0xA6, 0xF4, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF4] ; line 1645
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1647
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 1648
	0xD8, 0xA6, 0xF8, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF8] ; line 1649
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 1650
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EB0E4 ; line 1652
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1654
};
static JumpTarget ranJ6A_SKIPMOMENTUM_jumpData[] = {
	{ 0x05C, 0x4EB0E4 },
};
static PatchByteCode ranJ6A_SKIPMOMENTUM_patchByteCode = { ranJ6A_SKIPMOMENTUM_byteCode, 102, ranJ6A_SKIPMOMENTUM_jumpData, 1 };
// ranJ2A_SKIPMOMENTUM declared in ran.asm line 1658
static unsigned char ranJ2A_SKIPMOMENTUM_byteCode[] = {
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1659
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 1660
	0x0F, 0x8D, 0x50, 0x00, 0x00, 0x00,                         // JGE NEXT ; line 1661
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x00 ; line 1662
	0x0F, 0x8F, 0x3C, 0x00, 0x00, 0x00,                         // JG LABEL1 ; line 1663
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 1664
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x104],0x0 ; line 1665
	0x0F, 0x8F, 0x0B, 0x00, 0x00, 0x00,                         // JG FACING ; line 1666
	0xD8, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FADD DWORD PTR [ESI+0xF4] ; line 1667
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP NEXT1 ; line 1668
	0xD8, 0xA6, 0xF4, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF4] ; line 1670
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1672
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 1673
	0xD8, 0xA6, 0xF8, 0x00, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0xF8] ; line 1674
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 1675
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EB1AE ; line 1677
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1679
};
static JumpTarget ranJ2A_SKIPMOMENTUM_jumpData[] = {
	{ 0x05E, 0x4EB1AE },
};
static PatchByteCode ranJ2A_SKIPMOMENTUM_patchByteCode = { ranJ2A_SKIPMOMENTUM_byteCode, 104, ranJ2A_SKIPMOMENTUM_jumpData, 1 };
// ranJ2A_MOMENTUM declared in ran.asm line 1683
static unsigned char ranJ2A_MOMENTUM_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1684
};
static PatchByteCode ranJ2A_MOMENTUM_patchByteCode = { ranJ2A_MOMENTUM_byteCode, 2, nullptr, 0 };
// ranJ2A_NOP declared in ran.asm line 1688
static unsigned char ranJ2A_NOP_byteCode[] = {
	0x90, // NOP ; line 1689
};
static PatchByteCode ranJ2A_NOP_patchByteCode = { ranJ2A_NOP_byteCode, 1, nullptr, 0 };
// ranTO411_2 declared in ran.asm line 1705
static unsigned char ranTO411_2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4FE304 ; line 1706
};
static JumpTarget ranTO411_2_jumpData[] = {
	{ 0x001, 0x4FE304 },
};
static PatchByteCode ranTO411_2_patchByteCode = { ranTO411_2_byteCode, 5, ranTO411_2_jumpData, 1 };
// ranTO411 declared in ran.asm line 1711
static unsigned char ranTO411_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4ED91D ; line 1712
};
static JumpTarget ranTO411_jumpData[] = {
	{ 0x001, 0x4ED91D },
};
static PatchByteCode ranTO411_patchByteCode = { ranTO411_byteCode, 5, ranTO411_jumpData, 1 };
// ran6C_ARCMUL declared in ran.asm line 1717
static unsigned char ran6C_ARCMUL_byteCode[] = {
	0xDC, 0x0D, 0xF8, 0x81, 0x85, 0x00,                   // FMUL QWORD PTR [0x8581F8] ; line 1718
	0xDC, 0x25, 0x90, 0x89, 0x85, 0x00,                   // FSUB QWORD PTR [0x858990] ; line 1719
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9A, 0x01, // CMP WORD PTR [ESI+0x13C],0x019A ; line 1722
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE LABEL5C ; line 1723
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9B, 0x01, // CMP WORD PTR [ESI+0x13C],0x019B ; line 1725
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                   // JE LABEL6C ; line 1726
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9C, 0x01, // CMP WORD PTR [ESI+0x13C],0x019C ; line 1728
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,                   // JE LABEL2C ; line 1729
	0xE9, 0x10, 0x00, 0x00, 0x00,                         // JMP END ; line 1732
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1734
	0xDC, 0x25, 0x90, 0x89, 0x85, 0x00,                   // FSUB QWORD PTR [0x858990] ; line 1736
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1737
};
static PatchByteCode ran6C_ARCMUL_patchByteCode = { ran6C_ARCMUL_byteCode, 78, nullptr, 0 };
// ran6C_ARCSPD declared in ran.asm line 1742
static unsigned char ran6C_ARCSPD_byteCode[] = {
	0xDD, 0xD8,                         // FSTP ST0 ; line 1743
	0xDD, 0x05, 0xF8, 0x81, 0x85, 0x00, // FLD QWORD PTR [0x8581F8] ; line 1744
};
static PatchByteCode ran6C_ARCSPD_patchByteCode = { ran6C_ARCSPD_byteCode, 8, nullptr, 0 };
// ran6CH_ARCMUL declared in ran.asm line 1748
static unsigned char ran6CH_ARCMUL_byteCode[] = {
	0xDC, 0x0D, 0xF8, 0x81, 0x85, 0x00,                   // FMUL QWORD PTR [0x8581F8] ; line 1749
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9A, 0x01, // CMP WORD PTR [ESI+0x13C],0x019A ; line 1752
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE LABEL5C ; line 1753
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9B, 0x01, // CMP WORD PTR [ESI+0x13C],0x019B ; line 1755
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                   // JE LABEL6C ; line 1756
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9C, 0x01, // CMP WORD PTR [ESI+0x13C],0x019C ; line 1758
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,                   // JE LABEL2C ; line 1759
	0xE9, 0x10, 0x00, 0x00, 0x00,                         // JMP END ; line 1762
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1764
	0xDC, 0x25, 0x90, 0x89, 0x85, 0x00,                   // FSUB QWORD PTR [0x858990] ; line 1766
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1767
	0xD9, 0x9C, 0x24, 0x4C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x54C] ; line 1769
};
static PatchByteCode ran6CH_ARCMUL_patchByteCode = { ran6CH_ARCMUL_byteCode, 79, nullptr, 0 };
// ran6CH_ARCSPD declared in ran.asm line 1773
static unsigned char ran6CH_ARCSPD_byteCode[] = {
	0xDD, 0xD8,                         // FSTP ST0 ; line 1774
	0xDD, 0x05, 0xF8, 0x81, 0x85, 0x00, // FLD QWORD PTR [0x8581F8] ; line 1775
};
static PatchByteCode ran6CH_ARCSPD_patchByteCode = { ran6CH_ARCSPD_byteCode, 8, nullptr, 0 };
// ranB_ACCEL declared in ran.asm line 1779
static unsigned char ranB_ACCEL_byteCode[] = {
	0xDD, 0x05, 0x70, 0xBA, 0x85, 0x00, // FLD QWORD PTR [0x85BA70] ; line 1780
};
static PatchByteCode ranB_ACCEL_patchByteCode = { ranB_ACCEL_byteCode, 6, nullptr, 0 };
// ranB_STARTSPD declared in ran.asm line 1784
static unsigned char ranB_STARTSPD_byteCode[] = {
	0xD9, 0x05, 0xB8, 0xBD, 0x85, 0x00, // FLD DWORD PTR [0x85BDB8] ; line 1785
};
static PatchByteCode ranB_STARTSPD_patchByteCode = { ranB_STARTSPD_byteCode, 6, nullptr, 0 };
// ranJB_NOCHARGE declared in ran.asm line 1789
static unsigned char ranJB_NOCHARGE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x75, 0x00, // CMP WORD PTR [ESI+0x140],0x75 ; line 1790
};
static PatchByteCode ranJB_NOCHARGE_patchByteCode = { ranJB_NOCHARGE_byteCode, 9, nullptr, 0 };
// ranJB_STARTSPD declared in ran.asm line 1795
static unsigned char ranJB_STARTSPD_byteCode[] = {
	0xD9, 0x05, 0xB8, 0xBD, 0x85, 0x00, // FLD DWORD PTR [0x85BDB8] ; line 1796
};
static PatchByteCode ranJB_STARTSPD_patchByteCode = { ranJB_STARTSPD_byteCode, 6, nullptr, 0 };
// ranADDJ6B declared in ran.asm line 1806
static unsigned char ranADDJ6B_byteCode[] = {
	0x66, 0x39, 0xC7,                   // CMP DI,AX ; line 1807
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x501694 ; line 1808
	0xD9, 0xEE,                         // FLDZ ; line 1809
	0x8B, 0x06,                         // MOV EAX,[ESI] ; line 1810
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 1811
	0x68, 0x95, 0x01, 0x00, 0x00,       // PUSH 0x00000195 ; line 1812
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x501033 ; line 1813
};
static JumpTarget ranADDJ6B_jumpData[] = {
	{ 0x005, 0x501694 },
	{ 0x019, 0x501033 },
};
static PatchByteCode ranADDJ6B_patchByteCode = { ranADDJ6B_byteCode, 29, ranADDJ6B_jumpData, 2 };
// ranB_END declared in ran.asm line 1817
static unsigned char ranB_END_byteCode[] = {
	0x66, 0xC7, 0x86, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [ESI+0x196],0x00 ; line 1819
	0x8B, 0x0E,                                           // MOV ECX,[ESI] ; line 1821
	0x8B, 0x41, 0x08,                                     // MOV EAX,[ECX+0x08] ; line 1822
	0x6A, 0x09,                                           // PUSH 0x09 ; line 1823
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1824
	0xFF, 0xD0,                                           // CALL EAX ; line 1825
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4F4C7D ; line 1826
};
static JumpTarget ranB_END_jumpData[] = {
	{ 0x015, 0x4F4C7D },
};
static PatchByteCode ranB_END_patchByteCode = { ranB_END_byteCode, 25, ranB_END_jumpData, 1 };
// ranB_XY declared in ran.asm line 1830
static unsigned char ranB_XY_byteCode[] = {
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1832
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0A ; line 1833
	0x0F, 0x8C, 0xA7, 0x00, 0x00, 0x00,                         // JL LABELPASS ; line 1834
	0x0F, 0x8F, 0x8A, 0x00, 0x00, 0x00,                         // JG LABELOVER ; line 1835
	0xD9, 0x05, 0x68, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x867868] ; line 1837
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 1838
	0xD9, 0x05, 0x60, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC60] ; line 1840
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1841
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x90, 0x01,       // CMP WORD PTR [ESI+0x13C],0x0190 ; line 1843
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                         // JE LABELB ; line 1844
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x91, 0x01,       // CMP WORD PTR [ESI+0x13C],0x0191 ; line 1845
	0x0F, 0x84, 0x22, 0x00, 0x00, 0x00,                         // JE LABEL6B ; line 1846
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x92, 0x01,       // CMP WORD PTR [ESI+0x13C],0x0192 ; line 1847
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                         // JE LABEL2B ; line 1848
	0xD9, 0xEE,                                                 // FLDZ ; line 1851
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00,                         // FSUB DWORD PTR [0x86782C] ; line 1852
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1853
	0xE9, 0x32, 0x00, 0x00, 0x00,                               // JMP END ; line 1854
	0xD9, 0xEE,                                                 // FLDZ ; line 1857
	0xD8, 0x05, 0x60, 0xBC, 0x85, 0x00,                         // FADD DWORD PTR [0x85BC60] ; line 1858
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1859
	0xE9, 0x1F, 0x00, 0x00, 0x00,                               // JMP END ; line 1860
	0xD9, 0xEE,                                                 // FLDZ ; line 1863
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1864
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1865
	0xD8, 0x05, 0x70, 0xB8, 0x85, 0x00,                         // FADD DWORD PTR [0x85B870] ; line 1866
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1867
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP END ; line 1868
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 1872
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0x100] ; line 1873
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1874
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EB846 ; line 1876
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487CA0 ; line 1882
};
static JumpTarget ranB_XY_jumpData[] = {
	{ 0x0B5, 0x4EB846 },
	{ 0x0BA, 0x487CA0 },
};
static PatchByteCode ranB_XY_patchByteCode = { ranB_XY_byteCode, 190, ranB_XY_jumpData, 2 };
// ranB_OFFX declared in ran.asm line 1886
static unsigned char ranB_OFFX_byteCode[] = {
	0xDC, 0x0D, 0x38, 0x79, 0x85, 0x00, // FMUL QWORD PTR [0x857938] ; line 1887
};
static PatchByteCode ranB_OFFX_patchByteCode = { ranB_OFFX_byteCode, 6, nullptr, 0 };
// ranB_OFFY declared in ran.asm line 1891
static unsigned char ranB_OFFY_byteCode[] = {
	0xDC, 0x05, 0x20, 0x7B, 0x85, 0x00, // FADD QWORD PTR [0x857B20] ; line 1892
};
static PatchByteCode ranB_OFFY_patchByteCode = { ranB_OFFY_byteCode, 6, nullptr, 0 };
// ranTO400 declared in ran.asm line 1896
static unsigned char ranTO400_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EB83F ; line 1897
};
static JumpTarget ranTO400_jumpData[] = {
	{ 0x001, 0x4EB83F },
};
static PatchByteCode ranTO400_patchByteCode = { ranTO400_byteCode, 5, ranTO400_jumpData, 1 };
// ranB_FIREPERIOD declared in ran.asm line 1909
static unsigned char ranB_FIREPERIOD_byteCode[] = {
	0xB9, 0x06, 0x00, 0x00, 0x00, // MOV ECX,0x00000006 ; line 1910
};
static PatchByteCode ranB_FIREPERIOD_patchByteCode = { ranB_FIREPERIOD_byteCode, 5, nullptr, 0 };
// ranB_FIRELENGTH declared in ran.asm line 1914
static unsigned char ranB_FIRELENGTH_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD4, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D4],0x04 ; line 1915
};
static PatchByteCode ranB_FIRELENGTH_patchByteCode = { ranB_FIRELENGTH_byteCode, 9, nullptr, 0 };
// ranB_FIRENUM declared in ran.asm line 1919
static unsigned char ranB_FIRENUM_byteCode[] = {
	0x81, 0xC7, 0x0A, 0x00, 0x00, 0x00, // ADD EDI,0x0A ; line 1920
	0x81, 0xFF, 0x14, 0x00, 0x00, 0x00, // CMP EDI,0x14 ; line 1921
};
static PatchByteCode ranB_FIRENUM_patchByteCode = { ranB_FIRENUM_byteCode, 12, nullptr, 0 };
// ranJB_FIREANG declared in ran.asm line 1925
static unsigned char ranJB_FIREANG_byteCode[] = {
	0x8D, 0x84, 0x24, 0x3C, 0x02, 0x00, 0x00, // LEA EAX,[ESP+0x23C] ; line 1926
	0xDA, 0x44, 0x24, 0x1C,                   // FIADD DWORD PTR [ESP+0x1C] ; line 1927
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x4EC52D ; line 1928
};
static JumpTarget ranJB_FIREANG_jumpData[] = {
	{ 0x00C, 0x4EC52D },
};
static PatchByteCode ranJB_FIREANG_patchByteCode = { ranJB_FIREANG_byteCode, 16, ranJB_FIREANG_jumpData, 1 };
// ranJB_FIREPERIOD declared in ran.asm line 1932
static unsigned char ranJB_FIREPERIOD_byteCode[] = {
	0xB9, 0x06, 0x00, 0x00, 0x00, // MOV ECX,0x00000006 ; line 1933
};
static PatchByteCode ranJB_FIREPERIOD_patchByteCode = { ranJB_FIREPERIOD_byteCode, 5, nullptr, 0 };
// ranJB_FIRELENGTH declared in ran.asm line 1937
static unsigned char ranJB_FIRELENGTH_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD4, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D4],0x04 ; line 1938
};
static PatchByteCode ranJB_FIRELENGTH_patchByteCode = { ranJB_FIRELENGTH_byteCode, 9, nullptr, 0 };
// ranJB_FIRENUM declared in ran.asm line 1942
static unsigned char ranJB_FIRENUM_byteCode[] = {
	0x81, 0xC7, 0x0A, 0x00, 0x00, 0x00, // ADD EDI,0x0A ; line 1943
	0x81, 0xFF, 0x14, 0x00, 0x00, 0x00, // CMP EDI,0x14 ; line 1944
};
static PatchByteCode ranJB_FIRENUM_patchByteCode = { ranJB_FIRENUM_byteCode, 12, nullptr, 0 };
// ranJB_X declared in ran.asm line 1948
static unsigned char ranJB_X_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x4EC8FF ; line 1949
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x94, 0x01, // CMP WORD PTR [ESI+0x13C],0x0194 ; line 1950
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE LABELJB ; line 1951
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x95, 0x01, // CMP WORD PTR [ESI+0x13C],0x0195 ; line 1952
	0x0F, 0x84, 0x1C, 0x00, 0x00, 0x00,                   // JE LABELJ6B ; line 1953
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x96, 0x01, // CMP WORD PTR [ESI+0x13C],0x0196 ; line 1954
	0x0F, 0x84, 0x1A, 0x00, 0x00, 0x00,                   // JE LABELJ2B ; line 1955
	0xD9, 0xEE,                                           // FLDZ ; line 1958
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00,                   // FSUB DWORD PTR [0x86782C] ; line 1959
	0xE9, 0x1A, 0x00, 0x00, 0x00,                         // JMP END ; line 1960
	0xD9, 0xEE,                                           // FLDZ ; line 1963
	0xD8, 0x05, 0x60, 0xBC, 0x85, 0x00,                   // FADD DWORD PTR [0x85BC60] ; line 1964
	0xE9, 0x0D, 0x00, 0x00, 0x00,                         // JMP END ; line 1965
	0xD9, 0xEE,                                           // FLDZ ; line 1968
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,                   // FADD DWORD PTR [0x86782C] ; line 1969
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1970
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4EC833 ; line 1973
};
static JumpTarget ranJB_X_jumpData[] = {
	{ 0x002, 0x4EC8FF },
	{ 0x05B, 0x4EC833 },
};
static PatchByteCode ranJB_X_patchByteCode = { ranJB_X_byteCode, 95, ranJB_X_jumpData, 2 };
// ranJB_Y declared in ran.asm line 1977
static unsigned char ranJB_Y_byteCode[] = {
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1978
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x94, 0x01, // CMP WORD PTR [ESI+0x13C],0x0194 ; line 1979
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE LABELJB ; line 1980
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x95, 0x01, // CMP WORD PTR [ESI+0x13C],0x0195 ; line 1981
	0x0F, 0x84, 0x1A, 0x00, 0x00, 0x00,                   // JE LABELJ6B ; line 1982
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x96, 0x01, // CMP WORD PTR [ESI+0x13C],0x0196 ; line 1983
	0x0F, 0x84, 0x16, 0x00, 0x00, 0x00,                   // JE LABELJ2B ; line 1984
	0xD9, 0x05, 0x60, 0xBC, 0x85, 0x00,                   // FLD DWORD PTR [0x85BC60] ; line 1987
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1988
	0xD9, 0x05, 0x60, 0xBC, 0x85, 0x00,                   // FLD DWORD PTR [0x85BC60] ; line 1991
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1992
	0xD9, 0x05, 0x70, 0xBC, 0x85, 0x00,                   // FLD DWORD PTR [0x85BC70] ; line 1995
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1996
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1999
};
static PatchByteCode ranJB_Y_patchByteCode = { ranJB_Y_byteCode, 90, nullptr, 0 };
// ranGO404 declared in ran.asm line 2003
static unsigned char ranGO404_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EC468 ; line 2004
};
static JumpTarget ranGO404_jumpData[] = {
	{ 0x001, 0x4EC468 },
};
static PatchByteCode ranGO404_patchByteCode = { ranGO404_byteCode, 5, ranGO404_jumpData, 1 };
// ranJC_NOBOUNCE1 declared in ran.asm line 2009
static unsigned char ranJC_NOBOUNCE1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x502AB6 ; line 2010
};
static JumpTarget ranJC_NOBOUNCE1_jumpData[] = {
	{ 0x001, 0x502AB6 },
};
static PatchByteCode ranJC_NOBOUNCE1_patchByteCode = { ranJC_NOBOUNCE1_byteCode, 5, ranJC_NOBOUNCE1_jumpData, 1 };
// ranJC_NOBOUNCE2 declared in ran.asm line 2013
static unsigned char ranJC_NOBOUNCE2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x502813 ; line 2014
};
static JumpTarget ranJC_NOBOUNCE2_jumpData[] = {
	{ 0x001, 0x502813 },
};
static PatchByteCode ranJC_NOBOUNCE2_patchByteCode = { ranJC_NOBOUNCE2_byteCode, 5, ranJC_NOBOUNCE2_jumpData, 1 };
// ranJC_XY declared in ran.asm line 2018
static unsigned char ranJC_XY_byteCode[] = {
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 2020
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 2021
	0x0F, 0x85, 0x99, 0x00, 0x00, 0x00,                         // JNE STANDARD ; line 2022
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x11, 0x00,       // CMP WORD PTR [ESI+0x140],0x11 ; line 2023
	0x0F, 0x8F, 0x8A, 0x00, 0x00, 0x00,                         // JG STANDARD ; line 2024
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0A ; line 2025
	0x0F, 0x8D, 0x07, 0x00, 0x00, 0x00,                         // JGE NEXT ; line 2026
	0xD9, 0xEE,                                                 // FLDZ ; line 2027
	0xE9, 0x6E, 0x00, 0x00, 0x00,                               // JMP END ; line 2028
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9E, 0x01,       // CMP WORD PTR [ESI+0x13C],0x019E ; line 2030
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                         // JE LABELJC ; line 2031
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x9F, 0x01,       // CMP WORD PTR [ESI+0x13C],0x019F ; line 2032
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,                         // JE LABELJ6C ; line 2033
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0xA0, 0x01,       // CMP WORD PTR [ESI+0x13C],0x01A0 ; line 2034
	0x0F, 0x84, 0x28, 0x00, 0x00, 0x00,                         // JE LABELJ2C ; line 2035
	0xD9, 0xEE,                                                 // FLDZ ; line 2038
	0xD8, 0x25, 0x94, 0xC8, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C894] ; line 2039
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2040
	0xD9, 0xEE,                                                 // FLDZ ; line 2042
	0xE9, 0x2C, 0x00, 0x00, 0x00,                               // JMP END ; line 2043
	0xD9, 0x05, 0x94, 0xC8, 0x85, 0x00,                         // FLD DWORD PTR [0x85C894] ; line 2046
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2047
	0xD9, 0xEE,                                                 // FLDZ ; line 2049
	0xE9, 0x19, 0x00, 0x00, 0x00,                               // JMP END ; line 2051
	0xD9, 0x05, 0x90, 0xC5, 0x85, 0x00,                         // FLD DWORD PTR [0x85C590] ; line 2053
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2054
	0xD9, 0xEE,                                                 // FLDZ ; line 2056
	0xD8, 0x25, 0xB8, 0xC5, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C5B8] ; line 2057
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP END ; line 2058
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4EDE35 ; line 2061
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 2064
};
static JumpTarget ranJC_XY_jumpData[] = {
	{ 0x0A6, 0x4EDE35 },
};
static PatchByteCode ranJC_XY_patchByteCode = { ranJC_XY_byteCode, 176, ranJC_XY_jumpData, 1 };
// ranJC_ANG declared in ran.asm line 2068
static unsigned char ranJC_ANG_byteCode[] = {
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00, // FADD DWORD PTR [0x858830] ; line 2069
};
static PatchByteCode ranJC_ANG_patchByteCode = { ranJC_ANG_byteCode, 6, nullptr, 0 };
// ranJC_ANG2 declared in ran.asm line 2073
static unsigned char ranJC_ANG2_byteCode[] = {
	0xDD, 0xD8,                         // FSTP ST0 ; line 2074
	0xD9, 0xEE,                         // FLDZ ; line 2075
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00, // FADD DWORD PTR [0x858830] ; line 2076
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00, // FADD DWORD PTR [0x858830] ; line 2077
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00, // FADD DWORD PTR [0x858830] ; line 2078
	0xD8, 0x05, 0x30, 0x88, 0x85, 0x00, // FADD DWORD PTR [0x858830] ; line 2079
	0xD8, 0xA6, 0xDC, 0x07, 0x00, 0x00, // FSUB DWORD PTR [ESI+0x7DC] ; line 2080
};
static PatchByteCode ranJC_ANG2_patchByteCode = { ranJC_ANG2_byteCode, 34, nullptr, 0 };
// ranJC_DIST declared in ran.asm line 2084
static unsigned char ranJC_DIST_byteCode[] = {
	0xDC, 0x0D, 0x68, 0xC0, 0x85, 0x00, // FMUL QWORD PTR [0x85C068] ; line 2085
};
static PatchByteCode ranJC_DIST_patchByteCode = { ranJC_DIST_byteCode, 6, nullptr, 0 };
// ranGO414 declared in ran.asm line 2089
static unsigned char ranGO414_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4EDE1D ; line 2090
};
static JumpTarget ranGO414_jumpData[] = {
	{ 0x001, 0x4EDE1D },
};
static PatchByteCode ranGO414_patchByteCode = { ranGO414_byteCode, 5, ranGO414_jumpData, 1 };
// ranGO414_3 declared in ran.asm line 2095
static unsigned char ranGO414_3_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x4FE35C ; line 2096
	0xD9, 0xEE,                         // FLDZ ; line 2097
	0x31, 0xDB,                         // XOR EBX,EBX ; line 2098
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x100] ; line 2099
};
static JumpTarget ranGO414_3_jumpData[] = {
	{ 0x001, 0x4FE35C },
};
static PatchByteCode ranGO414_3_patchByteCode = { ranGO414_3_byteCode, 15, ranGO414_3_jumpData, 1 };


// ran.asm
std::array<PatchSkeleton, 177> patchList = {
	PatchSkeleton{ ranID_FIX_patchByteCode,                    (void *)0x4E7A2B,  7}, // Declared line 16. Patch on CMP [ESI+0x196], DI
	PatchSkeleton{ ran66B_HITCOUNT_patchByteCode,              (void *)0x4FE2AA,  7}, // Declared line 21. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ ranD22_SUBSEQ2B_patchByteCode,              (void *)0x4F583D,  8}, // Declared line 205. Patch on JNE SHORT 0x004F5860
	PatchSkeleton{ ranD22_SUBSEQ2C_patchByteCode,              (void *)0x4F5C22,  8}, // Declared line 206. Patch on JNE SHORT 0x004F5C45
	PatchSkeleton{ ranD22_SUBSEQ2AIRB_patchByteCode,           (void *)0x4F5FF3,  8}, // Declared line 207. Patch on JNE SHORT 0x004F6016
	PatchSkeleton{ ranD22_SUBSEQ2AIRC_patchByteCode,           (void *)0x4F6401,  8}, // Declared line 208. Patch on JNE SHORT 0x004F6445
	PatchSkeleton{ ranD22_DIVE_patchByteCode,                  (void *)0x4F582C,  6}, // Declared line 210. Patch on FLD ST, DWORD PTR [0x85C558]
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5752,  6}, // Declared line 211. Patch on FST DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5758,  6}, // Declared line 212. Patch on FST DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranD22_DIVE_patchByteCode,                  (void *)0x4F5BD2,  6}, // Declared line 214. Patch on FLD ST, DWORD PTR [0x85C554]
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5AF8,  6}, // Declared line 215. Patch on FST DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5AFE,  6}, // Declared line 216. Patch on FST DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranD22_DIVE_patchByteCode,                  (void *)0x4F5FE2,  6}, // Declared line 218. Patch on FLD ST, DWORD PTR [0x85BB50]
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5EC0,  6}, // Declared line 219. Patch on FST DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F5EC6,  6}, // Declared line 220. Patch on FST DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranD22_DIVE_patchByteCode,                  (void *)0x4F63B1,  6}, // Declared line 222. Patch on FLD ST, DWORD PTR [0x85C550]
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F628F,  6}, // Declared line 223. Patch on FST DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x4F6295,  6}, // Declared line 224. Patch on FST DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranD22_DIVE2_patchByteCode,                 (void *)0x4F5BFF,  6}, // Declared line 226. Patch on FLD ST, DWORD PTR [0x85AD04]
	PatchSkeleton{ ranD22_DIVE2_patchByteCode,                 (void *)0x4F63DE,  6}, // Declared line 228. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ ranD22_SPINB_patchByteCode,                 (void *)0x4F576F,  7}, // Declared line 329. Patch on CMP DWORD PTR [ESI+0x144], 0xA
	PatchSkeleton{ ranD22_SPINC_patchByteCode,                 (void *)0x4F5B15,  7}, // Declared line 330. Patch on CMP DWORD PTR [ESI+0x144], 0xA
	PatchSkeleton{ ranD22_SPINAIRB_patchByteCode,              (void *)0x4F5EDD,  7}, // Declared line 331. Patch on CMP DWORD PTR [ESI+0x144], 0xA
	PatchSkeleton{ ranD22_SPINAIRC_patchByteCode,              (void *)0x4F62AC,  7}, // Declared line 332. Patch on CMP DWORD PTR [ESI+0x144], 0xA
	PatchSkeleton{ ranD22_AIRLV_patchByteCode,                 (void *)0x500E96,  6}, // Declared line 337. Patch on CMP [ESI+0x6A7], BL
	PatchSkeleton{ ranD22_AIRLV_patchByteCode,                 (void *)0x500DB0,  6}, // Declared line 338. Patch on CMP [ESI+0x6A7], BL
	PatchSkeleton{ ran5SC_SPIN_BULLET_NOBOUNCE_patchByteCode,  (void *)0x5081B8,  6}, // Declared line 346. Patch on MOV EAX, [ESI+0x144]
	PatchSkeleton{ ranNOP_patchByteCode,                       (void *)0x508195,  6}, // Declared line 348. Patch on FADD ST, QWORD PTR [0x85B968]
	PatchSkeleton{ ran5SC_SPIN_LAUNCHSPD_patchByteCode,        (void *)0x5080C4,  6}, // Declared line 353. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ ran5SC_SPIN_INSTANTFIRE_patchByteCode,      (void *)0x5080A5,  7}, // Declared line 358. Patch on CMP DWORD PTR [ESI+0x144], 0x1E
	PatchSkeleton{ ran5SC_SPIN_BULLETCOUNT_patchByteCode,      (void *)0x4F84D4,  9}, // Declared line 364. Patch on CMP ECX, 0x3C
	PatchSkeleton{ ran5SC_SPIN_BULLETCOUNT_AIR_patchByteCode,  (void *)0x4F8722,  9}, // Declared line 370. Patch on CMP ECX, 0x3C
	PatchSkeleton{ ran5SC_SPIN_GRAVITY_patchByteCode,          (void *)0x4F86D2,  8}, // Declared line 387. Patch on CMP WORD PTR [ESI+0x13E], 4
	PatchSkeleton{ ran5SC_SPIN_GROUNDHITCOUNT_patchByteCode,   (void *)0x4F849B,  9}, // Declared line 402. Patch on MOV ECX, ESI
	PatchSkeleton{ ran5SC_SPIN_patchByteCode,                  (void *)0x4F8852,  6}, // Declared line 407. Patch on FLD ST, DWORD PTR [0x85BC68]
	PatchSkeleton{ ran3SC_SPIN_ACTIONS_patchByteCode,          (void *)0x4F9F5A,  7}, // Declared line 552. Patch on CMP DWORD PTR [ESI+0x144], 0x24
	PatchSkeleton{ ran3SC_SPIN_TRAIL_patchByteCode,            (void *)0x50860B,  6}, // Declared line 593. Patch on MOV ECX, [ESI+0x190]
	PatchSkeleton{ ran3SC_SPIN_ONEBULLET_patchByteCode,        (void *)0x4F9007,  6}, // Declared line 598. Patch on JNP 0x004F8F3C
	PatchSkeleton{ ran3SC_SPIN_ONEBULLET_AIR_patchByteCode,    (void *)0x4FA179,  6}, // Declared line 603. Patch on JNP 0x004FA0AE
	PatchSkeleton{ ran3SC_SPIN_TOAIR_patchByteCode,            (void *)0x4F8E74,  6}, // Declared line 624. Patch on JNE 0x004ED915
	PatchSkeleton{ ranSC_VAJRA_TRANSFORM_patchByteCode,        (void *)0x50D33B,  6}, // Declared line 648. Patch on FSTP DWORD PTR [ESI+0x12C], ST
	PatchSkeleton{ ranSC_VAJRA_SUBSEQREDIR_patchByteCode,      (void *)0x507782,  8}, // Declared line 659. Patch on CMP WORD PTR [ESI+0x13E], 0
	PatchSkeleton{ ranSC_VAJRA_ROTATE_patchByteCode,           (void *)0x5077FA,  6}, // Declared line 737. Patch on FADD ST, QWORD PTR [0x858990]
	PatchSkeleton{ ran2SC_VAJRA_SHOOT_patchByteCode,           (void *)0x507833,  6}, // Declared line 797. Patch on JP 0x0050795C
	PatchSkeleton{ ran3SC_VAJRAB_JUMP_patchByteCode,           (void *)0x507B07,  6}, // Declared line 802. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ ran3SC_VAJRA_JUMP_patchByteCode,            (void *)0x4F805C,  9}, // Declared line 807. Patch on MOV ECX, ESI
	PatchSkeleton{ ran3SC_VAJRA_HITS_patchByteCode,            (void *)0x507998,  6}, // Declared line 863. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ ran3SC_VAJRA_TO850_patchByteCode,           (void *)0x50CFBB,  7}, // Declared line 882. Patch on MOVZX EAX, BYTE PTR [EAX+0x50FBD0]
	PatchSkeleton{ ran3SC_VAJRA_TO600_patchByteCode,           (void *)0x4FE473,  5}, // Declared line 889. Patch on CMP EAX, 0xEF
	PatchSkeleton{ ran3SC_VAJRA_COUNTJUMP_patchByteCode,       (void *)0x4F7ECF,  6}, // Declared line 899. Patch on MOV EAX, [ESI+0x144]
	PatchSkeleton{ ran2SC_2VAJRA_COUNT_patchByteCode,          (void *)0x4F7DD9,  6}, // Declared line 911. Patch on MOV EAX, [ESI+0x144]
	PatchSkeleton{ ran2SC_2VAJRA_ANG1_patchByteCode,           (void *)0x4F7E1F,  6}, // Declared line 937. Patch on FADD ST, QWORD PTR [0x858E98]
	PatchSkeleton{ ran2SC_2VAJRA_ANG2_patchByteCode,           (void *)0x4F7EEE,  6}, // Declared line 938. Patch on FADD ST, QWORD PTR [0x85C540]
	PatchSkeleton{ ran2SC_2VAJRA_STARTX_patchByteCode,         (void *)0x4F7EA2,  6}, // Declared line 943. Patch on FMUL ST, QWORD PTR [0x857B20]
	PatchSkeleton{ ran2SC_2VAJRA_STARTX_patchByteCode,         (void *)0x4F7F47,  6}, // Declared line 944. Patch on FMUL ST, QWORD PTR [0x857B20]
	PatchSkeleton{ ranSC_VAJRAB_JUMP_patchByteCode,            (void *)0x5078FF,  5}, // Declared line 955. Patch on PUSH 0x352
	PatchSkeleton{ ranSC_VAJRAB_JUMP2_patchByteCode,           (void *)0x50793E,  5}, // Declared line 966. Patch on PUSH 0x352
	PatchSkeleton{ ranSC_VAJRA_SEQ_patchByteCode,              (void *)0x4F7EC5,  5}, // Declared line 977. Patch on PUSH 0x352
	PatchSkeleton{ ranSC_VAJRA_SEQ2_patchByteCode,             (void *)0x4F7F71,  5}, // Declared line 988. Patch on PUSH 0x352
	PatchSkeleton{ ranD214C_OFFSET_patchByteCode,              (void *)0x4F03C3,  6}, // Declared line 996. Patch on FSUB ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ ranD214C_OFFSET_patchByteCode,              (void *)0x4F0552,  6}, // Declared line 997. Patch on FSUB ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ ranD214_NOBOUNCE_patchByteCode,             (void *)0x503B88,  8}, // Declared line 1002. Patch on JP SHORT 0x00503BEF
	PatchSkeleton{ ranD214_NOBOUNCE2_patchByteCode,            (void *)0x503BEF,  8}, // Declared line 1007. Patch on FLDZ
	PatchSkeleton{ ranD214_ROTATELOOP_patchByteCode,           (void *)0x503ADC,  5}, // Declared line 1088. Patch on CMP AX, DI
	PatchSkeleton{ ranD214_BULLETSPD_patchByteCode,            (void *)0x4EFED5,  6}, // Declared line 1098. Patch on FLD ST, DWORD PTR [ESI+0x7DC]
	PatchSkeleton{ ranD214_BULLETSPD_C_patchByteCode,          (void *)0x4F0302,  6}, // Declared line 1099. Patch on FLD ST, DWORD PTR [ESI+0x7DC]
	PatchSkeleton{ ranD214_BULLETSPD_CHARGE_patchByteCode,     (void *)0x4F00A1,  6}, // Declared line 1109. Patch on FLD ST, DWORD PTR [ESI+0x7DC]
	PatchSkeleton{ ranD214_BULLETSPD_CHARGE_C_patchByteCode,   (void *)0x4F049B,  6}, // Declared line 1110. Patch on FLD ST, DWORD PTR [ESI+0x7DC]
	PatchSkeleton{ ranD214_BULLETCOUNT_patchByteCode,          (void *)0x4EFEA2,  7}, // Declared line 1184. Patch on MOVSX ECX, WORD PTR [ESI+0x7D2]
	PatchSkeleton{ ranD214_BULLETCOUNT_C_patchByteCode,        (void *)0x4F02CF,  7}, // Declared line 1185. Patch on MOVSX EDX, WORD PTR [ESI+0x7D2]
	PatchSkeleton{ ranD214_BULLETCOUNT_CHARGE_patchByteCode,   (void *)0x4F0031,  7}, // Declared line 1269. Patch on MOVSX EDX, WORD PTR [ESI+0x7D2]
	PatchSkeleton{ ranD214_BULLETCOUNT_CHARGE_C_patchByteCode, (void *)0x4F046B,  7}, // Declared line 1270. Patch on MOVSX EAX, WORD PTR [ESI+0x7D2]
	PatchSkeleton{ ranD214_LOOP_patchByteCode,                 (void *)0x4EFFC6,  6}, // Declared line 1275. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ ranD214_LOOP_patchByteCode,                 (void *)0x4F03F8,  6}, // Declared line 1276. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ ranD214_LOOP_CHARGE_patchByteCode,          (void *)0x4F014E,  6}, // Declared line 1281. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ ranD214_LOOP_CHARGE_patchByteCode,          (void *)0x4F0587,  6}, // Declared line 1282. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ ranD623_SOUND_patchByteCode,                (void *)0x4EEC21,  9}, // Declared line 1289. Patch on PUSH 0xA
	PatchSkeleton{ ranD623_SOUND_patchByteCode,                (void *)0x4EF353,  9}, // Declared line 1290. Patch on PUSH 0xA
	PatchSkeleton{ ranD623B_LVFIX_patchByteCode,               (void *)0x4EEC52,  8}, // Declared line 1295. Patch on JL SHORT 0x004EEC60
	PatchSkeleton{ ranD623B_LDUR_patchByteCode,                (void *)0x4EEA1C,  7}, // Declared line 1300. Patch on CMP DWORD PTR [ESI+0x144], 0x19
	PatchSkeleton{ ranD623B_patchByteCode,                     (void *)0x4EE971,  6}, // Declared line 1305. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ ranD623B_GRAV2_patchByteCode,               (void *)0x4EEC39,  6}, // Declared line 1319. Patch on FSTP DWORD PTR [ESI+0x7DC], ST
	PatchSkeleton{ ranD623C_TOB_patchByteCode,                 (void *)0x4EED35,  7}, // Declared line 1324. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ ranDJ623C_TOB_patchByteCode,                (void *)0x4EF3FF,  8}, // Declared line 1329. Patch on CMP WORD PTR [ESI+0x13E], 5
	PatchSkeleton{ ranDJ623C_TOB2_patchByteCode,               (void *)0x4FE4D3,  6}, // Declared line 1334. Patch on FLD ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ ranDJ623B_DUR_patchByteCode,                (void *)0x4EF127,  7}, // Declared line 1339. Patch on CMP DWORD PTR [ESI+0x144], 0x19
	PatchSkeleton{ ranDJ623B_HITCOUNT_patchByteCode,           (void *)0x4FE440,  7}, // Declared line 1344. Patch on MOV BYTE PTR [ESI+0x194], 4
	PatchSkeleton{ ranDJ623C_HITCOUNT_patchByteCode,           (void *)0x4FE4E9,  7}, // Declared line 1349. Patch on MOV BYTE PTR [ESI+0x194], 5
	PatchSkeleton{ ranDJ623B_GRAVFIX_patchByteCode,            (void *)0x4EF362,  6}, // Declared line 1354. Patch on FSTP DWORD PTR [ESI+0x100], ST
	PatchSkeleton{ ranDJ623B_SKIPMOMENTUM_patchByteCode,       (void *)0x4EF140,  6}, // Declared line 1391. Patch on CMP AX, 3
	PatchSkeleton{ ranFLD1_patchByteCode,                      (void *)0x4FE4AC,  6}, // Declared line 1396. Patch on FLD ST, QWORD PTR [0x858EA0]
	PatchSkeleton{ ranJD236_MMNTM_patchByteCode,               (void *)0x4FE69B,  6}, // Declared line 1401. Patch on FSTP DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ ranJD236_MMNTM_patchByteCode,               (void *)0x4FE6A7,  6}, // Declared line 1402. Patch on FSTP DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranJD236_MMNTM_patchByteCode,               (void *)0x4FE65D,  6}, // Declared line 1403. Patch on FSTP DWORD PTR [ESI+0x100], ST
	PatchSkeleton{ ranD236_X2_patchByteCode,                   (void *)0x4F11FF,  7}, // Declared line 1424. Patch on CMP DWORD PTR [ESI+0x144], 0
	PatchSkeleton{ ranD236C_patchByteCode,                     (void *)0x4F17BB,  9}, // Declared line 1429. Patch on MOV ECX, ESI
	PatchSkeleton{ ranJD236C_patchByteCode,                    (void *)0x4F23B4,  8}, // Declared line 1434. Patch on CMP WORD PTR [ESI+0x13E], 1
	PatchSkeleton{ ranD236_X_patchByteCode,                    (void *)0x4F1287,  8}, // Declared line 1449. Patch on FLDZ
	PatchSkeleton{ ranD236B_ANG8_patchByteCode,                (void *)0x4F1EBE,  6}, // Declared line 1489. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ ranD236B_ANG7_patchByteCode,                (void *)0x4F1F5D,  6}, // Declared line 1491. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ ranD236B_ANG6_patchByteCode,                (void *)0x4F1FFC,  6}, // Declared line 1493. Patch on FLD ST, DWORD PTR [0x85C590]
	PatchSkeleton{ ranD236B_ANG5_patchByteCode,                (void *)0x4F209B,  6}, // Declared line 1495. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ ranD236B_ANG4_patchByteCode,                (void *)0x4F213A,  6}, // Declared line 1497. Patch on FLD ST, DWORD PTR [0x85AD10]
	PatchSkeleton{ ranD236B_ANG3_patchByteCode,                (void *)0x4F21D9,  6}, // Declared line 1499. Patch on FLD ST, DWORD PTR [0x85B858]
	PatchSkeleton{ ranD236B_ANG2_patchByteCode,                (void *)0x4F2278,  6}, // Declared line 1501. Patch on FLD ST, DWORD PTR [0x85C580]
	PatchSkeleton{ ranD236B_ANG1_patchByteCode,                (void *)0x4F2317,  6}, // Declared line 1503. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ ranD236B_ANG8_patchByteCode,                (void *)0x4F12C5,  6}, // Declared line 1507. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ ranD236B_ANG7_patchByteCode,                (void *)0x4F1364,  6}, // Declared line 1508. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ ranD236B_ANG6_patchByteCode,                (void *)0x4F1403,  6}, // Declared line 1509. Patch on FLD ST, DWORD PTR [0x85C590]
	PatchSkeleton{ ranD236B_ANG5_patchByteCode,                (void *)0x4F14A2,  6}, // Declared line 1510. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ ranD236B_ANG4_patchByteCode,                (void *)0x4F1541,  6}, // Declared line 1511. Patch on FLD ST, DWORD PTR [0x85AD10]
	PatchSkeleton{ ranD236B_ANG3_patchByteCode,                (void *)0x4F15E0,  6}, // Declared line 1512. Patch on FLD ST, DWORD PTR [0x85B858]
	PatchSkeleton{ ranD236B_ANG2_patchByteCode,                (void *)0x4F167F,  6}, // Declared line 1513. Patch on FLD ST, DWORD PTR [0x85C580]
	PatchSkeleton{ ranD236B_ANG1_patchByteCode,                (void *)0x4F171E,  6}, // Declared line 1514. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ ranD236_LEVELS_patchByteCode,               (void *)0x4F129B,  7}, // Declared line 1519. Patch on CMP BYTE PTR [ESI+0x6A6], 2
	PatchSkeleton{ ranD236_LEVELS_patchByteCode,               (void *)0x4F12B0,  7}, // Declared line 1520. Patch on CMP BYTE PTR [ESI+0x6A6], 4
	PatchSkeleton{ ranD236_LEVELS_patchByteCode,               (void *)0x4F1E94,  7}, // Declared line 1521. Patch on CMP BYTE PTR [ESI+0x6A6], 2
	PatchSkeleton{ ranD236_LEVELS_patchByteCode,               (void *)0x4F1EA9,  7}, // Declared line 1522. Patch on CMP BYTE PTR [ESI+0x6A6], 4
	PatchSkeleton{ ranD236B_SPEED_patchByteCode,               (void *)0x4F1E8C,  8}, // Declared line 1528. Patch on FLD1
	PatchSkeleton{ ranD236B_SPEED_patchByteCode,               (void *)0x4F1293,  8}, // Declared line 1529. Patch on FLD1
	PatchSkeleton{ ranD236B_SIZE_patchByteCode,                (void *)0x503D94,  7}, // Declared line 1559. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ ranD236B_TRAIL_patchByteCode,               (void *)0x503DCC,  6}, // Declared line 1567. Patch on FLD ST, DWORD PTR [0x85BCA4]
	PatchSkeleton{ ranJ8A_Y_patchByteCode,                     (void *)0x4EB3AF,  6}, // Declared line 1572. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ ran3A_X_patchByteCode,                      (void *)0x4EABDD,  6}, // Declared line 1577. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ ran3A_X_patchByteCode,                      (void *)0x4EAC4A,  6}, // Declared line 1578. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ ranFLDZ_patchByteCode,                      (void *)0x4EABF1,  6}, // Declared line 1580. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ ranFLDZ_patchByteCode,                      (void *)0x4EAC6B,  6}, // Declared line 1581. Patch on FLD ST, DWORD PTR [0x85882C]
	PatchSkeleton{ ran3A_SOUND1_patchByteCode,                 (void *)0x4EAEA1,  8}, // Declared line 1586. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ ran3A_SOUND1_patchByteCode,                 (void *)0x4EAF6D,  8}, // Declared line 1587. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ ran3A_X2_patchByteCode,                     (void *)0x4EAC5E,  6}, // Declared line 1592. Patch on FCOMP ST, QWORD PTR [0x858EC0]
	PatchSkeleton{ ran3A_NOY1_patchByteCode,                   (void *)0x4EAC15,  6}, // Declared line 1615. Patch on JNE 0x004EA948
	PatchSkeleton{ ran3A_NOY21_patchByteCode,                  (void *)0x4EAD07,  6}, // Declared line 1631. Patch on FLD ST, DWORD PTR [0x85B9BC]
	PatchSkeleton{ ran3A_NOY22_patchByteCode,                  (void *)0x4EAD88,  6}, // Declared line 1632. Patch on FLD ST, DWORD PTR [0x85C5B8]
	PatchSkeleton{ ranJ6A_SKIPMOMENTUM_patchByteCode,          (void *)0x4EB0D2,  6}, // Declared line 1656. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ ranJ2A_SKIPMOMENTUM_patchByteCode,          (void *)0x4EB19A,  6}, // Declared line 1681. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ ranJ2A_MOMENTUM_patchByteCode,              (void *)0x4FE191,  6}, // Declared line 1686. Patch on FLD ST, QWORD PTR [0x857F78]
	PatchSkeleton{ ranJ2A_NOP_patchByteCode,                   (void *)0x4FE1AD,  8}, // Declared line 1691. Patch on FLDZ
	PatchSkeleton{ ranTO411_2_patchByteCode,                   (void *)0x4FE330,  8}, // Declared line 1708. Patch on XOR EBX, EBX
	PatchSkeleton{ ranTO411_2_patchByteCode,                   (void *)0x4FE2CE,  8}, // Declared line 1709. Patch on XOR EBX, EBX
	PatchSkeleton{ ranTO411_patchByteCode,                     (void *)0x4ED635,  7}, // Declared line 1714. Patch on MOV ECX, ESI
	PatchSkeleton{ ranTO411_patchByteCode,                     (void *)0x4EDC4F,  7}, // Declared line 1715. Patch on MOV ECX, ESI
	PatchSkeleton{ ran6C_ARCMUL_patchByteCode,                 (void *)0x4EDA50,  6}, // Declared line 1740. Patch on FSUBR ST, QWORD PTR [0x859910]
	PatchSkeleton{ ran6C_ARCSPD_patchByteCode,                 (void *)0x4EDA7C,  6}, // Declared line 1746. Patch on FSUBR ST, QWORD PTR [0x867830]
	PatchSkeleton{ ran6CH_ARCMUL_patchByteCode,                (void *)0x4EDB8A,  7}, // Declared line 1771. Patch on FSTP DWORD PTR [ESP+0x54C], ST
	PatchSkeleton{ ran6CH_ARCSPD_patchByteCode,                (void *)0x4EDBA3,  6}, // Declared line 1777. Patch on FSUBR ST, QWORD PTR [0x857958]
	PatchSkeleton{ ranB_ACCEL_patchByteCode,                   (void *)0x501C96,  6}, // Declared line 1782. Patch on FLD ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ ranB_STARTSPD_patchByteCode,                (void *)0x4EB8E3,  6}, // Declared line 1787. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ ranJB_NOCHARGE_patchByteCode,               (void *)0x4EC7A3,  8}, // Declared line 1792. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ ranJB_NOCHARGE_patchByteCode,               (void *)0x4EBB0D,  8}, // Declared line 1793. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ ranJB_STARTSPD_patchByteCode,               (void *)0x4EC541,  6}, // Declared line 1798. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ ranADDJ6B_patchByteCode,                    (void *)0x50167B,  5}, // Declared line 1815. Patch on CMP DI, AX
	PatchSkeleton{ ranB_END_patchByteCode,                     (void *)0x4EBAEB,  6}, // Declared line 1828. Patch on JE 0x004F4C72
	PatchSkeleton{ ranB_XY_patchByteCode,                      (void *)0x4EB83F,  7}, // Declared line 1884. Patch on MOV ECX, ESI
	PatchSkeleton{ ranB_OFFX_patchByteCode,                    (void *)0x4EB913,  6}, // Declared line 1889. Patch on FMUL ST, QWORD PTR [0x858990]
	PatchSkeleton{ ranB_OFFY_patchByteCode,                    (void *)0x4EB91F,  6}, // Declared line 1894. Patch on FADD ST, QWORD PTR [0x857968]
	PatchSkeleton{ ranTO400_patchByteCode,                     (void *)0x4EBCCB,  7}, // Declared line 1899. Patch on MOV ECX, ESI
	PatchSkeleton{ ranTO400_patchByteCode,                     (void *)0x4EC18A,  7}, // Declared line 1900. Patch on MOV ECX, ESI
	PatchSkeleton{ ranB_FIREPERIOD_patchByteCode,              (void *)0x4EB870,  5}, // Declared line 1912. Patch on MOV ECX, 3
	PatchSkeleton{ ranB_FIRELENGTH_patchByteCode,              (void *)0x4EB88D,  8}, // Declared line 1917. Patch on CMP WORD PTR [ESI+0x7D4], 5
	PatchSkeleton{ ranB_FIRENUM_patchByteCode,                 (void *)0x4EB976,  6}, // Declared line 1923. Patch on ADD EDI, 5
	PatchSkeleton{ ranJB_FIREANG_patchByteCode,                (void *)0x4EC522,  7}, // Declared line 1930. Patch on LEA EAX, [ESP+0x23C]
	PatchSkeleton{ ranJB_FIREPERIOD_patchByteCode,             (void *)0x4EC4DE,  5}, // Declared line 1935. Patch on MOV ECX, 3
	PatchSkeleton{ ranJB_FIRELENGTH_patchByteCode,             (void *)0x4EC4ED,  8}, // Declared line 1940. Patch on CMP WORD PTR [ESI+0x7D4], 5
	PatchSkeleton{ ranJB_FIRENUM_patchByteCode,                (void *)0x4EC5D4,  6}, // Declared line 1946. Patch on ADD EDI, 5
	PatchSkeleton{ ranJB_X_patchByteCode,                      (void *)0x4EC827,  6}, // Declared line 1975. Patch on JNE 0x004EC8FF
	PatchSkeleton{ ranJB_Y_patchByteCode,                      (void *)0x4EC848,  6}, // Declared line 2001. Patch on FSTP DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ ranGO404_patchByteCode,                     (void *)0x4EC9DB,  7}, // Declared line 2006. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ ranGO404_patchByteCode,                     (void *)0x4ECF3C,  7}, // Declared line 2007. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ ranJC_NOBOUNCE1_patchByteCode,              (void *)0x502A64,  8}, // Declared line 2012. Patch on JP SHORT 0x00502AB6
	PatchSkeleton{ ranJC_NOBOUNCE2_patchByteCode,              (void *)0x502AEF,  6}, // Declared line 2016. Patch on JNE 0x00502813
	PatchSkeleton{ ranJC_XY_patchByteCode,                     (void *)0x4EDE2D,  6}, // Declared line 2066. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ ranJC_ANG_patchByteCode,                    (void *)0x4EDEB3,  6}, // Declared line 2071. Patch on FADD ST, QWORD PTR [0x857F80]
	PatchSkeleton{ ranJC_ANG2_patchByteCode,                   (void *)0x4EDECE,  6}, // Declared line 2082. Patch on FADD ST, QWORD PTR [0x858210]
	PatchSkeleton{ ranJC_DIST_patchByteCode,                   (void *)0x4EDF4E,  6}, // Declared line 2087. Patch on FMUL ST, QWORD PTR [0x857938]
	PatchSkeleton{ ranGO414_patchByteCode,                     (void *)0x4EE09E,  7}, // Declared line 2092. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ ranGO414_patchByteCode,                     (void *)0x4EE465,  7}, // Declared line 2093. Patch on CMP [ESI+0x13E], BP
	PatchSkeleton{ ranGO414_3_patchByteCode,                   (void *)0x4FE3B3, 10}, // Declared line 2101. Patch on FLDZ
};


static std::array<unsigned, 138> skeletonsForUpdate = {
	   0,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
	  17,  18,  19,  20,  21,  22,  23,  30,  31,  32,  33,  34,  35,  37,  38,  39,
	  45,  49,  50,  51,  52,  53,  54,  57,  58,  59,  60,  64,  65,  66,  67,  68,
	  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  85,
	  88,  89,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107,
	 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 122, 123, 124, 125,
	 126, 127, 128, 129, 130, 131, 132, 133, 134, 139, 140, 141, 142, 143, 144, 146,
	 147, 148, 149, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
	 164, 165, 166, 167, 170, 171, 172, 173, 174, 175,
};
static std::array<unsigned, 14> skeletonsForInitializeAction = { 1, 48, 84, 86, 87, 90, 91, 92, 93, 135, 136, 137, 138, 176 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 3> skeletonsForHandleInputs = { 24, 25, 150 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 20> skeletonsForObjectUpdate = {
	  26,  27,  28,  29,  36,  41,  42,  43,  44,  46,  55,  56,  61,  62,  63, 120,
	 121, 145, 168, 169,
};
static std::array<unsigned, 2> skeletonsForObjectInitializeAction = { 40, 47 };



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
	printf("Internal memory used %zu/10453\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 10453);
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
