#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<16696> applyMemory;
StackedMemory<5729> internalMemory;
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


// kaguyaFLDZ declared in kaguya.asm line 1
static unsigned char kaguyaFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode kaguyaFLDZ_patchByteCode = { kaguyaFLDZ_byteCode, 2, nullptr, 0 };
// kaguyaNOP declared in kaguya.asm line 4
static unsigned char kaguyaNOP_byteCode[] = {
	0x90, // NOP ; line 5
};
static PatchByteCode kaguyaNOP_patchByteCode = { kaguyaNOP_byteCode, 1, nullptr, 0 };
// kaguyaID_FIX declared in kaguya.asm line 9
static unsigned char kaguyaID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x0000001B ; line 10
	0x66, 0x39, 0xBE, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],DI ; line 12
};
static PatchByteCode kaguyaID_FIX_patchByteCode = { kaguyaID_FIX_byteCode, 17, nullptr, 0 };
// kaguyaA623TIME_LV declared in kaguya.asm line 19
static unsigned char kaguyaA623TIME_LV_byteCode[] = {
	0x80, 0xB8, 0xAB, 0x06, 0x00, 0x00, 0x07, // CMP BYTE PTR [EAX+0x6AB],0x07 ; line 20
};
static PatchByteCode kaguyaA623TIME_LV_patchByteCode = { kaguyaA623TIME_LV_byteCode, 7, nullptr, 0 };
// kaguyaD22B_DIST declared in kaguya.asm line 26
static unsigned char kaguyaD22B_DIST_byteCode[] = {
	0xDD, 0xD8,                               // FSTP ST0 ; line 27
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x104],0x00 ; line 28
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,       // JGE SHORT LABEL1 ; line 29
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 30
	0xD8, 0x25, 0x70, 0x7F, 0x85, 0x00,       // FSUB DWORD PTR [0x857F70] ; line 31
	0xE9, 0x0C, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 32
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 34
	0xD8, 0x05, 0x70, 0x7F, 0x85, 0x00,       // FADD DWORD PTR [0x857F70] ; line 35
	0xD9, 0x5C, 0x24, 0x18,                   // FSTP DWORD PTR [ESP+0x18] ; line 37
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x54B19C ; line 38
};
static JumpTarget kaguyaD22B_DIST_jumpData[] = {
	{ 0x031, 0x54B19C },
};
static PatchByteCode kaguyaD22B_DIST_patchByteCode = { kaguyaD22B_DIST_byteCode, 53, kaguyaD22B_DIST_jumpData, 1 };
// kaguyaD22C_DIST declared in kaguya.asm line 41
static unsigned char kaguyaD22C_DIST_byteCode[] = {
	0xDD, 0xD8,                               // FSTP ST0 ; line 42
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x104],0x00 ; line 43
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,       // JGE SHORT LABEL1 ; line 44
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 45
	0xD8, 0x25, 0x78, 0x89, 0x85, 0x00,       // FSUB DWORD PTR [0x858978] ; line 46
	0xE9, 0x0C, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 47
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 49
	0xD8, 0x05, 0x78, 0x89, 0x85, 0x00,       // FADD DWORD PTR [0x858978] ; line 50
	0xD9, 0x5C, 0x24, 0x18,                   // FSTP DWORD PTR [ESP+0x18] ; line 52
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x54B30A ; line 53
};
static JumpTarget kaguyaD22C_DIST_jumpData[] = {
	{ 0x031, 0x54B30A },
};
static PatchByteCode kaguyaD22C_DIST_patchByteCode = { kaguyaD22C_DIST_byteCode, 53, kaguyaD22C_DIST_jumpData, 1 };
// kaguyaELIXIR_GRAVITY declared in kaguya.asm line 62
static unsigned char kaguyaELIXIR_GRAVITY_byteCode[] = {
	0xD9, 0x05, 0x78, 0xC0, 0x85, 0x00, // FLD DWORD PTR [0x85C078] ; line 63
};
static PatchByteCode kaguyaELIXIR_GRAVITY_patchByteCode = { kaguyaELIXIR_GRAVITY_byteCode, 6, nullptr, 0 };
// kaguyaELIXIR_Y declared in kaguya.asm line 65
static unsigned char kaguyaELIXIR_Y_byteCode[] = {
	0xD9, 0x05, 0x30, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B930] ; line 66
};
static PatchByteCode kaguyaELIXIR_Y_patchByteCode = { kaguyaELIXIR_Y_byteCode, 6, nullptr, 0 };
// kaguya2C_ONHIT declared in kaguya.asm line 74
static unsigned char kaguya2C_ONHIT_byteCode[] = {
	0x60,                               // PUSHAD ; line 75
	0x6A, 0x05,                         // PUSH 0x05 ; line 76
	0x6A, 0x03,                         // PUSH 0x03 ; line 77
	0x6A, 0x00,                         // PUSH 0x00 ; line 78
	0x89, 0xF1,                         // MOV ECX,ESI ; line 79
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,       // CALL 0x48CE90 ; line 80
	0x84, 0xC0,                         // TEST AL,AL ; line 81
	0x61,                               // POPAD ; line 82
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x5595F2 ; line 83
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x144] ; line 85
};
static JumpTarget kaguya2C_ONHIT_jumpData[] = {
	{ 0x00A, 0x48CE90 },
	{ 0x013, 0x5595F2 },
};
static PatchByteCode kaguya2C_ONHIT_patchByteCode = { kaguya2C_ONHIT_byteCode, 29, kaguya2C_ONHIT_jumpData, 2 };
// kaguyaD236_ORBFREQ declared in kaguya.asm line 89
static unsigned char kaguyaD236_ORBFREQ_byteCode[] = {
	0xB9, 0x03, 0x00, 0x00, 0x00, // MOV ECX,0x00000003 ; line 90
};
static PatchByteCode kaguyaD236_ORBFREQ_patchByteCode = { kaguyaD236_ORBFREQ_byteCode, 5, nullptr, 0 };
// kaguyaD236_ORBSTARTUP declared in kaguya.asm line 94
static unsigned char kaguyaD236_ORBSTARTUP_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x55A0EC ; line 95
	0x66, 0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x09, 0x00, // CMP WORD PTR [ESI+0x144],0x09 ; line 96
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x55A0EC ; line 97
};
static JumpTarget kaguyaD236_ORBSTARTUP_jumpData[] = {
	{ 0x002, 0x55A0EC },
	{ 0x011, 0x55A0EC },
};
static PatchByteCode kaguyaD236_ORBSTARTUP_patchByteCode = { kaguyaD236_ORBSTARTUP_byteCode, 21, kaguyaD236_ORBSTARTUP_jumpData, 2 };
// kaguyaD236_ONHIT declared in kaguya.asm line 101
static unsigned char kaguyaD236_ONHIT_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x13E],0x04 ; line 102
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                   // JNE SHORT LABELX ; line 103
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x09, 0x00, // CMP WORD PTR [ESI+0x140],0x09 ; line 104
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                   // JNE SHORT LABELX ; line 105
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x55A2E1 ; line 106
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x13E],0x05 ; line 109
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,                   // JNE SHORT LABELY ; line 110
	0x60,                                                 // PUSHAD ; line 111
	0x6A, 0x03,                                           // PUSH 0x03 ; line 112
	0x6A, 0x00,                                           // PUSH 0x00 ; line 113
	0x6A, 0x00,                                           // PUSH 0x00 ; line 114
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 115
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x48CE90 ; line 116
	0x84, 0xC0,                                           // TEST AL,AL ; line 117
	0x61,                                                 // POPAD ; line 118
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x55A2E1 ; line 119
	0x60,                                                 // PUSHAD ; line 122
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x398] ; line 123
	0x66, 0x8B, 0x80, 0x3C, 0x01, 0x00, 0x00,             // MOV AX, WORD PTR [EAX+0x13C] ; line 124
	0x66, 0x81, 0xF8, 0x32, 0x00,                         // CMP AX,0x32 ; line 125
	0x0F, 0x8C, 0x12, 0x00, 0x00, 0x00,                   // JL SHORT LABEL1 ; line 126
	0x66, 0x81, 0xF8, 0x96, 0x00,                         // CMP AX,0x96 ; line 127
	0x0F, 0x8D, 0x07, 0x00, 0x00, 0x00,                   // JGE SHORT LABEL1 ; line 128
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x194],0x00 ; line 129
	0x61,                                                 // POPAD ; line 131
	0xBD, 0x05, 0x00, 0x00, 0x00,                         // MOV EBP,0x05 ; line 132
	0x39, 0xAE, 0x90, 0x01, 0x00, 0x00,                   // CMP DWORD PTR [ESI+0x190],EBP ; line 133
};
static JumpTarget kaguyaD236_ONHIT_jumpData[] = {
	{ 0x01F, 0x55A2E1 },
	{ 0x03C, 0x48CE90 },
	{ 0x045, 0x55A2E1 },
};
static PatchByteCode kaguyaD236_ONHIT_patchByteCode = { kaguyaD236_ONHIT_byteCode, 128, kaguyaD236_ONHIT_jumpData, 3 };
// kaguya2C_NEW declared in kaguya.asm line 137
static unsigned char kaguya2C_NEW_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 138
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1F ; line 139
	0x0F, 0x8C, 0x07, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 140
	0xD9, 0xEE,                                                 // FLDZ ; line 141
	0xE9, 0x0C, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 142
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 144
	0xDC, 0x0D, 0x10, 0xBA, 0x85, 0x00,                         // FMUL QWORD PTR [0x85BA10] ; line 145
	0x66, 0x9D,                                                 // POPF ; line 147
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 148
	0xD9, 0xE8,                                                 // FLD1 ; line 149
};
static PatchByteCode kaguya2C_NEW_patchByteCode = { kaguya2C_NEW_byteCode, 47, nullptr, 0 };
// kaguya5AA_1HIT declared in kaguya.asm line 153
static unsigned char kaguya5AA_1HIT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 154
};
static PatchByteCode kaguya5AA_1HIT_patchByteCode = { kaguya5AA_1HIT_byteCode, 7, nullptr, 0 };
// kaguya5AA_PARTICLE declared in kaguya.asm line 160
static unsigned char kaguya5AA_PARTICLE_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x5608A5 ; line 161
};
static JumpTarget kaguya5AA_PARTICLE_jumpData[] = {
	{ 0x001, 0x5608A5 },
};
static PatchByteCode kaguya5AA_PARTICLE_patchByteCode = { kaguya5AA_PARTICLE_byteCode, 5, kaguya5AA_PARTICLE_jumpData, 1 };
// kaguyaD214_ALWAYSHOME1 declared in kaguya.asm line 169
static unsigned char kaguyaD214_ALWAYSHOME1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x55E8BA ; line 170
};
static JumpTarget kaguyaD214_ALWAYSHOME1_jumpData[] = {
	{ 0x001, 0x55E8BA },
};
static PatchByteCode kaguyaD214_ALWAYSHOME1_patchByteCode = { kaguyaD214_ALWAYSHOME1_byteCode, 5, kaguyaD214_ALWAYSHOME1_jumpData, 1 };
// kaguyaD214_ALWAYSHOME2 declared in kaguya.asm line 172
static unsigned char kaguyaD214_ALWAYSHOME2_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 173
	0x66, 0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x78, 0x00,       // CMP WORD PTR [ESI+0x144],0x78 ; line 174
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 175
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 176
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 177
	0xE9, 0x13, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 178
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 180
	0xD9, 0x05, 0xE0, 0xE2, 0x85, 0x00,                         // FLD DWORD PTR [0x85E2E0] ; line 181
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 182
	0x66, 0x9D,                                                 // POPF ; line 184
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x01 ; line 186
	0x0F, 0x85, 0x57, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 187
	0x66, 0x9C,                                                 // PUSHF ; line 188
	0x60,                                                       // PUSHAD ; line 189
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 190
	0x8B, 0x80, 0x70, 0x01, 0x00, 0x00,                         // MOV EAX,[EAX+0x170] ; line 191
	0x66, 0x8B, 0x80, 0x3C, 0x01, 0x00, 0x00,                   // MOV AX,[EAX+0x13C] ; line 192
	0x66, 0x81, 0xF8, 0x61, 0x00,                               // CMP AX,0x61 ; line 193
	0x0F, 0x8C, 0x10, 0x00, 0x00, 0x00,                         // JL SHORT LABEL4 ; line 194
	0x66, 0x81, 0xF8, 0x64, 0x00,                               // CMP AX,0x64 ; line 195
	0x0F, 0x8F, 0x05, 0x00, 0x00, 0x00,                         // JG SHORT LABEL4 ; line 196
	0xE9, 0x23, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 197
	0x66, 0x81, 0xF8, 0xC5, 0x00,                               // CMP AX,0xC5 ; line 199
	0x0F, 0x8C, 0x10, 0x00, 0x00, 0x00,                         // JL SHORT LABEL6 ; line 200
	0x66, 0x81, 0xF8, 0xC7, 0x00,                               // CMP AX,0xC7 ; line 201
	0x0F, 0x8F, 0x05, 0x00, 0x00, 0x00,                         // JG SHORT LABEL6 ; line 202
	0xE9, 0x08, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 203
	0x61,                                                       // POPAD ; line 205
	0x66, 0x9D,                                                 // POPF ; line 206
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x55EA0B ; line 207
	0x61,                                                       // POPAD ; line 209
	0x66, 0x9D,                                                 // POPF ; line 210
	0x66, 0x39, 0xAE, 0x40, 0x01, 0x00, 0x00,                   // CMP WORD PTR [ESI+0x140],BP ; line 212
};
static JumpTarget kaguyaD214_ALWAYSHOME2_jumpData[] = {
	{ 0x09A, 0x55EA0B },
};
static PatchByteCode kaguyaD214_ALWAYSHOME2_patchByteCode = { kaguyaD214_ALWAYSHOME2_byteCode, 168, kaguyaD214_ALWAYSHOME2_jumpData, 1 };
// kaguyaD214_SPDX declared in kaguya.asm line 227
static unsigned char kaguyaD214_SPDX_byteCode[] = {
	0xD8, 0x8E, 0xF4, 0x00, 0x00, 0x00, // FMUL DWORD PTR [ESI+0xF4] ; line 228
	0xD8, 0x0D, 0xA8, 0x77, 0x86, 0x00, // FMUL DWORD PTR [0x8677A8] ; line 229
};
static PatchByteCode kaguyaD214_SPDX_patchByteCode = { kaguyaD214_SPDX_byteCode, 12, nullptr, 0 };
// kaguyaD214_SPDY declared in kaguya.asm line 231
static unsigned char kaguyaD214_SPDY_byteCode[] = {
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF8] ; line 232
	0xD8, 0x0D, 0xA8, 0x77, 0x86, 0x00, // FMUL DWORD PTR [0x8677A8] ; line 233
};
static PatchByteCode kaguyaD214_SPDY_patchByteCode = { kaguyaD214_SPDY_byteCode, 12, nullptr, 0 };
// kaguyaD214_SPDIDK declared in kaguya.asm line 235
static unsigned char kaguyaD214_SPDIDK_byteCode[] = {
	0xD9, 0x05, 0xB8, 0x96, 0x85, 0x00, // FLD DWORD PTR [0x8596B8] ; line 236
};
static PatchByteCode kaguyaD214_SPDIDK_patchByteCode = { kaguyaD214_SPDIDK_byteCode, 6, nullptr, 0 };
// kaguyaD214_FAN0 declared in kaguya.asm line 242
static unsigned char kaguyaD214_FAN0_byteCode[] = {
	0xD8, 0x0D, 0x94, 0xAE, 0x85, 0x00, // FMUL DWORD PTR [0x85AE94] ; line 243
};
static PatchByteCode kaguyaD214_FAN0_patchByteCode = { kaguyaD214_FAN0_byteCode, 6, nullptr, 0 };
// kaguyaD214_FAN1 declared in kaguya.asm line 247
static unsigned char kaguyaD214_FAN1_byteCode[] = {
	0xD8, 0x0D, 0xE0, 0xB9, 0x85, 0x00, // FMUL DWORD PTR [0x85B9E0] ; line 248
};
static PatchByteCode kaguyaD214_FAN1_patchByteCode = { kaguyaD214_FAN1_byteCode, 6, nullptr, 0 };
// kaguya4B_DENSHIT2 declared in kaguya.asm line 262
static unsigned char kaguya4B_DENSHIT2_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 263
};
static PatchByteCode kaguya4B_DENSHIT2_patchByteCode = { kaguya4B_DENSHIT2_byteCode, 7, nullptr, 0 };
// kaguya4B_SPD declared in kaguya.asm line 268
static unsigned char kaguya4B_SPD_byteCode[] = {
	0xD9, 0x05, 0xA4, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BCA4] ; line 269
};
static PatchByteCode kaguya4B_SPD_patchByteCode = { kaguya4B_SPD_byteCode, 6, nullptr, 0 };
// kaguya4B_DUR declared in kaguya.asm line 273
static unsigned char kaguya4B_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0xB4, 0x01, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1B4 ; line 274
};
static PatchByteCode kaguya4B_DUR_patchByteCode = { kaguya4B_DUR_byteCode, 10, nullptr, 0 };
// kaguya4B_QUANTITY declared in kaguya.asm line 278
static unsigned char kaguya4B_QUANTITY_byteCode[] = {
	0xD8, 0x05, 0xA8, 0x77, 0x86, 0x00, // FADD DWORD PTR [0x8677A8] ; line 279
};
static PatchByteCode kaguya4B_QUANTITY_patchByteCode = { kaguya4B_QUANTITY_byteCode, 6, nullptr, 0 };
// kaguya5B_NEW declared in kaguya.asm line 286
static unsigned char kaguya5B_NEW_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 287
	0x60,                                                       // PUSHAD ; line 288
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 289
	0x0F, 0x85, 0xD1, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 290
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 291
	0x0F, 0x85, 0xC2, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 292
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 293
	0x0F, 0x85, 0xB3, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 294
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 295
	0x66, 0x8B, 0x80, 0xD2, 0x07, 0x00, 0x00,                   // MOV AX,[EAX+0x7D2] ; line 296
	0x66, 0x81, 0xF8, 0x01, 0x00,                               // CMP AX,0x01 ; line 297
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 298
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0xFF, 0x33, 0x33, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFF3333FF ; line 299
	0xE9, 0x8C, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 300
	0x66, 0x81, 0xF8, 0x02, 0x00,                               // CMP AX,0x02 ; line 302
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 303
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x33, 0xFF, 0x33, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFF33FF33 ; line 304
	0xE9, 0x72, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 305
	0x66, 0x81, 0xF8, 0x03, 0x00,                               // CMP AX,0x03 ; line 307
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 308
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x33, 0x33, 0xFF, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFFFF3333 ; line 309
	0xE9, 0x58, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 310
	0x66, 0x81, 0xF8, 0x04, 0x00,                               // CMP AX,0x04 ; line 312
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL5 ; line 313
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x33, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFF33FFFF ; line 314
	0xE9, 0x3E, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 315
	0x66, 0x81, 0xF8, 0x05, 0x00,                               // CMP AX,0x05 ; line 317
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL6 ; line 318
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0xFF, 0x33, 0xFF, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFFFF33FF ; line 319
	0xE9, 0x24, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 320
	0x66, 0x81, 0xF8, 0x06, 0x00,                               // CMP AX,0x06 ; line 322
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL7 ; line 323
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x33, 0xFF, 0xFF, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFFFFFF33 ; line 324
	0xE9, 0x0A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL1 ; line 325
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // MOV DWORD PTR [ESI+0x110],0xFFFFFFFF ; line 327
	0x61,                                                       // POPAD ; line 329
	0x66, 0x9D,                                                 // POPF ; line 330
	0xD9, 0x05, 0xB8, 0xBA, 0x85, 0x00,                         // FLD DWORD PTR [0x85BAB8] ; line 331
};
static PatchByteCode kaguya5B_NEW_patchByteCode = { kaguya5B_NEW_byteCode, 237, nullptr, 0 };
// kaguya5B_NEWA declared in kaguya.asm line 335
static unsigned char kaguya5B_NEWA_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x7D2],0x05 ; line 336
};
static PatchByteCode kaguya5B_NEWA_patchByteCode = { kaguya5B_NEWA_byteCode, 9, nullptr, 0 };
// kaguya5B_NEWACHARGE declared in kaguya.asm line 338
static unsigned char kaguya5B_NEWACHARGE_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x7D2],0x07 ; line 339
};
static PatchByteCode kaguya5B_NEWACHARGE_patchByteCode = { kaguya5B_NEWACHARGE_byteCode, 9, nullptr, 0 };
// kaguya5B_FIREFREQ declared in kaguya.asm line 341
static unsigned char kaguya5B_FIREFREQ_byteCode[] = {
	0xB9, 0x02, 0x00, 0x00, 0x00, // MOV ECX,0x00000002 ; line 342
};
static PatchByteCode kaguya5B_FIREFREQ_patchByteCode = { kaguya5B_FIREFREQ_byteCode, 5, nullptr, 0 };
// kaguya5C_METER declared in kaguya.asm line 367
static unsigned char kaguya5C_METER_byteCode[] = {
	0x6A, 0x10,                                                 // PUSH 0x10 ; line 368
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 369
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 370
};
static PatchByteCode kaguya5C_METER_patchByteCode = { kaguya5C_METER_byteCode, 14, nullptr, 0 };
// kaguyaJ5C_METER declared in kaguya.asm line 373
static unsigned char kaguyaJ5C_METER_byteCode[] = {
	0x6A, 0x10,                   // PUSH 0x10 ; line 374
	0x89, 0xF1,                   // MOV ECX,ESI ; line 375
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 376
};
static JumpTarget kaguyaJ5C_METER_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode kaguyaJ5C_METER_patchByteCode = { kaguyaJ5C_METER_byteCode, 9, kaguyaJ5C_METER_jumpData, 1 };
// kaguya5C_NEW declared in kaguya.asm line 380
static unsigned char kaguya5C_NEW_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x08, 0x00, // CMP WORD PTR [ESI+0x140],0x08 ; line 381
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x5483D9 ; line 382
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x140],0x07 ; line 383
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x5483D9 ; line 384
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x05 ; line 385
};
static JumpTarget kaguya5C_NEW_jumpData[] = {
	{ 0x00B, 0x5483D9 },
	{ 0x01A, 0x5483D9 },
};
static PatchByteCode kaguya5C_NEW_patchByteCode = { kaguya5C_NEW_byteCode, 39, kaguya5C_NEW_jumpData, 2 };
// kaguya5C_NEWJ declared in kaguya.asm line 387
static unsigned char kaguya5C_NEWJ_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x08, 0x00, // CMP WORD PTR [ESI+0x140],0x08 ; line 388
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 389
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x140],0x07 ; line 390
	0x0F, 0x84, 0x0E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 391
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x05 ; line 392
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x548AFC ; line 393
	0x89, 0x9E, 0x90, 0x01, 0x00, 0x00,                   // MOV [ESI+0x190],EBX ; line 395
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x548B16 ; line 396
};
static JumpTarget kaguya5C_NEWJ_jumpData[] = {
	{ 0x028, 0x548AFC },
	{ 0x033, 0x548B16 },
};
static PatchByteCode kaguya5C_NEWJ_patchByteCode = { kaguya5C_NEWJ_byteCode, 55, kaguya5C_NEWJ_jumpData, 2 };
// kaguya5C_NEW1 declared in kaguya.asm line 401
static unsigned char kaguya5C_NEW1_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 402
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x05 ; line 403
	0x0F, 0x85, 0x27, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 404
	0x66, 0x9D,                                           // POPF ; line 405
	0x6A, 0x30,                                           // PUSH 0x30 ; line 406
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 407
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 408
	0xD9, 0xEE,                                           // FLDZ ; line 409
	0xD9, 0x9C, 0x24, 0x04, 0x02, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x204] ; line 410
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 411
	0xD9, 0xEE,                                           // FLDZ ; line 412
	0xD9, 0x9C, 0x24, 0xFC, 0x01, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x1FC] ; line 413
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 414
	0x66, 0x9D,                                           // POPF ; line 416
	0xD9, 0xEE,                                           // FLDZ ; line 417
	0xD9, 0x9C, 0x24, 0xFC, 0x01, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x1FC] ; line 418
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x548419 ; line 420
};
static JumpTarget kaguya5C_NEW1_jumpData[] = {
	{ 0x026, 0x47A9E0 },
	{ 0x044, 0x548419 },
};
static PatchByteCode kaguya5C_NEW1_patchByteCode = { kaguya5C_NEW1_byteCode, 72, kaguya5C_NEW1_jumpData, 2 };
// kaguya5C_COUNT declared in kaguya.asm line 424
static unsigned char kaguya5C_COUNT_byteCode[] = {
	0x81, 0xC7, 0x2D, 0x00, 0x00, 0x00, // ADD EDI,0x2D ; line 425
	0x81, 0xFF, 0x0E, 0x01, 0x00, 0x00, // CMP EDI,0x0000010E ; line 426
};
static PatchByteCode kaguya5C_COUNT_patchByteCode = { kaguya5C_COUNT_byteCode, 12, nullptr, 0 };
// kaguya5C_ANG declared in kaguya.asm line 431
static unsigned char kaguya5C_ANG_byteCode[] = {
	0x31, 0xFF,                         // XOR EDI,EDI ; line 432
	0x81, 0xC7, 0x87, 0x00, 0x00, 0x00, // ADD EDI,0x87 ; line 433
	0x89, 0x7C, 0x24, 0x18,             // MOV [ESP+0x18],EDI ; line 434
};
static PatchByteCode kaguya5C_ANG_patchByteCode = { kaguya5C_ANG_byteCode, 12, nullptr, 0 };
// kaguya5C_ANGJ declared in kaguya.asm line 436
static unsigned char kaguya5C_ANGJ_byteCode[] = {
	0x31, 0xFF,                         // XOR EDI,EDI ; line 437
	0xD9, 0xEE,                         // FLDZ ; line 438
	0x81, 0xC7, 0x87, 0x00, 0x00, 0x00, // ADD EDI,0x87 ; line 439
	0x89, 0x7C, 0x24, 0x18,             // MOV [ESP+0x18],EDI ; line 440
};
static PatchByteCode kaguya5C_ANGJ_patchByteCode = { kaguya5C_ANGJ_byteCode, 14, nullptr, 0 };
// kaguya5C_SPD declared in kaguya.asm line 446
static unsigned char kaguya5C_SPD_byteCode[] = {
	0xD9, 0x05, 0x60, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B960] ; line 448
};
static PatchByteCode kaguya5C_SPD_patchByteCode = { kaguya5C_SPD_byteCode, 6, nullptr, 0 };
// kaguya5C_GRAZERES declared in kaguya.asm line 453
static unsigned char kaguya5C_GRAZERES_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 454
	0x89, 0xF1,                   // MOV ECX,ESI ; line 455
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CDE0 ; line 456
};
static JumpTarget kaguya5C_GRAZERES_jumpData[] = {
	{ 0x005, 0x48CDE0 },
};
static PatchByteCode kaguya5C_GRAZERES_patchByteCode = { kaguya5C_GRAZERES_byteCode, 9, kaguya5C_GRAZERES_jumpData, 1 };
// kaguya5C_DENSITY declared in kaguya.asm line 460
static unsigned char kaguya5C_DENSITY_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 461
	0x89, 0xF1,                   // MOV ECX,ESI ; line 462
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CE30 ; line 463
};
static JumpTarget kaguya5C_DENSITY_jumpData[] = {
	{ 0x005, 0x48CE30 },
};
static PatchByteCode kaguya5C_DENSITY_patchByteCode = { kaguya5C_DENSITY_byteCode, 9, kaguya5C_DENSITY_jumpData, 1 };
// kaguya5C_OFFSCREEN1 declared in kaguya.asm line 467
static unsigned char kaguya5C_OFFSCREEN1_byteCode[] = {
	0xD9, 0x05, 0xB8, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BAB8] ; line 468
	0xD8, 0x25, 0x14, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x867814] ; line 469
	0xD8, 0x25, 0x14, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x867814] ; line 470
};
static PatchByteCode kaguya5C_OFFSCREEN1_patchByteCode = { kaguya5C_OFFSCREEN1_byteCode, 18, nullptr, 0 };
// kaguya5C_OFFSCREEN2 declared in kaguya.asm line 474
static unsigned char kaguya5C_OFFSCREEN2_byteCode[] = {
	0xD9, 0x05, 0xC0, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BAC0] ; line 475
	0xD8, 0x05, 0x14, 0x78, 0x86, 0x00, // FADD DWORD PTR [0x867814] ; line 476
	0xD8, 0x05, 0x14, 0x78, 0x86, 0x00, // FADD DWORD PTR [0x867814] ; line 477
};
static PatchByteCode kaguya5C_OFFSCREEN2_patchByteCode = { kaguya5C_OFFSCREEN2_byteCode, 18, nullptr, 0 };
// kaguya5C_CURVE declared in kaguya.asm line 483
static unsigned char kaguya5C_CURVE_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 484
	0x60,                                                       // PUSHAD ; line 485
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000029 ; line 486
	0x0F, 0x84, 0x07, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 487
	0xD9, 0xEE,                                                 // FLDZ ; line 488
	0xE9, 0x06, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 489
	0xD9, 0x05, 0x3C, 0x15, 0x87, 0x00,                         // FLD DWORD PTR [0x87153C] ; line 491
	0x61,                                                       // POPAD ; line 493
	0x66, 0x9D,                                                 // POPF ; line 494
};
static PatchByteCode kaguya5C_CURVE_patchByteCode = { kaguya5C_CURVE_byteCode, 35, nullptr, 0 };
// kaguyaF5A_DIST declared in kaguya.asm line 503
static unsigned char kaguyaF5A_DIST_byteCode[] = {
	0xD8, 0x1D, 0x00, 0xAD, 0x85, 0x00, // FCOMP DWORD PTR [0x85AD00] ; line 504
};
static PatchByteCode kaguyaF5A_DIST_patchByteCode = { kaguyaF5A_DIST_byteCode, 6, nullptr, 0 };
// kaguya6C_SPD declared in kaguya.asm line 509
static unsigned char kaguya6C_SPD_byteCode[] = {
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00, // FLD DWORD PTR [0x85B870] ; line 510
};
static PatchByteCode kaguya6C_SPD_patchByteCode = { kaguya6C_SPD_byteCode, 6, nullptr, 0 };
// kaguya6C_OFFSET declared in kaguya.asm line 513
static unsigned char kaguya6C_OFFSET_byteCode[] = {
	0x6B, 0xD2, 0x55,                   // IMUL EDX,EDX,0x55 ; line 514
	0x01, 0xC2,                         // ADD EDX,EAX ; line 515
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00, // SUB ESP,0x08 ; line 516
};
static PatchByteCode kaguya6C_OFFSET_patchByteCode = { kaguya6C_OFFSET_byteCode, 11, nullptr, 0 };
// kaguyaJ6C_OFFSET declared in kaguya.asm line 518
static unsigned char kaguyaJ6C_OFFSET_byteCode[] = {
	0x6B, 0xD2, 0x55,       // IMUL EDX,EDX,0x55 ; line 519
	0xD9, 0x5C, 0x24, 0x1C, // FSTP DWORD PTR [ESP+0x1C] ; line 520
};
static PatchByteCode kaguyaJ6C_OFFSET_patchByteCode = { kaguyaJ6C_OFFSET_byteCode, 7, nullptr, 0 };
// kaguyaJ2C_SPD declared in kaguya.asm line 532
static unsigned char kaguyaJ2C_SPD_byteCode[] = {
	0xD9, 0x05, 0x60, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B960] ; line 536
};
static PatchByteCode kaguyaJ2C_SPD_patchByteCode = { kaguyaJ2C_SPD_byteCode, 6, nullptr, 0 };
// kaguyaJ2C_ANG1 declared in kaguya.asm line 540
static unsigned char kaguyaJ2C_ANG1_byteCode[] = {
	0xBF, 0xCD, 0x00, 0x00, 0x00, // MOV EDI,0x000000CD ; line 541
};
static PatchByteCode kaguyaJ2C_ANG1_patchByteCode = { kaguyaJ2C_ANG1_byteCode, 5, nullptr, 0 };
// kaguyaJ2C_ANG2 declared in kaguya.asm line 545
static unsigned char kaguyaJ2C_ANG2_byteCode[] = {
	0x81, 0xC7, 0x0F, 0x00, 0x00, 0x00, // ADD EDI,0x0F ; line 546
	0x81, 0xFF, 0x18, 0x01, 0x00, 0x00, // CMP EDI,0x118 ; line 547
};
static PatchByteCode kaguyaJ2C_ANG2_patchByteCode = { kaguyaJ2C_ANG2_byteCode, 12, nullptr, 0 };
// kaguyaJ2C_TO5C declared in kaguya.asm line 551
static unsigned char kaguyaJ2C_TO5C_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x5594A1 ; line 552
};
static JumpTarget kaguyaJ2C_TO5C_jumpData[] = {
	{ 0x001, 0x5594A1 },
};
static PatchByteCode kaguyaJ2C_TO5C_patchByteCode = { kaguyaJ2C_TO5C_byteCode, 5, kaguyaJ2C_TO5C_jumpData, 1 };
// kaguyaJ236B_ANG declared in kaguya.asm line 557
static unsigned char kaguyaJ236B_ANG_byteCode[] = {
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B984] ; line 558
};
static PatchByteCode kaguyaJ236B_ANG_patchByteCode = { kaguyaJ236B_ANG_byteCode, 6, nullptr, 0 };
// kaguyaA22_LV declared in kaguya.asm line 566
static unsigned char kaguyaA22_LV_byteCode[] = {
	0x80, 0xB8, 0xAA, 0x06, 0x00, 0x00, 0x00, // CMP BYTE PTR [EAX+0x6AA],0x00 ; line 567
};
static PatchByteCode kaguyaA22_LV_patchByteCode = { kaguyaA22_LV_byteCode, 7, nullptr, 0 };
// kaguyaD214 declared in kaguya.asm line 572
static unsigned char kaguyaD214_byteCode[] = {
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00, // MOV EAX,[ESI+0x398] ; line 573
	0x38, 0x98, 0xA7, 0x06, 0x00, 0x00, // CMP [EAX+0x6A7],BL ; line 574
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x55E7EA ; line 575
};
static JumpTarget kaguyaD214_jumpData[] = {
	{ 0x00D, 0x55E7EA },
};
static PatchByteCode kaguyaD214_patchByteCode = { kaguyaD214_byteCode, 17, kaguyaD214_jumpData, 1 };
// kaguyaSTATIC_INVLV declared in kaguya.asm line 580
static unsigned char kaguyaSTATIC_INVLV_byteCode[] = {
	0x80, 0xBE, 0xB0, 0x06, 0x00, 0x00, 0x05, // CMP BYTE PTR [ESI+0x6B0],0x05 ; line 581
};
static PatchByteCode kaguyaSTATIC_INVLV_patchByteCode = { kaguyaSTATIC_INVLV_byteCode, 7, nullptr, 0 };
// kaguya66B declared in kaguya.asm line 588
static unsigned char kaguya66B_byteCode[] = {
	0x0F, 0xB7, 0x86, 0xD0, 0x07, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x7D0] ; line 589
	0x66, 0x81, 0xF8, 0x04, 0x00,             // CMP AX,0x0004 ; line 590
};
static PatchByteCode kaguya66B_patchByteCode = { kaguya66B_byteCode, 12, nullptr, 0 };
// kaguyaFALL_0 declared in kaguya.asm line 595
static unsigned char kaguyaFALL_0_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x06 ; line 596
};
static PatchByteCode kaguyaFALL_0_patchByteCode = { kaguyaFALL_0_byteCode, 10, nullptr, 0 };
// kaguya6C_ANG_1A declared in kaguya.asm line 603
static unsigned char kaguya6C_ANG_1A_byteCode[] = {
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x86782C] ; line 604
};
static PatchByteCode kaguya6C_ANG_1A_patchByteCode = { kaguya6C_ANG_1A_byteCode, 6, nullptr, 0 };
// kaguya6C_ANG_1B declared in kaguya.asm line 606
static unsigned char kaguya6C_ANG_1B_byteCode[] = {
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00, // FSUB DWORD PTR [0x85B3B0] ; line 607
};
static PatchByteCode kaguya6C_ANG_1B_patchByteCode = { kaguya6C_ANG_1B_byteCode, 6, nullptr, 0 };
// kaguya6C_COUNTA declared in kaguya.asm line 609
static unsigned char kaguya6C_COUNTA_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x08, 0x00, // CMP WORD PTR [ESI+0x7D2],0x08 ; line 610
};
static PatchByteCode kaguya6C_COUNTA_patchByteCode = { kaguya6C_COUNTA_byteCode, 9, nullptr, 0 };
// kaguya6C_COUNTB declared in kaguya.asm line 612
static unsigned char kaguya6C_COUNTB_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x10, 0x00, // CMP WORD PTR [ESI+0x7D2],0x10 ; line 613
};
static PatchByteCode kaguya6C_COUNTB_patchByteCode = { kaguya6C_COUNTB_byteCode, 9, nullptr, 0 };
// kaguya6C_0A declared in kaguya.asm line 615
static unsigned char kaguya6C_0A_byteCode[] = {
	0x6A, 0x14,                   // PUSH 0x14 ; line 616
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x4099F0 ; line 617
};
static JumpTarget kaguya6C_0A_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode kaguya6C_0A_patchByteCode = { kaguya6C_0A_byteCode, 7, kaguya6C_0A_jumpData, 1 };
// kaguya6C_0B declared in kaguya.asm line 619
static unsigned char kaguya6C_0B_byteCode[] = {
	0x6A, 0x1E,                   // PUSH 0x1E ; line 620
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x4099F0 ; line 621
};
static JumpTarget kaguya6C_0B_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode kaguya6C_0B_patchByteCode = { kaguya6C_0B_byteCode, 7, kaguya6C_0B_jumpData, 1 };
// kaguyaC6A_X declared in kaguya.asm line 638
static unsigned char kaguyaC6A_X_byteCode[] = {
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 639
};
static PatchByteCode kaguyaC6A_X_patchByteCode = { kaguyaC6A_X_byteCode, 6, nullptr, 0 };
// kaguyaC6A_DEC declared in kaguya.asm line 643
static unsigned char kaguyaC6A_DEC_byteCode[] = {
	0xDC, 0x25, 0xC8, 0x77, 0x86, 0x00, // FSUB QWORD PTR [0x8677C8] ; line 644
};
static PatchByteCode kaguyaC6A_DEC_patchByteCode = { kaguyaC6A_DEC_byteCode, 6, nullptr, 0 };
// kaguyaC3A_X declared in kaguya.asm line 649
static unsigned char kaguyaC3A_X_byteCode[] = {
	0xD9, 0x05, 0x30, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB30] ; line 650
};
static PatchByteCode kaguyaC3A_X_patchByteCode = { kaguyaC3A_X_byteCode, 6, nullptr, 0 };
// kaguya2SCFWORK_SPD declared in kaguya.asm line 658
static unsigned char kaguya2SCFWORK_SPD_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 659
};
static PatchByteCode kaguya2SCFWORK_SPD_patchByteCode = { kaguya2SCFWORK_SPD_byteCode, 6, nullptr, 0 };
// kaguya3SCSTOP_FLASH declared in kaguya.asm line 667
static unsigned char kaguya3SCSTOP_FLASH_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x5502CB ; line 668
	0x6A, 0x50,                         // PUSH 0x50 ; line 669
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x550251 ; line 670
};
static JumpTarget kaguya3SCSTOP_FLASH_jumpData[] = {
	{ 0x002, 0x5502CB },
	{ 0x009, 0x550251 },
};
static PatchByteCode kaguya3SCSTOP_FLASH_patchByteCode = { kaguya3SCSTOP_FLASH_byteCode, 13, kaguya3SCSTOP_FLASH_jumpData, 2 };
// kaguyaD623_LV declared in kaguya.asm line 676
static unsigned char kaguyaD623_LV_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 677
	0x0F, 0x85, 0x86, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 678
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 679
	0x0F, 0x85, 0x77, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 680
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 681
	0x0F, 0x85, 0x68, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 682
	0x0F, 0xB6, 0x86, 0xA6, 0x06, 0x00, 0x00,             // MOVZX EAX, BYTE PTR [ESI+0x6A6] ; line 683
	0x80, 0xF8, 0x00,                                     // CMP AL,0x00 ; line 684
	0x0F, 0x84, 0x58, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 685
	0x80, 0xF8, 0x01,                                     // CMP AL,0x01 ; line 686
	0x0F, 0x84, 0x17, 0x00, 0x00, 0x00,                   // JE SHORT LABELV1 ; line 687
	0x80, 0xF8, 0x02,                                     // CMP AL,0x02 ; line 688
	0x0F, 0x84, 0x1C, 0x00, 0x00, 0x00,                   // JE SHORT LABELV2 ; line 689
	0x80, 0xF8, 0x03,                                     // CMP AL,0x03 ; line 690
	0x0F, 0x84, 0x21, 0x00, 0x00, 0x00,                   // JE SHORT LABELV3 ; line 691
	0xE9, 0x2A, 0x00, 0x00, 0x00,                         // JMP SHORT LABELV4 ; line 692
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x142],0x02 ; line 694
	0xE9, 0x2A, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL1 ; line 695
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x04, 0x00, // MOV WORD PTR [ESI+0x142],0x04 ; line 697
	0xE9, 0x1C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL1 ; line 698
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x05, 0x00, // MOV WORD PTR [ESI+0x142],0x05 ; line 700
	0xE9, 0x0E, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL1 ; line 701
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x06, 0x00, // MOV WORD PTR [ESI+0x142],0x06 ; line 703
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL1 ; line 704
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x13E] ; line 706
};
static PatchByteCode kaguyaD623_LV_patchByteCode = { kaguyaD623_LV_byteCode, 156, nullptr, 0 };
// kaguyaD623_0 declared in kaguya.asm line 712
static unsigned char kaguyaD623_0_byteCode[] = {
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 713
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 714
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x54C284 ; line 715
};
static JumpTarget kaguyaD623_0_jumpData[] = {
	{ 0x012, 0x54C284 },
};
static PatchByteCode kaguyaD623_0_patchByteCode = { kaguyaD623_0_byteCode, 22, kaguyaD623_0_jumpData, 1 };
// kaguyaD623_1 declared in kaguya.asm line 717
static unsigned char kaguyaD623_1_byteCode[] = {
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 718
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 719
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x54C550 ; line 720
};
static JumpTarget kaguyaD623_1_jumpData[] = {
	{ 0x012, 0x54C550 },
};
static PatchByteCode kaguyaD623_1_patchByteCode = { kaguyaD623_1_byteCode, 22, kaguyaD623_1_jumpData, 1 };
// kaguyaD623_SKIP declared in kaguya.asm line 726
static unsigned char kaguyaD623_SKIP_byteCode[] = {
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00, // FLD DWORD PTR [ESI+0x11C] ; line 727
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x55D11E ; line 728
};
static JumpTarget kaguyaD623_SKIP_jumpData[] = {
	{ 0x007, 0x55D11E },
};
static PatchByteCode kaguyaD623_SKIP_patchByteCode = { kaguyaD623_SKIP_byteCode, 11, kaguyaD623_SKIP_jumpData, 1 };
// kaguyaD623_A1 declared in kaguya.asm line 737
static unsigned char kaguyaD623_A1_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 738
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x4099F0 ; line 739
};
static JumpTarget kaguyaD623_A1_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode kaguyaD623_A1_patchByteCode = { kaguyaD623_A1_byteCode, 7, kaguyaD623_A1_jumpData, 1 };
// kaguyaD623_A2 declared in kaguya.asm line 741
static unsigned char kaguyaD623_A2_byteCode[] = {
	0x6A, 0x01,                         // PUSH 0x01 ; line 742
	0xD9, 0x9E, 0x24, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x124] ; line 743
};
static PatchByteCode kaguyaD623_A2_patchByteCode = { kaguyaD623_A2_byteCode, 8, nullptr, 0 };
// kaguyaD623_B declared in kaguya.asm line 745
static unsigned char kaguyaD623_B_byteCode[] = {
	0x6A, 0x01, // PUSH 0x00000001 ; line 746
};
static PatchByteCode kaguyaD623_B_patchByteCode = { kaguyaD623_B_byteCode, 2, nullptr, 0 };
// kaguya0 declared in kaguya.asm line 758
static unsigned char kaguya0_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x13E],0x06 ; line 759
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x56BEFA ; line 760
};
static JumpTarget kaguya0_jumpData[] = {
	{ 0x00A, 0x56BEFA },
};
static PatchByteCode kaguya0_patchByteCode = { kaguya0_byteCode, 14, kaguya0_jumpData, 1 };
// kaguyaPHILO declared in kaguya.asm line 767
static unsigned char kaguyaPHILO_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 768
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x398] ; line 769
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x5D, 0x02, // CMP WORD PTR [EAX+0x13C],0x025D ; line 770
	0x0F, 0x84, 0x09, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 771
	0x66, 0xC7, 0x80, 0x90, 0x08, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [EAX+0x890],0x0000 ; line 772
	0x61,                                                 // POPAD ; line 774
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x5630C8 ; line 775
};
static JumpTarget kaguyaPHILO_jumpData[] = {
	{ 0x021, 0x5630C8 },
};
static PatchByteCode kaguyaPHILO_patchByteCode = { kaguyaPHILO_byteCode, 37, kaguyaPHILO_jumpData, 1 };
// kaguyaPHILO_0 declared in kaguya.asm line 779
static unsigned char kaguyaPHILO_0_byteCode[] = {
	0x0F, 0xB7, 0x86, 0x6E, 0x03, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x36E] ; line 780
	0x66, 0x81, 0xF8, 0x1E, 0x00,             // CMP AX,0x001E ; line 781
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x5632BF ; line 782
};
static JumpTarget kaguyaPHILO_0_jumpData[] = {
	{ 0x00D, 0x5632BF },
};
static PatchByteCode kaguyaPHILO_0_patchByteCode = { kaguyaPHILO_0_byteCode, 17, kaguyaPHILO_0_jumpData, 1 };
// kaguyaPHILO_SPD declared in kaguya.asm line 786
static unsigned char kaguyaPHILO_SPD_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 787
};
static PatchByteCode kaguyaPHILO_SPD_patchByteCode = { kaguyaPHILO_SPD_byteCode, 6, nullptr, 0 };
// kaguyaPHILO_DUR declared in kaguya.asm line 791
static unsigned char kaguyaPHILO_DUR_byteCode[] = {
	0x66, 0xC7, 0x86, 0x90, 0x08, 0x00, 0x00, 0xF0, 0x00, // MOV WORD PTR [ESI+0x890],0x00F0 ; line 792
};
static PatchByteCode kaguyaPHILO_DUR_patchByteCode = { kaguyaPHILO_DUR_byteCode, 9, nullptr, 0 };
// kaguyaFLD1 declared in kaguya.asm line 796
static unsigned char kaguyaFLD1_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 797
};
static PatchByteCode kaguyaFLD1_patchByteCode = { kaguyaFLD1_byteCode, 2, nullptr, 0 };
// kaguyaD22_0 declared in kaguya.asm line 806
static unsigned char kaguyaD22_0_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x14 ; line 807
	0x0F, 0x8C, 0x07, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 808
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 809
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x55B085 ; line 811
};
static JumpTarget kaguyaD22_0_jumpData[] = {
	{ 0x018, 0x55B085 },
};
static PatchByteCode kaguyaD22_0_patchByteCode = { kaguyaD22_0_byteCode, 28, kaguyaD22_0_jumpData, 1 };
// kaguyaD22_COLORS0 declared in kaguya.asm line 814
static unsigned char kaguyaD22_COLORS0_byteCode[] = {
	0x8A, 0x86, 0x13, 0x01, 0x00, 0x00, // MOV AL,[ESI+0x113] ; line 815
	0x80, 0xF8, 0x04,                   // CMP AL,0x04 ; line 816
};
static PatchByteCode kaguyaD22_COLORS0_patchByteCode = { kaguyaD22_COLORS0_byteCode, 9, nullptr, 0 };
// kaguyaD22_COLORS1 declared in kaguya.asm line 818
static unsigned char kaguyaD22_COLORS1_byteCode[] = {
	0x80, 0xE8, 0x04,                   // SUB AL,0x04 ; line 819
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00, // MOV [ESI+0x113],AL ; line 820
};
static PatchByteCode kaguyaD22_COLORS1_patchByteCode = { kaguyaD22_COLORS1_byteCode, 9, nullptr, 0 };
// kaguyaD22B_BDELAY declared in kaguya.asm line 826
static unsigned char kaguyaD22B_BDELAY_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 827
};
static PatchByteCode kaguyaD22B_BDELAY_patchByteCode = { kaguyaD22B_BDELAY_byteCode, 6, nullptr, 0 };
// kaguyaSYNTH declared in kaguya.asm line 839
static unsigned char kaguyaSYNTH_byteCode[] = {
	0x81, 0xC4, 0x04, 0x00, 0x00, 0x00,                         // ADD ESP,0x04 ; line 840
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x06 ; line 841
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 842
	0xFE, 0x86, 0x94, 0x01, 0x00, 0x00,                         // INC BYTE PTR [ESI+0x194] ; line 843
	0xE9, 0x2D, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 844
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x01 ; line 846
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 847
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x02 ; line 848
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 849
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 851
	0x0F, 0x8E, 0x0A, 0x00, 0x00, 0x00,                         // JLE SHORT LABEL4 ; line 852
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 854
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000012C ; line 856
	0x0F, 0x85, 0x16, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL5 ; line 857
	0x60,                                                       // PUSHAD ; line 858
	0x6A, 0x03,                                                 // PUSH 0x03 ; line 859
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 860
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 861
	0x61,                                                       // POPAD ; line 862
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x0A,                   // MOV BYTE PTR [ESI+0x194],0x0A ; line 863
	0xD8, 0x1D, 0x60, 0x78, 0x86, 0x00,                         // FCOMP DWORD PTR [0x867860] ; line 865
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 866
	0x8B, 0x8E, 0x70, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x170] ; line 867
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x5652DF ; line 868
};
static JumpTarget kaguyaSYNTH_jumpData[] = {
	{ 0x072, 0x464980 },
	{ 0x08D, 0x5652DF },
};
static PatchByteCode kaguyaSYNTH_patchByteCode = { kaguyaSYNTH_byteCode, 145, kaguyaSYNTH_jumpData, 2 };
// kaguyaSYNTH_SPD declared in kaguya.asm line 873
static unsigned char kaguyaSYNTH_SPD_byteCode[] = {
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867860] ; line 874
};
static PatchByteCode kaguyaSYNTH_SPD_patchByteCode = { kaguyaSYNTH_SPD_byteCode, 6, nullptr, 0 };
// kaguyaSYNTH_0 declared in kaguya.asm line 880
static unsigned char kaguyaSYNTH_0_byteCode[] = {
	0xD9, 0x05, 0xA4, 0x75, 0x85, 0x00, // FLD DWORD PTR [0x8575A4] ; line 881
};
static PatchByteCode kaguyaSYNTH_0_patchByteCode = { kaguyaSYNTH_0_byteCode, 6, nullptr, 0 };
// kaguyaA22ROCK_0 declared in kaguya.asm line 900
static unsigned char kaguyaA22ROCK_0_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 901
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 902
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x13E],0x00 ; line 905
	0x0F, 0x85, 0x37, 0x00, 0x00, 0x00,                         // JNE SHORT LABELX ; line 906
	0x66, 0x81, 0xB8, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x140],0x00 ; line 907
	0x0F, 0x85, 0x28, 0x00, 0x00, 0x00,                         // JNE SHORT LABELX ; line 908
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02,       // CMP WORD PTR [EAX+0x13C],0x20D ; line 909
	0x0F, 0x8C, 0x19, 0x00, 0x00, 0x00,                         // JL SHORT LABELX ; line 910
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x10, 0x02,       // CMP WORD PTR [EAX+0x13C],0x210 ; line 911
	0x0F, 0x8F, 0x0A, 0x00, 0x00, 0x00,                         // JG SHORT LABELX ; line 912
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00 ; line 913
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 917
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 918
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 919
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 920
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CE90 ; line 921
	0x84, 0xC0,                                                 // TEST AL,AL ; line 922
	0x61,                                                       // POPAD ; line 923
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x6365F7 ; line 924
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 927
};
static JumpTarget kaguyaA22ROCK_0_jumpData[] = {
	{ 0x056, 0x48CE90 },
	{ 0x05F, 0x6365F7 },
};
static PatchByteCode kaguyaA22ROCK_0_patchByteCode = { kaguyaA22ROCK_0_byteCode, 105, kaguyaA22ROCK_0_jumpData, 2 };
// kaguyaA22ROCK_Z declared in kaguya.asm line 932
static unsigned char kaguyaA22ROCK_Z_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00 ; line 933
};
static PatchByteCode kaguyaA22ROCK_Z_patchByteCode = { kaguyaA22ROCK_Z_byteCode, 10, nullptr, 0 };
// kaguyaA22ROCK_1 declared in kaguya.asm line 937
static unsigned char kaguyaA22ROCK_1_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00 ; line 938
};
static PatchByteCode kaguyaA22ROCK_1_patchByteCode = { kaguyaA22ROCK_1_byteCode, 10, nullptr, 0 };
// kaguyaHANGEKI_MOMENTUM declared in kaguya.asm line 946
static unsigned char kaguyaHANGEKI_MOMENTUM_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 947
	0x0F, 0x85, 0x1B, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 948
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 949
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 950
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                   // FLD DWORD PTR [0x86782C] ; line 951
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 952
	0x0F, 0x8C, 0x2E, 0x00, 0x00, 0x00,                   // JL SHORT LABEL2 ; line 954
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x07, 0x00, // CMP WORD PTR [ESI+0x140],0x07 ; line 955
	0x0F, 0x8C, 0x0D, 0x00, 0x00, 0x00,                   // JL SHORT LABEL3 ; line 956
	0xD9, 0xEE,                                           // FLDZ ; line 957
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 958
	0xE9, 0x12, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 959
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF4] ; line 961
	0xD8, 0x25, 0x28, 0x78, 0x86, 0x00,                   // FSUB DWORD PTR [0x867828] ; line 962
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 963
	0x80, 0xBE, 0x9A, 0x04, 0x00, 0x00, 0x00,             // CMP BYTE PTR [ESI+0x49A],0x00 ; line 965
};
static PatchByteCode kaguyaHANGEKI_MOMENTUM_patchByteCode = { kaguyaHANGEKI_MOMENTUM_byteCode, 101, nullptr, 0 };


// kaguya.asm
std::array<PatchSkeleton, 142> patchList = {
	PatchSkeleton{ kaguyaID_FIX_patchByteCode,           (void *)0x541ECE,  7}, // Declared line 14. Patch on CMP [ESI+0x196], DI
	PatchSkeleton{ kaguyaA623TIME_LV_patchByteCode,      (void *)0x55D982,  7}, // Declared line 22. Patch on CMP BYTE PTR [EAX+0x6AB], 3
	PatchSkeleton{ kaguyaA623TIME_LV_patchByteCode,      (void *)0x55DA0B,  7}, // Declared line 23. Patch on CMP BYTE PTR [EAX+0x6AB], 4
	PatchSkeleton{ kaguyaD22B_DIST_patchByteCode,        (void *)0x54B160,  6}, // Declared line 59. Patch on FLD ST, QWORD PTR [0x857F68]
	PatchSkeleton{ kaguyaD22C_DIST_patchByteCode,        (void *)0x54B2A0,  6}, // Declared line 60. Patch on FLD ST, QWORD PTR [0x857F68]
	PatchSkeleton{ kaguyaELIXIR_GRAVITY_patchByteCode,   (void *)0x550D92,  6}, // Declared line 68. Patch on FLD ST, DWORD PTR [0x85AE7C]
	PatchSkeleton{ kaguyaELIXIR_Y_patchByteCode,         (void *)0x550D81,  6}, // Declared line 69. Patch on FLD ST, DWORD PTR [0x85AD0C]
	PatchSkeleton{ kaguyaFLDZ_patchByteCode,             (void *)0x550D6D,  6}, // Declared line 72. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ kaguya2C_ONHIT_patchByteCode,         (void *)0x559BFC,  6}, // Declared line 87. Patch on MOV EAX, [ESI+0x144]
	PatchSkeleton{ kaguyaD236_ORBFREQ_patchByteCode,     (void *)0x55A000,  5}, // Declared line 92. Patch on MOV ECX, 6
	PatchSkeleton{ kaguyaD236_ORBSTARTUP_patchByteCode,  (void *)0x55A009,  6}, // Declared line 99. Patch on JNE 0x0055A0EC
	PatchSkeleton{ kaguyaD236_ONHIT_patchByteCode,       (void *)0x559FC8,  5}, // Declared line 135. Patch on MOV EBP, 5
	PatchSkeleton{ kaguya2C_NEW_patchByteCode,           (void *)0x559C76,  6}, // Declared line 151. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ kaguya5AA_1HIT_patchByteCode,         (void *)0x554F8D,  7}, // Declared line 156. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ kaguya5AA_1HIT_patchByteCode,         (void *)0x554EAF,  7}, // Declared line 158. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ kaguya5AA_PARTICLE_patchByteCode,     (void *)0x560935,  5}, // Declared line 163. Patch on JMP 0x0056089F
	PatchSkeleton{ kaguyaD214_ALWAYSHOME1_patchByteCode, (void *)0x55EA8A,  6}, // Declared line 221. Patch on MOV [ESI+0x190], EBP
	PatchSkeleton{ kaguyaD214_ALWAYSHOME2_patchByteCode, (void *)0x55E89A,  7}, // Declared line 222. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ kaguyaD214_SPDIDK_patchByteCode,      (void *)0x55EA78,  6}, // Declared line 238. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ kaguyaD214_SPDX_patchByteCode,        (void *)0x55E954,  6}, // Declared line 239. Patch on FMUL ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ kaguyaD214_SPDY_patchByteCode,        (void *)0x55E966,  6}, // Declared line 240. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ kaguyaD214_FAN0_patchByteCode,        (void *)0x54DC30,  6}, // Declared line 245. Patch on FMUL ST, QWORD PTR [0x857930]
	PatchSkeleton{ kaguyaD214_FAN0_patchByteCode,        (void *)0x54DF08,  6}, // Declared line 246. Patch on FMUL ST, QWORD PTR [0x858EC0]
	PatchSkeleton{ kaguyaD214_FAN1_patchByteCode,        (void *)0x54DCDC,  6}, // Declared line 250. Patch on FMUL ST, QWORD PTR [0x85C8E0]
	PatchSkeleton{ kaguyaD214_FAN1_patchByteCode,        (void *)0x54DFC5,  6}, // Declared line 251. Patch on FMUL ST, QWORD PTR [0x85CBB0]
	PatchSkeleton{ kaguya4B_DENSHIT2_patchByteCode,      (void *)0x568B11,  7}, // Declared line 266. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ kaguya4B_SPD_patchByteCode,           (void *)0x559250,  9}, // Declared line 271. Patch on FLD ST, DWORD PTR [ECX+4]
	PatchSkeleton{ kaguya4B_DUR_patchByteCode,           (void *)0x559331,  7}, // Declared line 276. Patch on CMP DWORD PTR [ESI+0x144], 0x3C
	PatchSkeleton{ kaguya4B_QUANTITY_patchByteCode,      (void *)0x547A07,  6}, // Declared line 281. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ kaguya4B_QUANTITY_patchByteCode,      (void *)0x547ABE,  6}, // Declared line 282. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ kaguya4B_QUANTITY_patchByteCode,      (void *)0x547FD8,  6}, // Declared line 283. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ kaguya4B_QUANTITY_patchByteCode,      (void *)0x54808B,  6}, // Declared line 284. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ kaguya5B_NEW_patchByteCode,           (void *)0x558E27,  6}, // Declared line 333. Patch on FLD ST, DWORD PTR [0x85BAB8]
	PatchSkeleton{ kaguya5B_FIREFREQ_patchByteCode,      (void *)0x546329,  5}, // Declared line 345. Patch on MOV ECX, 3
	PatchSkeleton{ kaguya5B_NEWA_patchByteCode,          (void *)0x546345,  7}, // Declared line 346. Patch on CMP [ESI+0x7D2], CX
	PatchSkeleton{ kaguya5B_NEWACHARGE_patchByteCode,    (void *)0x5463FD,  8}, // Declared line 347. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya5B_FIREFREQ_patchByteCode,      (void *)0x546758,  5}, // Declared line 349. Patch on MOV ECX, 3
	PatchSkeleton{ kaguya5B_NEWA_patchByteCode,          (void *)0x546774,  7}, // Declared line 350. Patch on CMP [ESI+0x7D2], CX
	PatchSkeleton{ kaguya5B_NEWACHARGE_patchByteCode,    (void *)0x546828,  8}, // Declared line 351. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya5B_FIREFREQ_patchByteCode,      (void *)0x546B80,  5}, // Declared line 353. Patch on MOV ECX, 3
	PatchSkeleton{ kaguya5B_NEWA_patchByteCode,          (void *)0x546B9C,  7}, // Declared line 354. Patch on CMP [ESI+0x7D2], CX
	PatchSkeleton{ kaguya5B_NEWACHARGE_patchByteCode,    (void *)0x546C50,  8}, // Declared line 355. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya5B_FIREFREQ_patchByteCode,      (void *)0x546FD0,  5}, // Declared line 357. Patch on MOV ECX, 3
	PatchSkeleton{ kaguya5B_NEWA_patchByteCode,          (void *)0x546FDF,  7}, // Declared line 358. Patch on CMP [ESI+0x7D2], CX
	PatchSkeleton{ kaguya5B_NEWACHARGE_patchByteCode,    (void *)0x5470C8,  8}, // Declared line 359. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya5B_FIREFREQ_patchByteCode,      (void *)0x5475A7,  5}, // Declared line 361. Patch on MOV ECX, 3
	PatchSkeleton{ kaguya5B_NEWA_patchByteCode,          (void *)0x5474CD,  7}, // Declared line 362. Patch on CMP [ESI+0x7D2], CX
	PatchSkeleton{ kaguya5B_NEWACHARGE_patchByteCode,    (void *)0x5475B6,  8}, // Declared line 363. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguyaFLDZ_patchByteCode,             (void *)0x558F1A,  6}, // Declared line 365. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ kaguya5C_METER_patchByteCode,         (void *)0x5483D9, 14}, // Declared line 372. Patch on PUSH 0x32
	PatchSkeleton{ kaguyaJ5C_METER_patchByteCode,        (void *)0x548B16,  9}, // Declared line 378. Patch on PUSH 0x32
	PatchSkeleton{ kaguya5C_NEW_patchByteCode,           (void *)0x5483CB,  8}, // Declared line 398. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ kaguya5C_NEWJ_patchByteCode,          (void *)0x548AF4,  8}, // Declared line 399. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ kaguya5C_NEW1_patchByteCode,          (void *)0x548402,  9}, // Declared line 422. Patch on PUSH 0x30
	PatchSkeleton{ kaguya5C_COUNT_patchByteCode,         (void *)0x548484,  9}, // Declared line 428. Patch on ADD EDI, 0x2D
	PatchSkeleton{ kaguya5C_COUNT_patchByteCode,         (void *)0x548BA9,  9}, // Declared line 429. Patch on ADD EDI, 0x2D
	PatchSkeleton{ kaguya5C_ANG_patchByteCode,           (void *)0x548419,  6}, // Declared line 442. Patch on XOR EDI, EDI
	PatchSkeleton{ kaguya5C_ANGJ_patchByteCode,          (void *)0x548B35,  8}, // Declared line 443. Patch on XOR EDI, EDI
	PatchSkeleton{ kaguya5C_SPD_patchByteCode,           (void *)0x5483F5,  6}, // Declared line 450. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ kaguya5C_SPD_patchByteCode,           (void *)0x548B28,  6}, // Declared line 451. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ kaguya5C_GRAZERES_patchByteCode,      (void *)0x55951C,  9}, // Declared line 458. Patch on PUSH 6
	PatchSkeleton{ kaguya5C_DENSITY_patchByteCode,       (void *)0x559525,  9}, // Declared line 465. Patch on PUSH 2
	PatchSkeleton{ kaguya5C_OFFSCREEN1_patchByteCode,    (void *)0x55942E,  6}, // Declared line 472. Patch on FLD ST, DWORD PTR [0x85BAB8]
	PatchSkeleton{ kaguya5C_OFFSCREEN2_patchByteCode,    (void *)0x55944B,  6}, // Declared line 479. Patch on FLD ST, QWORD PTR [0x85BAC0]
	PatchSkeleton{ kaguya5C_CURVE_patchByteCode,         (void *)0x5594D5,  6}, // Declared line 496. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ kaguyaF5A_DIST_patchByteCode,         (void *)0x55840E,  6}, // Declared line 506. Patch on FCOMP ST, QWORD PTR [0x85B900]
	PatchSkeleton{ kaguya6C_SPD_patchByteCode,           (void *)0x5485B7,  6}, // Declared line 522. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ kaguya6C_SPD_patchByteCode,           (void *)0x54871E,  6}, // Declared line 523. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ kaguya6C_SPD_patchByteCode,           (void *)0x548D2A,  6}, // Declared line 524. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ kaguya6C_SPD_patchByteCode,           (void *)0x548E88,  6}, // Declared line 525. Patch on FLD ST, DWORD PTR [0x85C1C0]
	PatchSkeleton{ kaguya6C_OFFSET_patchByteCode,        (void *)0x5485F2,  6}, // Declared line 526. Patch on SHL EDX, 6
	PatchSkeleton{ kaguya6C_OFFSET_patchByteCode,        (void *)0x548759,  6}, // Declared line 527. Patch on SHL EDX, 6
	PatchSkeleton{ kaguyaJ6C_OFFSET_patchByteCode,       (void *)0x548D5A,  7}, // Declared line 528. Patch on IMUL EDX, 0x46
	PatchSkeleton{ kaguyaJ6C_OFFSET_patchByteCode,       (void *)0x548EB8,  7}, // Declared line 529. Patch on IMUL EDX, 0x46
	PatchSkeleton{ kaguyaJ2C_SPD_patchByteCode,          (void *)0x5491E8,  6}, // Declared line 538. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ kaguyaJ2C_ANG1_patchByteCode,         (void *)0x5491F5,  5}, // Declared line 543. Patch on MOV EDI, 0x19
	PatchSkeleton{ kaguyaJ2C_ANG2_patchByteCode,         (void *)0x549276,  6}, // Declared line 549. Patch on ADD EDI, 0xF
	PatchSkeleton{ kaguyaJ2C_TO5C_patchByteCode,         (void *)0x5596C7,  6}, // Declared line 554. Patch on MOV EAX, [ESI+0x35C]
	PatchSkeleton{ kaguyaJ236B_ANG_patchByteCode,        (void *)0x549BF7,  6}, // Declared line 560. Patch on FLD ST, DWORD PTR [0x85B98C]
	PatchSkeleton{ kaguyaFLDZ_patchByteCode,             (void *)0x549F3F,  6}, // Declared line 563. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ kaguyaA22_LV_patchByteCode,           (void *)0x55B848,  7}, // Declared line 569. Patch on CMP BYTE PTR [EAX+0x6AA], 2
	PatchSkeleton{ kaguyaD214_patchByteCode,             (void *)0x55E7DC,  6}, // Declared line 577. Patch on MOV EAX, [ESI+0x398]
	PatchSkeleton{ kaguyaSTATIC_INVLV_patchByteCode,     (void *)0x54CF14,  7}, // Declared line 583. Patch on CMP BYTE PTR [ESI+0x6B0], 2
	PatchSkeleton{ kaguyaSTATIC_INVLV_patchByteCode,     (void *)0x54D5D7,  7}, // Declared line 584. Patch on CMP BYTE PTR [ESI+0x6B0], 2
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x555496,  9}, // Declared line 586. Patch on MOV WORD PTR [ESI+0x51C], 3
	PatchSkeleton{ kaguya66B_patchByteCode,              (void *)0x547DC7,  7}, // Declared line 592. Patch on MOVZX EAX, WORD PTR [ESI+0x7D0]
	PatchSkeleton{ kaguyaFALL_0_patchByteCode,           (void *)0x54E9FD,  7}, // Declared line 598. Patch on CMP DWORD PTR [ESI+0x144], 0x18
	PatchSkeleton{ kaguyaFALL_0_patchByteCode,           (void *)0x54EC40,  7}, // Declared line 599. Patch on CMP DWORD PTR [ESI+0x144], 0x18
	PatchSkeleton{ kaguya6C_0A_patchByteCode,            (void *)0x54857B,  7}, // Declared line 623. Patch on PUSH 8
	PatchSkeleton{ kaguya6C_ANG_1A_patchByteCode,        (void *)0x548597,  6}, // Declared line 624. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ kaguya6C_COUNTA_patchByteCode,        (void *)0x54855E,  8}, // Declared line 625. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya6C_0B_patchByteCode,            (void *)0x5486E2,  7}, // Declared line 626. Patch on PUSH 0xA
	PatchSkeleton{ kaguya6C_ANG_1B_patchByteCode,        (void *)0x5486FE,  6}, // Declared line 627. Patch on FSUB ST, QWORD PTR [0x858210]
	PatchSkeleton{ kaguya6C_COUNTB_patchByteCode,        (void *)0x5486C5,  8}, // Declared line 628. Patch on CMP WORD PTR [ESI+0x7D2], 0xA
	PatchSkeleton{ kaguya6C_0A_patchByteCode,            (void *)0x548CEE,  7}, // Declared line 629. Patch on PUSH 8
	PatchSkeleton{ kaguya6C_ANG_1A_patchByteCode,        (void *)0x548D0A,  6}, // Declared line 630. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ kaguya6C_COUNTA_patchByteCode,        (void *)0x548CD1,  8}, // Declared line 631. Patch on CMP WORD PTR [ESI+0x7D2], 5
	PatchSkeleton{ kaguya6C_0B_patchByteCode,            (void *)0x548E4C,  7}, // Declared line 632. Patch on PUSH 0xA
	PatchSkeleton{ kaguya6C_ANG_1B_patchByteCode,        (void *)0x548E68,  6}, // Declared line 633. Patch on FSUB ST, QWORD PTR [0x858210]
	PatchSkeleton{ kaguya6C_COUNTB_patchByteCode,        (void *)0x548E2F,  8}, // Declared line 634. Patch on CMP WORD PTR [ESI+0x7D2], 0xA
	PatchSkeleton{ kaguyaC6A_X_patchByteCode,            (void *)0x5450B2,  6}, // Declared line 641. Patch on FLD ST, DWORD PTR [0x85BAE4]
	PatchSkeleton{ kaguyaC6A_DEC_patchByteCode,          (void *)0x544F72,  6}, // Declared line 646. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ kaguyaC3A_X_patchByteCode,            (void *)0x5453EF,  6}, // Declared line 652. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ kaguya2SCFWORK_SPD_patchByteCode,     (void *)0x5504B9,  6}, // Declared line 661. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ kaguya2SCFWORK_SPD_patchByteCode,     (void *)0x550560,  6}, // Declared line 662. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ kaguya2SCFWORK_SPD_patchByteCode,     (void *)0x55081C,  6}, // Declared line 663. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ kaguya2SCFWORK_SPD_patchByteCode,     (void *)0x5508BE,  6}, // Declared line 664. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ kaguya3SCSTOP_FLASH_patchByteCode,    (void *)0x550249,  6}, // Declared line 672. Patch on JNE 0x005502CB
	PatchSkeleton{ kaguyaD623_LV_patchByteCode,          (void *)0x54BFC6,  7}, // Declared line 708. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ kaguyaD623_LV_patchByteCode,          (void *)0x54C28C,  7}, // Declared line 709. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ kaguyaD623_0_patchByteCode,           (void *)0x54C278,  6}, // Declared line 722. Patch on FLD ST, DWORD PTR [0x85C230]
	PatchSkeleton{ kaguyaD623_1_patchByteCode,           (void *)0x54C544,  6}, // Declared line 724. Patch on FLD ST, DWORD PTR [0x85BB28]
	PatchSkeleton{ kaguyaD623_SKIP_patchByteCode,        (void *)0x55D107,  6}, // Declared line 730. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x5694AA,  6}, // Declared line 748. Patch on FADD ST, QWORD PTR [0x858210]
	PatchSkeleton{ kaguyaD623_A1_patchByteCode,          (void *)0x5694E3,  7}, // Declared line 749. Patch on PUSH 0x50
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x5694FC,  6}, // Declared line 750. Patch on FADD ST, QWORD PTR [0x857990]
	PatchSkeleton{ kaguyaD623_A2_patchByteCode,          (void *)0x569502,  8}, // Declared line 751. Patch on PUSH 0x50
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x569521,  6}, // Declared line 752. Patch on FSUB ST, QWORD PTR [0x858990]
	PatchSkeleton{ kaguyaD623_B_patchByteCode,           (void *)0x569527,  5}, // Declared line 753. Patch on PUSH 0x168
	PatchSkeleton{ kaguya0_patchByteCode,                (void *)0x56AE05,  8}, // Declared line 762. Patch on CMP WORD PTR [ESI+0x13E], 6
	PatchSkeleton{ kaguyaPHILO_patchByteCode,            (void *)0x563038,  9}, // Declared line 777. Patch on JNE SHORT 0x00563053
	PatchSkeleton{ kaguyaPHILO_0_patchByteCode,          (void *)0x5632B4,  7}, // Declared line 784. Patch on MOVZX EAX, WORD PTR [ESI+0x36E]
	PatchSkeleton{ kaguyaPHILO_SPD_patchByteCode,        (void *)0x5638AD,  9}, // Declared line 789. Patch on FLD ST, DWORD PTR [ECX+4]
	PatchSkeleton{ kaguyaPHILO_DUR_patchByteCode,        (void *)0x551442,  9}, // Declared line 794. Patch on MOV WORD PTR [ESI+0x890], 0x4B0
	PatchSkeleton{ kaguyaFLD1_patchByteCode,             (void *)0x551427,  6}, // Declared line 799. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x5417B9,  7}, // Declared line 801. Patch on MOV [ESI+0x890], BX
	PatchSkeleton{ kaguyaD22_0_patchByteCode,            (void *)0x55B06D,  6}, // Declared line 813. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ kaguyaD22_COLORS0_patchByteCode,      (void *)0x55B08B,  8}, // Declared line 822. Patch on MOV AL, [ESI+0x113]
	PatchSkeleton{ kaguyaD22_COLORS1_patchByteCode,      (void *)0x55B099,  8}, // Declared line 823. Patch on SUB AL, 0xF
	PatchSkeleton{ kaguyaD22B_BDELAY_patchByteCode,      (void *)0x54B12A,  6}, // Declared line 832. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ kaguyaD22B_BDELAY_patchByteCode,      (void *)0x54B26A,  6}, // Declared line 833. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ kaguyaSYNTH_patchByteCode,            (void *)0x565276,  6}, // Declared line 871. Patch on FCOMP ST, QWORD PTR [0x857B20]
	PatchSkeleton{ kaguyaSYNTH_SPD_patchByteCode,        (void *)0x5652EF,  6}, // Declared line 876. Patch on FLD ST, QWORD PTR [0x859910]
	PatchSkeleton{ kaguyaSYNTH_0_patchByteCode,          (void *)0x551BFA,  6}, // Declared line 883. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x54D5E0,  9}, // Declared line 886. Patch on MOV WORD PTR [ESI+0x51C], 3
	PatchSkeleton{ kaguyaA22ROCK_0_patchByteCode,        (void *)0x55B4E1,  6}, // Declared line 929. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ kaguyaA22ROCK_0_patchByteCode,        (void *)0x55BB4C,  6}, // Declared line 930. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ kaguyaA22ROCK_Z_patchByteCode,        (void *)0x55B96B,  7}, // Declared line 935. Patch on ADD DWORD PTR [ESI+0x34C], 0xFFFFFFFF
	PatchSkeleton{ kaguyaA22ROCK_1_patchByteCode,        (void *)0x55B60B,  7}, // Declared line 940. Patch on ADD DWORD PTR [ESI+0x34C], 0xFFFFFFFF
	PatchSkeleton{ kaguyaA22ROCK_1_patchByteCode,        (void *)0x55BC6F,  7}, // Declared line 941. Patch on ADD DWORD PTR [ESI+0x34C], 0xFFFFFFFF
	PatchSkeleton{ kaguyaNOP_patchByteCode,              (void *)0x560403,  5}, // Declared line 944. Patch on CMP CX, 0x2B8
	PatchSkeleton{ kaguyaHANGEKI_MOMENTUM_patchByteCode, (void *)0x5527E5,  7}, // Declared line 967. Patch on CMP BYTE PTR [ESI+0x49A], 0
};


static std::array<unsigned, 90> skeletonsForUpdate = {
	   0,   3,   4,   5,   6,   7,  21,  22,  23,  24,  28,  29,  30,  31,  33,  34,
	  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  49,  50,  51,
	  52,  53,  54,  55,  56,  57,  58,  59,  66,  67,  68,  69,  70,  71,  72,  73,
	  74,  75,  76,  78,  79,  82,  83,  85,  86,  87,  88,  89,  90,  91,  92,  93,
	  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	 110, 111, 123, 124, 125, 129, 130, 133, 134, 141,
};
static std::array<unsigned, 3> skeletonsForInitializeAction = { 13, 14, 84 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 1> skeletonsForHandleInputs = { 65 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 48> skeletonsForObjectUpdate = {
	   1,   2,   8,   9,  10,  11,  12,  15,  16,  17,  18,  19,  20,  25,  26,  27,
	  32,  48,  60,  61,  62,  63,  64,  77,  80,  81, 112, 113, 114, 115, 116, 117,
	 118, 119, 120, 121, 122, 126, 127, 128, 131, 132, 135, 136, 137, 138, 139, 140,
};
static std::array<unsigned, 8> skeletonsForObjectInitializeAction = { 25, 113, 114, 115, 116, 117, 118, 119 };



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
	printf("Internal memory used %zu/5729\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 5729);
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
