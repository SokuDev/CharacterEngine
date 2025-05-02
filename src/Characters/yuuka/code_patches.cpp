#include <windows.h>
#include <SokuLib.hpp>
#include "code_patches.hpp"

StackedMemory<11344> applyMemory;
StackedMemory<8822> internalMemory;
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


// yuukaID_FIX declared in yuuka.asm line 8
static unsigned char yuukaID_FIX_byteCode[] = {
	0xC7, 0x86, 0x4C, 0x03, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x34C],0x0000001A ; line 9
	0x66, 0x81, 0xBE, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [ESI+0x196],0x0000 ; line 11
};
static PatchByteCode yuukaID_FIX_patchByteCode = { yuukaID_FIX_byteCode, 19, nullptr, 0 };
// yuukaAIRBACKDASHTURN declared in yuuka.asm line 15
static unsigned char yuukaAIRBACKDASHTURN_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x10, 0x00, // CMP WORD PTR [ESI+0x140],0x10 ; line 16
};
static PatchByteCode yuukaAIRBACKDASHTURN_patchByteCode = { yuukaAIRBACKDASHTURN_byteCode, 9, nullptr, 0 };
// yuukaWALK declared in yuuka.asm line 20
static unsigned char yuukaWALK_byteCode[] = {
	0xD9, 0x05, 0x60, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x867860] ; line 21
};
static PatchByteCode yuukaWALK_patchByteCode = { yuukaWALK_byteCode, 6, nullptr, 0 };
// yuukaWALKBACK declared in yuuka.asm line 25
static unsigned char yuukaWALKBACK_byteCode[] = {
	0xD9, 0xEE,                         // FLDZ ; line 26
	0xD8, 0x25, 0x60, 0x78, 0x86, 0x00, // FSUB DWORD PTR [0x867860] ; line 27
};
static PatchByteCode yuukaWALKBACK_patchByteCode = { yuukaWALKBACK_byteCode, 8, nullptr, 0 };
// yuuka3SCPALM_HITS declared in yuuka.asm line 31
static unsigned char yuuka3SCPALM_HITS_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x01, // MOV BYTE PTR [ESI+0x194],0x01 ; line 32
};
static PatchByteCode yuuka3SCPALM_HITS_patchByteCode = { yuuka3SCPALM_HITS_byteCode, 7, nullptr, 0 };
// yuukaJ8A_Y declared in yuuka.asm line 37
static unsigned char yuukaJ8A_Y_byteCode[] = {
	0xD9, 0x05, 0xD4, 0xB9, 0x85, 0x00, // FLD DWORD PTR [0x85B9D4] ; line 42
};
static PatchByteCode yuukaJ8A_Y_patchByteCode = { yuukaJ8A_Y_byteCode, 6, nullptr, 0 };
// yuukaBDASH_GRAV declared in yuuka.asm line 46
static unsigned char yuukaBDASH_GRAV_byteCode[] = {
	0xD9, 0x05, 0x10, 0xC1, 0x85, 0x00, // FLD DWORD PTR [0x85C110] ; line 47
};
static PatchByteCode yuukaBDASH_GRAV_patchByteCode = { yuukaBDASH_GRAV_byteCode, 6, nullptr, 0 };
// yuukaNOP declared in yuuka.asm line 51
static unsigned char yuukaNOP_byteCode[] = {
	0x90, // NOP ; line 52
};
static PatchByteCode yuukaNOP_patchByteCode = { yuukaNOP_byteCode, 1, nullptr, 0 };
// yuukaDASH_SPD declared in yuuka.asm line 73
static unsigned char yuukaDASH_SPD_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 74
};
static PatchByteCode yuukaDASH_SPD_patchByteCode = { yuukaDASH_SPD_byteCode, 6, nullptr, 0 };
// yuukaADASH_SPD declared in yuuka.asm line 79
static unsigned char yuukaADASH_SPD_byteCode[] = {
	0xD9, 0x05, 0x44, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BC44] ; line 80
};
static PatchByteCode yuukaADASH_SPD_patchByteCode = { yuukaADASH_SPD_byteCode, 6, nullptr, 0 };
// yuukaABDASH_SPD declared in yuuka.asm line 85
static unsigned char yuukaABDASH_SPD_byteCode[] = {
	0xD9, 0x05, 0xAC, 0xC8, 0x85, 0x00, // FLD DWORD PTR [0x85C8AC] ; line 86
};
static PatchByteCode yuukaABDASH_SPD_patchByteCode = { yuukaABDASH_SPD_byteCode, 6, nullptr, 0 };
// yuukaDASHCOMMIT declared in yuuka.asm line 91
static unsigned char yuukaDASHCOMMIT_byteCode[] = {
	0x66, 0x81, 0xF8, 0x03, 0x00,       // CMP AX,0x0003 ; line 92
	0x0F, 0x8F, 0xDE, 0xAD, 0xBE, 0xEF, // JG 0x7A050B ; line 93
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,       // JMP 0x7A0505 ; line 94
};
static JumpTarget yuukaDASHCOMMIT_jumpData[] = {
	{ 0x007, 0x7A050B },
	{ 0x00C, 0x7A0505 },
};
static PatchByteCode yuukaDASHCOMMIT_patchByteCode = { yuukaDASHCOMMIT_byteCode, 16, yuukaDASHCOMMIT_jumpData, 2 };
// yuukaHJ7X declared in yuuka.asm line 99
static unsigned char yuukaHJ7X_byteCode[] = {
	0xD9, 0x05, 0xAC, 0xC8, 0x85, 0x00, // FLD DWORD PTR [0x85C8AC] ; line 100
};
static PatchByteCode yuukaHJ7X_patchByteCode = { yuukaHJ7X_byteCode, 6, nullptr, 0 };
// yuukaHJ9X declared in yuuka.asm line 104
static unsigned char yuukaHJ9X_byteCode[] = {
	0xD9, 0x05, 0x44, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BC44] ; line 105
};
static PatchByteCode yuukaHJ9X_patchByteCode = { yuukaHJ9X_byteCode, 6, nullptr, 0 };
// yuukaHJ8Y declared in yuuka.asm line 113
static unsigned char yuukaHJ8Y_byteCode[] = {
	0xD9, 0x05, 0x60, 0xBC, 0x85, 0x00, // FLD DWORD PTR [0x85BC60] ; line 114
};
static PatchByteCode yuukaHJ8Y_patchByteCode = { yuukaHJ8Y_byteCode, 6, nullptr, 0 };
// yuukaHJ9Y declared in yuuka.asm line 120
static unsigned char yuukaHJ9Y_byteCode[] = {
	0xD9, 0x05, 0x30, 0xC2, 0x85, 0x00, // FLD DWORD PTR [0x85C230] ; line 121
};
static PatchByteCode yuukaHJ9Y_patchByteCode = { yuukaHJ9Y_byteCode, 6, nullptr, 0 };
// yuuka1DDX declared in yuuka.asm line 128
static unsigned char yuuka1DDX_byteCode[] = {
	0xD9, 0x05, 0xAC, 0xC8, 0x85, 0x00, // FLD DWORD PTR [0x85C8AC] ; line 129
};
static PatchByteCode yuuka1DDX_patchByteCode = { yuuka1DDX_byteCode, 6, nullptr, 0 };
// yuuka2DDGRAV declared in yuuka.asm line 133
static unsigned char yuuka2DDGRAV_byteCode[] = {
	0xD9, 0x05, 0x10, 0xC1, 0x85, 0x00, // FLD DWORD PTR [0x85C110] ; line 134
};
static PatchByteCode yuuka2DDGRAV_patchByteCode = { yuuka2DDGRAV_byteCode, 6, nullptr, 0 };
// yuuka1DDGRAV declared in yuuka.asm line 136
static unsigned char yuuka1DDGRAV_byteCode[] = {
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00, // FLD DWORD PTR [0x8677A8] ; line 137
};
static PatchByteCode yuuka1DDGRAV_patchByteCode = { yuuka1DDGRAV_byteCode, 6, nullptr, 0 };
// yuukaCF5A_REMOVE declared in yuuka.asm line 150
static unsigned char yuukaCF5A_REMOVE_byteCode[] = {
	0x80, 0xBE, 0xF4, 0x07, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x7F4],0x01 ; line 151
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x7A2F1C ; line 152
};
static JumpTarget yuukaCF5A_REMOVE_jumpData[] = {
	{ 0x008, 0x7A2F1C },
};
static PatchByteCode yuukaCF5A_REMOVE_patchByteCode = { yuukaCF5A_REMOVE_byteCode, 12, yuukaCF5A_REMOVE_jumpData, 1 };
// yuuka3AX declared in yuuka.asm line 157
static unsigned char yuuka3AX_byteCode[] = {
	0xD9, 0x05, 0x00, 0x9F, 0x85, 0x00, // FLD DWORD PTR [0x859F00] ; line 158
};
static PatchByteCode yuuka3AX_patchByteCode = { yuuka3AX_byteCode, 6, nullptr, 0 };
// yuukaC3AX declared in yuuka.asm line 162
static unsigned char yuukaC3AX_byteCode[] = {
	0xD9, 0x05, 0x94, 0xAE, 0x85, 0x00, // FLD DWORD PTR [0x85AE94] ; line 163
};
static PatchByteCode yuukaC3AX_patchByteCode = { yuukaC3AX_byteCode, 6, nullptr, 0 };
// yuuka3ADEC declared in yuuka.asm line 167
static unsigned char yuuka3ADEC_byteCode[] = {
	0xDC, 0x25, 0xC8, 0x77, 0x86, 0x00, // FSUB QWORD PTR [0x8677C8] ; line 168
};
static PatchByteCode yuuka3ADEC_patchByteCode = { yuuka3ADEC_byteCode, 6, nullptr, 0 };
// yuuka3AKEY declared in yuuka.asm line 172
static unsigned char yuuka3AKEY_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x02, 0x00, // CMP WORD PTR [ESI+0x140],0x02 ; line 173
};
static PatchByteCode yuuka3AKEY_patchByteCode = { yuuka3AKEY_byteCode, 9, nullptr, 0 };
// yuuka66CX declared in yuuka.asm line 180
static unsigned char yuuka66CX_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 181
};
static PatchByteCode yuuka66CX_patchByteCode = { yuuka66CX_byteCode, 6, nullptr, 0 };
// yuuka66CY declared in yuuka.asm line 185
static unsigned char yuuka66CY_byteCode[] = {
	0xD9, 0x05, 0x30, 0x88, 0x85, 0x00, // FLD DWORD PTR [0x858830] ; line 186
};
static PatchByteCode yuuka66CY_patchByteCode = { yuuka66CY_byteCode, 6, nullptr, 0 };
// yuukaFLDZ declared in yuuka.asm line 191
static unsigned char yuukaFLDZ_byteCode[] = {
	0xD9, 0xEE, // FLDZ ; line 192
};
static PatchByteCode yuukaFLDZ_patchByteCode = { yuukaFLDZ_byteCode, 2, nullptr, 0 };
// yuuka5CSPD_0 declared in yuuka.asm line 199
static unsigned char yuuka5CSPD_0_byteCode[] = {
	0xDC, 0x1D, 0x2C, 0x81, 0x86, 0x00, // FCOMP QWORD PTR [0x86812C] ; line 200
};
static PatchByteCode yuuka5CSPD_0_patchByteCode = { yuuka5CSPD_0_byteCode, 6, nullptr, 0 };
// yuuka5CSPD_3 declared in yuuka.asm line 204
static unsigned char yuuka5CSPD_3_byteCode[] = {
	0xDD, 0x05, 0x2C, 0x81, 0x86, 0x00, // FLD QWORD PTR [0x86812C] ; line 205
};
static PatchByteCode yuuka5CSPD_3_patchByteCode = { yuuka5CSPD_3_byteCode, 6, nullptr, 0 };
// yuukaC5C_REMOVE declared in yuuka.asm line 240
static unsigned char yuukaC5C_REMOVE_byteCode[] = {
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x25, 0x00, // CMP WORD PTR [ESI+0x140],0x25 ; line 241
};
static PatchByteCode yuukaC5C_REMOVE_patchByteCode = { yuukaC5C_REMOVE_byteCode, 9, nullptr, 0 };
// yuuka6B_XPOS1 declared in yuuka.asm line 252
static unsigned char yuuka6B_XPOS1_byteCode[] = {
	0xDC, 0x0D, 0x88, 0xA2, 0x85, 0x00, // FMUL QWORD PTR [0x85A288] ; line 253
};
static PatchByteCode yuuka6B_XPOS1_patchByteCode = { yuuka6B_XPOS1_byteCode, 6, nullptr, 0 };
// yuuka6B_XPOS2 declared in yuuka.asm line 257
static unsigned char yuuka6B_XPOS2_byteCode[] = {
	0x6B, 0xD2, 0x04,       // IMUL EDX,EDX,0x04 ; line 258
	0xD9, 0x5C, 0x24, 0x20, // FSTP DWORD PTR [ESP+0x20] ; line 259
};
static PatchByteCode yuuka6B_XPOS2_patchByteCode = { yuuka6B_XPOS2_byteCode, 7, nullptr, 0 };
// yuuka6B_HITCOUNT declared in yuuka.asm line 263
static unsigned char yuuka6B_HITCOUNT_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x0A, // MOV BYTE PTR [ESI+0x194],0x0A ; line 264
};
static PatchByteCode yuuka6B_HITCOUNT_patchByteCode = { yuuka6B_HITCOUNT_byteCode, 7, nullptr, 0 };
// yuuka6B_HEIGHT declared in yuuka.asm line 269
static unsigned char yuuka6B_HEIGHT_byteCode[] = {
	0xD9, 0x80, 0xF0, 0x00, 0x00, 0x00, // FLD DWORD PTR [EAX+0xF0] ; line 270
	0xD8, 0x25, 0x40, 0x15, 0x87, 0x00, // FSUB DWORD PTR [0x871540] ; line 271
	0xD8, 0x25, 0x40, 0x15, 0x87, 0x00, // FSUB DWORD PTR [0x871540] ; line 272
};
static PatchByteCode yuuka6B_HEIGHT_patchByteCode = { yuuka6B_HEIGHT_byteCode, 18, nullptr, 0 };
// yuuka6B_HITRATE1 declared in yuuka.asm line 276
static unsigned char yuuka6B_HITRATE1_byteCode[] = {
	0x0F, 0xB7, 0x86, 0x6C, 0x03, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x36C] ; line 277
	0x66, 0x81, 0xF8, 0x04, 0x00,             // CMP AX,0x0004 ; line 278
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x7BB00B ; line 279
};
static JumpTarget yuuka6B_HITRATE1_jumpData[] = {
	{ 0x00D, 0x7BB00B },
};
static PatchByteCode yuuka6B_HITRATE1_patchByteCode = { yuuka6B_HITRATE1_byteCode, 17, yuuka6B_HITRATE1_jumpData, 1 };
// yuuka6B_HITRATE2 declared in yuuka.asm line 281
static unsigned char yuuka6B_HITRATE2_byteCode[] = {
	0x0F, 0xB7, 0x86, 0x6C, 0x03, 0x00, 0x00, // MOVZX EAX,WORD PTR [ESI+0x36C] ; line 282
	0x66, 0x81, 0xF8, 0x04, 0x00,             // CMP AX,0x0004 ; line 283
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x7BB0E6 ; line 284
};
static JumpTarget yuuka6B_HITRATE2_jumpData[] = {
	{ 0x00D, 0x7BB0E6 },
};
static PatchByteCode yuuka6B_HITRATE2_patchByteCode = { yuuka6B_HITRATE2_byteCode, 17, yuuka6B_HITRATE2_jumpData, 1 };
// yuuka6B_DUR declared in yuuka.asm line 296
static unsigned char yuuka6B_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000118 ; line 297
};
static PatchByteCode yuuka6B_DUR_patchByteCode = { yuuka6B_DUR_byteCode, 10, nullptr, 0 };
// yuukaD236_0 declared in yuuka.asm line 304
static unsigned char yuukaD236_0_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00 ; line 306
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 307
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x0A,                   // MOV BYTE PTR [ESI+0x194],0x0A ; line 308
	0x8B, 0x86, 0x90, 0x01, 0x00, 0x00,                         // MOV EAX,[ESI+0x190] ; line 311
	0x81, 0xF8, 0x00, 0x00, 0x00, 0x00,                         // CMP EAX,0x00 ; line 312
	0x0F, 0x84, 0x6B, 0x00, 0x00, 0x00,                         // JE SHORT LABEL2 ; line 313
	0x81, 0xF8, 0x04, 0x00, 0x00, 0x00,                         // CMP EAX,0x04 ; line 314
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL3 ; line 315
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 316
	0x6A, 0x0F,                                                 // PUSH 0x0F ; line 318
	0x89, 0xF1,                                                 // MOV ECX,ESI ; line 319
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CDE0 ; line 320
	0x81, 0xBE, 0x90, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, // CMP DWORD PTR [ESI+0x190],0x06 ; line 321
	0x0F, 0x85, 0x07, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL4 ; line 322
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,                   // MOV BYTE PTR [ESI+0x194],0x00 ; line 323
	0x80, 0xBE, 0x94, 0x01, 0x00, 0x00, 0x00,                   // CMP BYTE PTR [ESI+0x194],0x00 ; line 325
	0x0F, 0x8E, 0x30, 0x00, 0x00, 0x00,                         // JLE SHORT LABEL5 ; line 326
	0x66, 0x81, 0xBE, 0x6C, 0x03, 0x00, 0x00, 0x05, 0x00,       // CMP WORD PTR [ESI+0x36C],0x0005 ; line 327
	0x0F, 0x85, 0x13, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL6 ; line 328
	0x66, 0xC7, 0x86, 0x6C, 0x03, 0x00, 0x00, 0x00, 0x00,       // MOV WORD PTR [ESI+0x36C],0x0000 ; line 329
	0xC7, 0x86, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV DWORD PTR [ESI+0x190],0x00000000 ; line 330
	0x66, 0x81, 0x86, 0x6C, 0x03, 0x00, 0x00, 0x01, 0x00,       // ADD WORD PTR [ESI+0x36C],0x01 ; line 332
	0xE9, 0x00, 0x00, 0x00, 0x00,                               // JMP LABELEND ; line 334
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x25, 0x00,       // CMP WORD PTR [ESI+0x140],0x25 ; line 340
};
static JumpTarget yuukaD236_0_jumpData[] = {
	{ 0x041, 0x48CDE0 },
};
static PatchByteCode yuukaD236_0_patchByteCode = { yuukaD236_0_byteCode, 162, yuukaD236_0_jumpData, 1 };
// yuukaD236_1 declared in yuuka.asm line 344
static unsigned char yuukaD236_1_byteCode[] = {
	0xD9, 0x05, 0x3C, 0x15, 0x87, 0x00, // FLD DWORD PTR [0x87153C] ; line 345
};
static PatchByteCode yuukaD236_1_patchByteCode = { yuukaD236_1_byteCode, 6, nullptr, 0 };
// yuukaD236_ROT declared in yuuka.asm line 352
static unsigned char yuukaD236_ROT_byteCode[] = {
	0x60,                               // PUSHAD ; line 353
	0x66, 0x9C,                         // PUSHF ; line 354
	0xD8, 0x15, 0xC8, 0x7D, 0x85, 0x00, // FCOM DWORD PTR [0x857DC8] ; line 355
	0xDF, 0xE0,                         // FNSTSW AX ; line 356
	0xF6, 0xC4, 0x41,                   // TEST AH,0x41 ; line 357
	0x0F, 0x87, 0x05, 0x00, 0x00, 0x00, // JA SHORT LABEL1 ; line 358
	0xE9, 0x1C, 0x00, 0x00, 0x00,       // JMP SHORT LABEL2 ; line 359
	0xD8, 0x15, 0x94, 0xAE, 0x85, 0x00, // FCOM DWORD PTR [0x85AE94] ; line 361
	0xDF, 0xE0,                         // FNSTSW AX ; line 362
	0xF6, 0xC4, 0x41,                   // TEST AH,0x41 ; line 363
	0x0F, 0x86, 0x05, 0x00, 0x00, 0x00, // JNA SHORT LABEL3 ; line 364
	0xE9, 0x06, 0x00, 0x00, 0x00,       // JMP SHORT LABEL2 ; line 365
	0xD8, 0x25, 0x30, 0x88, 0x85, 0x00, // FSUB DWORD PTR [0x858830] ; line 367
	0xD8, 0x25, 0xF0, 0x89, 0x85, 0x00, // FSUB DWORD PTR [0x8589F0] ; line 369
	0x66, 0x9D,                         // POPF ; line 370
	0x61,                               // POPAD ; line 371
};
static PatchByteCode yuukaD236_ROT_patchByteCode = { yuukaD236_ROT_byteCode, 62, nullptr, 0 };
// yuukaD236_2 declared in yuuka.asm line 375
static unsigned char yuukaD236_2_byteCode[] = {
	0xB9, 0x02, 0x00, 0x00, 0x00, // MOV ECX,0x00000002 ; line 376
};
static PatchByteCode yuukaD236_2_patchByteCode = { yuukaD236_2_byteCode, 5, nullptr, 0 };
// yuukaD236_LV3 declared in yuuka.asm line 380
static unsigned char yuukaD236_LV3_byteCode[] = {
	0x80, 0xBA, 0xA5, 0x06, 0x00, 0x00, 0x03, // CMP BYTE PTR [EDX+0x6A5],0x03 ; line 381
};
static PatchByteCode yuukaD236_LV3_patchByteCode = { yuukaD236_LV3_byteCode, 7, nullptr, 0 };
// yuukaD22_EFF declared in yuuka.asm line 392
static unsigned char yuukaD22_EFF_byteCode[] = {
	0x68, 0x31, 0x03, 0x00, 0x00, // PUSH 0x00000331 ; line 393
};
static PatchByteCode yuukaD22_EFF_patchByteCode = { yuukaD22_EFF_byteCode, 5, nullptr, 0 };
// yuukaD236_RECAST declared in yuuka.asm line 415
static unsigned char yuukaD236_RECAST_byteCode[] = {
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                               // CALL 0x48CDE0 ; line 416
	0x66, 0x9C,                                                 // PUSHF ; line 417
	0x60,                                                       // PUSHAD ; line 418
	0x8B, 0x86, 0x98, 0x03, 0x00, 0x00,                         // MOV EAX,[ESI+0x398] ; line 419
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x08, 0x02,       // CMP WORD PTR [EAX+0x13C],0x0208 ; line 420
	0x0F, 0x8C, 0x37, 0x00, 0x00, 0x00,                         // JL SHORT LABEL1 ; line 421
	0x66, 0x81, 0xB8, 0x3C, 0x01, 0x00, 0x00, 0x0B, 0x02,       // CMP WORD PTR [EAX+0x13C],0x020B ; line 422
	0x0F, 0x8F, 0x28, 0x00, 0x00, 0x00,                         // JG SHORT LABEL1 ; line 423
	0x66, 0x81, 0xB8, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00,       // CMP WORD PTR [EAX+0x13E],0x00 ; line 424
	0x0F, 0x85, 0x19, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 425
	0x81, 0xB8, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // CMP DWORD PTR [EAX+0x144],0x00 ; line 426
	0x0F, 0x85, 0x09, 0x00, 0x00, 0x00,                         // JNE SHORT LABEL1 ; line 427
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x61, 0x00,       // MOV WORD PTR [ESI+0x140],0x0061 ; line 428
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x29, 0x00,       // CMP WORD PTR [ESI+0x140],0x0029 ; line 431
	0x0F, 0x8E, 0xA6, 0x00, 0x00, 0x00,                         // JLE LABELLVNOJUMP ; line 432
	0x0F, 0xB6, 0x80, 0xA5, 0x06, 0x00, 0x00,                   // MOVZX EAX, BYTE PTR [EAX+0x6A5] ; line 433
	0x81, 0xF8, 0x00, 0x00, 0x00, 0x00,                         // CMP EAX,0x00 ; line 434
	0x0F, 0x84, 0x2F, 0x00, 0x00, 0x00,                         // JE LABELLV0 ; line 435
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                         // CMP EAX,0x01 ; line 436
	0x0F, 0x84, 0x31, 0x00, 0x00, 0x00,                         // JE LABELLV1 ; line 437
	0x81, 0xF8, 0x02, 0x00, 0x00, 0x00,                         // CMP EAX,0x02 ; line 438
	0x0F, 0x84, 0x33, 0x00, 0x00, 0x00,                         // JE LABELLV2 ; line 439
	0x81, 0xF8, 0x03, 0x00, 0x00, 0x00,                         // CMP EAX,0x03 ; line 440
	0x0F, 0x84, 0x35, 0x00, 0x00, 0x00,                         // JE LABELLV3 ; line 441
	0x81, 0xF8, 0x04, 0x00, 0x00, 0x00,                         // CMP EAX,0x04 ; line 442
	0xE9, 0x38, 0x00, 0x00, 0x00,                               // JMP LABELLV4 ; line 443
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x36, 0x00,       // CMP WORD PTR [ESI+0x140],0x0036 ; line 446
	0xE9, 0x2F, 0x00, 0x00, 0x00,                               // JMP LABELLVEND ; line 447
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x40, 0x00,       // CMP WORD PTR [ESI+0x140],0x0040 ; line 449
	0xE9, 0x21, 0x00, 0x00, 0x00,                               // JMP LABELLVEND ; line 450
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x4A, 0x00,       // CMP WORD PTR [ESI+0x140],0x004A ; line 452
	0xE9, 0x13, 0x00, 0x00, 0x00,                               // JMP LABELLVEND ; line 453
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x54, 0x00,       // CMP WORD PTR [ESI+0x140],0x0054 ; line 455
	0xE9, 0x05, 0x00, 0x00, 0x00,                               // JMP LABELLVEND ; line 456
	0xE9, 0x27, 0x00, 0x00, 0x00,                               // JMP LABELLVNOJUMP ; line 458
	0x0F, 0x8C, 0x21, 0x00, 0x00, 0x00,                         // JL LABELLVNOJUMP ; line 461
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x5D, 0x00,       // CMP WORD PTR [ESI+0x140],0x005D ; line 462
	0x0F, 0x8D, 0x12, 0x00, 0x00, 0x00,                         // JGE LABELLVNOJUMP ; line 463
	0x66, 0xC7, 0x86, 0x40, 0x01, 0x00, 0x00, 0x5D, 0x00,       // MOV WORD PTR [ESI+0x140],0x005D ; line 464
	0x66, 0xC7, 0x86, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00,       // MOV WORD PTR [ESI+0x142],0x0001 ; line 465
	0x61,                                                       // POPAD ; line 467
	0x66, 0x9D,                                                 // POPF ; line 468
};
static JumpTarget yuukaD236_RECAST_jumpData[] = {
	{ 0x001, 0x48CDE0 },
};
static PatchByteCode yuukaD236_RECAST_patchByteCode = { yuukaD236_RECAST_byteCode, 268, yuukaD236_RECAST_jumpData, 1 };
// yuukaCHICKENATTACK declared in yuuka.asm line 477
static unsigned char yuukaCHICKENATTACK_byteCode[] = {
	0xE8, 0x05, 0x00, 0x00, 0x00,                         // CALL BEGIN ; line 478
	0xE9, 0xC0, 0x02, 0x00, 0x00,                         // JMP EXIT ; line 479
	0x66, 0x9C,                                           // PUSHF ; line 482
	0x60,                                                 // PUSHAD ; line 483
	0x8B, 0x96, 0x98, 0x03, 0x00, 0x00,                   // MOV EDX,[ESI+0x398] ; line 485
	0x8B, 0x9A, 0x3C, 0x01, 0x00, 0x00,                   // MOV EBX,[EDX+0x13C] ; line 487
	0x81, 0xFB, 0x26, 0x02, 0x01, 0x00,                   // CMP EBX,0x00010226 ; line 488
	0x0F, 0x84, 0x29, 0x00, 0x00, 0x00,                   // JE SHORT LABEL01 ; line 489
	0x81, 0xFB, 0x27, 0x02, 0x01, 0x00,                   // CMP EBX,0x00010227 ; line 490
	0x0F, 0x84, 0x1D, 0x00, 0x00, 0x00,                   // JE SHORT LABEL01 ; line 491
	0x81, 0xFB, 0x28, 0x02, 0x01, 0x00,                   // CMP EBX,0x00010228 ; line 492
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                   // JE SHORT LABEL01 ; line 493
	0x81, 0xFB, 0x29, 0x02, 0x01, 0x00,                   // CMP EBX,0x00010229 ; line 494
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                   // JE SHORT LABEL01 ; line 495
	0xE9, 0x17, 0x00, 0x00, 0x00,                         // JMP SHORT LABEL02 ; line 496
	0xB8, 0x00, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000000 ; line 498
	0x8B, 0x9A, 0x44, 0x01, 0x00, 0x00,                   // MOV EBX,[EDX+0x144] ; line 499
	0x81, 0xFB, 0x00, 0x00, 0x00, 0x00,                   // CMP EBX,0x00000000 ; line 500
	0x0F, 0x84, 0x05, 0x00, 0x00, 0x00,                   // JE SHORT LABEL03 ; line 502
	0xB8, 0x01, 0x00, 0x00, 0x00,                         // MOV EAX,0x00000001 ; line 504
	0x0F, 0xB6, 0x8A, 0xAE, 0x06, 0x00, 0x00,             // MOVZX ECX,BYTE PTR [EDX+0x6AE] ; line 506
	0x8B, 0x9E, 0x3C, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x13C] ; line 508
	0x81, 0xFB, 0x22, 0x03, 0x00, 0x00,                   // CMP EBX,0x00000322 ; line 509
	0x0F, 0x84, 0x53, 0x00, 0x00, 0x00,                   // JE SHORT LABELX1 ; line 510
	0x81, 0xFB, 0x22, 0x03, 0x05, 0x00,                   // CMP EBX,0x00050322 ; line 511
	0x0F, 0x84, 0x95, 0x00, 0x00, 0x00,                   // JE LABELX2 ; line 512
	0x66, 0x81, 0xFB, 0x21, 0x03,                         // CMP BX,0x0321 ; line 513
	0x0F, 0x84, 0x8A, 0x00, 0x00, 0x00,                   // JE LABELX2 ; line 514
	0x66, 0x81, 0xFB, 0x2F, 0x03,                         // CMP BX,0x032F ; line 515
	0x0F, 0x84, 0xCD, 0x00, 0x00, 0x00,                   // JE LABELX3 ; line 516
	0x66, 0x81, 0xFB, 0x30, 0x03,                         // CMP BX,0x0330 ; line 517
	0x0F, 0x84, 0x11, 0x01, 0x00, 0x00,                   // JE LABELX4 ; line 518
	0x66, 0x81, 0xFB, 0x35, 0x03,                         // CMP BX,0x0335 ; line 519
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x7D0DD4 ; line 520
	0x66, 0x81, 0xFB, 0x5F, 0x03,                         // CMP BX,0x035F ; line 521
	0x0F, 0x84, 0xDE, 0xAD, 0xBE, 0xEF,                   // JE 0x7D0DD4 ; line 522
	0x66, 0x81, 0xFB, 0x59, 0x03,                         // CMP BX,0x0359 ; line 523
	0x0F, 0x84, 0xAB, 0x01, 0x00, 0x00,                   // JE LABELX6 ; line 524
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x7D0DD4 ; line 525
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 528
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX1END ; line 529
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 530
	0x6B, 0xC9, 0x18,                                     // IMUL ECX,ECX,0x18 ; line 531
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 532
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 533
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 534
	0x66, 0x81, 0xFB, 0xDC, 0x00,                         // CMP BX,0x00DC ; line 535
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX1A ; line 536
	0x66, 0xBB, 0xDC, 0x00,                               // MOV BX,0x00DC ; line 537
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0xDC, 0x00, // CMP WORD PTR [ESI+0x140],0x00DC ; line 539
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX1END ; line 540
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 541
	0x61,                                                 // POPAD ; line 543
	0x66, 0x9D,                                           // POPF ; line 544
	0x8B, 0x8E, 0x5C, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x35C] ; line 546
	0xC3,                                                 // RET ; line 547
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 550
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX2END ; line 551
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 552
	0x6B, 0xC9, 0x18,                                     // IMUL ECX,ECX,0x18 ; line 553
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 554
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 555
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 556
	0x66, 0x81, 0xFB, 0xDC, 0x00,                         // CMP BX,0x00DC ; line 557
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX2A ; line 558
	0x66, 0xBB, 0xDC, 0x00,                               // MOV BX,0x00DC ; line 559
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0xDC, 0x00, // CMP WORD PTR [ESI+0x140],0x00DC ; line 561
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX2END ; line 562
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 563
	0x61,                                                 // POPAD ; line 565
	0x66, 0x9D,                                           // POPF ; line 566
	0x8B, 0x86, 0x9C, 0x03, 0x00, 0x00,                   // MOV EAX,[ESI+0x39C] ; line 567
	0xC3,                                                 // RET ; line 568
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 571
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX3END ; line 572
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 573
	0x6B, 0xC9, 0x03,                                     // IMUL ECX,ECX,0x03 ; line 574
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 575
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 576
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 577
	0x66, 0x81, 0xFB, 0x24, 0x00,                         // CMP BX,0x0024 ; line 578
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX3A ; line 579
	0x66, 0xBB, 0x24, 0x00,                               // MOV BX,0x0024 ; line 580
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x24, 0x00, // CMP WORD PTR [ESI+0x140],0x0024 ; line 582
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX3END ; line 583
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 584
	0x61,                                                 // POPAD ; line 586
	0x66, 0x9D,                                           // POPF ; line 587
	0x66, 0x39, 0xAE, 0x40, 0x01, 0x00, 0x00,             // CMP [ESI+0x140],BP ; line 588
	0xC3,                                                 // RET ; line 589
	0x81, 0xFB, 0x30, 0x03, 0x00, 0x00,                   // CMP EBX,0x00000330 ; line 592
	0x0F, 0x84, 0x11, 0x00, 0x00, 0x00,                   // JE SHORT LABELX4TRUE ; line 593
	0x81, 0xFB, 0x30, 0x03, 0x03, 0x00,                   // CMP EBX,0x00030330 ; line 594
	0x0F, 0x84, 0x54, 0x00, 0x00, 0x00,                   // JE SHORT LABELX5 ; line 595
	0xE9, 0x44, 0x00, 0x00, 0x00,                         // JMP SHORT LABELX4END ; line 596
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 599
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX4END ; line 600
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 601
	0x6B, 0xC9, 0x03,                                     // IMUL ECX,ECX,0x03 ; line 602
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 603
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 604
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 605
	0x66, 0x81, 0xFB, 0x29, 0x00,                         // CMP BX,0x0029 ; line 606
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX4A ; line 607
	0x66, 0xBB, 0x29, 0x00,                               // MOV BX,0x0029 ; line 608
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x29, 0x00, // CMP WORD PTR [ESI+0x140],0x0029 ; line 610
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX4END ; line 611
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 612
	0x61,                                                 // POPAD ; line 614
	0x66, 0x9D,                                           // POPF ; line 615
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 616
	0xC3,                                                 // RET ; line 617
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 620
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX5END ; line 621
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 622
	0x6B, 0xC9, 0x03,                                     // IMUL ECX,ECX,0x03 ; line 623
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 624
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 625
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 626
	0x66, 0x81, 0xFB, 0x23, 0x00,                         // CMP BX,0x0023 ; line 627
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX5A ; line 628
	0x66, 0xBB, 0x23, 0x00,                               // MOV BX,0x0023 ; line 629
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x23, 0x00, // CMP WORD PTR [ESI+0x140],0x0023 ; line 631
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX5END ; line 632
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 633
	0x61,                                                 // POPAD ; line 635
	0x66, 0x9D,                                           // POPF ; line 636
	0x0F, 0xB7, 0x86, 0x3E, 0x01, 0x00, 0x00,             // MOVZX EAX,WORD PTR [ESI+0x13E] ; line 637
	0xC3,                                                 // RET ; line 638
	0x81, 0xF8, 0x01, 0x00, 0x00, 0x00,                   // CMP EAX,0x01 ; line 641
	0x0F, 0x84, 0x38, 0x00, 0x00, 0x00,                   // JE SHORT LABELX6END ; line 642
	0x81, 0xC1, 0x03, 0x00, 0x00, 0x00,                   // ADD ECX,0x03 ; line 643
	0x6B, 0xC9, 0x03,                                     // IMUL ECX,ECX,0x03 ; line 644
	0x8B, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV EBX,[ESI+0x140] ; line 645
	0x01, 0xCB,                                           // ADD EBX,ECX ; line 646
	0x0F, 0xB7, 0xDB,                                     // MOVZX EBX,BX ; line 647
	0x66, 0x81, 0xFB, 0x24, 0x00,                         // CMP BX,0x0024 ; line 648
	0x0F, 0x8C, 0x04, 0x00, 0x00, 0x00,                   // JL SHORT LABELX6A ; line 649
	0x66, 0xBB, 0x24, 0x00,                               // MOV BX,0x0024 ; line 650
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x24, 0x00, // CMP WORD PTR [ESI+0x140],0x0024 ; line 652
	0x0F, 0x8F, 0x06, 0x00, 0x00, 0x00,                   // JG SHORT LABELX6END ; line 653
	0x89, 0x9E, 0x40, 0x01, 0x00, 0x00,                   // MOV [ESI+0x140],EBX ; line 654
	0x61,                                                 // POPAD ; line 656
	0x66, 0x9D,                                           // POPF ; line 657
	0xD9, 0x86, 0x1C, 0x01, 0x00, 0x00,                   // FLD DWORD PTR [ESI+0x11C] ; line 658
	0xC3,                                                 // RET ; line 659
	0x90,                                                 // NOP ; line 662
};
static JumpTarget yuukaCHICKENATTACK_jumpData[] = {
	{ 0x0B7, 0x7D0DD4 },
	{ 0x0C2, 0x7D0DD4 },
	{ 0x0D2, 0x7D0DD4 },
};
static PatchByteCode yuukaCHICKENATTACK_patchByteCode = { yuukaCHICKENATTACK_byteCode, 715, yuukaCHICKENATTACK_jumpData, 3 };
// yuukaD22_LV declared in yuuka.asm line 683
static unsigned char yuukaD22_LV_byteCode[] = {
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x00, // CMP BYTE PTR [ESI+0x6A6],0x00 ; line 684
	0x0F, 0x84, 0x2C, 0x00, 0x00, 0x00,       // JE SHORT LV0 ; line 685
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x01, // CMP BYTE PTR [ESI+0x6A6],0x01 ; line 686
	0x0F, 0x84, 0x2A, 0x00, 0x00, 0x00,       // JE SHORT LV1 ; line 687
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x02, // CMP BYTE PTR [ESI+0x6A6],0x02 ; line 688
	0x0F, 0x84, 0x28, 0x00, 0x00, 0x00,       // JE SHORT LV2 ; line 689
	0x80, 0xBE, 0xA6, 0x06, 0x00, 0x00, 0x03, // CMP BYTE PTR [ESI+0x6A6],0x03 ; line 690
	0x0F, 0x84, 0x26, 0x00, 0x00, 0x00,       // JE SHORT LV3 ; line 691
	0xE9, 0x2C, 0x00, 0x00, 0x00,             // JMP SHORT LV4 ; line 692
	0x81, 0xF8, 0x46, 0x00, 0x00, 0x00,       // CMP EAX,0x00000046 ; line 694
	0xE9, 0x2C, 0x00, 0x00, 0x00,             // JMP SHORT END ; line 695
	0x81, 0xF8, 0x64, 0x00, 0x00, 0x00,       // CMP EAX,0x00000064 ; line 697
	0xE9, 0x21, 0x00, 0x00, 0x00,             // JMP SHORT END ; line 698
	0x81, 0xF8, 0x82, 0x00, 0x00, 0x00,       // CMP EAX,0x00000082 ; line 700
	0xE9, 0x16, 0x00, 0x00, 0x00,             // JMP SHORT END ; line 701
	0x81, 0xF8, 0xA0, 0x00, 0x00, 0x00,       // CMP EAX,0x000000A0 ; line 703
	0xE9, 0x0B, 0x00, 0x00, 0x00,             // JMP SHORT END ; line 704
	0x81, 0xF8, 0xBE, 0x00, 0x00, 0x00,       // CMP EAX,0x000000BE ; line 706
	0xE9, 0x00, 0x00, 0x00, 0x00,             // JMP SHORT END ; line 707
	0x90,                                     // NOP ; line 709
};
static PatchByteCode yuukaD22_LV_patchByteCode = { yuukaD22_LV_byteCode, 113, nullptr, 0 };
// yuuka3SCBLADE declared in yuuka.asm line 715
static unsigned char yuuka3SCBLADE_byteCode[] = {
	0xD9, 0x96, 0x1C, 0x01, 0x00, 0x00, // FST DWORD PTR [ESI+0x11C] ; line 716
};
static PatchByteCode yuuka3SCBLADE_patchByteCode = { yuuka3SCBLADE_byteCode, 6, nullptr, 0 };
// yuuka5SCGOS declared in yuuka.asm line 721
static unsigned char yuuka5SCGOS_byteCode[] = {
	0x0F, 0xB6, 0x86, 0x04, 0x01, 0x00, 0x00, // MOVZX EAX,BYTE PTR [ESI+0x104] ; line 722
	0x50,                                     // PUSH EAX ; line 723
	0x0F, 0xBE, 0xC0,                         // MOVSX EAX,AL ; line 724
	0x69, 0xC0, 0x00, 0xFB, 0xFF, 0xFF,       // IMUL EAX,EAX,0xFFFFFB00 ; line 725
	0x81, 0xEC, 0x08, 0x00, 0x00, 0x00,       // SUB ESP,0x08 ; line 726
	0x89, 0x44, 0x24, 0x28,                   // MOV [ESP+0x28],EAX ; line 727
	0x89, 0xF1,                               // MOV ECX,ESI ; line 728
	0xD9, 0xEE,                               // FLDZ ; line 729
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x7AFBF4 ; line 730
};
static JumpTarget yuuka5SCGOS_jumpData[] = {
	{ 0x020, 0x7AFBF4 },
};
static PatchByteCode yuuka5SCGOS_patchByteCode = { yuuka5SCGOS_byteCode, 36, yuuka5SCGOS_jumpData, 1 };
// yuuka5SCGOS1 declared in yuuka.asm line 733
static unsigned char yuuka5SCGOS1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x7C6FF9 ; line 734
};
static JumpTarget yuuka5SCGOS1_jumpData[] = {
	{ 0x001, 0x7C6FF9 },
};
static PatchByteCode yuuka5SCGOS1_patchByteCode = { yuuka5SCGOS1_byteCode, 5, yuuka5SCGOS1_jumpData, 1 };
// yuukaSOUND0 declared in yuuka.asm line 742
static unsigned char yuukaSOUND0_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 743
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0xDD, 0x00, // CMP WORD PTR [ESI+0x140],0x00DD ; line 744
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 745
	0x6A, 0x03,                                           // PUSH 0x03 ; line 746
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 747
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 748
	0x61,                                                 // POPAD ; line 750
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x21, 0x03, // CMP WORD PTR [ESI+0x13C],0x0321 ; line 751
	0x0F, 0x85, 0x06, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 752
	0xD9, 0x05, 0xA8, 0x77, 0x86, 0x00,                   // FLD DWORD PTR [0x8677A8] ; line 753
	0x90,                                                 // NOP ; line 755
};
static JumpTarget yuukaSOUND0_jumpData[] = {
	{ 0x019, 0x464980 },
};
static PatchByteCode yuukaSOUND0_patchByteCode = { yuukaSOUND0_byteCode, 52, yuukaSOUND0_jumpData, 1 };
// yuukaSOUND6B1 declared in yuuka.asm line 763
static unsigned char yuukaSOUND6B1_byteCode[] = {
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF, // JMP 0x7BADDC ; line 764
};
static JumpTarget yuukaSOUND6B1_jumpData[] = {
	{ 0x001, 0x7BADDC },
};
static PatchByteCode yuukaSOUND6B1_patchByteCode = { yuukaSOUND6B1_byteCode, 5, yuukaSOUND6B1_jumpData, 1 };
// yuukaSOUND1 declared in yuuka.asm line 769
static unsigned char yuukaSOUND1_byteCode[] = {
	0xD9, 0xE8,                                           // FLD1 ; line 770
	0x60,                                                 // PUSHAD ; line 771
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x25, 0x00, // CMP WORD PTR [ESI+0x140],0x0025 ; line 772
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 773
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 774
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 775
	0x6A, 0x6D,                                           // PUSH 0x6D ; line 776
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 777
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 778
	0x61,                                                 // POPAD ; line 780
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x59, 0x03, // CMP WORD PTR [ESI+0x13C],0x0359 ; line 781
	0x0F, 0x85, 0x06, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 782
	0xD8, 0x86, 0x78, 0x03, 0x00, 0x00,                   // FADD DWORD PTR [ESI+0x378] ; line 783
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,                         // JMP 0x7BE516 ; line 785
};
static JumpTarget yuukaSOUND1_jumpData[] = {
	{ 0x02A, 0x464980 },
	{ 0x045, 0x7BE516 },
};
static PatchByteCode yuukaSOUND1_patchByteCode = { yuukaSOUND1_byteCode, 73, yuukaSOUND1_jumpData, 2 };
// yuukaSOUND2 declared in yuuka.asm line 789
static unsigned char yuukaSOUND2_byteCode[] = {
	0x60,                                                 // PUSHAD ; line 790
	0x66, 0x81, 0xBE, 0x40, 0x01, 0x00, 0x00, 0x25, 0x00, // CMP WORD PTR [ESI+0x140],0x0025 ; line 791
	0x0F, 0x85, 0x1C, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 792
	0x66, 0x81, 0xBE, 0x42, 0x01, 0x00, 0x00, 0x00, 0x00, // CMP WORD PTR [ESI+0x142],0x0000 ; line 793
	0x0F, 0x85, 0x0D, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL1 ; line 794
	0x6A, 0x6D,                                           // PUSH 0x6D ; line 795
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00,                   // MOV ECX,[ESI+0x398] ; line 796
	0xE8, 0xDE, 0xAD, 0xBE, 0xEF,                         // CALL 0x464980 ; line 797
	0x61,                                                 // POPAD ; line 799
	0x66, 0x81, 0xBE, 0x3C, 0x01, 0x00, 0x00, 0x59, 0x03, // CMP WORD PTR [ESI+0x13C],0x0359 ; line 800
	0x0F, 0x85, 0x06, 0x00, 0x00, 0x00,                   // JNE SHORT LABEL2 ; line 801
	0xD8, 0x86, 0x78, 0x03, 0x00, 0x00,                   // FADD DWORD PTR [ESI+0x378] ; line 802
	0x90,                                                 // NOP ; line 804
};
static JumpTarget yuukaSOUND2_jumpData[] = {
	{ 0x028, 0x464980 },
};
static PatchByteCode yuukaSOUND2_patchByteCode = { yuukaSOUND2_byteCode, 67, yuukaSOUND2_jumpData, 1 };
// yuukaSOUND3 declared in yuuka.asm line 807
static unsigned char yuukaSOUND3_byteCode[] = {
	0x8B, 0x8E, 0x98, 0x03, 0x00, 0x00, // MOV ECX,[ESI+0x398] ; line 808
	0x6A, 0x02,                         // PUSH 0x02 ; line 809
};
static PatchByteCode yuukaSOUND3_patchByteCode = { yuukaSOUND3_byteCode, 8, nullptr, 0 };
// yuukaD214_ANGLE declared in yuuka.asm line 814
static unsigned char yuukaD214_ANGLE_byteCode[] = {
	0xD9, 0x05, 0xBC, 0xC5, 0x85, 0x00, // FLD DWORD PTR [0x85C5BC] ; line 815
};
static PatchByteCode yuukaD214_ANGLE_patchByteCode = { yuukaD214_ANGLE_byteCode, 6, nullptr, 0 };
// yuukaD214_ANGLE1 declared in yuuka.asm line 818
static unsigned char yuukaD214_ANGLE1_byteCode[] = {
	0xD9, 0x05, 0x54, 0xBB, 0x85, 0x00, // FLD DWORD PTR [0x85BB54] ; line 819
};
static PatchByteCode yuukaD214_ANGLE1_patchByteCode = { yuukaD214_ANGLE1_byteCode, 6, nullptr, 0 };
// yuukaA214SP_DUR declared in yuuka.asm line 825
static unsigned char yuukaA214SP_DUR_byteCode[] = {
	0x81, 0xBE, 0x44, 0x01, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, // CMP DWORD PTR [ESI+0x144],0x00000E10 ; line 826
};
static PatchByteCode yuukaA214SP_DUR_patchByteCode = { yuukaA214SP_DUR_byteCode, 10, nullptr, 0 };
// yuukaA214SIGN_COUNT declared in yuuka.asm line 831
static unsigned char yuukaA214SIGN_COUNT_byteCode[] = {
	0x0F, 0xBE, 0x91, 0xAF, 0x06, 0x00, 0x00, // MOVSX EDX,BYTE PTR [ECX+0x6AF] ; line 832
	0x81, 0xFA, 0x01, 0x00, 0x00, 0x00,       // CMP EDX,0x01 ; line 833
	0x0F, 0x85, 0x01, 0x00, 0x00, 0x00,       // JNE SHORT LABEL1 ; line 834
	0x42,                                     // INC EDX ; line 835
	0x68, 0x42, 0x26, 0x7C, 0x00,             // PUSH 0x007C2642 ; line 837
	0xC3,                                     // RET ; line 838
};
static PatchByteCode yuukaA214SIGN_COUNT_patchByteCode = { yuukaA214SIGN_COUNT_byteCode, 26, nullptr, 0 };
// yuukaA214SIGN_ANG declared in yuuka.asm line 842
static unsigned char yuukaA214SIGN_ANG_byteCode[] = {
	0x8B, 0x89, 0x70, 0x01, 0x00, 0x00,       // MOV ECX,[ECX+0x170] ; line 843
	0x0F, 0xBE, 0x81, 0x04, 0x01, 0x00, 0x00, // MOVSX EAX,BYTE PTR [ECX+0x104] ; line 844
	0xE9, 0xDE, 0xAD, 0xBE, 0xEF,             // JMP 0x7C265F ; line 845
};
static JumpTarget yuukaA214SIGN_ANG_jumpData[] = {
	{ 0x00E, 0x7C265F },
};
static PatchByteCode yuukaA214SIGN_ANG_patchByteCode = { yuukaA214SIGN_ANG_byteCode, 18, yuukaA214SIGN_ANG_jumpData, 1 };
// yuukaVILESPARK declared in yuuka.asm line 850
static unsigned char yuukaVILESPARK_byteCode[] = {
	0xC6, 0x86, 0x94, 0x01, 0x00, 0x00, 0x00,             // MOV BYTE PTR [ESI+0x194],0x00 ; line 851
	0x66, 0x81, 0xB9, 0x40, 0x01, 0x00, 0x00, 0x15, 0x00, // CMP WORD PTR [ECX+0x140],0x15 ; line 852
};
static PatchByteCode yuukaVILESPARK_patchByteCode = { yuukaVILESPARK_byteCode, 16, nullptr, 0 };
// yuukaFLIGHT declared in yuuka.asm line 857
static unsigned char yuukaFLIGHT_byteCode[] = {
	0xD9, 0x05, 0x2C, 0x78, 0x86, 0x00, // FLD DWORD PTR [0x86782C] ; line 858
};
static PatchByteCode yuukaFLIGHT_patchByteCode = { yuukaFLIGHT_byteCode, 6, nullptr, 0 };


// yuuka.asm
std::array<PatchSkeleton, 100> patchList = {
	PatchSkeleton{ yuukaID_FIX_patchByteCode,          (void *)0x79FB22,  8}, // Declared line 13. Patch on CMP WORD PTR [ESI+0x196], 0
	PatchSkeleton{ yuukaAIRBACKDASHTURN_patchByteCode, (void *)0x7A0D35,  7}, // Declared line 18. Patch on CMP [ESI+0x140], AX
	PatchSkeleton{ yuukaWALK_patchByteCode,            (void *)0x79FE17,  6}, // Declared line 23. Patch on FLD ST, DWORD PTR [0x871538]
	PatchSkeleton{ yuukaWALKBACK_patchByteCode,        (void *)0x79FE40,  6}, // Declared line 29. Patch on FLD ST, DWORD PTR [0x85B85C]
	PatchSkeleton{ yuuka3SCPALM_HITS_patchByteCode,    (void *)0x7C766A,  7}, // Declared line 34. Patch on MOV BYTE PTR [ESI+0x194], 5
	PatchSkeleton{ yuukaJ8A_Y_patchByteCode,           (void *)0x7A3AD0,  6}, // Declared line 44. Patch on FLD ST, DWORD PTR [0x85B9BC]
	PatchSkeleton{ yuukaBDASH_GRAV_patchByteCode,      (void *)0x7A080E,  6}, // Declared line 49. Patch on FLD ST, DWORD PTR [0x85AD04]
	PatchSkeleton{ yuukaDASH_SPD_patchByteCode,        (void *)0x7A0557,  6}, // Declared line 76. Patch on FLD ST, DWORD PTR [0x85BB28]
	PatchSkeleton{ yuukaADASH_SPD_patchByteCode,       (void *)0x7A0A11,  6}, // Declared line 82. Patch on FLD ST, DWORD PTR [0x85BB28]
	PatchSkeleton{ yuukaABDASH_SPD_patchByteCode,      (void *)0x7A0C71,  6}, // Declared line 88. Patch on FLD ST, DWORD PTR [0x85F450]
	PatchSkeleton{ yuukaDASHCOMMIT_patchByteCode,      (void *)0x7A04FF,  6}, // Declared line 96. Patch on CMP AX, 0x14
	PatchSkeleton{ yuukaHJ7X_patchByteCode,            (void *)0x7A110E,  6}, // Declared line 102. Patch on FLD ST, DWORD PTR [0x85B908]
	PatchSkeleton{ yuukaHJ9X_patchByteCode,            (void *)0x7A0FF9,  6}, // Declared line 107. Patch on FLD ST, DWORD PTR [0x86782C]
	PatchSkeleton{ yuukaHJ8Y_patchByteCode,            (void *)0x7A0EBD,  6}, // Declared line 116. Patch on FLD ST, DWORD PTR [0x85B9D0]
	PatchSkeleton{ yuukaHJ9Y_patchByteCode,            (void *)0x7A100A,  6}, // Declared line 123. Patch on FLD ST, DWORD PTR [0x859F00]
	PatchSkeleton{ yuuka1DDX_patchByteCode,            (void *)0x7A2437,  6}, // Declared line 131. Patch on FLD ST, DWORD PTR [0x85B908]
	PatchSkeleton{ yuukaHJ8Y_patchByteCode,            (void *)0x7A2208,  6}, // Declared line 141. Patch on FLD ST, DWORD PTR [0x85BB30]
	PatchSkeleton{ yuuka2DDGRAV_patchByteCode,         (void *)0x7A2215,  6}, // Declared line 143. Patch on FLD ST, DWORD PTR [0x85C228]
	PatchSkeleton{ yuukaHJ9Y_patchByteCode,            (void *)0x7A2333,  6}, // Declared line 145. Patch on FLD ST, DWORD PTR [0x85BA68]
	PatchSkeleton{ yuuka1DDGRAV_patchByteCode,         (void *)0x7A233F,  6}, // Declared line 147. Patch on FLD ST, DWORD PTR [0x85C228]
	PatchSkeleton{ yuukaCF5A_REMOVE_patchByteCode,     (void *)0x7A2EB6,  7}, // Declared line 154. Patch on CMP BYTE PTR [ESI+0x7F4], 1
	PatchSkeleton{ yuuka3AX_patchByteCode,             (void *)0x7A3476,  6}, // Declared line 160. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ yuukaC3AX_patchByteCode,            (void *)0x7A34B1,  6}, // Declared line 165. Patch on FLD ST, DWORD PTR [0x867810]
	PatchSkeleton{ yuuka3ADEC_patchByteCode,           (void *)0x7A3378,  6}, // Declared line 170. Patch on FSUB ST, QWORD PTR [0x859910]
	PatchSkeleton{ yuuka3AKEY_patchByteCode,           (void *)0x7A341C,  8}, // Declared line 175. Patch on CMP WORD PTR [ESI+0x140], 3
	PatchSkeleton{ yuuka66CX_patchByteCode,            (void *)0x7A6096,  6}, // Declared line 183. Patch on FLD ST, DWORD PTR [0x85B9BC]
	PatchSkeleton{ yuuka66CY_patchByteCode,            (void *)0x7A60A2,  6}, // Declared line 188. Patch on FLD ST, DWORD PTR [0x85B3B0]
	PatchSkeleton{ yuukaFLDZ_patchByteCode,            (void *)0x7BAD49,  6}, // Declared line 194. Patch on FLD ST, DWORD PTR [0x8589F0]
	PatchSkeleton{ yuuka5CSPD_0_patchByteCode,         (void *)0x7BB1F2,  6}, // Declared line 202. Patch on FCOMP ST, QWORD PTR [0x867830]
	PatchSkeleton{ yuuka5CSPD_3_patchByteCode,         (void *)0x7BB1FF,  6}, // Declared line 207. Patch on FLD ST, DWORD PTR [0x85BBE4]
	PatchSkeleton{ yuukaC5C_REMOVE_patchByteCode,      (void *)0x7A5116,  8}, // Declared line 243. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ yuukaC5C_REMOVE_patchByteCode,      (void *)0x7A585C,  8}, // Declared line 245. Patch on CMP WORD PTR [ESI+0x140], 5
	PatchSkeleton{ yuukaFLDZ_patchByteCode,            (void *)0x7A5925,  6}, // Declared line 247. Patch on FLD ST, DWORD PTR [ESI+0xF0]
	PatchSkeleton{ yuuka6B_XPOS1_patchByteCode,        (void *)0x7BAD38,  6}, // Declared line 255. Patch on FMUL ST, QWORD PTR [0x85BA30]
	PatchSkeleton{ yuuka6B_XPOS2_patchByteCode,        (void *)0x7A42A0,  7}, // Declared line 261. Patch on IMUL EDX, 0x35
	PatchSkeleton{ yuuka6B_HITCOUNT_patchByteCode,     (void *)0x7CF39B,  7}, // Declared line 266. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ yuuka6B_HITCOUNT_patchByteCode,     (void *)0x7CF3EB,  7}, // Declared line 267. Patch on MOV BYTE PTR [ESI+0x194], 3
	PatchSkeleton{ yuuka6B_HEIGHT_patchByteCode,       (void *)0x7BB035,  6}, // Declared line 274. Patch on FLD ST, DWORD PTR [EAX+0xF0]
	PatchSkeleton{ yuuka6B_HITRATE1_patchByteCode,     (void *)0x7BB000,  7}, // Declared line 286. Patch on MOVZX EAX, WORD PTR [ESI+0x36C]
	PatchSkeleton{ yuuka6B_HITRATE2_patchByteCode,     (void *)0x7BB0DB,  7}, // Declared line 287. Patch on MOVZX EAX, WORD PTR [ESI+0x36C]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB059,  6}, // Declared line 289. Patch on FMUL ST, QWORD PTR [0x857930]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB08D,  6}, // Declared line 290. Patch on FMUL ST, QWORD PTR [0x8581D8]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB09F,  6}, // Declared line 291. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB134,  6}, // Declared line 292. Patch on FMUL ST, QWORD PTR [0x85C8E0]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB168,  6}, // Declared line 293. Patch on FMUL ST, QWORD PTR [0x8581D8]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7BB17A,  6}, // Declared line 294. Patch on FSUB ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ yuuka6B_DUR_patchByteCode,          (void *)0x7BAD73, 10}, // Declared line 299. Patch on CMP DWORD PTR [ESI+0x144], 0xF0
	PatchSkeleton{ yuukaD236_0_patchByteCode,          (void *)0x7BE52A,  8}, // Declared line 342. Patch on CMP WORD PTR [ESI+0x140], 0xB
	PatchSkeleton{ yuukaD236_1_patchByteCode,          (void *)0x7A814E,  6}, // Declared line 347. Patch on FLD ST, DWORD PTR [0x857DD8]
	PatchSkeleton{ yuukaD236_1_patchByteCode,          (void *)0x7A842B,  6}, // Declared line 348. Patch on FLD ST, DWORD PTR [0x857DD8]
	PatchSkeleton{ yuukaD236_1_patchByteCode,          (void *)0x7A877E,  6}, // Declared line 349. Patch on FLD ST, DWORD PTR [0x857DD8]
	PatchSkeleton{ yuukaD236_1_patchByteCode,          (void *)0x7A8AF9,  6}, // Declared line 350. Patch on FLD ST, DWORD PTR [0x857DD8]
	PatchSkeleton{ yuukaD236_ROT_patchByteCode,        (void *)0x7BE0BD,  6}, // Declared line 373. Patch on FMUL ST, QWORD PTR [0x85B818]
	PatchSkeleton{ yuukaD236_2_patchByteCode,          (void *)0x7BE286,  5}, // Declared line 378. Patch on MOV ECX, 3
	PatchSkeleton{ yuukaD236_LV3_patchByteCode,        (void *)0x7BE2CE,  6}, // Declared line 383. Patch on CMP [EDX+0x6A5], CL
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA524,  5}, // Declared line 395. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA5A0,  5}, // Declared line 396. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA61C,  5}, // Declared line 397. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA698,  5}, // Declared line 398. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA834,  5}, // Declared line 401. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA8B4,  5}, // Declared line 402. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA930,  5}, // Declared line 403. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AA9AC,  5}, // Declared line 404. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AAB88,  5}, // Declared line 406. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AAC04,  5}, // Declared line 407. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AAC80,  5}, // Declared line 408. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AACFA,  5}, // Declared line 409. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AAF5C,  5}, // Declared line 411. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AAFDC,  5}, // Declared line 412. Patch on PUSH 0x336
	PatchSkeleton{ yuukaD22_EFF_patchByteCode,         (void *)0x7AB058,  5}, // Declared line 413. Patch on PUSH 0x336
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BB1D8,  6}, // Declared line 667. Patch on MOV ECX, [ESI+0x35C]
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BB6BD,  6}, // Declared line 668. Patch on MOV EAX, [ESI+0x39C]
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BB020,  6}, // Declared line 670. Patch on MOV EAX, [ESI+0x39C]
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BB0FB,  6}, // Declared line 671. Patch on MOV EAX, [ESI+0x39C]
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BE501,  7}, // Declared line 675. Patch on CMP [ESI+0x140], BP
	PatchSkeleton{ yuukaD236_RECAST_patchByteCode,     (void *)0x7BE4FC,  5}, // Declared line 676. Patch on CALL 0x0048CDE0
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7BE7E7,  7}, // Declared line 679. Patch on MOVZX EAX, WORD PTR [ESI+0x13E]
	PatchSkeleton{ yuukaCHICKENATTACK_patchByteCode,   (void *)0x7C6E0D,  6}, // Declared line 681. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ yuukaD22_LV_patchByteCode,          (void *)0x7A9BC3,  5}, // Declared line 711. Patch on CMP EAX, 0x96
	PatchSkeleton{ yuukaD22_LV_patchByteCode,          (void *)0x7A9D07,  5}, // Declared line 712. Patch on CMP EAX, 0x96
	PatchSkeleton{ yuukaD22_LV_patchByteCode,          (void *)0x7A9E56,  5}, // Declared line 713. Patch on CMP EAX, 0xB4
	PatchSkeleton{ yuuka3SCBLADE_patchByteCode,        (void *)0x7C765B,  8}, // Declared line 718. Patch on JNE SHORT 0x007C7671
	PatchSkeleton{ yuuka5SCGOS_patchByteCode,          (void *)0x7AFBD7,  9}, // Declared line 732. Patch on MOV ECX, ESI
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7C7401,  6}, // Declared line 737. Patch on JNP 0x007BF848
	PatchSkeleton{ yuuka5SCGOS1_patchByteCode,         (void *)0x7C7412,  6}, // Declared line 738. Patch on JP 0x007C6FF9
	PatchSkeleton{ yuukaSOUND0_patchByteCode,          (void *)0x7BB1E1,  6}, // Declared line 758. Patch on FADD ST, QWORD PTR [0x8677C8]
	PatchSkeleton{ yuukaSOUND0_patchByteCode,          (void *)0x7BB01A,  6}, // Declared line 760. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ yuukaSOUND0_patchByteCode,          (void *)0x7BB0F5,  6}, // Declared line 761. Patch on FLD ST, DWORD PTR [0x8677A8]
	PatchSkeleton{ yuukaSOUND6B1_patchByteCode,        (void *)0x7BADCF,  6}, // Declared line 766. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ yuukaSOUND1_patchByteCode,          (void *)0x7BE50A,  6}, // Declared line 787. Patch on FLD ST, DWORD PTR [ESI+0x11C]
	PatchSkeleton{ yuukaSOUND2_patchByteCode,          (void *)0x7C6E13,  6}, // Declared line 806. Patch on FADD ST, DWORD PTR [ESI+0x378]
	PatchSkeleton{ yuukaSOUND3_patchByteCode,          (void *)0x7C735D,  8}, // Declared line 811. Patch on MOV ECX, [ESI+0x398]
	PatchSkeleton{ yuukaD214_ANGLE_patchByteCode,      (void *)0x7ABD67,  6}, // Declared line 817. Patch on FLD ST, DWORD PTR [0x85C500]
	PatchSkeleton{ yuukaD214_ANGLE1_patchByteCode,     (void *)0x7AC150,  6}, // Declared line 821. Patch on FLD ST, DWORD PTR [0x85BC00]
	PatchSkeleton{ yuukaA214SP_DUR_patchByteCode,      (void *)0x7C27BC, 10}, // Declared line 828. Patch on CMP DWORD PTR [ESI+0x144], 0x1A4
	PatchSkeleton{ yuukaA214SIGN_COUNT_patchByteCode,  (void *)0x7C263B,  7}, // Declared line 840. Patch on MOVSX EDX, BYTE PTR [ECX+0x6AF]
	PatchSkeleton{ yuukaA214SIGN_ANG_patchByteCode,    (void *)0x7C2658,  7}, // Declared line 847. Patch on MOVSX EAX, BYTE PTR [ECX+0x104]
	PatchSkeleton{ yuukaNOP_patchByteCode,             (void *)0x7C26BF,  6}, // Declared line 848. Patch on FADD ST, QWORD PTR [0x871510]
	PatchSkeleton{ yuukaVILESPARK_patchByteCode,       (void *)0x7C5DA4,  7}, // Declared line 854. Patch on CMP [ECX+0x140], DI
	PatchSkeleton{ yuukaFLIGHT_patchByteCode,          (void *)0x7A1497,  6}, // Declared line 860. Patch on FLD ST, DWORD PTR [0x858830]
};


static std::array<unsigned, 56> skeletonsForUpdate = {
	   0,   1,   2,   3,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
	  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  30,  31,  32,  34,  48,  49,
	  50,  51,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,
	  69,  78,  79,  80,  82,  92,  93,  99,
};
static std::vector<unsigned> skeletonsForInitializeAction;
static std::vector<unsigned> skeletonsForInitialize;
static std::vector<unsigned> skeletonsForHandleInputs;
static std::vector<unsigned> skeletonsForVUnknown58;
static std::vector<unsigned> skeletonsForVUnknown5C;
static std::vector<unsigned> skeletonsForVUnknown60;
static std::array<unsigned, 42> skeletonsForObjectUpdate = {
	   4,  27,  28,  29,  33,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	  52,  53,  54,  70,  71,  72,  73,  74,  75,  76,  77,  81,  83,  84,  85,  86,
	  87,  88,  89,  90,  91,  94,  95,  96,  97,  98,
};
static std::array<unsigned, 2> skeletonsForObjectInitializeAction = { 35, 36 };



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
	printf("Internal memory used %zu/8822\n", internalMemory.getIndex());
	assert(internalMemory.getIndex() == 8822);
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
