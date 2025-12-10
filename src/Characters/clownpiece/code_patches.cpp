#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<41480> applyMemory;
StackedMemory<17826> internalMemory;
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


// clownpieceID_FIX declared in clownpiece.asm line 5
static unsigned char clownpieceID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x00000017 ; line 6
	0x66, 0x39, 0x8E, 0x96, 0x01, 0x00, 0x00,                   // CMP [ESI+0x196],CX ; line 9
};
static PatchByteCode clownpieceID_FIX_patchByteCode = { clownpieceID_FIX_byteCode, 17, nullptr, 0 };
// clownpiece0 declared in clownpiece.asm line 14
static unsigned char clownpiece0_byteCode[] = {
	0x66, 0xC7, 0x86, 0x92, 0x08, 0x00, 0x00, 0xFF, 0x00, // MOV WORD PTR [ESI+0x892],0x00FF ; line 15
};
static PatchByteCode clownpiece0_patchByteCode = { clownpiece0_byteCode, 9, nullptr, 0 };
// clownpiece1 declared in clownpiece.asm line 20
static unsigned char clownpiece1_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x85882C] ; line 21
};
static PatchByteCode clownpiece1_patchByteCode = { clownpiece1_byteCode, 6, nullptr, 0 };
// clownpieceDASHJUMP declared in clownpiece.asm line 25
static unsigned char clownpieceDASHJUMP_byteCode[] = {
	0xD9, 0x05, 0x40, 0x8F, 0x85, 0x00, // FLD DWORD PTR [0x858F40] ; line 26
};
static PatchByteCode clownpieceDASHJUMP_patchByteCode = { clownpieceDASHJUMP_byteCode, 6, nullptr, 0 };
// clownpiece2_FLDZ declared in clownpiece.asm line 31
static unsigned char clownpiece2_FLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 32
};
static PatchByteCode clownpiece2_FLDZ_patchByteCode = { clownpiece2_FLDZ_byteCode, 2, nullptr, 0 };
// clownpiece2_FLD1 declared in clownpiece.asm line 34
static unsigned char clownpiece2_FLD1_byteCode[] = {
	0xD9, 0xE8, // FLD1 ; line 35
};
static PatchByteCode clownpiece2_FLD1_patchByteCode = { clownpiece2_FLD1_byteCode, 2, nullptr, 0 };
// clownpiece3 declared in clownpiece.asm line 40
static unsigned char clownpiece3_byteCode[] = {
	0xD9, 0x05, 0x30, 0xC2, 0x85, 0x00, // FLD DWORD PTR [0x85C230] ; line 41
};
static PatchByteCode clownpiece3_patchByteCode = { clownpiece3_byteCode, 6, nullptr, 0 };
// clownpiece4 declared in clownpiece.asm line 46
static unsigned char clownpiece4_byteCode[] = {
	0xC7, 0x86, 0x92, 0x08, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x892],0x000000FF ; line 47
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x621582 ; line 48
};
static JumpTarget clownpiece4_jumpData[] = {
	{ 0x00B, 0x621582 },
};
static PatchByteCode clownpiece4_patchByteCode = { clownpiece4_byteCode, 15, clownpiece4_jumpData, 1 };
// clownpiece5 declared in clownpiece.asm line 53
static unsigned char clownpiece5_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0F ; line 54
};
static PatchByteCode clownpiece5_patchByteCode = { clownpiece5_byteCode, 10, nullptr, 0 };
// clownpiece6_FLD15 declared in clownpiece.asm line 59
static unsigned char clownpiece6_FLD15_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 60
};
static PatchByteCode clownpiece6_FLD15_patchByteCode = { clownpiece6_FLD15_byteCode, 6, nullptr, 0 };
// clownpiece7 declared in clownpiece.asm line 68
static unsigned char clownpiece7_byteCode[] = {
	0x66, 0xC7, 0x86, 0x92, 0x08, 0x00, 0x00, 0xFF, 0x00, // MOV WORD PTR [ESI+0x892],0x00FF ; line 69
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xFF,             // MOV BYTE PTR [ESI+0x113],0xFF ; line 70
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62109D ; line 71
};
static JumpTarget clownpiece7_jumpData[] = {
	{ 0x011, 0x62109D },
};
static PatchByteCode clownpiece7_patchByteCode = { clownpiece7_byteCode, 21, clownpiece7_jumpData, 1 };
// clownpiece8 declared in clownpiece.asm line 76
static unsigned char clownpiece8_byteCode[] = {
	0xDC, 0x25, 0xC8, 0x77, 0x86, 0x00, // FSUB QWORD PTR [0x8677C8] ; line 77
};
static PatchByteCode clownpiece8_patchByteCode = { clownpiece8_byteCode, 6, nullptr, 0 };
// clownpiece9 declared in clownpiece.asm line 82
static unsigned char clownpiece9_byteCode[] = {
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x0 ; line 83
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x621176 ; line 84
};
static JumpTarget clownpiece9_jumpData[] = {
	{ 0x00B, 0x621176 },
};
static PatchByteCode clownpiece9_patchByteCode = { clownpiece9_byteCode, 15, clownpiece9_jumpData, 1 };
// clownpiece10 declared in clownpiece.asm line 89
static unsigned char clownpiece10_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0E ; line 90
};
static PatchByteCode clownpiece10_patchByteCode = { clownpiece10_byteCode, 10, nullptr, 0 };
// clownpiece10A declared in clownpiece.asm line 98
static unsigned char clownpiece10A_byteCode[] = {
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x0 ; line 99
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6211F2 ; line 100
};
static JumpTarget clownpiece10A_jumpData[] = {
	{ 0x00B, 0x6211F2 },
};
static PatchByteCode clownpiece10A_patchByteCode = { clownpiece10A_byteCode, 15, clownpiece10A_jumpData, 1 };
// clownpiece11 declared in clownpiece.asm line 105
static unsigned char clownpiece11_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0C ; line 106
};
static PatchByteCode clownpiece11_patchByteCode = { clownpiece11_byteCode, 10, nullptr, 0 };
// clownpiece12 declared in clownpiece.asm line 111
static unsigned char clownpiece12_byteCode[] = {
	0xDC, 0x25, 0xB8, 0x81, 0x85, 0x00, // FSUB QWORD PTR [0x8581B8] ; line 112
};
static PatchByteCode clownpiece12_patchByteCode = { clownpiece12_byteCode, 6, nullptr, 0 };
// clownpiece13 declared in clownpiece.asm line 117
static unsigned char clownpiece13_byteCode[] = {
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867860] ; line 118
};
static PatchByteCode clownpiece13_patchByteCode = { clownpiece13_byteCode, 6, nullptr, 0 };
// clownpiece14 declared in clownpiece.asm line 123
static unsigned char clownpiece14_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 124
};
static PatchByteCode clownpiece14_patchByteCode = { clownpiece14_byteCode, 6, nullptr, 0 };
// clownpiece15 declared in clownpiece.asm line 129
static unsigned char clownpiece15_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 130
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x632773 ; line 131
};
static JumpTarget clownpiece15_jumpData[] = {
	{ 0x008, 0x632773 },
};
static PatchByteCode clownpiece15_patchByteCode = { clownpiece15_byteCode, 12, clownpiece15_jumpData, 1 };
// clownpiece16 declared in clownpiece.asm line 136
static unsigned char clownpiece16_byteCode[] = {
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xFF, // MOV BYTE PTR [ESI+0x113],0xFF ; line 137
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x624EED ; line 138
};
static JumpTarget clownpiece16_jumpData[] = {
	{ 0x008, 0x624EED },
};
static PatchByteCode clownpiece16_patchByteCode = { clownpiece16_byteCode, 12, clownpiece16_jumpData, 1 };
// clownpiece18 declared in clownpiece.asm line 149
static unsigned char clownpiece18_byteCode[] = {
	0x66, 0xC7, 0x86, 0x92, 0x08, 0x00, 0x00, 0xFF, 0x00, // MOV WORD PTR [ESI+0x892],0x00FF ; line 150
};
static PatchByteCode clownpiece18_patchByteCode = { clownpiece18_byteCode, 9, nullptr, 0 };
// clownpiece18A declared in clownpiece.asm line 155
static unsigned char clownpiece18A_byteCode[] = {
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x0 ; line 156
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x6281E7 ; line 157
};
static JumpTarget clownpiece18A_jumpData[] = {
	{ 0x00B, 0x6281E7 },
};
static PatchByteCode clownpiece18A_patchByteCode = { clownpiece18A_byteCode, 15, clownpiece18A_jumpData, 1 };
// clownpiece19_NOP declared in clownpiece.asm line 162
static unsigned char clownpiece19_NOP_byteCode[] = {
	0x90, // NOP ; line 163
};
static PatchByteCode clownpiece19_NOP_patchByteCode = { clownpiece19_NOP_byteCode, 1, nullptr, 0 };
// clownpiece19_MULZERO declared in clownpiece.asm line 165
static unsigned char clownpiece19_MULZERO_byteCode[] = {
	0xDC, 0x0D, 0x2C, 0x81, 0x86, 0x00, // FMUL QWORD PTR [0x86812C] ; line 166
};
static PatchByteCode clownpiece19_MULZERO_patchByteCode = { clownpiece19_MULZERO_byteCode, 6, nullptr, 0 };
// clownpiece20 declared in clownpiece.asm line 171
static unsigned char clownpiece20_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x78 ; line 172
};
static PatchByteCode clownpiece20_patchByteCode = { clownpiece20_byteCode, 10, nullptr, 0 };
// clownpiece21_Cspd declared in clownpiece.asm line 182
static unsigned char clownpiece21_Cspd_byteCode[] = {
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871538] ; line 183
};
static PatchByteCode clownpiece21_Cspd_patchByteCode = { clownpiece21_Cspd_byteCode, 6, nullptr, 0 };
// clownpiece22 declared in clownpiece.asm line 203
static unsigned char clownpiece22_byteCode[] = {
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00, // FSUB DWORD PTR [0x85AE94] ; line 204
};
static PatchByteCode clownpiece22_patchByteCode = { clownpiece22_byteCode, 6, nullptr, 0 };
// clownpiece23_FLD12 declared in clownpiece.asm line 209
static unsigned char clownpiece23_FLD12_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 210
};
static PatchByteCode clownpiece23_FLD12_patchByteCode = { clownpiece23_FLD12_byteCode, 6, nullptr, 0 };
// clownpiece5B_NEW0 declared in clownpiece.asm line 214
static unsigned char clownpiece5B_NEW0_byteCode[] = {
	0x66, 0xC7, 0x86, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // MOV WORD PTR [ESI+0x7D2],0x02 ; line 216
};
static PatchByteCode clownpiece5B_NEW0_patchByteCode = { clownpiece5B_NEW0_byteCode, 9, nullptr, 0 };
// clownpiece5B_NEW1 declared in clownpiece.asm line 219
static unsigned char clownpiece5B_NEW1_byteCode[] = {
	0x8B, 0x86, 0x70, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x170] ; line 220
	0x0F, 0xBE, 0x96, 0x04, 0x01, 0x00, 0x00, // MOVSX EDX,BYTE PTR [ESI+0x104] ; line 221
};
static PatchByteCode clownpiece5B_NEW1_patchByteCode = { clownpiece5B_NEW1_byteCode, 13, nullptr, 0 };
// clownpiece5B_NEW1B declared in clownpiece.asm line 224
static unsigned char clownpiece5B_NEW1B_byteCode[] = {
	0x8B, 0x86, 0x70, 0x01, 0x00, 0x00,       // MOV EAX,[ESI+0x170] ; line 225
	0x0F, 0xBE, 0x8E, 0x04, 0x01, 0x00, 0x00, // MOVSX ECX,BYTE PTR [ESI+0x104] ; line 226
};
static PatchByteCode clownpiece5B_NEW1B_patchByteCode = { clownpiece5B_NEW1B_byteCode, 13, nullptr, 0 };
// clownpiece5B_RNG0 declared in clownpiece.asm line 234
static unsigned char clownpiece5B_RNG0_byteCode[] = {
	0x6A, 0x11,                                           // PUSH 0x11 ; line 235
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x4099F0 ; line 236
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 238
	0x0F, 0x84, 0x23, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 239
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 240
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 241
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 242
	0x0F, 0x84, 0x19, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 243
	0xE9, 0x1E, 0x00, 0x00, 0x00,                         // JMP LABEL4 ; line 244
	0xB8, 0x01, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000001 ; line 246
	0xE9, 0x19, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 247
	0xB8, 0x02, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000002 ; line 249
	0xE9, 0x0F, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 250
	0xB8, 0x03, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000003 ; line 252
	0xE9, 0x05, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 253
	0xB8, 0x04, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000004 ; line 255
	0xBA, 0x04, 0x00, 0x00, 0x00,                         // MOV EDX,0x00000004 ; line 257
	0xF7, 0xEA,                                           // IMUL EDX ; line 258
	0x89, 0xC2,                                           // MOV EDX,EAX ; line 259
};
static JumpTarget clownpiece5B_RNG0_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode clownpiece5B_RNG0_patchByteCode = { clownpiece5B_RNG0_byteCode, 101, clownpiece5B_RNG0_jumpData, 1 };
// clownpiece5B_2B declared in clownpiece.asm line 262
static unsigned char clownpiece5B_2B_byteCode[] = {
	0x6A, 0x11,                                           // PUSH 0x11 ; line 263
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x4099F0 ; line 264
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 266
	0x0F, 0x84, 0x23, 0x00, 0x00, 0x00,                   // JE SHORT LABEL1 ; line 267
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x7D2],0x01 ; line 268
	0x0F, 0x84, 0x1E, 0x00, 0x00, 0x00,                   // JE SHORT LABEL2 ; line 269
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x7D2],0x02 ; line 270
	0x0F, 0x84, 0x19, 0x00, 0x00, 0x00,                   // JE SHORT LABEL3 ; line 271
	0xE9, 0x1E, 0x00, 0x00, 0x00,                         // JMP LABEL4 ; line 272
	0xB8, 0x01, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000001 ; line 274
	0xE9, 0x19, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 275
	0xB8, 0x02, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000002 ; line 277
	0xE9, 0x0F, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 278
	0xB8, 0x03, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000003 ; line 280
	0xE9, 0x05, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL5 ; line 281
	0xB8, 0x04, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000004 ; line 283
	0xBA, 0x04, 0x00, 0x00, 0x00,                         // MOV EDX,0x00000004 ; line 285
	0xF7, 0xEA,                                           // IMUL EDX ; line 286
	0x89, 0xC2,                                           // MOV EDX,EAX ; line 287
	0x81, 0xC4, 0x04, 0x00, 0x00, 0x00,                   // ADD ESP,0x04 ; line 290
	0x85, 0xC0,                                           // TEST EAX,EAX ; line 291
};
static JumpTarget clownpiece5B_2B_jumpData[] = {
	{ 0x003, 0x4099F0 },
};
static PatchByteCode clownpiece5B_2B_patchByteCode = { clownpiece5B_2B_byteCode, 109, clownpiece5B_2B_jumpData, 1 };
// clownpiece5B_RNG1 declared in clownpiece.asm line 294
static unsigned char clownpiece5B_RNG1_byteCode[] = {
	0xD8, 0x25, 0x2C, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x86782C] ; line 295
};
static PatchByteCode clownpiece5B_RNG1_patchByteCode = { clownpiece5B_RNG1_byteCode, 6, nullptr, 0 };
// clownpieceC5B_3 declared in clownpiece.asm line 298
static unsigned char clownpieceC5B_3_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x63120A ; line 299
};
static JumpTarget clownpieceC5B_3_jumpData[] = {
	{ 0x001, 0x63120A },
};
static PatchByteCode clownpieceC5B_3_patchByteCode = { clownpieceC5B_3_byteCode, 5, clownpieceC5B_3_jumpData, 1 };
// clownpieceC5B_3B declared in clownpiece.asm line 301
static unsigned char clownpieceC5B_3B_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x624D6A ; line 302
};
static JumpTarget clownpieceC5B_3B_jumpData[] = {
	{ 0x001, 0x624D6A },
};
static PatchByteCode clownpieceC5B_3B_patchByteCode = { clownpieceC5B_3B_byteCode, 5, clownpieceC5B_3B_jumpData, 1 };
// clownpiece5B_NEW2_A declared in clownpiece.asm line 307
static unsigned char clownpiece5B_NEW2_A_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 308
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 309
	0x66, 0x9C,                                           // PUSHF ; line 310
	0x60,                                                 // PUSHAD ; line 311
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 312
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 313
	0x61,                                                 // POPAD ; line 314
	0x66, 0x9D,                                           // POPF ; line 315
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x622383 ; line 316
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 318
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 319
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 320
	0x61,                                                 // POPAD ; line 321
	0x66, 0x9D,                                           // POPF ; line 322
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6221E2 ; line 323
};
static JumpTarget clownpiece5B_NEW2_A_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x622383 },
	{ 0x03B, 0x6221E2 },
};
static PatchByteCode clownpiece5B_NEW2_A_patchByteCode = { clownpiece5B_NEW2_A_byteCode, 63, clownpiece5B_NEW2_A_jumpData, 3 };
// clownpiece5B_NEW2_B declared in clownpiece.asm line 326
static unsigned char clownpiece5B_NEW2_B_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 327
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 328
	0x66, 0x9C,                                           // PUSHF ; line 329
	0x60,                                                 // PUSHAD ; line 330
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 331
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 332
	0x61,                                                 // POPAD ; line 333
	0x66, 0x9D,                                           // POPF ; line 334
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62272A ; line 335
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 337
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 338
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 339
	0x61,                                                 // POPAD ; line 340
	0x66, 0x9D,                                           // POPF ; line 341
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6225F7 ; line 342
};
static JumpTarget clownpiece5B_NEW2_B_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x62272A },
	{ 0x03B, 0x6225F7 },
};
static PatchByteCode clownpiece5B_NEW2_B_patchByteCode = { clownpiece5B_NEW2_B_byteCode, 63, clownpiece5B_NEW2_B_jumpData, 3 };
// clownpiece5B_NEW2_C declared in clownpiece.asm line 345
static unsigned char clownpiece5B_NEW2_C_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 346
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 347
	0x66, 0x9C,                                           // PUSHF ; line 348
	0x60,                                                 // PUSHAD ; line 349
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 350
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 351
	0x61,                                                 // POPAD ; line 352
	0x66, 0x9D,                                           // POPF ; line 353
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x622BE9 ; line 354
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 356
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 357
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 358
	0x61,                                                 // POPAD ; line 359
	0x66, 0x9D,                                           // POPF ; line 360
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x622AAC ; line 361
};
static JumpTarget clownpiece5B_NEW2_C_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x622BE9 },
	{ 0x03B, 0x622AAC },
};
static PatchByteCode clownpiece5B_NEW2_C_patchByteCode = { clownpiece5B_NEW2_C_byteCode, 63, clownpiece5B_NEW2_C_jumpData, 3 };
// clownpiece5B_NEW2_D declared in clownpiece.asm line 364
static unsigned char clownpiece5B_NEW2_D_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 365
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 366
	0x66, 0x9C,                                           // PUSHF ; line 367
	0x60,                                                 // PUSHAD ; line 368
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 369
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 370
	0x61,                                                 // POPAD ; line 371
	0x66, 0x9D,                                           // POPF ; line 372
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x622FF0 ; line 373
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 375
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 376
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 377
	0x61,                                                 // POPAD ; line 378
	0x66, 0x9D,                                           // POPF ; line 379
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x622EBF ; line 380
};
static JumpTarget clownpiece5B_NEW2_D_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x622FF0 },
	{ 0x03B, 0x622EBF },
};
static PatchByteCode clownpiece5B_NEW2_D_patchByteCode = { clownpiece5B_NEW2_D_byteCode, 63, clownpiece5B_NEW2_D_jumpData, 3 };
// clownpiece5B_NEW2_E declared in clownpiece.asm line 383
static unsigned char clownpiece5B_NEW2_E_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 384
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 385
	0x66, 0x9C,                                           // PUSHF ; line 386
	0x60,                                                 // PUSHAD ; line 387
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 388
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 389
	0x61,                                                 // POPAD ; line 390
	0x66, 0x9D,                                           // POPF ; line 391
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623413 ; line 392
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 394
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 395
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 396
	0x61,                                                 // POPAD ; line 397
	0x66, 0x9D,                                           // POPF ; line 398
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623374 ; line 399
};
static JumpTarget clownpiece5B_NEW2_E_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x623413 },
	{ 0x03B, 0x623374 },
};
static PatchByteCode clownpiece5B_NEW2_E_patchByteCode = { clownpiece5B_NEW2_E_byteCode, 63, clownpiece5B_NEW2_E_jumpData, 3 };
// clownpiece5B_NEW2_F declared in clownpiece.asm line 402
static unsigned char clownpiece5B_NEW2_F_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 403
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 404
	0x66, 0x9C,                                           // PUSHF ; line 405
	0x60,                                                 // PUSHAD ; line 406
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 407
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 408
	0x61,                                                 // POPAD ; line 409
	0x66, 0x9D,                                           // POPF ; line 410
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623780 ; line 411
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 413
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 414
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 415
	0x61,                                                 // POPAD ; line 416
	0x66, 0x9D,                                           // POPF ; line 417
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x6236EA ; line 418
};
static JumpTarget clownpiece5B_NEW2_F_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x623780 },
	{ 0x03B, 0x6236EA },
};
static PatchByteCode clownpiece5B_NEW2_F_patchByteCode = { clownpiece5B_NEW2_F_byteCode, 63, clownpiece5B_NEW2_F_jumpData, 3 };
// clownpiece5B_NEW2_G declared in clownpiece.asm line 421
static unsigned char clownpiece5B_NEW2_G_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 422
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 423
	0x66, 0x9C,                                           // PUSHF ; line 424
	0x60,                                                 // PUSHAD ; line 425
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 426
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 427
	0x61,                                                 // POPAD ; line 428
	0x66, 0x9D,                                           // POPF ; line 429
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623CD1 ; line 430
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 432
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 433
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 434
	0x61,                                                 // POPAD ; line 435
	0x66, 0x9D,                                           // POPF ; line 436
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623B92 ; line 437
};
static JumpTarget clownpiece5B_NEW2_G_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x623CD1 },
	{ 0x03B, 0x623B92 },
};
static PatchByteCode clownpiece5B_NEW2_G_patchByteCode = { clownpiece5B_NEW2_G_byteCode, 63, clownpiece5B_NEW2_G_jumpData, 3 };
// clownpiece5B_NEW2_H declared in clownpiece.asm line 440
static unsigned char clownpiece5B_NEW2_H_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 441
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 442
	0x66, 0x9C,                                           // PUSHF ; line 443
	0x60,                                                 // PUSHAD ; line 444
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 445
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 446
	0x61,                                                 // POPAD ; line 447
	0x66, 0x9D,                                           // POPF ; line 448
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x624105 ; line 449
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 451
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 452
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 453
	0x61,                                                 // POPAD ; line 454
	0x66, 0x9D,                                           // POPF ; line 455
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x623FC6 ; line 456
};
static JumpTarget clownpiece5B_NEW2_H_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x624105 },
	{ 0x03B, 0x623FC6 },
};
static PatchByteCode clownpiece5B_NEW2_H_patchByteCode = { clownpiece5B_NEW2_H_byteCode, 63, clownpiece5B_NEW2_H_jumpData, 3 };
// clownpiece5B_NEW2_I declared in clownpiece.asm line 459
static unsigned char clownpiece5B_NEW2_I_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 460
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 461
	0x66, 0x9C,                                           // PUSHF ; line 462
	0x60,                                                 // PUSHAD ; line 463
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 464
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 465
	0x61,                                                 // POPAD ; line 466
	0x66, 0x9D,                                           // POPF ; line 467
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62469C ; line 468
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 470
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 471
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 472
	0x61,                                                 // POPAD ; line 473
	0x66, 0x9D,                                           // POPF ; line 474
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62454F ; line 475
};
static JumpTarget clownpiece5B_NEW2_I_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x62469C },
	{ 0x03B, 0x62454F },
};
static PatchByteCode clownpiece5B_NEW2_I_patchByteCode = { clownpiece5B_NEW2_I_byteCode, 63, clownpiece5B_NEW2_I_jumpData, 3 };
// clownpiece5B_NEW2_J declared in clownpiece.asm line 478
static unsigned char clownpiece5B_NEW2_J_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00,                         // PUSH 0x00000320 ; line 479
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 480
	0x66, 0x9C,                                           // PUSHF ; line 481
	0x60,                                                 // PUSHAD ; line 482
	0x66, 0x81, 0xBE, 0xD2, 0x07, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x7D2],0x00 ; line 483
	0x0F, 0x8F, 0x08, 0x00, 0x00, 0x00,                   // JG SHORT LABEL1 ; line 484
	0x61,                                                 // POPAD ; line 485
	0x66, 0x9D,                                           // POPF ; line 486
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x624AEA ; line 487
	0x66, 0x8B, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV AX,[ESI+0x7D2] ; line 489
	0x66, 0x81, 0xE8, 0x01, 0x00,                         // SUB AX,0x01 ; line 490
	0x66, 0x89, 0x86, 0xD2, 0x07, 0x00, 0x00,             // MOV [ESI+0x7D2],AX ; line 491
	0x61,                                                 // POPAD ; line 492
	0x66, 0x9D,                                           // POPF ; line 493
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62499D ; line 494
};
static JumpTarget clownpiece5B_NEW2_J_jumpData[] = {
	{ 0x006, 0x46EB30 },
	{ 0x020, 0x624AEA },
	{ 0x03B, 0x62499D },
};
static PatchByteCode clownpiece5B_NEW2_J_patchByteCode = { clownpiece5B_NEW2_J_byteCode, 63, clownpiece5B_NEW2_J_jumpData, 3 };
// clownpiece5B_GRAZERES declared in clownpiece.asm line 564
static unsigned char clownpiece5B_GRAZERES_byteCode[] = {
	0x6A, 0x01,                   // PUSH 0x01 ; line 565
	0x89, 0xF1,                   // MOV ECX,ESI ; line 566
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x48CE30 ; line 567
};
static JumpTarget clownpiece5B_GRAZERES_jumpData[] = {
	{ 0x005, 0x48CE30 },
};
static PatchByteCode clownpiece5B_GRAZERES_patchByteCode = { clownpiece5B_GRAZERES_byteCode, 9, clownpiece5B_GRAZERES_jumpData, 1 };
// clownpiece24A declared in clownpiece.asm line 571
static unsigned char clownpiece24A_byteCode[] = {
	0x66, 0x81, 0xF8, 0xF7, 0x00,       // CMP AX,0x00F7 ; line 572
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x622596 ; line 573
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x6221C2 ; line 574
};
static JumpTarget clownpiece24A_jumpData[] = {
	{ 0x007, 0x622596 },
	{ 0x00C, 0x6221C2 },
};
static PatchByteCode clownpiece24A_patchByteCode = { clownpiece24A_byteCode, 16, clownpiece24A_jumpData, 2 };
// clownpiece24B declared in clownpiece.asm line 576
static unsigned char clownpiece24B_byteCode[] = {
	0x66, 0x81, 0xF8, 0xF7, 0x00,       // CMP AX,0x00F7 ; line 577
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x622E5E ; line 578
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x622A8C ; line 579
};
static JumpTarget clownpiece24B_jumpData[] = {
	{ 0x007, 0x622E5E },
	{ 0x00C, 0x622A8C },
};
static PatchByteCode clownpiece24B_patchByteCode = { clownpiece24B_byteCode, 16, clownpiece24B_jumpData, 2 };
// clownpiece24C declared in clownpiece.asm line 581
static unsigned char clownpiece24C_byteCode[] = {
	0x66, 0x81, 0xF8, 0xF7, 0x00,       // CMP AX,0x00F7 ; line 582
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x623688 ; line 583
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x623353 ; line 584
};
static JumpTarget clownpiece24C_jumpData[] = {
	{ 0x007, 0x623688 },
	{ 0x00C, 0x623353 },
};
static PatchByteCode clownpiece24C_patchByteCode = { clownpiece24C_byteCode, 16, clownpiece24C_jumpData, 2 };
// clownpiece24D declared in clownpiece.asm line 586
static unsigned char clownpiece24D_byteCode[] = {
	0x66, 0x81, 0xF8, 0xF7, 0x00,       // CMP AX,0x00F7 ; line 587
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x623F45 ; line 588
};
static JumpTarget clownpiece24D_jumpData[] = {
	{ 0x007, 0x623F45 },
};
static PatchByteCode clownpiece24D_patchByteCode = { clownpiece24D_byteCode, 11, clownpiece24D_jumpData, 1 };
// clownpiece25 declared in clownpiece.asm line 590
static unsigned char clownpiece25_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x62491C ; line 591
};
static JumpTarget clownpiece25_jumpData[] = {
	{ 0x001, 0x62491C },
};
static PatchByteCode clownpiece25_patchByteCode = { clownpiece25_byteCode, 5, clownpiece25_jumpData, 1 };
// clownpiece26_Bframe declared in clownpiece.asm line 607
static unsigned char clownpiece26_Bframe_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 608
};
static PatchByteCode clownpiece26_Bframe_patchByteCode = { clownpiece26_Bframe_byteCode, 9, nullptr, 0 };
// clownpiece26_Bframe1 declared in clownpiece.asm line 610
static unsigned char clownpiece26_Bframe1_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x03 ; line 611
};
static PatchByteCode clownpiece26_Bframe1_patchByteCode = { clownpiece26_Bframe1_byteCode, 9, nullptr, 0 };
// clownpiece27 declared in clownpiece.asm line 632
static unsigned char clownpiece27_byteCode[] = {
	0xD9, 0x05, 0xB8, 0xBA, 0x85, 0x00, // FLD DWORD PTR [0x85BAB8] ; line 633
};
static PatchByteCode clownpiece27_patchByteCode = { clownpiece27_byteCode, 6, nullptr, 0 };
// clownpiece28 declared in clownpiece.asm line 638
static unsigned char clownpiece28_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x78 ; line 639
};
static PatchByteCode clownpiece28_patchByteCode = { clownpiece28_byteCode, 10, nullptr, 0 };
// clownpiece29 declared in clownpiece.asm line 644
static unsigned char clownpiece29_byteCode[] = {
	0xDC, 0x25, 0x90, 0x79, 0x85, 0x00, // FSUB QWORD PTR [0x857990] ; line 645
};
static PatchByteCode clownpiece29_patchByteCode = { clownpiece29_byteCode, 6, nullptr, 0 };
// clownpieceA236DROP_COUNT declared in clownpiece.asm line 649
static unsigned char clownpieceA236DROP_COUNT_byteCode[] = {
	0xD9, 0x05, 0x04, 0xC0, 0x85, 0x00, // FLD DWORD PTR [0x85C004] ; line 650
};
static PatchByteCode clownpieceA236DROP_COUNT_patchByteCode = { clownpieceA236DROP_COUNT_byteCode, 6, nullptr, 0 };
// clownpiece30 declared in clownpiece.asm line 665
static unsigned char clownpiece30_byteCode[] = {
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867860] ; line 666
};
static PatchByteCode clownpiece30_patchByteCode = { clownpiece30_byteCode, 6, nullptr, 0 };
// clownpiece31 declared in clownpiece.asm line 671
static unsigned char clownpiece31_byteCode[] = {
	0xDD, 0x05, 0xC0, 0xBA, 0x85, 0x00, // FLD QWORD PTR [0x85BAC0] ; line 672
};
static PatchByteCode clownpiece31_patchByteCode = { clownpiece31_byteCode, 6, nullptr, 0 };
// clownpiece32 declared in clownpiece.asm line 677
static unsigned char clownpiece32_byteCode[] = {
	0xD8, 0x25, 0x94, 0xAE, 0x85, 0x00, // FSUB DWORD PTR [0x85AE94] ; line 678
};
static PatchByteCode clownpiece32_patchByteCode = { clownpiece32_byteCode, 6, nullptr, 0 };
// clownpiece33 declared in clownpiece.asm line 683
static unsigned char clownpiece33_byteCode[] = {
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00, // FLD DWORD PTR [0x85C610] ; line 684
};
static PatchByteCode clownpiece33_patchByteCode = { clownpiece33_byteCode, 6, nullptr, 0 };
// clownpiece34 declared in clownpiece.asm line 689
static unsigned char clownpiece34_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xAB, 0x85, 0x00, // FLD DWORD PTR [0x85ABE4] ; line 690
};
static PatchByteCode clownpiece34_patchByteCode = { clownpiece34_byteCode, 6, nullptr, 0 };
// clownpieceD236_SKIP declared in clownpiece.asm line 697
static unsigned char clownpieceD236_SKIP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x637B0F ; line 698
};
static JumpTarget clownpieceD236_SKIP_jumpData[] = {
	{ 0x001, 0x637B0F },
};
static PatchByteCode clownpieceD236_SKIP_patchByteCode = { clownpieceD236_SKIP_byteCode, 5, clownpieceD236_SKIP_jumpData, 1 };
// clownpieceD623_0 declared in clownpiece.asm line 700
static unsigned char clownpieceD623_0_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 701
	0x60,                                                       // PUSHAD ; line 702
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 703
	0x90,                                                       // NOP ; line 704
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 705
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 706
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 707
	0x90,                                                       // NOP ; line 708
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 712
	0x0F, 0xB7, 0x80, 0x3C, 0x01, 0x00, 0x00,                   // MOVZX EAX, WORD PTR [EAX+0x13C] ; line 713
	0x66, 0x81, 0xF8, 0x31, 0x00,                               // CMP AX,0x0031 ; line 714
	0x0F, 0x8E, 0x0B, 0x00, 0x00, 0x00,                         // JLE SHORT LABEL2 ; line 715
	0x66, 0x81, 0xF8, 0x96, 0x00,                               // CMP AX,0x0096 ; line 716
	0x0F, 0x8C, 0x1D, 0x00, 0x00, 0x00,                         // JL SHORT LABEL3 ; line 717
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 720
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 721
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x08 ; line 722
	0x0F, 0x85, 0x00, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 723
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 725
	0x90,                                                       // NOP ; line 726
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, // MOV DWORD PTR [ESI+0x110],0x55555555 ; line 727
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 729
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 730
	0x61,                                                       // POPAD ; line 731
	0x66, 0x9D,                                                 // POPF ; line 732
};
static PatchByteCode clownpieceD623_0_patchByteCode = { clownpieceD623_0_byteCode, 139, nullptr, 0 };
// clownpieceD623_1 declared in clownpiece.asm line 742
static unsigned char clownpieceD623_1_byteCode[] = {
	0xB9, 0x10, 0x00, 0x00, 0x00, // MOV ECX,0x00000010 ; line 743
};
static PatchByteCode clownpieceD623_1_patchByteCode = { clownpieceD623_1_byteCode, 5, nullptr, 0 };
// clownpieceD623B_LV declared in clownpiece.asm line 748
static unsigned char clownpieceD623B_LV_byteCode[] = {
	0xD8, 0x0D, 0x00, 0x9F, 0x85, 0x00, // FMUL DWORD PTR [0x859F00] ; line 749
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B984] ; line 750
};
static PatchByteCode clownpieceD623B_LV_patchByteCode = { clownpieceD623B_LV_byteCode, 12, nullptr, 0 };
// clownpieceD623C_LV declared in clownpiece.asm line 755
static unsigned char clownpieceD623C_LV_byteCode[] = {
	0xD8, 0x0D, 0x00, 0x9F, 0x85, 0x00, // FMUL DWORD PTR [0x859F00] ; line 756
};
static PatchByteCode clownpieceD623C_LV_patchByteCode = { clownpieceD623C_LV_byteCode, 6, nullptr, 0 };
// clownpieceD623_STACK declared in clownpiece.asm line 762
static unsigned char clownpieceD623_STACK_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 763
	0x60,                                                       // PUSHAD ; line 764
	0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x34, 0x03, 0x01, 0x00, // CMP DWORD PTR [ESI+0x13C],0x00010334 ; line 765
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 766
	0xE9, 0x47, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 768
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 770
	0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x1C, 0x02, 0x00, 0x00, // CMP DWORD PTR [EAX+0x13C],0x0000021C ; line 771
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 772
	0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x1D, 0x02, 0x00, 0x00, // CMP DWORD PTR [EAX+0x13C],0x0000021D ; line 773
	0x0F, 0x85, 0xD5, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 774
	0x81, 0xB8, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [EAX+0x142],0x00000000 ; line 776
	0x90,                                                       // NOP ; line 777
	0x0F, 0x85, 0xC4, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 778
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x000000FF ; line 779
	0xE9, 0xB5, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL4 ; line 780
	0x90,                                                       // NOP ; line 781
	0x61,                                                       // POPAD ; line 783
	0x66, 0x9D,                                                 // POPF ; line 784
	0xBB, 0x03, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000003 ; line 785
};
static PatchByteCode clownpieceD623_STACK_patchByteCode = { clownpieceD623_STACK_byteCode, 103, nullptr, 0 };
// clownpieceMECH declared in clownpiece.asm line 792
static unsigned char clownpieceMECH_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 793
	0x60,                                                       // PUSHAD ; line 794
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 795
	0x85, 0xC0,                                                 // TEST EAX,EAX ; line 796
	0x0F, 0x84, 0x3D, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 797
	0x8B, 0x90, 0x70, 0x01, 0x00, 0x00,                         // MOV EDX,[EAX+0x170] ; line 798
	0x85, 0xD2,                                                 // TEST EDX,EDX ; line 799
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 800
	0x66, 0x81, 0xB8, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x184],0x00 ; line 801
	0x0F, 0x8E, 0x0F, 0x00, 0x00, 0x00,                         // JLE SHORT LABELB ; line 802
	0x66, 0x81, 0xBA, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EDX+0x184],0x00 ; line 803
	0x0F, 0x8F, 0x11, 0x00, 0x00, 0x00,                         // JG SHORT LABELA ; line 804
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 806
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 807
	0x61,                                                       // POPAD ; line 809
	0x66, 0x9D,                                                 // POPF ; line 810
	0x90,                                                       // NOP ; line 811
	0x90,                                                       // NOP ; line 812
	0x90,                                                       // NOP ; line 813
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x28 ; line 814
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 815
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // CMP DWORD PTR [ESI+0x110],0xFFFFFFFF ; line 816
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 817
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 818
	0x90,                                                       // NOP ; line 819
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 820
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x04 ; line 822
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 823
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x08 ; line 824
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 825
	0xE9, 0x40, 0x00, 0x00, 0x00,                               // JMP SHORT EXIT ; line 827
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 829
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 830
	0x81, 0xAE, 0x10, 0x01, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, // SUB DWORD PTR [ESI+0x110],0x11111111 ; line 831
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x110],0x00000000 ; line 833
	0x0F, 0x84, 0x07, 0x00, 0x00, 0x00,                         // JE SHORT LABEL5 ; line 834
	0x39, 0xC0,                                                 // CMP EAX,EAX ; line 835
	0xE9, 0x0A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL6 ; line 836
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0xDD, 0xFF, 0xFF, 0xFF, // CMP DWORD PTR [ESI+0x110],0xFFFFFFDD ; line 838
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP SHORT EXIT ; line 840
	0x90,                                                       // NOP ; line 842
};
static PatchByteCode clownpieceMECH_patchByteCode = { clownpieceMECH_byteCode, 236, nullptr, 0 };
// clownpieceMECH2 declared in clownpiece.asm line 845
static unsigned char clownpieceMECH2_byteCode[] = {
	0x60,                                                       // PUSHAD ; line 846
	0x66, 0x9C,                                                 // PUSHF ; line 847
	0x53,                                                       // PUSH EBX ; line 848
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 849
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CDE0 ; line 850
	0x66, 0x9D,                                                 // POPF ; line 851
	0x61,                                                       // POPAD ; line 852
	0x90,                                                       // NOP ; line 853
	0x66, 0x9C,                                                 // PUSHF ; line 854
	0x60,                                                       // PUSHAD ; line 855
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 856
	0x85, 0xC0,                                                 // TEST EAX,EAX ; line 857
	0x0F, 0x84, 0x3D, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 858
	0x8B, 0x90, 0x70, 0x01, 0x00, 0x00,                         // MOV EDX,[EAX+0x170] ; line 859
	0x85, 0xD2,                                                 // TEST EDX,EDX ; line 860
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 861
	0x66, 0x81, 0xB8, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x184],0x00 ; line 862
	0x0F, 0x8E, 0x0F, 0x00, 0x00, 0x00,                         // JLE SHORT LABELB ; line 863
	0x66, 0x81, 0xBA, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EDX+0x184],0x00 ; line 864
	0x0F, 0x8F, 0x11, 0x00, 0x00, 0x00,                         // JG SHORT LABELA ; line 865
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 867
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 868
	0x61,                                                       // POPAD ; line 870
	0x66, 0x9D,                                                 // POPF ; line 871
	0x90,                                                       // NOP ; line 872
	0x90,                                                       // NOP ; line 873
	0x90,                                                       // NOP ; line 874
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x28 ; line 875
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 876
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, // CMP DWORD PTR [ESI+0x110],0xFFFFFFFF ; line 877
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 878
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 879
	0x90,                                                       // NOP ; line 880
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 881
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x04 ; line 883
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 884
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x08 ; line 885
	0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 886
	0xE9, 0x40, 0x00, 0x00, 0x00,                               // JMP EXIT ; line 888
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x00 ; line 890
	0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 891
	0x81, 0xAE, 0x10, 0x01, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, // SUB DWORD PTR [ESI+0x110],0x11111111 ; line 892
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x110],0x00000000 ; line 894
	0x0F, 0x84, 0x07, 0x00, 0x00, 0x00,                         // JE SHORT LABEL5 ; line 895
	0x39, 0xC0,                                                 // CMP EAX,EAX ; line 896
	0xE9, 0x0A, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL6 ; line 897
	0x81, 0xBE, 0x10, 0x01, 0x00, 0x00, 0xDD, 0xFF, 0xFF, 0xFF, // CMP DWORD PTR [ESI+0x110],0xFFFFFFDD ; line 899
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP SHORT EXIT ; line 901
	0x90,                                                       // NOP ; line 903
};
static JumpTarget clownpieceMECH2_jumpData[] = {
	{ 0x007, 0x48CDE0 },
};
static PatchByteCode clownpieceMECH2_patchByteCode = { clownpieceMECH2_byteCode, 251, clownpieceMECH2_jumpData, 1 };
// clownpiece5C_SKIP declared in clownpiece.asm line 921
static unsigned char clownpiece5C_SKIP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x636EE4 ; line 922
};
static JumpTarget clownpiece5C_SKIP_jumpData[] = {
	{ 0x001, 0x636EE4 },
};
static PatchByteCode clownpiece5C_SKIP_patchByteCode = { clownpiece5C_SKIP_byteCode, 5, clownpiece5C_SKIP_jumpData, 1 };
// clownpiece5C_SKIP2 declared in clownpiece.asm line 924
static unsigned char clownpiece5C_SKIP2_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x636CA9 ; line 925
};
static JumpTarget clownpiece5C_SKIP2_jumpData[] = {
	{ 0x002, 0x636CA9 },
};
static PatchByteCode clownpiece5C_SKIP2_patchByteCode = { clownpiece5C_SKIP2_byteCode, 6, clownpiece5C_SKIP2_jumpData, 1 };
// clownpieceA214 declared in clownpiece.asm line 934
static unsigned char clownpieceA214_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 935
	0x60,                                                       // PUSHAD ; line 936
	0xC6, 0x86, 0x8C, 0x01, 0x00, 0x00, 0x05,                   // MOV BYTE PTR [ESI+0x18C],0x05 ; line 937
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x0E, 0x02,       // CMP WORD PTR [ESI+0x13C],0x020E ; line 938
	0x0F, 0x8C, 0x0A, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 939
	0xBB, 0x00, 0x00, 0x09, 0x00,                               // MOV EBX,0x00090000 ; line 940
	0xE9, 0x05, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 941
	0xBB, 0x00, 0x00, 0x0B, 0x00,                               // MOV EBX,0x000B0000 ; line 943
	0x39, 0x9E, 0x3E, 0x01, 0x00, 0x00,                         // CMP DWORD PTR [ESI+0x13E],EBX ; line 945
	0x0F, 0x84, 0x20, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 946
	0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, // CMP DWORD PTR [ESI+0x13E],0x00030001 ; line 947
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 948
	0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, 0x07, 0x00, // CMP DWORD PTR [ESI+0x13E],0x00070001 ; line 949
	0x0F, 0x85, 0x20, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 950
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x0000 ; line 952
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 953
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 954
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 955
	0x61,                                                       // POPAD ; line 957
	0x66, 0x9D,                                                 // POPF ; line 958
	0xD9, 0xEB,                                                 // FLDPI ; line 959
	0xD8, 0xC8,                                                 // FMUL ST0,ST0 ; line 960
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 961
};
static PatchByteCode clownpieceA214_patchByteCode = { clownpieceA214_byteCode, 129, nullptr, 0 };
// clownpieceA214_MAXDUR declared in clownpiece.asm line 965
static unsigned char clownpieceA214_MAXDUR_byteCode[] = {
	0x60,                                     // PUSHAD ; line 966
	0x66, 0x9C,                               // PUSHF ; line 967
	0x0F, 0xB6, 0x96, 0xA9, 0x06, 0x00, 0x00, // MOVZX EDX,BYTE PTR [ESI+0x6A9] ; line 968
	0x66, 0x81, 0xC2, 0x03, 0x00,             // ADD DX,0x03 ; line 969
	0x6B, 0xD2, 0x0C,                         // IMUL DX,DX,0x0C ; line 970
	0x66, 0x9D,                               // POPF ; line 971
	0x66, 0x39, 0x96, 0x44, 0x01, 0x00, 0x00, // CMP WORD PTR [ESI+0x144],DX ; line 972
	0x61,                                     // POPAD ; line 973
};
static PatchByteCode clownpieceA214_MAXDUR_patchByteCode = { clownpieceA214_MAXDUR_byteCode, 28, nullptr, 0 };
// clownpieceSKIP_1 declared in clownpiece.asm line 977
static unsigned char clownpieceSKIP_1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x639959 ; line 978
};
static JumpTarget clownpieceSKIP_1_jumpData[] = {
	{ 0x001, 0x639959 },
};
static PatchByteCode clownpieceSKIP_1_patchByteCode = { clownpieceSKIP_1_byteCode, 5, clownpieceSKIP_1_jumpData, 1 };
// clownpieceA214_Y declared in clownpiece.asm line 982
static unsigned char clownpieceA214_Y_byteCode[] = {
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x11C] ; line 983
	0x81, 0xC4, 0x04, 0x00, 0x00, 0x00, // ADD ESP,0x04 ; line 984
	0x90,                               // NOP ; line 985
	0x66, 0x9C,                         // PUSHF ; line 986
	0x60,                               // PUSHAD ; line 987
	0xD9, 0x86, 0x2C, 0x01, 0x00, 0x00, // FLD DWORD PTR [ESI+0x12C] ; line 988
	0xD8, 0x05, 0x2C, 0x78, 0x86, 0x00, // FADD DWORD PTR [0x86782C] ; line 989
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 990
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00, // MOV EAX,[ESI+0x398] ; line 991
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [EAX+0xF0] ; line 992
	0xD8, 0x25, 0x84, 0xBB, 0x85, 0x00, // FSUB DWORD PTR [0x85BB84] ; line 993
	0xD9, 0x9E, 0xF0, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF0] ; line 994
	0x61,                               // POPAD ; line 995
	0x66, 0x9D,                         // POPF ; line 996
	0xD9, 0xE8,                         // FLD1 ; line 997
	0x90,                               // NOP ; line 998
	0xD9, 0x9E, 0x1C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x11C] ; line 999
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63987F ; line 1000
};
static JumpTarget clownpieceA214_Y_jumpData[] = {
	{ 0x047, 0x63987F },
};
static PatchByteCode clownpieceA214_Y_patchByteCode = { clownpieceA214_Y_byteCode, 75, clownpieceA214_Y_jumpData, 1 };
// clownpieceA214_ROTDISABLE declared in clownpiece.asm line 1021
static unsigned char clownpieceA214_ROTDISABLE_byteCode[] = {
	0xD9, 0x86, 0x2C, 0x01, 0x00, 0x00, // FLD DWORD PTR [ESI+0x12C] ; line 1022
};
static PatchByteCode clownpieceA214_ROTDISABLE_patchByteCode = { clownpieceA214_ROTDISABLE_byteCode, 6, nullptr, 0 };
// clownpieceA214_OFFSET declared in clownpiece.asm line 1026
static unsigned char clownpieceA214_OFFSET_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 1027
	0xD8, 0x81, 0xEC, 0x00, 0x00, 0x00, // FADD DWORD PTR [ECX+0xEC] ; line 1028
};
static PatchByteCode clownpieceA214_OFFSET_patchByteCode = { clownpieceA214_OFFSET_byteCode, 8, nullptr, 0 };
// clownpiece100 declared in clownpiece.asm line 1037
static unsigned char clownpiece100_byteCode[] = {
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871538] ; line 1038
};
static PatchByteCode clownpiece100_patchByteCode = { clownpiece100_byteCode, 6, nullptr, 0 };
// clownpiece101 declared in clownpiece.asm line 1046
static unsigned char clownpiece101_byteCode[] = {
	0xDC, 0x0D, 0x90, 0x89, 0x85, 0x00, // FMUL QWORD PTR [0x858990] ; line 1047
};
static PatchByteCode clownpiece101_patchByteCode = { clownpiece101_byteCode, 6, nullptr, 0 };
// clownpiece102 declared in clownpiece.asm line 1067
static unsigned char clownpiece102_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 1068
};
static PatchByteCode clownpiece102_patchByteCode = { clownpiece102_byteCode, 6, nullptr, 0 };
// clownpiece103 declared in clownpiece.asm line 1073
static unsigned char clownpiece103_byteCode[] = {
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871538] ; line 1074
};
static PatchByteCode clownpiece103_patchByteCode = { clownpiece103_byteCode, 6, nullptr, 0 };
// clownpiece104 declared in clownpiece.asm line 1079
static unsigned char clownpiece104_byteCode[] = {
	0xD9, 0x05, 0xE0, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B9E0] ; line 1080
};
static PatchByteCode clownpiece104_patchByteCode = { clownpiece104_byteCode, 6, nullptr, 0 };
// clownpiece105 declared in clownpiece.asm line 1093
static unsigned char clownpiece105_byteCode[] = {
	0xD9, 0x05, 0x38, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871538] ; line 1094
};
static PatchByteCode clownpiece105_patchByteCode = { clownpiece105_byteCode, 6, nullptr, 0 };
// clownpieceD214 declared in clownpiece.asm line 1100
static unsigned char clownpieceD214_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 1101
	0x60,                                                 // PUSHAD ; line 1102
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x14, 0x00, // CMP WORD PTR [ESI+0x140],0x0014 ; line 1103
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1104
	0x6A, 0x06,                                           // PUSH 0x06 ; line 1105
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 1106
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 1107
	0x61,                                                 // POPAD ; line 1109
	0x66, 0x9D,                                           // POPF ; line 1110
	0x0F, 0xBE, 0x8E, 0x04, 0x01, 0x00, 0x00,             // MOVSX ECX,BYTE PTR [ESI+0x104] ; line 1111
};
static JumpTarget clownpieceD214_jumpData[] = {
	{ 0x01B, 0x464980 },
};
static PatchByteCode clownpieceD214_patchByteCode = { clownpieceD214_byteCode, 41, clownpieceD214_jumpData, 1 };
// clownpieceD214_EQ declared in clownpiece.asm line 1122
static unsigned char clownpieceD214_EQ_byteCode[] = {
	0xDC, 0x05, 0xC8, 0xBB, 0x85, 0x00, // FADD QWORD PTR [0x85BBC8] ; line 1123
};
static PatchByteCode clownpieceD214_EQ_patchByteCode = { clownpieceD214_EQ_byteCode, 6, nullptr, 0 };
// clownpiece3SCSTAR declared in clownpiece.asm line 1134
static unsigned char clownpiece3SCSTAR_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1135
	0x60,                                                       // PUSHAD ; line 1136
	0x8B, 0x86, 0x44, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x144] ; line 1142
	0x31, 0xD2,                                                 // XOR EDX,EDX ; line 1143
	0xBB, 0x05, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000005 ; line 1144
	0xF7, 0xF3,                                                 // DIV EBX ; line 1145
	0x81, 0xFA, 0x00, 0x00, 0x00, 0x00,                         // CMP EDX,0x00 ; line 1146
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1147
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1148
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1149
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1151
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 1152
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 1153
	0x6A, 0x05,                                                 // PUSH 0x05 ; line 1154
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1155
	0x61,                                                       // POPAD ; line 1157
	0x66, 0x9D,                                                 // POPF ; line 1158
	0xD9, 0xEE,                                                 // FLDZ ; line 1159
};
static JumpTarget clownpiece3SCSTAR_jumpData[] = {
	{ 0x048, 0x464980 },
};
static PatchByteCode clownpiece3SCSTAR_patchByteCode = { clownpiece3SCSTAR_byteCode, 81, clownpiece3SCSTAR_jumpData, 1 };
// clownpiece106 declared in clownpiece.asm line 1169
static unsigned char clownpiece106_byteCode[] = {
	0xD8, 0x05, 0xA4, 0x75, 0x85, 0x00, // FADD DWORD PTR [0x8575A4] ; line 1170
};
static PatchByteCode clownpiece106_patchByteCode = { clownpiece106_byteCode, 6, nullptr, 0 };
// clownpiece107 declared in clownpiece.asm line 1178
static unsigned char clownpiece107_byteCode[] = {
	0xD8, 0x0D, 0xD4, 0x7D, 0x85, 0x00, // FMUL DWORD PTR [0x857DD4] ; line 1179
};
static PatchByteCode clownpiece107_patchByteCode = { clownpiece107_byteCode, 6, nullptr, 0 };
// clownpiece108 declared in clownpiece.asm line 1184
static unsigned char clownpiece108_byteCode[] = {
	0x68, 0x51, 0x03, 0x00, 0x00, // PUSH 0x00000351 ; line 1185
};
static PatchByteCode clownpiece108_patchByteCode = { clownpiece108_byteCode, 5, nullptr, 0 };
// clownpiece109 declared in clownpiece.asm line 1193
static unsigned char clownpiece109_byteCode[] = {
	0xD9, 0x05, 0xA8, 0x75, 0x85, 0x00, // FLD DWORD PTR [0x8575A8] ; line 1194
};
static PatchByteCode clownpiece109_patchByteCode = { clownpiece109_byteCode, 6, nullptr, 0 };
// clownpiece200 declared in clownpiece.asm line 1217
static unsigned char clownpiece200_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x02 ; line 1218
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x636EF3 ; line 1219
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x06, 0x00, // CMP WORD PTR [ESI+0x13E],0x06 ; line 1220
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                   // JG 0x636EF3 ; line 1221
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x636EE4 ; line 1222
};
static JumpTarget clownpiece200_jumpData[] = {
	{ 0x00B, 0x636EF3 },
	{ 0x01A, 0x636EF3 },
	{ 0x01F, 0x636EE4 },
};
static PatchByteCode clownpiece200_patchByteCode = { clownpiece200_byteCode, 35, clownpiece200_jumpData, 3 };
// clownpiece201 declared in clownpiece.asm line 1229
static unsigned char clownpiece201_byteCode[] = {
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 1230
	0xD9, 0x05, 0x44, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BC44] ; line 1231
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF4] ; line 1232
};
static PatchByteCode clownpiece201_patchByteCode = { clownpiece201_byteCode, 18, nullptr, 0 };
// clownpiece1SCCRUSH declared in clownpiece.asm line 1239
static unsigned char clownpiece1SCCRUSH_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1240
	0x60,                                                       // PUSHAD ; line 1241
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                         // MOV EBX,[ESI+0x140] ; line 1242
	0x80, 0xFB, 0x10,                                           // CMP BL,0x10 ; line 1243
	0x0F, 0x8C, 0x82, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 1244
	0x80, 0xFB, 0x24,                                           // CMP BL,0x24 ; line 1245
	0x0F, 0x8F, 0x79, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 1246
	0x80, 0xFB, 0x10,                                           // CMP BL,0x10 ; line 1247
	0x0F, 0x85, 0x0C, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL2 ; line 1248
	0xD9, 0x05, 0x00, 0x9F, 0x85, 0x00,                         // FLD DWORD PTR [0x859F00] ; line 1249
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1250
	0xD9, 0x86, 0xF4, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF4] ; line 1252
	0xD8, 0x25, 0x14, 0xCB, 0x85, 0x00,                         // FSUB DWORD PTR [0x85CB14] ; line 1253
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1254
	0x80, 0xFB, 0x10,                                           // CMP BL,0x10 ; line 1255
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1256
	0x80, 0xFB, 0x14,                                           // CMP BL,0x14 ; line 1257
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1258
	0x80, 0xFB, 0x18,                                           // CMP BL,0x18 ; line 1259
	0x0F, 0x84, 0x21, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1260
	0x80, 0xFB, 0x1C,                                           // CMP BL,0x1C ; line 1261
	0x0F, 0x84, 0x18, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1262
	0x80, 0xFB, 0x20,                                           // CMP BL,0x20 ; line 1263
	0x0F, 0x84, 0x0F, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1264
	0x80, 0xFB, 0x24,                                           // CMP BL,0x24 ; line 1265
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1266
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 1267
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1269
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1270
	0xE9, 0x08, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL5 ; line 1272
	0xD9, 0xEE,                                                 // FLDZ ; line 1274
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 1275
	0x61,                                                       // POPAD ; line 1277
	0x66, 0x9D,                                                 // POPF ; line 1278
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x00 ; line 1279
};
static PatchByteCode clownpiece1SCCRUSH_patchByteCode = { clownpiece1SCCRUSH_byteCode, 168, nullptr, 0 };
// clownpiece1SCCRUSH_FLASH declared in clownpiece.asm line 1283
static unsigned char clownpiece1SCCRUSH_FLASH_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 1284
};
static PatchByteCode clownpiece1SCCRUSH_FLASH_patchByteCode = { clownpiece1SCCRUSH_FLASH_byteCode, 9, nullptr, 0 };
// clownpiece1SCCRUSH_START declared in clownpiece.asm line 1288
static unsigned char clownpiece1SCCRUSH_START_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1289
};
static PatchByteCode clownpiece1SCCRUSH_START_patchByteCode = { clownpiece1SCCRUSH_START_byteCode, 9, nullptr, 0 };
// clownpiece1SCCRUSH_Y declared in clownpiece.asm line 1293
static unsigned char clownpiece1SCCRUSH_Y_byteCode[] = {
	0xD8, 0x05, 0xC4, 0xBD, 0x85, 0x00, // FADD DWORD PTR [0x85BDC4] ; line 1294
};
static PatchByteCode clownpiece1SCCRUSH_Y_patchByteCode = { clownpiece1SCCRUSH_Y_byteCode, 6, nullptr, 0 };
// clownpiece1SCCRUSH_2 declared in clownpiece.asm line 1299
static unsigned char clownpiece1SCCRUSH_2_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 1300
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                   // MOV EBX,[ESI+0x398] ; line 1301
	0xD9, 0x83, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [EBX+0xEC] ; line 1302
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xEC] ; line 1303
	0x61,                                                 // POPAD ; line 1304
	0x60,                                                 // PUSHAD ; line 1305
	0x8B, 0x9E, 0x98, 0x03, 0x00, 0x00,                   // MOV EBX,[ESI+0x398] ; line 1306
	0x8B, 0x83, 0x3C, 0x01, 0x00, 0x00,                   // MOV EAX,[EBX+0x13C] ; line 1307
	0x66, 0x81, 0xF8, 0x5E, 0x02,                         // CMP AX,0x025E ; line 1308
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1309
	0x8B, 0x83, 0x40, 0x01, 0x00, 0x00,                   // MOV EAX,[EBX+0x140] ; line 1310
	0x66, 0x81, 0xF8, 0x28, 0x00,                         // CMP AX,0x0028 ; line 1311
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,                   // JNL SHORT LABEL1 ; line 1312
	0x8B, 0x9B, 0x96, 0x01, 0x00, 0x00,                   // MOV EBX,[EBX+0x196] ; line 1313
	0x89, 0x9E, 0x96, 0x01, 0x00, 0x00,                   // MOV DWORD PTR [ESI+0x196],EBX ; line 1314
	0xE9, 0x1A, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL2 ; line 1315
	0x8B, 0x86, 0x40, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x140] ; line 1317
	0x66, 0x81, 0xF8, 0xFF, 0x00,                         // CMP AX,0x00FF ; line 1318
	0x0F, 0x8D, 0x09, 0x00, 0x00, 0x00,                   // JNL SHORT LABEL2 ; line 1319
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0xFF, 0x00, // MOV WORD PTR [ESI+0x140],0x00FF ; line 1320
	0x61,                                                 // POPAD ; line 1322
};
static PatchByteCode clownpiece1SCCRUSH_2_patchByteCode = { clownpiece1SCCRUSH_2_byteCode, 105, nullptr, 0 };
// clownpiece1SCCRUSH_DELAY declared in clownpiece.asm line 1327
static unsigned char clownpiece1SCCRUSH_DELAY_byteCode[] = {
	0x68, 0x68, 0x01, 0x00, 0x00, // PUSH 0x00000168 ; line 1328
};
static PatchByteCode clownpiece1SCCRUSH_DELAY_patchByteCode = { clownpiece1SCCRUSH_DELAY_byteCode, 5, nullptr, 0 };
// clownpiece2SCFIELD_DUR declared in clownpiece.asm line 1339
static unsigned char clownpiece2SCFIELD_DUR_byteCode[] = {
	0x66, 0xC7, 0x86, 0xB2, 0x08, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [ESI+0x8B2],0x0000 ; line 1340
};
static PatchByteCode clownpiece2SCFIELD_DUR_patchByteCode = { clownpiece2SCFIELD_DUR_byteCode, 9, nullptr, 0 };
// clownpiece2SCFIELD_1 declared in clownpiece.asm line 1346
static unsigned char clownpiece2SCFIELD_1_byteCode[] = {
	0x8D, 0x0C, 0x00, // LEA ECX,[EAX+EAX] ; line 1347
};
static PatchByteCode clownpiece2SCFIELD_1_patchByteCode = { clownpiece2SCFIELD_1_byteCode, 3, nullptr, 0 };
// clownpiece2SCFIELD_2 declared in clownpiece.asm line 1351
static unsigned char clownpiece2SCFIELD_2_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1352
	0x60,                                                       // PUSHAD ; line 1353
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 1354
	0x90,                                                       // NOP ; line 1355
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1356
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1357
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 1358
	0x90,                                                       // NOP ; line 1359
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 1362
	0x0F, 0xB7, 0x80, 0x3C, 0x01, 0x00, 0x00,                   // MOVZX EAX, WORD PTR [EAX+0x13C] ; line 1363
	0x66, 0x81, 0xF8, 0x31, 0x00,                               // CMP AX,0x0031 ; line 1364
	0x0F, 0x8E, 0x0B, 0x00, 0x00, 0x00,                         // JLE SHORT LABEL2 ; line 1365
	0x66, 0x81, 0xF8, 0x96, 0x00,                               // CMP AX,0x0096 ; line 1366
	0x0F, 0x8C, 0x1D, 0x00, 0x00, 0x00,                         // JL SHORT LABEL3 ; line 1367
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 1369
	0x0F, 0x85, 0x22, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 1370
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x08 ; line 1371
	0x0F, 0x85, 0x00, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 1372
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 1374
	0x90,                                                       // NOP ; line 1375
	0xC7, 0x86, 0x10, 0x01, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, // MOV DWORD PTR [ESI+0x110],0x55555555 ; line 1376
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 1378
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 1379
	0x61,                                                       // POPAD ; line 1380
	0x66, 0x9D,                                                 // POPF ; line 1381
	0xD9, 0xE8,                                                 // FLD1 ; line 1382
};
static PatchByteCode clownpiece2SCFIELD_2_patchByteCode = { clownpiece2SCFIELD_2_byteCode, 141, nullptr, 0 };
// clownpiece2SCD623_DUR declared in clownpiece.asm line 1389
static unsigned char clownpiece2SCD623_DUR_byteCode[] = {
	0x81, 0xF8, 0x1C, 0x02, 0x00, 0x00, // CMP EAX,0x21C ; line 1390
};
static PatchByteCode clownpiece2SCD623_DUR_patchByteCode = { clownpiece2SCD623_DUR_byteCode, 6, nullptr, 0 };
// clownpieceD22B declared in clownpiece.asm line 1401
static unsigned char clownpieceD22B_byteCode[] = {
	0x81, 0xEC, 0x40, 0x00, 0x00, 0x00, // SUB ESP,0x40 ; line 1403
	0xE8, 0x1E, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1404
	0xE8, 0x19, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1406
	0xE8, 0x14, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1408
	0xE8, 0x0F, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1410
	0x81, 0xC4, 0x40, 0x00, 0x00, 0x00, // ADD ESP,0x40 ; line 1411
	0x66, 0x9C,                         // PUSHF ; line 1412
	0x66, 0x9D,                         // POPF ; line 1414
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63B6BB ; line 1415
	0xD9, 0xEE,                         // FLDZ ; line 1417
	0x6A, 0x1E,                         // PUSH 0x1E ; line 1418
	0xD9, 0x9E, 0x80, 0x03, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x380] ; line 1419
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63B58F ; line 1420
};
static JumpTarget clownpieceD22B_jumpData[] = {
	{ 0x025, 0x63B6BB },
	{ 0x034, 0x63B58F },
};
static PatchByteCode clownpieceD22B_patchByteCode = { clownpieceD22B_byteCode, 56, clownpieceD22B_jumpData, 2 };
// clownpieceD22_1 declared in clownpiece.asm line 1424
static unsigned char clownpieceD22_1_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00, // PUSH 0x00000320 ; line 1425
	0xFF, 0xD2,                   // CALL EDX ; line 1426
	0xC3,                         // RET ; line 1427
};
static PatchByteCode clownpieceD22_1_patchByteCode = { clownpieceD22_1_byteCode, 8, nullptr, 0 };
// clownpieceD22_2 declared in clownpiece.asm line 1481
static unsigned char clownpieceD22_2_byteCode[] = {
	0xD8, 0x25, 0x30, 0x88, 0x85, 0x00, // FSUB DWORD PTR [0x858830] ; line 1482
};
static PatchByteCode clownpieceD22_2_patchByteCode = { clownpieceD22_2_byteCode, 6, nullptr, 0 };
// clownpieceD22_3 declared in clownpiece.asm line 1487
static unsigned char clownpieceD22_3_byteCode[] = {
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0xFF, // CMP BYTE PTR [ESI+0x104],0xFF ; line 1488
	0x0F, 0x84, 0x08, 0x00, 0x00, 0x00,       // JE SHORT LABEL1 ; line 1489
	0xD9, 0xEE,                               // FLDZ ; line 1490
	0x0F, 0x85, 0x06, 0x00, 0x00, 0x00,       // JNE SHORT LABEL2 ; line 1491
	0xD9, 0x05, 0xF4, 0xB8, 0x85, 0x00,       // FLD DWORD PTR [0x85B8F4] ; line 1493
	0x90,                                     // NOP ; line 1495
};
static PatchByteCode clownpieceD22_3_patchByteCode = { clownpieceD22_3_byteCode, 28, nullptr, 0 };
// clownpieceD22C declared in clownpiece.asm line 1501
static unsigned char clownpieceD22C_byteCode[] = {
	0x81, 0xEC, 0x40, 0x00, 0x00, 0x00, // SUB ESP,0x40 ; line 1503
	0xE8, 0x1E, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1504
	0xE8, 0x19, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1506
	0xE8, 0x14, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1508
	0xE8, 0x0F, 0x00, 0x00, 0x00,       // CALL LABEL1 ; line 1510
	0x81, 0xC4, 0x40, 0x00, 0x00, 0x00, // ADD ESP,0x40 ; line 1511
	0x66, 0x9C,                         // PUSHF ; line 1512
	0x66, 0x9D,                         // POPF ; line 1514
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63B8B0 ; line 1515
	0xD9, 0xEE,                         // FLDZ ; line 1517
	0x6A, 0x1E,                         // PUSH 0x1E ; line 1518
	0xD9, 0x9E, 0x80, 0x03, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x380] ; line 1519
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63B77F ; line 1520
};
static JumpTarget clownpieceD22C_jumpData[] = {
	{ 0x025, 0x63B8B0 },
	{ 0x034, 0x63B77F },
};
static PatchByteCode clownpieceD22C_patchByteCode = { clownpieceD22C_byteCode, 56, clownpieceD22C_jumpData, 2 };
// clownpieceD22C_1 declared in clownpiece.asm line 1524
static unsigned char clownpieceD22C_1_byteCode[] = {
	0x68, 0x20, 0x03, 0x00, 0x00, // PUSH 0x00000320 ; line 1525
	0xFF, 0xD2,                   // CALL EDX ; line 1526
	0xB9, 0x04, 0x00, 0x00, 0x00, // MOV ECX,0x00000004 ; line 1527
	0xC3,                         // RET ; line 1528
};
static PatchByteCode clownpieceD22C_1_patchByteCode = { clownpieceD22C_1_byteCode, 13, nullptr, 0 };
// clownpieceD22_NOJUMP declared in clownpiece.asm line 1538
static unsigned char clownpieceD22_NOJUMP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x62C538 ; line 1539
};
static JumpTarget clownpieceD22_NOJUMP_jumpData[] = {
	{ 0x001, 0x62C538 },
};
static PatchByteCode clownpieceD22_NOJUMP_patchByteCode = { clownpieceD22_NOJUMP_byteCode, 5, clownpieceD22_NOJUMP_jumpData, 1 };
// clownpieceD22_SPIRIT declared in clownpiece.asm line 1545
static unsigned char clownpieceD22_SPIRIT_byteCode[] = {
	0x6A, 0x78,                   // PUSH 0x78 ; line 1546
	0x68, 0xC8, 0x00, 0x00, 0x00, // PUSH 0x000000C8 ; line 1547
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1548
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x47A9E0 ; line 1549
	0x6A, 0x32,                   // PUSH 0x32 ; line 1550
	0x89, 0xF1,                   // MOV ECX,ESI ; line 1551
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 1552
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x62C554 ; line 1553
};
static JumpTarget clownpieceD22_SPIRIT_jumpData[] = {
	{ 0x00A, 0x47A9E0 },
	{ 0x013, 0x487870 },
	{ 0x018, 0x62C554 },
};
static PatchByteCode clownpieceD22_SPIRIT_patchByteCode = { clownpieceD22_SPIRIT_byteCode, 28, clownpieceD22_SPIRIT_jumpData, 3 };
// clownpieceA623CLONE_0 declared in clownpiece.asm line 1571
static unsigned char clownpieceA623CLONE_0_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xAB, 0x85, 0x00,       // FLD DWORD PTR [0x85ABE4] ; line 1572
	0xD9, 0x9C, 0x24, 0xE4, 0x04, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x4E4] ; line 1573
	0xD8, 0x0D, 0x00, 0x9F, 0x85, 0x00,       // FMUL DWORD PTR [0x859F00] ; line 1574
	0xDC, 0x05, 0xF0, 0x8E, 0x85, 0x00,       // FADD QWORD PTR [0x858EF0] ; line 1575
	0xD9, 0x9C, 0x24, 0xE8, 0x04, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x4E8] ; line 1576
};
static PatchByteCode clownpieceA623CLONE_0_patchByteCode = { clownpieceA623CLONE_0_byteCode, 32, nullptr, 0 };
// clownpieceA623CLONE_1 declared in clownpiece.asm line 1578
static unsigned char clownpieceA623CLONE_1_byteCode[] = {
	0xD9, 0x05, 0xE4, 0xAB, 0x85, 0x00,       // FLD DWORD PTR [0x85ABE4] ; line 1579
	0xD9, 0x9C, 0x24, 0x54, 0x04, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x454] ; line 1580
	0xD8, 0x0D, 0x00, 0x9F, 0x85, 0x00,       // FMUL DWORD PTR [0x859F00] ; line 1581
	0xDC, 0x05, 0xF0, 0x8E, 0x85, 0x00,       // FADD QWORD PTR [0x858EF0] ; line 1582
	0xD9, 0x9C, 0x24, 0x58, 0x04, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x458] ; line 1583
};
static PatchByteCode clownpieceA623CLONE_1_patchByteCode = { clownpieceA623CLONE_1_byteCode, 32, nullptr, 0 };
// clownpieceA623CLONE_2 declared in clownpiece.asm line 1589
static unsigned char clownpieceA623CLONE_2_byteCode[] = {
	0xB9, 0x10, 0x00, 0x00, 0x00, // MOV ECX,0x00000010 ; line 1590
};
static PatchByteCode clownpieceA623CLONE_2_patchByteCode = { clownpieceA623CLONE_2_byteCode, 5, nullptr, 0 };
// clownpieceA623_STACK declared in clownpiece.asm line 1596
static unsigned char clownpieceA623_STACK_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1597
	0x60,                                                       // PUSHAD ; line 1598
	0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x35, 0x03, 0x01, 0x00, // CMP DWORD PTR [ESI+0x13C],0x00010335 ; line 1599
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1600
	0xE9, 0x47, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1602
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 1604
	0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x21, 0x02, 0x00, 0x00, // CMP DWORD PTR [EAX+0x13C],0x00000221 ; line 1605
	0x0F, 0x84, 0x10, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1606
	0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x22, 0x02, 0x00, 0x00, // CMP DWORD PTR [EAX+0x13C],0x00000222 ; line 1607
	0x0F, 0x85, 0xD5, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 1608
	0x81, 0xB8, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [EAX+0x142],0x00000000 ; line 1610
	0x90,                                                       // NOP ; line 1611
	0x0F, 0x85, 0xC4, 0xFF, 0xFF, 0xFF,                         // JNE SHORT LABEL4 ; line 1612
	0xC7, 0x86, 0x44, 0x01, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x144],0x000000FF ; line 1613
	0xE9, 0xB5, 0xFF, 0xFF, 0xFF,                               // JMP SHORT LABEL4 ; line 1614
	0x90,                                                       // NOP ; line 1615
	0x61,                                                       // POPAD ; line 1617
	0x66, 0x9D,                                                 // POPF ; line 1618
	0xBB, 0x03, 0x00, 0x00, 0x00,                               // MOV EBX,0x00000003 ; line 1619
};
static PatchByteCode clownpieceA623_STACK_patchByteCode = { clownpieceA623_STACK_byteCode, 103, nullptr, 0 };
// clownpiece4SCBEAM_ANGLE declared in clownpiece.asm line 1625
static unsigned char clownpiece4SCBEAM_ANGLE_byteCode[] = {
	0xD9, 0x05, 0x90, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B990] ; line 1626
};
static PatchByteCode clownpiece4SCBEAM_ANGLE_patchByteCode = { clownpiece4SCBEAM_ANGLE_byteCode, 6, nullptr, 0 };
// clownpiece4SCBEAM declared in clownpiece.asm line 1630
static unsigned char clownpiece4SCBEAM_byteCode[] = {
	0x66, 0x9C,                               // PUSHF ; line 1631
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0xFF, // CMP BYTE PTR [ESI+0x104],0xFF ; line 1632
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,       // JNE SHORT LABEL1 ; line 1633
	0xD8, 0x25, 0x48, 0xD6, 0x85, 0x00,       // FSUB DWORD PTR [0x85D648] ; line 1634
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 1635
	0xD8, 0x05, 0x48, 0xD6, 0x85, 0x00,       // FADD DWORD PTR [0x85D648] ; line 1637
	0x66, 0x9D,                               // POPF ; line 1639
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xEC] ; line 1640
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [EAX+0xF0] ; line 1641
};
static PatchByteCode clownpiece4SCBEAM_patchByteCode = { clownpiece4SCBEAM_byteCode, 46, nullptr, 0 };
// clownpiece4SCBEAM_SOUND declared in clownpiece.asm line 1646
static unsigned char clownpiece4SCBEAM_SOUND_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1647
	0x60,                                                       // PUSHAD ; line 1648
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x0000004A ; line 1649
	0x0F, 0x8F, 0x2C, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 1650
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000014 ; line 1651
	0x0F, 0x8C, 0x1C, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 1652
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x142],0x0000 ; line 1653
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1654
	0x6A, 0x05,                                                 // PUSH 0x05 ; line 1655
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                         // MOV ECX,[ESI+0x398] ; line 1656
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x464980 ; line 1657
	0x61,                                                       // POPAD ; line 1659
	0x66, 0x9D,                                                 // POPF ; line 1660
	0x0F, 0xBE, 0x96, 0x04, 0x01, 0x00, 0x00,                   // MOVSX EDX,BYTE PTR[ESI+0x104] ; line 1661
};
static JumpTarget clownpiece4SCBEAM_SOUND_jumpData[] = {
	{ 0x03B, 0x464980 },
};
static PatchByteCode clownpiece4SCBEAM_SOUND_patchByteCode = { clownpiece4SCBEAM_SOUND_byteCode, 73, clownpiece4SCBEAM_SOUND_jumpData, 1 };
// clownpiece5SCLRE_0 declared in clownpiece.asm line 1667
static unsigned char clownpiece5SCLRE_0_byteCode[] = {
	0xDC, 0x05, 0x28, 0xC0, 0x85, 0x00, // FADD QWORD PTR [0x85C028] ; line 1668
};
static PatchByteCode clownpiece5SCLRE_0_patchByteCode = { clownpiece5SCLRE_0_byteCode, 6, nullptr, 0 };
// clownpiece5SCLRE_1 declared in clownpiece.asm line 1671
static unsigned char clownpiece5SCLRE_1_byteCode[] = {
	0x8A, 0x86, 0x13, 0x01, 0x00, 0x00, // MOV AL,[ESI+0x113] ; line 1672
	0x80, 0xF8, 0x0A,                   // CMP AL,0x0A ; line 1673
};
static PatchByteCode clownpiece5SCLRE_1_patchByteCode = { clownpiece5SCLRE_1_byteCode, 9, nullptr, 0 };
// clownpiece5SCLRE_2 declared in clownpiece.asm line 1675
static unsigned char clownpiece5SCLRE_2_byteCode[] = {
	0x80, 0xE8, 0x0A,                   // SUB AL,0x0A ; line 1676
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00, // MOV [ESI+0x113],AL ; line 1677
};
static PatchByteCode clownpiece5SCLRE_2_patchByteCode = { clownpiece5SCLRE_2_byteCode, 9, nullptr, 0 };
// clownpiece5SCLRE_3 declared in clownpiece.asm line 1679
static unsigned char clownpiece5SCLRE_3_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x140],0x00 ; line 1680
};
static PatchByteCode clownpiece5SCLRE_3_patchByteCode = { clownpiece5SCLRE_3_byteCode, 9, nullptr, 0 };
// clownpiece5SCLRE declared in clownpiece.asm line 1688
static unsigned char clownpiece5SCLRE_byteCode[] = {
	0x66, 0x9C,                                                 // PUSHF ; line 1689
	0x60,                                                       // PUSHAD ; line 1690
	0x8B, 0x8E, 0x70, 0x01, 0x00, 0x00,                         // MOV ECX,[ESI+0x170] ; line 1691
	0x81, 0xB9, 0x54, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x754],0x00000000 ; line 1692
	0x0F, 0x85, 0x4D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1693
	0x81, 0xB9, 0x74, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x774],0x00000000 ; line 1694
	0x0F, 0x85, 0x3D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 1695
	0x81, 0xB9, 0x58, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x758],0x00000000 ; line 1696
	0x0F, 0x84, 0x0C, 0x00, 0x00, 0x00,                         // JE SHORT LABEL4 ; line 1697
	0x0F, 0x8C, 0x43, 0x00, 0x00, 0x00,                         // JL SHORT LABEL6 ; line 1698
	0x0F, 0x8F, 0x2F, 0x00, 0x00, 0x00,                         // JG SHORT LABEL5 ; line 1699
	0x81, 0xB9, 0x78, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ECX+0x778],0x00000000 ; line 1701
	0x0F, 0x84, 0x3F, 0x00, 0x00, 0x00,                         // JE SHORT LABEL3 ; line 1702
	0x0F, 0x8C, 0x27, 0x00, 0x00, 0x00,                         // JL SHORT LABEL6 ; line 1703
	0x0F, 0x8F, 0x13, 0x00, 0x00, 0x00,                         // JG SHORT LABEL5 ; line 1704
	0xE9, 0x2E, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 1705
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // MOV WORD PTR [ESI+0x140],0x0000 ; line 1707
	0xE9, 0x17, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1708
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x140],0x0001 ; line 1710
	0xE9, 0x09, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL2 ; line 1711
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00,       // MOV WORD PTR [ESI+0x140],0x0002 ; line 1713
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0xC7, 0x00,       // MOV WORD PTR [ESI+0x142],0x00C7 ; line 1715
	0x61,                                                       // POPAD ; line 1717
	0x66, 0x9D,                                                 // POPF ; line 1718
	0x66, 0x9C,                                                 // PUSHF ; line 1720
	0x60,                                                       // PUSHAD ; line 1721
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 1722
	0x85, 0xC0,                                                 // TEST EAX,EAX ; line 1723
	0x0F, 0x84, 0x3D, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 1724
	0x8B, 0x90, 0x70, 0x01, 0x00, 0x00,                         // MOV EDX,[EAX+0x170] ; line 1725
	0x85, 0xD2,                                                 // TEST EDX,EDX ; line 1726
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE SHORT LABELA ; line 1727
	0x66, 0x81, 0xB8, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x184],0x00 ; line 1728
	0x0F, 0x8E, 0x0F, 0x00, 0x00, 0x00,                         // JLE SHORT LABELB ; line 1729
	0x66, 0x81, 0xBA, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EDX+0x184],0x00 ; line 1730
	0x0F, 0x8F, 0x11, 0x00, 0x00, 0x00,                         // JG SHORT LABELA ; line 1731
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 1733
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 1734
	0x61,                                                       // POPAD ; line 1736
	0x66, 0x9D,                                                 // POPF ; line 1737
	0xBF, 0x01, 0x00, 0x00, 0x00,                               // MOV EDI,0x00000001 ; line 1738
};
static PatchByteCode clownpiece5SCLRE_patchByteCode = { clownpiece5SCLRE_byteCode, 237, nullptr, 0 };
// clownpieceD22_FIX declared in clownpiece.asm line 1743
static unsigned char clownpieceD22_FIX_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000000 ; line 1744
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                         // JNE 0x62C40D ; line 1745
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000001 ; line 1746
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x62C40D ; line 1747
};
static JumpTarget clownpieceD22_FIX_jumpData[] = {
	{ 0x00C, 0x62C40D },
	{ 0x01B, 0x62C40D },
};
static PatchByteCode clownpieceD22_FIX_patchByteCode = { clownpieceD22_FIX_byteCode, 31, clownpieceD22_FIX_jumpData, 2 };
// clownpiece132 declared in clownpiece.asm line 1751
static unsigned char clownpiece132_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x637D40 ; line 1752
};
static JumpTarget clownpiece132_jumpData[] = {
	{ 0x001, 0x637D40 },
};
static PatchByteCode clownpiece132_patchByteCode = { clownpiece132_byteCode, 5, clownpiece132_jumpData, 1 };
// clownpiece133 declared in clownpiece.asm line 1758
static unsigned char clownpiece133_byteCode[] = {
	0xDC, 0x05, 0x28, 0xC0, 0x85, 0x00, // FADD QWORD PTR [0x85C028] ; line 1759
};
static PatchByteCode clownpiece133_patchByteCode = { clownpiece133_byteCode, 6, nullptr, 0 };
// clownpieceA214RING_0 declared in clownpiece.asm line 1767
static unsigned char clownpieceA214RING_0_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 1768
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 1769
	0xE9, 0x33, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 1770
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 1772
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 1773
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x113],0x00 ; line 1774
	0x60,                                                       // PUSHAD ; line 1776
	0x8B, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x113] ; line 1777
	0x80, 0xF8, 0xE6,                                           // CMP AL,0xE6 ; line 1778
	0x0F, 0x86, 0x02, 0x00, 0x00, 0x00,                         // JNA SHORT LABEL4 ; line 1779
	0xB0, 0xE6,                                                 // MOV AL,0xE6 ; line 1780
	0x80, 0xC0, 0x19,                                           // ADD AL,0x19 ; line 1782
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV BYTE PTR [ESI+0x113],AL ; line 1783
	0x61,                                                       // POPAD ; line 1784
	0xD9, 0xEE,                                                 // FLDZ ; line 1786
	0x90,                                                       // NOP ; line 1787
	0xD9, 0x96, 0x24, 0x01, 0x00, 0x00,                         // FST DWORD PTR [ESI+0x124] ; line 1788
	0xD9, 0x9E, 0x28, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x128] ; line 1789
};
static PatchByteCode clownpieceA214RING_0_patchByteCode = { clownpieceA214RING_0_byteCode, 86, nullptr, 0 };
// clownpiece110 declared in clownpiece.asm line 1798
static unsigned char clownpiece110_byteCode[] = {
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x194],0x0 ; line 1799
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x639F45 ; line 1800
};
static JumpTarget clownpiece110_jumpData[] = {
	{ 0x008, 0x639F45 },
};
static PatchByteCode clownpiece110_patchByteCode = { clownpiece110_byteCode, 12, clownpiece110_jumpData, 1 };
// clownpiece111 declared in clownpiece.asm line 1805
static unsigned char clownpiece111_byteCode[] = {
	0xDD, 0x05, 0xF8, 0x81, 0x85, 0x00, // FLD QWORD PTR [0x8581F8] ; line 1806
};
static PatchByteCode clownpiece111_patchByteCode = { clownpiece111_byteCode, 6, nullptr, 0 };
// clownpiece112 declared in clownpiece.asm line 1808
static unsigned char clownpiece112_byteCode[] = {
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 1809
};
static PatchByteCode clownpiece112_patchByteCode = { clownpiece112_byteCode, 6, nullptr, 0 };
// clownpiece113 declared in clownpiece.asm line 1824
static unsigned char clownpiece113_byteCode[] = {
	0x80, 0xB8, 0xAD, 0x06, 0x00, 0x00, 0x04, // CMP BYTE PTR [EAX+0x6AD],0x04 ; line 1825
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x639CA3 ; line 1826
};
static JumpTarget clownpiece113_jumpData[] = {
	{ 0x008, 0x639CA3 },
};
static PatchByteCode clownpiece113_patchByteCode = { clownpiece113_byteCode, 12, clownpiece113_jumpData, 1 };
// clownpiece114 declared in clownpiece.asm line 1831
static unsigned char clownpiece114_byteCode[] = {
	0x80, 0xB8, 0xAD, 0x06, 0x00, 0x00, 0x04, // CMP BYTE PTR [EAX+0x6AD],0x04 ; line 1832
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x639EF5 ; line 1833
};
static JumpTarget clownpiece114_jumpData[] = {
	{ 0x008, 0x639EF5 },
};
static PatchByteCode clownpiece114_patchByteCode = { clownpiece114_byteCode, 12, clownpiece114_jumpData, 1 };
// clownpieceA214RING_1A declared in clownpiece.asm line 1838
static unsigned char clownpieceA214RING_1A_byteCode[] = {
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x639D59 ; line 1839
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1841
	0x0F, 0x85, 0x2A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1842
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x194],0x00 ; line 1843
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1844
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1845
	0x80, 0xBE, 0x13, 0x01, 0x00, 0x00, 0xA0,             // CMP BYTE PTR [ESI+0x113],0xA0 ; line 1846
	0x0F, 0x87, 0x07, 0x00, 0x00, 0x00,                   // JA SHORT LABEL1 ; line 1847
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xA0,             // MOV BYTE PTR [ESI+0x113],0xA0 ; line 1848
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x35C] ; line 1850
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x639D0B ; line 1851
};
static JumpTarget clownpieceA214RING_1A_jumpData[] = {
	{ 0x002, 0x639D59 },
	{ 0x046, 0x639D0B },
};
static PatchByteCode clownpieceA214RING_1A_patchByteCode = { clownpieceA214RING_1A_byteCode, 74, clownpieceA214RING_1A_jumpData, 2 };
// clownpieceA214RING_1B declared in clownpiece.asm line 1853
static unsigned char clownpieceA214RING_1B_byteCode[] = {
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x639FCE ; line 1854
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 1856
	0x0F, 0x85, 0x2A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1857
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x194],0x00 ; line 1858
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 1859
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 1860
	0x80, 0xBE, 0x13, 0x01, 0x00, 0x00, 0xA0,             // CMP BYTE PTR [ESI+0x113],0xA0 ; line 1861
	0x0F, 0x87, 0x07, 0x00, 0x00, 0x00,                   // JA SHORT LABEL1 ; line 1862
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xA0,             // MOV BYTE PTR [ESI+0x113],0xA0 ; line 1863
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x35C] ; line 1865
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x639F80 ; line 1866
};
static JumpTarget clownpieceA214RING_1B_jumpData[] = {
	{ 0x002, 0x639FCE },
	{ 0x046, 0x639F80 },
};
static PatchByteCode clownpieceA214RING_1B_patchByteCode = { clownpieceA214RING_1B_byteCode, 74, clownpieceA214RING_1B_jumpData, 2 };
// clownpieceA214RING_4 declared in clownpiece.asm line 1881
static unsigned char clownpieceA214RING_4_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x62B1A4 ; line 1882
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 1883
	0x6A, 0x03,                         // PUSH 0x03 ; line 1884
	0xB9, 0x0C, 0x05, 0x00, 0x00,       // MOV ECX,0x0000050C ; line 1885
	0xD9, 0x1C, 0x0C,                   // FSTP DWORD PTR [ESP+ECX] ; line 1886
	0x8D, 0x0C, 0x0C,                   // LEA ECX,[ESP+ECX] ; line 1887
	0xD9, 0xEE,                         // FLDZ ; line 1888
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62B1CA ; line 1889
};
static JumpTarget clownpieceA214RING_4_jumpData[] = {
	{ 0x002, 0x62B1A4 },
	{ 0x01C, 0x62B1CA },
};
static PatchByteCode clownpieceA214RING_4_patchByteCode = { clownpieceA214RING_4_byteCode, 32, clownpieceA214RING_4_jumpData, 2 };
// clownpieceA214RING_5 declared in clownpiece.asm line 1891
static unsigned char clownpieceA214RING_5_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x62B6C9 ; line 1892
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 1893
	0x6A, 0x03,                         // PUSH 0x03 ; line 1894
	0xB9, 0x6C, 0x05, 0x00, 0x00,       // MOV ECX,0x0000056C ; line 1895
	0xD9, 0x1C, 0x0C,                   // FSTP DWORD PTR [ESP+ECX] ; line 1896
	0x8D, 0x0C, 0x0C,                   // LEA ECX,[ESP+ECX] ; line 1897
	0xD9, 0xEE,                         // FLDZ ; line 1898
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62B6EF ; line 1899
};
static JumpTarget clownpieceA214RING_5_jumpData[] = {
	{ 0x002, 0x62B6C9 },
	{ 0x01C, 0x62B6EF },
};
static PatchByteCode clownpieceA214RING_5_patchByteCode = { clownpieceA214RING_5_byteCode, 32, clownpieceA214RING_5_jumpData, 2 };
// clownpieceA214RING_4A declared in clownpiece.asm line 1901
static unsigned char clownpieceA214RING_4A_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x62B271 ; line 1902
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 1903
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62B287 ; line 1904
};
static JumpTarget clownpieceA214RING_4A_jumpData[] = {
	{ 0x002, 0x62B271 },
	{ 0x00D, 0x62B287 },
};
static PatchByteCode clownpieceA214RING_4A_patchByteCode = { clownpieceA214RING_4A_byteCode, 17, clownpieceA214RING_4A_jumpData, 2 };
// clownpieceA214RING_5A declared in clownpiece.asm line 1906
static unsigned char clownpieceA214RING_5A_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x62B7A0 ; line 1907
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 1908
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,                   // JNE 0x62BB33 ; line 1909
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00,                   // FLD QWORD PTR [0x867830] ; line 1910
	0x6A, 0x03,                                           // PUSH 0x03 ; line 1911
	0xB9, 0x04, 0x04, 0x00, 0x00,                         // MOV ECX,0x00000404 ; line 1912
	0xD9, 0x1C, 0x0C,                                     // FSTP DWORD PTR [ESP+ECX] ; line 1913
	0x8D, 0x0C, 0x0C,                                     // LEA ECX,[ESP+ECX] ; line 1914
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62B7D4 ; line 1915
};
static JumpTarget clownpieceA214RING_5A_jumpData[] = {
	{ 0x002, 0x62B7A0 },
	{ 0x011, 0x62BB33 },
	{ 0x029, 0x62B7D4 },
};
static PatchByteCode clownpieceA214RING_5A_patchByteCode = { clownpieceA214RING_5A_byteCode, 45, clownpieceA214RING_5A_jumpData, 3 };
// clownpieceA214RING_ANGLE_0 declared in clownpiece.asm line 1925
static unsigned char clownpieceA214RING_ANGLE_0_byteCode[] = {
	0xD9, 0x05, 0x80, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB80] ; line 1926
};
static PatchByteCode clownpieceA214RING_ANGLE_0_patchByteCode = { clownpieceA214RING_ANGLE_0_byteCode, 6, nullptr, 0 };
// clownpieceA214RING_ANGLE_1 declared in clownpiece.asm line 1928
static unsigned char clownpieceA214RING_ANGLE_1_byteCode[] = {
	0xD9, 0x05, 0x40, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x871540] ; line 1929
};
static PatchByteCode clownpieceA214RING_ANGLE_1_patchByteCode = { clownpieceA214RING_ANGLE_1_byteCode, 6, nullptr, 0 };
// clownpieceA214RING_THISSUCKS declared in clownpiece.asm line 1948
static unsigned char clownpieceA214RING_THISSUCKS_byteCode[] = {
	0x80, 0xF8, 0x10,                   // CMP AL,0x10 ; line 1949
	0x0F, 0x82, 0xDE, 0xAD, 0xBE, 0xEF, // JB 0x6365F7 ; line 1950
	0x80, 0xE8, 0x10,                   // SUB AL,0x10 ; line 1951
};
static JumpTarget clownpieceA214RING_THISSUCKS_jumpData[] = {
	{ 0x005, 0x6365F7 },
};
static PatchByteCode clownpieceA214RING_THISSUCKS_patchByteCode = { clownpieceA214RING_THISSUCKS_byteCode, 12, clownpieceA214RING_THISSUCKS_jumpData, 1 };
// clownpieceA22SPLIT declared in clownpiece.asm line 1989
static unsigned char clownpieceA22SPLIT_byteCode[] = {
	0x66, 0x9C,                                           // PUSHF ; line 1990
	0x60,                                                 // PUSHAD ; line 1991
	0x81, 0xEC, 0x40, 0x00, 0x00, 0x00,                   // SUB ESP,0x40 ; line 1992
	0xE8, 0x1D, 0x00, 0x00, 0x00,                         // CALL LABELZ ; line 1993
	0xE8, 0x18, 0x00, 0x00, 0x00,                         // CALL LABELZ ; line 1994
	0xE8, 0x13, 0x00, 0x00, 0x00,                         // CALL LABELZ ; line 1995
	0xE8, 0x0E, 0x00, 0x00, 0x00,                         // CALL LABELZ ; line 1996
	0x81, 0xC4, 0x40, 0x00, 0x00, 0x00,                   // ADD ESP,0x40 ; line 1997
	0x61,                                                 // POPAD ; line 1998
	0x66, 0x9D,                                           // POPF ; line 1999
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62CB7C ; line 2000
	0xE8, 0x0B, 0x00, 0x00, 0x00,                         // CALL LABELX ; line 2003
	0xE8, 0x06, 0x00, 0x00, 0x00,                         // CALL LABELX ; line 2004
	0xE8, 0x01, 0x00, 0x00, 0x00,                         // CALL LABELX ; line 2005
	0xC3,                                                 // RET ; line 2006
	0xD9, 0xEE,                                           // FLDZ ; line 2009
	0xD9, 0x94, 0x24, 0x5C, 0x01, 0x00, 0x00,             // FST DWORD PTR [ESP+0x15C] ; line 2010
	0xD9, 0x9C, 0x24, 0x60, 0x01, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x160] ; line 2011
	0xD9, 0x05, 0x70, 0xB8, 0x85, 0x00,                   // FLD DWORD PTR [0x85B870] ; line 2012
	0xD9, 0x9C, 0x24, 0x64, 0x01, 0x00, 0x00,             // FSTP DWORD PTR [ESP+0x164] ; line 2014
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 2015
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF0] ; line 2016
	0x6A, 0x03,                                           // PUSH 0x03 ; line 2017
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B984] ; line 2019
	0x66, 0x8B, 0x86, 0x40, 0x01, 0x00, 0x00,             // MOV AX,[ESI+0x140] ; line 2020
	0x0F, 0xB7, 0xC0,                                     // MOVZX EAX,AX ; line 2021
	0x50,                                                 // PUSH EAX ; line 2022
	0xDB, 0x04, 0x24,                                     // FILD DWORD PTR [ESP] ; line 2023
	0x58,                                                 // POP EAX ; line 2024
	0xDE, 0xC9,                                           // FMULP ST1 ; line 2025
	0xDE, 0xC1,                                           // FADDP ; line 2026
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00,             // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 2027
	0x8D, 0x94, 0x24, 0x60, 0x01, 0x00, 0x00,             // LEA EDX,[ESP+0x160] ; line 2029
	0x52,                                                 // PUSH EDX ; line 2030
	0x57,                                                 // PUSH EDI ; line 2031
	0xD9, 0x5C, 0x24, 0x1C,                               // FSTP DWORD PTR [ESP+0x1C] ; line 2032
	0x50,                                                 // PUSH EAX ; line 2033
	0xD9, 0x44, 0x24, 0x20,                               // FLD DWORD PTR [ESP+0x20] ; line 2034
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                   // SUB ESP,0x08 ; line 2036
	0xD9, 0x5C, 0x24, 0x04,                               // FSTP DWORD PTR [ESP+0x04] ; line 2037
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 2038
	0xE9, 0x1B, 0x00, 0x00, 0x00,                         // JMP SHORT LABELXX ; line 2039
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0xFF,             // CMP BYTE PTR [ESI+0x104],0xFF ; line 2042
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                   // JNE SHORT LABELA ; line 2043
	0xDE, 0xE9,                                           // FSUBP ST1 ; line 2044
	0xE9, 0x02, 0x00, 0x00, 0x00,                         // JMP SHORT LABELB ; line 2045
	0xDE, 0xC1,                                           // FADDP ; line 2047
	0xE9, 0x28, 0x00, 0x00, 0x00,                         // JMP SHORT LABELC ; line 2049
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xEC] ; line 2052
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00,                   // FLD DWORD PTR [0x85B984] ; line 2053
	0x66, 0x8B, 0x86, 0x42, 0x01, 0x00, 0x00,             // MOV AX,[ESI+0x142] ; line 2054
	0x0F, 0xB7, 0xC0,                                     // MOVZX EAX,AX ; line 2055
	0x50,                                                 // PUSH EAX ; line 2056
	0xDB, 0x04, 0x24,                                     // FILD DWORD PTR [ESP] ; line 2057
	0x58,                                                 // POP EAX ; line 2058
	0xDE, 0xC9,                                           // FMULP ST1 ; line 2059
	0xD8, 0x25, 0x84, 0xB9, 0x85, 0x00,                   // FSUB DWORD PTR [0x85B984] ; line 2061
	0xE9, 0xBD, 0xFF, 0xFF, 0xFF,                         // JMP SHORT LABELD ; line 2062
	0x66, 0xFF, 0x86, 0x40, 0x01, 0x00, 0x00,             // INC WORD PTR [ESI+0x140] ; line 2064
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x140],0x03 ; line 2065
	0x0F, 0x8C, 0x10, 0x00, 0x00, 0x00,                   // JL SHORT LABELE ; line 2066
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, // MOV WORD PTR [ESI+0x140],0x0000 ; line 2067
	0x66, 0xFF, 0x86, 0x42, 0x01, 0x00, 0x00,             // INC WORD PTR [ESI+0x142] ; line 2068
	0xD9, 0x1C, 0x24,                                     // FSTP DWORD PTR [ESP] ; line 2071
	0x68, 0x3A, 0x03, 0x00, 0x00,                         // PUSH 0x0000033A ; line 2072
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x46EB30 ; line 2073
	0xC3,                                                 // RET ; line 2074
};
static JumpTarget clownpieceA22SPLIT_jumpData[] = {
	{ 0x027, 0x62CB7C },
	{ 0x11C, 0x46EB30 },
};
static PatchByteCode clownpieceA22SPLIT_patchByteCode = { clownpieceA22SPLIT_byteCode, 289, clownpieceA22SPLIT_jumpData, 2 };
// clownpieceA22SPLIT_1 declared in clownpiece.asm line 2078
static unsigned char clownpieceA22SPLIT_1_byteCode[] = {
	0xD9, 0x86, 0x78, 0x03, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x378] ; line 2079
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2080
	0x0F, 0x85, 0x4D, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 2081
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 2082
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 2083
	0x60,                                                       // PUSHAD ; line 2085
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 2086
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x36, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0236 ; line 2087
	0x0F, 0x84, 0x15, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 2088
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x38, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0238 ; line 2089
	0x0F, 0x84, 0x06, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 2090
	0x61,                                                       // POPAD ; line 2092
	0xE9, 0x11, 0x00, 0x00, 0x00,                               // JMP SHORT LABEL3 ; line 2093
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,                         // FLD DWORD PTR [0x858830] ; line 2095
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2096
	0xE9, 0xE9, 0xFF, 0xFF, 0xFF,                               // JMP LABEL2 ; line 2097
	0x90,                                                       // NOP ; line 2099
};
static PatchByteCode clownpieceA22SPLIT_1_patchByteCode = { clownpieceA22SPLIT_1_byteCode, 100, nullptr, 0 };
// clownpieceA22SPLIT_ONHIT declared in clownpiece.asm line 2103
static unsigned char clownpieceA22SPLIT_ONHIT_byteCode[] = {
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x5 ; line 2104
};
static PatchByteCode clownpieceA22SPLIT_ONHIT_patchByteCode = { clownpieceA22SPLIT_ONHIT_byteCode, 10, nullptr, 0 };
// clownpieceA22SPLIT_3 declared in clownpiece.asm line 2108
static unsigned char clownpieceA22SPLIT_3_byteCode[] = {
	0xD9, 0xE8,                               // FLD1 ; line 2109
	0xD9, 0x96, 0x1C, 0x01, 0x00, 0x00,       // FST DWORD PTR [ESI+0x11C] ; line 2110
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0x120] ; line 2111
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xEC] ; line 2113
	0x80, 0xBE, 0x04, 0x01, 0x00, 0x00, 0xFF, // CMP BYTE PTR [ESI+0x104],0xFF ; line 2114
	0x0F, 0x85, 0x0B, 0x00, 0x00, 0x00,       // JNE SHORT LABEL1 ; line 2115
	0xD8, 0xA6, 0xF4, 0x00, 0x00, 0x00,       // FSUB DWORD PTR [ESI+0xF4] ; line 2116
	0xE9, 0x06, 0x00, 0x00, 0x00,             // JMP SHORT LABEL2 ; line 2117
	0xD8, 0x86, 0xF4, 0x00, 0x00, 0x00,       // FADD DWORD PTR [ESI+0xF4] ; line 2119
	0xD9, 0x9E, 0xEC, 0x00, 0x00, 0x00,       // FSTP DWORD PTR [ESI+0xEC] ; line 2121
	0x60,                                     // PUSHAD ; line 2140
	0x6A, 0x02,                               // PUSH 0x02 ; line 2141
	0x6A, 0x02,                               // PUSH 0x02 ; line 2142
	0x6A, 0x00,                               // PUSH 0x00 ; line 2143
	0x89, 0xF1,                               // MOV ECX,ESI ; line 2144
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,             // CALL 0x48CE90 ; line 2145
	0x84, 0xC0,                               // TEST AL,AL ; line 2146
	0x61,                                     // POPAD ; line 2147
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF,       // JNE 0x6365F7 ; line 2148
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x194],0x01 ; line 2152
	0x0F, 0x84, 0x18, 0x00, 0x00, 0x00,       // JE SHORT LABEL7 ; line 2153
	0x8A, 0x86, 0x13, 0x01, 0x00, 0x00,       // MOV AL,[ESI+0x113] ; line 2154
	0x80, 0xF8, 0x0A,                         // CMP AL,0x0A ; line 2155
	0x0F, 0x82, 0x09, 0x00, 0x00, 0x00,       // JB SHORT LABEL7 ; line 2156
	0x80, 0xE8, 0x0A,                         // SUB AL,0x0A ; line 2157
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00,       // MOV BYTE PTR [ESI+0x113],AL ; line 2158
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x63BECF ; line 2160
};
static JumpTarget clownpieceA22SPLIT_3_jumpData[] = {
	{ 0x042, 0x48CE90 },
	{ 0x04B, 0x6365F7 },
	{ 0x075, 0x63BECF },
};
static PatchByteCode clownpieceA22SPLIT_3_patchByteCode = { clownpieceA22SPLIT_3_byteCode, 121, clownpieceA22SPLIT_3_jumpData, 3 };
// clownpieceA22SPLIT_8 declared in clownpiece.asm line 2165
static unsigned char clownpieceA22SPLIT_8_byteCode[] = {
	0x8A, 0x86, 0x13, 0x01, 0x00, 0x00, // MOV AL,[ESI+0x113] ; line 2166
	0x80, 0xF8, 0x05,                   // CMP AL,0x05 ; line 2167
	0x0F, 0x82, 0xDE, 0xAD, 0xBE, 0xEF, // JB 0x6365F7 ; line 2168
	0x80, 0xE8, 0x05,                   // SUB AL,0x05 ; line 2169
	0xB3, 0xFB,                         // MOV BL,0xFB ; line 2170
};
static JumpTarget clownpieceA22SPLIT_8_jumpData[] = {
	{ 0x00B, 0x6365F7 },
};
static PatchByteCode clownpieceA22SPLIT_8_patchByteCode = { clownpieceA22SPLIT_8_byteCode, 20, clownpieceA22SPLIT_8_jumpData, 1 };
// clownpieceA22SPLIT_JMP declared in clownpiece.asm line 2174
static unsigned char clownpieceA22SPLIT_JMP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x62C859 ; line 2175
};
static JumpTarget clownpieceA22SPLIT_JMP_jumpData[] = {
	{ 0x001, 0x62C859 },
};
static PatchByteCode clownpieceA22SPLIT_JMP_patchByteCode = { clownpieceA22SPLIT_JMP_byteCode, 5, clownpieceA22SPLIT_JMP_jumpData, 1 };
// clownpieceA22SPLIT_UNFLDZ declared in clownpiece.asm line 2191
static unsigned char clownpieceA22SPLIT_UNFLDZ_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x487870 ; line 2192
};
static JumpTarget clownpieceA22SPLIT_UNFLDZ_jumpData[] = {
	{ 0x001, 0x487870 },
};
static PatchByteCode clownpieceA22SPLIT_UNFLDZ_patchByteCode = { clownpieceA22SPLIT_UNFLDZ_byteCode, 5, clownpieceA22SPLIT_UNFLDZ_jumpData, 1 };
// clownpieceA623USB_DUR declared in clownpiece.asm line 2201
static unsigned char clownpieceA623USB_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x29 ; line 2202
};
static PatchByteCode clownpieceA623USB_DUR_patchByteCode = { clownpieceA623USB_DUR_byteCode, 10, nullptr, 0 };
// clownpieceA623USB declared in clownpiece.asm line 2208
static unsigned char clownpieceA623USB_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2209
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 2210
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xEC] ; line 2211
};
static PatchByteCode clownpieceA623USB_patchByteCode = { clownpieceA623USB_byteCode, 14, nullptr, 0 };
// clownpieceA623USB_CX declared in clownpiece.asm line 2219
static unsigned char clownpieceA623USB_CX_byteCode[] = {
	0x60,                               // PUSHAD ; line 2220
	0x8B, 0x86, 0x70, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x170] ; line 2221
	0xD9, 0x80, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [EAX+0xEC] ; line 2222
	0x61,                               // POPAD ; line 2223
};
static PatchByteCode clownpieceA623USB_CX_patchByteCode = { clownpieceA623USB_CX_byteCode, 14, nullptr, 0 };
// clownpieceA623USB_CY declared in clownpiece.asm line 2225
static unsigned char clownpieceA623USB_CY_byteCode[] = {
	0x60,                               // PUSHAD ; line 2226
	0x8B, 0x86, 0x70, 0x01, 0x00, 0x00, // MOV EAX,[ESI+0x170] ; line 2227
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [EAX+0xF0] ; line 2228
	0x61,                               // POPAD ; line 2229
};
static PatchByteCode clownpieceA623USB_CY_patchByteCode = { clownpieceA623USB_CY_byteCode, 14, nullptr, 0 };
// clownpiece140 declared in clownpiece.asm line 2236
static unsigned char clownpiece140_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x63A88F ; line 2237
};
static JumpTarget clownpiece140_jumpData[] = {
	{ 0x001, 0x63A88F },
};
static PatchByteCode clownpiece140_patchByteCode = { clownpiece140_byteCode, 5, clownpiece140_jumpData, 1 };
// clownpieceA623USB_1HIT declared in clownpiece.asm line 2242
static unsigned char clownpieceA623USB_1HIT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00, // MOV BYTE PTR [ESI+0x194],0x00 ; line 2243
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0x7F, // MOV BYTE PTR [ESI+0x113],0x7F ; line 2244
};
static PatchByteCode clownpieceA623USB_1HIT_patchByteCode = { clownpieceA623USB_1HIT_byteCode, 14, nullptr, 0 };
// clownpiece3SCUSB_SKIP declared in clownpiece.asm line 2253
static unsigned char clownpiece3SCUSB_SKIP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x63EBD4 ; line 2254
	0x55,                         // PUSH EBP ; line 2255
	0x89, 0xF1,                   // MOV ECX,ESI ; line 2256
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x63EBCF ; line 2257
};
static JumpTarget clownpiece3SCUSB_SKIP_jumpData[] = {
	{ 0x001, 0x63EBD4 },
	{ 0x009, 0x63EBCF },
};
static PatchByteCode clownpiece3SCUSB_SKIP_patchByteCode = { clownpiece3SCUSB_SKIP_byteCode, 13, clownpiece3SCUSB_SKIP_jumpData, 2 };
// clownpiece3SCUSB_ANGLES declared in clownpiece.asm line 2264
static unsigned char clownpiece3SCUSB_ANGLES_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2265
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 2266
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00, // FLD DWORD PTR [ESI+0xEC] ; line 2267
};
static PatchByteCode clownpiece3SCUSB_ANGLES_patchByteCode = { clownpiece3SCUSB_ANGLES_byteCode, 14, nullptr, 0 };
// clownpiece3SCUSB_IDK declared in clownpiece.asm line 2271
static unsigned char clownpiece3SCUSB_IDK_byteCode[] = {
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 2272
	0xD9, 0xEE,                         // FLDZ ; line 2273
	0xD9, 0x9E, 0x2C, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESI+0x12C] ; line 2274
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x63EB7A ; line 2275
};
static JumpTarget clownpiece3SCUSB_IDK_jumpData[] = {
	{ 0x00F, 0x63EB7A },
};
static PatchByteCode clownpiece3SCUSB_IDK_patchByteCode = { clownpiece3SCUSB_IDK_byteCode, 19, clownpiece3SCUSB_IDK_jumpData, 1 };
// clownpiece3SCUSB_UNCANCEL declared in clownpiece.asm line 2280
static unsigned char clownpiece3SCUSB_UNCANCEL_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x63EBCA ; line 2281
};
static JumpTarget clownpiece3SCUSB_UNCANCEL_jumpData[] = {
	{ 0x001, 0x63EBCA },
};
static PatchByteCode clownpiece3SCUSB_UNCANCEL_patchByteCode = { clownpiece3SCUSB_UNCANCEL_byteCode, 5, clownpiece3SCUSB_UNCANCEL_jumpData, 1 };
// clownpiece3SCUSB_HITS declared in clownpiece.asm line 2285
static unsigned char clownpiece3SCUSB_HITS_byteCode[] = {
	0x80, 0xBE, 0x13, 0x01, 0x00, 0x00, 0x7F,             // CMP BYTE PTR [ESI+0x113],0x7F ; line 2286
	0x0F, 0x86, 0x1D, 0x00, 0x00, 0x00,                   // JNA SHORT LABEL1 ; line 2287
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,             // CMP BYTE PTR [ESI+0x194],0x00 ; line 2288
	0x0F, 0x85, 0x10, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2289
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0x7F,             // MOV BYTE PTR [ESI+0x113],0x7F ; line 2290
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x59, 0x00, // MOV WORD PTR [ESI+0x140],0x0059 ; line 2291
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                   // MOV EAX,[ESI+0x190] ; line 2293
};
static PatchByteCode clownpiece3SCUSB_HITS_patchByteCode = { clownpiece3SCUSB_HITS_byteCode, 48, nullptr, 0 };
// clownpiece115 declared in clownpiece.asm line 2308
static unsigned char clownpiece115_byteCode[] = {
	0xD8, 0x0D, 0x84, 0xBB, 0x85, 0x00, // FMUL DWORD PTR [0x85BB84] ; line 2309
};
static PatchByteCode clownpiece115_patchByteCode = { clownpiece115_byteCode, 6, nullptr, 0 };
// clownpiece116 declared in clownpiece.asm line 2313
static unsigned char clownpiece116_byteCode[] = {
	0xD8, 0x0D, 0x94, 0xAE, 0x85, 0x00, // FMUL DWORD PTR [0x85AE94] ; line 2314
};
static PatchByteCode clownpiece116_patchByteCode = { clownpiece116_byteCode, 6, nullptr, 0 };
// clownpiece117 declared in clownpiece.asm line 2322
static unsigned char clownpiece117_byteCode[] = {
	0xBD, 0x04, 0x00, 0x00, 0x00, // MOV EBP,0x00000004 ; line 2323
};
static PatchByteCode clownpiece117_patchByteCode = { clownpiece117_byteCode, 5, nullptr, 0 };
// clownpiece118 declared in clownpiece.asm line 2328
static unsigned char clownpiece118_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD0, 0x07, 0x00, 0x00, 0x14, 0x00, // CMP WORD PTR [ESI+0x7D0],0x0014 ; line 2329
};
static PatchByteCode clownpiece118_patchByteCode = { clownpiece118_byteCode, 9, nullptr, 0 };
// clownpiece119 declared in clownpiece.asm line 2334
static unsigned char clownpiece119_byteCode[] = {
	0x66, 0x81, 0xBE, 0xD0, 0x07, 0x00, 0x00, 0x09, 0x00, // CMP WORD PTR [ESI+0x7D0],0x09 ; line 2335
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x62E6BC ; line 2336
};
static JumpTarget clownpiece119_jumpData[] = {
	{ 0x00A, 0x62E6BC },
};
static PatchByteCode clownpiece119_patchByteCode = { clownpiece119_byteCode, 14, clownpiece119_jumpData, 1 };
// clownpiece1SCRING_FLD declared in clownpiece.asm line 2344
static unsigned char clownpiece1SCRING_FLD_byteCode[] = {
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 2345
};
static PatchByteCode clownpiece1SCRING_FLD_patchByteCode = { clownpiece1SCRING_FLD_byteCode, 6, nullptr, 0 };
// clownpiece1SCRING_TRANSP0 declared in clownpiece.asm line 2351
static unsigned char clownpiece1SCRING_TRANSP0_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 2352
	0x8B, 0x86, 0x5C, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x35C] ; line 2353
	0xD9, 0x40, 0x04,                                     // FLD DWORD PTR [EAX+0x04] ; line 2354
	0xDC, 0x0D, 0x08, 0xC0, 0x85, 0x00,                   // FMUL QWORD PTR [0x85C008] ; line 2355
	0xD9, 0x58, 0x04,                                     // FSTP DWORD PTR [EAX+0x04] ; line 2356
	0x61,                                                 // POPAD ; line 2357
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x63C87F ; line 2358
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x140],0x01 ; line 2360
	0x0F, 0x85, 0x2A, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2361
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x194],0x00 ; line 2362
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x00 ; line 2363
	0x0F, 0x85, 0x14, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 2364
	0x80, 0xBE, 0x13, 0x01, 0x00, 0x00, 0xA0,             // CMP BYTE PTR [ESI+0x113],0xA0 ; line 2365
	0x0F, 0x87, 0x07, 0x00, 0x00, 0x00,                   // JA SHORT LABEL1 ; line 2366
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xA0,             // MOV BYTE PTR [ESI+0x113],0xA0 ; line 2367
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x35C] ; line 2369
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x63C82F ; line 2371
};
static JumpTarget clownpiece1SCRING_TRANSP0_jumpData[] = {
	{ 0x016, 0x63C87F },
	{ 0x05A, 0x63C82F },
};
static PatchByteCode clownpiece1SCRING_TRANSP0_patchByteCode = { clownpiece1SCRING_TRANSP0_byteCode, 94, clownpiece1SCRING_TRANSP0_jumpData, 2 };
// clownpiece1SCRING_N declared in clownpiece.asm line 2377
static unsigned char clownpiece1SCRING_N_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2378
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABELA ; line 2379
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 2380
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0x11C] ; line 2382
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x140],0x00 ; line 2384
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                         // JE SHORT LABEL1 ; line 2385
	0xE9, 0x33, 0x00, 0x00, 0x00,                               // JMP LABEL2 ; line 2386
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2388
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 2389
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x113],0x00 ; line 2390
	0x60,                                                       // PUSHAD ; line 2392
	0x8B, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x113] ; line 2393
	0x80, 0xF8, 0xE6,                                           // CMP AL,0xE6 ; line 2394
	0x0F, 0x86, 0x02, 0x00, 0x00, 0x00,                         // JNA SHORT LABEL4 ; line 2395
	0xB0, 0xE6,                                                 // MOV AL,0xE6 ; line 2396
	0x80, 0xC0, 0x19,                                           // ADD AL,0x19 ; line 2398
	0x88, 0x86, 0x13, 0x01, 0x00, 0x00,                         // MOV BYTE PTR [ESI+0x113],AL ; line 2399
	0x61,                                                       // POPAD ; line 2400
	0xD9, 0xEE,                                                 // FLDZ ; line 2402
	0x90,                                                       // NOP ; line 2403
	0x53,                                                       // PUSH EBX ; line 2405
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 2406
	0xD9, 0x96, 0x24, 0x01, 0x00, 0x00,                         // FST DWORD PTR [ESI+0x124] ; line 2407
	0xD9, 0x9E, 0x28, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x128] ; line 2408
	0xD9, 0x9E, 0x20, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x120] ; line 2409
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x63C7C8 ; line 2411
};
static JumpTarget clownpiece1SCRING_N_jumpData[] = {
	{ 0x07D, 0x63C7C8 },
};
static PatchByteCode clownpiece1SCRING_N_patchByteCode = { clownpiece1SCRING_N_byteCode, 129, clownpiece1SCRING_N_jumpData, 1 };
// clownpiece1SCRING_ANGLE declared in clownpiece.asm line 2419
static unsigned char clownpiece1SCRING_ANGLE_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x62E1BB ; line 2420
	0xD9, 0x05, 0x84, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB84] ; line 2421
	0x6A, 0x03,                         // PUSH 0x03 ; line 2422
	0xBA, 0x3C, 0x05, 0x00, 0x00,       // MOV EDX,0x0000053C ; line 2423
	0xD9, 0x1C, 0x14,                   // FSTP DWORD PTR [ESP+EDX] ; line 2424
	0x8D, 0x14, 0x14,                   // LEA EDX,[ESP+EDX] ; line 2425
	0xD9, 0xEE,                         // FLDZ ; line 2426
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62E1E1 ; line 2427
};
static JumpTarget clownpiece1SCRING_ANGLE_jumpData[] = {
	{ 0x002, 0x62E1BB },
	{ 0x01C, 0x62E1E1 },
};
static PatchByteCode clownpiece1SCRING_ANGLE_patchByteCode = { clownpiece1SCRING_ANGLE_byteCode, 32, clownpiece1SCRING_ANGLE_jumpData, 2 };
// clownpiece1SCRING_ANGLEAIR declared in clownpiece.asm line 2429
static unsigned char clownpiece1SCRING_ANGLEAIR_byteCode[] = {
	0x0F, 0x85, 0xDE, 0xAD, 0xBE, 0xEF, // JNE 0x62F9D9 ; line 2430
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 2431
	0x6A, 0x03,                         // PUSH 0x03 ; line 2432
	0xBA, 0x28, 0x04, 0x00, 0x00,       // MOV EDX,0x00000428 ; line 2433
	0xD9, 0x1C, 0x14,                   // FSTP DWORD PTR [ESP+EDX] ; line 2434
	0x8D, 0x14, 0x14,                   // LEA EDX,[ESP+EDX] ; line 2435
	0xD9, 0xEE,                         // FLDZ ; line 2436
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62F9FF ; line 2437
};
static JumpTarget clownpiece1SCRING_ANGLEAIR_jumpData[] = {
	{ 0x002, 0x62F9D9 },
	{ 0x01C, 0x62F9FF },
};
static PatchByteCode clownpiece1SCRING_ANGLEAIR_patchByteCode = { clownpiece1SCRING_ANGLEAIR_byteCode, 32, clownpiece1SCRING_ANGLEAIR_jumpData, 2 };
// clownpiece1SCRING_TRANSP declared in clownpiece.asm line 2444
static unsigned char clownpiece1SCRING_TRANSP_byteCode[] = {
	0x80, 0xF8, 0x10,                   // CMP AL,0x10 ; line 2445
	0x0F, 0x82, 0xDE, 0xAD, 0xBE, 0xEF, // JB 0x6365F7 ; line 2446
	0x80, 0xE8, 0x10,                   // SUB AL,0x10 ; line 2447
	0xB3, 0xF0,                         // MOV BL,0xF0 ; line 2448
};
static JumpTarget clownpiece1SCRING_TRANSP_jumpData[] = {
	{ 0x005, 0x6365F7 },
};
static PatchByteCode clownpiece1SCRING_TRANSP_patchByteCode = { clownpiece1SCRING_TRANSP_byteCode, 14, clownpiece1SCRING_TRANSP_jumpData, 1 };
// clownpiece134 declared in clownpiece.asm line 2461
static unsigned char clownpiece134_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x637CB6 ; line 2462
};
static JumpTarget clownpiece134_jumpData[] = {
	{ 0x001, 0x637CB6 },
};
static PatchByteCode clownpiece134_patchByteCode = { clownpiece134_byteCode, 5, clownpiece134_jumpData, 1 };
// clownpiece135 declared in clownpiece.asm line 2466
static unsigned char clownpiece135_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x637DF1 ; line 2467
};
static JumpTarget clownpiece135_jumpData[] = {
	{ 0x001, 0x637DF1 },
};
static PatchByteCode clownpiece135_patchByteCode = { clownpiece135_byteCode, 5, clownpiece135_jumpData, 1 };
// clownpiece136 declared in clownpiece.asm line 2471
static unsigned char clownpiece136_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x6365F7 ; line 2472
};
static JumpTarget clownpiece136_jumpData[] = {
	{ 0x001, 0x6365F7 },
};
static PatchByteCode clownpiece136_patchByteCode = { clownpiece136_byteCode, 5, clownpiece136_jumpData, 1 };
// clownpiece3SCDISC declared in clownpiece.asm line 2479
static unsigned char clownpiece3SCDISC_byteCode[] = {
	0x66, 0x9C,                               // PUSHF ; line 2481
	0x60,                                     // PUSHAD ; line 2482
	0x81, 0xEC, 0x40, 0x00, 0x00, 0x00,       // SUB ESP,0x40 ; line 2484
	0xE8, 0x25, 0x00, 0x00, 0x00,             // CALL LABEL2 ; line 2485
	0x81, 0xC4, 0x40, 0x00, 0x00, 0x00,       // ADD ESP,0x40 ; line 2486
	0x61,                                     // POPAD ; line 2494
	0x66, 0x9D,                               // POPF ; line 2495
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x62EF79 ; line 2497
	0x6A, 0x03,                               // PUSH 0x03 ; line 2499
	0xD9, 0xEE,                               // FLDZ ; line 2500
	0xD9, 0x9C, 0x24, 0x30, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x130] ; line 2501
	0x8D, 0x8C, 0x24, 0x30, 0x01, 0x00, 0x00, // LEA ECX,[ESP+0x130] ; line 2502
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x62EF0C ; line 2503
	0xE8, 0xE4, 0xFF, 0xFF, 0xFF,             // CALL LABEL1 ; line 2505
	0xE8, 0xDF, 0xFF, 0xFF, 0xFF,             // CALL LABEL1 ; line 2506
	0xE8, 0xDA, 0xFF, 0xFF, 0xFF,             // CALL LABEL1 ; line 2507
	0xC3,                                     // RET ; line 2508
};
static JumpTarget clownpiece3SCDISC_jumpData[] = {
	{ 0x018, 0x62EF79 },
	{ 0x02F, 0x62EF0C },
};
static PatchByteCode clownpiece3SCDISC_patchByteCode = { clownpiece3SCDISC_byteCode, 67, clownpiece3SCDISC_jumpData, 2 };
// clownpiece3SCDISC_RET declared in clownpiece.asm line 2512
static unsigned char clownpiece3SCDISC_RET_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF, // CALL 0x46EB30 ; line 2513
	0xC3,                         // RET ; line 2514
};
static JumpTarget clownpiece3SCDISC_RET_jumpData[] = {
	{ 0x001, 0x46EB30 },
};
static PatchByteCode clownpiece3SCDISC_RET_patchByteCode = { clownpiece3SCDISC_RET_byteCode, 6, clownpiece3SCDISC_RET_jumpData, 1 };
// clownpiece3SCDISC_3 declared in clownpiece.asm line 2516
static unsigned char clownpiece3SCDISC_3_byteCode[] = {
	0xD9, 0xE8,                               // FLD1 ; line 2517
	0xD9, 0x9C, 0x24, 0x34, 0x01, 0x00, 0x00, // FSTP DWORD PTR [ESP+0x134] ; line 2518
	0x6A, 0x03,                               // PUSH 0x03 ; line 2519
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00, // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 2520
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 2521
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x62EF95 ; line 2522
};
static JumpTarget clownpiece3SCDISC_3_jumpData[] = {
	{ 0x019, 0x62EF95 },
};
static PatchByteCode clownpiece3SCDISC_3_patchByteCode = { clownpiece3SCDISC_3_byteCode, 29, clownpiece3SCDISC_3_jumpData, 1 };
// clownpiece138 declared in clownpiece.asm line 2528
static unsigned char clownpiece138_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2529
	0xD8, 0x86, 0xEC, 0x00, 0x00, 0x00, // FADD DWORD PTR [ESI+0xEC] ; line 2530
};
static PatchByteCode clownpiece138_patchByteCode = { clownpiece138_byteCode, 8, nullptr, 0 };
// clownpiece3SCDISC_0 declared in clownpiece.asm line 2535
static unsigned char clownpiece3SCDISC_0_byteCode[] = {
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,       // FLD DWORD PTR [ESI+0xF0] ; line 2536
	0xD9, 0x05, 0x84, 0xB9, 0x85, 0x00,       // FLD DWORD PTR [0x85B984] ; line 2537
	0x66, 0x8B, 0x86, 0x42, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x142] ; line 2538
	0x0F, 0xB7, 0xC0,                         // MOVZX EAX,AX ; line 2539
	0x50,                                     // PUSH EAX ; line 2540
	0xDB, 0x04, 0x24,                         // FILD DWORD PTR [ESP] ; line 2541
	0x58,                                     // POP EAX ; line 2542
	0xDE, 0xC9,                               // FMULP ST1 ; line 2543
	0xDE, 0xC1,                               // FADDP ; line 2544
	0x66, 0xFF, 0x86, 0x42, 0x01, 0x00, 0x00, // INC WORD PTR [ESI+0x142] ; line 2545
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00, // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 2546
};
static PatchByteCode clownpiece3SCDISC_0_patchByteCode = { clownpiece3SCDISC_0_byteCode, 45, nullptr, 0 };
// clownpiece3SCDISC_1 declared in clownpiece.asm line 2550
static unsigned char clownpiece3SCDISC_1_byteCode[] = {
	0x66, 0x8B, 0x86, 0x40, 0x01, 0x00, 0x00, // MOV AX,[ESI+0x140] ; line 2551
	0x66, 0x81, 0xF8, 0x04, 0x00,             // CMP AX,0x0004 ; line 2552
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,       // JL 0x63120A ; line 2553
	0x66, 0x81, 0xF8, 0x0A, 0x00,             // CMP AX,0x000A ; line 2554
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,       // JG 0x63120A ; line 2555
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x62EEFA ; line 2556
};
static JumpTarget clownpiece3SCDISC_1_jumpData[] = {
	{ 0x00E, 0x63120A },
	{ 0x019, 0x63120A },
	{ 0x01E, 0x62EEFA },
};
static PatchByteCode clownpiece3SCDISC_1_patchByteCode = { clownpiece3SCDISC_1_byteCode, 34, clownpiece3SCDISC_1_jumpData, 3 };
// clownpiece137 declared in clownpiece.asm line 2562
static unsigned char clownpiece137_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 2563
};
static PatchByteCode clownpiece137_patchByteCode = { clownpiece137_byteCode, 6, nullptr, 0 };
// clownpiece3SCDISC_SKIP declared in clownpiece.asm line 2569
static unsigned char clownpiece3SCDISC_SKIP_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x63DF66 ; line 2570
	0x6A, 0x03,                               // PUSH 0x03 ; line 2571
	0xD9, 0x94, 0x24, 0x74, 0x02, 0x00, 0x00, // FST DWORD PTR [ESP+0x274] ; line 2572
};
static JumpTarget clownpiece3SCDISC_SKIP_jumpData[] = {
	{ 0x001, 0x63DF66 },
};
static PatchByteCode clownpiece3SCDISC_SKIP_patchByteCode = { clownpiece3SCDISC_SKIP_byteCode, 14, clownpiece3SCDISC_SKIP_jumpData, 1 };
// clownpieceA214WALK_5 declared in clownpiece.asm line 2579
static unsigned char clownpieceA214WALK_5_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x03, 0x00, // CMP WORD PTR [ESI+0x13E],0x03 ; line 2580
	0x0F, 0x8C, 0x08, 0x00, 0x00, 0x00,                   // JL SHORT LABEL1 ; line 2581
	0xD9, 0xEE,                                           // FLDZ ; line 2582
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF4] ; line 2583
	0x90,                                                 // NOP ; line 2585
};
static PatchByteCode clownpieceA214WALK_5_patchByteCode = { clownpieceA214WALK_5_byteCode, 24, nullptr, 0 };
// clownpieceA214WALK_6 declared in clownpiece.asm line 2592
static unsigned char clownpieceA214WALK_6_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, // CMP WORD PTR [ESI+0x140],0x04 ; line 2593
	0x0F, 0x8C, 0x07, 0x00, 0x00, 0x00,                   // JL SHORT LABEL1 ; line 2594
	0x80, 0xBE, 0xF4, 0x07, 0x00, 0x00, 0x00,             // CMP BYTE PTR [ESI+0x7F4],0x00 ; line 2595
	0x90,                                                 // NOP ; line 2597
};
static PatchByteCode clownpieceA214WALK_6_patchByteCode = { clownpieceA214WALK_6_byteCode, 23, nullptr, 0 };
// clownpieceA22ACURA_0 declared in clownpiece.asm line 2608
static unsigned char clownpieceA22ACURA_0_byteCode[] = {
	0xC6, 0x86, 0x8C, 0x01, 0x00, 0x00, 0x0B,             // MOV BYTE PTR [ESI+0x18C],0x0B ; line 2609
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x02 ; line 2610
	0x0F, 0x85, 0x12, 0x00, 0x00, 0x00,                   // JNE SHORT LABELA ; line 2611
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0xF8] ; line 2612
	0xD8, 0xA6, 0x00, 0x01, 0x00, 0x00,                   // FSUB DWORD PTR [ESI+0x100] ; line 2613
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                   // FSTP DWORD PTR [ESI+0xF8] ; line 2614
	0xBB, 0x01, 0x00, 0x00, 0x00,                         // MOV EBX,0x00000001 ; line 2616
};
static PatchByteCode clownpieceA22ACURA_0_patchByteCode = { clownpieceA22ACURA_0_byteCode, 45, nullptr, 0 };
// clownpieceA22ACURA_1 declared in clownpiece.asm line 2619
static unsigned char clownpieceA22ACURA_1_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2620
	0xD9, 0x96, 0xF4, 0x00, 0x00, 0x00, // FST DWORD PTR [ESI+0xF4] ; line 2621
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 2622
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62D532 ; line 2623
};
static JumpTarget clownpieceA22ACURA_1_jumpData[] = {
	{ 0x00F, 0x62D532 },
};
static PatchByteCode clownpieceA22ACURA_1_patchByteCode = { clownpieceA22ACURA_1_byteCode, 19, clownpieceA22ACURA_1_jumpData, 1 };
// clownpieceA22ACURA_2 declared in clownpiece.asm line 2626
static unsigned char clownpieceA22ACURA_2_byteCode[] = {
	0xDD, 0x05, 0x30, 0x78, 0x86, 0x00, // FLD QWORD PTR [0x867830] ; line 2627
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 2628
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62D532 ; line 2629
};
static JumpTarget clownpieceA22ACURA_2_jumpData[] = {
	{ 0x00D, 0x62D532 },
};
static PatchByteCode clownpieceA22ACURA_2_patchByteCode = { clownpieceA22ACURA_2_byteCode, 17, clownpieceA22ACURA_2_jumpData, 1 };
// clownpieceA22ACURA_3 declared in clownpiece.asm line 2632
static unsigned char clownpieceA22ACURA_3_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 2633
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00, // FSTP DWORD PTR [ESI+0xF8] ; line 2634
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x62D532 ; line 2635
};
static JumpTarget clownpieceA22ACURA_3_jumpData[] = {
	{ 0x00D, 0x62D532 },
};
static PatchByteCode clownpieceA22ACURA_3_patchByteCode = { clownpieceA22ACURA_3_byteCode, 17, clownpieceA22ACURA_3_jumpData, 1 };
// clownpieceA22ACURA_CREATION declared in clownpiece.asm line 2638
static unsigned char clownpieceA22ACURA_CREATION_byteCode[] = {
	0xD9, 0x86, 0xF8, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF8] ; line 2640
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2641
	0x0F, 0x85, 0x32, 0x00, 0x00, 0x00,                         // JNE LABEL4 ; line 2642
	0xD8, 0x0D, 0x28, 0xC2, 0x85, 0x00,                         // FMUL DWORD PTR [0x85C228] ; line 2643
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2644
	0xE9, 0xE0, 0x00, 0x00, 0x00,                               // JMP LABEL3 ; line 2645
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 2647
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,                         // JNL LABEL4 ; line 2648
	0xD8, 0x0D, 0x28, 0xC2, 0x85, 0x00,                         // FMUL DWORD PTR [0x85C228] ; line 2649
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2650
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x62D368 ; line 2652
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x10 ; line 2654
	0x0F, 0x85, 0x23, 0x00, 0x00, 0x00,                         // JNE LABEL6 ; line 2655
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2656
	0xD9, 0x05, 0x60, 0xB9, 0x85, 0x00,                         // FLD DWORD PTR [0x85B960] ; line 2657
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2658
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00,                         // FLD DWORD PTR [0x85AE94] ; line 2659
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2660
	0xE9, 0xC8, 0xFF, 0xFF, 0xFF,                               // JMP LABEL5 ; line 2661
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x24 ; line 2663
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,                         // JNL LABEL7 ; line 2664
	0xD8, 0x0D, 0x28, 0xC2, 0x85, 0x00,                         // FMUL DWORD PTR [0x85C228] ; line 2665
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2666
	0xE9, 0xA7, 0xFF, 0xFF, 0xFF,                               // JMP LABEL5 ; line 2667
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x24 ; line 2669
	0x0F, 0x85, 0x11, 0x00, 0x00, 0x00,                         // JNE LABEL8 ; line 2670
	0xD8, 0x0D, 0x0C, 0x15, 0x87, 0x00,                         // FMUL DWORD PTR [0x87150C] ; line 2671
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2672
	0xE9, 0x86, 0xFF, 0xFF, 0xFF,                               // JMP LABEL5 ; line 2673
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x38 ; line 2675
	0x0F, 0x8D, 0x11, 0x00, 0x00, 0x00,                         // JNL LABEL9 ; line 2676
	0xD8, 0x35, 0x28, 0xC2, 0x85, 0x00,                         // FDIV DWORD PTR [0x85C228] ; line 2677
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2678
	0xE9, 0x65, 0xFF, 0xFF, 0xFF,                               // JMP LABEL5 ; line 2679
	0xD8, 0x35, 0x38, 0x15, 0x87, 0x00,                         // FDIV DWORD PTR [0x871538] ; line 2681
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00,                         // FLD DWORD PTR [0x858830] ; line 2682
	0xD9, 0x9E, 0xF4, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF4] ; line 2683
	0xD9, 0x9E, 0xF8, 0x00, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0xF8] ; line 2684
	0xD9, 0x05, 0x6C, 0xBC, 0x85, 0x00,                         // FLD DWORD PTR [0x85BC6C] ; line 2685
	0xD9, 0x9E, 0x00, 0x01, 0x00, 0x00,                         // FSTP DWORD PTR [ESI+0x100] ; line 2686
	0xE9, 0x75, 0x00, 0x00, 0x00,                               // JMP LABEL1 ; line 2687
	0xD9, 0xEE,                                                 // FLDZ ; line 2690
	0x6A, 0x03,                                                 // PUSH 0x03 ; line 2691
	0xD9, 0x94, 0x24, 0xB8, 0x02, 0x00, 0x00,                   // FST DWORD PTR [ESP+0x2B8] ; line 2692
	0x8D, 0x8C, 0x24, 0xB8, 0x02, 0x00, 0x00,                   // LEA ECX,[ESP+0x2B8] ; line 2693
	0xD9, 0x9C, 0x24, 0xBC, 0x02, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x2BC] ; line 2694
	0x51,                                                       // PUSH ECX ; line 2695
	0xD9, 0xE8,                                                 // FLD1 ; line 2696
	0x6A, 0x01,                                                 // PUSH 0x01 ; line 2697
	0xD9, 0x9C, 0x24, 0xC8, 0x02, 0x00, 0x00,                   // FSTP DWORD PTR [ESP+0x2C8] ; line 2698
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 2699
	0x0F, 0xB6, 0x96, 0x04, 0x01, 0x00, 0x00,                   // MOVZX EDX,BYTE PTR [ESI+0x104] ; line 2700
	0xD9, 0x86, 0xF0, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xF0] ; line 2701
	0x52,                                                       // PUSH EDX ; line 2702
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,                         // SUB ESP,0x08 ; line 2703
	0xD9, 0x5C, 0x24, 0x04,                                     // FSTP DWORD PTR [ESP+0x04] ; line 2704
	0xD9, 0x86, 0xEC, 0x00, 0x00, 0x00,                         // FLD DWORD PTR [ESI+0xEC] ; line 2705
	0xD9, 0x1C, 0x24,                                           // FSTP DWORD PTR [ESP] ; line 2706
	0x68, 0x3B, 0x03, 0x00, 0x00,                               // PUSH 0x0000033B ; line 2707
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x46EB30 ; line 2708
	0x80, 0xBE, 0xAF, 0x06, 0x00, 0x00, 0x01,                   // CMP BYTE PTR [ESI+0x6AF],0x01 ; line 2709
	0x0F, 0x8C, 0x16, 0x00, 0x00, 0x00,                         // JL LABEL1 ; line 2710
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 2711
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01,                   // MOV BYTE PTR [ESI+0x194],0x01 ; line 2712
	0xE9, 0xC7, 0xFE, 0xFF, 0xFF,                               // JMP LABEL5 ; line 2714
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 2716
	0xE9, 0xEF, 0xFF, 0xFF, 0xFF,                               // JMP LABEL2 ; line 2717
	0x90,                                                       // NOP ; line 2718
};
static JumpTarget clownpieceA22ACURA_CREATION_jumpData[] = {
	{ 0x044, 0x62D368 },
	{ 0x155, 0x46EB30 },
};
static PatchByteCode clownpieceA22ACURA_CREATION_patchByteCode = { clownpieceA22ACURA_CREATION_byteCode, 393, clownpieceA22ACURA_CREATION_jumpData, 2 };
// clownpieceA22ACURA_4 declared in clownpiece.asm line 2723
static unsigned char clownpieceA22ACURA_4_byteCode[] = {
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 2724
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0x3A, 0x02, // CMP WORD PTR [ECX+0x13C],0x023A ; line 2725
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x6365F7 ; line 2726
	0x66, 0x81, 0xB9, 0x3C, 0x01, 0x00, 0x00, 0x3D, 0x02, // CMP WORD PTR [ECX+0x13C],0x023D ; line 2727
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF,                   // JG 0x6365F7 ; line 2728
	0x6A, 0x00,                                           // PUSH 0x00 ; line 2729
	0x6A, 0x00,                                           // PUSH 0x00 ; line 2730
	0x89, 0xF1,                                           // MOV ECX,ESI ; line 2731
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x48CF60 ; line 2732
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x13E],0x02 ; line 2733
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x63C40C ; line 2734
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x63C49A ; line 2735
};
static JumpTarget clownpieceA22ACURA_4_jumpData[] = {
	{ 0x011, 0x6365F7 },
	{ 0x020, 0x6365F7 },
	{ 0x02B, 0x48CF60 },
	{ 0x03A, 0x63C40C },
	{ 0x03F, 0x63C49A },
};
static PatchByteCode clownpieceA22ACURA_4_patchByteCode = { clownpieceA22ACURA_4_byteCode, 67, clownpieceA22ACURA_4_jumpData, 5 };
// clownpiece130 declared in clownpiece.asm line 2761
static unsigned char clownpiece130_byteCode[] = {
	0x80, 0xBE, 0xAF, 0x06, 0x00, 0x00, 0x02, // CMP BYTE PTR [ESI+0x6AF],0x02 ; line 2762
};
static PatchByteCode clownpiece130_patchByteCode = { clownpiece130_byteCode, 7, nullptr, 0 };
// clownpiece131 declared in clownpiece.asm line 2764
static unsigned char clownpiece131_byteCode[] = {
	0x80, 0xBE, 0xAF, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6AF],0x01 ; line 2765
};
static PatchByteCode clownpiece131_patchByteCode = { clownpiece131_byteCode, 7, nullptr, 0 };
// clownpiece126 declared in clownpiece.asm line 2774
static unsigned char clownpiece126_byteCode[] = {
	0xDC, 0x25, 0x60, 0xF3, 0x85, 0x00, // FSUB QWORD PTR [0x85F360] ; line 2775
};
static PatchByteCode clownpiece126_patchByteCode = { clownpiece126_byteCode, 6, nullptr, 0 };
// clownpiece127 declared in clownpiece.asm line 2780
static unsigned char clownpiece127_byteCode[] = {
	0xD9, 0x05, 0x00, 0x9F, 0x85, 0x00, // FLD DWORD PTR [0x859F00] ; line 2781
};
static PatchByteCode clownpiece127_patchByteCode = { clownpiece127_byteCode, 6, nullptr, 0 };
// clownpiece128 declared in clownpiece.asm line 2786
static unsigned char clownpiece128_byteCode[] = {
	0xD9, 0x05, 0xF0, 0x89, 0x85, 0x00, // FLD DWORD PTR [0x8589F0] ; line 2787
};
static PatchByteCode clownpiece128_patchByteCode = { clownpiece128_byteCode, 6, nullptr, 0 };
// clownpiece129 declared in clownpiece.asm line 2793
static unsigned char clownpiece129_byteCode[] = {
	0xC6, 0x86, 0x13, 0x01, 0x00, 0x00, 0xFF, // MOV BYTE PTR [ESI+0x113],0xFF ; line 2794
};
static PatchByteCode clownpiece129_patchByteCode = { clownpiece129_byteCode, 7, nullptr, 0 };
// clownpiece120 declared in clownpiece.asm line 2801
static unsigned char clownpiece120_byteCode[] = {
	0xD9, 0x05, 0x10, 0xC6, 0x85, 0x00, // FLD DWORD PTR [0x85C610] ; line 2802
};
static PatchByteCode clownpiece120_patchByteCode = { clownpiece120_byteCode, 6, nullptr, 0 };
// clownpiece121 declared in clownpiece.asm line 2806
static unsigned char clownpiece121_byteCode[] = {
	0xD9, 0x05, 0x40, 0x8F, 0x85, 0x00, // FLD DWORD PTR [0x858F40] ; line 2807
};
static PatchByteCode clownpiece121_patchByteCode = { clownpiece121_byteCode, 6, nullptr, 0 };
// clownpiece122 declared in clownpiece.asm line 2811
static unsigned char clownpiece122_byteCode[] = {
	0xD9, 0x05, 0x40, 0x8F, 0x85, 0x00, // FLD DWORD PTR [0x858F40] ; line 2812
};
static PatchByteCode clownpiece122_patchByteCode = { clownpiece122_byteCode, 6, nullptr, 0 };
// clownpiece123 declared in clownpiece.asm line 2817
static unsigned char clownpiece123_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 2818
};
static PatchByteCode clownpiece123_patchByteCode = { clownpiece123_byteCode, 6, nullptr, 0 };
// clownpiece124 declared in clownpiece.asm line 2823
static unsigned char clownpiece124_byteCode[] = {
	0xDC, 0x25, 0x20, 0x88, 0x85, 0x00, // FSUB QWORD PTR [0x858820] ; line 2824
};
static PatchByteCode clownpiece124_patchByteCode = { clownpiece124_byteCode, 6, nullptr, 0 };
// clownpiece125 declared in clownpiece.asm line 2829
static unsigned char clownpiece125_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 2830
	0xD8, 0x25, 0x40, 0x8F, 0x85, 0x00, // FSUB DWORD PTR [0x858F40] ; line 2831
};
static PatchByteCode clownpiece125_patchByteCode = { clownpiece125_byteCode, 8, nullptr, 0 };
// clownpiece66B declared in clownpiece.asm line 2838
static unsigned char clownpiece66B_byteCode[] = {
	0x66, 0x81, 0xBE, 0x3E, 0x01, 0x00, 0x00, 0x01, 0x00, // CMP WORD PTR [ESI+0x13E],0x01 ; line 2839
	0x0F, 0x8C, 0xDE, 0xAD, 0xBE, 0xEF,                   // JL 0x624E4B ; line 2840
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x624E0F ; line 2841
};
static JumpTarget clownpiece66B_jumpData[] = {
	{ 0x00B, 0x624E4B },
	{ 0x010, 0x624E0F },
};
static PatchByteCode clownpiece66B_patchByteCode = { clownpiece66B_byteCode, 20, clownpiece66B_jumpData, 2 };
// clownpiece66B_LOOP declared in clownpiece.asm line 2844
static unsigned char clownpiece66B_LOOP_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x8 ; line 2845
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x624F03 ; line 2846
};
static JumpTarget clownpiece66B_LOOP_jumpData[] = {
	{ 0x00B, 0x624F03 },
};
static PatchByteCode clownpiece66B_LOOP_patchByteCode = { clownpiece66B_LOOP_byteCode, 15, clownpiece66B_LOOP_jumpData, 1 };
// clownpiece66B_SOUND declared in clownpiece.asm line 2849
static unsigned char clownpiece66B_SOUND_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 2850
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 2851
	0x6A, 0x1D,                                                 // PUSH 0x1D ; line 2852
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 2853
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x439490 ; line 2854
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                               // JMP 0x624F0C ; line 2856
};
static JumpTarget clownpiece66B_SOUND_jumpData[] = {
	{ 0x015, 0x439490 },
	{ 0x01A, 0x624F0C },
};
static PatchByteCode clownpiece66B_SOUND_patchByteCode = { clownpiece66B_SOUND_byteCode, 30, clownpiece66B_SOUND_jumpData, 2 };
// clownpiece66B_X declared in clownpiece.asm line 2859
static unsigned char clownpiece66B_X_byteCode[] = {
	0xD9, 0x05, 0xB0, 0xB3, 0x85, 0x00, // FLD DWORD PTR [0x85B3B0] ; line 2860
};
static PatchByteCode clownpiece66B_X_patchByteCode = { clownpiece66B_X_byteCode, 6, nullptr, 0 };
// clownpieceJ8A_X declared in clownpiece.asm line 2864
static unsigned char clownpieceJ8A_X_byteCode[] = {
	0xD9, 0x05, 0x90, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C590] ; line 2865
};
static PatchByteCode clownpieceJ8A_X_patchByteCode = { clownpieceJ8A_X_byteCode, 6, nullptr, 0 };


// clownpiece.asm
std::array<PatchSkeleton, 342> patchList = {
	PatchSkeleton{ clownpieceID_FIX_patchByteCode,             (void *)0x61DFF8,  7}, // Declared line 11. Patch on CMP [ESI+0x196], CX
	PatchSkeleton{ clownpiece0_patchByteCode,                  (void *)0x635B20,  9}, // Declared line 17. Patch on MOV WORD PTR [ESI+0x892], 0
	PatchSkeleton{ clownpiece1_patchByteCode,                  (void *)0x630321,  6}, // Declared line 23. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpieceDASHJUMP_patchByteCode,           (void *)0x61F63B,  6}, // Declared line 28. Patch on FLD ST, DWORD PTR [0x85BDB8]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x62159D,  6}, // Declared line 37. Patch on FLD ST, DWORD PTR [0x85DC08]
	PatchSkeleton{ clownpiece3_patchByteCode,                  (void *)0x6214CD,  6}, // Declared line 43. Patch on FLD ST, DWORD PTR [0x85B9BC]
	PatchSkeleton{ clownpiece4_patchByteCode,                  (void *)0x621574,  7}, // Declared line 50. Patch on MOV [ESI+0x892], DI
	PatchSkeleton{ clownpiece5_patchByteCode,                  (void *)0x621500,  7}, // Declared line 56. Patch on CMP DWORD PTR [ESI+0x144], 9
	PatchSkeleton{ clownpiece6_FLD15_patchByteCode,            (void *)0x621087,  6}, // Declared line 62. Patch on FLD ST, DWORD PTR [0x85D8A0]
	PatchSkeleton{ clownpiece6_FLD15_patchByteCode,            (void *)0x620F25,  6}, // Declared line 65. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ clownpiece7_patchByteCode,                  (void *)0x62108D,  9}, // Declared line 73. Patch on MOV WORD PTR [ESI+0x892], 0
	PatchSkeleton{ clownpiece8_patchByteCode,                  (void *)0x621120,  6}, // Declared line 79. Patch on FSUB ST, QWORD PTR [0x857F90]
	PatchSkeleton{ clownpiece9_patchByteCode,                  (void *)0x621155,  7}, // Declared line 86. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ clownpiece10_patchByteCode,                 (void *)0x62114C,  7}, // Declared line 92. Patch on CMP DWORD PTR [ESI+0x144], 0x12
	PatchSkeleton{ clownpiece6_FLD15_patchByteCode,            (void *)0x6210FC,  6}, // Declared line 95. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ clownpiece10A_patchByteCode,                (void *)0x6211D4,  7}, // Declared line 102. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ clownpiece11_patchByteCode,                 (void *)0x6211CB,  7}, // Declared line 108. Patch on CMP DWORD PTR [ESI+0x144], 0x18
	PatchSkeleton{ clownpiece12_patchByteCode,                 (void *)0x620F50,  6}, // Declared line 114. Patch on FSUB ST, QWORD PTR [0x858820]
	PatchSkeleton{ clownpiece13_patchByteCode,                 (void *)0x620EB7,  6}, // Declared line 120. Patch on FLD ST, DWORD PTR [0x85BC30]
	PatchSkeleton{ clownpiece14_patchByteCode,                 (void *)0x621950,  6}, // Declared line 126. Patch on FLD ST, DWORD PTR [0x85D150]
	PatchSkeleton{ clownpiece15_patchByteCode,                 (void *)0x63276A,  7}, // Declared line 133. Patch on MOV BYTE PTR [ESI+0x194], 1
	PatchSkeleton{ clownpiece16_patchByteCode,                 (void *)0x624EDF,  7}, // Declared line 140. Patch on MOV BYTE PTR [ESI+0x113], 0
	PatchSkeleton{ clownpiece18_patchByteCode,                 (void *)0x628396,  9}, // Declared line 152. Patch on MOV WORD PTR [ESI+0x892], 0
	PatchSkeleton{ clownpiece18A_patchByteCode,                (void *)0x6281BA,  7}, // Declared line 159. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x6369B8,  6}, // Declared line 168. Patch on FADD ST, QWORD PTR [0x858820]
	PatchSkeleton{ clownpiece20_patchByteCode,                 (void *)0x636A94,  7}, // Declared line 174. Patch on CMP DWORD PTR [ESI+0x144], 0x1E
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x636F68,  6}, // Declared line 177. Patch on FADD ST, QWORD PTR [0x858820]
	PatchSkeleton{ clownpiece20_patchByteCode,                 (void *)0x637044,  7}, // Declared line 180. Patch on CMP DWORD PTR [ESI+0x144], 0x1E
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x62519F,  6}, // Declared line 186. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x625564,  6}, // Declared line 188. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x625B2A,  6}, // Declared line 190. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x625EEF,  6}, // Declared line 192. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x626693,  6}, // Declared line 194. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x626A4C,  6}, // Declared line 196. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x62799C,  6}, // Declared line 198. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece21_Cspd_patchByteCode,            (void *)0x627D4C,  6}, // Declared line 200. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece22_patchByteCode,                 (void *)0x6363DB,  6}, // Declared line 206. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpiece23_FLD12_patchByteCode,           (void *)0x6363EC,  6}, // Declared line 212. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x6221DC,  6}, // Declared line 500. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1_patchByteCode,            (void *)0x6221E2,  7}, // Declared line 501. Patch on MOVSX EDX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_A_patchByteCode,          (void *)0x622379,  5}, // Declared line 502. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x622275,  7}, // Declared line 503. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x622291,  6}, // Declared line 504. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceC5B_3_patchByteCode,              (void *)0x6225C9,  5}, // Declared line 506. Patch on CMP AX, BP
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x6225F1,  6}, // Declared line 507. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1B_patchByteCode,           (void *)0x6225F7,  7}, // Declared line 508. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_B_patchByteCode,          (void *)0x622720,  5}, // Declared line 509. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x62268A,  7}, // Declared line 510. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x6226A6,  6}, // Declared line 511. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x622AA6,  6}, // Declared line 513. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1B_patchByteCode,           (void *)0x622AAC,  7}, // Declared line 514. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_C_patchByteCode,          (void *)0x622BDF,  5}, // Declared line 515. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x622B3D,  7}, // Declared line 516. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x622B59,  6}, // Declared line 517. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceC5B_3_patchByteCode,              (void *)0x622E91,  5}, // Declared line 519. Patch on CMP AX, BP
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x622EB9,  6}, // Declared line 520. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1B_patchByteCode,           (void *)0x622EBF,  7}, // Declared line 521. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_D_patchByteCode,          (void *)0x622FE6,  5}, // Declared line 522. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x622F50,  7}, // Declared line 523. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x622F6C,  6}, // Declared line 524. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x62336D,  7}, // Declared line 526. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_2B_patchByteCode,              (void *)0x623374,  5}, // Declared line 527. Patch on ADD ESP, 4
	PatchSkeleton{ clownpiece5B_NEW2_E_patchByteCode,          (void *)0x623409,  5}, // Declared line 528. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x623389,  6}, // Declared line 529. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceC5B_3_patchByteCode,              (void *)0x6236BB,  5}, // Declared line 531. Patch on CMP AX, BP
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x6236E3,  7}, // Declared line 532. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_2B_patchByteCode,              (void *)0x6236EA,  5}, // Declared line 533. Patch on ADD ESP, 4
	PatchSkeleton{ clownpiece5B_NEW2_F_patchByteCode,          (void *)0x623776,  5}, // Declared line 534. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x6236FF,  6}, // Declared line 535. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x623B8C,  6}, // Declared line 537. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1_patchByteCode,            (void *)0x623B92,  7}, // Declared line 538. Patch on MOVSX EDX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_G_patchByteCode,          (void *)0x623CC7,  5}, // Declared line 539. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x623C25,  7}, // Declared line 540. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x623C41,  6}, // Declared line 541. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceC5B_3B_patchByteCode,             (void *)0x623F98,  5}, // Declared line 543. Patch on CMP AX, BP
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x623FC0,  6}, // Declared line 544. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1B_patchByteCode,           (void *)0x623FC6,  7}, // Declared line 545. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_H_patchByteCode,          (void *)0x6240FB,  5}, // Declared line 546. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x624059,  7}, // Declared line 547. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x624075,  6}, // Declared line 548. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x624549,  6}, // Declared line 550. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1_patchByteCode,            (void *)0x62454F,  7}, // Declared line 551. Patch on MOVSX EDX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_I_patchByteCode,          (void *)0x624692,  5}, // Declared line 552. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x6245E4,  7}, // Declared line 553. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x624600,  6}, // Declared line 554. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceC5B_3B_patchByteCode,             (void *)0x62496F,  5}, // Declared line 556. Patch on CMP AX, BP
	PatchSkeleton{ clownpiece5B_NEW0_patchByteCode,            (void *)0x624997,  6}, // Declared line 557. Patch on MOV EAX, [ESI+0x170]
	PatchSkeleton{ clownpiece5B_NEW1B_patchByteCode,           (void *)0x62499D,  7}, // Declared line 558. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5B_NEW2_J_patchByteCode,          (void *)0x624AE0,  5}, // Declared line 559. Patch on PUSH 0x320
	PatchSkeleton{ clownpiece5B_RNG0_patchByteCode,            (void *)0x624A32,  7}, // Declared line 560. Patch on PUSH 9
	PatchSkeleton{ clownpiece5B_RNG1_patchByteCode,            (void *)0x624A4E,  6}, // Declared line 561. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece5B_GRAZERES_patchByteCode,        (void *)0x636477,  9}, // Declared line 569. Patch on PUSH 2
	PatchSkeleton{ clownpiece24A_patchByteCode,                (void *)0x6221B8, 10}, // Declared line 595. Patch on CMP AX, 9
	PatchSkeleton{ clownpiece24B_patchByteCode,                (void *)0x622A82, 10}, // Declared line 597. Patch on CMP AX, 9
	PatchSkeleton{ clownpiece24C_patchByteCode,                (void *)0x623349, 10}, // Declared line 599. Patch on CMP AX, 9
	PatchSkeleton{ clownpiece24D_patchByteCode,                (void *)0x623B68, 10}, // Declared line 601. Patch on CMP AX, 9
	PatchSkeleton{ clownpiece25_patchByteCode,                 (void *)0x624529,  6}, // Declared line 603. Patch on JNE 0x0062491C
	PatchSkeleton{ clownpiece26_Bframe_patchByteCode,          (void *)0x62212A,  8}, // Declared line 614. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ clownpiece26_Bframe_patchByteCode,          (void *)0x6229F1,  8}, // Declared line 616. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ clownpiece26_Bframe_patchByteCode,          (void *)0x6232BE,  8}, // Declared line 618. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ clownpiece26_Bframe_patchByteCode,          (void *)0x623AD7,  8}, // Declared line 620. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ clownpiece26_Bframe1_patchByteCode,         (void *)0x62448F,  8}, // Declared line 622. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x63777B,  6}, // Declared line 626. Patch on FADD ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x6375E5,  6}, // Declared line 628. Patch on FLD ST, DWORD PTR [0x85C110]
	PatchSkeleton{ clownpiece27_patchByteCode,                 (void *)0x6375D6,  6}, // Declared line 635. Patch on FLD ST, DWORD PTR [0x871544]
	PatchSkeleton{ clownpiece28_patchByteCode,                 (void *)0x6380E7,  7}, // Declared line 641. Patch on CMP DWORD PTR [ESI+0x144], 0x1E
	PatchSkeleton{ clownpiece29_patchByteCode,                 (void *)0x637F6A,  6}, // Declared line 647. Patch on FSUB ST, QWORD PTR [0x858EA8]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x637F7B,  6}, // Declared line 654. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ clownpieceA236DROP_COUNT_patchByteCode,     (void *)0x638D31,  6}, // Declared line 656. Patch on FADD ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x638DBC,  6}, // Declared line 658. Patch on FADD ST, QWORD PTR [0x857930]
	PatchSkeleton{ clownpiece19_MULZERO_patchByteCode,         (void *)0x638F61,  6}, // Declared line 660. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpiece19_MULZERO_patchByteCode,         (void *)0x638DAC,  6}, // Declared line 662. Patch on FMUL ST, QWORD PTR [0x867840]
	PatchSkeleton{ clownpiece30_patchByteCode,                 (void *)0x62983A,  6}, // Declared line 668. Patch on FLD ST, DWORD PTR [0x85AAF4]
	PatchSkeleton{ clownpiece31_patchByteCode,                 (void *)0x62982D,  6}, // Declared line 674. Patch on FLD ST, DWORD PTR [0x85B9E4]
	PatchSkeleton{ clownpiece32_patchByteCode,                 (void *)0x63AF04,  6}, // Declared line 680. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpiece33_patchByteCode,                 (void *)0x63AF15,  6}, // Declared line 686. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ clownpiece34_patchByteCode,                 (void *)0x62BD75,  6}, // Declared line 692. Patch on FLD ST, DWORD PTR [0x85B954]
	PatchSkeleton{ clownpiece19_MULZERO_patchByteCode,         (void *)0x63A3FA,  6}, // Declared line 695. Patch on FMUL ST, QWORD PTR [0x857B20]
	PatchSkeleton{ clownpieceD236_SKIP_patchByteCode,          (void *)0x6379E0,  7}, // Declared line 735. Patch on CMP [ESI+0x140], BX
	PatchSkeleton{ clownpieceD623_0_patchByteCode,             (void *)0x63A4FC,  6}, // Declared line 739. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpieceD623_1_patchByteCode,             (void *)0x63A432,  5}, // Declared line 745. Patch on MOV ECX, 0xF
	PatchSkeleton{ clownpieceD623B_LV_patchByteCode,           (void *)0x62BC3F, 12}, // Declared line 752. Patch on FMUL ST, QWORD PTR [0x85BA38]
	PatchSkeleton{ clownpieceD623C_LV_patchByteCode,           (void *)0x62BD9E,  6}, // Declared line 758. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpieceD623_STACK_patchByteCode,         (void *)0x63A390,  5}, // Declared line 787. Patch on MOV EBX, 3
	PatchSkeleton{ clownpieceMECH_patchByteCode,               (void *)0x636A88,  6}, // Declared line 906. Patch on CMP [ESI+0x190], EDI
	PatchSkeleton{ clownpieceMECH_patchByteCode,               (void *)0x637038,  6}, // Declared line 908. Patch on CMP [ESI+0x190], EDI
	PatchSkeleton{ clownpieceMECH_patchByteCode,               (void *)0x6377DA,  6}, // Declared line 910. Patch on CMP [ESI+0x190], EBX
	PatchSkeleton{ clownpieceMECH_patchByteCode,               (void *)0x6380F0,  7}, // Declared line 912. Patch on CMP DWORD PTR [ESI+0x190], 0
	PatchSkeleton{ clownpieceMECH2_patchByteCode,              (void *)0x638FB4,  6}, // Declared line 915. Patch on CMP [ESI+0x190], EBP
	PatchSkeleton{ clownpiece5C_SKIP_patchByteCode,            (void *)0x636B1F,  6}, // Declared line 927. Patch on MOV EAX, [ESI+0x35C]
	PatchSkeleton{ clownpiece5C_SKIP2_patchByteCode,           (void *)0x636B19,  6}, // Declared line 928. Patch on JNE 0x00636CE2
	PatchSkeleton{ clownpieceA214_patchByteCode,               (void *)0x62994F, 18}, // Declared line 963. Patch on FLD ST, DWORD PTR [ESI+0x89C]
	PatchSkeleton{ clownpieceA214_MAXDUR_patchByteCode,        (void *)0x629AC4, 10}, // Declared line 975. Patch on CMP DWORD PTR [ESI+0x144], 0xB4
	PatchSkeleton{ clownpieceSKIP_1_patchByteCode,             (void *)0x639899, 10}, // Declared line 980. Patch on FLD1
	PatchSkeleton{ clownpieceA214_Y_patchByteCode,             (void *)0x63986A,  6}, // Declared line 1002. Patch on FMUL ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpieceA214_patchByteCode,               (void *)0x629F05, 18}, // Declared line 1004. Patch on FLD ST, DWORD PTR [ESI+0x89C]
	PatchSkeleton{ clownpieceA214_MAXDUR_patchByteCode,        (void *)0x62A076, 10}, // Declared line 1006. Patch on CMP DWORD PTR [ESI+0x144], 0xB4
	PatchSkeleton{ clownpieceA214_patchByteCode,               (void *)0x62A49A, 18}, // Declared line 1009. Patch on FLD ST, DWORD PTR [ESI+0x89C]
	PatchSkeleton{ clownpieceA214_MAXDUR_patchByteCode,        (void *)0x62A654, 10}, // Declared line 1011. Patch on CMP DWORD PTR [ESI+0x144], 0xB4
	PatchSkeleton{ clownpieceA214_patchByteCode,               (void *)0x62AAB9, 18}, // Declared line 1014. Patch on FLD ST, DWORD PTR [ESI+0x89C]
	PatchSkeleton{ clownpieceA214_MAXDUR_patchByteCode,        (void *)0x62AC4D, 10}, // Declared line 1016. Patch on CMP DWORD PTR [ESI+0x144], 0xB4
	PatchSkeleton{ clownpieceA214_ROTDISABLE_patchByteCode,    (void *)0x6397B9, 13}, // Declared line 1024. Patch on FLD ST, DWORD PTR [ECX+0x89C]
	PatchSkeleton{ clownpieceA214_OFFSET_patchByteCode,        (void *)0x6397ED, 10}, // Declared line 1030. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ clownpieceA214_OFFSET_patchByteCode,        (void *)0x63982E, 10}, // Declared line 1032. Patch on FILD ST, DWORD PTR [ESP+0x14]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x627021,  6}, // Declared line 1040. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x6273DA,  6}, // Declared line 1042. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece101_patchByteCode,                (void *)0x6255AD,  6}, // Declared line 1049. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x62561C,  6}, // Declared line 1051. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece101_patchByteCode,                (void *)0x625F38,  6}, // Declared line 1053. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x625FA7,  6}, // Declared line 1054. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece101_patchByteCode,                (void *)0x626A95,  6}, // Declared line 1056. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x626B04,  6}, // Declared line 1057. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece101_patchByteCode,                (void *)0x62741A,  6}, // Declared line 1059. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x627486,  6}, // Declared line 1060. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece101_patchByteCode,                (void *)0x627D8F,  6}, // Declared line 1062. Patch on FMUL ST, QWORD PTR [0x8581F8]
	PatchSkeleton{ clownpiece100_patchByteCode,                (void *)0x627DF8,  6}, // Declared line 1063. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece102_patchByteCode,                (void *)0x61E712,  6}, // Declared line 1070. Patch on FLD ST, DWORD PTR [0x85B9BC]
	PatchSkeleton{ clownpiece103_patchByteCode,                (void *)0x61E706,  6}, // Declared line 1076. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ clownpiece104_patchByteCode,                (void *)0x61E889,  6}, // Declared line 1082. Patch on FLD ST, DWORD PTR [0x85BC64]
	PatchSkeleton{ clownpiece103_patchByteCode,                (void *)0x61E87D,  6}, // Declared line 1085. Patch on FLD ST, DWORD PTR [0x85BC7C]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x629273,  6}, // Declared line 1089. Patch on FLD ST, DWORD PTR [0x85C000]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x629429,  6}, // Declared line 1091. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ clownpiece105_patchByteCode,                (void *)0x639056,  6}, // Declared line 1096. Patch on FLD ST, DWORD PTR [0x85BCA4]
	PatchSkeleton{ clownpieceD214_patchByteCode,               (void *)0x6394AC,  7}, // Declared line 1118. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpieceD214_patchByteCode,               (void *)0x6394EF,  7}, // Declared line 1120. Patch on MOVSX ECX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpieceD214_EQ_patchByteCode,            (void *)0x639517,  6}, // Declared line 1125. Patch on FADD ST, QWORD PTR [0x857980]
	PatchSkeleton{ clownpiece3SCSTAR_patchByteCode,            (void *)0x63CA69,  6}, // Declared line 1162. Patch on FLD ST, DWORD PTR [ESI+0xF4]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x63CA89,  6}, // Declared line 1163. Patch on FLD ST, DWORD PTR [ESI+0xF8]
	PatchSkeleton{ clownpiece106_patchByteCode,                (void *)0x62E37B,  6}, // Declared line 1172. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x62E337,  6}, // Declared line 1175. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ clownpiece107_patchByteCode,                (void *)0x62E2DB,  6}, // Declared line 1181. Patch on FMUL ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ clownpiece108_patchByteCode,                (void *)0x62E480,  5}, // Declared line 1187. Patch on PUSH 0x354
	PatchSkeleton{ clownpiece108_patchByteCode,                (void *)0x62E572,  5}, // Declared line 1190. Patch on PUSH 0x354
	PatchSkeleton{ clownpiece109_patchByteCode,                (void *)0x62E30A,  6}, // Declared line 1196. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece106_patchByteCode,                (void *)0x62FC0A,  6}, // Declared line 1200. Patch on FADD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x62FBC6,  6}, // Declared line 1202. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ clownpiece107_patchByteCode,                (void *)0x62FB6A,  6}, // Declared line 1206. Patch on FMUL ST, QWORD PTR [0x858EF0]
	PatchSkeleton{ clownpiece108_patchByteCode,                (void *)0x62FD0F,  5}, // Declared line 1208. Patch on PUSH 0x354
	PatchSkeleton{ clownpiece108_patchByteCode,                (void *)0x62FE01,  5}, // Declared line 1210. Patch on PUSH 0x354
	PatchSkeleton{ clownpiece109_patchByteCode,                (void *)0x62FB99,  6}, // Declared line 1212. Patch on FLD ST, DWORD PTR [0x85B870]
	PatchSkeleton{ clownpiece200_patchByteCode,                (void *)0x636CA9,  6}, // Declared line 1224. Patch on JE 0x00636EE4
	PatchSkeleton{ clownpiece201_patchByteCode,                (void *)0x63282E,  6}, // Declared line 1234. Patch on FSTP DWORD PTR [ESI+0xF8], ST
	PatchSkeleton{ clownpiece1SCCRUSH_patchByteCode,           (void *)0x62EBCB,  8}, // Declared line 1281. Patch on CMP WORD PTR [ESI+0x142], 0
	PatchSkeleton{ clownpiece1SCCRUSH_FLASH_patchByteCode,     (void *)0x62EBD9,  8}, // Declared line 1286. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ clownpiece1SCCRUSH_START_patchByteCode,     (void *)0x62EC6B,  8}, // Declared line 1291. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ clownpiece1SCCRUSH_Y_patchByteCode,         (void *)0x62ECB2,  6}, // Declared line 1296. Patch on FADD ST, QWORD PTR [0x85BC50]
	PatchSkeleton{ clownpiece1SCCRUSH_2_patchByteCode,         (void *)0x63D54B, 18}, // Declared line 1325. Patch on FILD ST, DWORD PTR [ESP+0x20]
	PatchSkeleton{ clownpiece1SCCRUSH_DELAY_patchByteCode,     (void *)0x63D536,  7}, // Declared line 1330. Patch on JNE SHORT 0x0063D55D
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x63F075,  6}, // Declared line 1337. Patch on JLE 0x006396AF
	PatchSkeleton{ clownpiece2SCFIELD_DUR_patchByteCode,       (void *)0x62F758,  9}, // Declared line 1342. Patch on MOV WORD PTR [ESI+0x8B2], 0x1A4
	PatchSkeleton{ clownpiece2SCFIELD_1_patchByteCode,         (void *)0x63F0CF,  5}, // Declared line 1349. Patch on LEA ECX, [EAX+EAX*2]
	PatchSkeleton{ clownpiece2SCFIELD_2_patchByteCode,         (void *)0x63F0A9, 12}, // Declared line 1385. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece2SCD623_DUR_patchByteCode,        (void *)0x63A574,  5}, // Declared line 1392. Patch on CMP EAX, 0xB4
	PatchSkeleton{ clownpieceD22B_patchByteCode,               (void *)0x63B585, 10}, // Declared line 1422. Patch on FLDZ
	PatchSkeleton{ clownpieceD22_1_patchByteCode,              (void *)0x63B6B4,  7}, // Declared line 1429. Patch on PUSH 0x320
	PatchSkeleton{ clownpieceD22_2_patchByteCode,              (void *)0x63B5A9,  6}, // Declared line 1484. Patch on FSUB ST, QWORD PTR [0x857F50]
	PatchSkeleton{ clownpieceD22_3_patchByteCode,              (void *)0x63B62F, 10}, // Declared line 1497. Patch on FILD ST, DWORD PTR [ESP+0x2C]
	PatchSkeleton{ clownpieceD22C_patchByteCode,               (void *)0x63B771, 14}, // Declared line 1522. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ clownpieceD22C_1_patchByteCode,             (void *)0x63B8A4, 12}, // Declared line 1530. Patch on PUSH 0x320
	PatchSkeleton{ clownpieceD22_3_patchByteCode,              (void *)0x63B81F, 10}, // Declared line 1535. Patch on FILD ST, DWORD PTR [ESP+0x2C]
	PatchSkeleton{ clownpieceD22_NOJUMP_patchByteCode,         (void *)0x62C4EB,  6}, // Declared line 1541. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x62C633,  7}, // Declared line 1543. Patch on MOV BYTE PTR [ESI+0x113], 0
	PatchSkeleton{ clownpieceD22_SPIRIT_patchByteCode,         (void *)0x62C538,  6}, // Declared line 1555. Patch on FLD ST, DWORD PTR [0x85BC58]
	PatchSkeleton{ clownpiece19_MULZERO_patchByteCode,         (void *)0x63A6CA,  6}, // Declared line 1567. Patch on FMUL ST, QWORD PTR [0x857B20]
	PatchSkeleton{ clownpieceD623_0_patchByteCode,             (void *)0x63A7CC,  6}, // Declared line 1569. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpieceA623CLONE_0_patchByteCode,        (void *)0x62BEDF, 32}, // Declared line 1585. Patch on FMUL ST, QWORD PTR [0x85A280]
	PatchSkeleton{ clownpieceA623CLONE_1_patchByteCode,        (void *)0x62C040, 32}, // Declared line 1587. Patch on FMUL ST, QWORD PTR [0x858EA0]
	PatchSkeleton{ clownpieceA623CLONE_2_patchByteCode,        (void *)0x63A702,  5}, // Declared line 1593. Patch on MOV ECX, 0xF
	PatchSkeleton{ clownpieceA623_STACK_patchByteCode,         (void *)0x63A660,  5}, // Declared line 1621. Patch on MOV EBX, 3
	PatchSkeleton{ clownpiece4SCBEAM_ANGLE_patchByteCode,      (void *)0x62E969,  6}, // Declared line 1628. Patch on FLD ST, DWORD PTR [0x85BC14]
	PatchSkeleton{ clownpiece4SCBEAM_patchByteCode,            (void *)0x63D36D, 18}, // Declared line 1643. Patch on FSTP DWORD PTR [ESI+0xEC], ST
	PatchSkeleton{ clownpiece4SCBEAM_SOUND_patchByteCode,      (void *)0x63D355,  7}, // Declared line 1663. Patch on MOVSX EDX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece5SCLRE_0_patchByteCode,           (void *)0x63D40C,  6}, // Declared line 1683. Patch on FADD ST, QWORD PTR [0x8579D8]
	PatchSkeleton{ clownpiece5SCLRE_1_patchByteCode,           (void *)0x63D430,  8}, // Declared line 1684. Patch on MOV AL, [ESI+0x113]
	PatchSkeleton{ clownpiece5SCLRE_2_patchByteCode,           (void *)0x63D43E,  8}, // Declared line 1685. Patch on SUB AL, 0x1E
	PatchSkeleton{ clownpiece5SCLRE_3_patchByteCode,           (void *)0x63D426,  8}, // Declared line 1686. Patch on CMP WORD PTR [ESI+0x140], 2
	PatchSkeleton{ clownpiece5SCLRE_patchByteCode,             (void *)0x63D3F4,  5}, // Declared line 1740. Patch on MOV EDI, 1
	PatchSkeleton{ clownpieceD22_FIX_patchByteCode,            (void *)0x62C407,  6}, // Declared line 1749. Patch on CMP [ESI+0x760], ECX
	PatchSkeleton{ clownpiece132_patchByteCode,                (void *)0x637B60,  6}, // Declared line 1754. Patch on MOV ECX, [ESI+0x35C]
	PatchSkeleton{ clownpiece132_patchByteCode,                (void *)0x637C20,  6}, // Declared line 1755. Patch on MOV ECX, [ESI+0x35C]
	PatchSkeleton{ clownpiece133_patchByteCode,                (void *)0x640ED7,  6}, // Declared line 1761. Patch on FADD ST, QWORD PTR [0x858EA8]
	PatchSkeleton{ clownpieceA214RING_0_patchByteCode,         (void *)0x639BAF, 18}, // Declared line 1791. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ clownpieceA214RING_0_patchByteCode,         (void *)0x639DDF, 18}, // Declared line 1792. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x639C25,  6}, // Declared line 1794. Patch on FADD ST, QWORD PTR [0x85BAD8]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x639E77,  6}, // Declared line 1796. Patch on FADD ST, QWORD PTR [0x857F90]
	PatchSkeleton{ clownpiece110_patchByteCode,                (void *)0x639F3C,  7}, // Declared line 1802. Patch on CMP BYTE PTR [ESI+0x194], 0
	PatchSkeleton{ clownpiece111_patchByteCode,                (void *)0x62B1CA,  6}, // Declared line 1811. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece112_patchByteCode,                (void *)0x62B297,  6}, // Declared line 1812. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece111_patchByteCode,                (void *)0x62B425,  6}, // Declared line 1814. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece112_patchByteCode,                (void *)0x62B4F0,  6}, // Declared line 1815. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece111_patchByteCode,                (void *)0x62B6EF,  6}, // Declared line 1817. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece112_patchByteCode,                (void *)0x62B7D4,  6}, // Declared line 1818. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece111_patchByteCode,                (void *)0x62B9BC,  6}, // Declared line 1820. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece112_patchByteCode,                (void *)0x62BAA7,  6}, // Declared line 1821. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece113_patchByteCode,                (void *)0x639C8D,  7}, // Declared line 1828. Patch on CMP BYTE PTR [EAX+0x6AD], 4
	PatchSkeleton{ clownpiece114_patchByteCode,                (void *)0x639EDF,  7}, // Declared line 1835. Patch on CMP BYTE PTR [EAX+0x6AD], 4
	PatchSkeleton{ clownpieceA214RING_1A_patchByteCode,        (void *)0x639D03,  8}, // Declared line 1869. Patch on JNE SHORT 0x00639D59
	PatchSkeleton{ clownpieceA214RING_1B_patchByteCode,        (void *)0x639F78,  8}, // Declared line 1870. Patch on JNE SHORT 0x00639FCE
	PatchSkeleton{ clownpieceA214RING_4_patchByteCode,         (void *)0x62B1B2,  6}, // Declared line 1917. Patch on JNE 0x0062B25C
	PatchSkeleton{ clownpieceA214RING_4A_patchByteCode,        (void *)0x62B27F,  6}, // Declared line 1918. Patch on JNE 0x0063120A
	PatchSkeleton{ clownpieceA214RING_5_patchByteCode,         (void *)0x62B6D7,  6}, // Declared line 1922. Patch on JNE 0x0062B779
	PatchSkeleton{ clownpieceA214RING_5A_patchByteCode,        (void *)0x62B7AE,  6}, // Declared line 1923. Patch on JNE 0x0063120A
	PatchSkeleton{ clownpieceA214RING_ANGLE_0_patchByteCode,   (void *)0x62B40F,  6}, // Declared line 1931. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ clownpieceA214RING_ANGLE_0_patchByteCode,   (void *)0x62B4DA,  6}, // Declared line 1932. Patch on FLD ST, DWORD PTR [0x85BB84]
	PatchSkeleton{ clownpieceA214RING_ANGLE_1_patchByteCode,   (void *)0x62B9A6,  6}, // Declared line 1934. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ clownpieceA214RING_ANGLE_1_patchByteCode,   (void *)0x62BA91,  6}, // Declared line 1935. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ clownpieceA214RING_THISSUCKS_patchByteCode, (void *)0x639D3D, 10}, // Declared line 1953. Patch on CMP AL, 0xA
	PatchSkeleton{ clownpieceA214RING_THISSUCKS_patchByteCode, (void *)0x639FB2, 10}, // Declared line 1954. Patch on CMP AL, 0xA
	PatchSkeleton{ clownpieceA22SPLIT_patchByteCode,           (void *)0x62C859,  7}, // Declared line 2076. Patch on CMP BYTE PTR [ESI+0x6AB], 4
	PatchSkeleton{ clownpieceA22SPLIT_1_patchByteCode,         (void *)0x63BE7A,  6}, // Declared line 2101. Patch on FLD ST, DWORD PTR [ESI+0x378]
	PatchSkeleton{ clownpieceA22SPLIT_ONHIT_patchByteCode,     (void *)0x63BD2A,  7}, // Declared line 2106. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ clownpieceA22SPLIT_3_patchByteCode,         (void *)0x63BEAF,  6}, // Declared line 2163. Patch on FLD ST, DWORD PTR [ESI+0x378]
	PatchSkeleton{ clownpieceA22SPLIT_8_patchByteCode,         (void *)0x63BECF, 18}, // Declared line 2172. Patch on MOV AL, [ESI+0x113]
	PatchSkeleton{ clownpieceA22SPLIT_JMP_patchByteCode,       (void *)0x62CAA0,  7}, // Declared line 2179. Patch on CMP BYTE PTR [ESI+0x6AB], 4
	PatchSkeleton{ clownpieceA22SPLIT_JMP_patchByteCode,       (void *)0x62CD7A,  7}, // Declared line 2185. Patch on CMP BYTE PTR [ESI+0x6AB], 4
	PatchSkeleton{ clownpieceA22SPLIT_JMP_patchByteCode,       (void *)0x62D01A,  7}, // Declared line 2188. Patch on CMP BYTE PTR [ESI+0x6AB], 4
	PatchSkeleton{ clownpieceA22SPLIT_UNFLDZ_patchByteCode,    (void *)0x62C84C,  7}, // Declared line 2194. Patch on CALL 0x00487870
	PatchSkeleton{ clownpieceA22SPLIT_UNFLDZ_patchByteCode,    (void *)0x62CD6D,  7}, // Declared line 2195. Patch on CALL 0x00487870
	PatchSkeleton{ clownpieceA623USB_DUR_patchByteCode,        (void *)0x62C29A,  7}, // Declared line 2204. Patch on CMP DWORD PTR [ESI+0x144], 0x12
	PatchSkeleton{ clownpieceA623USB_DUR_patchByteCode,        (void *)0x62C3E9,  7}, // Declared line 2206. Patch on CMP DWORD PTR [ESI+0x144], 0x19
	PatchSkeleton{ clownpieceA623USB_patchByteCode,            (void *)0x63A981,  6}, // Declared line 2213. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x63AAC7, 24}, // Declared line 2215. Patch on FLD ST, DWORD PTR [EAX+0xEC]
	PatchSkeleton{ clownpieceA623USB_CX_patchByteCode,         (void *)0x62C3C9,  6}, // Declared line 2231. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpieceA623USB_CY_patchByteCode,         (void *)0x62C3A3,  6}, // Declared line 2233. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ clownpiece140_patchByteCode,                (void *)0x63A885,  5}, // Declared line 2239. Patch on PUSH 0x78
	PatchSkeleton{ clownpieceA623USB_1HIT_patchByteCode,       (void *)0x63A9B4,  6}, // Declared line 2247. Patch on MOV [ESI+0x190], EBP
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x63EB9F,  9}, // Declared line 2251. Patch on MOV WORD PTR [EDX+0x8B0], 0x168
	PatchSkeleton{ clownpiece3SCUSB_SKIP_patchByteCode,        (void *)0x63EBCA,  5}, // Declared line 2259. Patch on PUSH 0x78
	PatchSkeleton{ clownpieceA623USB_CX_patchByteCode,         (void *)0x62F70A,  6}, // Declared line 2261. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpieceA623USB_CY_patchByteCode,         (void *)0x62F6E3,  6}, // Declared line 2262. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ clownpiece3SCUSB_ANGLES_patchByteCode,      (void *)0x63EC57,  6}, // Declared line 2269. Patch on FLD ST, DWORD PTR [ESI+0xEC]
	PatchSkeleton{ clownpiece3SCUSB_IDK_patchByteCode,         (void *)0x63EB6C,  8}, // Declared line 2277. Patch on JGE SHORT 0x0063EB74
	PatchSkeleton{ clownpiece3SCUSB_UNCANCEL_patchByteCode,    (void *)0x63EBB7,  6}, // Declared line 2283. Patch on JNE 0x006365F7
	PatchSkeleton{ clownpiece3SCUSB_HITS_patchByteCode,        (void *)0x63EB83,  6}, // Declared line 2295. Patch on MOV EAX, [ESI+0x190]
	PatchSkeleton{ clownpiece2_FLDZ_patchByteCode,             (void *)0x62E691,  6}, // Declared line 2301. Patch on FLD ST, DWORD PTR [0x85B830]
	PatchSkeleton{ clownpiece2_FLD1_patchByteCode,             (void *)0x63CE87,  6}, // Declared line 2303. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece2_FLD1_patchByteCode,             (void *)0x63CE99,  6}, // Declared line 2304. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece2_FLD1_patchByteCode,             (void *)0x63CEAB,  6}, // Declared line 2305. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece2_FLD1_patchByteCode,             (void *)0x63CEBD,  6}, // Declared line 2306. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece115_patchByteCode,                (void *)0x63CE8D,  6}, // Declared line 2311. Patch on FMUL ST, QWORD PTR [0x85DE00]
	PatchSkeleton{ clownpiece116_patchByteCode,                (void *)0x63CEB1,  6}, // Declared line 2316. Patch on FMUL ST, QWORD PTR [0x85BBD8]
	PatchSkeleton{ clownpiece19_NOP_patchByteCode,             (void *)0x63CF31,  6}, // Declared line 2319. Patch on JLE 0x00639888
	PatchSkeleton{ clownpiece117_patchByteCode,                (void *)0x62E669,  5}, // Declared line 2325. Patch on MOV EBP, 0xA
	PatchSkeleton{ clownpiece118_patchByteCode,                (void *)0x62E67B,  7}, // Declared line 2331. Patch on CMP [ESI+0x7D0], BP
	PatchSkeleton{ clownpiece119_patchByteCode,                (void *)0x62E6A5,  8}, // Declared line 2338. Patch on CMP WORD PTR [ESI+0x7D0], 9
	PatchSkeleton{ clownpiece1SCRING_FLD_patchByteCode,        (void *)0x62E1E1,  6}, // Declared line 2347. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece1SCRING_FLD_patchByteCode,        (void *)0x62F9FF,  6}, // Declared line 2349. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ clownpiece1SCRING_TRANSP0_patchByteCode,    (void *)0x63C827,  8}, // Declared line 2374. Patch on JNE SHORT 0x0063C87F
	PatchSkeleton{ clownpiece1SCRING_N_patchByteCode,          (void *)0x63C7A5,  6}, // Declared line 2414. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ clownpiece1SCRING_ANGLE_patchByteCode,      (void *)0x62E1C9,  6}, // Declared line 2439. Patch on JNE 0x0063120A
	PatchSkeleton{ clownpiece1SCRING_ANGLEAIR_patchByteCode,   (void *)0x62F9E7,  6}, // Declared line 2441. Patch on JNE 0x0063120A
	PatchSkeleton{ clownpiece1SCRING_TRANSP_patchByteCode,     (void *)0x63C861, 12}, // Declared line 2450. Patch on CMP AL, 0xA
	PatchSkeleton{ clownpiece134_patchByteCode,                (void *)0x637CDF,  6}, // Declared line 2464. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ clownpiece135_patchByteCode,                (void *)0x637D4F,  6}, // Declared line 2469. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ clownpiece136_patchByteCode,                (void *)0x637DA1,  6}, // Declared line 2474. Patch on FLD ST, DWORD PTR [ESI+0x12C]
	PatchSkeleton{ clownpiece3SCDISC_patchByteCode,            (void *)0x62EEFA, 11}, // Declared line 2510. Patch on FLDZ
	PatchSkeleton{ clownpiece3SCDISC_RET_patchByteCode,        (void *)0x62EF72,  5}, // Declared line 2524. Patch on CALL 0x0046EB30
	PatchSkeleton{ clownpiece3SCDISC_3_patchByteCode,          (void *)0x62EF79,  7}, // Declared line 2525. Patch on FSTP DWORD PTR [ESP+0x134], ST
	PatchSkeleton{ clownpiece138_patchByteCode,                (void *)0x62EF58, 10}, // Declared line 2532. Patch on FILD ST, DWORD PTR [ESP+0x28]
	PatchSkeleton{ clownpiece3SCDISC_0_patchByteCode,          (void *)0x62EF2B, 19}, // Declared line 2548. Patch on MOVZX EAX, BYTE PTR [ESI+0x104]
	PatchSkeleton{ clownpiece3SCDISC_1_patchByteCode,          (void *)0x62EEEC,  8}, // Declared line 2559. Patch on CMP WORD PTR [ESI+0x140], 4
	PatchSkeleton{ clownpiece137_patchByteCode,                (void *)0x62EF0C,  6}, // Declared line 2565. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ clownpiece137_patchByteCode,                (void *)0x63DE5C,  6}, // Declared line 2566. Patch on FLD ST, DWORD PTR [0x8596B8]
	PatchSkeleton{ clownpiece3SCDISC_SKIP_patchByteCode,       (void *)0x63DF0A, 11}, // Declared line 2574. Patch on FLDZ
	PatchSkeleton{ clownpieceA214WALK_5_patchByteCode,         (void *)0x629967,  6}, // Declared line 2588. Patch on FSUB ST, QWORD PTR [0x857F78]
	PatchSkeleton{ clownpieceA214WALK_5_patchByteCode,         (void *)0x629F1D,  6}, // Declared line 2589. Patch on FSUB ST, QWORD PTR [0x857F78]
	PatchSkeleton{ clownpieceA214WALK_6_patchByteCode,         (void *)0x629AB7,  7}, // Declared line 2600. Patch on CMP BYTE PTR [ESI+0x7F4], 0
	PatchSkeleton{ clownpieceA214WALK_6_patchByteCode,         (void *)0x62A06D,  7}, // Declared line 2601. Patch on CMP BYTE PTR [ESI+0x7F4], 0
	PatchSkeleton{ clownpieceA214WALK_6_patchByteCode,         (void *)0x62A647,  7}, // Declared line 2602. Patch on CMP BYTE PTR [ESI+0x7F4], 0
	PatchSkeleton{ clownpieceA214WALK_6_patchByteCode,         (void *)0x62AC40,  7}, // Declared line 2603. Patch on CMP BYTE PTR [ESI+0x7F4], 0
	PatchSkeleton{ clownpieceA22ACURA_4_patchByteCode,         (void *)0x63BEF8,  7}, // Declared line 2737. Patch on CMP DWORD PTR [ESI+0x190], 5
	PatchSkeleton{ clownpieceA22ACURA_3_patchByteCode,         (void *)0x62D40A,  6}, // Declared line 2741. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ clownpieceA22ACURA_0_patchByteCode,         (void *)0x62D36A,  5}, // Declared line 2742. Patch on MOV EBX, 1
	PatchSkeleton{ clownpieceA22ACURA_CREATION_patchByteCode,  (void *)0x62D17E,  6}, // Declared line 2743. Patch on FMUL ST, DWORD PTR [ESI+0x7E0]
	PatchSkeleton{ clownpieceA22ACURA_2_patchByteCode,         (void *)0x62D7E7,  6}, // Declared line 2746. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ clownpieceA22ACURA_0_patchByteCode,         (void *)0x62D747,  5}, // Declared line 2747. Patch on MOV EBX, 1
	PatchSkeleton{ clownpieceA22ACURA_CREATION_patchByteCode,  (void *)0x62D55B,  6}, // Declared line 2748. Patch on FMUL ST, DWORD PTR [ESI+0x7E0]
	PatchSkeleton{ clownpieceA22ACURA_1_patchByteCode,         (void *)0x62DBBC,  6}, // Declared line 2751. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ clownpieceA22ACURA_0_patchByteCode,         (void *)0x62DB15,  5}, // Declared line 2752. Patch on MOV EBX, 1
	PatchSkeleton{ clownpieceA22ACURA_CREATION_patchByteCode,  (void *)0x62D93C,  6}, // Declared line 2753. Patch on FMUL ST, DWORD PTR [ESI+0x7E0]
	PatchSkeleton{ clownpieceA22ACURA_1_patchByteCode,         (void *)0x62DFBF,  6}, // Declared line 2756. Patch on MOV [ESI+0x190], EBX
	PatchSkeleton{ clownpieceA22ACURA_0_patchByteCode,         (void *)0x62DF18,  5}, // Declared line 2757. Patch on MOV EBX, 1
	PatchSkeleton{ clownpieceA22ACURA_CREATION_patchByteCode,  (void *)0x62DD0D,  6}, // Declared line 2758. Patch on FMUL ST, DWORD PTR [ESI+0x7E0]
	PatchSkeleton{ clownpiece130_patchByteCode,                (void *)0x635683,  7}, // Declared line 2767. Patch on CMP BYTE PTR [ESI+0x6AF], 3
	PatchSkeleton{ clownpiece130_patchByteCode,                (void *)0x635592,  7}, // Declared line 2768. Patch on CMP BYTE PTR [ESI+0x6AF], 3
	PatchSkeleton{ clownpiece131_patchByteCode,                (void *)0x635197,  7}, // Declared line 2769. Patch on CMP BYTE PTR [ESI+0x6AF], 2
	PatchSkeleton{ clownpiece126_patchByteCode,                (void *)0x6211BF,  6}, // Declared line 2777. Patch on FSUB ST, QWORD PTR [0x867840]
	PatchSkeleton{ clownpiece127_patchByteCode,                (void *)0x632821,  6}, // Declared line 2783. Patch on FLD ST, DWORD PTR [0x85C590]
	PatchSkeleton{ clownpiece128_patchByteCode,                (void *)0x6327A6,  6}, // Declared line 2789. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ clownpiece129_patchByteCode,                (void *)0x6219CF, 13}, // Declared line 2796. Patch on CMP AX, 0xA
	PatchSkeleton{ clownpiece120_patchByteCode,                (void *)0x61EAFC,  6}, // Declared line 2804. Patch on FLD ST, DWORD PTR [0x85BC44]
	PatchSkeleton{ clownpiece121_patchByteCode,                (void *)0x61EEF5,  6}, // Declared line 2809. Patch on FLD ST, DWORD PTR [0x85BC44]
	PatchSkeleton{ clownpiece122_patchByteCode,                (void *)0x61F43C,  6}, // Declared line 2814. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ clownpiece123_patchByteCode,                (void *)0x6325F6,  6}, // Declared line 2820. Patch on FLD ST, DWORD PTR [0x85BC44]
	PatchSkeleton{ clownpiece124_patchByteCode,                (void *)0x621713,  6}, // Declared line 2826. Patch on FSUB ST, QWORD PTR [0x8677A0]
	PatchSkeleton{ clownpiece125_patchByteCode,                (void *)0x620429,  6}, // Declared line 2834. Patch on FLD ST, DWORD PTR [0x85AC54]
	PatchSkeleton{ clownpiece125_patchByteCode,                (void *)0x61F109,  6}, // Declared line 2836. Patch on FLD ST, DWORD PTR [0x85C8AC]
	PatchSkeleton{ clownpiece66B_patchByteCode,                (void *)0x624E05,  8}, // Declared line 2843. Patch on CMP WORD PTR [ESI+0x13E], 2
	PatchSkeleton{ clownpiece66B_LOOP_patchByteCode,           (void *)0x624EFB,  6}, // Declared line 2848. Patch on CMP [ESI+0x144], EDI
	PatchSkeleton{ clownpiece66B_SOUND_patchByteCode,          (void *)0x624F03,  9}, // Declared line 2858. Patch on PUSH 0x1D
	PatchSkeleton{ clownpiece66B_X_patchByteCode,              (void *)0x624E80,  6}, // Declared line 2862. Patch on FLD ST, DWORD PTR [0x858830]
	PatchSkeleton{ clownpieceJ8A_X_patchByteCode,              (void *)0x621B74,  6}, // Declared line 2867. Patch on FLD ST, DWORD PTR [0x867860]
};


static std::array<unsigned, 222> skeletonsForUpdate = {
	   0,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
	  17,  18,  19,  21,  22,  23,  28,  29,  30,  31,  32,  33,  34,  35,  38,  39,
	  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,
	  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,
	  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
	  88,  89,  90,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 112, 113, 116,
	 121, 122, 131, 132, 135, 136, 137, 138, 139, 140, 144, 145, 146, 147, 148, 149,
	 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 168, 169, 170, 171,
	 172, 173, 174, 175, 176, 177, 178, 179, 182, 183, 184, 185, 189, 200, 201, 202,
	 205, 206, 209, 217, 226, 227, 228, 229, 230, 231, 232, 233, 238, 239, 240, 241,
	 242, 243, 244, 245, 248, 253, 254, 255, 256, 257, 258, 259, 262, 263, 268, 269,
	 274, 282, 283, 284, 285, 286, 289, 290, 295, 296, 297, 298, 299, 300, 301, 304,
	 305, 306, 307, 308, 309, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321,
	 322, 326, 329, 330, 331, 332, 334, 335, 336, 337, 338, 339, 340, 341,
};
static std::array<unsigned, 5> skeletonsForInitializeAction = { 20, 181, 327, 328, 333 };
static std::vector<unsigned> skeletonsForInitialize;
static std::array<unsigned, 4> skeletonsForHandleInputs = { 1, 323, 324, 325 };
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 111> skeletonsForObjectUpdate = {
	  24,  25,  26,  27,  36,  37,  91, 102, 103, 104, 105, 106, 107, 108, 109, 110,
	 111, 114, 115, 117, 118, 119, 120, 123, 124, 125, 126, 127, 128, 129, 130, 133,
	 134, 141, 142, 143, 162, 163, 164, 165, 166, 167, 180, 186, 187, 188, 190, 191,
	 192, 193, 194, 195, 196, 197, 198, 199, 203, 204, 207, 208, 210, 211, 212, 213,
	 214, 215, 216, 218, 219, 220, 221, 222, 223, 224, 225, 234, 235, 236, 237, 246,
	 247, 249, 250, 251, 252, 260, 261, 264, 265, 266, 267, 270, 271, 272, 273, 275,
	 276, 277, 278, 279, 280, 281, 287, 288, 291, 292, 293, 294, 302, 303, 310,
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
	printf("Internal memory used %zu/17826\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 17826);

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
