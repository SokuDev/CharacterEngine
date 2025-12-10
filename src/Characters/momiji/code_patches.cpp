#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<13560> applyMemory;
StackedMemory<4714> internalMemory;
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


// momijiFLDZ declared in momiji.asm line 1
static unsigned char momijiFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode momijiFLDZ_patchByteCode = { momijiFLDZ_byteCode, 2, nullptr, 0 };
// momijiNOP declared in momiji.asm line 4
static unsigned char momijiNOP_byteCode[] = {
	0x90, // NOP ; line 5
};
static PatchByteCode momijiNOP_patchByteCode = { momijiNOP_byteCode, 1, nullptr, 0 };
// momijiID_FIX declared in momiji.asm line 13
static unsigned char momijiID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000016 ; line 14
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x0000 ; line 16
};
static PatchByteCode momijiID_FIX_patchByteCode = { momijiID_FIX_byteCode, 19, nullptr, 0 };
// momiji5C_ACCEL declared in momiji.asm line 25
static unsigned char momiji5C_ACCEL_byteCode[] = {
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 27
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                         // JNE SHORT LABELZ ; line 28
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4B1744 ; line 29
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x30 ; line 32
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00,                         // JL SHORT LABELY ; line 33
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x4B1744 ; line 34
	0x60,                                                       // PUSHAD ; line 38
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x02 ; line 39
	0x0F, 0x8E, 0x35, 0x00, 0x00, 0x00,                         // JLE SHORT LABELX ; line 40
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 41
	0x0F, 0x84, 0x25, 0x00, 0x00, 0x00,                         // JE SHORT LABELX ; line 42
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                         // MOV EAX, DWORD PTR [ESI+0x144] ; line 43
	0x81, 0xE8, 0x01, 0x00, 0x00, 0x00,                         // SUB EAX,0x1 ; line 44
	0x89, 0x86, 0x44, 0x01, 0x00, 0x00,                         // MOV [ESI+0x144],EAX ; line 45
	0x66, 0x8B, 0x86, 0x40, 0x01, 0x00, 0x00,                   // MOV AX, WORD PTR [ESI+0x140] ; line 46
	0x66, 0x81, 0xE8, 0x01, 0x00,                               // SUB AX,0x1 ; line 47
	0x66, 0x89, 0x86, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],AX ; line 48
	0x61,                                                       // POPAD ; line 50
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 52
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 53
	0xD9, 0x58, 0x04,                                           // FSTP DWORD PTR [EAX+0x04] ; line 54
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                         // FLD DWORD PTR [0x85B3B0] ; line 55
};
static JumpTarget momiji5C_ACCEL_jumpData[] = {
	{ 0x00E, 0x4B1744 },
	{ 0x023, 0x4B1744 },
};
static PatchByteCode momiji5C_ACCEL_patchByteCode = { momiji5C_ACCEL_byteCode, 135, momiji5C_ACCEL_jumpData, 2 };
// momiji6D_MIN declared in momiji.asm line 61
static unsigned char momiji6D_MIN_byteCode[] = {
	0x66, 0x81, 0xF8, 0x02, 0x00,       // CMP AX,0x0002 ; line 62
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x49272A ; line 63
};
static JumpTarget momiji6D_MIN_jumpData[] = {
	{ 0x007, 0x49272A },
};
static PatchByteCode momiji6D_MIN_patchByteCode = { momiji6D_MIN_byteCode, 11, momiji6D_MIN_jumpData, 1 };
// momijiD214_ONHIT declared in momiji.asm line 67
static unsigned char momijiD214_ONHIT_byteCode[] = {
	0x60,                               // PUSHAD ; line 68
	0x6A, 0x07,                         // PUSH 0x07 ; line 69
	0x6A, 0x03,                         // PUSH 0x03 ; line 70
	0x6A, 0x00,                         // PUSH 0x00 ; line 71
	0x89, 0xF1,                         // MOV ECX,ESI ; line 72
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x48CE90 ; line 73
	0x84, 0xC0,                         // TEST AL,AL ; line 74
	0x61,                               // POPAD ; line 75
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x4B1321 ; line 76
	0xBD, 0x05, 0x00, 0x00, 0x00,       // MOV EBP,0x5 ; line 77
	0x39, 0xAE, 0x90, 0x01, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],EBP ; line 78
};
static JumpTarget momijiD214_ONHIT_jumpData[] = {
	{ 0x00A, 0x48CE90 },
	{ 0x013, 0x4B1321 },
};
static PatchByteCode momijiD214_ONHIT_patchByteCode = { momijiD214_ONHIT_byteCode, 34, momijiD214_ONHIT_jumpData, 2 };
// momiji2C_ONHIT declared in momiji.asm line 80
static unsigned char momiji2C_ONHIT_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 81
	0x6A, 0x02,                                           // PUSH 0x02 ; line 82
	0x6A, 0x01,                                           // PUSH 0x01 ; line 83
	0x6A, 0x00,                                           // PUSH 0x00 ; line 84
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 85
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x48CE90 ; line 86
	0x84, 0xC0,                                           // TEST AL,AL ; line 87
	0x61,                                                 // POPAD ; line 88
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x4B1321 ; line 89
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 90
};
static JumpTarget momiji2C_ONHIT_jumpData[] = {
	{ 0x00A, 0x48CE90 },
	{ 0x013, 0x4B1321 },
};
static PatchByteCode momiji2C_ONHIT_patchByteCode = { momiji2C_ONHIT_byteCode, 32, momiji2C_ONHIT_jumpData, 2 };
// momijiSHIELD1 declared in momiji.asm line 96
static unsigned char momijiSHIELD1_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 97
	0x60,                                                 // PUSHAD ; line 98
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x0001 ; line 100
	0x0F, 0x85, 0x67, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 101
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 102
	0x0F, 0x85, 0x58, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 103
	0xD9, 0xEE,                                           // FLDZ ; line 104
	0xD9, 0x94, 0x24, 0x7C, 0x05, 0x00, 0x00,             // FST DWORD PTR [ESP+0x57C] ; line 105
	0x6A, 0x03,                                           // PUSH 0x03 ; line 106
	0xD9, 0x9C, 0x24, 0x84, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x584] ; line 107
	0x8D, 0x84, 0x24, 0x80, 0x05, 0x00, 0x00,             // LEA EAX,[ESP+0x580] ; line 108
	0xD9, 0xE8,                                           // FLD1 ; line 109
	0x50,                                                 // PUSH EAX ; line 111
	0xD9, 0x9C, 0x24, 0x8C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x58C] ; line 112
	0x6A, 0x01,                                           // PUSH 0x01 ; line 113
	0x0F, 0xB6, 0x8E, 0x04, 0x01, 0x00, 0x00,             // MOVZX ECX,BYTE PTR [ESI+0x104] ; line 114
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 115
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                   // FADD DWORD PTR [0x871544] ; line 116
	0x51,                                                 // PUSH ECX ; line 117
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                   // SUB ESP,0x08 ; line 118
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 119
	0xD9, 0x5C, 0x24, 0x04,                               // FSTP DWORD PTR [ESP+0x04] ; line 120
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 121
	0xD9, 0x1C, 0x24,                                     // FSTP DWORD PTR [ESP] ; line 122
	0x68, 0x50, 0x03, 0x00, 0x00,                         // PUSH 0x00000350 ; line 123
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 124
	0x61,                                                 // POPAD ; line 126
	0x66, 0x9D,                                           // POPF ; line 127
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                   // FSUB DWORD PTR [ESI+0x100] ; line 129
};
static JumpTarget momijiSHIELD1_jumpData[] = {
	{ 0x075, 0x46EB30 },
};
static PatchByteCode momijiSHIELD1_patchByteCode = { momijiSHIELD1_byteCode, 130, momijiSHIELD1_jumpData, 1 };
// momijiSHIELD2 declared in momiji.asm line 131
static unsigned char momijiSHIELD2_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 132
	0x60,                                                 // PUSHAD ; line 133
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x0000 ; line 135
	0x0F, 0x85, 0x76, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 136
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x0001 ; line 137
	0x0F, 0x85, 0x67, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 138
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 139
	0x0F, 0x85, 0x58, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 140
	0xD9, 0xEE,                                           // FLDZ ; line 141
	0xD9, 0x94, 0x24, 0x7C, 0x05, 0x00, 0x00,             // FST DWORD PTR [ESP+0x57C] ; line 142
	0x6A, 0x03,                                           // PUSH 0x03 ; line 143
	0xD9, 0x9C, 0x24, 0x84, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x584] ; line 144
	0x8D, 0x84, 0x24, 0x80, 0x05, 0x00, 0x00,             // LEA EAX,[ESP+0x580] ; line 145
	0xD9, 0xE8,                                           // FLD1 ; line 146
	0x50,                                                 // PUSH EAX ; line 148
	0xD9, 0x9C, 0x24, 0x8C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x58C] ; line 149
	0x6A, 0x01,                                           // PUSH 0x01 ; line 150
	0x0F, 0xB6, 0x8E, 0x04, 0x01, 0x00, 0x00,             // MOVZX ECX,BYTE PTR [ESI+0x104] ; line 151
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 152
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                   // FADD DWORD PTR [0x871544] ; line 153
	0x51,                                                 // PUSH ECX ; line 154
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                   // SUB ESP,0x08 ; line 155
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 156
	0xD9, 0x5C, 0x24, 0x04,                               // FSTP DWORD PTR [ESP+0x04] ; line 157
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 158
	0xD9, 0x1C, 0x24,                                     // FSTP DWORD PTR [ESP] ; line 159
	0x68, 0x50, 0x03, 0x00, 0x00,                         // PUSH 0x00000350 ; line 160
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 161
	0x61,                                                 // POPAD ; line 163
	0x66, 0x9D,                                           // POPF ; line 164
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 166
};
static JumpTarget momijiSHIELD2_jumpData[] = {
	{ 0x084, 0x46EB30 },
};
static PatchByteCode momijiSHIELD2_patchByteCode = { momijiSHIELD2_byteCode, 146, momijiSHIELD2_jumpData, 1 };
// momijiSHIELD3 declared in momiji.asm line 168
static unsigned char momijiSHIELD3_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 169
	0x60,                                                 // PUSHAD ; line 170
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 172
	0x0F, 0x85, 0x76, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 173
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x0000 ; line 174
	0x0F, 0x85, 0x67, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 175
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 176
	0x0F, 0x85, 0x58, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 177
	0xD9, 0xEE,                                           // FLDZ ; line 179
	0xD9, 0x94, 0x24, 0x7C, 0x05, 0x00, 0x00,             // FST DWORD PTR [ESP+0x57C] ; line 180
	0x6A, 0x03,                                           // PUSH 0x03 ; line 181
	0xD9, 0x9C, 0x24, 0x84, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x584] ; line 182
	0x8D, 0x84, 0x24, 0x80, 0x05, 0x00, 0x00,             // LEA EAX,[ESP+0x580] ; line 183
	0xD9, 0xE8,                                           // FLD1 ; line 184
	0x50,                                                 // PUSH EAX ; line 186
	0xD9, 0x9C, 0x24, 0x8C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x58C] ; line 187
	0x6A, 0x01,                                           // PUSH 0x01 ; line 188
	0x0F, 0xB6, 0x8E, 0x04, 0x01, 0x00, 0x00,             // MOVZX ECX,BYTE PTR [ESI+0x104] ; line 189
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 190
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                   // FADD DWORD PTR [0x871544] ; line 191
	0x51,                                                 // PUSH ECX ; line 192
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                   // SUB ESP,0x08 ; line 193
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 194
	0xD9, 0x5C, 0x24, 0x04,                               // FSTP DWORD PTR [ESP+0x04] ; line 195
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 196
	0xD9, 0x1C, 0x24,                                     // FSTP DWORD PTR [ESP] ; line 197
	0x68, 0x50, 0x03, 0x00, 0x00,                         // PUSH 0x00000350 ; line 198
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 199
	0x61,                                                 // POPAD ; line 201
	0x66, 0x9D,                                           // POPF ; line 202
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 204
};
static JumpTarget momijiSHIELD3_jumpData[] = {
	{ 0x084, 0x46EB30 },
};
static PatchByteCode momijiSHIELD3_patchByteCode = { momijiSHIELD3_byteCode, 145, momijiSHIELD3_jumpData, 1 };
// momijiSHIELD4 declared in momiji.asm line 207
static unsigned char momijiSHIELD4_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 208
	0x60,                                                 // PUSHAD ; line 209
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 211
	0x0F, 0x85, 0x85, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 212
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x0000 ; line 213
	0x0F, 0x85, 0x76, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 214
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 215
	0x0F, 0x85, 0x67, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 216
	0x66, 0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x144],0x0000 ; line 217
	0x0F, 0x85, 0x58, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 218
	0xD9, 0xEE,                                           // FLDZ ; line 220
	0xD9, 0x94, 0x24, 0x7C, 0x05, 0x00, 0x00,             // FST DWORD PTR [ESP+0x57C] ; line 221
	0x6A, 0x03,                                           // PUSH 0x03 ; line 222
	0xD9, 0x9C, 0x24, 0x84, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x584] ; line 223
	0x8D, 0x84, 0x24, 0x80, 0x05, 0x00, 0x00,             // LEA EAX,[ESP+0x580] ; line 224
	0xD9, 0xE8,                                           // FLD1 ; line 225
	0x50,                                                 // PUSH EAX ; line 227
	0xD9, 0x9C, 0x24, 0x8C, 0x05, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x58C] ; line 228
	0x6A, 0x01,                                           // PUSH 0x01 ; line 229
	0x0F, 0xB6, 0x8E, 0x04, 0x01, 0x00, 0x00,             // MOVZX ECX,BYTE PTR [ESI+0x104] ; line 230
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 231
	0xD8, 0x05, 0x44, 0x15, 0x87, 0x00,                   // FADD DWORD PTR [0x871544] ; line 232
	0x51,                                                 // PUSH ECX ; line 233
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                   // SUB ESP,0x08 ; line 234
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 235
	0xD9, 0x5C, 0x24, 0x04,                               // FSTP DWORD PTR [ESP+0x04] ; line 236
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 237
	0xD9, 0x1C, 0x24,                                     // FSTP DWORD PTR [ESP] ; line 238
	0x68, 0x50, 0x03, 0x00, 0x00,                         // PUSH 0x00000350 ; line 239
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 240
	0x61,                                                 // POPAD ; line 242
	0x66, 0x9D,                                           // POPF ; line 243
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 245
};
static JumpTarget momijiSHIELD4_jumpData[] = {
	{ 0x093, 0x46EB30 },
};
static PatchByteCode momijiSHIELD4_patchByteCode = { momijiSHIELD4_byteCode, 161, momijiSHIELD4_jumpData, 1 };
// momijiSHIELD33 declared in momiji.asm line 248
static unsigned char momijiSHIELD33_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x495052 ; line 249
};
static JumpTarget momijiSHIELD33_jumpData[] = {
	{ 0x001, 0x495052 },
};
static PatchByteCode momijiSHIELD33_patchByteCode = { momijiSHIELD33_byteCode, 5, momijiSHIELD33_jumpData, 1 };
// momijiSHIELDA declared in momiji.asm line 252
static unsigned char momijiSHIELDA_byteCode[] = {
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x35, 0x02, // CMP WORD PTR [EAX+0x13C],0x0235 ; line 253
	0x0F, 0x84, 0x51, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 254
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x36, 0x02, // CMP WORD PTR [EAX+0x13C],0x0236 ; line 255
	0x0F, 0x84, 0x42, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 256
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x33, 0x01, // CMP WORD PTR [EAX+0x13C],0x0133 ; line 257
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 258
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x98, 0x01, // CMP WORD PTR [EAX+0x13C],0x0198 ; line 259
	0x0F, 0x84, 0x24, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 260
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x2E, 0x01, // CMP WORD PTR [EAX+0x13C],0x012E ; line 261
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 262
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [EAX+0x13E],0x0001 ; line 263
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 264
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x4B1321 ; line 266
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EAX+0xEC] ; line 268
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xEC] ; line 269
};
static JumpTarget momijiSHIELDA_jumpData[] = {
	{ 0x05C, 0x4B1321 },
};
static PatchByteCode momijiSHIELDA_patchByteCode = { momijiSHIELDA_byteCode, 108, momijiSHIELDA_jumpData, 1 };
// momijiSHIELDB declared in momiji.asm line 272
static unsigned char momijiSHIELDB_byteCode[] = {
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x35, 0x02, // CMP WORD PTR [EAX+0x13C],0x0235 ; line 273
	0x0F, 0x84, 0x41, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 274
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x36, 0x02, // CMP WORD PTR [EAX+0x13C],0x0236 ; line 275
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 276
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x2E, 0x01, // CMP WORD PTR [EAX+0x13C],0x012E ; line 277
	0x0F, 0x84, 0x23, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 278
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x98, 0x01, // CMP WORD PTR [EAX+0x13C],0x0198 ; line 279
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 280
	0x66, 0x81, 0xB8, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [EAX+0x140],0x0000 ; line 281
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x4B58C6 ; line 282
	0xE9, 0x05, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 283
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x4B58C6 ; line 285
	0x90,                                                 // NOP ; line 287
};
static JumpTarget momijiSHIELDB_jumpData[] = {
	{ 0x047, 0x4B58C6 },
	{ 0x051, 0x4B58C6 },
};
static PatchByteCode momijiSHIELDB_patchByteCode = { momijiSHIELDB_byteCode, 86, momijiSHIELDB_jumpData, 2 };
// momijiC6A_SKIP declared in momiji.asm line 300
static unsigned char momijiC6A_SKIP_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 301
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 302
	0x0F, 0x85, 0x25, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 303
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 304
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 305
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 306
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 307
	0xC6, 0x86, 0x9D, 0x04, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x49D],0x00 ; line 308
	0x80, 0xBE, 0x9D, 0x04, 0x00, 0x00, 0x01,             // CMP BYTE PTR [ESI+0x49D],0x01 ; line 310
	0x0F, 0x85, 0x25, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 311
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x01 ; line 312
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 313
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 314
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 315
	0xC6, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF,             // MOV BYTE PTR [ESI+0x142],0xFF ; line 316
	0x66, 0x9D,                                           // POPF ; line 318
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 319
	0xDD, 0xD8,                                           // FSTP ST0 ; line 320
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x487CA0 ; line 321
};
static JumpTarget momijiC6A_SKIP_jumpData[] = {
	{ 0x06F, 0x487CA0 },
};
static PatchByteCode momijiC6A_SKIP_patchByteCode = { momijiC6A_SKIP_byteCode, 115, momijiC6A_SKIP_jumpData, 1 };
// momijiJ6A_SKIP declared in momiji.asm line 325
static unsigned char momijiJ6A_SKIP_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 326
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 327
	0x0F, 0x85, 0x25, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 328
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 329
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 330
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 331
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 332
	0xC6, 0x86, 0x9D, 0x04, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x49D],0x00 ; line 333
	0x80, 0xBE, 0x9D, 0x04, 0x00, 0x00, 0x01,             // CMP BYTE PTR [ESI+0x49D],0x01 ; line 335
	0x0F, 0x85, 0x25, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 336
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 337
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 338
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 339
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 340
	0xC6, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF,             // MOV BYTE PTR [ESI+0x142],0xFF ; line 341
	0x66, 0x9D,                                           // POPF ; line 343
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 344
};
static PatchByteCode momijiJ6A_SKIP_patchByteCode = { momijiJ6A_SKIP_byteCode, 112, nullptr, 0 };
// momijiD421_JUMPY declared in momiji.asm line 348
static unsigned char momijiD421_JUMPY_byteCode[] = {
	0xD9, 0x05, 0x8C, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B98C] ; line 349
};
static PatchByteCode momijiD421_JUMPY_patchByteCode = { momijiD421_JUMPY_byteCode, 6, nullptr, 0 };
// momijiD421_JUMPYDOWN declared in momiji.asm line 351
static unsigned char momijiD421_JUMPYDOWN_byteCode[] = {
	0xD9, 0x05, 0x00, 0xC0, 0x85, 0x00, // FLD DWORD PTR [0x85C000] ; line 352
};
static PatchByteCode momijiD421_JUMPYDOWN_patchByteCode = { momijiD421_JUMPYDOWN_byteCode, 6, nullptr, 0 };
// momijiD421_JUMPYGRAV declared in momiji.asm line 354
static unsigned char momijiD421_JUMPYGRAV_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 355
	0x60,                                                 // PUSHAD ; line 356
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x1C, 0x02, // CMP WORD PTR [ESI+0x13C],0x021C ; line 357
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 358
	0xDC, 0x35, 0x20, 0xDC, 0x85, 0x00,                   // FDIV QWORD PTR [0x85DC20] ; line 359
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP LABEL2 ; line 360
	0xD8, 0x35, 0xC8, 0xD0, 0x85, 0x00,                   // FDIV DWORD PTR [0x85D0C8] ; line 362
	0x61,                                                 // POPAD ; line 364
	0x66, 0x9D,                                           // POPF ; line 365
};
static PatchByteCode momijiD421_JUMPYGRAV_patchByteCode = { momijiD421_JUMPYGRAV_byteCode, 38, nullptr, 0 };
// momijiD421_SPAWN declared in momiji.asm line 367
static unsigned char momijiD421_SPAWN_byteCode[] = {
	0x6B, 0xC9, 0x01,                   // IMUL ECX,ECX,0x00000001 ; line 368
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xEC] ; line 369
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x49F028 ; line 370
};
static JumpTarget momijiD421_SPAWN_jumpData[] = {
	{ 0x00A, 0x49F028 },
};
static PatchByteCode momijiD421_SPAWN_patchByteCode = { momijiD421_SPAWN_byteCode, 14, momijiD421_SPAWN_jumpData, 1 };
// momijiD421_SPAWNY declared in momiji.asm line 372
static unsigned char momijiD421_SPAWNY_byteCode[] = {
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 373
};
static PatchByteCode momijiD421_SPAWNY_patchByteCode = { momijiD421_SPAWNY_byteCode, 6, nullptr, 0 };
// momijiFLD5 declared in momiji.asm line 376
static unsigned char momijiFLD5_byteCode[] = {
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00, // FLD DWORD PTR [0x85B870] ; line 377
};
static PatchByteCode momijiFLD5_patchByteCode = { momijiFLD5_byteCode, 6, nullptr, 0 };
// momijiD421C_TO_B declared in momiji.asm line 387
static unsigned char momijiD421C_TO_B_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x49EE20 ; line 388
};
static JumpTarget momijiD421C_TO_B_jumpData[] = {
	{ 0x001, 0x49EE20 },
};
static PatchByteCode momijiD421C_TO_B_patchByteCode = { momijiD421C_TO_B_byteCode, 5, momijiD421C_TO_B_jumpData, 1 };
// momijiD421EFF declared in momiji.asm line 391
static unsigned char momijiD421EFF_byteCode[] = {
	0x66, 0x81, 0xF9, 0x1C, 0x02,       // CMP CX,0x021C ; line 392
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF, // JL 0x4B4795 ; line 393
	0x66, 0x81, 0xF9, 0x1E, 0x02,       // CMP CX,0x021E ; line 394
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x4B1321 ; line 395
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x4B4795 ; line 396
};
static JumpTarget momijiD421EFF_jumpData[] = {
	{ 0x007, 0x4B4795 },
	{ 0x012, 0x4B1321 },
	{ 0x017, 0x4B4795 },
};
static PatchByteCode momijiD421EFF_patchByteCode = { momijiD421EFF_byteCode, 27, momijiD421EFF_jumpData, 3 };
// momiji5B_METER1 declared in momiji.asm line 400
static unsigned char momiji5B_METER1_byteCode[] = {
	0x6A, 0x02,             // PUSH 0x02 ; line 401
	0x89, 0xF1,             // MOV ECX,ESI ; line 402
	0xDB, 0x44, 0x24, 0x18, // FILD DWORD PTR [ESP+0x18] ; line 403
};
static PatchByteCode momiji5B_METER1_patchByteCode = { momiji5B_METER1_byteCode, 8, nullptr, 0 };
// momiji5B_METER2 declared in momiji.asm line 408
static unsigned char momiji5B_METER2_byteCode[] = {
	0x6A, 0x02,                   // PUSH 0x02 ; line 409
	0x89, 0xF1,                   // MOV ECX,ESI ; line 410
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 411
};
static JumpTarget momiji5B_METER2_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode momiji5B_METER2_patchByteCode = { momiji5B_METER2_byteCode, 9, momiji5B_METER2_jumpData, 1 };
// momiji5B_METER3 declared in momiji.asm line 416
static unsigned char momiji5B_METER3_byteCode[] = {
	0x6A, 0x02,                         // PUSH 0x02 ; line 417
	0x89, 0xF1,                         // MOV ECX,ESI ; line 418
	0x89, 0xBE, 0x90, 0x01, 0x00, 0x00, // MOV [ESI+0x190],EDI ; line 419
};
static PatchByteCode momiji5B_METER3_patchByteCode = { momiji5B_METER3_byteCode, 10, nullptr, 0 };
// momiji5B_METER4 declared in momiji.asm line 424
static unsigned char momiji5B_METER4_byteCode[] = {
	0x6A, 0x02,                         // PUSH 0x02 ; line 425
	0x89, 0xF1,                         // MOV ECX,ESI ; line 426
	0x89, 0x96, 0x90, 0x01, 0x00, 0x00, // MOV [ESI+0x190],EDX ; line 427
};
static PatchByteCode momiji5B_METER4_patchByteCode = { momiji5B_METER4_byteCode, 10, nullptr, 0 };
// momiji5B_NEW declared in momiji.asm line 433
static unsigned char momiji5B_NEW_byteCode[] = {
	0x81, 0xE8, 0x02, 0x00, 0x00, 0x00, // SUB EAX,0x02 ; line 434
	0xBA, 0x14, 0x00, 0x00, 0x00,       // MOV EDX,0x00000014 ; line 435
	0xF7, 0xEA,                         // IMUL EDX ; line 436
	0x81, 0xC0, 0x2F, 0x00, 0x00, 0x00, // ADD EAX,0x2F ; line 437
	0x89, 0xC2,                         // MOV EDX,EAX ; line 438
};
static PatchByteCode momiji5B_NEW_patchByteCode = { momiji5B_NEW_byteCode, 21, nullptr, 0 };
// momiji6B_NEW declared in momiji.asm line 440
static unsigned char momiji6B_NEW_byteCode[] = {
	0x81, 0xE8, 0x03, 0x00, 0x00, 0x00, // SUB EAX,0x03 ; line 441
	0xBA, 0x14, 0x00, 0x00, 0x00,       // MOV EDX,0x00000014 ; line 442
	0xF7, 0xEA,                         // IMUL EDX ; line 443
	0x81, 0xC0, 0x2F, 0x00, 0x00, 0x00, // ADD EAX,0x2F ; line 444
	0xBA, 0x00, 0x00, 0x00, 0x00,       // MOV EDX,0x00000000 ; line 445
	0x29, 0xC2,                         // SUB EDX,EAX ; line 446
	0x89, 0xD0,                         // MOV EAX,EDX ; line 447
};
static PatchByteCode momiji6B_NEW_patchByteCode = { momiji6B_NEW_byteCode, 28, nullptr, 0 };
// momiji5B_NEWCHARGE declared in momiji.asm line 449
static unsigned char momiji5B_NEWCHARGE_byteCode[] = {
	0x81, 0xE8, 0x02, 0x00, 0x00, 0x00, // SUB EAX,0x02 ; line 450
	0xBA, 0x12, 0x00, 0x00, 0x00,       // MOV EDX,0x00000012 ; line 451
	0xF7, 0xEA,                         // IMUL EDX ; line 452
	0x81, 0xC0, 0x2F, 0x00, 0x00, 0x00, // ADD EAX,0x2F ; line 453
	0x89, 0xC2,                         // MOV EDX,EAX ; line 454
};
static PatchByteCode momiji5B_NEWCHARGE_patchByteCode = { momiji5B_NEWCHARGE_byteCode, 21, nullptr, 0 };
// momiji6B_NEWCHARGE declared in momiji.asm line 456
static unsigned char momiji6B_NEWCHARGE_byteCode[] = {
	0x81, 0xE8, 0x03, 0x00, 0x00, 0x00, // SUB EAX,0x03 ; line 457
	0xBA, 0x12, 0x00, 0x00, 0x00,       // MOV EDX,0x00000012 ; line 458
	0xF7, 0xEA,                         // IMUL EDX ; line 459
	0x81, 0xC0, 0x2F, 0x00, 0x00, 0x00, // ADD EAX,0x2F ; line 460
	0xBA, 0x00, 0x00, 0x00, 0x00,       // MOV EDX,0x00000000 ; line 461
	0x29, 0xC2,                         // SUB EDX,EAX ; line 462
	0x89, 0xD0,                         // MOV EAX,EDX ; line 463
};
static PatchByteCode momiji6B_NEWCHARGE_patchByteCode = { momiji6B_NEWCHARGE_byteCode, 28, nullptr, 0 };
// momiji5B_DIST declared in momiji.asm line 472
static unsigned char momiji5B_DIST_byteCode[] = {
	0x6A, 0x03,       // PUSH 0x03 ; line 473
	0x6B, 0xD2, 0x52, // IMUL EDX,EDX,0x52 ; line 474
};
static PatchByteCode momiji5B_DIST_patchByteCode = { momiji5B_DIST_byteCode, 5, nullptr, 0 };
// momiji5B_DIST1 declared in momiji.asm line 476
static unsigned char momiji5B_DIST1_byteCode[] = {
	0x0F, 0xBE, 0xD0, // MOVSX EDX,AL ; line 477
	0x6B, 0xD2, 0x52, // IMUL EDX,EDX,0x52 ; line 478
};
static PatchByteCode momiji5B_DIST1_patchByteCode = { momiji5B_DIST1_byteCode, 6, nullptr, 0 };
// momiji5B_DIST2 declared in momiji.asm line 480
static unsigned char momiji5B_DIST2_byteCode[] = {
	0xD9, 0x44, 0x24, 0x24, // FLD DWORD PTR [ESP+0x24] ; line 481
	0x6B, 0xC0, 0x52,       // IMUL EAX,EAX,0x52 ; line 482
};
static PatchByteCode momiji5B_DIST2_patchByteCode = { momiji5B_DIST2_byteCode, 7, nullptr, 0 };
// momiji5B_DIST3 declared in momiji.asm line 484
static unsigned char momiji5B_DIST3_byteCode[] = {
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00, // SUB ESP,0x08 ; line 485
	0x6B, 0xD2, 0x52,                   // IMUL EDX,EDX,0x52 ; line 486
};
static PatchByteCode momiji5B_DIST3_patchByteCode = { momiji5B_DIST3_byteCode, 9, nullptr, 0 };
// momiji5B_COUNT declared in momiji.asm line 488
static unsigned char momiji5B_COUNT_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x12, 0x00, // CMP WORD PTR [ESI+0x7D2],0x12 ; line 489
};
static PatchByteCode momiji5B_COUNT_patchByteCode = { momiji5B_COUNT_byteCode, 9, nullptr, 0 };
// momiji5B_COUNTCHARGE declared in momiji.asm line 491
static unsigned char momiji5B_COUNTCHARGE_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x24, 0x00, // CMP WORD PTR [ESI+0x7D2],0x24 ; line 492
};
static PatchByteCode momiji5B_COUNTCHARGE_patchByteCode = { momiji5B_COUNTCHARGE_byteCode, 9, nullptr, 0 };
// momiji5B_YOFF declared in momiji.asm line 495
static unsigned char momiji5B_YOFF_byteCode[] = {
	0xDC, 0x05, 0x08, 0xBC, 0x85, 0x00, // FADD QWORD PTR [0x85BC08] ; line 496
};
static PatchByteCode momiji5B_YOFF_patchByteCode = { momiji5B_YOFF_byteCode, 6, nullptr, 0 };
// momiji5B_YOFFA declared in momiji.asm line 499
static unsigned char momiji5B_YOFFA_byteCode[] = {
	0xDC, 0x05, 0xE8, 0xBB, 0x85, 0x00, // FADD QWORD PTR [0x85BBE8] ; line 500
};
static PatchByteCode momiji5B_YOFFA_patchByteCode = { momiji5B_YOFFA_byteCode, 6, nullptr, 0 };
// momiji5B_SPD declared in momiji.asm line 503
static unsigned char momiji5B_SPD_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 504
};
static PatchByteCode momiji5B_SPD_patchByteCode = { momiji5B_SPD_byteCode, 2, nullptr, 0 };
// momiji5B_SPDCHARGE declared in momiji.asm line 506
static unsigned char momiji5B_SPDCHARGE_byteCode[] = {
	0xB9, 0x01, 0x00, 0x00, 0x00, // MOV ECX,0x00000001 ; line 507
};
static PatchByteCode momiji5B_SPDCHARGE_patchByteCode = { momiji5B_SPDCHARGE_byteCode, 5, nullptr, 0 };
// momiji5B_CHARGELEAP declared in momiji.asm line 510
static unsigned char momiji5B_CHARGELEAP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x495FC0 ; line 511
};
static JumpTarget momiji5B_CHARGELEAP_jumpData[] = {
	{ 0x001, 0x495FC0 },
};
static PatchByteCode momiji5B_CHARGELEAP_patchByteCode = { momiji5B_CHARGELEAP_byteCode, 5, momiji5B_CHARGELEAP_jumpData, 1 };
// momiji5B_CHARGELEAP1 declared in momiji.asm line 513
static unsigned char momiji5B_CHARGELEAP1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x49653A ; line 514
};
static JumpTarget momiji5B_CHARGELEAP1_jumpData[] = {
	{ 0x001, 0x49653A },
};
static PatchByteCode momiji5B_CHARGELEAP1_patchByteCode = { momiji5B_CHARGELEAP1_byteCode, 5, momiji5B_CHARGELEAP1_jumpData, 1 };
// momiji5B_CHARGELEAP2 declared in momiji.asm line 516
static unsigned char momiji5B_CHARGELEAP2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x496D32 ; line 517
};
static JumpTarget momiji5B_CHARGELEAP2_jumpData[] = {
	{ 0x001, 0x496D32 },
};
static PatchByteCode momiji5B_CHARGELEAP2_patchByteCode = { momiji5B_CHARGELEAP2_byteCode, 5, momiji5B_CHARGELEAP2_jumpData, 1 };
// momiji5B_CHARGELEAP3 declared in momiji.asm line 519
static unsigned char momiji5B_CHARGELEAP3_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x497310 ; line 520
};
static JumpTarget momiji5B_CHARGELEAP3_jumpData[] = {
	{ 0x001, 0x497310 },
};
static PatchByteCode momiji5B_CHARGELEAP3_patchByteCode = { momiji5B_CHARGELEAP3_byteCode, 5, momiji5B_CHARGELEAP3_jumpData, 1 };
// momiji5B_NODOUBLE declared in momiji.asm line 522
static unsigned char momiji5B_NODOUBLE_byteCode[] = {
	0x81, 0xED, 0x02, 0x00, 0x00, 0x00, // SUB EBP,0x02 ; line 523
};
static PatchByteCode momiji5B_NODOUBLE_patchByteCode = { momiji5B_NODOUBLE_byteCode, 6, nullptr, 0 };
// momiji5B_FIREFREQ declared in momiji.asm line 525
static unsigned char momiji5B_FIREFREQ_byteCode[] = {
	0xBA, 0x01, 0x00, 0x00, 0x00, // MOV EDX,0x01 ; line 526
};
static PatchByteCode momiji5B_FIREFREQ_patchByteCode = { momiji5B_FIREFREQ_byteCode, 5, nullptr, 0 };
// momiji5B_CHARGEDUR declared in momiji.asm line 528
static unsigned char momiji5B_CHARGEDUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x28 ; line 529
};
static PatchByteCode momiji5B_CHARGEDUR_patchByteCode = { momiji5B_CHARGEDUR_byteCode, 10, nullptr, 0 };
// momiji5B_DUR declared in momiji.asm line 536
static unsigned char momiji5B_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x12 ; line 537
};
static PatchByteCode momiji5B_DUR_patchByteCode = { momiji5B_DUR_byteCode, 10, nullptr, 0 };
// momijiFLIGHT declared in momiji.asm line 608
static unsigned char momijiFLIGHT_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 609
};
static PatchByteCode momijiFLIGHT_patchByteCode = { momijiFLIGHT_byteCode, 6, nullptr, 0 };
// momijiDASH declared in momiji.asm line 613
static unsigned char momijiDASH_byteCode[] = {
	0xD9, 0x05, 0xB8, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C5B8] ; line 614
};
static PatchByteCode momijiDASH_patchByteCode = { momijiDASH_byteCode, 6, nullptr, 0 };
// momijiC6A_X declared in momiji.asm line 619
static unsigned char momijiC6A_X_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 620
};
static PatchByteCode momijiC6A_X_patchByteCode = { momijiC6A_X_byteCode, 6, nullptr, 0 };
// momijiDBY declared in momiji.asm line 627
static unsigned char momijiDBY_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 628
};
static PatchByteCode momijiDBY_patchByteCode = { momijiDBY_byteCode, 6, nullptr, 0 };
// momijiDBX declared in momiji.asm line 632
static unsigned char momijiDBX_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 633
};
static PatchByteCode momijiDBX_patchByteCode = { momijiDBX_byteCode, 6, nullptr, 0 };
// momiji2A declared in momiji.asm line 639
static unsigned char momiji2A_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x495174 ; line 640
};
static JumpTarget momiji2A_jumpData[] = {
	{ 0x001, 0x495174 },
};
static PatchByteCode momiji2A_patchByteCode = { momiji2A_byteCode, 5, momiji2A_jumpData, 1 };
// momiji2A_HITS declared in momiji.asm line 644
static unsigned char momiji2A_HITS_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 645
};
static PatchByteCode momiji2A_HITS_patchByteCode = { momiji2A_HITS_byteCode, 7, nullptr, 0 };
// momiji2A_SELFCHAIN declared in momiji.asm line 649
static unsigned char momiji2A_SELFCHAIN_byteCode[] = {
	0x66, 0x8B, 0x86, 0x3C, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x13C] ; line 650
	0x66, 0x81, 0xF8, 0x2F, 0x01,             // CMP AX,0x012F ; line 651
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 652
	0x68, 0x2F, 0x01, 0x00, 0x00,             // PUSH 0x0000012F ; line 653
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x4B04B2 ; line 654
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x4B04BE ; line 656
};
static JumpTarget momiji2A_SELFCHAIN_jumpData[] = {
	{ 0x018, 0x4B04B2 },
	{ 0x01D, 0x4B04BE },
};
static PatchByteCode momiji2A_SELFCHAIN_patchByteCode = { momiji2A_SELFCHAIN_byteCode, 33, momiji2A_SELFCHAIN_jumpData, 2 };
// momiji2B_RANGE declared in momiji.asm line 661
static unsigned char momiji2B_RANGE_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x64 ; line 662
};
static PatchByteCode momiji2B_RANGE_patchByteCode = { momiji2B_RANGE_byteCode, 10, nullptr, 0 };
// momijiD214_GRAZE declared in momiji.asm line 668
static unsigned char momijiD214_GRAZE_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x4B32EF ; line 669
};
static JumpTarget momijiD214_GRAZE_jumpData[] = {
	{ 0x001, 0x4B32EF },
};
static PatchByteCode momijiD214_GRAZE_patchByteCode = { momijiD214_GRAZE_byteCode, 5, momijiD214_GRAZE_jumpData, 1 };
// momijiD623_INVLEVEL declared in momiji.asm line 674
static unsigned char momijiD623_INVLEVEL_byteCode[] = {
	0x80, 0xBE, 0xA7, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6A7],0x01 ; line 675
};
static PatchByteCode momijiD623_INVLEVEL_patchByteCode = { momijiD623_INVLEVEL_byteCode, 7, nullptr, 0 };
// momijiFH_0 declared in momiji.asm line 691
static unsigned char momijiFH_0_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x490B4B ; line 692
	0x81, 0xF8, 0x07, 0x00, 0x00, 0x00, // CMP EAX,0x07 ; line 693
};
static JumpTarget momijiFH_0_jumpData[] = {
	{ 0x001, 0x490B4B },
};
static PatchByteCode momijiFH_0_patchByteCode = { momijiFH_0_byteCode, 11, momijiFH_0_jumpData, 1 };
// momijiFH_DUR declared in momiji.asm line 697
static unsigned char momijiFH_DUR_byteCode[] = {
	0x66, 0xC7, 0x86, 0xB4, 0x08, 0x00, 0x00, 0x58, 0x02, // MOV WORD PTR [ESI+0x8B4],0x0258 ; line 698
};
static PatchByteCode momijiFH_DUR_patchByteCode = { momijiFH_DUR_byteCode, 9, nullptr, 0 };
// momijiFH declared in momiji.asm line 702
static unsigned char momijiFH_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 703
	0x60,                                                       // PUSHAD ; line 704
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 705
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 706
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 707
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 708
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 710
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 711
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000000 ; line 712
	0xDB, 0x86, 0x44, 0x01, 0x00, 0x00,                         // FILD DWORD PTR [ESI+0x144] ; line 714
	0x61,                                                       // POPAD ; line 715
	0x66, 0x9D,                                                 // POPF ; line 716
};
static PatchByteCode momijiFH_patchByteCode = { momijiFH_byteCode, 68, nullptr, 0 };
// momijiFH_2 declared in momiji.asm line 721
static unsigned char momijiFH_2_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000000 ; line 722
};
static PatchByteCode momijiFH_2_patchByteCode = { momijiFH_2_byteCode, 10, nullptr, 0 };


// momiji.asm
std::array<PatchSkeleton, 114> patchList = {
	PatchSkeleton{ momijiID_FIX_patchByteCode,         (void *)0x490B85,  8}, // Declared line 18. Patch on CMP WORD PTR [ESI+0x196], 0
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4B1468,  6}, // Declared line 22. Patch on FADD ST, QWORD PTR [0x859630]
	PatchSkeleton{ momiji5C_ACCEL_patchByteCode,       (void *)0x4B14E6,  6}, // Declared line 59. Patch on FADD ST, QWORD PTR [0x85B938]
	PatchSkeleton{ momiji6D_MIN_patchByteCode,         (void *)0x49271E,  6}, // Declared line 65. Patch on CMP AX, 5
	PatchSkeleton{ momijiD214_ONHIT_patchByteCode,     (void *)0x4B30B5,  6}, // Declared line 92. Patch on CMP [ESI+0x190], EBP
	PatchSkeleton{ momiji2C_ONHIT_patchByteCode,       (void *)0x4B1C98,  8}, // Declared line 93. Patch on CMP WORD PTR [ESI+0x13E], 0
	PatchSkeleton{ momijiSHIELDA_patchByteCode,        (void *)0x4B5754,  9}, // Declared line 290. Patch on CMP WORD PTR [EAX+0x13C], 0x12E
	PatchSkeleton{ momijiSHIELDB_patchByteCode,        (void *)0x4B5763,  8}, // Declared line 291. Patch on CMP WORD PTR [EAX+0x140], 0
	PatchSkeleton{ momijiSHIELD1_patchByteCode,        (void *)0x495632,  6}, // Declared line 293. Patch on FSUB ST, DWORD PTR [ESI+0x100]
	PatchSkeleton{ momijiSHIELD2_patchByteCode,        (void *)0x49738E,  7}, // Declared line 294. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ momijiSHIELD3_patchByteCode,        (void *)0x494E8E,  6}, // Declared line 295. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ momijiSHIELD33_patchByteCode,       (void *)0x494FDD,  9}, // Declared line 296. Patch on FLDZ
	PatchSkeleton{ momijiSHIELD4_patchByteCode,        (void *)0x4A094F,  7}, // Declared line 297. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ momijiSHIELD4_patchByteCode,        (void *)0x4A0F1C,  7}, // Declared line 298. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ momijiC6A_SKIP_patchByteCode,       (void *)0x494D9D,  9}, // Declared line 323. Patch on MOV ECX, ESI
	PatchSkeleton{ momijiJ6A_SKIP_patchByteCode,       (void *)0x49562C,  6}, // Declared line 346. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ momijiD421_SPAWN_patchByteCode,     (void *)0x49F01C,  6}, // Declared line 380. Patch on IMUL ECX, 0xB4
	PatchSkeleton{ momijiFLD5_patchByteCode,           (void *)0x49F046,  6}, // Declared line 381. Patch on FLD ST, DWORD PTR [0x85BCA4]
	PatchSkeleton{ momijiD421_SPAWNY_patchByteCode,    (void *)0x49F03A,  6}, // Declared line 382. Patch on FLD ST, DWORD PTR [0x859184]
	PatchSkeleton{ momijiFLD5_patchByteCode,           (void *)0x49F14E,  6}, // Declared line 383. Patch on FLD ST, DWORD PTR [0x85BC30]
	PatchSkeleton{ momijiD421_JUMPY_patchByteCode,     (void *)0x49F052,  6}, // Declared line 384. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ momijiD421_JUMPYDOWN_patchByteCode, (void *)0x49F15E,  6}, // Declared line 385. Patch on FLD ST, DWORD PTR [0x85BB58]
	PatchSkeleton{ momijiD421_JUMPYGRAV_patchByteCode, (void *)0x49EF77,  6}, // Declared line 386. Patch on FSUB ST, QWORD PTR [0x85A288]
	PatchSkeleton{ momijiD421C_TO_B_patchByteCode,     (void *)0x49F177,  7}, // Declared line 390. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ momijiD421EFF_patchByteCode,        (void *)0x4B4783,  5}, // Declared line 398. Patch on CMP CX, 0x21C
	PatchSkeleton{ momiji5B_METER1_patchByteCode,      (void *)0x495E89,  8}, // Declared line 405. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER1_patchByteCode,      (void *)0x495F5E,  8}, // Declared line 406. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER2_patchByteCode,      (void *)0x4963D3,  9}, // Declared line 413. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER2_patchByteCode,      (void *)0x4964C0,  9}, // Declared line 414. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER3_patchByteCode,      (void *)0x496BB2, 10}, // Declared line 421. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER3_patchByteCode,      (void *)0x4971B5, 10}, // Declared line 422. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER4_patchByteCode,      (void *)0x496CA1, 10}, // Declared line 429. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_METER4_patchByteCode,      (void *)0x49727A, 10}, // Declared line 430. Patch on PUSH 0xA
	PatchSkeleton{ momiji5B_NEW_patchByteCode,         (void *)0x495E7F,  6}, // Declared line 540. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x495E46,  6}, // Declared line 542. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_DIST_patchByteCode,        (void *)0x495ECF,  5}, // Declared line 543. Patch on PUSH 3
	PatchSkeleton{ momiji5B_COUNT_patchByteCode,       (void *)0x495E4C,  8}, // Declared line 544. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_SPD_patchByteCode,         (void *)0x495E9E,  6}, // Declared line 545. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ momiji5B_DUR_patchByteCode,         (void *)0x495CC2,  6}, // Declared line 546. Patch on CMP [ESI+0x144], EBP
	PatchSkeleton{ momiji5B_NEWCHARGE_patchByteCode,   (void *)0x495F54,  6}, // Declared line 548. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_CHARGELEAP_patchByteCode,  (void *)0x495FA0,  8}, // Declared line 549. Patch on MOV [ESP+0x14], EAX
	PatchSkeleton{ momiji5B_NODOUBLE_patchByteCode,    (void *)0x496030,  9}, // Declared line 550. Patch on SUB EBP, 1
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x495F1B,  6}, // Declared line 551. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_COUNTCHARGE_patchByteCode, (void *)0x495F21,  8}, // Declared line 552. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_DIST2_patchByteCode,       (void *)0x495FFD,  7}, // Declared line 553. Patch on FLD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ momiji5B_SPDCHARGE_patchByteCode,   (void *)0x495FC0,  7}, // Declared line 554. Patch on MOV ECX, 0xF
	PatchSkeleton{ momiji6B_NEW_patchByteCode,         (void *)0x4963E3,  6}, // Declared line 557. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x4963A1,  6}, // Declared line 558. Patch on JNE 0x004965C8
	PatchSkeleton{ momiji5B_DIST1_patchByteCode,       (void *)0x496434,  6}, // Declared line 559. Patch on MOVSX EDX, AL
	PatchSkeleton{ momiji5B_COUNT_patchByteCode,       (void *)0x4963A7,  8}, // Declared line 560. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_SPD_patchByteCode,         (void *)0x49640B,  6}, // Declared line 561. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ momiji5B_YOFF_patchByteCode,        (void *)0x49642E,  6}, // Declared line 562. Patch on FADD ST, QWORD PTR [0x85BBF8]
	PatchSkeleton{ momiji5B_DUR_patchByteCode,         (void *)0x49621D,  6}, // Declared line 563. Patch on CMP [ESI+0x144], EBX
	PatchSkeleton{ momiji6B_NEWCHARGE_patchByteCode,   (void *)0x4964D0,  6}, // Declared line 565. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_CHARGELEAP1_patchByteCode, (void *)0x49651A,  8}, // Declared line 566. Patch on MOV [ESP+0x14], EAX
	PatchSkeleton{ momiji5B_NODOUBLE_patchByteCode,    (void *)0x4965AA,  9}, // Declared line 567. Patch on SUB EBP, 1
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x49648E,  6}, // Declared line 568. Patch on JNE 0x004965C8
	PatchSkeleton{ momiji5B_COUNTCHARGE_patchByteCode, (void *)0x496494,  8}, // Declared line 569. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_DIST2_patchByteCode,       (void *)0x496577,  7}, // Declared line 570. Patch on FLD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ momiji5B_SPDCHARGE_patchByteCode,   (void *)0x49653A,  7}, // Declared line 571. Patch on MOV ECX, 0xF
	PatchSkeleton{ momiji5B_YOFF_patchByteCode,        (void *)0x496569,  6}, // Declared line 572. Patch on FADD ST, QWORD PTR [0x85BBF8]
	PatchSkeleton{ momiji5B_NEW_patchByteCode,         (void *)0x496BE0,  6}, // Declared line 575. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x496B9C,  6}, // Declared line 576. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_DIST3_patchByteCode,       (void *)0x496C33,  6}, // Declared line 577. Patch on SUB ESP, 8
	PatchSkeleton{ momiji5B_COUNT_patchByteCode,       (void *)0x496BA2,  8}, // Declared line 578. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_SPD_patchByteCode,         (void *)0x496C06,  6}, // Declared line 579. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ momiji5B_NEWCHARGE_patchByteCode,   (void *)0x496CCF,  6}, // Declared line 581. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_CHARGELEAP2_patchByteCode, (void *)0x496D12,  8}, // Declared line 582. Patch on MOV [ESP+0x14], EAX
	PatchSkeleton{ momiji5B_NODOUBLE_patchByteCode,    (void *)0x496DA2,  9}, // Declared line 583. Patch on SUB EBP, 1
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x496C8D,  6}, // Declared line 584. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_COUNTCHARGE_patchByteCode, (void *)0x496C93,  8}, // Declared line 585. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_DIST2_patchByteCode,       (void *)0x496D6F,  7}, // Declared line 586. Patch on FLD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ momiji5B_SPDCHARGE_patchByteCode,   (void *)0x496D32,  7}, // Declared line 587. Patch on MOV ECX, 0xF
	PatchSkeleton{ momiji5B_CHARGEDUR_patchByteCode,   (void *)0x496B22,  7}, // Declared line 588. Patch on CMP DWORD PTR [ESI+0x144], 0x14
	PatchSkeleton{ momiji6B_NEW_patchByteCode,         (void *)0x4971E3,  6}, // Declared line 590. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x49719F,  6}, // Declared line 591. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_DIST3_patchByteCode,       (void *)0x497236,  6}, // Declared line 592. Patch on SUB ESP, 8
	PatchSkeleton{ momiji5B_COUNT_patchByteCode,       (void *)0x4971A5,  8}, // Declared line 593. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_SPD_patchByteCode,         (void *)0x497209,  6}, // Declared line 594. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ momiji5B_YOFFA_patchByteCode,       (void *)0x49722C,  6}, // Declared line 595. Patch on FADD ST, QWORD PTR [0x85BBD8]
	PatchSkeleton{ momiji6B_NEWCHARGE_patchByteCode,   (void *)0x4972A8,  6}, // Declared line 597. Patch on SUB EAX, 2
	PatchSkeleton{ momiji5B_CHARGELEAP3_patchByteCode, (void *)0x4972F0,  8}, // Declared line 598. Patch on MOV [ESP+0x14], EAX
	PatchSkeleton{ momiji5B_NODOUBLE_patchByteCode,    (void *)0x497380,  9}, // Declared line 599. Patch on SUB EBP, 1
	PatchSkeleton{ momiji5B_FIREFREQ_patchByteCode,    (void *)0x497266,  6}, // Declared line 600. Patch on JNE 0x0049604A
	PatchSkeleton{ momiji5B_COUNTCHARGE_patchByteCode, (void *)0x49726C,  8}, // Declared line 601. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ momiji5B_DIST2_patchByteCode,       (void *)0x49734D,  7}, // Declared line 602. Patch on FLD ST, DWORD PTR [ESP+0x24]
	PatchSkeleton{ momiji5B_SPDCHARGE_patchByteCode,   (void *)0x497310,  7}, // Declared line 603. Patch on MOV ECX, 0xF
	PatchSkeleton{ momiji5B_YOFFA_patchByteCode,       (void *)0x49733F,  6}, // Declared line 604. Patch on FADD ST, QWORD PTR [0x85BBD8]
	PatchSkeleton{ momiji5B_CHARGEDUR_patchByteCode,   (void *)0x497126,  6}, // Declared line 605. Patch on CMP [ESI+0x144], EBP
	PatchSkeleton{ momijiFLIGHT_patchByteCode,         (void *)0x4937D9,  6}, // Declared line 611. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ momijiDASH_patchByteCode,           (void *)0x4925C4,  6}, // Declared line 616. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ momijiC6A_X_patchByteCode,          (void *)0x494FAD,  6}, // Declared line 622. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ momijiFLDZ_patchByteCode,           (void *)0x497499,  6}, // Declared line 625. Patch on FLD ST, DWORD PTR [0x85AD0C]
	PatchSkeleton{ momijiDBY_patchByteCode,            (void *)0x4974A5,  6}, // Declared line 630. Patch on FLD ST, DWORD PTR [0x85B9C0]
	PatchSkeleton{ momijiDBX_patchByteCode,            (void *)0x4ACF31,  6}, // Declared line 635. Patch on FLD ST, DWORD PTR [0x85B9D4]
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4950E2, 10}, // Declared line 638. Patch on MOV DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ momiji2A_patchByteCode,             (void *)0x49510D,  8}, // Declared line 642. Patch on CMP WORD PTR [ESI+0x142], 0
	PatchSkeleton{ momiji2A_HITS_patchByteCode,        (void *)0x4ACDAD,  7}, // Declared line 647. Patch on MOV BYTE PTR [ESI+0x194], 6
	PatchSkeleton{ momiji2A_SELFCHAIN_patchByteCode,   (void *)0x4B04AD,  5}, // Declared line 658. Patch on PUSH 0x12F
	PatchSkeleton{ momiji2B_RANGE_patchByteCode,       (void *)0x4B1C17,  7}, // Declared line 664. Patch on CMP DWORD PTR [ESI+0x144], 0xA
	PatchSkeleton{ momijiD214_GRAZE_patchByteCode,     (void *)0x4B32DB,  5}, // Declared line 671. Patch on PUSH 0xA
	PatchSkeleton{ momijiD623_INVLEVEL_patchByteCode,  (void *)0x4A0663,  7}, // Declared line 677. Patch on CMP BYTE PTR [ESI+0x6A7], 4
	PatchSkeleton{ momijiD623_INVLEVEL_patchByteCode,  (void *)0x4AD25C,  7}, // Declared line 678. Patch on CMP BYTE PTR [ESI+0x6A7], 4
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4A15A6,  6}, // Declared line 681. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4A1801,  6}, // Declared line 682. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4A19E1,  6}, // Declared line 683. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4A1BCD,  6}, // Declared line 684. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ momijiNOP_patchByteCode,            (void *)0x4B7500,  6}, // Declared line 687. Patch on FADD ST, QWORD PTR [0x857F90]
	PatchSkeleton{ momijiFH_0_patchByteCode,           (void *)0x490AE6,  5}, // Declared line 695. Patch on JE SHORT 0x00490AED
	PatchSkeleton{ momijiFH_DUR_patchByteCode,         (void *)0x4A42A6,  9}, // Declared line 700. Patch on MOV WORD PTR [ESI+0x8B4], 0x384
	PatchSkeleton{ momijiFH_patchByteCode,             (void *)0x4B7A84,  6}, // Declared line 719. Patch on FILD ST, DWORD PTR [ESI+0x144]
	PatchSkeleton{ momijiFH_2_patchByteCode,           (void *)0x4B7A72,  7}, // Declared line 724. Patch on ADD DWORD PTR [ESI+0x34C], 0xFFFFFFFF
	PatchSkeleton{ momijiFH_2_patchByteCode,           (void *)0x4B7CEE,  6}, // Declared line 725. Patch on ADD [ESI+0x34C], EBX
	PatchSkeleton{ momijiFH_2_patchByteCode,           (void *)0x4B7FC6,  6}, // Declared line 726. Patch on ADD [ESI+0x34C], EBX
};


static std::array<unsigned, 96> skeletonsForUpdate = {
	   0,   3,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
	  22,  23,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
	  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
	  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
	  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,
	  87,  88,  89,  90,  91,  92,  93,  95,  96, 101, 103, 104, 105, 106, 108, 109,
};
static std::array<unsigned, 3> skeletonsForInitializeAction = { 94, 97, 102 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 1> skeletonsForHandleInputs = { 98 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 14> skeletonsForObjectUpdate = { 1, 2, 4, 5, 6, 7, 24, 99, 100, 107, 110, 111, 112, 113 };
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
	printf("Internal memory used %zu/4714\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 4714);

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
