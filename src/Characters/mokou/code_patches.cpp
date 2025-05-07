#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<22392> applyMemory;
StackedMemory<10455> internalMemory;
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


// mokouFLDZ declared in mokou.asm line 1
static unsigned char mokouFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 2
};
static PatchByteCode mokouFLDZ_patchByteCode = { mokouFLDZ_byteCode, 2, nullptr, 0 };
// mokouNOP declared in mokou.asm line 4
static unsigned char mokouNOP_byteCode[] = {
	0x90, // NOP ; line 5
};
static PatchByteCode mokouNOP_patchByteCode = { mokouNOP_byteCode, 1, nullptr, 0 };
// mokouID_FIX declared in mokou.asm line 9
static unsigned char mokouID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x0000001C ; line 10
	0x66, 0x39, 0xBE, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],DI ; line 12
};
static PatchByteCode mokouID_FIX_patchByteCode = { mokouID_FIX_byteCode, 17, nullptr, 0 };
// mokouA214PUNCH_HPCOST_1 declared in mokou.asm line 16
static unsigned char mokouA214PUNCH_HPCOST_1_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x47A9E0 ; line 17
	0x0F, 0xB7, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOVZX ECX, WORD PTR [ESI+0x184] ; line 19
	0x66, 0x81, 0xE9, 0x90, 0x01,             // SUB CX,0x0190 ; line 20
	0x66, 0x81, 0xF9, 0x01, 0x00,             // CMP CX,0x0001 ; line 21
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,       // JAE LABEL3A ; line 22
	0x66, 0xB9, 0x01, 0x00,                   // MOV CX,0x0001 ; line 23
	0x66, 0x89, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOV [ESI+0x184],CX ; line 25
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x190] ; line 26
};
static JumpTarget mokouA214PUNCH_HPCOST_1_jumpData[] = {
	{ 0x001, 0x47A9E0 },
};
static PatchByteCode mokouA214PUNCH_HPCOST_1_patchByteCode = { mokouA214PUNCH_HPCOST_1_byteCode, 45, mokouA214PUNCH_HPCOST_1_jumpData, 1 };
// mokouD214_HPCOST_1 declared in mokou.asm line 29
static unsigned char mokouD214_HPCOST_1_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x47A9E0 ; line 30
	0x0F, 0xB7, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOVZX ECX, WORD PTR [ESI+0x184] ; line 32
	0x66, 0x81, 0xE9, 0x58, 0x02,             // SUB CX,0x0258 ; line 33
	0x66, 0x81, 0xF9, 0x01, 0x00,             // CMP CX,0x0001 ; line 34
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,       // JAE LABEL3A ; line 35
	0x66, 0xB9, 0x01, 0x00,                   // MOV CX,0x0001 ; line 36
	0x66, 0x89, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOV [ESI+0x184],CX ; line 38
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x190] ; line 39
};
static JumpTarget mokouD214_HPCOST_1_jumpData[] = {
	{ 0x001, 0x47A9E0 },
};
static PatchByteCode mokouD214_HPCOST_1_patchByteCode = { mokouD214_HPCOST_1_byteCode, 45, mokouD214_HPCOST_1_jumpData, 1 };
// mokouA214REKKA_HPCOST_1 declared in mokou.asm line 42
static unsigned char mokouA214REKKA_HPCOST_1_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x47A9E0 ; line 43
	0x0F, 0xB7, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOVZX ECX, WORD PTR [ESI+0x184] ; line 45
	0x66, 0x81, 0xE9, 0xC8, 0x00,             // SUB CX,0x00C8 ; line 46
	0x66, 0x81, 0xF9, 0x01, 0x00,             // CMP CX,0x0001 ; line 47
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,       // JAE LABEL3A ; line 48
	0x66, 0xB9, 0x01, 0x00,                   // MOV CX,0x0001 ; line 49
	0x66, 0x89, 0x8E, 0x84, 0x01, 0x00, 0x00, // MOV [ESI+0x184],CX ; line 51
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x190] ; line 52
};
static JumpTarget mokouA214REKKA_HPCOST_1_jumpData[] = {
	{ 0x001, 0x47A9E0 },
};
static PatchByteCode mokouA214REKKA_HPCOST_1_patchByteCode = { mokouA214REKKA_HPCOST_1_byteCode, 45, mokouA214REKKA_HPCOST_1_jumpData, 1 };
// mokouA214REKKA_DECEL declared in mokou.asm line 78
static unsigned char mokouA214REKKA_DECEL_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00000001 ; line 79
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 80
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x00000003 ; line 81
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 82
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 83
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 88
	0xD8, 0x25, 0x10, 0xC1, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C110] ; line 89
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 90
	0xD9, 0xEE,                                                 // FLDZ ; line 92
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 93
	0xDE, 0xD9,                                                 // FCOMPP ; line 94
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 96
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 97
	0x0F, 0x84, 0x08, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 98
	0xD9, 0xEE,                                                 // FLDZ ; line 100
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 101
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,                   // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 104
};
static PatchByteCode mokouA214REKKA_DECEL_patchByteCode = { mokouA214REKKA_DECEL_byteCode, 94, nullptr, 0 };
// mokouA214REKKA_DECEL2 declared in mokou.asm line 107
static unsigned char mokouA214REKKA_DECEL2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00000001 ; line 108
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 109
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x00000003 ; line 110
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 111
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 112
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 117
	0xD8, 0x25, 0x10, 0xC1, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C110] ; line 118
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 119
	0xD9, 0xEE,                                                 // FLDZ ; line 121
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 122
	0xDE, 0xD9,                                                 // FCOMPP ; line 123
	0xDF, 0xE0,                                                 // FNSTSW AX ; line 125
	0xF6, 0xC4, 0x41,                                           // TEST AH,0x41 ; line 126
	0x0F, 0x84, 0x08, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 127
	0xD9, 0xEE,                                                 // FLDZ ; line 129
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 130
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x05, 0x00,       // CMP WORD PTR [ESI+0x13E],0x05 ; line 133
};
static PatchByteCode mokouA214REKKA_DECEL2_patchByteCode = { mokouA214REKKA_DECEL2_byteCode, 96, nullptr, 0 };
// mokouA214REKKA_DECEL3 declared in mokou.asm line 142
static unsigned char mokouA214REKKA_DECEL3_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00000002 ; line 143
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 144
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x140],0x00000003 ; line 145
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 146
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 147
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0B, 0x00,       // CMP WORD PTR [ESI+0x13E],0x0000000B ; line 150
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 151
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x140],0x00000002 ; line 152
	0x0F, 0x85, 0x0A, 0x00, 0x00, 0x00,                         // JNE LABEL3 ; line 153
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 154
	0x0F, 0xB7, 0x8E, 0x3E, 0x01, 0x00, 0x00,                   // MOVZX ECX,WORD PTR [ESI+0x13E] ; line 158
	0x66, 0x81, 0xF9, 0x02, 0x00,                               // CMP CX,0x0002 ; line 160
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE LABELA1 ; line 161
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 162
	0xD8, 0x25, 0x10, 0xC1, 0x85, 0x00,                         // FSUB DWORD PTR [0x85C110] ; line 163
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 164
	0x66, 0x81, 0xF9, 0x0B, 0x00,                               // CMP CX,0x000B ; line 166
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE LABELA2 ; line 167
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 168
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0x100] ; line 169
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 170
	0x66, 0x81, 0xF9, 0x0C, 0x00,                               // CMP CX,0x000C ; line 172
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE LABELA3 ; line 173
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 174
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                         // FSUB DWORD PTR [ESI+0x100] ; line 175
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 176
};
static PatchByteCode mokouA214REKKA_DECEL3_patchByteCode = { mokouA214REKKA_DECEL3_byteCode, 174, nullptr, 0 };
// mokouA214REKKA_2MAXDUR declared in mokou.asm line 181
static unsigned char mokouA214REKKA_2MAXDUR_byteCode[] = {
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x144] ; line 182
	0x81, 0xF8, 0x1E, 0x00, 0x00, 0x00, // CMP EAX,0x1E ; line 183
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x66DDA0 ; line 184
};
static JumpTarget mokouA214REKKA_2MAXDUR_jumpData[] = {
	{ 0x00D, 0x66DDA0 },
};
static PatchByteCode mokouA214REKKA_2MAXDUR_patchByteCode = { mokouA214REKKA_2MAXDUR_byteCode, 17, mokouA214REKKA_2MAXDUR_jumpData, 1 };
// mokouA214REKKA_IDKLOL declared in mokou.asm line 188
static unsigned char mokouA214REKKA_IDKLOL_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x0002 ; line 189
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                   // JE LABELB ; line 190
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x0B, 0x00, // CMP WORD PTR [ESI+0x13E],0x000B ; line 191
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE LABELC ; line 192
	0xE9, 0x4E, 0x00, 0x00, 0x00,                         // JMP LABELNO ; line 193
	0xD9, 0x05, 0x30, 0xC2, 0x85, 0x00,                   // FLD DWORD PTR [0x85C230] ; line 196
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 197
	0xD9, 0xEE,                                           // FLDZ ; line 198
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 199
	0xD9, 0x05, 0xE8, 0xAA, 0x85, 0x00,                   // FLD DWORD PTR [0x85AAE8] ; line 200
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x100] ; line 201
	0xE9, 0x29, 0x00, 0x00, 0x00,                         // JMP LABELNO ; line 202
	0xD9, 0x05, 0x2C, 0xC2, 0x85, 0x00,                   // FLD DWORD PTR [0x85C22C] ; line 205
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 206
	0xD9, 0x05, 0x2C, 0xC2, 0x85, 0x00,                   // FLD DWORD PTR [0x85C22C] ; line 207
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 208
	0xD9, 0x05, 0x48, 0xBC, 0x85, 0x00,                   // FLD DWORD PTR [0x85BC48] ; line 209
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x100] ; line 211
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP LABELNO ; line 212
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 215
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66E0E9 ; line 216
};
static JumpTarget mokouA214REKKA_IDKLOL_jumpData[] = {
	{ 0x07B, 0x66E0E9 },
};
static PatchByteCode mokouA214REKKA_IDKLOL_patchByteCode = { mokouA214REKKA_IDKLOL_byteCode, 127, mokouA214REKKA_IDKLOL_jumpData, 1 };
// mokouA214REKKA_ENDGRAVITY declared in mokou.asm line 220
static unsigned char mokouA214REKKA_ENDGRAVITY_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 221
};
static PatchByteCode mokouA214REKKA_ENDGRAVITY_patchByteCode = { mokouA214REKKA_ENDGRAVITY_byteCode, 2, nullptr, 0 };
// mokouA214REKKA_BCHOICE declared in mokou.asm line 225
static unsigned char mokouA214REKKA_BCHOICE_byteCode[] = {
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 226
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x144] ; line 228
	0x81, 0xF8, 0x1E, 0x00, 0x00, 0x00,                   // CMP EAX,0x1E ; line 229
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00,                   // JL LABELA ; line 230
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66D1CD ; line 231
	0x81, 0xF8, 0x14, 0x00, 0x00, 0x00,                   // CMP EAX,0x14 ; line 234
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x66D2F0 ; line 235
	0x39, 0x96, 0x60, 0x07, 0x00, 0x00,                   // CMP [ESI+0x760],EDX ; line 237
	0x0F, 0x87, 0x11, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 238
	0x39, 0x96, 0x64, 0x07, 0x00, 0x00,                   // CMP [ESI+0x764],EDX ; line 239
	0x0F, 0x87, 0x3D, 0x00, 0x00, 0x00,                   // JA LABEL2 ; line 240
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66D2F0 ; line 241
	0xDD, 0xD8,                                           // FSTP ST0 ; line 244
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0000 ; line 245
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 247
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 248
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 249
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3A ; line 250
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 251
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 253
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 254
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66D2DA ; line 256
	0xDD, 0xD8,                                           // FSTP ST0 ; line 259
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0002 ; line 260
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 262
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 263
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 264
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3B ; line 265
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 266
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 268
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 269
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66D2DA ; line 271
};
static JumpTarget mokouA214REKKA_BCHOICE_jumpData[] = {
	{ 0x019, 0x66D1CD },
	{ 0x025, 0x66D2F0 },
	{ 0x042, 0x66D2F0 },
	{ 0x07A, 0x66D2DA },
	{ 0x0B2, 0x66D2DA },
};
static PatchByteCode mokouA214REKKA_BCHOICE_patchByteCode = { mokouA214REKKA_BCHOICE_byteCode, 182, mokouA214REKKA_BCHOICE_jumpData, 5 };
// mokouA214REKKA_ABCHOICE declared in mokou.asm line 275
static unsigned char mokouA214REKKA_ABCHOICE_byteCode[] = {
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 276
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x144] ; line 278
	0x81, 0xF8, 0x1E, 0x00, 0x00, 0x00,                   // CMP EAX,0x1E ; line 279
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00,                   // JL LABELA ; line 280
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66D4C9 ; line 281
	0x81, 0xF8, 0x14, 0x00, 0x00, 0x00,                   // CMP EAX,0x14 ; line 284
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x66D7F0 ; line 285
	0x39, 0x96, 0x60, 0x07, 0x00, 0x00,                   // CMP [ESI+0x760],EDX ; line 287
	0x0F, 0x87, 0x11, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 288
	0x39, 0x96, 0x64, 0x07, 0x00, 0x00,                   // CMP [ESI+0x764],EDX ; line 289
	0x0F, 0x87, 0x3B, 0x00, 0x00, 0x00,                   // JA LABEL2 ; line 290
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DA52 ; line 291
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0000 ; line 294
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 296
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 297
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 298
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3A ; line 299
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 300
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 302
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 303
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DA3C ; line 305
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0002 ; line 308
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 310
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 311
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 312
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3B ; line 313
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 314
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 316
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 317
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DA3C ; line 319
};
static JumpTarget mokouA214REKKA_ABCHOICE_jumpData[] = {
	{ 0x019, 0x66D4C9 },
	{ 0x025, 0x66D7F0 },
	{ 0x042, 0x66DA52 },
	{ 0x078, 0x66DA3C },
	{ 0x0AE, 0x66DA3C },
};
static PatchByteCode mokouA214REKKA_ABCHOICE_patchByteCode = { mokouA214REKKA_ABCHOICE_byteCode, 178, mokouA214REKKA_ABCHOICE_jumpData, 5 };
// mokouA214REKKA_CTOB declared in mokou.asm line 323
static unsigned char mokouA214REKKA_CTOB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x66D15E ; line 324
};
static JumpTarget mokouA214REKKA_CTOB_jumpData[] = {
	{ 0x001, 0x66D15E },
};
static PatchByteCode mokouA214REKKA_CTOB_patchByteCode = { mokouA214REKKA_CTOB_byteCode, 5, mokouA214REKKA_CTOB_jumpData, 1 };
// mokouA214REKKA_ACTOAB declared in mokou.asm line 328
static unsigned char mokouA214REKKA_ACTOAB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x66D778 ; line 329
};
static JumpTarget mokouA214REKKA_ACTOAB_jumpData[] = {
	{ 0x001, 0x66D778 },
};
static PatchByteCode mokouA214REKKA_ACTOAB_patchByteCode = { mokouA214REKKA_ACTOAB_byteCode, 5, mokouA214REKKA_ACTOAB_jumpData, 1 };
// mokouA214REKKA_CONTINUE1 declared in mokou.asm line 333
static unsigned char mokouA214REKKA_CONTINUE1_byteCode[] = {
	0x39, 0x96, 0x60, 0x07, 0x00, 0x00,                   // CMP [ESI+0x760],EDX ; line 334
	0x0F, 0x87, 0x11, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 335
	0x39, 0x96, 0x64, 0x07, 0x00, 0x00,                   // CMP [ESI+0x764],EDX ; line 336
	0x0F, 0x87, 0x05, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 337
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DEDC ; line 338
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x0002 ; line 340
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66DEDC ; line 341
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0002 ; line 342
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 344
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 345
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 346
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3 ; line 347
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 348
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 350
	0x0F, 0xB7, 0x86, 0xD8, 0x07, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x7D8] ; line 351
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DECD ; line 353
};
static JumpTarget mokouA214REKKA_CONTINUE1_jumpData[] = {
	{ 0x019, 0x66DEDC },
	{ 0x028, 0x66DEDC },
	{ 0x05F, 0x66DECD },
};
static PatchByteCode mokouA214REKKA_CONTINUE1_patchByteCode = { mokouA214REKKA_CONTINUE1_byteCode, 99, mokouA214REKKA_CONTINUE1_jumpData, 3 };
// mokouA214REKKA_CONTINUE2 declared in mokou.asm line 356
static unsigned char mokouA214REKKA_CONTINUE2_byteCode[] = {
	0x39, 0x96, 0x60, 0x07, 0x00, 0x00,                   // CMP [ESI+0x760],EDX ; line 357
	0x0F, 0x87, 0x11, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 358
	0x39, 0x96, 0x64, 0x07, 0x00, 0x00,                   // CMP [ESI+0x764],EDX ; line 359
	0x0F, 0x87, 0x05, 0x00, 0x00, 0x00,                   // JA LABEL1 ; line 360
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DFB3 ; line 361
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x0002 ; line 363
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66DFB3 ; line 364
	0x66, 0xC7, 0x86, 0xD0, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D0],0x0002 ; line 365
	0x0F, 0xB7, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOVZX EAX, WORD PTR [ESI+0x184] ; line 367
	0x66, 0x81, 0xE8, 0xC8, 0x00,                         // SUB AX,0x00C8 ; line 368
	0x66, 0x81, 0xF8, 0x01, 0x00,                         // CMP AX,0x0001 ; line 369
	0x0F, 0x83, 0x04, 0x00, 0x00, 0x00,                   // JAE LABEL3 ; line 370
	0x66, 0xB8, 0x01, 0x00,                               // MOV AX,0x0001 ; line 371
	0x66, 0x89, 0x86, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],AX ; line 373
	0x0F, 0xB7, 0x86, 0xD8, 0x07, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x7D8] ; line 374
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66DECD ; line 376
};
static JumpTarget mokouA214REKKA_CONTINUE2_jumpData[] = {
	{ 0x019, 0x66DFB3 },
	{ 0x028, 0x66DFB3 },
	{ 0x05F, 0x66DECD },
};
static PatchByteCode mokouA214REKKA_CONTINUE2_patchByteCode = { mokouA214REKKA_CONTINUE2_byteCode, 99, mokouA214REKKA_CONTINUE2_jumpData, 3 };
// mokouA214REKKA_3RDANG declared in mokou.asm line 383
static unsigned char mokouA214REKKA_3RDANG_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 384
};
static PatchByteCode mokouA214REKKA_3RDANG_patchByteCode = { mokouA214REKKA_3RDANG_byteCode, 2, nullptr, 0 };
// mokouA214REKKA_V2 declared in mokou.asm line 408
static unsigned char mokouA214REKKA_V2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x0000 ; line 409
	0x0F, 0x85, 0xD9, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 410
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x01 ; line 411
	0x0F, 0x85, 0xC9, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 412
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX, DWORD PTR [ESI+0x398] ; line 414
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0D, 0x02,       // CMP WORD PTR [EAX+0x13C],0x020D ; line 415
	0x0F, 0x84, 0x41, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 416
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0E, 0x02,       // CMP WORD PTR [EAX+0x13C],0x020E ; line 417
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 418
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0F, 0x02,       // CMP WORD PTR [EAX+0x13C],0x020F ; line 419
	0x0F, 0x84, 0x23, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 420
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x10, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0210 ; line 421
	0x0F, 0x84, 0x14, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 422
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x11, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0211 ; line 423
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE LABEL3 ; line 424
	0xE9, 0xAF, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 425
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [EAX+0x13E],0x0001 ; line 428
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                         // JNE LABELD ; line 429
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x11, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0211 ; line 430
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE LABEL0A ; line 431
	0xE9, 0x22, 0x00, 0x00, 0x00,                               // JMP LABEL0B ; line 432
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x0A, 0x00,       // CMP WORD PTR [EAX+0x13E],0x000A ; line 434
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                         // JE LABEL0C ; line 435
	0xE9, 0x3C, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 436
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x0E, 0x00,       // MOV WORD PTR [ESI+0x142],0x000E ; line 439
	0xE9, 0x2E, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 440
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x0C, 0x00,       // MOV WORD PTR [ESI+0x140],0x000C ; line 442
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x0E, 0x00,       // MOV WORD PTR [ESI+0x142],0x000E ; line 443
	0xE9, 0x17, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 444
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x19, 0x00,       // MOV WORD PTR [ESI+0x140],0x0019 ; line 446
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x0E, 0x00,       // MOV WORD PTR [ESI+0x142],0x000E ; line 447
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 448
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0B, 0x00,       // CMP WORD PTR [ESI+0x140],0x000B ; line 451
	0x0F, 0x84, 0x2D, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 452
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x18, 0x00,       // CMP WORD PTR [ESI+0x140],0x0018 ; line 453
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 454
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x25, 0x00,       // CMP WORD PTR [ESI+0x140],0x0025 ; line 455
	0x0F, 0x84, 0x0F, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 456
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 457
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x438C60 ; line 458
	0x84, 0xC0,                                                 // TEST AL,AL ; line 459
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x685A7A ; line 460
	0x81, 0x86, 0x4C, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // ADD DWORD PTR [ESI+0x34C],0xFFFFFFFF ; line 462
	0x5F,                                                       // POP EDI ; line 463
	0x5E,                                                       // POP ESI ; line 464
	0x5D,                                                       // POP EBP ; line 465
	0x5B,                                                       // POP EBX ; line 466
	0x89, 0xEC,                                                 // MOV ESP,EBP ; line 467
	0x5D,                                                       // POP EBP ; line 468
	0xC3,                                                       // RET ; line 469
};
static JumpTarget mokouA214REKKA_V2_jumpData[] = {
	{ 0x118, 0x438C60 },
	{ 0x120, 0x685A7A },
};
static PatchByteCode mokouA214REKKA_V2_patchByteCode = { mokouA214REKKA_V2_byteCode, 310, mokouA214REKKA_V2_jumpData, 2 };
// mokouA22RES1 declared in mokou.asm line 473
static unsigned char mokouA22RES1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 474
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x66F32C ; line 475
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 476
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66B834 ; line 477
};
static JumpTarget mokouA22RES1_jumpData[] = {
	{ 0x00B, 0x66F32C },
	{ 0x01A, 0x66B834 },
};
static PatchByteCode mokouA22RES1_patchByteCode = { mokouA22RES1_byteCode, 30, mokouA22RES1_jumpData, 2 };
// mokouA22RES2 declared in mokou.asm line 481
static unsigned char mokouA22RES2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 482
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                   // JE LABEL1 ; line 483
	0xE9, 0x47, 0x00, 0x00, 0x00,                         // JMP LABEL2 ; line 484
	0x0F, 0xB6, 0x8E, 0xAA, 0x06, 0x00, 0x00,             // MOVZX ECX, BYTE PTR [ESI+0x6AA] ; line 488
	0x69, 0xC9, 0x96, 0x00, 0x00, 0x00,                   // IMUL ECX,ECX,0x96 ; line 489
	0x81, 0xC1, 0x52, 0x03, 0x00, 0x00,                   // ADD ECX,0x352 ; line 490
	0x66, 0x03, 0x8E, 0x84, 0x01, 0x00, 0x00,             // ADD CX,[ESI+0x184] ; line 491
	0x66, 0x81, 0xF9, 0x10, 0x27,                         // CMP CX,0x2710 ; line 492
	0x0F, 0x86, 0x04, 0x00, 0x00, 0x00,                   // JBE LABEL3 ; line 493
	0x66, 0xB9, 0x10, 0x27,                               // MOV CX,0x2710 ; line 494
	0x66, 0x89, 0x8E, 0x84, 0x01, 0x00, 0x00,             // MOV [ESI+0x184],CX ; line 496
	0xD9, 0x05, 0xC0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9C0] ; line 498
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0x100] ; line 499
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00,                   // FLD DWORD PTR [0x86782C] ; line 500
	0xE9, 0x07, 0x00, 0x00, 0x00,                         // JMP LABELEND ; line 501
	0xD9, 0xEE,                                           // FLDZ ; line 504
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP LABELEND ; line 505
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66F33B ; line 508
};
static JumpTarget mokouA22RES2_jumpData[] = {
	{ 0x063, 0x66F33B },
};
static PatchByteCode mokouA22RES2_patchByteCode = { mokouA22RES2_byteCode, 103, mokouA22RES2_jumpData, 1 };
// mokouA22RESTRAJECTORY declared in mokou.asm line 512
static unsigned char mokouA22RESTRAJECTORY_byteCode[] = {
	0xD9, 0x05, 0x90, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B990] ; line 513
};
static PatchByteCode mokouA22RESTRAJECTORY_patchByteCode = { mokouA22RESTRAJECTORY_byteCode, 6, nullptr, 0 };
// mokouA22PARTICLE declared in mokou.asm line 518
static unsigned char mokouA22PARTICLE_byteCode[] = {
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                   // JE LABEL1 ; line 519
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x22, 0x02, // CMP WORD PTR [EAX+0x13C],0x0222 ; line 520
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                   // JE LABEL1 ; line 521
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6801B9 ; line 522
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [EAX+0x13E],0x00 ; line 524
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6801B9 ; line 525
};
static JumpTarget mokouA22PARTICLE_jumpData[] = {
	{ 0x017, 0x6801B9 },
	{ 0x026, 0x6801B9 },
};
static PatchByteCode mokouA22PARTICLE_patchByteCode = { mokouA22PARTICLE_byteCode, 42, mokouA22PARTICLE_jumpData, 2 };
// mokouA22PARTICLE2 declared in mokou.asm line 529
static unsigned char mokouA22PARTICLE2_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 530
	0xD8, 0x80, 0xEC, 0x00, 0x00, 0x00, // FADD DWORD PTR [EAX+0xEC] ; line 531
};
static PatchByteCode mokouA22PARTICLE2_patchByteCode = { mokouA22PARTICLE2_byteCode, 8, nullptr, 0 };
// mokouA22RESTRAJECTORY2 declared in mokou.asm line 535
static unsigned char mokouA22RESTRAJECTORY2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 536
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                   // JNE LABEL1 ; line 537
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 538
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                   // FSUB DWORD PTR [ESI+0x100] ; line 539
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 540
	0x66, 0x39, 0xBE, 0x3E, 0x01, 0x00, 0x00,             // CMP [ESI+0x13E],DI ; line 542
};
static PatchByteCode mokouA22RESTRAJECTORY2_patchByteCode = { mokouA22RESTRAJECTORY2_byteCode, 40, nullptr, 0 };
// mokouA22CJUMPTOB declared in mokou.asm line 546
static unsigned char mokouA22CJUMPTOB_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x66F160 ; line 547
};
static JumpTarget mokouA22CJUMPTOB_jumpData[] = {
	{ 0x001, 0x66F160 },
};
static PatchByteCode mokouA22CJUMPTOB_patchByteCode = { mokouA22CJUMPTOB_byteCode, 5, mokouA22CJUMPTOB_jumpData, 1 };
// mokouTORNADOSC declared in mokou.asm line 551
static unsigned char mokouTORNADOSC_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00000001 ; line 552
	0x0F, 0x85, 0x43, 0x00, 0x00, 0x00,                         // JNE LABEL0 ; line 553
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x2F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000002F ; line 554
	0x0F, 0x8C, 0x05, 0x00, 0x00, 0x00,                         // JL LABEL1 ; line 555
	0xE9, 0x17, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 556
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 559
	0xDC, 0x05, 0xA0, 0x8E, 0x85, 0x00,                         // FADD QWORD PTR [0x858EA0] ; line 560
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 561
	0xE9, 0x1F, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 562
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 565
	0xDC, 0x25, 0xA0, 0x8E, 0x85, 0x00,                         // FSUB QWORD PTR [0x858EA0] ; line 566
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 567
	0xE9, 0x08, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 568
	0xD9, 0xEE,                                                 // FLDZ ; line 571
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 572
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x13E],0x01 ; line 574
};
static PatchByteCode mokouTORNADOSC_patchByteCode = { mokouTORNADOSC_byteCode, 99, nullptr, 0 };
// mokouTORNADOSC2 declared in mokou.asm line 578
static unsigned char mokouTORNADOSC2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0C, 0x00, // CMP WORD PTR [ESI+0x140],0x0C ; line 579
};
static PatchByteCode mokouTORNADOSC2_patchByteCode = { mokouTORNADOSC2_byteCode, 9, nullptr, 0 };
// mokouD22GROUNDEDLEVEL declared in mokou.asm line 590
static unsigned char mokouD22GROUNDEDLEVEL_byteCode[] = {
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00,                         // CMP EAX,0x02 ; line 591
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 592
	0x81, 0xF8, 0x0B, 0x00, 0x00, 0x00,                         // CMP EAX,0x0B ; line 593
	0x0F, 0x87, 0xDE, 0xAD, 0xBE, 0xEF,                         // JA 0x67DCFB ; line 594
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x67DAB0 ; line 595
	0x80, 0xBE, 0x02, 0x08, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x802],0x00 ; line 598
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 599
	0x81, 0x86, 0xC8, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ADD DWORD PTR [ESI+0x4C8],0x01 ; line 600
	0xC6, 0x86, 0x02, 0x08, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x802],0x01 ; line 601
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 603
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x483CE0 ; line 604
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 605
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x489660 ; line 606
	0x55,                                                       // PUSH EBP ; line 607
	0x68, 0x1C, 0x02, 0x00, 0x00,                               // PUSH 0x21C ; line 608
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 609
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x487BA0 ; line 610
	0x5D,                                                       // POP EBP ; line 611
	0x5F,                                                       // POP EDI ; line 612
	0x5E,                                                       // POP ESI ; line 613
	0x5B,                                                       // POP EBX ; line 614
	0x81, 0xC4, 0x08, 0x00, 0x00, 0x00,                         // ADD ESP,0x08 ; line 615
	0xC3,                                                       // RET ; line 616
};
static JumpTarget mokouD22GROUNDEDLEVEL_jumpData[] = {
	{ 0x014, 0x67DCFB },
	{ 0x019, 0x67DAB0 },
	{ 0x03E, 0x483CE0 },
	{ 0x045, 0x489660 },
	{ 0x052, 0x487BA0 },
};
static PatchByteCode mokouD22GROUNDEDLEVEL_patchByteCode = { mokouD22GROUNDEDLEVEL_byteCode, 97, mokouD22GROUNDEDLEVEL_jumpData, 5 };
// mokouD22GROUNDED_B declared in mokou.asm line 620
static unsigned char mokouD22GROUNDED_B_byteCode[] = {
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,       // JGE 0x67E690 ; line 621
	0x80, 0xBE, 0xAA, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6AA],0x01 ; line 622
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,       // JGE 0x67F536 ; line 623
	0x68, 0x1C, 0x02, 0x00, 0x00,             // PUSH 0x0000021C ; line 624
	0x89, 0xF1,                               // MOV ECX,ESI ; line 625
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x489610 ; line 626
	0x66, 0x39, 0xC3,                         // CMP BX,AX ; line 627
	0x0F, 0x8E, 0x2A, 0x00, 0x00, 0x00,       // JLE LABEL1 ; line 628
	0x0F, 0xB7, 0x86, 0x3C, 0x01, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x13C] ; line 629
	0x66, 0x81, 0xF8, 0xF4, 0x01,             // CMP AX,0x1F4 ; line 630
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,       // JL 0x67F536 ; line 631
	0x66, 0x81, 0xF8, 0x57, 0x02,             // CMP AX,0x257 ; line 632
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,       // JG 0x67F536 ; line 633
	0x80, 0xBE, 0x02, 0x08, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x802],0x00 ; line 635
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,       // JNE 0x67F536 ; line 636
	0x80, 0xBE, 0x02, 0x08, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x802],0x00 ; line 638
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,       // JNE LABEL2 ; line 639
	0x01, 0xBE, 0xC8, 0x04, 0x00, 0x00,       // ADD [ESI+0x4C8],EDI ; line 640
	0xC6, 0x86, 0x02, 0x08, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x802],0x01 ; line 641
	0x89, 0xF1,                               // MOV ECX,ESI ; line 643
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x483CE0 ; line 644
	0x0F, 0xBF, 0xCB,                         // MOVSX ECX,BX ; line 645
	0x51,                                     // PUSH ECX ; line 646
	0x68, 0x1C, 0x02, 0x00, 0x00,             // PUSH 0x0000021C ; line 647
	0x89, 0xF1,                               // MOV ECX,ESI ; line 648
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487BA0 ; line 649
	0x5D,                                     // POP EBP ; line 650
	0x5F,                                     // POP EDI ; line 651
	0x5E,                                     // POP ESI ; line 652
	0x5B,                                     // POP EBX ; line 653
	0x81, 0xC4, 0x08, 0x00, 0x00, 0x00,       // ADD ESP,0x08 ; line 654
	0xC3,                                     // RET ; line 655
};
static JumpTarget mokouD22GROUNDED_B_jumpData[] = {
	{ 0x002, 0x67E690 },
	{ 0x00F, 0x67F536 },
	{ 0x01B, 0x489610 },
	{ 0x036, 0x67F536 },
	{ 0x041, 0x67F536 },
	{ 0x04E, 0x67F536 },
	{ 0x06F, 0x483CE0 },
	{ 0x07F, 0x487BA0 },
};
static PatchByteCode mokouD22GROUNDED_B_patchByteCode = { mokouD22GROUNDED_B_byteCode, 142, mokouD22GROUNDED_B_jumpData, 8 };
// mokouD22GROUNDED_C declared in mokou.asm line 659
static unsigned char mokouD22GROUNDED_C_byteCode[] = {
	0x80, 0xBE, 0xAE, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6AE],0x01 ; line 660
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,       // JGE 0x67E611 ; line 661
	0x80, 0xBE, 0xAA, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6AA],0x01 ; line 662
	0x0F, 0x8D, 0xDE, 0xAD, 0xBE, 0xEF,       // JGE 0x67E673 ; line 663
	0x68, 0x1C, 0x02, 0x00, 0x00,             // PUSH 0x0000021C ; line 664
	0x89, 0xF1,                               // MOV ECX,ESI ; line 665
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x489610 ; line 666
	0x66, 0x39, 0xC3,                         // CMP BX,AX ; line 667
	0x0F, 0x8E, 0x2A, 0x00, 0x00, 0x00,       // JLE LABEL1 ; line 668
	0x0F, 0xB7, 0x86, 0x3C, 0x01, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x13C] ; line 669
	0x66, 0x81, 0xF8, 0xF4, 0x01,             // CMP AX,0x1F4 ; line 670
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,       // JL 0x67E673 ; line 671
	0x66, 0x81, 0xF8, 0x57, 0x02,             // CMP AX,0x257 ; line 672
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,       // JG 0x67E673 ; line 673
	0x80, 0xBE, 0x02, 0x08, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x802],0x00 ; line 675
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,       // JNE 0x67E673 ; line 676
	0x80, 0xBE, 0x02, 0x08, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x802],0x00 ; line 678
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,       // JNE LABEL2 ; line 679
	0x01, 0xBE, 0xC8, 0x04, 0x00, 0x00,       // ADD [ESI+0x4C8],EDI ; line 680
	0xC6, 0x86, 0x02, 0x08, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x802],0x01 ; line 681
	0x89, 0xF1,                               // MOV ECX,ESI ; line 683
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x483CE0 ; line 684
	0x0F, 0xBF, 0xC3,                         // MOVSX EAX,BX ; line 685
	0x50,                                     // PUSH EAX ; line 686
	0x68, 0x1C, 0x02, 0x00, 0x00,             // PUSH 0x0000021C ; line 687
	0x89, 0xF1,                               // MOV ECX,ESI ; line 688
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x487BA0 ; line 689
	0x5D,                                     // POP EBP ; line 690
	0x5F,                                     // POP EDI ; line 691
	0x5E,                                     // POP ESI ; line 692
	0x5B,                                     // POP EBX ; line 693
	0x81, 0xC4, 0x08, 0x00, 0x00, 0x00,       // ADD ESP,0x08 ; line 694
	0xC3,                                     // RET ; line 695
};
static JumpTarget mokouD22GROUNDED_C_jumpData[] = {
	{ 0x009, 0x67E611 },
	{ 0x016, 0x67E673 },
	{ 0x022, 0x489610 },
	{ 0x03D, 0x67E673 },
	{ 0x048, 0x67E673 },
	{ 0x055, 0x67E673 },
	{ 0x076, 0x483CE0 },
	{ 0x086, 0x487BA0 },
};
static PatchByteCode mokouD22GROUNDED_C_patchByteCode = { mokouD22GROUNDED_C_byteCode, 149, mokouD22GROUNDED_C_jumpData, 8 };
// mokou6DINIT declared in mokou.asm line 699
static unsigned char mokou6DINIT_byteCode[] = {
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BA68] ; line 700
};
static PatchByteCode mokou6DINIT_patchByteCode = { mokou6DINIT_byteCode, 6, nullptr, 0 };
// mokouDRUMX declared in mokou.asm line 704
static unsigned char mokouDRUMX_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 705
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00, // FSUB DWORD PTR [0x8589F0] ; line 706
};
static PatchByteCode mokouDRUMX_patchByteCode = { mokouDRUMX_byteCode, 8, nullptr, 0 };
// mokouDRUMY declared in mokou.asm line 710
static unsigned char mokouDRUMY_byteCode[] = {
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00, // FLD DWORD PTR [0x8589F0] ; line 711
	0xD8, 0x05, 0xF0, 0x89, 0x85, 0x00, // FADD DWORD PTR [0x8589F0] ; line 712
};
static PatchByteCode mokouDRUMY_patchByteCode = { mokouDRUMY_byteCode, 12, nullptr, 0 };
// mokouJ8A declared in mokou.asm line 716
static unsigned char mokouJ8A_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x668B37 ; line 717
};
static JumpTarget mokouJ8A_jumpData[] = {
	{ 0x001, 0x668B37 },
};
static PatchByteCode mokouJ8A_patchByteCode = { mokouJ8A_byteCode, 5, mokouJ8A_jumpData, 1 };
// mokouONHIT_REMOVE declared in mokou.asm line 721
static unsigned char mokouONHIT_REMOVE_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 722
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 723
	0x6A, 0x02,                                                 // PUSH 0x02 ; line 724
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 725
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 726
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CE90 ; line 727
	0x84, 0xC0,                                                 // TEST AL,AL ; line 728
	0x61,                                                       // POPAD ; line 729
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x681E51 ; line 730
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 731
};
static JumpTarget mokouONHIT_REMOVE_jumpData[] = {
	{ 0x00A, 0x48CE90 },
	{ 0x013, 0x681E51 },
};
static PatchByteCode mokouONHIT_REMOVE_patchByteCode = { mokouONHIT_REMOVE_byteCode, 33, mokouONHIT_REMOVE_jumpData, 2 };
// mokouD236C declared in mokou.asm line 737
static unsigned char mokouD236C_byteCode[] = {
	0x68, 0xF5, 0x01, 0x00, 0x00, // PUSH 0x000001F5 ; line 738
};
static PatchByteCode mokouD236C_patchByteCode = { mokouD236C_byteCode, 5, nullptr, 0 };
// mokouD236C_JMP declared in mokou.asm line 743
static unsigned char mokouD236C_JMP_byteCode[] = {
	0x81, 0xF8, 0xF5, 0x01, 0x00, 0x00, // CMP EAX,0x000001F5 ; line 744
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x668189 ; line 745
	0x81, 0xF8, 0xF4, 0x01, 0x00, 0x00, // CMP EAX,0x000001F4 ; line 747
};
static JumpTarget mokouD236C_JMP_jumpData[] = {
	{ 0x008, 0x668189 },
};
static PatchByteCode mokouD236C_JMP_patchByteCode = { mokouD236C_JMP_byteCode, 18, mokouD236C_JMP_jumpData, 1 };
// mokouD236C_MOMENTUM declared in mokou.asm line 752
static unsigned char mokouD236C_MOMENTUM_byteCode[] = {
	0x81, 0xF8, 0xF5, 0x01, 0x00, 0x00, // CMP EAX,0x000001F5 ; line 753
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF, // JE 0x67C817 ; line 754
	0x81, 0xF8, 0xF4, 0x01, 0x00, 0x00, // CMP EAX,0x000001F4 ; line 755
};
static JumpTarget mokouD236C_MOMENTUM_jumpData[] = {
	{ 0x008, 0x67C817 },
};
static PatchByteCode mokouD236C_MOMENTUM_patchByteCode = { mokouD236C_MOMENTUM_byteCode, 18, mokouD236C_MOMENTUM_jumpData, 1 };
// mokouD236C_SPAWN declared in mokou.asm line 759
static unsigned char mokouD236C_SPAWN_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 760
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0xF5, 0x01, // CMP WORD PTR [ESI+0x13C],0x01F5 ; line 761
	0x0F, 0x84, 0x08, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 762
	0x6B, 0xD2, 0x1E,                                     // IMUL EDX,EDX,0x0000001E ; line 763
	0xE9, 0x03, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 764
	0x6B, 0xD2, 0xC0,                                     // IMUL EDX,EDX,0xFFFFFFC0 ; line 766
	0x66, 0x9D,                                           // POPF ; line 768
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66B940 ; line 769
};
static JumpTarget mokouD236C_SPAWN_jumpData[] = {
	{ 0x01F, 0x66B940 },
};
static PatchByteCode mokouD236C_SPAWN_patchByteCode = { mokouD236C_SPAWN_byteCode, 35, mokouD236C_SPAWN_jumpData, 1 };
// mokouWAKEUP declared in mokou.asm line 774
static unsigned char mokouWAKEUP_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 775
	0x60,                                                 // PUSHAD ; line 776
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x398] ; line 777
	0x66, 0x8B, 0x80, 0x3C, 0x01, 0x00, 0x00,             // MOV AX,[EAX+0x13C] ; line 778
	0x66, 0x81, 0xF8, 0xC5, 0x00,                         // CMP AX,0xC5 ; line 779
	0x0F, 0x8C, 0x31, 0x00, 0x00, 0x00,                   // JL SHORT LABEL1 ; line 780
	0x66, 0x81, 0xF8, 0xC7, 0x00,                         // CMP AX,0xC7 ; line 781
	0x0F, 0x8F, 0x26, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 782
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0F, 0x00, // CMP WORD PTR [ESI+0x140],0x0F ; line 783
	0x0F, 0x8F, 0x2E, 0x00, 0x00, 0x00,                   // JG SHORT LABEL3 ; line 784
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x10, 0x00, // MOV WORD PTR [ESI+0x140],0x10 ; line 785
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x10, 0x00, // MOV WORD PTR [ESI+0x142],0x10 ; line 786
	0xE9, 0x46, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 787
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x0F, 0x00, // CMP WORD PTR [ESI+0x140],0x0F ; line 789
	0x0F, 0x85, 0x37, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 790
	0x61,                                                 // POPAD ; line 791
	0x66, 0x9D,                                           // POPF ; line 792
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x680292 ; line 793
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x1F, 0x00, // CMP WORD PTR [ESI+0x140],0x1F ; line 796
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL4 ; line 797
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x398] ; line 798
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EAX+0xEC] ; line 799
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xEC] ; line 800
	0x61,                                                 // POPAD ; line 802
	0x66, 0x9D,                                           // POPF ; line 803
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0x11C] ; line 804
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6858AF ; line 805
	0x61,                                                 // POPAD ; line 808
	0x66, 0x9D,                                           // POPF ; line 809
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0x11C] ; line 810
};
static JumpTarget mokouWAKEUP_jumpData[] = {
	{ 0x05F, 0x680292 },
	{ 0x08E, 0x6858AF },
};
static PatchByteCode mokouWAKEUP_patchByteCode = { mokouWAKEUP_byteCode, 155, mokouWAKEUP_jumpData, 2 };
// mokouWAKEUP_TELE1 declared in mokou.asm line 814
static unsigned char mokouWAKEUP_TELE1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 815
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 816
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x0003 ; line 817
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x66599F ; line 818
	0x66, 0x39, 0xAE, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BP ; line 820
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6659CF ; line 821
};
static JumpTarget mokouWAKEUP_TELE1_jumpData[] = {
	{ 0x01A, 0x66599F },
	{ 0x026, 0x6659CF },
};
static PatchByteCode mokouWAKEUP_TELE1_patchByteCode = { mokouWAKEUP_TELE1_byteCode, 42, mokouWAKEUP_TELE1_jumpData, 2 };
// mokouWAKEUP_TELE2 declared in mokou.asm line 823
static unsigned char mokouWAKEUP_TELE2_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 824
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 825
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x0003 ; line 826
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x665B6E ; line 827
	0x66, 0x39, 0xAE, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BP ; line 829
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x665B9E ; line 830
};
static JumpTarget mokouWAKEUP_TELE2_jumpData[] = {
	{ 0x01A, 0x665B6E },
	{ 0x026, 0x665B9E },
};
static PatchByteCode mokouWAKEUP_TELE2_patchByteCode = { mokouWAKEUP_TELE2_byteCode, 42, mokouWAKEUP_TELE2_jumpData, 2 };
// mokouWAKEUP_TELE3 declared in mokou.asm line 832
static unsigned char mokouWAKEUP_TELE3_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 833
	0x0F, 0x85, 0x0F, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 834
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x0003 ; line 835
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x665CC5 ; line 836
	0x66, 0x39, 0xAE, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BP ; line 838
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x665CF5 ; line 839
};
static JumpTarget mokouWAKEUP_TELE3_jumpData[] = {
	{ 0x01A, 0x665CC5 },
	{ 0x026, 0x665CF5 },
};
static PatchByteCode mokouWAKEUP_TELE3_patchByteCode = { mokouWAKEUP_TELE3_byteCode, 42, mokouWAKEUP_TELE3_jumpData, 2 };
// mokouWAKEUP_TELE1A declared in mokou.asm line 845
static unsigned char mokouWAKEUP_TELE1A_byteCode[] = {
	0xD9, 0x05, 0xB4, 0x96, 0x85, 0x00, // FLD DWORD PTR [0x8596B4] ; line 846
};
static PatchByteCode mokouWAKEUP_TELE1A_patchByteCode = { mokouWAKEUP_TELE1A_byteCode, 6, nullptr, 0 };
// mokouWAKEUP_TELE1B declared in mokou.asm line 848
static unsigned char mokouWAKEUP_TELE1B_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 849
	0xD8, 0x25, 0xB4, 0x96, 0x85, 0x00, // FSUB DWORD PTR [0x8596B4] ; line 850
};
static PatchByteCode mokouWAKEUP_TELE1B_patchByteCode = { mokouWAKEUP_TELE1B_byteCode, 8, nullptr, 0 };
// mokouFLD075 declared in mokou.asm line 852
static unsigned char mokouFLD075_byteCode[] = {
	0xDD, 0x05, 0x20, 0x88, 0x85, 0x00, // FLD QWORD PTR [0x858820] ; line 853
};
static PatchByteCode mokouFLD075_patchByteCode = { mokouFLD075_byteCode, 6, nullptr, 0 };
// mokou2B declared in mokou.asm line 861
static unsigned char mokou2B_byteCode[] = {
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 862
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 863
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 864
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 865
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // MOV WORD PTR [ESI+0x7D2],0x03 ; line 867
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x668F6D ; line 868
};
static JumpTarget mokou2B_jumpData[] = {
	{ 0x00A, 0x47A9E0 },
	{ 0x018, 0x668F6D },
};
static PatchByteCode mokou2B_patchByteCode = { mokou2B_byteCode, 28, mokou2B_jumpData, 2 };
// mokou5B2B_OFFSET declared in mokou.asm line 872
static unsigned char mokou5B2B_OFFSET_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 873
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x92, 0x01, // CMP WORD [ESI+0x13C],0x192 ; line 874
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 875
	0xDC, 0x05, 0xF8, 0xE0, 0x85, 0x00,                   // FADD QWORD PTR [0x85E0F8] ; line 876
	0xE9, 0x06, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 877
	0xD8, 0x05, 0x54, 0xBB, 0x85, 0x00,                   // FADD DWORD PTR [0x85BB54] ; line 879
	0x66, 0x9D,                                           // POPF ; line 881
};
static PatchByteCode mokou5B2B_OFFSET_patchByteCode = { mokou5B2B_OFFSET_byteCode, 36, nullptr, 0 };
// mokou5B2B_ANG1 declared in mokou.asm line 897
static unsigned char mokou5B2B_ANG1_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 898
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x92, 0x01, // CMP WORD [ESI+0x13C],0x192 ; line 899
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 900
	0xD9, 0x05, 0xE0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9E0] ; line 901
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 904
	0xD9, 0xEE,                                           // FLDZ ; line 906
	0x66, 0x9D,                                           // POPF ; line 908
};
static PatchByteCode mokou5B2B_ANG1_patchByteCode = { mokou5B2B_ANG1_byteCode, 32, nullptr, 0 };
// mokou5B2B_ANG2 declared in mokou.asm line 910
static unsigned char mokou5B2B_ANG2_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 911
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x92, 0x01, // CMP WORD [ESI+0x13C],0x192 ; line 912
	0x0F, 0x84, 0x0B, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 913
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00,                   // FLD DWORD PTR [0x85AE94] ; line 914
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 916
	0xD9, 0xEE,                                           // FLDZ ; line 918
	0x66, 0x9D,                                           // POPF ; line 920
};
static PatchByteCode mokou5B2B_ANG2_patchByteCode = { mokou5B2B_ANG2_byteCode, 32, nullptr, 0 };
// mokouFLD30 declared in mokou.asm line 932
static unsigned char mokouFLD30_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBE4] ; line 933
};
static PatchByteCode mokouFLD30_patchByteCode = { mokouFLD30_byteCode, 6, nullptr, 0 };
// mokouFLDN30 declared in mokou.asm line 935
static unsigned char mokouFLDN30_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 936
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 937
};
static PatchByteCode mokouFLDN30_patchByteCode = { mokouFLDN30_byteCode, 8, nullptr, 0 };
// mokouFLD60 declared in mokou.asm line 940
static unsigned char mokouFLD60_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BBE4] ; line 941
	0xD8, 0x05, 0xE4, 0xBB, 0x85, 0x00, // FADD DWORD PTR [0x85BBE4] ; line 942
};
static PatchByteCode mokouFLD60_patchByteCode = { mokouFLD60_byteCode, 12, nullptr, 0 };
// mokouFLDN60 declared in mokou.asm line 944
static unsigned char mokouFLDN60_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 945
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 946
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 947
};
static PatchByteCode mokouFLDN60_patchByteCode = { mokouFLDN60_byteCode, 14, nullptr, 0 };
// mokouFLDN90 declared in mokou.asm line 956
static unsigned char mokouFLDN90_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 957
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 958
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 959
	0xD8, 0x25, 0xE4, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BBE4] ; line 960
};
static PatchByteCode mokouFLDN90_patchByteCode = { mokouFLDN90_byteCode, 20, nullptr, 0 };
// mokouJ5C_OFFSET declared in mokou.asm line 998
static unsigned char mokouJ5C_OFFSET_byteCode[] = {
	0xD8, 0x05, 0x6C, 0xBB, 0x85, 0x00, // FADD DWORD PTR [0x85BB6C] ; line 999
};
static PatchByteCode mokouJ5C_OFFSET_patchByteCode = { mokouJ5C_OFFSET_byteCode, 6, nullptr, 0 };
// mokou5C_NEWSYS declared in mokou.asm line 1003
static unsigned char mokou5C_NEWSYS_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 1004
	0x6A, 0x03,                                                 // PUSH 0x03 ; line 1005
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1006
	0x6A, 0x00,                                                 // PUSH 0x00 ; line 1007
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1008
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CE90 ; line 1009
	0x84, 0xC0,                                                 // TEST AL,AL ; line 1010
	0x61,                                                       // POPAD ; line 1011
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x681E51 ; line 1012
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x05 ; line 1013
	0x66, 0x9C,                                                 // PUSHF ; line 1014
	0x60,                                                       // PUSHAD ; line 1015
	0x66, 0x8B, 0x86, 0x3E, 0x01, 0x00, 0x00,                   // MOV AX,[ESI+0x13E] ; line 1016
	0x66, 0x81, 0xF8, 0x00, 0x00,                               // CMP AX,0x00 ; line 1017
	0x0F, 0x85, 0x01, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1018
	0x90,                                                       // NOP ; line 1019
	0x61,                                                       // POPAD ; line 1021
	0x66, 0x9D,                                                 // POPF ; line 1022
};
static JumpTarget mokou5C_NEWSYS_jumpData[] = {
	{ 0x00A, 0x48CE90 },
	{ 0x013, 0x681E51 },
};
static PatchByteCode mokou5C_NEWSYS_patchByteCode = { mokou5C_NEWSYS_byteCode, 58, mokou5C_NEWSYS_jumpData, 2 };
// mokou5C_SPD declared in mokou.asm line 1026
static unsigned char mokou5C_SPD_byteCode[] = {
	0x66, 0x9C,                               // PUSHF ; line 1027
	0x60,                                     // PUSHAD ; line 1028
	0xDD, 0x05, 0x88, 0xBB, 0x85, 0x00,       // FLD QWORD PTR [0x85BB88] ; line 1029
	0x66, 0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x144] ; line 1031
	0x0F, 0xB7, 0xC0,                         // MOVZX EAX,AX ; line 1032
	0x50,                                     // PUSH EAX ; line 1033
	0xDB, 0x04, 0x24,                         // FILD DWORD PTR [ESP] ; line 1034
	0x58,                                     // POP EAX ; line 1035
	0xDE, 0xE9,                               // FSUBP ; line 1036
	0x61,                                     // POPAD ; line 1037
	0x66, 0x9D,                               // POPF ; line 1038
};
static PatchByteCode mokou5C_SPD_patchByteCode = { mokou5C_SPD_byteCode, 29, nullptr, 0 };
// mokouHASTE declared in mokou.asm line 1043
static unsigned char mokouHASTE_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1044
	0x60,                                                       // PUSHAD ; line 1045
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x08 ; line 1046
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1047
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00 ; line 1048
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1049
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 1051
	0x8A, 0x81, 0x04, 0x01, 0x00, 0x00,                         // MOV AL,[ECX+0x104] ; line 1052
	0x88, 0x86, 0x04, 0x01, 0x00, 0x00,                         // MOV [ESI+0x104],AL ; line 1053
	0x61,                                                       // POPAD ; line 1054
	0x66, 0x9D,                                                 // POPF ; line 1055
	0x0F, 0xBF, 0x86, 0x3E, 0x01, 0x00, 0x00,                   // MOVSX EAX,WORD PTR [ESI+0x13E] ; line 1056
};
static PatchByteCode mokouHASTE_patchByteCode = { mokouHASTE_byteCode, 64, nullptr, 0 };
// mokouHASTE_DUR declared in mokou.asm line 1060
static unsigned char mokouHASTE_DUR_byteCode[] = {
	0x66, 0xC7, 0x86, 0x92, 0x08, 0x00, 0x00, 0xB4, 0x00, // MOV WORD PTR [ESI+0x892],0xB4 ; line 1061
};
static PatchByteCode mokouHASTE_DUR_patchByteCode = { mokouHASTE_DUR_byteCode, 9, nullptr, 0 };
// mokou5B_NEWA declared in mokou.asm line 1066
static unsigned char mokou5B_NEWA_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x6690AC ; line 1067
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1068
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1069
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1070
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1071
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D2],0x02 ; line 1073
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x668F6D ; line 1074
};
static JumpTarget mokou5B_NEWA_jumpData[] = {
	{ 0x002, 0x6690AC },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x668F6D },
};
static PatchByteCode mokou5B_NEWA_patchByteCode = { mokou5B_NEWA_byteCode, 34, mokou5B_NEWA_jumpData, 3 };
// mokou5B_NEWA_CHARGE declared in mokou.asm line 1076
static unsigned char mokou5B_NEWA_CHARGE_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66B834 ; line 1077
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1078
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1079
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1080
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1081
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // MOV WORD PTR [ESI+0x7D2],0x04 ; line 1083
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6690D5 ; line 1084
};
static JumpTarget mokou5B_NEWA_CHARGE_jumpData[] = {
	{ 0x002, 0x66B834 },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x6690D5 },
};
static PatchByteCode mokou5B_NEWA_CHARGE_patchByteCode = { mokou5B_NEWA_CHARGE_byteCode, 34, mokou5B_NEWA_CHARGE_jumpData, 3 };
// mokou5B_NEWB declared in mokou.asm line 1086
static unsigned char mokou5B_NEWB_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66955B ; line 1087
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1088
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1089
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1090
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1091
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D2],0x02 ; line 1093
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66942F ; line 1094
};
static JumpTarget mokou5B_NEWB_jumpData[] = {
	{ 0x002, 0x66955B },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x66942F },
};
static PatchByteCode mokou5B_NEWB_patchByteCode = { mokou5B_NEWB_byteCode, 34, mokou5B_NEWB_jumpData, 3 };
// mokou5B_NEWB_CHARGE declared in mokou.asm line 1096
static unsigned char mokou5B_NEWB_CHARGE_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66B834 ; line 1097
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1098
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1099
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1100
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1101
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // MOV WORD PTR [ESI+0x7D2],0x04 ; line 1103
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669584 ; line 1104
};
static JumpTarget mokou5B_NEWB_CHARGE_jumpData[] = {
	{ 0x002, 0x66B834 },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x669584 },
};
static PatchByteCode mokou5B_NEWB_CHARGE_patchByteCode = { mokou5B_NEWB_CHARGE_byteCode, 34, mokou5B_NEWB_CHARGE_jumpData, 3 };
// mokou5B_NEWC declared in mokou.asm line 1106
static unsigned char mokou5B_NEWC_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x669B78 ; line 1107
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1108
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1109
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1110
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1111
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D2],0x02 ; line 1113
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669A39 ; line 1114
};
static JumpTarget mokou5B_NEWC_jumpData[] = {
	{ 0x002, 0x669B78 },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x669A39 },
};
static PatchByteCode mokou5B_NEWC_patchByteCode = { mokou5B_NEWC_byteCode, 34, mokou5B_NEWC_jumpData, 3 };
// mokou5B_NEWC_CHARGE declared in mokou.asm line 1116
static unsigned char mokou5B_NEWC_CHARGE_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66B834 ; line 1117
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1118
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1119
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1120
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1121
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // MOV WORD PTR [ESI+0x7D2],0x04 ; line 1123
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669BA1 ; line 1124
};
static JumpTarget mokou5B_NEWC_CHARGE_jumpData[] = {
	{ 0x002, 0x66B834 },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x669BA1 },
};
static PatchByteCode mokou5B_NEWC_CHARGE_patchByteCode = { mokou5B_NEWC_CHARGE_byteCode, 34, mokou5B_NEWC_CHARGE_jumpData, 3 };
// mokou5B_NEWD declared in mokou.asm line 1126
static unsigned char mokou5B_NEWD_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66A09C ; line 1127
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1128
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1129
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1130
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1131
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D2],0x02 ; line 1133
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669F5A ; line 1134
};
static JumpTarget mokou5B_NEWD_jumpData[] = {
	{ 0x002, 0x66A09C },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x669F5A },
};
static PatchByteCode mokou5B_NEWD_patchByteCode = { mokou5B_NEWD_byteCode, 34, mokou5B_NEWD_jumpData, 3 };
// mokou5B_NEWD_CHARGE declared in mokou.asm line 1136
static unsigned char mokou5B_NEWD_CHARGE_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x66B834 ; line 1137
	0x6A, 0x2D,                                           // PUSH 0x2D ; line 1138
	0x68, 0xC8, 0x00, 0x00, 0x00,                         // PUSH 0x000000C8 ; line 1139
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 1140
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x47A9E0 ; line 1141
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // MOV WORD PTR [ESI+0x7D2],0x04 ; line 1143
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66A0CA ; line 1144
};
static JumpTarget mokou5B_NEWD_CHARGE_jumpData[] = {
	{ 0x002, 0x66B834 },
	{ 0x010, 0x47A9E0 },
	{ 0x01E, 0x66A0CA },
};
static PatchByteCode mokou5B_NEWD_CHARGE_patchByteCode = { mokou5B_NEWD_CHARGE_byteCode, 34, mokou5B_NEWD_CHARGE_jumpData, 3 };
// mokou5B_METER declared in mokou.asm line 1162
static unsigned char mokou5B_METER_byteCode[] = {
	0x6A, 0x04,                   // PUSH 0x04 ; line 1163
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1164
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 1165
};
static JumpTarget mokou5B_METER_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode mokou5B_METER_patchByteCode = { mokou5B_METER_byteCode, 9, mokou5B_METER_jumpData, 1 };
// mokou6B_METER declared in mokou.asm line 1169
static unsigned char mokou6B_METER_byteCode[] = {
	0x6A, 0x04,                                                 // PUSH 0x04 ; line 1170
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1171
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 1172
};
static PatchByteCode mokou6B_METER_patchByteCode = { mokou6B_METER_byteCode, 14, nullptr, 0 };
// mokou2B_METER declared in mokou.asm line 1176
static unsigned char mokou2B_METER_byteCode[] = {
	0x6A, 0x04,                   // PUSH 0x04 ; line 1177
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1178
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 1179
};
static JumpTarget mokou2B_METER_jumpData[] = {
	{ 0x005, 0x487870 },
};
static PatchByteCode mokou2B_METER_patchByteCode = { mokou2B_METER_byteCode, 9, mokou2B_METER_jumpData, 1 };
// mokouJ5B_METER declared in mokou.asm line 1182
static unsigned char mokouJ5B_METER_byteCode[] = {
	0x6A, 0x04,                                                 // PUSH 0x04 ; line 1183
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1184
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x01 ; line 1185
};
static PatchByteCode mokouJ5B_METER_patchByteCode = { mokouJ5B_METER_byteCode, 14, nullptr, 0 };
// mokouJ2B_METER declared in mokou.asm line 1189
static unsigned char mokouJ2B_METER_byteCode[] = {
	0x6A, 0x04,                         // PUSH 0x04 ; line 1190
	0x89, 0xF1,                         // MOV ECX,ESI ; line 1191
	0x89, 0xBE, 0x90, 0x01, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],EDI ; line 1192
};
static PatchByteCode mokouJ2B_METER_patchByteCode = { mokouJ2B_METER_byteCode, 10, nullptr, 0 };
// mokou5B_NEW1A declared in mokou.asm line 1197
static unsigned char mokou5B_NEW1A_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1198
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1199
	0x66, 0x9C,                                           // PUSHF ; line 1200
	0x60,                                                 // PUSHAD ; line 1201
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1202
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1203
	0x61,                                                 // POPAD ; line 1204
	0x66, 0x9D,                                           // POPF ; line 1205
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6690A3 ; line 1206
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1208
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1209
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1210
	0x61,                                                 // POPAD ; line 1211
	0x66, 0x9D,                                           // POPF ; line 1212
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x668F6D ; line 1213
};
static JumpTarget mokou5B_NEW1A_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x6690A3 },
	{ 0x03B, 0x668F6D },
};
static PatchByteCode mokou5B_NEW1A_patchByteCode = { mokou5B_NEW1A_byteCode, 63, mokou5B_NEW1A_jumpData, 3 };
// mokou5B_NEW1A_CHARGE declared in mokou.asm line 1215
static unsigned char mokou5B_NEW1A_CHARGE_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1216
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1217
	0x66, 0x9C,                                           // PUSHF ; line 1218
	0x60,                                                 // PUSHAD ; line 1219
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1220
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1221
	0x61,                                                 // POPAD ; line 1222
	0x66, 0x9D,                                           // POPF ; line 1223
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669312 ; line 1224
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1226
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1227
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1228
	0x61,                                                 // POPAD ; line 1229
	0x66, 0x9D,                                           // POPF ; line 1230
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6690D5 ; line 1231
};
static JumpTarget mokou5B_NEW1A_CHARGE_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669312 },
	{ 0x03B, 0x6690D5 },
};
static PatchByteCode mokou5B_NEW1A_CHARGE_patchByteCode = { mokou5B_NEW1A_CHARGE_byteCode, 63, mokou5B_NEW1A_CHARGE_jumpData, 3 };
// mokou5B_NEW1B declared in mokou.asm line 1233
static unsigned char mokou5B_NEW1B_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1234
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1235
	0x66, 0x9C,                                           // PUSHF ; line 1236
	0x60,                                                 // PUSHAD ; line 1237
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1238
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1239
	0x61,                                                 // POPAD ; line 1240
	0x66, 0x9D,                                           // POPF ; line 1241
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669552 ; line 1242
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1244
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1245
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1246
	0x61,                                                 // POPAD ; line 1247
	0x66, 0x9D,                                           // POPF ; line 1248
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66942F ; line 1249
};
static JumpTarget mokou5B_NEW1B_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669552 },
	{ 0x03B, 0x66942F },
};
static PatchByteCode mokou5B_NEW1B_patchByteCode = { mokou5B_NEW1B_byteCode, 63, mokou5B_NEW1B_jumpData, 3 };
// mokou5B_NEW1B_CHARGE declared in mokou.asm line 1251
static unsigned char mokou5B_NEW1B_CHARGE_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1252
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1253
	0x66, 0x9C,                                           // PUSHF ; line 1254
	0x60,                                                 // PUSHAD ; line 1255
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1256
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1257
	0x61,                                                 // POPAD ; line 1258
	0x66, 0x9D,                                           // POPF ; line 1259
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669312 ; line 1260
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1262
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1263
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1264
	0x61,                                                 // POPAD ; line 1265
	0x66, 0x9D,                                           // POPF ; line 1266
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669584 ; line 1267
};
static JumpTarget mokou5B_NEW1B_CHARGE_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669312 },
	{ 0x03B, 0x669584 },
};
static PatchByteCode mokou5B_NEW1B_CHARGE_patchByteCode = { mokou5B_NEW1B_CHARGE_byteCode, 63, mokou5B_NEW1B_CHARGE_jumpData, 3 };
// mokou5B_NEW1C declared in mokou.asm line 1269
static unsigned char mokou5B_NEW1C_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1270
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1271
	0x66, 0x9C,                                           // PUSHF ; line 1272
	0x60,                                                 // PUSHAD ; line 1273
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1274
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1275
	0x61,                                                 // POPAD ; line 1276
	0x66, 0x9D,                                           // POPF ; line 1277
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669B6F ; line 1278
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1280
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1281
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1282
	0x61,                                                 // POPAD ; line 1283
	0x66, 0x9D,                                           // POPF ; line 1284
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669A39 ; line 1285
};
static JumpTarget mokou5B_NEW1C_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669B6F },
	{ 0x03B, 0x669A39 },
};
static PatchByteCode mokou5B_NEW1C_patchByteCode = { mokou5B_NEW1C_byteCode, 63, mokou5B_NEW1C_jumpData, 3 };
// mokou5B_NEW1C_CHARGE declared in mokou.asm line 1287
static unsigned char mokou5B_NEW1C_CHARGE_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1288
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1289
	0x66, 0x9C,                                           // PUSHF ; line 1290
	0x60,                                                 // PUSHAD ; line 1291
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1292
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1293
	0x61,                                                 // POPAD ; line 1294
	0x66, 0x9D,                                           // POPF ; line 1295
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669312 ; line 1296
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1298
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1299
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1300
	0x61,                                                 // POPAD ; line 1301
	0x66, 0x9D,                                           // POPF ; line 1302
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669BA1 ; line 1303
};
static JumpTarget mokou5B_NEW1C_CHARGE_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669312 },
	{ 0x03B, 0x669BA1 },
};
static PatchByteCode mokou5B_NEW1C_CHARGE_patchByteCode = { mokou5B_NEW1C_CHARGE_byteCode, 63, mokou5B_NEW1C_CHARGE_jumpData, 3 };
// mokou5B_NEW1D declared in mokou.asm line 1305
static unsigned char mokou5B_NEW1D_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1306
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1307
	0x66, 0x9C,                                           // PUSHF ; line 1308
	0x60,                                                 // PUSHAD ; line 1309
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1310
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1311
	0x61,                                                 // POPAD ; line 1312
	0x66, 0x9D,                                           // POPF ; line 1313
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66A091 ; line 1314
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1316
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1317
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1318
	0x61,                                                 // POPAD ; line 1319
	0x66, 0x9D,                                           // POPF ; line 1320
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669F5A ; line 1321
};
static JumpTarget mokou5B_NEW1D_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x66A091 },
	{ 0x03B, 0x669F5A },
};
static PatchByteCode mokou5B_NEW1D_patchByteCode = { mokou5B_NEW1D_byteCode, 63, mokou5B_NEW1D_jumpData, 3 };
// mokou5B_NEW1D_CHARGE declared in mokou.asm line 1323
static unsigned char mokou5B_NEW1D_CHARGE_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 1324
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 1325
	0x66, 0x9C,                                           // PUSHF ; line 1326
	0x60,                                                 // PUSHAD ; line 1327
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 1328
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 1329
	0x61,                                                 // POPAD ; line 1330
	0x66, 0x9D,                                           // POPF ; line 1331
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x669312 ; line 1332
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 1334
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 1335
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 1336
	0x61,                                                 // POPAD ; line 1337
	0x66, 0x9D,                                           // POPF ; line 1338
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x66A0CA ; line 1339
};
static JumpTarget mokou5B_NEW1D_CHARGE_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x669312 },
	{ 0x03B, 0x66A0CA },
};
static PatchByteCode mokou5B_NEW1D_CHARGE_patchByteCode = { mokou5B_NEW1D_CHARGE_byteCode, 63, mokou5B_NEW1D_CHARGE_jumpData, 3 };
// mokou5B_NEW2A declared in mokou.asm line 1358
static unsigned char mokou5B_NEW2A_byteCode[] = {
	0xD9, 0x9C, 0x24, 0xE8, 0x03, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x3E8] ; line 1359
	0x66, 0x9C,                                           // PUSHF ; line 1360
	0x60,                                                 // PUSHAD ; line 1361
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1362
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1363
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1364
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1365
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1366
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1367
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1368
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1369
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1370
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1373
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1374
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1377
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1378
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1381
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1382
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1385
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1386
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1389
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1390
	0x61,                                                 // POPAD ; line 1392
	0x66, 0x9D,                                           // POPF ; line 1393
	0xD9, 0x9C, 0x24, 0xE8, 0x03, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x3E8] ; line 1394
};
static PatchByteCode mokou5B_NEW2A_patchByteCode = { mokou5B_NEW2A_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2A_CHARGE declared in mokou.asm line 1396
static unsigned char mokou5B_NEW2A_CHARGE_byteCode[] = {
	0xD9, 0x9C, 0x24, 0x9C, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x9C] ; line 1397
	0x66, 0x9C,                                           // PUSHF ; line 1398
	0x60,                                                 // PUSHAD ; line 1399
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1400
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1401
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1402
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1403
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1404
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1405
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1406
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1407
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1408
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1411
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1412
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1415
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1416
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1419
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1420
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1423
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1424
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1427
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1428
	0x61,                                                 // POPAD ; line 1430
	0x66, 0x9D,                                           // POPF ; line 1431
	0xD9, 0x9C, 0x24, 0x9C, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x9C] ; line 1432
};
static PatchByteCode mokou5B_NEW2A_CHARGE_patchByteCode = { mokou5B_NEW2A_CHARGE_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2B declared in mokou.asm line 1434
static unsigned char mokou5B_NEW2B_byteCode[] = {
	0xD9, 0x9C, 0x24, 0x8C, 0x04, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x48C] ; line 1435
	0x66, 0x9C,                                           // PUSHF ; line 1436
	0x60,                                                 // PUSHAD ; line 1437
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1438
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1439
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1440
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1441
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1442
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1443
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1444
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1445
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1446
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1449
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1450
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1453
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1454
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1457
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1458
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1461
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1462
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1465
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1466
	0x61,                                                 // POPAD ; line 1468
	0x66, 0x9D,                                           // POPF ; line 1469
	0xD9, 0x9C, 0x24, 0x8C, 0x04, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x48C] ; line 1470
};
static PatchByteCode mokou5B_NEW2B_patchByteCode = { mokou5B_NEW2B_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2B_CHARGE declared in mokou.asm line 1472
static unsigned char mokou5B_NEW2B_CHARGE_byteCode[] = {
	0xD9, 0x9C, 0x24, 0xB4, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0xB4] ; line 1473
	0x66, 0x9C,                                           // PUSHF ; line 1474
	0x60,                                                 // PUSHAD ; line 1475
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1476
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1477
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1478
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1479
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1480
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1481
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1482
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1483
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1484
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1487
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1488
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1491
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1492
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1495
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1496
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1499
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1500
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1503
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1504
	0x61,                                                 // POPAD ; line 1506
	0x66, 0x9D,                                           // POPF ; line 1507
	0xD9, 0x9C, 0x24, 0xB4, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0xB4] ; line 1508
};
static PatchByteCode mokou5B_NEW2B_CHARGE_patchByteCode = { mokou5B_NEW2B_CHARGE_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2C declared in mokou.asm line 1510
static unsigned char mokou5B_NEW2C_byteCode[] = {
	0xD9, 0x9C, 0x24, 0xB8, 0x03, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x3B8] ; line 1511
	0x66, 0x9C,                                           // PUSHF ; line 1512
	0x60,                                                 // PUSHAD ; line 1513
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1514
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1515
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1516
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1517
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1518
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1519
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1520
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1521
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1522
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1525
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1526
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1529
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1530
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1533
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1534
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1537
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1538
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1541
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1542
	0x61,                                                 // POPAD ; line 1544
	0x66, 0x9D,                                           // POPF ; line 1545
	0xD9, 0x9C, 0x24, 0xB8, 0x03, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x3B8] ; line 1546
};
static PatchByteCode mokou5B_NEW2C_patchByteCode = { mokou5B_NEW2C_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2C_CHARGE declared in mokou.asm line 1548
static unsigned char mokou5B_NEW2C_CHARGE_byteCode[] = {
	0xD9, 0x5C, 0x24, 0x78,                               // FSTP DWORD PTR [ESP+0x78] ; line 1549
	0x66, 0x9C,                                           // PUSHF ; line 1550
	0x60,                                                 // PUSHAD ; line 1551
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1552
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1553
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1554
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1555
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1556
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1557
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1558
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1559
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1560
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1563
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1564
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1567
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1568
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1571
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1572
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1575
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1576
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1579
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1580
	0x61,                                                 // POPAD ; line 1582
	0x66, 0x9D,                                           // POPF ; line 1583
	0xD9, 0x5C, 0x24, 0x78,                               // FSTP DWORD PTR [ESP+0x78] ; line 1584
	0x52,                                                 // PUSH EDX ; line 1585
};
static PatchByteCode mokou5B_NEW2C_CHARGE_patchByteCode = { mokou5B_NEW2C_CHARGE_byteCode, 135, nullptr, 0 };
// mokou5B_NEW2D declared in mokou.asm line 1587
static unsigned char mokou5B_NEW2D_byteCode[] = {
	0xD9, 0x9C, 0x24, 0xE0, 0x02, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x2E0] ; line 1588
	0x66, 0x9C,                                           // PUSHF ; line 1589
	0x60,                                                 // PUSHAD ; line 1590
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1591
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1592
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1593
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1594
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1595
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1596
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1597
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1598
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1599
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1602
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1603
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1606
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1607
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1610
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1611
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1614
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1615
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1618
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1619
	0x61,                                                 // POPAD ; line 1621
	0x66, 0x9D,                                           // POPF ; line 1622
	0xD9, 0x9C, 0x24, 0xE0, 0x02, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x2E0] ; line 1623
};
static PatchByteCode mokou5B_NEW2D_patchByteCode = { mokou5B_NEW2D_byteCode, 140, nullptr, 0 };
// mokou5B_NEW2D_CHARGE declared in mokou.asm line 1625
static unsigned char mokou5B_NEW2D_CHARGE_byteCode[] = {
	0xD9, 0x9C, 0x24, 0x84, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x84] ; line 1626
	0x66, 0x9C,                                           // PUSHF ; line 1627
	0x60,                                                 // PUSHAD ; line 1628
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x7D2],0x04 ; line 1629
	0x0F, 0x84, 0x32, 0x00, 0x00, 0x00,                   // JE SHORT LABEL0 ; line 1630
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x7D2],0x03 ; line 1631
	0x0F, 0x84, 0x2E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 1632
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 1633
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1634
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 1635
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 1636
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL4 ; line 1637
	0xD9, 0x05, 0x24, 0xD1, 0x85, 0x00,                   // FLD DWORD PTR [0x85D124] ; line 1640
	0xE9, 0x2C, 0x00, 0x00, 0x00,                         // JMP END ; line 1641
	0xD9, 0x05, 0x28, 0x88, 0x85, 0x00,                   // FLD DWORD PTR [0x858828] ; line 1644
	0xE9, 0x21, 0x00, 0x00, 0x00,                         // JMP END ; line 1645
	0xD9, 0x05, 0xD0, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D0] ; line 1648
	0xE9, 0x16, 0x00, 0x00, 0x00,                         // JMP END ; line 1649
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B9D4] ; line 1652
	0xE9, 0x0B, 0x00, 0x00, 0x00,                         // JMP END ; line 1653
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00,                   // FLD DWORD PTR [0x85C610] ; line 1656
	0xE9, 0x00, 0x00, 0x00, 0x00,                         // JMP END ; line 1657
	0x61,                                                 // POPAD ; line 1659
	0x66, 0x9D,                                           // POPF ; line 1660
	0xD9, 0x9C, 0x24, 0x84, 0x00, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x84] ; line 1661
};
static PatchByteCode mokou5B_NEW2D_CHARGE_patchByteCode = { mokou5B_NEW2D_CHARGE_byteCode, 140, nullptr, 0 };
// mokou5SC_NEWVOLC_FOLLOW declared in mokou.asm line 1711
static unsigned char mokou5SC_NEWVOLC_FOLLOW_byteCode[] = {
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,       // MOV EAX,[ESI+0x398] ; line 1712
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [EAX+0xEC] ; line 1713
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xEC] ; line 1714
	0x0F, 0xBF, 0x86, 0x3E, 0x01, 0x00, 0x00, // MOVSX EAX,WORD PTR [ESI+0x13E] ; line 1715
};
static PatchByteCode mokou5SC_NEWVOLC_FOLLOW_patchByteCode = { mokou5SC_NEWVOLC_FOLLOW_byteCode, 25, nullptr, 0 };
// mokou5SC_NEWVOLC_SUBSEQ declared in mokou.asm line 1719
static unsigned char mokou5SC_NEWVOLC_SUBSEQ_byteCode[] = {
	0x66, 0x81, 0xF9, 0xB4, 0x00,                         // CMP CX,0x00B4 ; line 1720
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                   // JG 0x673B1F ; line 1721
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 1722
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x673B1F ; line 1723
};
static JumpTarget mokou5SC_NEWVOLC_SUBSEQ_jumpData[] = {
	{ 0x007, 0x673B1F },
	{ 0x016, 0x673B1F },
};
static PatchByteCode mokou5SC_NEWVOLC_SUBSEQ_patchByteCode = { mokou5SC_NEWVOLC_SUBSEQ_byteCode, 26, mokou5SC_NEWVOLC_SUBSEQ_jumpData, 2 };
// mokou5SC_NEWVOLC_BULLET declared in mokou.asm line 1727
static unsigned char mokou5SC_NEWVOLC_BULLET_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00,       // CMP WORD PTR [ESI+0x13E],0x02 ; line 1728
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                         // JE 0x68025F ; line 1729
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1F ; line 1730
	0x0F, 0x8D, 0x29, 0x00, 0x00, 0x00,                         // JGE LABELEND ; line 1731
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 1732
	0xD9, 0x81, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ECX+0xEC] ; line 1733
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xEC] ; line 1734
	0xD9, 0xE8,                                                 // FLD1 ; line 1735
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF0] ; line 1736
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1737
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x438C60 ; line 1738
	0x84, 0xC0,                                                 // TEST AL,AL ; line 1739
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,                         // JE LABEL1 ; line 1740
	0x81, 0x86, 0x4C, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // ADD DWORD PTR [ESI+0x34C],0xFFFFFFFF ; line 1742
	0x5F,                                                       // POP EDI ; line 1744
	0x5E,                                                       // POP ESI ; line 1745
	0x5D,                                                       // POP EBP ; line 1746
	0x5B,                                                       // POP EBX ; line 1747
	0x89, 0xEC,                                                 // MOV ESP,EBP ; line 1748
	0x5D,                                                       // POP EBP ; line 1749
	0xC3,                                                       // RET ; line 1750
};
static JumpTarget mokou5SC_NEWVOLC_BULLET_jumpData[] = {
	{ 0x00B, 0x68025F },
	{ 0x03C, 0x438C60 },
};
static PatchByteCode mokou5SC_NEWVOLC_BULLET_patchByteCode = { mokou5SC_NEWVOLC_BULLET_byteCode, 90, mokou5SC_NEWVOLC_BULLET_jumpData, 2 };
// mokou5SC_NEWVOLC_FLASH declared in mokou.asm line 1754
static unsigned char mokou5SC_NEWVOLC_FLASH_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x0003 ; line 1755
};
static PatchByteCode mokou5SC_NEWVOLC_FLASH_patchByteCode = { mokou5SC_NEWVOLC_FLASH_byteCode, 9, nullptr, 0 };
// mokou5SC_NEWVOLC_SOUND declared in mokou.asm line 1759
static unsigned char mokou5SC_NEWVOLC_SOUND_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00,       // CMP WORD PTR [ESI+0x13E],0x03 ; line 1760
	0x0F, 0x85, 0x0E, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1761
	0x6A, 0x1A,                                                 // PUSH 0x1A ; line 1763
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1764
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1765
	0xE9, 0x4C, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 1766
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x13E],0x00 ; line 1769
	0x0F, 0x85, 0x3D, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 1770
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00,       // CMP WORD PTR [ESI+0x140],0x04 ; line 1771
	0x0F, 0x85, 0x2E, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 1772
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1773
	0x0F, 0x85, 0x1F, 0x00, 0x00, 0x00,                         // JNE LABELEND ; line 1774
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1775
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1776
	0x6A, 0x08,                                                 // PUSH 0x08 ; line 1777
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1778
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1779
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 1780
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x673B38 ; line 1783
};
static JumpTarget mokou5SC_NEWVOLC_SOUND_jumpData[] = {
	{ 0x014, 0x464980 },
	{ 0x060, 0x464980 },
	{ 0x06A, 0x673B38 },
};
static PatchByteCode mokou5SC_NEWVOLC_SOUND_patchByteCode = { mokou5SC_NEWVOLC_SOUND_byteCode, 110, mokou5SC_NEWVOLC_SOUND_jumpData, 3 };
// mokou5SC_NEWVOLC_NOP declared in mokou.asm line 1787
static unsigned char mokou5SC_NEWVOLC_NOP_byteCode[] = {
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x194],0x00 ; line 1788
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x6842A1 ; line 1789
};
static JumpTarget mokou5SC_NEWVOLC_NOP_jumpData[] = {
	{ 0x008, 0x6842A1 },
};
static PatchByteCode mokou5SC_NEWVOLC_NOP_patchByteCode = { mokou5SC_NEWVOLC_NOP_byteCode, 12, mokou5SC_NEWVOLC_NOP_jumpData, 1 };
// mokou5SC_NEWVOLC_DIVE declared in mokou.asm line 1793
static unsigned char mokou5SC_NEWVOLC_DIVE_byteCode[] = {
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1794
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x13E],0x00 ; line 1795
	0x0F, 0x85, 0x29, 0x00, 0x00, 0x00,                   // JNE LABELEND ; line 1796
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x05, 0x00, // CMP WORD PTR [ESI+0x140],0x05 ; line 1797
	0x0F, 0x8C, 0x1A, 0x00, 0x00, 0x00,                   // JL LABELEND ; line 1798
	0xD9, 0xEE,                                           // FLDZ ; line 1799
	0xD8, 0x25, 0xB0, 0xB3, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B3B0] ; line 1800
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1801
	0xD9, 0x05, 0x60, 0xC1, 0x85, 0x00,                   // FLD DWORD PTR [0x85C160] ; line 1802
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 1803
};
static PatchByteCode mokou5SC_NEWVOLC_DIVE_patchByteCode = { mokou5SC_NEWVOLC_DIVE_byteCode, 62, nullptr, 0 };
// mokou5SC_NEWVOLC_HEIGHTCHECK declared in mokou.asm line 1808
static unsigned char mokou5SC_NEWVOLC_HEIGHTCHECK_byteCode[] = {
	0x90,                                                 // NOP ; line 1809
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1810
	0x0F, 0x8E, 0x2F, 0x00, 0x00, 0x00,                   // JLE LABEL1 ; line 1811
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 1812
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                   // FSUB DWORD PTR [ESI+0x100] ; line 1813
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1814
	0xD9, 0xEE,                                           // FLDZ ; line 1816
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 1817
	0xDE, 0xD9,                                           // FCOMPP ; line 1818
	0xDF, 0xE0,                                           // FNSTSW AX ; line 1819
	0xF6, 0xC4, 0x41,                                     // TEST AH,0x41 ; line 1820
	0x0F, 0x84, 0x08, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 1821
	0xD9, 0xEE,                                           // FLDZ ; line 1823
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 1824
};
static PatchByteCode mokou5SC_NEWVOLC_HEIGHTCHECK_patchByteCode = { mokou5SC_NEWVOLC_HEIGHTCHECK_byteCode, 63, nullptr, 0 };
// mokou5SC_NEWVOLC_GROUND declared in mokou.asm line 1831
static unsigned char mokou5SC_NEWVOLC_GROUND_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // CMP WORD PTR [ESI+0x140],0x01 ; line 1832
	0x0F, 0x85, 0x29, 0x00, 0x00, 0x00,                         // JNE LABEL2 ; line 1833
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00 ; line 1834
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1835
	0xD9, 0x05, 0x68, 0xBA, 0x85, 0x00,                         // FLD DWORD PTR [0x85BA68] ; line 1837
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 1838
	0xDD, 0x05, 0x20, 0x88, 0x85, 0x00,                         // FLD QWORD PTR [0x858820] ; line 1839
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 1840
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x09, 0x00,       // CMP WORD PTR [ESI+0x140],0x09 ; line 1842
	0x0F, 0x85, 0x0E, 0x00, 0x00, 0x00,                         // JNE LABEL1 ; line 1843
	0x8B, 0x16,                                                 // MOV EDX,[ESI] ; line 1845
	0x8B, 0x42, 0x08,                                           // MOV EAX,[EDX+0x08] ; line 1846
	0x68, 0x92, 0x02, 0x00, 0x00,                               // PUSH 0x00000292 ; line 1847
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 1848
	0xFF, 0xD0,                                                 // CALL EAX ; line 1849
};
static PatchByteCode mokou5SC_NEWVOLC_GROUND_patchByteCode = { mokou5SC_NEWVOLC_GROUND_byteCode, 85, nullptr, 0 };
// mokouD22_BULLET_ONHIT declared in mokou.asm line 1854
static unsigned char mokouD22_BULLET_ONHIT_byteCode[] = {
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 1855
	0x66, 0x81, 0xF8, 0x06, 0x00,             // CMP AX,0x0006 ; line 1856
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,       // JNE LABEL1 ; line 1857
	0x6A, 0x02,                               // PUSH 0x02 ; line 1858
	0x6A, 0x02,                               // PUSH 0x02 ; line 1859
	0x6A, 0x00,                               // PUSH 0x00 ; line 1860
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x48CE90 ; line 1861
	0x84, 0xC0,                               // TEST AL,AL ; line 1862
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,       // JNE 0x6801B9 ; line 1863
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 1865
};
static JumpTarget mokouD22_BULLET_ONHIT_jumpData[] = {
	{ 0x019, 0x48CE90 },
	{ 0x021, 0x6801B9 },
};
static PatchByteCode mokouD22_BULLET_ONHIT_patchByteCode = { mokouD22_BULLET_ONHIT_byteCode, 44, mokouD22_BULLET_ONHIT_jumpData, 2 };
// mokouD22_NEWDUR declared in mokou.asm line 1869
static unsigned char mokouD22_NEWDUR_byteCode[] = {
	0xB9, 0x3C, 0x00, 0x00, 0x00,       // MOV ECX,0x0000003C ; line 1870
	0x39, 0x8E, 0x44, 0x01, 0x00, 0x00, // CMP [ESI+0x144],ECX ; line 1871
};
static PatchByteCode mokouD22_NEWDUR_patchByteCode = { mokouD22_NEWDUR_byteCode, 11, nullptr, 0 };
// mokouD22_NEWBULLET declared in mokou.asm line 1875
static unsigned char mokouD22_NEWBULLET_byteCode[] = {
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00, // MOV EAX,[ESI+0x35C] ; line 1876
	0xD9, 0x40, 0x04,                   // FLD DWORD PTR [EAX+0x04] ; line 1877
	0xD8, 0x0D, 0x70, 0x78, 0x86, 0x00, // FMUL DWORD PTR [0x867870] ; line 1878
	0xD9, 0x50, 0x04,                   // FST DWORD PTR [EAX+0x04] ; line 1879
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x681693 ; line 1880
};
static JumpTarget mokouD22_NEWBULLET_jumpData[] = {
	{ 0x013, 0x681693 },
};
static PatchByteCode mokouD22_NEWBULLET_patchByteCode = { mokouD22_NEWBULLET_byteCode, 23, mokouD22_NEWBULLET_jumpData, 1 };
// mokouD22_NEWANGLE1 declared in mokou.asm line 1884
static unsigned char mokouD22_NEWANGLE1_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 1885
};
static PatchByteCode mokouD22_NEWANGLE1_patchByteCode = { mokouD22_NEWANGLE1_byteCode, 2, nullptr, 0 };
// mokouD22_NEWANGLE2 declared in mokou.asm line 1889
static unsigned char mokouD22_NEWANGLE2_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1890
	0xD8, 0x25, 0x8C, 0xB9, 0x85, 0x00, // FSUB DWORD PTR [0x85B98C] ; line 1891
};
static PatchByteCode mokouD22_NEWANGLE2_patchByteCode = { mokouD22_NEWANGLE2_byteCode, 8, nullptr, 0 };
// mokouD22_NEWANGLE3 declared in mokou.asm line 1895
static unsigned char mokouD22_NEWANGLE3_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1896
	0xD8, 0x05, 0x8C, 0xB9, 0x85, 0x00, // FADD DWORD PTR [0x85B98C] ; line 1897
};
static PatchByteCode mokouD22_NEWANGLE3_patchByteCode = { mokouD22_NEWANGLE3_byteCode, 8, nullptr, 0 };
// mokouD22_NEWALWAYS42 declared in mokou.asm line 1901
static unsigned char mokouD22_NEWALWAYS42_byteCode[] = {
	0xB0, 0x04, // MOV AL,0x04 ; line 1902
};
static PatchByteCode mokouD22_NEWALWAYS42_patchByteCode = { mokouD22_NEWALWAYS42_byteCode, 2, nullptr, 0 };
// mokouD22_NEWALWAYS4 declared in mokou.asm line 1906
static unsigned char mokouD22_NEWALWAYS4_byteCode[] = {
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x6A6],0x00 ; line 1907
};
static PatchByteCode mokouD22_NEWALWAYS4_patchByteCode = { mokouD22_NEWALWAYS4_byteCode, 7, nullptr, 0 };
// mokouD22_NEWHEIGHT declared in mokou.asm line 1913
static unsigned char mokouD22_NEWHEIGHT_byteCode[] = {
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xF0] ; line 1914
	0xD8, 0x05, 0xC8, 0x7D, 0x85, 0x00, // FADD DWORD PTR [0x857DC8] ; line 1915
};
static PatchByteCode mokouD22_NEWHEIGHT_patchByteCode = { mokouD22_NEWHEIGHT_byteCode, 12, nullptr, 0 };
// mokouD22_ALWAYSLEAP declared in mokou.asm line 1922
static unsigned char mokouD22_ALWAYSLEAP_byteCode[] = {
	0x6A, 0x07,                   // PUSH 0x07 ; line 1924
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x66EF43 ; line 1925
};
static JumpTarget mokouD22_ALWAYSLEAP_jumpData[] = {
	{ 0x003, 0x66EF43 },
};
static PatchByteCode mokouD22_ALWAYSLEAP_patchByteCode = { mokouD22_ALWAYSLEAP_byteCode, 7, mokouD22_ALWAYSLEAP_jumpData, 1 };
// mokouD22_FORWARDLEAP declared in mokou.asm line 1930
static unsigned char mokouD22_FORWARDLEAP_byteCode[] = {
	0xD9, 0xEE,                               // FLDZ ; line 1931
	0xD8, 0x25, 0x30, 0x88, 0x85, 0x00,       // FSUB DWORD PTR [0x858830] ; line 1932
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF4] ; line 1933
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1934
	0x6A, 0x03,                               // PUSH 0x03 ; line 1935
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF8] ; line 1936
	0x8D, 0x8C, 0x24, 0xE4, 0x00, 0x00, 0x00, // LEA ECX,[ESP+0xE4] ; line 1937
	0xD9, 0x05, 0xC0, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B9C0] ; line 1938
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x66ECA0 ; line 1939
};
static JumpTarget mokouD22_FORWARDLEAP_jumpData[] = {
	{ 0x02A, 0x66ECA0 },
};
static PatchByteCode mokouD22_FORWARDLEAP_patchByteCode = { mokouD22_FORWARDLEAP_byteCode, 46, mokouD22_FORWARDLEAP_jumpData, 1 };
// mokouD22_BACKLEAP declared in mokou.asm line 1945
static unsigned char mokouD22_BACKLEAP_byteCode[] = {
	0xD9, 0xEE,                               // FLDZ ; line 1946
	0xD8, 0x25, 0x30, 0x88, 0x85, 0x00,       // FSUB DWORD PTR [0x858830] ; line 1947
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF4] ; line 1948
	0x6A, 0x03,                               // PUSH 0x03 ; line 1949
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1950
	0x8D, 0x84, 0x24, 0xD8, 0x00, 0x00, 0x00, // LEA EAX,[ESP+0xD8] ; line 1951
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF8] ; line 1952
	0x50,                                     // PUSH EAX ; line 1953
	0xD9, 0x05, 0xC0, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B9C0] ; line 1954
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x66EE24 ; line 1955
};
static JumpTarget mokouD22_BACKLEAP_jumpData[] = {
	{ 0x02B, 0x66EE24 },
};
static PatchByteCode mokouD22_BACKLEAP_patchByteCode = { mokouD22_BACKLEAP_byteCode, 47, mokouD22_BACKLEAP_jumpData, 1 };
// mokouD22_UPLEAP declared in mokou.asm line 1959
static unsigned char mokouD22_UPLEAP_byteCode[] = {
	0xD9, 0xEE,                               // FLDZ ; line 1960
	0xD8, 0x25, 0x30, 0x88, 0x85, 0x00,       // FSUB DWORD PTR [0x858830] ; line 1961
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF4] ; line 1962
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,       // FLD DWORD PTR [0x858830] ; line 1963
	0x8D, 0x8C, 0x24, 0xF0, 0x00, 0x00, 0x00, // LEA ECX,[ESP+0xF0] ; line 1964
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xF8] ; line 1965
	0x51,                                     // PUSH ECX ; line 1966
	0xD9, 0x05, 0xC0, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B9C0] ; line 1967
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x66EF77 ; line 1968
};
static JumpTarget mokouD22_UPLEAP_jumpData[] = {
	{ 0x029, 0x66EF77 },
};
static PatchByteCode mokouD22_UPLEAP_patchByteCode = { mokouD22_UPLEAP_byteCode, 45, mokouD22_UPLEAP_jumpData, 1 };
// mokouA214PUNCH_CANG declared in mokou.asm line 1975
static unsigned char mokouA214PUNCH_CANG_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 1976
};
static PatchByteCode mokouA214PUNCH_CANG_patchByteCode = { mokouA214PUNCH_CANG_byteCode, 6, nullptr, 0 };
// mokouA236MAPLE_0 declared in mokou.asm line 1983
static unsigned char mokouA236MAPLE_0_byteCode[] = {
	0x6B, 0xD2, 0x00, // IMUL EDX,EDX,0x00000000 ; line 1984
};
static PatchByteCode mokouA236MAPLE_0_patchByteCode = { mokouA236MAPLE_0_byteCode, 3, nullptr, 0 };
// mokouA236MAPLE_1 declared in mokou.asm line 1986
static unsigned char mokouA236MAPLE_1_byteCode[] = {
	0x6B, 0xC0, 0x00, // IMUL EAX,EAX,0x00000000 ; line 1987
};
static PatchByteCode mokouA236MAPLE_1_patchByteCode = { mokouA236MAPLE_1_byteCode, 3, nullptr, 0 };
// mokouA236MAPLE_2 declared in mokou.asm line 1989
static unsigned char mokouA236MAPLE_2_byteCode[] = {
	0x69, 0xC0, 0xC8, 0x00, 0x00, 0x00, // IMUL EAX,EAX,0x000000C8 ; line 1990
};
static PatchByteCode mokouA236MAPLE_2_patchByteCode = { mokouA236MAPLE_2_byteCode, 6, nullptr, 0 };
// mokou3SCTORN_DEC declared in mokou.asm line 1999
static unsigned char mokou3SCTORN_DEC_byteCode[] = {
	0xDC, 0x25, 0xA8, 0x8E, 0x85, 0x00, // FSUB QWORD PTR [0x858EA8] ; line 2000
};
static PatchByteCode mokou3SCTORN_DEC_patchByteCode = { mokou3SCTORN_DEC_byteCode, 6, nullptr, 0 };
// mokou3SCTORN_MINSPD declared in mokou.asm line 2004
static unsigned char mokou3SCTORN_MINSPD_byteCode[] = {
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00, // FST DWORD PTR [ESI+0xF4] ; line 2005
	0xD9, 0xEE,                         // FLDZ ; line 2006
};
static PatchByteCode mokou3SCTORN_MINSPD_patchByteCode = { mokou3SCTORN_MINSPD_byteCode, 8, nullptr, 0 };
// mokou66C declared in mokou.asm line 2010
static unsigned char mokou66C_byteCode[] = {
	0xDD, 0x05, 0xB8, 0xCA, 0x85, 0x00, // FLD QWORD PTR [0x85CAB8] ; line 2011
};
static PatchByteCode mokou66C_patchByteCode = { mokou66C_byteCode, 6, nullptr, 0 };
// mokouPOSSESS_SKIP declared in mokou.asm line 2030
static unsigned char mokouPOSSESS_SKIP_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x683DE8 ; line 2031
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x683F38 ; line 2032
};
static JumpTarget mokouPOSSESS_SKIP_jumpData[] = {
	{ 0x002, 0x683DE8 },
	{ 0x007, 0x683F38 },
};
static PatchByteCode mokouPOSSESS_SKIP_patchByteCode = { mokouPOSSESS_SKIP_byteCode, 11, mokouPOSSESS_SKIP_jumpData, 2 };
// mokouPOSSESS_SKIP1 declared in mokou.asm line 2034
static unsigned char mokouPOSSESS_SKIP1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x683F38 ; line 2035
};
static JumpTarget mokouPOSSESS_SKIP1_jumpData[] = {
	{ 0x001, 0x683F38 },
};
static PatchByteCode mokouPOSSESS_SKIP1_patchByteCode = { mokouPOSSESS_SKIP1_byteCode, 5, mokouPOSSESS_SKIP1_jumpData, 1 };
// mokouPOSSESS_SKIP2 declared in mokou.asm line 2041
static unsigned char mokouPOSSESS_SKIP2_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x683B9A ; line 2042
};
static JumpTarget mokouPOSSESS_SKIP2_jumpData[] = {
	{ 0x001, 0x683B9A },
};
static PatchByteCode mokouPOSSESS_SKIP2_patchByteCode = { mokouPOSSESS_SKIP2_byteCode, 5, mokouPOSSESS_SKIP2_jumpData, 1 };
// mokouPOSSESS declared in mokou.asm line 2048
static unsigned char mokouPOSSESS_byteCode[] = {
	0x8B, 0x8E, 0x70, 0x01, 0x00, 0x00,       // MOV ECX,[ESI+0x170] ; line 2049
	0x8B, 0x99, 0xF0, 0x00, 0x00, 0x00,       // MOV EBX,[ECX+0xF0] ; line 2050
	0x8B, 0x89, 0xEC, 0x00, 0x00, 0x00,       // MOV ECX,[ECX+0xEC] ; line 2051
	0x89, 0x8E, 0xEC, 0x00, 0x00, 0x00,       // MOV [ESI+0xEC],ECX ; line 2052
	0x89, 0x9E, 0xF0, 0x00, 0x00, 0x00,       // MOV [ESI+0xF0],EBX ; line 2053
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,       // MOV EBX,[ESI+0x398] ; line 2055
	0x0F, 0xB7, 0x9B, 0x3C, 0x01, 0x00, 0x00, // MOVZX EBX,WORD PTR [EBX+0x13C] ; line 2056
	0x66, 0x81, 0xFB, 0x32, 0x00,             // CMP BX,0x0032 ; line 2057
	0x0F, 0x8C, 0x10, 0x00, 0x00, 0x00,       // JL SHORT LABEL1 ; line 2059
	0x66, 0x81, 0xFB, 0xC7, 0x00,             // CMP BX,0x00C7 ; line 2061
	0x0F, 0x8D, 0x05, 0x00, 0x00, 0x00,       // JNL SHORT LABEL1 ; line 2062
	0xE9, 0x28, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 2063
	0x8B, 0x9E, 0x42, 0x01, 0x00, 0x00,       // MOV EBX,[ESI+0x142] ; line 2065
	0x66, 0x81, 0xFB, 0x04, 0x00,             // CMP BX,0x0004 ; line 2066
	0x0F, 0x85, 0x17, 0x00, 0x00, 0x00,       // JNE SHORT LABEL2 ; line 2067
	0xBD, 0x08, 0x00, 0x00, 0x00,             // MOV EBP,0x00000008 ; line 2068
	0x89, 0xF3,                               // MOV EBX,ESI ; line 2069
	0x8B, 0xB6, 0x98, 0x03, 0x00, 0x00,       // MOV ESI,[ESI+0x398] ; line 2070
	0xB9, 0x00, 0x00, 0x00, 0x00,             // MOV ECX,0x00000000 ; line 2071
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x672A90 ; line 2072
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x680283 ; line 2075
};
static JumpTarget mokouPOSSESS_jumpData[] = {
	{ 0x06A, 0x672A90 },
	{ 0x06F, 0x680283 },
};
static PatchByteCode mokouPOSSESS_patchByteCode = { mokouPOSSESS_byteCode, 115, mokouPOSSESS_jumpData, 2 };
// mokouPOSSESS_2 declared in mokou.asm line 2080
static unsigned char mokouPOSSESS_2_byteCode[] = {
	0x89, 0xDE,                   // MOV ESI,EBX ; line 2081
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x680283 ; line 2082
};
static JumpTarget mokouPOSSESS_2_jumpData[] = {
	{ 0x003, 0x680283 },
};
static PatchByteCode mokouPOSSESS_2_patchByteCode = { mokouPOSSESS_2_byteCode, 7, mokouPOSSESS_2_jumpData, 1 };
// mokouPOSSESS_3 declared in mokou.asm line 2085
static unsigned char mokouPOSSESS_3_byteCode[] = {
	0x68, 0xFF, 0xFF, 0xFF, 0xFF, // PUSH 0xFFFFFFFF ; line 2086
	0x68, 0xC8, 0x00, 0x00, 0x00, // PUSH 0x000000C8 ; line 2087
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x672AB7 ; line 2088
};
static JumpTarget mokouPOSSESS_3_jumpData[] = {
	{ 0x00B, 0x672AB7 },
};
static PatchByteCode mokouPOSSESS_3_patchByteCode = { mokouPOSSESS_3_byteCode, 15, mokouPOSSESS_3_jumpData, 1 };
// mokouPOSSESS_A declared in mokou.asm line 2094
static unsigned char mokouPOSSESS_A_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 2095
	0x60,                                                       // PUSHAD ; line 2096
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2097
	0x0F, 0x85, 0x1E, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 2098
	0x8B, 0x8E, 0x70, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x170] ; line 2099
	0x8B, 0x99, 0xF0, 0x00, 0x00, 0x00,                         // MOV EBX,[ECX+0xF0] ; line 2100
	0x8B, 0x89, 0xEC, 0x00, 0x00, 0x00,                         // MOV ECX,[ECX+0xEC] ; line 2101
	0x89, 0x8E, 0xEC, 0x00, 0x00, 0x00,                         // MOV [ESI+0xEC],ECX ; line 2102
	0x89, 0x9E, 0xF0, 0x00, 0x00, 0x00,                         // MOV [ESI+0xF0],EBX ; line 2103
	0x61,                                                       // POPAD ; line 2105
	0x66, 0x9D,                                                 // POPF ; line 2106
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x1E ; line 2108
	0x0F, 0x8E, 0xDE, 0xAD, 0xBE, 0xEF,                         // JLE 0x683E64 ; line 2109
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x683E86 ; line 2110
};
static JumpTarget mokouPOSSESS_A_jumpData[] = {
	{ 0x040, 0x683E64 },
	{ 0x045, 0x683E86 },
};
static PatchByteCode mokouPOSSESS_A_patchByteCode = { mokouPOSSESS_A_byteCode, 73, mokouPOSSESS_A_jumpData, 2 };
// mokouPOSSESS_END declared in mokou.asm line 2125
static unsigned char mokouPOSSESS_END_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x680283 ; line 2126
};
static JumpTarget mokouPOSSESS_END_jumpData[] = {
	{ 0x001, 0x680283 },
};
static PatchByteCode mokouPOSSESS_END_patchByteCode = { mokouPOSSESS_END_byteCode, 5, mokouPOSSESS_END_jumpData, 1 };
// mokouPOSSESS_4 declared in mokou.asm line 2130
static unsigned char mokouPOSSESS_4_byteCode[] = {
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 2131
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x35C] ; line 2132
	0xC7, 0x86, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x12C],0x00000000 ; line 2133
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                         // SUB ESP,0x08 ; line 2134
	0x8D, 0x4E, 0x04,                                           // LEA ECX,[ESI+0x04] ; line 2135
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x11C] ; line 2136
	0xD9, 0x86, 0x20, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x120] ; line 2137
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 2138
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x683E18 ; line 2139
};
static JumpTarget mokouPOSSESS_4_jumpData[] = {
	{ 0x032, 0x683E18 },
};
static PatchByteCode mokouPOSSESS_4_patchByteCode = { mokouPOSSESS_4_byteCode, 54, mokouPOSSESS_4_jumpData, 1 };
// mokouPOSSESS_FLASH declared in mokou.asm line 2144
static unsigned char mokouPOSSESS_FLASH_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x03 ; line 2145
};
static PatchByteCode mokouPOSSESS_FLASH_patchByteCode = { mokouPOSSESS_FLASH_byteCode, 9, nullptr, 0 };
// mokou3SCMETEOR_SOUND declared in mokou.asm line 2151
static unsigned char mokou3SCMETEOR_SOUND_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x0001 ; line 2152
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2153
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x59, 0x00, // CMP WORD PTR [ESI+0x140],0x0059 ; line 2154
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2155
	0x6A, 0x18,                                           // PUSH 0x18 ; line 2156
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 2157
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 2158
	0x66, 0x39, 0x9E, 0x3E, 0x01, 0x00, 0x00,             // CMP [ESI+0x13E],BX ; line 2160
	0x90,                                                 // NOP ; line 2161
};
static JumpTarget mokou3SCMETEOR_SOUND_jumpData[] = {
	{ 0x027, 0x464980 },
};
static PatchByteCode mokou3SCMETEOR_SOUND_patchByteCode = { mokou3SCMETEOR_SOUND_byteCode, 51, mokou3SCMETEOR_SOUND_jumpData, 1 };


// mokou.asm
std::array<PatchSkeleton, 188> patchList = {
	PatchSkeleton{ mokouID_FIX_patchByteCode,                  (void *)0x665388,  7}, // Declared line 14. Patch on CMP [ESI+0x196], DI
	PatchSkeleton{ mokouA214REKKA_HPCOST_1_patchByteCode,      (void *)0x66D451,  5}, // Declared line 54. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouA214PUNCH_HPCOST_1_patchByteCode,      (void *)0x66E53A,  5}, // Declared line 56. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouD214_HPCOST_1_patchByteCode,           (void *)0x66CA07,  5}, // Declared line 58. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouD214_HPCOST_1_patchByteCode,           (void *)0x66CC4D,  5}, // Declared line 60. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouD214_HPCOST_1_patchByteCode,           (void *)0x66CE65,  5}, // Declared line 62. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouD214_HPCOST_1_patchByteCode,           (void *)0x66D0EA,  5}, // Declared line 64. Patch on CALL 0x0047A9E0
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E118,  6}, // Declared line 69. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E13C,  6}, // Declared line 71. Patch on FLD ST, DWORD PTR [0x85B998]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E160,  6}, // Declared line 72. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E1AC,  6}, // Declared line 74. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E1BC,  6}, // Declared line 75. Patch on FLD ST, DWORD PTR [0x85B990]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66E1D3,  6}, // Declared line 76. Patch on FLD ST, DWORD PTR [0x85B98C]
	PatchSkeleton{ mokouA214REKKA_DECEL_patchByteCode,         (void *)0x66D15E,  7}, // Declared line 137. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokouA214REKKA_DECEL2_patchByteCode,        (void *)0x66D778,  8}, // Declared line 140. Patch on CMP WORD PTR [ESI+0x13E], 5
	PatchSkeleton{ mokouA214REKKA_DECEL3_patchByteCode,        (void *)0x66DD6A,  7}, // Declared line 179. Patch on MOVZX ECX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokouA214REKKA_2MAXDUR_patchByteCode,       (void *)0x66DD97,  6}, // Declared line 186. Patch on MOV EAX, [ESI+0x144]
	PatchSkeleton{ mokouA214REKKA_IDKLOL_patchByteCode,        (void *)0x66E0E1,  8}, // Declared line 218. Patch on CMP WORD PTR [ESI+0x13E], 1
	PatchSkeleton{ mokouA214REKKA_ENDGRAVITY_patchByteCode,    (void *)0x66E040,  6}, // Declared line 223. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ mokouA214REKKA_BCHOICE_patchByteCode,       (void *)0x66D1A0,  6}, // Declared line 273. Patch on MOV EAX, [ESI+0x190]
	PatchSkeleton{ mokouA214REKKA_ABCHOICE_patchByteCode,      (void *)0x66D7A5,  6}, // Declared line 321. Patch on MOV EAX, [ESI+0x190]
	PatchSkeleton{ mokouA214REKKA_CTOB_patchByteCode,          (void *)0x66D470,  7}, // Declared line 326. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokouA214REKKA_ACTOAB_patchByteCode,        (void *)0x66D8DC,  5}, // Declared line 331. Patch on MOV EBP, 5
	PatchSkeleton{ mokouA214REKKA_CONTINUE1_patchByteCode,     (void *)0x66DDCA,  9}, // Declared line 379. Patch on JNE SHORT 0x0066DDD5
	PatchSkeleton{ mokouA214REKKA_CONTINUE2_patchByteCode,     (void *)0x66DF6C,  9}, // Declared line 381. Patch on JNE SHORT 0x0066DF77
	PatchSkeleton{ mokouA214REKKA_3RDANG_patchByteCode,        (void *)0x66E187,  6}, // Declared line 386. Patch on FLD ST, DWORD PTR [0x85B990]
	PatchSkeleton{ mokouA214REKKA_3RDANG_patchByteCode,        (void *)0x66E170,  6}, // Declared line 387. Patch on FLD ST, DWORD PTR [0x85B998]
	PatchSkeleton{ mokouA214REKKA_V2_patchByteCode,            (void *)0x68111E,  6}, // Declared line 471. Patch on JNE 0x00680283
	PatchSkeleton{ mokouA22RES1_patchByteCode,                 (void *)0x66F308,  7}, // Declared line 479. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ mokouA22RES2_patchByteCode,                 (void *)0x66F335,  6}, // Declared line 510. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ mokouA22RESTRAJECTORY_patchByteCode,        (void *)0x66F345,  6}, // Declared line 515. Patch on FLD ST, DWORD PTR [0x85C500]
	PatchSkeleton{ mokouA22RESTRAJECTORY_patchByteCode,        (void *)0x66F353,  6}, // Declared line 516. Patch on FLD ST, DWORD PTR [0x85C500]
	PatchSkeleton{ mokouA22PARTICLE_patchByteCode,             (void *)0x681965,  6}, // Declared line 527. Patch on JNE 0x006801B9
	PatchSkeleton{ mokouA22PARTICLE2_patchByteCode,            (void *)0x68198A, 10}, // Declared line 533. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ mokouA22RESTRAJECTORY2_patchByteCode,       (void *)0x66F167,  7}, // Declared line 544. Patch on CMP [ESI+0x13E], DI
	PatchSkeleton{ mokouA22CJUMPTOB_patchByteCode,             (void *)0x66F3CE,  5}, // Declared line 549. Patch on MOV EDI, 2
	PatchSkeleton{ mokouTORNADOSC_patchByteCode,               (void *)0x671B5D,  8}, // Declared line 576. Patch on CMP WORD PTR [ESI+0x13E], 1
	PatchSkeleton{ mokouTORNADOSC2_patchByteCode,              (void *)0x671C63,  8}, // Declared line 581. Patch on CMP WORD PTR [ESI+0x140], 6
	PatchSkeleton{ mokouD22GROUNDEDLEVEL_patchByteCode,        (void *)0x67DAA7,  9}, // Declared line 618. Patch on CMP EAX, 0xB
	PatchSkeleton{ mokouD22GROUNDED_B_patchByteCode,           (void *)0x67E68A,  6}, // Declared line 657. Patch on JL 0x0067F536
	PatchSkeleton{ mokouD22GROUNDED_C_patchByteCode,           (void *)0x67E608,  7}, // Declared line 697. Patch on CMP BYTE PTR [ESI+0x6AE], 1
	PatchSkeleton{ mokou6DINIT_patchByteCode,                  (void *)0x665F61,  6}, // Declared line 702. Patch on FLD ST, DWORD PTR [0x85BB28]
	PatchSkeleton{ mokouDRUMX_patchByteCode,                   (void *)0x66F945,  6}, // Declared line 708. Patch on FLD ST, DWORD PTR [0x85B9CC]
	PatchSkeleton{ mokouDRUMY_patchByteCode,                   (void *)0x66F951,  6}, // Declared line 714. Patch on FLD ST, DWORD PTR [0x85BCA4]
	PatchSkeleton{ mokouJ8A_patchByteCode,                     (void *)0x668B1F,  6}, // Declared line 719. Patch on FLD ST, DWORD PTR [0x85BC30]
	PatchSkeleton{ mokouONHIT_REMOVE_patchByteCode,            (void *)0x681E60,  7}, // Declared line 734. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ mokouD236C_patchByteCode,                   (void *)0x67E1CB,  5}, // Declared line 740. Patch on PUSH 0x1F4
	PatchSkeleton{ mokouD236C_patchByteCode,                   (void *)0x67E219,  5}, // Declared line 741. Patch on PUSH 0x1F4
	PatchSkeleton{ mokouD236C_JMP_patchByteCode,               (void *)0x66817E,  5}, // Declared line 749. Patch on CMP EAX, 0x1F4
	PatchSkeleton{ mokouD236C_MOMENTUM_patchByteCode,          (void *)0x67C3BC,  5}, // Declared line 757. Patch on CMP EAX, 0x1F4
	PatchSkeleton{ mokouD236C_SPAWN_patchByteCode,             (void *)0x66B939,  5}, // Declared line 771. Patch on SHL EDX, 4
	PatchSkeleton{ mokouWAKEUP_patchByteCode,                  (void *)0x6858A3,  6}, // Declared line 812. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ mokouWAKEUP_TELE1_patchByteCode,            (void *)0x665996,  7}, // Declared line 841. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ mokouWAKEUP_TELE2_patchByteCode,            (void *)0x665B65,  7}, // Declared line 842. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ mokouWAKEUP_TELE3_patchByteCode,            (void *)0x665CBB,  8}, // Declared line 843. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ mokouWAKEUP_TELE1A_patchByteCode,           (void *)0x665B0E,  6}, // Declared line 855. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ mokouFLD075_patchByteCode,                  (void *)0x665B02,  6}, // Declared line 856. Patch on FLD ST, DWORD PTR [0x85E10C]
	PatchSkeleton{ mokouWAKEUP_TELE1B_patchByteCode,           (void *)0x665C9D,  6}, // Declared line 858. Patch on FLD ST, DWORD PTR [0x85B9E8]
	PatchSkeleton{ mokouFLD075_patchByteCode,                  (void *)0x665C91,  6}, // Declared line 859. Patch on FLD ST, DWORD PTR [0x85E10C]
	PatchSkeleton{ mokou2B_patchByteCode,                      (void *)0x669801,  7}, // Declared line 870. Patch on PUSH 0x2D
	PatchSkeleton{ mokou5B2B_OFFSET_patchByteCode,             (void *)0x669064,  6}, // Declared line 883. Patch on FADD ST, QWORD PTR [0x85E0F8]
	PatchSkeleton{ mokou5B2B_ANG1_patchByteCode,               (void *)0x668FF1,  6}, // Declared line 923. Patch on FLD ST, DWORD PTR [0x85B9E0]
	PatchSkeleton{ mokou5B2B_ANG2_patchByteCode,               (void *)0x669000,  6}, // Declared line 924. Patch on FLD ST, DWORD PTR [0x85AE94]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66A515,  6}, // Declared line 928. Patch on FLD ST, DWORD PTR [0x85C000]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66A524,  6}, // Declared line 930. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouFLD30_patchByteCode,                   (void *)0x66A665,  6}, // Declared line 964. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66A674,  6}, // Declared line 965. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLDN30_patchByteCode,                  (void *)0x66A6E1,  6}, // Declared line 966. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66A6F0,  6}, // Declared line 967. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouFLDN60_patchByteCode,                  (void *)0x66AA9B,  6}, // Declared line 970. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66AAAA,  6}, // Declared line 971. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLDN90_patchByteCode,                  (void *)0x66ABEB,  6}, // Declared line 973. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66ABFA,  6}, // Declared line 974. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLDN30_patchByteCode,                  (void *)0x66AC67,  6}, // Declared line 975. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66AC76,  6}, // Declared line 976. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66AEA2,  6}, // Declared line 979. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66AEB1,  6}, // Declared line 980. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouFLD30_patchByteCode,                   (void *)0x66AFF2,  6}, // Declared line 983. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B001,  6}, // Declared line 984. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLDN30_patchByteCode,                  (void *)0x66B06E,  6}, // Declared line 985. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B07D,  6}, // Declared line 986. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouFLD30_patchByteCode,                   (void *)0x66B441,  6}, // Declared line 989. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B450,  6}, // Declared line 990. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLD60_patchByteCode,                   (void *)0x66B591,  6}, // Declared line 993. Patch on FLD ST, DWORD PTR [0x85BB54]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B5A0,  6}, // Declared line 994. Patch on FLD ST, DWORD PTR [0x85BC70]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B60D,  6}, // Declared line 995. Patch on FLD ST, DWORD PTR [0x85C5BC]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x66B61C,  6}, // Declared line 996. Patch on FLD ST, DWORD PTR [0x867860]
	PatchSkeleton{ mokouJ5C_OFFSET_patchByteCode,              (void *)0x66AEDF,  6}, // Declared line 1001. Patch on FADD ST, QWORD PTR [0x85E0E8]
	PatchSkeleton{ mokou5C_NEWSYS_patchByteCode,               (void *)0x6802A1,  7}, // Declared line 1024. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ mokou5C_SPD_patchByteCode,                  (void *)0x680368,  6}, // Declared line 1040. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ mokouHASTE_patchByteCode,                   (void *)0x683F6A,  7}, // Declared line 1058. Patch on MOVSX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokouHASTE_DUR_patchByteCode,               (void *)0x67363E,  9}, // Declared line 1063. Patch on MOV WORD PTR [ESI+0x892], 0x168
	PatchSkeleton{ mokouHASTE_DUR_patchByteCode,               (void *)0x67379B,  9}, // Declared line 1064. Patch on MOV WORD PTR [ESI+0x892], 0x1C2
	PatchSkeleton{ mokou5B_NEWA_patchByteCode,                 (void *)0x668F67,  6}, // Declared line 1146. Patch on JNE 0x006690AC
	PatchSkeleton{ mokou5B_NEWA_CHARGE_patchByteCode,          (void *)0x6690CF,  6}, // Declared line 1148. Patch on JNE 0x0066B834
	PatchSkeleton{ mokou5B_NEWB_patchByteCode,                 (void *)0x669429,  6}, // Declared line 1150. Patch on JNE 0x0066955B
	PatchSkeleton{ mokou5B_NEWB_CHARGE_patchByteCode,          (void *)0x66957E,  6}, // Declared line 1152. Patch on JNE 0x0066B834
	PatchSkeleton{ mokou5B_NEWC_patchByteCode,                 (void *)0x669A33,  6}, // Declared line 1154. Patch on JNE 0x00669B78
	PatchSkeleton{ mokou5B_NEWC_CHARGE_patchByteCode,          (void *)0x669B9B,  6}, // Declared line 1156. Patch on JNE 0x0066B834
	PatchSkeleton{ mokou5B_NEWD_patchByteCode,                 (void *)0x669F54,  6}, // Declared line 1158. Patch on JNE 0x0066A09C
	PatchSkeleton{ mokou5B_NEWD_CHARGE_patchByteCode,          (void *)0x66A0C4,  6}, // Declared line 1160. Patch on JNE 0x0066B834
	PatchSkeleton{ mokou5B_METER_patchByteCode,                (void *)0x668F6D,  8}, // Declared line 1167. Patch on PUSH EBP
	PatchSkeleton{ mokou5B_METER_patchByteCode,                (void *)0x6690D5,  9}, // Declared line 1168. Patch on PUSH 0x1E
	PatchSkeleton{ mokou6B_METER_patchByteCode,                (void *)0x6694B9, 14}, // Declared line 1174. Patch on PUSH 0xA
	PatchSkeleton{ mokou6B_METER_patchByteCode,                (void *)0x66962B, 14}, // Declared line 1175. Patch on PUSH 0x1E
	PatchSkeleton{ mokou2B_METER_patchByteCode,                (void *)0x6697F8,  9}, // Declared line 1181. Patch on PUSH 0x1E
	PatchSkeleton{ mokouJ5B_METER_patchByteCode,               (void *)0x669AE0, 13}, // Declared line 1187. Patch on PUSH EBP
	PatchSkeleton{ mokouJ5B_METER_patchByteCode,               (void *)0x669C48, 14}, // Declared line 1188. Patch on PUSH 0x1E
	PatchSkeleton{ mokouJ2B_METER_patchByteCode,               (void *)0x66A006, 10}, // Declared line 1194. Patch on PUSH 0xA
	PatchSkeleton{ mokouJ2B_METER_patchByteCode,               (void *)0x66A171, 10}, // Declared line 1195. Patch on PUSH 0x1E
	PatchSkeleton{ mokou5B_NEW1A_patchByteCode,                (void *)0x669099,  5}, // Declared line 1341. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1A_CHARGE_patchByteCode,         (void *)0x669202,  5}, // Declared line 1343. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1B_patchByteCode,                (void *)0x669548,  5}, // Declared line 1345. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1B_CHARGE_patchByteCode,         (void *)0x6696B1,  5}, // Declared line 1347. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1C_patchByteCode,                (void *)0x669B65,  5}, // Declared line 1349. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1C_CHARGE_patchByteCode,         (void *)0x669CC5,  5}, // Declared line 1351. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1D_patchByteCode,                (void *)0x66A087,  5}, // Declared line 1353. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW1D_CHARGE_patchByteCode,         (void *)0x66A1EF,  5}, // Declared line 1355. Patch on PUSH 0x320
	PatchSkeleton{ mokou5B_NEW2A_patchByteCode,                (void *)0x669042,  7}, // Declared line 1663. Patch on FSTP DWORD PTR [ESP+0x3E8], ST
	PatchSkeleton{ mokou5B_NEW2A_CHARGE_patchByteCode,         (void *)0x6691AB,  7}, // Declared line 1665. Patch on FSTP DWORD PTR [ESP+0x9C], ST
	PatchSkeleton{ mokou5B_NEW2B_patchByteCode,                (void *)0x6694F6,  7}, // Declared line 1667. Patch on FSTP DWORD PTR [ESP+0x48C], ST
	PatchSkeleton{ mokou5B_NEW2B_CHARGE_patchByteCode,         (void *)0x66965A,  7}, // Declared line 1669. Patch on FSTP DWORD PTR [ESP+0xB4], ST
	PatchSkeleton{ mokou5B_NEW2C_patchByteCode,                (void *)0x669B0E,  7}, // Declared line 1671. Patch on FSTP DWORD PTR [ESP+0x3B8], ST
	PatchSkeleton{ mokou5B_NEW2C_CHARGE_patchByteCode,         (void *)0x669C71,  5}, // Declared line 1673. Patch on FSTP DWORD PTR [ESP+0x78], ST
	PatchSkeleton{ mokou5B_NEW2D_patchByteCode,                (void *)0x66A031,  7}, // Declared line 1675. Patch on FSTP DWORD PTR [ESP+0x2E0], ST
	PatchSkeleton{ mokou5B_NEW2D_CHARGE_patchByteCode,         (void *)0x66A199,  7}, // Declared line 1677. Patch on FSTP DWORD PTR [ESP+0x84], ST
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x66900E, 14}, // Declared line 1680. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x669177, 14}, // Declared line 1682. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x6694AB, 14}, // Declared line 1684. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x66961D, 14}, // Declared line 1686. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x669AD2, 14}, // Declared line 1688. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x669C3A, 14}, // Declared line 1690. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x669FF3, 14}, // Declared line 1692. Patch on PUSH 0x2D
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x66A163, 14}, // Declared line 1694. Patch on PUSH 0x2D
	PatchSkeleton{ mokou5SC_NEWVOLC_FOLLOW_patchByteCode,      (void *)0x684154,  7}, // Declared line 1717. Patch on MOVSX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokou5SC_NEWVOLC_SUBSEQ_patchByteCode,      (void *)0x673A47,  6}, // Declared line 1725. Patch on JG 0x00673B1F
	PatchSkeleton{ mokou5SC_NEWVOLC_BULLET_patchByteCode,      (void *)0x6842AC,  9}, // Declared line 1752. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ mokou5SC_NEWVOLC_FLASH_patchByteCode,       (void *)0x673BB7,  8}, // Declared line 1757. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ mokou5SC_NEWVOLC_SOUND_patchByteCode,       (void *)0x673B2F,  9}, // Declared line 1785. Patch on PUSH 0x11
	PatchSkeleton{ mokou5SC_NEWVOLC_NOP_patchByteCode,         (void *)0x68422C,  7}, // Declared line 1791. Patch on CMP BYTE PTR [ESI+0x194], 0
	PatchSkeleton{ mokou5SC_NEWVOLC_DIVE_patchByteCode,        (void *)0x673C82,  6}, // Declared line 1806. Patch on FSTP DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ mokou5SC_NEWVOLC_HEIGHTCHECK_patchByteCode, (void *)0x6737B5,  5}, // Declared line 1829. Patch on CALL 0x00487CA0
	PatchSkeleton{ mokou5SC_NEWVOLC_GROUND_patchByteCode,      (void *)0x673951,  8}, // Declared line 1852. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ mokouD22_BULLET_ONHIT_patchByteCode,        (void *)0x6813EC,  7}, // Declared line 1867. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ mokouD22_NEWDUR_patchByteCode,              (void *)0x68160C,  6}, // Declared line 1873. Patch on CMP [ESI+0x144], ECX
	PatchSkeleton{ mokouD22_NEWBULLET_patchByteCode,           (void *)0x68168A,  6}, // Declared line 1882. Patch on MOV EAX, [ESI+0x35C]
	PatchSkeleton{ mokouD22_NEWANGLE1_patchByteCode,           (void *)0x66EF81,  6}, // Declared line 1887. Patch on FLD ST, DWORD PTR [0x85B984]
	PatchSkeleton{ mokouD22_NEWANGLE2_patchByteCode,           (void *)0x66F036,  6}, // Declared line 1893. Patch on FLD ST, DWORD PTR [0x85BDBC]
	PatchSkeleton{ mokouD22_NEWANGLE3_patchByteCode,           (void *)0x66F070,  6}, // Declared line 1899. Patch on FLD ST, DWORD PTR [0x85BAE0]
	PatchSkeleton{ mokouD22_NEWALWAYS42_patchByteCode,         (void *)0x6815E0,  6}, // Declared line 1904. Patch on MOV AL, [EAX+0x6A6]
	PatchSkeleton{ mokouD22_NEWALWAYS4_patchByteCode,          (void *)0x66EEC0,  7}, // Declared line 1909. Patch on CMP BYTE PTR [ESI+0x6A6], 4
	PatchSkeleton{ mokouD22_NEWALWAYS4_patchByteCode,          (void *)0x66ED3D,  7}, // Declared line 1910. Patch on CMP BYTE PTR [ESI+0x6A6], 4
	PatchSkeleton{ mokouD22_NEWALWAYS4_patchByteCode,          (void *)0x66F013,  7}, // Declared line 1911. Patch on CMP BYTE PTR [ESI+0x6A6], 4
	PatchSkeleton{ mokouD22_NEWHEIGHT_patchByteCode,           (void *)0x66F04B,  6}, // Declared line 1917. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ mokouD22_NEWHEIGHT_patchByteCode,           (void *)0x66EFE6,  6}, // Declared line 1918. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ mokouD22_NEWHEIGHT_patchByteCode,           (void *)0x66EFAF,  6}, // Declared line 1919. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ mokouD22_NEWHEIGHT_patchByteCode,           (void *)0x66EDBB,  6}, // Declared line 1920. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x66EC4D,  6}, // Declared line 1927. Patch on JGE 0x0066F089
	PatchSkeleton{ mokouD22_ALWAYSLEAP_patchByteCode,          (void *)0x66EC67,  6}, // Declared line 1928. Patch on JLE 0x0066EDE5
	PatchSkeleton{ mokouD22_FORWARDLEAP_patchByteCode,         (void *)0x66EC7F,  6}, // Declared line 1943. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ mokouD22_BACKLEAP_patchByteCode,            (void *)0x66EDFC,  6}, // Declared line 1957. Patch on FLD ST, DWORD PTR [0x85B960]
	PatchSkeleton{ mokouD22_UPLEAP_patchByteCode,              (void *)0x66EF5D,  6}, // Declared line 1970. Patch on FLD ST, DWORD PTR [0x859F00]
	PatchSkeleton{ mokouA214PUNCH_CANG_patchByteCode,          (void *)0x66E6ED,  6}, // Declared line 1978. Patch on FLD ST, DWORD PTR [0x85B98C]
	PatchSkeleton{ mokouA214PUNCH_CANG_patchByteCode,          (void *)0x66EAE0,  6}, // Declared line 1979. Patch on FLD ST, DWORD PTR [0x85B98C]
	PatchSkeleton{ mokouA236MAPLE_0_patchByteCode,             (void *)0x66C3A7,  6}, // Declared line 1992. Patch on IMUL EDX, 0x113
	PatchSkeleton{ mokouA236MAPLE_1_patchByteCode,             (void *)0x66C479,  6}, // Declared line 1993. Patch on IMUL EAX, 0x177
	PatchSkeleton{ mokouA236MAPLE_2_patchByteCode,             (void *)0x66C5CE,  6}, // Declared line 1995. Patch on IMUL EAX, 0x28A
	PatchSkeleton{ mokouA236MAPLE_2_patchByteCode,             (void *)0x66C69D,  6}, // Declared line 1996. Patch on IMUL EAX, 0x28A
	PatchSkeleton{ mokou3SCTORN_DEC_patchByteCode,             (void *)0x6827E9,  6}, // Declared line 2002. Patch on FSUB ST, QWORD PTR [0x858EA8]
	PatchSkeleton{ mokou3SCTORN_MINSPD_patchByteCode,          (void *)0x6827F7,  8}, // Declared line 2008. Patch on FST DWORD PTR [ESI+0xF4], ST
	PatchSkeleton{ mokou66C_patchByteCode,                     (void *)0x66B7B6,  6}, // Declared line 2013. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x673297,  6}, // Declared line 2017. Patch on FLD ST, DWORD PTR [0x871540]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x6732A5,  6}, // Declared line 2019. Patch on FLD ST, DWORD PTR [0x859F00]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x6727A6,  6}, // Declared line 2021. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ mokouFLDZ_patchByteCode,                    (void *)0x6733A3,  6}, // Declared line 2023. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x683F3E,  6}, // Declared line 2026. Patch on FADD ST, QWORD PTR [0x857F78]
	PatchSkeleton{ mokouNOP_patchByteCode,                     (void *)0x683F50,  6}, // Declared line 2027. Patch on FADD ST, QWORD PTR [0x85A280]
	PatchSkeleton{ mokouPOSSESS_SKIP_patchByteCode,            (void *)0x683B8D,  7}, // Declared line 2037. Patch on JMP DWORD PTR [EAX*4+0x685C04]
	PatchSkeleton{ mokouPOSSESS_SKIP1_patchByteCode,           (void *)0x683B94,  6}, // Declared line 2038. Patch on FLD ST, DWORD PTR [0x871540]
	PatchSkeleton{ mokouPOSSESS_SKIP2_patchByteCode,           (void *)0x683F5C,  5}, // Declared line 2044. Patch on JMP 0x00680283
	PatchSkeleton{ mokouPOSSESS_patchByteCode,                 (void *)0x683B9A,  7}, // Declared line 2078. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ mokouPOSSESS_2_patchByteCode,               (void *)0x672B3C,  8}, // Declared line 2084. Patch on ADD WORD PTR [ESI+0x7D2], 1
	PatchSkeleton{ mokouPOSSESS_3_patchByteCode,               (void *)0x672AAF,  8}, // Declared line 2090. Patch on JNE SHORT 0x00672AEF
	PatchSkeleton{ mokouPOSSESS_A_patchByteCode,               (void *)0x683E48, 10}, // Declared line 2112. Patch on FLD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ mokouPOSSESS_END_patchByteCode,             (void *)0x683CF4,  6}, // Declared line 2128. Patch on JA 0x00680283
	PatchSkeleton{ mokouPOSSESS_4_patchByteCode,               (void *)0x683DE8,  6}, // Declared line 2141. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ mokouPOSSESS_FLASH_patchByteCode,           (void *)0x6733F6,  8}, // Declared line 2147. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ mokou3SCMETEOR_SOUND_patchByteCode,         (void *)0x6828BE,  7}, // Declared line 2164. Patch on CMP [ESI+0x13E], BX
};


static std::array<unsigned, 155> skeletonsForUpdate = {
	   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
	  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  28,  29,  30,  31,  34,
	  35,  36,  37,  41,  42,  43,  44,  48,  50,  52,  53,  54,  55,  56,  57,  58,
	  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
	  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  91,  92,  93,
	  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
	 126, 127, 128, 129, 130, 131, 132, 133, 135, 137, 138, 140, 141, 142, 146, 147,
	 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
	 165, 166, 167, 170, 171, 172, 173, 174, 181, 182, 186,
};
static std::array<unsigned, 1> skeletonsForInitializeAction = { 49 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 5> skeletonsForHandleInputs = { 38, 39, 40, 46, 47 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 27> skeletonsForObjectUpdate = {
	  27,  32,  33,  45,  51,  88,  89,  90, 134, 136, 139, 143, 144, 145, 149, 168,
	 169, 175, 176, 177, 178, 179, 180, 183, 184, 185, 187,
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
	printf("Internal memory used %zu/10455\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 10455);
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
