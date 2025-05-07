#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<14632> applyMemory;
StackedMemory<5532> internalMemory;
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


// orinFLDZ declared in orin.asm line 1
static unsigned char orinFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode orinFLDZ_patchByteCode = { orinFLDZ_byteCode, 2, nullptr, 0 };
// orinFLD1 declared in orin.asm line 4
static unsigned char orinFLD1_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 5
};
static PatchByteCode orinFLD1_patchByteCode = { orinFLD1_byteCode, 2, nullptr, 0 };
// orinFMULZERO declared in orin.asm line 7
static unsigned char orinFMULZERO_byteCode[] = {
	0xDC, 0x0D, 0x2C, 0x81, 0x86, 0x00, // FMUL QWORD PTR [0x86812C] ; line 8
};
static PatchByteCode orinFMULZERO_patchByteCode = { orinFMULZERO_byteCode, 6, nullptr, 0 };
// orinNOP declared in orin.asm line 10
static unsigned char orinNOP_byteCode[] = {
	0x90, // NOP ; line 11
};
static PatchByteCode orinNOP_patchByteCode = { orinNOP_byteCode, 1, nullptr, 0 };
// orinID_FIX declared in orin.asm line 15
static unsigned char orinID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000019 ; line 16
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x0000 ; line 18
};
static PatchByteCode orinID_FIX_patchByteCode = { orinID_FIX_byteCode, 19, nullptr, 0 };
// orin2SC_AIR_FIX declared in orin.asm line 22
static unsigned char orin2SC_AIR_FIX_byteCode[] = {
	0x8B, 0x42, 0x0C, // MOV EAX,[EDX+0x0C] ; line 23
	0x6A, 0x05,       // PUSH 0x05 ; line 24
};
static PatchByteCode orin2SC_AIR_FIX_patchByteCode = { orin2SC_AIR_FIX_byteCode, 5, nullptr, 0 };
// orinD214_NEW declared in orin.asm line 28
static unsigned char orinD214_NEW_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 29
	0x0F, 0x85, 0x2A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 30
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x0005 ; line 31
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 32
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0B, 0x00, // CMP WORD PTR [ESI+0x140],0x000B ; line 33
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 34
	0x0F, 0x85, 0x06, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 35
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x6EB1A1 ; line 37
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6EB2CA ; line 39
};
static JumpTarget orinD214_NEW_jumpData[] = {
	{ 0x035, 0x6EB1A1 },
	{ 0x03B, 0x6EB2CA },
};
static PatchByteCode orinD214_NEW_patchByteCode = { orinD214_NEW_byteCode, 63, orinD214_NEW_jumpData, 2 };
// orinD214_NEWBASE declared in orin.asm line 42
static unsigned char orinD214_NEWBASE_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 43
	0x60,                                                       // PUSHAD ; line 44
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 46
	0x0F, 0x85, 0x2C, 0x00, 0x00, 0x00,                         // JNE SHORT LABELC ; line 47
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00,       // CMP WORD PTR [ESI+0x140],0x07 ; line 48
	0x0F, 0x85, 0x1D, 0x00, 0x00, 0x00,                         // JNE SHORT LABELC ; line 49
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 50
	0x0F, 0x85, 0x0E, 0x00, 0x00, 0x00,                         // JNE SHORT LABELC ; line 51
	0x6A, 0x2D,                                                 // PUSH 0x2D ; line 52
	0x68, 0xC8, 0x00, 0x00, 0x00,                               // PUSH 0x000000C8 ; line 53
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 54
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x47A9E0 ; line 55
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0001 ; line 58
	0x0F, 0x85, 0x77, 0x01, 0x00, 0x00,                         // JNE LABELZ ; line 59
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x104],0x01 ; line 60
	0x0F, 0x85, 0x15, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 61
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 63
	0x0F, 0x8C, 0x52, 0x00, 0x00, 0x00,                         // JL SHORT LABELA1 ; line 64
	0xE9, 0x15, 0x00, 0x00, 0x00,                               // JMP SHORT LABELA0 ; line 65
	0x81, 0xBE, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x754],0x00 ; line 68
	0x0F, 0x8F, 0x3D, 0x00, 0x00, 0x00,                         // JG SHORT LABELA1 ; line 69
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP SHORT LABELA0 ; line 70
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 73
	0xD8, 0x05, 0xC0, 0xB9, 0x85, 0x00,                         // FADD DWORD PTR [0x85B9C0] ; line 74
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 75
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x06, 0x00,       // CMP WORD PTR [ESI+0x140],0x0006 ; line 76
	0x0F, 0x8C, 0x5E, 0x00, 0x00, 0x00,                         // JL LABEL1 ; line 77
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // MOV WORD PTR [ESI+0x140],0x0000 ; line 78
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x142],0x00FF ; line 79
	0xE9, 0x47, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 80
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 83
	0xD8, 0x25, 0xC0, 0xB9, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B9C0] ; line 84
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 85
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0D, 0x00,       // CMP WORD PTR [ESI+0x140],0x000D ; line 86
	0x0F, 0x84, 0x0F, 0x00, 0x00, 0x00,                         // JE LABELA2 ; line 87
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x06, 0x00,       // CMP WORD PTR [ESI+0x140],0x0006 ; line 88
	0x0F, 0x8D, 0x17, 0x00, 0x00, 0x00,                         // JGE LABEL1 ; line 89
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00,       // MOV WORD PTR [ESI+0x140],0x0007 ; line 91
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xFF, 0x00,       // MOV WORD PTR [ESI+0x142],0x00FF ; line 92
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 93
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x08, 0x02,       // CMP WORD PTR [ESI+0x13C],0x0208 ; line 96
	0x0F, 0x85, 0x59, 0x00, 0x00, 0x00,                         // JNE LABELVER ; line 97
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x86782C] ; line 98
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 99
	0xDE, 0xD9,                                                 // FCOMPP ; line 100
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 101
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 102
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE SHORT LABELX ; line 103
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 104
	0xD9, 0x05, 0x08, 0xB9, 0x85, 0x00,                         // FLD DWORD PTR [0x85B908] ; line 105
	0xDE, 0xD9,                                                 // FCOMPP ; line 106
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 107
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 108
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABELY ; line 109
	0xE9, 0x7B, 0x00, 0x00, 0x00,                               // JMP LABELZ ; line 110
	0xD9, 0x05, 0x08, 0xB9, 0x85, 0x00,                         // FLD DWORD PTR [0x85B908] ; line 112
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 113
	0xE9, 0x6A, 0x00, 0x00, 0x00,                               // JMP SHORT LABELZ ; line 114
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x86782C] ; line 116
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 117
	0xE9, 0x59, 0x00, 0x00, 0x00,                               // JMP LABELZ ; line 118
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                         // FLD DWORD PTR [0x85B3B0] ; line 121
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 122
	0xDE, 0xD9,                                                 // FCOMPP ; line 123
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 124
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 125
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE SHORT LABELX1 ; line 126
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 127
	0xDD, 0x05, 0x60, 0xB8, 0x85, 0x00,                         // FLD QWORD PTR [0x85B860] ; line 128
	0xDE, 0xD9,                                                 // FCOMPP ; line 129
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 130
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 131
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABELY1 ; line 132
	0xE9, 0x22, 0x00, 0x00, 0x00,                               // JMP LABELZ ; line 133
	0xDD, 0x05, 0x60, 0xB8, 0x85, 0x00,                         // FLD QWORD PTR [0x85B860] ; line 135
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 136
	0xE9, 0x11, 0x00, 0x00, 0x00,                               // JMP SHORT LABELZ ; line 137
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,                         // FLD DWORD PTR [0x85B3B0] ; line 139
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 140
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABELZ ; line 141
	0x61,                                                       // POPAD ; line 144
	0x66, 0x9D,                                                 // POPF ; line 145
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 147
	0xDD, 0xD8,                                                 // FSTP ST0 ; line 148
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487CA0 ; line 149
};
static JumpTarget orinD214_NEWBASE_jumpData[] = {
	{ 0x03A, 0x47A9E0 },
	{ 0x1CC, 0x487CA0 },
};
static PatchByteCode orinD214_NEWBASE_patchByteCode = { orinD214_NEWBASE_byteCode, 464, orinD214_NEWBASE_jumpData, 2 };
// orinD214_NEW1 declared in orin.asm line 153
static unsigned char orinD214_NEW1_byteCode[] = {
	0x66, 0x81, 0xB9, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ECX+0x13E],0x01 ; line 154
};
static PatchByteCode orinD214_NEW1_patchByteCode = { orinD214_NEW1_byteCode, 9, nullptr, 0 };
// orinD214_SPIRIT declared in orin.asm line 157
static unsigned char orinD214_SPIRIT_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EB1AE ; line 158
};
static JumpTarget orinD214_SPIRIT_jumpData[] = {
	{ 0x001, 0x6EB1AE },
};
static PatchByteCode orinD214_SPIRIT_patchByteCode = { orinD214_SPIRIT_byteCode, 5, orinD214_SPIRIT_jumpData, 1 };
// orinD214_CJUMP declared in orin.asm line 160
static unsigned char orinD214_CJUMP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EAFFD ; line 161
};
static JumpTarget orinD214_CJUMP_jumpData[] = {
	{ 0x001, 0x6EAFFD },
};
static PatchByteCode orinD214_CJUMP_patchByteCode = { orinD214_CJUMP_byteCode, 5, orinD214_CJUMP_jumpData, 1 };
// orinD214_CNOEND declared in orin.asm line 164
static unsigned char orinD214_CNOEND_byteCode[] = {
	0xDC, 0x1D, 0x94, 0xAE, 0x85, 0x00, // FCOMP QWORD PTR [0x85AE94] ; line 165
};
static PatchByteCode orinD214_CNOEND_patchByteCode = { orinD214_CNOEND_byteCode, 6, nullptr, 0 };
// orinD214_CNOEND1 declared in orin.asm line 168
static unsigned char orinD214_CNOEND1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x08, 0x02,       // CMP WORD PTR [ESI+0x13C],0x208 ; line 169
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 170
	0x81, 0xBE, 0x60, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x760],0x00 ; line 171
	0xE9, 0x0A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 172
	0x81, 0xBE, 0x64, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x764],0x00 ; line 174
	0x90,                                                       // NOP ; line 176
};
static PatchByteCode orinD214_CNOEND1_patchByteCode = { orinD214_CNOEND1_byteCode, 41, nullptr, 0 };
// orinD214_LV declared in orin.asm line 190
static unsigned char orinD214_LV_byteCode[] = {
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,       // FLD DWORD PTR [ESI+0x11C] ; line 191
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 192
	0x66, 0x9C,                               // PUSHF ; line 193
	0x60,                                     // PUSHAD ; line 194
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,       // MOV EAX,[ESI+0x398] ; line 195
	0x80, 0xB8, 0xA5, 0x06, 0x00, 0x00, 0x00, // CMP BYTE PTR [EAX+0x6A5],0x00 ; line 196
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE LV0 ; line 197
	0x80, 0xB8, 0xA5, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [EAX+0x6A5],0x01 ; line 198
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,       // JE LV1 ; line 199
	0x80, 0xB8, 0xA5, 0x06, 0x00, 0x00, 0x02, // CMP BYTE PTR [EAX+0x6A5],0x02 ; line 200
	0x0F, 0x84, 0x34, 0x00, 0x00, 0x00,       // JE LV2 ; line 201
	0x80, 0xB8, 0xA5, 0x06, 0x00, 0x00, 0x03, // CMP BYTE PTR [EAX+0x6A5],0x03 ; line 202
	0x0F, 0x84, 0x4A, 0x00, 0x00, 0x00,       // JE LV3 ; line 203
	0xE9, 0x74, 0x00, 0x00, 0x00,             // JMP LV4 ; line 204
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 206
	0xE9, 0x9F, 0x00, 0x00, 0x00,             // JMP END ; line 207
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 209
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 210
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 211
	0xE9, 0x88, 0x00, 0x00, 0x00,             // JMP END ; line 212
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 214
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 215
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 216
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 217
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 218
	0xE9, 0x65, 0x00, 0x00, 0x00,             // JMP END ; line 219
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 221
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 222
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 223
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 224
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 225
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 226
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 227
	0xE9, 0x36, 0x00, 0x00, 0x00,             // JMP END ; line 228
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 230
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 231
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 232
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00,       // FADD DWORD PTR [0x86782C] ; line 233
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF0] ; line 234
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 235
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 236
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 237
	0xD8, 0x05, 0x04, 0xAD, 0x85, 0x00,       // FADD DWORD PTR [0x85AD04] ; line 238
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0x11C] ; line 240
	0x61,                                     // POPAD ; line 241
	0x66, 0x9D,                               // POPF ; line 242
	0xD9, 0xE8,                               // FLD1 ; line 243
};
static PatchByteCode orinD214_LV_patchByteCode = { orinD214_LV_byteCode, 259, nullptr, 0 };
// orinD214_HP declared in orin.asm line 250
static unsigned char orinD214_HP_byteCode[] = {
	0x66, 0xC7, 0x86, 0x84, 0x01, 0x00, 0x00, 0xF4, 0x01, // MOV WORD PTR [ESI+0x184],0x01F4 ; line 251
};
static PatchByteCode orinD214_HP_patchByteCode = { orinD214_HP_byteCode, 9, nullptr, 0 };
// orinD214_ONHIT declared in orin.asm line 259
static unsigned char orinD214_ONHIT_byteCode[] = {
	0xD9, 0x05, 0xF4, 0xB8, 0x85, 0x00,                   // FLD DWORD PTR [0x85B8F4] ; line 261
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 262
	0xDE, 0xD9,                                           // FCOMPP ; line 263
	0xDF, 0xE0,                                           // FNSTSW AX ; line 264
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 265
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x70349C ; line 267
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 269
	0xD9, 0xEE,                                           // FLDZ ; line 270
	0xDE, 0xD9,                                           // FCOMPP ; line 271
	0xDF, 0xE0,                                           // FNSTSW AX ; line 272
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 273
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x70349C ; line 275
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x13E],0x05 ; line 277
	0x0F, 0x8C, 0x26, 0x00, 0x00, 0x00,                   // JL SHORT LABEL1 ; line 278
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x13E],0x07 ; line 279
	0x0F, 0x8F, 0x17, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 280
	0x60,                                                 // PUSHAD ; line 281
	0x6A, 0x02,                                           // PUSH 0x02 ; line 282
	0x6A, 0x02,                                           // PUSH 0x02 ; line 283
	0x6A, 0x00,                                           // PUSH 0x00 ; line 284
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 285
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x48CE90 ; line 286
	0x84, 0xC0,                                           // TEST AL,AL ; line 287
	0x61,                                                 // POPAD ; line 288
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x70349C ; line 290
	0x0F, 0xBF, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVSX EAX,WORD PTR [ESI+0x13E] ; line 292
};
static JumpTarget orinD214_ONHIT_jumpData[] = {
	{ 0x015, 0x70349C },
	{ 0x02A, 0x70349C },
	{ 0x056, 0x48CE90 },
	{ 0x05F, 0x70349C },
};
static PatchByteCode orinD214_ONHIT_patchByteCode = { orinD214_ONHIT_byteCode, 106, orinD214_ONHIT_jumpData, 4 };
// orinJ2B declared in orin.asm line 296
static unsigned char orinJ2B_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6E7DBC ; line 297
};
static JumpTarget orinJ2B_jumpData[] = {
	{ 0x001, 0x6E7DBC },
};
static PatchByteCode orinJ2B_patchByteCode = { orinJ2B_byteCode, 5, orinJ2B_jumpData, 1 };
// orinJ2B_ANG0 declared in orin.asm line 299
static unsigned char orinJ2B_ANG0_byteCode[] = {
	0x66, 0x9C,                               // PUSHF ; line 300
	0x60,                                     // PUSHAD ; line 301
	0x66, 0x8B, 0x86, 0x3C, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x13C] ; line 302
	0x66, 0x81, 0xF8, 0x96, 0x01,             // CMP AX,0x0196 ; line 303
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 304
	0xD9, 0x05, 0xE8, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B9E8] ; line 305
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 306
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,       // FLD DWORD PTR [0x85B3B0] ; line 308
	0x61,                                     // POPAD ; line 310
	0x66, 0x9D,                               // POPF ; line 311
};
static PatchByteCode orinJ2B_ANG0_patchByteCode = { orinJ2B_ANG0_byteCode, 41, nullptr, 0 };
// orinJ2B_ANG1 declared in orin.asm line 313
static unsigned char orinJ2B_ANG1_byteCode[] = {
	0x66, 0x9C,                               // PUSHF ; line 314
	0x60,                                     // PUSHAD ; line 315
	0x66, 0x8B, 0x86, 0x3C, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x13C] ; line 316
	0x66, 0x81, 0xF8, 0x96, 0x01,             // CMP AX,0x0196 ; line 317
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 318
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00,       // FLD DWORD PTR [0x85B3B0] ; line 319
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 320
	0xD9, 0x05, 0x40, 0x15, 0x87, 0x00,       // FLD DWORD PTR [0x871540] ; line 322
	0x61,                                     // POPAD ; line 324
	0x66, 0x9D,                               // POPF ; line 325
};
static PatchByteCode orinJ2B_ANG1_patchByteCode = { orinJ2B_ANG1_byteCode, 41, nullptr, 0 };
// orinFLOAT0 declared in orin.asm line 345
static unsigned char orinFLOAT0_byteCode[] = {
	0xF6, 0xC4, 0x01,             // TEST AH,0x01 ; line 346
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6E3121 ; line 347
};
static JumpTarget orinFLOAT0_jumpData[] = {
	{ 0x004, 0x6E3121 },
};
static PatchByteCode orinFLOAT0_patchByteCode = { orinFLOAT0_byteCode, 8, orinFLOAT0_jumpData, 1 };
// orinFLOAT1 declared in orin.asm line 351
static unsigned char orinFLOAT1_byteCode[] = {
	0x81, 0xB9, 0x58, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x758],0x00 ; line 352
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6E3118 ; line 353
};
static JumpTarget orinFLOAT1_jumpData[] = {
	{ 0x00B, 0x6E3118 },
};
static PatchByteCode orinFLOAT1_patchByteCode = { orinFLOAT1_byteCode, 15, orinFLOAT1_jumpData, 1 };
// orinFLOAT2 declared in orin.asm line 357
static unsigned char orinFLOAT2_byteCode[] = {
	0xF6, 0xC4, 0x41,             // TEST AH,0x41 ; line 358
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6E3196 ; line 359
};
static JumpTarget orinFLOAT2_jumpData[] = {
	{ 0x004, 0x6E3196 },
};
static PatchByteCode orinFLOAT2_patchByteCode = { orinFLOAT2_byteCode, 8, orinFLOAT2_jumpData, 1 };
// orin5C_SPD declared in orin.asm line 364
static unsigned char orin5C_SPD_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 365
};
static PatchByteCode orin5C_SPD_patchByteCode = { orin5C_SPD_byteCode, 6, nullptr, 0 };
// orin5C_ACC declared in orin.asm line 369
static unsigned char orin5C_ACC_byteCode[] = {
	0xDC, 0x0D, 0xC8, 0x77, 0x86, 0x00, // FMUL QWORD PTR [0x8677C8] ; line 370
};
static PatchByteCode orin5C_ACC_patchByteCode = { orin5C_ACC_byteCode, 6, nullptr, 0 };
// orin5C_DUR declared in orin.asm line 374
static unsigned char orin5C_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x7F ; line 375
};
static PatchByteCode orin5C_DUR_patchByteCode = { orin5C_DUR_byteCode, 10, nullptr, 0 };
// orinD623B_DUR declared in orin.asm line 381
static unsigned char orinD623B_DUR_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 382
	0x60,                                                       // PUSHAD ; line 383
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 384
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 385
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x33 ; line 386
	0x61,                                                       // POPAD ; line 388
	0x66, 0x9D,                                                 // POPF ; line 389
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x32 ; line 390
};
static PatchByteCode orinD623B_DUR_patchByteCode = { orinD623B_DUR_byteCode, 39, nullptr, 0 };
// orinD623C_DUR declared in orin.asm line 392
static unsigned char orinD623C_DUR_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 393
	0x60,                                                       // PUSHAD ; line 394
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 395
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 396
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x33 ; line 397
	0x61,                                                       // POPAD ; line 399
	0x66, 0x9D,                                                 // POPF ; line 400
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x32 ; line 401
};
static PatchByteCode orinD623C_DUR_patchByteCode = { orinD623C_DUR_byteCode, 39, nullptr, 0 };
// orinD623_GRAZERES declared in orin.asm line 406
static unsigned char orinD623_GRAZERES_byteCode[] = {
	0x6A, 0x02,                   // PUSH 0x02 ; line 407
	0x89, 0xF1,                   // MOV ECX,ESI ; line 408
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CDE0 ; line 409
	0x6A, 0x02,                   // PUSH 0x02 ; line 410
	0x89, 0xF1,                   // MOV ECX,ESI ; line 411
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CE30 ; line 412
};
static JumpTarget orinD623_GRAZERES_jumpData[] = {
	{ 0x005, 0x48CDE0 },
	{ 0x00E, 0x48CE30 },
};
static PatchByteCode orinD623_GRAZERES_patchByteCode = { orinD623_GRAZERES_byteCode, 18, orinD623_GRAZERES_jumpData, 2 };
// orinD623_RAD declared in orin.asm line 420
static unsigned char orinD623_RAD_byteCode[] = {
	0xD9, 0xE8,                         // FLD1 ; line 421
	0xD8, 0x05, 0x4C, 0xD9, 0x85, 0x00, // FADD DWORD PTR [0x85D94C] ; line 422
	0xDE, 0xC9,                         // FMULP ; line 423
};
static PatchByteCode orinD623_RAD_patchByteCode = { orinD623_RAD_byteCode, 10, nullptr, 0 };
// orin6C_DUR declared in orin.asm line 428
static unsigned char orin6C_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 429
};
static PatchByteCode orin6C_DUR_patchByteCode = { orin6C_DUR_byteCode, 10, nullptr, 0 };
// orin2C_GRAV_OLD declared in orin.asm line 480
static unsigned char orin2C_GRAV_OLD_byteCode[] = {
	0xD8, 0x35, 0x10, 0xC0, 0x85, 0x00, // FDIV DWORD PTR [0x85C010] ; line 481
};
static PatchByteCode orin2C_GRAV_OLD_patchByteCode = { orin2C_GRAV_OLD_byteCode, 6, nullptr, 0 };
// orin2C_HP declared in orin.asm line 489
static unsigned char orin2C_HP_byteCode[] = {
	0x66, 0x81, 0xBE, 0x84, 0x01, 0x00, 0x00, 0x0C, 0xFE, // CMP WORD PTR [ESI+0x184],0xFE0C ; line 490
};
static PatchByteCode orin2C_HP_patchByteCode = { orin2C_HP_byteCode, 9, nullptr, 0 };
// orinD236_SPD declared in orin.asm line 497
static unsigned char orinD236_SPD_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 498
};
static PatchByteCode orinD236_SPD_patchByteCode = { orinD236_SPD_byteCode, 6, nullptr, 0 };
// orinD236_SPD1 declared in orin.asm line 502
static unsigned char orinD236_SPD1_byteCode[] = {
	0xD9, 0x05, 0x00, 0x9F, 0x85, 0x00, // FLD DWORD PTR [0x859F00] ; line 503
};
static PatchByteCode orinD236_SPD1_patchByteCode = { orinD236_SPD1_byteCode, 6, nullptr, 0 };
// orinD236_MINCHARGE declared in orin.asm line 507
static unsigned char orinD236_MINCHARGE_byteCode[] = {
	0xBB, 0x00, 0x00, 0x00, 0x00, // MOV EBX,0x00000000 ; line 508
};
static PatchByteCode orinD236_MINCHARGE_patchByteCode = { orinD236_MINCHARGE_byteCode, 5, nullptr, 0 };
// orinD236_IDKFIX declared in orin.asm line 512
static unsigned char orinD236_IDKFIX_byteCode[] = {
	0x80, 0xF8, 0x03,                   // CMP AL,0x03 ; line 513
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00, // JL SHORT LABEL1 ; line 514
	0xB9, 0x0A, 0x00, 0x00, 0x00,       // MOV ECX,0x0000000A ; line 515
	0x90,                               // NOP ; line 517
};
static PatchByteCode orinD236_IDKFIX_patchByteCode = { orinD236_IDKFIX_byteCode, 15, nullptr, 0 };
// orinJ6A_FIX declared in orin.asm line 527
static unsigned char orinJ6A_FIX_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x140],0x07 ; line 528
};
static PatchByteCode orinJ6A_FIX_patchByteCode = { orinJ6A_FIX_byteCode, 9, nullptr, 0 };
// orinJ6A_GRAV declared in orin.asm line 531
static unsigned char orinJ6A_GRAV_byteCode[] = {
	0xD9, 0x05, 0xE8, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BAE8] ; line 532
	0xD8, 0x05, 0xE8, 0xBA, 0x85, 0x00, // FADD DWORD PTR [0x85BAE8] ; line 533
};
static PatchByteCode orinJ6A_GRAV_patchByteCode = { orinJ6A_GRAV_byteCode, 12, nullptr, 0 };
// orinA214CHARGE_NORECOIL1 declared in orin.asm line 538
static unsigned char orinA214CHARGE_NORECOIL1_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 539
	0xD8, 0x25, 0xC0, 0xB9, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B9C0] ; line 540
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 541
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 542
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6EB61F ; line 543
};
static JumpTarget orinA214CHARGE_NORECOIL1_jumpData[] = {
	{ 0x01D, 0x6EB61F },
};
static PatchByteCode orinA214CHARGE_NORECOIL1_patchByteCode = { orinA214CHARGE_NORECOIL1_byteCode, 33, orinA214CHARGE_NORECOIL1_jumpData, 1 };
// orinA214CHARGE_NORECOIL2 declared in orin.asm line 545
static unsigned char orinA214CHARGE_NORECOIL2_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 546
	0xD8, 0x25, 0xC0, 0xB9, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B9C0] ; line 547
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 548
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 549
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6EB7CB ; line 550
};
static JumpTarget orinA214CHARGE_NORECOIL2_jumpData[] = {
	{ 0x01D, 0x6EB7CB },
};
static PatchByteCode orinA214CHARGE_NORECOIL2_patchByteCode = { orinA214CHARGE_NORECOIL2_byteCode, 33, orinA214CHARGE_NORECOIL2_jumpData, 1 };
// orinA214CHARGE_NORECOIL3 declared in orin.asm line 552
static unsigned char orinA214CHARGE_NORECOIL3_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 553
	0xD8, 0x25, 0xC0, 0xB9, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B9C0] ; line 554
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 555
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 556
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6EB9AB ; line 557
};
static JumpTarget orinA214CHARGE_NORECOIL3_jumpData[] = {
	{ 0x01D, 0x6EB9AB },
};
static PatchByteCode orinA214CHARGE_NORECOIL3_patchByteCode = { orinA214CHARGE_NORECOIL3_byteCode, 33, orinA214CHARGE_NORECOIL3_jumpData, 1 };
// orinA214CHARGE_NORECOIL4 declared in orin.asm line 559
static unsigned char orinA214CHARGE_NORECOIL4_byteCode[] = {
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 560
	0xD8, 0x25, 0xC0, 0xB9, 0x85, 0x00,                         // FSUB DWORD PTR [0x85B9C0] ; line 561
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 562
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 563
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6EBBE9 ; line 564
};
static JumpTarget orinA214CHARGE_NORECOIL4_jumpData[] = {
	{ 0x01D, 0x6EBBE9 },
};
static PatchByteCode orinA214CHARGE_NORECOIL4_patchByteCode = { orinA214CHARGE_NORECOIL4_byteCode, 33, orinA214CHARGE_NORECOIL4_jumpData, 1 };
// orinA214CHARGE_AIRLV declared in orin.asm line 571
static unsigned char orinA214CHARGE_AIRLV_byteCode[] = {
	0x80, 0xBE, 0xA9, 0x06, 0x00, 0x00, 0x06, // CMP BYTE PTR [ESI+0x6A9],0x06 ; line 572
};
static PatchByteCode orinA214CHARGE_AIRLV_patchByteCode = { orinA214CHARGE_AIRLV_byteCode, 7, nullptr, 0 };
// orinA214CHARGE_AIRLV2 declared in orin.asm line 574
static unsigned char orinA214CHARGE_AIRLV2_byteCode[] = {
	0x80, 0xBE, 0xA9, 0x06, 0x00, 0x00, 0x05, // CMP BYTE PTR [ESI+0x6A9],0x05 ; line 575
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6FCB23 ; line 576
};
static JumpTarget orinA214CHARGE_AIRLV2_jumpData[] = {
	{ 0x008, 0x6FCB23 },
};
static PatchByteCode orinA214CHARGE_AIRLV2_patchByteCode = { orinA214CHARGE_AIRLV2_byteCode, 12, orinA214CHARGE_AIRLV2_jumpData, 1 };
// orinA214CHARGE_SPDB declared in orin.asm line 582
static unsigned char orinA214CHARGE_SPDB_byteCode[] = {
	0x60,                               // PUSHAD ; line 583
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 584
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00, // FLD DWORD PTR [0x8589F0] ; line 585
	0x8A, 0x86, 0xA9, 0x06, 0x00, 0x00, // MOV AL,[ESI+0x6A9] ; line 586
	0x0F, 0xB6, 0xC0,                   // MOVZX EAX,AL ; line 587
	0x50,                               // PUSH EAX ; line 588
	0xDB, 0x04, 0x24,                   // FILD DWORD PTR [ESP] ; line 589
	0x58,                               // POP EAX ; line 590
	0xDE, 0xC9,                         // FMULP ST1 ; line 591
	0xDE, 0xC1,                         // FADDP ; line 592
	0x61,                               // POPAD ; line 593
};
static PatchByteCode orinA214CHARGE_SPDB_patchByteCode = { orinA214CHARGE_SPDB_byteCode, 32, nullptr, 0 };
// orinA214CHARGE_SPDC declared in orin.asm line 595
static unsigned char orinA214CHARGE_SPDC_byteCode[] = {
	0x60,                               // PUSHAD ; line 596
	0xD9, 0x05, 0x30, 0xB8, 0x85, 0x00, // FLD DWORD PTR [0x85B830] ; line 597
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00, // FLD DWORD PTR [0x8589F0] ; line 598
	0x8A, 0x86, 0xA9, 0x06, 0x00, 0x00, // MOV AL,[ESI+0x6A9] ; line 599
	0x0F, 0xB6, 0xC0,                   // MOVZX EAX,AL ; line 600
	0x50,                               // PUSH EAX ; line 601
	0xDB, 0x04, 0x24,                   // FILD DWORD PTR [ESP] ; line 602
	0x58,                               // POP EAX ; line 603
	0xDE, 0xC9,                         // FMULP ST1 ; line 604
	0xDE, 0xC1,                         // FADDP ; line 605
	0x61,                               // POPAD ; line 606
};
static PatchByteCode orinA214CHARGE_SPDC_patchByteCode = { orinA214CHARGE_SPDC_byteCode, 32, nullptr, 0 };
// orinA236_HITDELAY declared in orin.asm line 632
static unsigned char orinA236_HITDELAY_byteCode[] = {
	0x0F, 0xB7, 0x86, 0x6E, 0x03, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x36E] ; line 633
	0x66, 0x81, 0xF8, 0x01, 0x00,             // CMP AX,0x0001 ; line 634
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x702B97 ; line 635
};
static JumpTarget orinA236_HITDELAY_jumpData[] = {
	{ 0x00D, 0x702B97 },
};
static PatchByteCode orinA236_HITDELAY_patchByteCode = { orinA236_HITDELAY_byteCode, 17, orinA236_HITDELAY_jumpData, 1 };
// orinA236_HITCOUNT declared in orin.asm line 640
static unsigned char orinA236_HITCOUNT_byteCode[] = {
	0x60,                               // PUSHAD ; line 641
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00, // MOV EAX,[ESI+0x398] ; line 642
	0x8A, 0x80, 0xAC, 0x06, 0x00, 0x00, // MOV AL,[EAX+0x6AC] ; line 643
	0x66, 0x9C,                         // PUSHF ; line 644
	0x80, 0xF8, 0x02,                   // CMP AL,0x02 ; line 645
	0x0F, 0x8F, 0x07, 0x00, 0x00, 0x00, // JG SHORT LABEL1 ; line 646
	0xB4, 0x02,                         // MOV AH,0x02 ; line 647
	0xE9, 0x02, 0x00, 0x00, 0x00,       // JMP SHORT LABEL2 ; line 648
	0xB4, 0x03,                         // MOV AH,0x03 ; line 650
	0x66, 0x9D,                         // POPF ; line 652
	0x88, 0xA6, 0x94, 0x01, 0x00, 0x00, // MOV [ESI+0x194],AH ; line 653
	0x61,                               // POPAD ; line 654
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 655
	0xD9, 0x5C, 0x24, 0x04,             // FSTP DWORD PTR [ESP+0x04] ; line 656
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x713EE0 ; line 657
};
static JumpTarget orinA236_HITCOUNT_jumpData[] = {
	{ 0x035, 0x713EE0 },
};
static PatchByteCode orinA236_HITCOUNT_patchByteCode = { orinA236_HITCOUNT_byteCode, 57, orinA236_HITCOUNT_jumpData, 1 };
// orinA236_FLDZ1 declared in orin.asm line 661
static unsigned char orinA236_FLDZ1_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 662
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EAC77 ; line 663
};
static JumpTarget orinA236_FLDZ1_jumpData[] = {
	{ 0x003, 0x6EAC77 },
};
static PatchByteCode orinA236_FLDZ1_patchByteCode = { orinA236_FLDZ1_byteCode, 7, orinA236_FLDZ1_jumpData, 1 };
// orinA236_FLDZ2 declared in orin.asm line 665
static unsigned char orinA236_FLDZ2_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 666
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EACDE ; line 667
};
static JumpTarget orinA236_FLDZ2_jumpData[] = {
	{ 0x003, 0x6EACDE },
};
static PatchByteCode orinA236_FLDZ2_patchByteCode = { orinA236_FLDZ2_byteCode, 7, orinA236_FLDZ2_jumpData, 1 };
// orinA236_FLDZ3 declared in orin.asm line 669
static unsigned char orinA236_FLDZ3_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 670
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EAEB3 ; line 671
};
static JumpTarget orinA236_FLDZ3_jumpData[] = {
	{ 0x003, 0x6EAEB3 },
};
static PatchByteCode orinA236_FLDZ3_patchByteCode = { orinA236_FLDZ3_byteCode, 7, orinA236_FLDZ3_jumpData, 1 };
// orinA236_FLDZ4 declared in orin.asm line 673
static unsigned char orinA236_FLDZ4_byteCode[] = {
	0xD9, 0xEE,                   // FLDZ ; line 674
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6EAF1A ; line 675
};
static JumpTarget orinA236_FLDZ4_jumpData[] = {
	{ 0x003, 0x6EAF1A },
};
static PatchByteCode orinA236_FLDZ4_patchByteCode = { orinA236_FLDZ4_byteCode, 7, orinA236_FLDZ4_jumpData, 1 };
// orinA22BOMB_DIST declared in orin.asm line 695
static unsigned char orinA22BOMB_DIST_byteCode[] = {
	0xD8, 0x1D, 0xD8, 0xCA, 0x85, 0x00, // FCOMP DWORD PTR [0x85CAD8] ; line 696
};
static PatchByteCode orinA22BOMB_DIST_patchByteCode = { orinA22BOMB_DIST_byteCode, 6, nullptr, 0 };
// orinD22_HOLD declared in orin.asm line 701
static unsigned char orinD22_HOLD_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 702
};
static PatchByteCode orinD22_HOLD_patchByteCode = { orinD22_HOLD_byteCode, 10, nullptr, 0 };
// orinD22_CRASH declared in orin.asm line 706
static unsigned char orinD22_CRASH_byteCode[] = {
	0xDF, 0xE0,       // FNSTSW AX ; line 707
	0xF6, 0xC4, 0x02, // TEST AH,0x02 ; line 708
};
static PatchByteCode orinD22_CRASH_patchByteCode = { orinD22_CRASH_byteCode, 5, nullptr, 0 };
// orinD22_CRASH1 declared in orin.asm line 711
static unsigned char orinD22_CRASH1_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x705145 ; line 712
};
static JumpTarget orinD22_CRASH1_jumpData[] = {
	{ 0x002, 0x705145 },
};
static PatchByteCode orinD22_CRASH1_patchByteCode = { orinD22_CRASH1_byteCode, 6, orinD22_CRASH1_jumpData, 1 };
// orin3SCRING_DUR declared in orin.asm line 721
static unsigned char orin3SCRING_DUR_byteCode[] = {
	0xC7, 0x86, 0x90, 0x08, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x890],0x30 ; line 722
};
static PatchByteCode orin3SCRING_DUR_patchByteCode = { orin3SCRING_DUR_byteCode, 10, nullptr, 0 };
// orinA214DIVE_SKIP0 declared in orin.asm line 734
static unsigned char orinA214DIVE_SKIP0_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x7049F3 ; line 735
};
static JumpTarget orinA214DIVE_SKIP0_jumpData[] = {
	{ 0x001, 0x7049F3 },
};
static PatchByteCode orinA214DIVE_SKIP0_patchByteCode = { orinA214DIVE_SKIP0_byteCode, 5, orinA214DIVE_SKIP0_jumpData, 1 };
// orinA214DIVE_ONHIT declared in orin.asm line 739
static unsigned char orinA214DIVE_ONHIT_byteCode[] = {
	0x6A, 0x02,                         // PUSH 0x02 ; line 740
	0x6A, 0x02,                         // PUSH 0x02 ; line 741
	0x6A, 0x00,                         // PUSH 0x00 ; line 742
	0x89, 0xF1,                         // MOV ECX,ESI ; line 743
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x48CE90 ; line 744
	0x84, 0xC0,                         // TEST AL,AL ; line 745
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x6FE60E ; line 746
	0xD9, 0x86, 0x78, 0x03, 0x00, 0x00, // FLD DWORD PTR [ESI+0x378] ; line 747
};
static JumpTarget orinA214DIVE_ONHIT_jumpData[] = {
	{ 0x009, 0x48CE90 },
	{ 0x011, 0x6FE60E },
};
static PatchByteCode orinA214DIVE_ONHIT_patchByteCode = { orinA214DIVE_ONHIT_byteCode, 27, orinA214DIVE_ONHIT_jumpData, 2 };
// orinA214DIVE_HIT declared in orin.asm line 752
static unsigned char orinA214DIVE_HIT_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 753
	0x60,                                                       // PUSHAD ; line 754
	0xE9, 0x1D, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 755
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 767
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 768
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 769
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 770
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CE90 ; line 771
	0x84, 0xC0,                                                 // TEST AL,AL ; line 772
	0x61,                                                       // POPAD ; line 775
	0x66, 0x9D,                                                 // POPF ; line 776
	0x8A, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV AL,[ESI+0x113] ; line 777
	0xE9, 0x7A, 0x00, 0x00, 0x00,                               // JMP LABELZ ; line 778
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 781
	0x0F, 0x85, 0x65, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 782
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                         // MOV EBX,[ESI+0x398] ; line 783
	0x8B, 0x9B, 0x3C, 0x01, 0x00, 0x00,                         // MOV EBX,[EBX+0x13C] ; line 784
	0x81, 0xFB, 0x12, 0x02, 0x03, 0x00,                         // CMP EBX,0x00030212 ; line 785
	0x0F, 0x84, 0x35, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 786
	0x81, 0xFB, 0x13, 0x02, 0x03, 0x00,                         // CMP EBX,0x00030213 ; line 787
	0x0F, 0x84, 0x29, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 788
	0x81, 0xFB, 0x14, 0x02, 0x01, 0x00,                         // CMP EBX,0x00010214 ; line 789
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 790
	0x81, 0xFB, 0x15, 0x02, 0x01, 0x00,                         // CMP EBX,0x00010215 ; line 791
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 792
	0x81, 0xFB, 0x5B, 0x02, 0x03, 0x00,                         // CMP EBX,0x0003025B ; line 793
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 794
	0xE9, 0x86, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL5 ; line 795
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 797
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 798
	0xC6, 0x86, 0x8C, 0x01, 0x00, 0x00, 0x09,                   // MOV BYTE PTR [ESI+0x18C],0x09 ; line 799
	0xE9, 0x69, 0xFF, 0xFF, 0xFF,                               // JMP LABELS ; line 802
	0x90,                                                       // NOP ; line 805
};
static JumpTarget orinA214DIVE_HIT_jumpData[] = {
	{ 0x011, 0x48CE90 },
};
static PatchByteCode orinA214DIVE_HIT_patchByteCode = { orinA214DIVE_HIT_byteCode, 160, orinA214DIVE_HIT_jumpData, 1 };
// orinA214DIVE_FLD10 declared in orin.asm line 816
static unsigned char orinA214DIVE_FLD10_byteCode[] = {
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 817
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                         // FLD DWORD PTR [0x86782C] ; line 818
};
static PatchByteCode orinA214DIVE_FLD10_patchByteCode = { orinA214DIVE_FLD10_byteCode, 16, nullptr, 0 };
// orinA214DIVE_FLDN2 declared in orin.asm line 820
static unsigned char orinA214DIVE_FLDN2_byteCode[] = {
	0xD9, 0x05, 0x10, 0xAD, 0x85, 0x00, // FLD DWORD PTR [0x85AD10] ; line 821
};
static PatchByteCode orinA214DIVE_FLDN2_patchByteCode = { orinA214DIVE_FLDN2_byteCode, 6, nullptr, 0 };
// orinA214DIVE_IDK declared in orin.asm line 856
static unsigned char orinA214DIVE_IDK_byteCode[] = {
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 857
};
static PatchByteCode orinA214DIVE_IDK_patchByteCode = { orinA214DIVE_IDK_byteCode, 10, nullptr, 0 };
// orinA214DIVE_DISAPP declared in orin.asm line 869
static unsigned char orinA214DIVE_DISAPP_byteCode[] = {
	0x0F, 0x82, 0xDE, 0xAD, 0xBE, 0xEF, // JB 0x6FE60E ; line 870
	0x80, 0xE8, 0x01,                   // SUB AL,0x01 ; line 871
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x7043E9 ; line 872
};
static JumpTarget orinA214DIVE_DISAPP_jumpData[] = {
	{ 0x002, 0x6FE60E },
	{ 0x00A, 0x7043E9 },
};
static PatchByteCode orinA214DIVE_DISAPP_patchByteCode = { orinA214DIVE_DISAPP_byteCode, 14, orinA214DIVE_DISAPP_jumpData, 2 };
// orinA214DIVE_HURR0A declared in orin.asm line 881
static unsigned char orinA214DIVE_HURR0A_byteCode[] = {
	0x60,                                     // PUSHAD ; line 882
	0x0F, 0xB6, 0x96, 0xAD, 0x06, 0x00, 0x00, // MOVZX EDX,BYTE PTR [ESI+0x6AD] ; line 883
	0x81, 0xFA, 0x01, 0x00, 0x00, 0x00,       // CMP EDX,0x01 ; line 884
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 885
	0x81, 0xFA, 0x02, 0x00, 0x00, 0x00,       // CMP EDX,0x02 ; line 886
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,       // JE SHORT LABEL2 ; line 887
	0x81, 0xFA, 0x03, 0x00, 0x00, 0x00,       // CMP EDX,0x03 ; line 888
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,       // JE SHORT LABEL3 ; line 889
	0x80, 0xF8, 0x38,                         // CMP AL,0x38 ; line 890
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 891
	0x80, 0xF8, 0x2A,                         // CMP AL,0x2A ; line 893
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 894
	0x80, 0xF8, 0x32,                         // CMP AL,0x32 ; line 896
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 897
	0x80, 0xF8, 0x3E,                         // CMP AL,0x3E ; line 899
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 900
	0x80, 0xF8, 0x24,                         // CMP AL,0x24 ; line 901
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 902
	0x81, 0xE0, 0x0F, 0x00, 0x00, 0x80,       // AND EAX,0x8000000F ; line 904
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL5 ; line 905
	0x81, 0xE0, 0x00, 0x00, 0x00, 0x80,       // AND EAX,0x80000000 ; line 907
	0x61,                                     // POPAD ; line 909
	0x0F, 0x89, 0xDE, 0xAD, 0xBE, 0xEF,       // JNS 0x6EBE00 ; line 911
	0x48,                                     // DEC EAX ; line 912
	0x40,                                     // INC EAX ; line 913
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6EBE00 ; line 914
};
static JumpTarget orinA214DIVE_HURR0A_jumpData[] = {
	{ 0x06D, 0x6EBE00 },
	{ 0x074, 0x6EBE00 },
};
static PatchByteCode orinA214DIVE_HURR0A_patchByteCode = { orinA214DIVE_HURR0A_byteCode, 120, orinA214DIVE_HURR0A_jumpData, 2 };
// orinA214DIVE_HURR0B declared in orin.asm line 916
static unsigned char orinA214DIVE_HURR0B_byteCode[] = {
	0x60,                                     // PUSHAD ; line 917
	0x0F, 0xB6, 0x96, 0xAD, 0x06, 0x00, 0x00, // MOVZX EDX,BYTE PTR [ESI+0x6AD] ; line 918
	0x81, 0xFA, 0x01, 0x00, 0x00, 0x00,       // CMP EDX,0x01 ; line 919
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 920
	0x81, 0xFA, 0x02, 0x00, 0x00, 0x00,       // CMP EDX,0x02 ; line 921
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,       // JE SHORT LABEL2 ; line 922
	0x81, 0xFA, 0x03, 0x00, 0x00, 0x00,       // CMP EDX,0x03 ; line 923
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,       // JE SHORT LABEL3 ; line 924
	0x80, 0xF8, 0x38,                         // CMP AL,0x38 ; line 925
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 926
	0x80, 0xF8, 0x2A,                         // CMP AL,0x2A ; line 928
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 929
	0x80, 0xF8, 0x32,                         // CMP AL,0x32 ; line 931
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 932
	0x80, 0xF8, 0x3E,                         // CMP AL,0x3E ; line 934
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 935
	0x80, 0xF8, 0x24,                         // CMP AL,0x24 ; line 936
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 937
	0x81, 0xE0, 0x0F, 0x00, 0x00, 0x80,       // AND EAX,0x8000000F ; line 939
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL5 ; line 940
	0x81, 0xE0, 0x00, 0x00, 0x00, 0x80,       // AND EAX,0x80000000 ; line 942
	0x61,                                     // POPAD ; line 944
	0x0F, 0x89, 0xDE, 0xAD, 0xBE, 0xEF,       // JNS 0x6EC3DE ; line 946
	0x48,                                     // DEC EAX ; line 947
	0x40,                                     // INC EAX ; line 948
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6EC3DE ; line 949
};
static JumpTarget orinA214DIVE_HURR0B_jumpData[] = {
	{ 0x06D, 0x6EC3DE },
	{ 0x074, 0x6EC3DE },
};
static PatchByteCode orinA214DIVE_HURR0B_patchByteCode = { orinA214DIVE_HURR0B_byteCode, 120, orinA214DIVE_HURR0B_jumpData, 2 };
// orinA214DIVE_HURR1 declared in orin.asm line 952
static unsigned char orinA214DIVE_HURR1_byteCode[] = {
	0x60,                                     // PUSHAD ; line 953
	0x0F, 0xB6, 0x96, 0xAD, 0x06, 0x00, 0x00, // MOVZX EDX,BYTE PTR [ESI+0x6AD] ; line 954
	0x81, 0xFA, 0x01, 0x00, 0x00, 0x00,       // CMP EDX,0x01 ; line 955
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 956
	0x81, 0xFA, 0x02, 0x00, 0x00, 0x00,       // CMP EDX,0x02 ; line 957
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,       // JE SHORT LABEL2 ; line 958
	0x81, 0xFA, 0x03, 0x00, 0x00, 0x00,       // CMP EDX,0x03 ; line 959
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,       // JE SHORT LABEL3 ; line 960
	0x80, 0xF9, 0x3D,                         // CMP CL,0x3D ; line 961
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 962
	0x80, 0xF9, 0x2F,                         // CMP CL,0x2F ; line 964
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 965
	0x80, 0xF9, 0x37,                         // CMP CL,0x37 ; line 967
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 968
	0x80, 0xF9, 0x43,                         // CMP CL,0x43 ; line 970
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 971
	0x80, 0xF9, 0x29,                         // CMP CL,0x29 ; line 972
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 973
	0x81, 0xE1, 0x0F, 0x00, 0x00, 0x80,       // AND ECX,0x8000000F ; line 975
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL5 ; line 976
	0x81, 0xE1, 0x00, 0x00, 0x00, 0x80,       // AND ECX,0x80000000 ; line 978
	0x61,                                     // POPAD ; line 980
	0x0F, 0x89, 0xDE, 0xAD, 0xBE, 0xEF,       // JNS 0x6EC112 ; line 982
	0x49,                                     // DEC ECX ; line 983
	0x41,                                     // INC ECX ; line 984
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6EC112 ; line 985
};
static JumpTarget orinA214DIVE_HURR1_jumpData[] = {
	{ 0x06D, 0x6EC112 },
	{ 0x074, 0x6EC112 },
};
static PatchByteCode orinA214DIVE_HURR1_patchByteCode = { orinA214DIVE_HURR1_byteCode, 120, orinA214DIVE_HURR1_jumpData, 2 };
// orinA214DIVE_HURR3 declared in orin.asm line 1021
static unsigned char orinA214DIVE_HURR3_byteCode[] = {
	0x60,                                     // PUSHAD ; line 1022
	0x0F, 0xB6, 0x96, 0xAD, 0x06, 0x00, 0x00, // MOVZX EDX,BYTE PTR [ESI+0x6AD] ; line 1023
	0x81, 0xFA, 0x01, 0x00, 0x00, 0x00,       // CMP EDX,0x01 ; line 1024
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 1025
	0x81, 0xFA, 0x02, 0x00, 0x00, 0x00,       // CMP EDX,0x02 ; line 1026
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,       // JE SHORT LABEL2 ; line 1027
	0x81, 0xFA, 0x03, 0x00, 0x00, 0x00,       // CMP EDX,0x03 ; line 1028
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,       // JE SHORT LABEL3 ; line 1029
	0x80, 0xF9, 0x2A,                         // CMP CL,0x2A ; line 1030
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 1031
	0x80, 0xF9, 0x1C,                         // CMP CL,0x1C ; line 1033
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 1034
	0x80, 0xF9, 0x24,                         // CMP CL,0x24 ; line 1036
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 1037
	0x80, 0xF9, 0x30,                         // CMP CL,0x30 ; line 1039
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 1040
	0x80, 0xF9, 0x16,                         // CMP CL,0x16 ; line 1041
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,       // JE SHORT LABEL4 ; line 1042
	0x81, 0xE1, 0x0F, 0x00, 0x00, 0x80,       // AND ECX,0x8000000F ; line 1044
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL5 ; line 1045
	0x81, 0xE1, 0x00, 0x00, 0x00, 0x80,       // AND ECX,0x80000000 ; line 1047
	0x61,                                     // POPAD ; line 1049
	0x0F, 0x89, 0xDE, 0xAD, 0xBE, 0xEF,       // JNS 0x6EC712 ; line 1051
	0x49,                                     // DEC ECX ; line 1052
	0x41,                                     // INC ECX ; line 1053
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6EC712 ; line 1054
};
static JumpTarget orinA214DIVE_HURR3_jumpData[] = {
	{ 0x06D, 0x6EC712 },
	{ 0x074, 0x6EC712 },
};
static PatchByteCode orinA214DIVE_HURR3_patchByteCode = { orinA214DIVE_HURR3_byteCode, 120, orinA214DIVE_HURR3_jumpData, 2 };
// orinA214DIVE_HECK declared in orin.asm line 1057
static unsigned char orinA214DIVE_HECK_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 1058
	0x80, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02,                   // CMP BYTE PTR [ESI+0x13E],0x02 ; line 1059
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1060
	0x0F, 0x85, 0x10, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 1061
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x35 ; line 1063
	0x0F, 0x84, 0x0C, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1064
	0xBB, 0x02, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000002 ; line 1066
	0x21, 0xDB,                                                 // AND EBX,EBX ; line 1067
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL4 ; line 1068
	0x81, 0xE1, 0x00, 0x00, 0x00, 0x80,                         // AND ECX,0x80000000 ; line 1070
	0x61,                                                       // POPAD ; line 1072
};
static PatchByteCode orinA214DIVE_HECK_patchByteCode = { orinA214DIVE_HECK_byteCode, 55, nullptr, 0 };
// orin3SCBUFF_DELAY declared in orin.asm line 1082
static unsigned char orin3SCBUFF_DELAY_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 1083
};
static PatchByteCode orin3SCBUFF_DELAY_patchByteCode = { orin3SCBUFF_DELAY_byteCode, 10, nullptr, 0 };
// orin3SCDIVE_RISE declared in orin.asm line 1089
static unsigned char orin3SCDIVE_RISE_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 1090
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF4] ; line 1091
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6F082C ; line 1092
};
static JumpTarget orin3SCDIVE_RISE_jumpData[] = {
	{ 0x00D, 0x6F082C },
};
static PatchByteCode orin3SCDIVE_RISE_patchByteCode = { orin3SCDIVE_RISE_byteCode, 17, orin3SCDIVE_RISE_jumpData, 1 };
// orin3SCDIVE_Y declared in orin.asm line 1098
static unsigned char orin3SCDIVE_Y_byteCode[] = {
	0xD9, 0x05, 0xBC, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C5BC] ; line 1099
};
static PatchByteCode orin3SCDIVE_Y_patchByteCode = { orin3SCDIVE_Y_byteCode, 6, nullptr, 0 };
// orinWHAT_CHANGESEQ declared in orin.asm line 1107
static unsigned char orinWHAT_CHANGESEQ_byteCode[] = {
	0x68, 0x5C, 0x03, 0x00, 0x00, // PUSH 0x0000035C ; line 1108
};
static PatchByteCode orinWHAT_CHANGESEQ_patchByteCode = { orinWHAT_CHANGESEQ_byteCode, 5, nullptr, 0 };
// orinWHAT_HALTROT declared in orin.asm line 1116
static unsigned char orinWHAT_HALTROT_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1117
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 1118
	0xD9, 0x9E, 0x78, 0x03, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x378] ; line 1119
	0xD9, 0xE8,                         // FLD1 ; line 1120
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x70ED36 ; line 1121
};
static JumpTarget orinWHAT_HALTROT_jumpData[] = {
	{ 0x011, 0x70ED36 },
};
static PatchByteCode orinWHAT_HALTROT_patchByteCode = { orinWHAT_HALTROT_byteCode, 21, orinWHAT_HALTROT_jumpData, 1 };
// orinWHAT_1HIT1 declared in orin.asm line 1127
static unsigned char orinWHAT_1HIT1_byteCode[] = {
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x190] ; line 1128
	0x39, 0xE8,                         // CMP EAX,EBP ; line 1129
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6F05F0 ; line 1130
};
static JumpTarget orinWHAT_1HIT1_jumpData[] = {
	{ 0x009, 0x6F05F0 },
};
static PatchByteCode orinWHAT_1HIT1_patchByteCode = { orinWHAT_1HIT1_byteCode, 13, orinWHAT_1HIT1_jumpData, 1 };
// orinSPRINKLER_SPD declared in orin.asm line 1135
static unsigned char orinSPRINKLER_SPD_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 1136
	0xD9, 0xE8, // FLD1 ; line 1137
	0xDE, 0xC1, // FADDP ; line 1138
};
static PatchByteCode orinSPRINKLER_SPD_patchByteCode = { orinSPRINKLER_SPD_byteCode, 6, nullptr, 0 };
// orinF5A_RECEPTION declared in orin.asm line 1151
static unsigned char orinF5A_RECEPTION_byteCode[] = {
	0xD8, 0x1D, 0xD8, 0x7D, 0x85, 0x00, // FCOMP DWORD PTR [0x857DD8] ; line 1152
};
static PatchByteCode orinF5A_RECEPTION_patchByteCode = { orinF5A_RECEPTION_byteCode, 6, nullptr, 0 };
// orinRINGFIX declared in orin.asm line 1157
static unsigned char orinRINGFIX_byteCode[] = {
	0x68, 0x59, 0x03, 0x00, 0x00, // PUSH 0x00000359 ; line 1158
};
static PatchByteCode orinRINGFIX_patchByteCode = { orinRINGFIX_byteCode, 5, nullptr, 0 };


// orin.asm
std::array<PatchSkeleton, 127> patchList = {
	PatchSkeleton{ orinID_FIX_patchByteCode,               (void *)0x6E36A3,  8}, // Declared line 20. Patch on CMP WORD PTR [ESI+0x196], 0
	PatchSkeleton{ orin2SC_AIR_FIX_patchByteCode,          (void *)0x6EC0DF,  5}, // Declared line 26. Patch on MOV EAX, [EDX+0xC]
	PatchSkeleton{ orinD214_NEW_patchByteCode,             (void *)0x6EB194,  7}, // Declared line 179. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EB17B,  6}, // Declared line 180. Patch on JNE 0x006EB2CA
	PatchSkeleton{ orinD214_NEWBASE_patchByteCode,         (void *)0x6EAFFD,  9}, // Declared line 181. Patch on MOV ECX, ESI
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EB03A,  6}, // Declared line 182. Patch on FADD ST, QWORD PTR [0x857F78]
	PatchSkeleton{ orinD214_NEW1_patchByteCode,            (void *)0x702F3F,  8}, // Declared line 183. Patch on CMP WORD PTR [ECX+0x13E], 3
	PatchSkeleton{ orinD214_SPIRIT_patchByteCode,          (void *)0x6EB1A1,  6}, // Declared line 185. Patch on PUSH EDI
	PatchSkeleton{ orinD214_CJUMP_patchByteCode,           (void *)0x6EB337,  9}, // Declared line 186. Patch on MOV ECX, ESI
	PatchSkeleton{ orinD214_CNOEND_patchByteCode,          (void *)0x6EB04E,  6}, // Declared line 187. Patch on FCOMP ST, QWORD PTR [0x85C4D8]
	PatchSkeleton{ orinD214_CNOEND1_patchByteCode,         (void *)0x6EB00D,  7}, // Declared line 188. Patch on CMP DWORD PTR [ESI+0x760], 0
	PatchSkeleton{ orinD214_LV_patchByteCode,              (void *)0x702F6D,  6}, // Declared line 247. Patch on FLD ST, DWORD PTR [ECX+0xF4]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x70364C,  6}, // Declared line 248. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ orinD214_HP_patchByteCode,              (void *)0x7142A9,  9}, // Declared line 253. Patch on MOV WORD PTR [ESI+0x184], 0x320
	PatchSkeleton{ orinD214_HP_patchByteCode,              (void *)0x7142BB,  9}, // Declared line 254. Patch on MOV WORD PTR [ESI+0x184], 0x514
	PatchSkeleton{ orinD214_HP_patchByteCode,              (void *)0x7142CC,  9}, // Declared line 255. Patch on MOV WORD PTR [ESI+0x184], 0x5AA
	PatchSkeleton{ orinD214_HP_patchByteCode,              (void *)0x7142DE,  9}, // Declared line 256. Patch on MOV WORD PTR [ESI+0x184], 0x640
	PatchSkeleton{ orinD214_HP_patchByteCode,              (void *)0x7142F0,  9}, // Declared line 257. Patch on MOV WORD PTR [ESI+0x184], 0x708
	PatchSkeleton{ orinD214_ONHIT_patchByteCode,           (void *)0x702C8E,  7}, // Declared line 294. Patch on MOVSX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ orinJ2B_patchByteCode,                  (void *)0x6E8617,  9}, // Declared line 327. Patch on PUSH 0x1E
	PatchSkeleton{ orinJ2B_ANG0_patchByteCode,             (void *)0x6E7E48,  6}, // Declared line 328. Patch on FLD ST, DWORD PTR [0x85B9E8]
	PatchSkeleton{ orinJ2B_ANG1_patchByteCode,             (void *)0x6E7E57,  6}, // Declared line 329. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ orinFLOAT0_patchByteCode,               (void *)0x6E310A,  5}, // Declared line 349. Patch on TEST AH, 1
	PatchSkeleton{ orinFLOAT1_patchByteCode,               (void *)0x6E310F,  7}, // Declared line 355. Patch on CMP DWORD PTR [ECX+0x758], 0
	PatchSkeleton{ orinFLOAT2_patchByteCode,               (void *)0x6E3130,  5}, // Declared line 361. Patch on TEST AH, 0x41
	PatchSkeleton{ orin5C_SPD_patchByteCode,               (void *)0x6E895D,  6}, // Declared line 367. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ orin5C_ACC_patchByteCode,               (void *)0x6FF72B,  6}, // Declared line 372. Patch on FMUL ST, QWORD PTR [0x85BA30]
	PatchSkeleton{ orin5C_DUR_patchByteCode,               (void *)0x6FF971,  7}, // Declared line 377. Patch on CMP DWORD PTR [ESI+0x144], 0x2D
	PatchSkeleton{ orin5C_SPD_patchByteCode,               (void *)0x6E8FD5,  6}, // Declared line 379. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ orinD623B_DUR_patchByteCode,            (void *)0x707526,  7}, // Declared line 403. Patch on CMP DWORD PTR [ESI+0x144], 0xD
	PatchSkeleton{ orinD623C_DUR_patchByteCode,            (void *)0x707547,  7}, // Declared line 404. Patch on CMP DWORD PTR [ESI+0x144], 0x28
	PatchSkeleton{ orinD623_GRAZERES_patchByteCode,        (void *)0x7074C1,  9}, // Declared line 414. Patch on PUSH 0
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x7074DB,  6}, // Declared line 416. Patch on JNP 0x006FE60E
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x7074F6,  6}, // Declared line 417. Patch on JE 0x00703F74
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x70750D,  6}, // Declared line 418. Patch on JE 0x00703F74
	PatchSkeleton{ orinD623_RAD_patchByteCode,             (void *)0x707307,  6}, // Declared line 425. Patch on FMUL ST, QWORD PTR [0x85B818]
	PatchSkeleton{ orin6C_DUR_patchByteCode,               (void *)0x6FFE4E,  7}, // Declared line 431. Patch on CMP DWORD PTR [ESI+0x144], 0x28
	PatchSkeleton{ orin6C_DUR_patchByteCode,               (void *)0x700162,  7}, // Declared line 432. Patch on CMP DWORD PTR [ESI+0x144], 0x28
	PatchSkeleton{ orin2C_GRAV_OLD_patchByteCode,          (void *)0x700FBD,  6}, // Declared line 484. Patch on FSUB ST, QWORD PTR [0x858820]
	PatchSkeleton{ orin2C_HP_patchByteCode,                (void *)0x700E06,  9}, // Declared line 492. Patch on CMP WORD PTR [ESI+0x184], 0xFC18
	PatchSkeleton{ orinD236_SPD_patchByteCode,             (void *)0x6E9A55,  6}, // Declared line 500. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ orinD236_SPD1_patchByteCode,            (void *)0x6E9E4E,  6}, // Declared line 505. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ orinD236_MINCHARGE_patchByteCode,       (void *)0x6E9BE8,  5}, // Declared line 510. Patch on MOV EBX, 0xA
	PatchSkeleton{ orinD236_IDKFIX_patchByteCode,          (void *)0x6E9C91,  6}, // Declared line 519. Patch on CMP AL, 3
	PatchSkeleton{ orinD236_SPD_patchByteCode,             (void *)0x6EA2C9,  6}, // Declared line 521. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ orinD236_SPD1_patchByteCode,            (void *)0x6EA72C,  6}, // Declared line 523. Patch on FLD ST, DWORD PTR [0x85BC10]
	PatchSkeleton{ orinD236_MINCHARGE_patchByteCode,       (void *)0x6EA4AE,  5}, // Declared line 525. Patch on MOV EBX, 0xA
	PatchSkeleton{ orinJ6A_FIX_patchByteCode,              (void *)0x6E6C72,  8}, // Declared line 530. Patch on CMP WORD PTR [ESI+0x140], 0xD
	PatchSkeleton{ orinJ6A_GRAV_patchByteCode,             (void *)0x6E6C80,  6}, // Declared line 535. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ orinA214CHARGE_NORECOIL1_patchByteCode, (void *)0x6EB5ED,  7}, // Declared line 566. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214CHARGE_NORECOIL2_patchByteCode, (void *)0x6EB7BE,  7}, // Declared line 567. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214CHARGE_NORECOIL3_patchByteCode, (void *)0x6EB964,  7}, // Declared line 568. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214CHARGE_NORECOIL4_patchByteCode, (void *)0x6EBBDC,  7}, // Declared line 569. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214CHARGE_AIRLV_patchByteCode,     (void *)0x6FD2A9,  7}, // Declared line 578. Patch on CMP BYTE PTR [ESI+0x6A9], 2
	PatchSkeleton{ orinA214CHARGE_AIRLV_patchByteCode,     (void *)0x6FD1C9,  7}, // Declared line 579. Patch on CMP BYTE PTR [ESI+0x6A9], 2
	PatchSkeleton{ orinA214CHARGE_AIRLV2_patchByteCode,    (void *)0x6FCB1D,  6}, // Declared line 580. Patch on CMP [ESI+0x6A9], BL
	PatchSkeleton{ orinA214CHARGE_SPDB_patchByteCode,      (void *)0x6EB746,  6}, // Declared line 608. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ orinA214CHARGE_SPDC_patchByteCode,      (void *)0x6EB8D5,  6}, // Declared line 609. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ orinA214CHARGE_SPDB_patchByteCode,      (void *)0x6EBAE8,  6}, // Declared line 610. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ orinA214CHARGE_SPDC_patchByteCode,      (void *)0x6EBCE1,  6}, // Declared line 611. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ orinA236_HITDELAY_patchByteCode,        (void *)0x702B8C,  7}, // Declared line 637. Patch on MOVZX EAX, WORD PTR [ESI+0x36E]
	PatchSkeleton{ orinA236_HITCOUNT_patchByteCode,        (void *)0x713ECF,  6}, // Declared line 659. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ orinA236_FLDZ1_patchByteCode,           (void *)0x6EAC71,  6}, // Declared line 677. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ orinA236_FLDZ2_patchByteCode,           (void *)0x6EACD8,  6}, // Declared line 678. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ orinA236_FLDZ3_patchByteCode,           (void *)0x6EAEAD,  6}, // Declared line 679. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ orinA236_FLDZ4_patchByteCode,           (void *)0x6EAF14,  6}, // Declared line 680. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x702AD4,  6}, // Declared line 682. Patch on FADD ST, QWORD PTR [0x8579B0]
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x6EAC03,  6}, // Declared line 684. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x6EAE3F,  6}, // Declared line 685. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ orinA22BOMB_DIST_patchByteCode,         (void *)0x705CAF,  6}, // Declared line 698. Patch on FCOMP ST, QWORD PTR [0x85EB68]
	PatchSkeleton{ orinD22_HOLD_patchByteCode,             (void *)0x6ECADC,  7}, // Declared line 704. Patch on CMP DWORD PTR [ESI+0x144], 0x3C
	PatchSkeleton{ orinD22_CRASH_patchByteCode,            (void *)0x70500D,  5}, // Declared line 710. Patch on FNSTSW AX
	PatchSkeleton{ orinD22_CRASH1_patchByteCode,           (void *)0x705012,  6}, // Declared line 714. Patch on JE 0x00705145
	PatchSkeleton{ orin3SCRING_DUR_patchByteCode,          (void *)0x6F213E,  6}, // Declared line 724. Patch on MOV [ESI+0x890], EDI
	PatchSkeleton{ orin3SCRING_DUR_patchByteCode,          (void *)0x6F22AB,  6}, // Declared line 725. Patch on MOV [ESI+0x890], EDI
	PatchSkeleton{ orin3SCRING_DUR_patchByteCode,          (void *)0x6F205E,  6}, // Declared line 726. Patch on MOV [ESI+0x890], EDI
	PatchSkeleton{ orin3SCRING_DUR_patchByteCode,          (void *)0x6F21BF,  6}, // Declared line 727. Patch on MOV [ESI+0x890], EDI
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x70EC16,  6}, // Declared line 729. Patch on JE 0x0070ECEC
	PatchSkeleton{ orinA214DIVE_SKIP0_patchByteCode,       (void *)0x704445,  7}, // Declared line 737. Patch on MOVZX EAX, WORD PTR [EDI+0x13C]
	PatchSkeleton{ orinA214DIVE_ONHIT_patchByteCode,       (void *)0x70431F,  6}, // Declared line 749. Patch on FLD ST, DWORD PTR [ESI+0x378]
	PatchSkeleton{ orinA214DIVE_HIT_patchByteCode,         (void *)0x7043D9,  6}, // Declared line 808. Patch on MOV AL, [ESI+0x113]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EBF81,  6}, // Declared line 810. Patch on MOV [ESI+0x190], EDI
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EC293,  6}, // Declared line 811. Patch on MOV [ESI+0x190], EDI
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EB580,  5}, // Declared line 812. Patch on IN AL, DX
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EC8AD,  6}, // Declared line 813. Patch on MOV [ESI+0x190], EAX
	PatchSkeleton{ orinA214DIVE_FLD10_patchByteCode,       (void *)0x6EC078,  6}, // Declared line 823. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ orinA214DIVE_FLD10_patchByteCode,       (void *)0x6EC606,  6}, // Declared line 825. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ orinA214DIVE_FLDN2_patchByteCode,       (void *)0x6EC084,  6}, // Declared line 827. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ orinA214DIVE_FLDN2_patchByteCode,       (void *)0x6EC61C,  6}, // Declared line 829. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x6EC004,  6}, // Declared line 831. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x6EC316,  6}, // Declared line 833. Patch on FLD ST, DWORD PTR [0x85C110]
	PatchSkeleton{ orinA214DIVE_FLD10_patchByteCode,       (void *)0x6EC381,  6}, // Declared line 835. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ orinA214DIVE_FLDN2_patchByteCode,       (void *)0x6EC39A,  6}, // Declared line 836. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EBE56,  6}, // Declared line 845. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EC168,  6}, // Declared line 846. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EC434,  6}, // Declared line 847. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x6EC768,  6}, // Declared line 848. Patch on FADD ST, QWORD PTR [0x859910]
	PatchSkeleton{ orinA214DIVE_FLD10_patchByteCode,       (void *)0x6EBFF8,  6}, // Declared line 850. Patch on FLD ST, DWORD PTR [0x85C5B8]
	PatchSkeleton{ orinA214DIVE_FLD10_patchByteCode,       (void *)0x6EC30A,  6}, // Declared line 852. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ orinA214DIVE_IDK_patchByteCode,         (void *)0x6EC072,  6}, // Declared line 860. Patch on MOV [ESI+0x190], EDI
	PatchSkeleton{ orinA214DIVE_IDK_patchByteCode,         (void *)0x6EC60C, 10}, // Declared line 862. Patch on MOV DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214DIVE_IDK_patchByteCode,         (void *)0x6EC38E,  6}, // Declared line 864. Patch on MOV [ESI+0x190], EDI
	PatchSkeleton{ orinA214DIVE_IDK_patchByteCode,         (void *)0x6EC93F, 10}, // Declared line 866. Patch on MOV DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ orinA214DIVE_DISAPP_patchByteCode,      (void *)0x7043E1,  6}, // Declared line 874. Patch on JB 0x006FE60E
	PatchSkeleton{ orinFLD1_patchByteCode,                 (void *)0x70434F,  6}, // Declared line 876. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ orinA214DIVE_HURR0A_patchByteCode,      (void *)0x6EBDF4,  5}, // Declared line 1075. Patch on AND EAX, 0x80000001
	PatchSkeleton{ orinA214DIVE_HURR1_patchByteCode,       (void *)0x6EC105,  6}, // Declared line 1076. Patch on AND ECX, 0x80000001
	PatchSkeleton{ orinA214DIVE_HURR0B_patchByteCode,      (void *)0x6EC3D2,  5}, // Declared line 1077. Patch on AND EAX, 0x80000001
	PatchSkeleton{ orinA214DIVE_HURR3_patchByteCode,       (void *)0x6EC705,  6}, // Declared line 1078. Patch on AND ECX, 0x80000001
	PatchSkeleton{ orin3SCBUFF_DELAY_patchByteCode,        (void *)0x70EEC0,  7}, // Declared line 1085. Patch on CMP DWORD PTR [ESI+0x144], 0x3C
	PatchSkeleton{ orin3SCDIVE_RISE_patchByteCode,         (void *)0x6F0824,  8}, // Declared line 1094. Patch on FLDZ
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x6F0923,  6}, // Declared line 1096. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ orin3SCDIVE_Y_patchByteCode,            (void *)0x6F0940,  6}, // Declared line 1101. Patch on FLD ST, DWORD PTR [0x85B9E4]
	PatchSkeleton{ orinA214DIVE_HECK_patchByteCode,        (void *)0x6F0400,  6}, // Declared line 1105. Patch on AND ECX, 0x80000001
	PatchSkeleton{ orinWHAT_CHANGESEQ_patchByteCode,       (void *)0x6F04C5,  5}, // Declared line 1110. Patch on PUSH 0x331
	PatchSkeleton{ orinNOP_patchByteCode,                  (void *)0x70ED79,  6}, // Declared line 1114. Patch on FADD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ orinWHAT_HALTROT_patchByteCode,         (void *)0x70ED24,  6}, // Declared line 1123. Patch on FMUL ST, QWORD PTR [0x8581D8]
	PatchSkeleton{ orinFMULZERO_patchByteCode,             (void *)0x7059E5,  6}, // Declared line 1125. Patch on FMUL ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ orinWHAT_1HIT1_patchByteCode,           (void *)0x6F0592,  6}, // Declared line 1132. Patch on MOV EAX, [ESI+0x190]
	PatchSkeleton{ orinSPRINKLER_SPD_patchByteCode,        (void *)0x6EFF58,  6}, // Declared line 1140. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ orinSPRINKLER_SPD_patchByteCode,        (void *)0x6F01E6,  6}, // Declared line 1141. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ orinFLDZ_patchByteCode,                 (void *)0x70BDAE,  6}, // Declared line 1145. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ orinF5A_RECEPTION_patchByteCode,        (void *)0x6FD74B,  6}, // Declared line 1154. Patch on FCOMP ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ orinRINGFIX_patchByteCode,              (void *)0x70F037,  5}, // Declared line 1160. Patch on PUSH 0x35C
	PatchSkeleton{ orinRINGFIX_patchByteCode,              (void *)0x70EBE8,  5}, // Declared line 1161. Patch on PUSH 0x35C
	PatchSkeleton{ orinRINGFIX_patchByteCode,              (void *)0x70F138,  5}, // Declared line 1162. Patch on PUSH 0x35C
	PatchSkeleton{ orinRINGFIX_patchByteCode,              (void *)0x70EEB9,  5}, // Declared line 1163. Patch on PUSH 0x35C
};


static std::array<unsigned, 80> skeletonsForUpdate = {
	   0,   1,   2,   3,   4,   5,   7,   8,   9,  10,  19,  20,  21,  22,  23,  24,
	  25,  28,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  56,
	  57,  58,  59,  62,  63,  64,  65,  67,  68,  70,  73,  74,  75,  76,  81,  82,
	  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
	  99, 100, 101, 102, 105, 106, 107, 108, 110, 111, 112, 113, 114, 118, 119, 120,
};
static std::vector<unsigned> skeletonsForInitializeAction;
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 4> skeletonsForHandleInputs = { 53, 54, 55, 122 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 37> skeletonsForObjectUpdate = {
	   6,  11,  12,  18,  26,  27,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
	  39,  60,  66,  69,  71,  72,  77,  78,  79,  80, 103, 104, 109, 115, 116, 117,
	 121, 123, 124, 125, 126,
};
static std::array<unsigned, 6> skeletonsForObjectInitializeAction = { 13, 14, 15, 16, 17, 61 };



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
	printf("Internal memory used %zu/5532\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 5532);
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
