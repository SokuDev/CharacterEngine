import sys
import argparse
import os
import re
import math
import hashlib
import iced_x86
try:
	import termcolor
	has_colors = True
except ModuleNotFoundError:
	has_colors = False


class AssemblyError(Exception):
	def __init__(self, message):
		super().__init__(message)


class UnresolvableError(Exception):
	def __init__(self, message):
		super().__init__(message)


game_vtables = {
	'reimu':     (0x85BF4C, 0x85BEBC),
	'marisa':    (0x85C37C, 0x85C2EC),
	'sakuya':    (0x85C704, 0x85C674),
	'alice':     (0x85C9F4, 0x85C964),
	'patchouli': (0x85CD2C, 0x85CC9C),
	'youmu':     (0x85CFDC, 0x85CF4C),
	'remilia':   (0x85D284, 0x85D1F4),
	'yuyuko':    (0x85D524, 0x85D494),
	'yukari':    (0x85D7BC, 0x85D72C),
	'suika':     (0x85DA84, 0x85D9F4),
	'udonge':    (0x85DD5C, 0x85DCCC),
	'aya':       (0x85E23C, 0x85E1AC),
	'komachi':   (0x85DFAC, 0x85DF1C),
	'iku':       (0x85E4A4, 0x85E414),
	'tenshi':    (0x85E71C, 0x85E68C),
	'sanae':     (0x85EFEC, 0x85EF5C),
	'chirno':    (0x85EAB4, 0x85EA24),
	'meirin':    (0x85ED3C, 0x85ECAC),
	'utsuho':    (0x85F49C, 0x85F55C),
	'suwako':    (0x85F2BC, 0x85F22C),
}

reg_8bits  = ['AL',  'CL',  'DL',  'BL',  'AH',  'CH',  'DH',  'BH']
reg_16bits = ['AX',  'CX',  'DX',  'BX',  'SP',  'BP',  'SI',  'DI']
reg_32bits = ['EAX', 'ECX', 'EDX', 'EBX', 'ESP', 'EBP', 'ESI', 'EDI']
ptr_names  = ['BYTE', 'WORD', 'DWORD', 'QWORD']

REG_TYPE_DIRECT = 0
REG_TYPE_INDIRECT = 1
REG_TYPE_IMMEDIATE = 2

fpu_only_st = {
	'FXCH':  (0xD9, 0xC8),
	'FADDP': (0xDE, 0xC0),
	'FMULP': (0xDE, 0xC8),
	'FSUBP': (0xDE, 0xE8),
	'FDIVP': (0xDE, 0xF8),
	'FSUBRP':(0xDE, 0xE0)
}
fpu_opcodes = {
	'FADD':  (0x00, 0xDC, 0xD8),
	'FMUL':  (0x08, 0xDC, 0xD8),
	'FCOM':  (0x10, 0xDC, 0xD8),
	'FCOMP': (0x18, 0xDC, 0xD8),
	'FSUB':  (0x20, 0xDC, 0xD8),
	'FSUBR': (0x28, 0xDC, 0xD8),
	'FDIV':  (0x30, 0xDC, 0xD8),
	'FLD':   (0x00, 0xDD, 0xD9),
	'FST':   (0x10, 0xDD, 0xD9),
	'FSTP':  (0x18, 0xDD, 0xD9),
	'FLDCW': (0x28, 0xDD, 0xD9),
	'FNSTCW':(0x38, 0xDD, 0xD9),
	'FIADD': (0x00, 0xDE, 0xDA),
	'FIMUL': (0x08, 0xDE, 0xDA),
	'FISUB': (0x20, 0xDE, 0xDA),
	'FDIVRP':(0x30, 0xDE, 0xDE),
	'FILD':  (0x00, 0xDF, 0xDB),
	'FISTP': (0x18, 0xDF, 0xDB),
}
default_params = {
	'FADDP': 'ST(1)',
	'FMULP': 'ST(1)',
	'FSUBP': 'ST(1)',
	'FDIVP': 'ST(1)',
	'FSUBRP':'ST(1)',
	'FCOM':  'ST(1)',
	'FCOMP': 'ST(1)',
	'FDIVRP':'ST(1)',
	'FXCH':  'ST(1)',
	'FNSTSW':'AX'
}
no_operands = {
	'NOP':   (0x90,),
	'FCHS':  (0xD9, 0xE0),
	'FABS':  (0xD9, 0xE1),
	'FLD1':  (0xD9, 0xE8),
	'FLDL2T':(0xD9, 0xE9),
	'FLDPI': (0xD9, 0xEB),
	'FLDZ':  (0xD9, 0xEE),
	'FCOMPP':(0xDE, 0xD9),
	'POPF':  (0x66, 0x9D),
	'PUSHF': (0x66, 0x9C),
	'POPAD': (0x61,),
	'PUSHAD':(0x60,),
	'RET':   (0xC3,),
	'CDQ':   (0x99,),
}
r_operands_bin = {
	'MOV': { 'DD': (0x88, 0x89), 'DIm': ((0xB0,),      (0xB8,)),      'DI': (0x8A, 0x8B), 'IIm': (0xC6, 0xC7), 'r': 0, 'ESP': (0xBC,) },
	'TEST':{ 'DD': (0x84, 0x85), 'DIm': ((0xF6, 0xC0), (0xF7, 0xC0)), 'DI': (0x84, 0x85), 'IIm': (0xF6, 0xF7), 'r': 0, 'ESP': (0xF7, 0xC4) },
	'ADD': { 'DD': (0x00, 0x01), 'DIm': ((0x80, 0xC0), (0x81, 0xC0)), 'DI': (0x02, 0x03), 'IIm': (0x80, 0x81), 'r': 0, 'ESP': (0x83, 0xC4) },
	'OR':  { 'DD': (0x08, 0x09), 'DIm': ((0x80, 0xC8), (0x81, 0xC8)), 'DI': (0x0A, 0x0B), 'IIm': (0x80, 0x81), 'r': 1, 'ESP': (0x83, 0xC4) },
	'ADC': { 'DD': (0x10, 0x11), 'DIm': ((0x80, 0xD0), (0x81, 0xD0)), 'DI': (0x12, 0x13), 'IIm': (0x80, 0x81), 'r': 2, 'ESP': (0x83, 0xC4) },
	'SBB': { 'DD': (0x18, 0x19), 'DIm': ((0x80, 0xD8), (0x81, 0xD8)), 'DI': (0x1A, 0x1B), 'IIm': (0x80, 0x81), 'r': 3, 'ESP': (0x83, 0xC4) },
	'AND': { 'DD': (0x20, 0x21), 'DIm': ((0x80, 0xE0), (0x81, 0xE0)), 'DI': (0x22, 0x23), 'IIm': (0x80, 0x81), 'r': 4, 'ESP': (0x83, 0xC4) },
	'SUB': { 'DD': (0x28, 0x29), 'DIm': ((0x80, 0xE8), (0x81, 0xE8)), 'DI': (0x2A, 0x2B), 'IIm': (0x80, 0x81), 'r': 5, 'ESP': (0x83, 0xC4) },
	'XOR': { 'DD': (0x30, 0x31), 'DIm': ((0x80, 0xF0), (0x81, 0xF0)), 'DI': (0x32, 0x33), 'IIm': (0x80, 0x81), 'r': 6, 'ESP': (0x83, 0xC4) },
	'CMP': { 'DD': (0x38, 0x39), 'DIm': ((0x80, 0xF8), (0x81, 0xF8)), 'DI': (0x3A, 0x3B), 'IIm': (0x80, 0x81), 'r': 7, 'ESP': (0x83, 0xC4) },
	'SHL': { 'DD': None,         'DIm': ((0xC1, 0xE0), (0xC1, 0xE0)), 'DI': None,         'IIm': None,         'r': 0, 'ESP': None },
}
jumps = {
	'CALL': ((0xE8,),       None,   (0xFF, 0xD0), 2),
	'JMP':  ((0xE9,),      (0xEB,), (0xFF, 0xE0), 4),
	'JO':   ((0x0F, 0x80), (0x70,), None, None),
	'JNO':  ((0x0F, 0x81), (0x71,), None, None),
	'JB':   ((0x0F, 0x82), (0x72,), None, None),
	'JNAE': ((0x0F, 0x82), (0x72,), None, None),
	'JC':   ((0x0F, 0x82), (0x72,), None, None),
	'JNB':  ((0x0F, 0x83), (0x73,), None, None),
	'JAE':  ((0x0F, 0x83), (0x73,), None, None),
	'JNC':  ((0x0F, 0x83), (0x73,), None, None),
	'JZ':   ((0x0F, 0x84), (0x74,), None, None),
	'JE':   ((0x0F, 0x84), (0x74,), None, None),
	'JNZ':  ((0x0F, 0x85), (0x75,), None, None),
	'JNE':  ((0x0F, 0x85), (0x75,), None, None),
	'JBE':  ((0x0F, 0x86), (0x76,), None, None),
	'JNA':  ((0x0F, 0x86), (0x76,), None, None),
	'JNBE': ((0x0F, 0x87), (0x77,), None, None),
	'JA':   ((0x0F, 0x87), (0x77,), None, None),
	'JS':   ((0x0F, 0x88), (0x78,), None, None),
	'JNS':  ((0x0F, 0x89), (0x79,), None, None),
	'JP':   ((0x0F, 0x8A), (0x7A,), None, None),
	'JPE':  ((0x0F, 0x8A), (0x7A,), None, None),
	'JNP':  ((0x0F, 0x8B), (0x7B,), None, None),
	'JPO':  ((0x0F, 0x8B), (0x7B,), None, None),
	'JL':   ((0x0F, 0x8C), (0x7C,), None, None),
	'JNGE': ((0x0F, 0x8C), (0x7C,), None, None),
	'JNL':  ((0x0F, 0x8D), (0x7D,), None, None),
	'JGE':  ((0x0F, 0x8D), (0x7D,), None, None),
	'JLE':  ((0x0F, 0x8E), (0x7E,), None, None),
	'JNG':  ((0x0F, 0x8E), (0x7E,), None, None),
	'JNLE': ((0x0F, 0x8F), (0x7F,), None, None),
	'JG':   ((0x0F, 0x8F), (0x7F,), None, None),
}
current_line = None
current_file = None
current_fct = None
has_error = False
args = None


def emit_message(prepend, message):
	s = f'{prepend}: {current_file}'
	if current_fct:
		s += f' in {current_fct}'
	if current_line is not None:
		s += f' line {current_line}'
	s += f': {message}'
	print(s)


def emit_error(message):
	emit_message(colored('error', 'red'), message)
	global has_error
	has_error = True


def emit_warning(message):
	emit_message(colored('warning', 'magenta'), message)


def get_indirect_info(reg, mnemonic):
	content = reg.split('[')[1].split(']')[0].strip()
	values = content.replace('-', '+-').split('+')
	if len(values) > 3:
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
	register = [v for v in values if v.startswith('E') and not '*' in v]
	register_mul = [v for v in values if '*' in v and any(map(lambda d: d in reg_32bits, v.split('*')))]
	constant = [v for v in values if v not in register and v not in register_mul]

	if len(register_mul) > 1:
		if args.debug:
			print("register_mul", register_mul)
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
	if len(constant) > 1:
		if args.debug:
			print("constant", constant)
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
	if len(register) > 2:
		if args.debug:
			print("register", register)
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
	if len(register) == 2 and len(register_mul):
		print("register register_mul", register, register_mul)
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
	for c in constant:
		try:
			int(c, 0)
			continue
		except ValueError:
			pass
		try:
			int(c, 10)
		except ValueError:
			if args.debug:
				print("number", constant)
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")

	if len(register) == 2:
		register_mul = [register[1]]

	if register:
		op, size, extra = get_reg_size(register[0], mnemonic)
		assert size == 32 and op == REG_TYPE_DIRECT
		register = { 'value': extra }
	else:
		register = None
	if register_mul:
		register_mul = register_mul[0].split('*')
		if len(register_mul) > 2:
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
		if len(register_mul) == 1:
			register_mul = [register_mul[0], '1']
		if register_mul[0] not in reg_32bits:
			tmp = register_mul[0]
			register_mul[0] = register_mul[1]
			register_mul[1] = tmp
		op, size, extra1 = get_reg_size(register_mul[0], mnemonic)
		assert size == 32 and op == REG_TYPE_DIRECT
		op, size, extra2 = get_reg_size(register_mul[1], mnemonic)
		if op != REG_TYPE_IMMEDIATE:
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid syntax")
		if extra2 not in [1, 2, 4, 8]:
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Only 1, 2, 4 and 8 are allowed for register multiplication")
		register_mul = { 'valueR': extra1, 'valueM': int(math.log2(extra2)) }
	else:
		register_mul = None
	if constant:
		op, size, extra = get_reg_size(constant[0], mnemonic)
		if size > 32:
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): {constant[0]} is not a 32 bits number")
		constant = { 'value': extra, 'size': size }
	else:
		constant = None

	return register, register_mul, constant


def get_reg_size(reg, mnemonic):
	if '[' in reg and ']' in reg:
		pre = reg.split('[')[0].strip()
		arr = pre.split(' ')
		if pre == '':
			return REG_TYPE_INDIRECT, 0, get_indirect_info(reg, mnemonic)
		if arr[0] not in ptr_names or (len(arr) != 1 and arr[1] != "PTR"):
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid ptr specification")
		return REG_TYPE_INDIRECT, 8 * (1 << ptr_names.index(arr[0])), get_indirect_info(reg, mnemonic)
	if reg in reg_8bits:
		return REG_TYPE_DIRECT, 8, reg_8bits.index(reg)
	if reg in reg_16bits:
		return REG_TYPE_DIRECT, 16, reg_16bits.index(reg)
	if reg in reg_32bits:
		return REG_TYPE_DIRECT, 32, reg_32bits.index(reg)
	try:
		val = int(reg, 0)
	except ValueError:
		try:
			val = int(reg, 10)
		except ValueError:
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Unknown register")
	if val < 0:
		return REG_TYPE_IMMEDIATE, max(8, 1 << (val + 1).bit_length().bit_length()), val
	return REG_TYPE_IMMEDIATE, max(8, 1 << val.bit_length().bit_length()), val


def parse_st_arg(reg, mnemonic):
	if reg == 'ST':
		return 0
	if len(reg) == 3:
		if reg[:2] != "ST":
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid stack index")
		digit = reg[2]
	else:
		if not reg.startswith('ST(') or not reg.endswith(')'):
			raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid stack index")
		digit = reg[3:-1]
	try:
		digit = int(digit)
	except ValueError:
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid stack index")
	if digit >= 8:
		raise AssemblyError(f"Invalid operand to {mnemonic} ({reg}): Invalid stack index")
	return digit


def assemble(line, pos, debug, labels):
	arr = line.split(' ')
	mnemonic = arr[0]
	if len(arr) == 1:
		args = []
	else:
		args = list(map(lambda d: d.strip(), " ".join(arr[1:]).split(',')))
	if debug:
		print(f"Assembling mnemonic {mnemonic} with args {args}")
	if mnemonic in default_params and len(args) == 0:
		args = [default_params[mnemonic]]

	arr = ['MUL', 'IMUL', 'DIV', 'IDIV']
	if mnemonic in arr:
		if len(args) == 0 or len(args) > 3:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 to 3 operands")
		if len(args) == 1:
			op, size, extra = get_reg_size(args[0], mnemonic)
			if op != REG_TYPE_DIRECT:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected direct register")
			b = 0xE0 | (arr.index(mnemonic) << 3)
			if size == 8:
				return [0xF6, b | extra]
			if size == 16:
				return [0x66, 0xF7, b | extra]
			return [0xF7, b | extra]
		if mnemonic != 'IMUL':
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
		dop, dsize, dextra = get_reg_size(args[0], mnemonic)
		if dop != REG_TYPE_DIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): First operand must be a direct register")
		if len(args) == 2:
			op1, size1, extra1 = get_reg_size(args[1], mnemonic)
			if op1 == REG_TYPE_DIRECT:
				return [0x0F, 0xAF, 0xC0 | extra1 | (dextra << 3)]
			op2, size2, extra2 = op1, size1, extra1
			op1, size1, extra1 = dop, dsize, dextra
		else:
			op1, size1, extra1 = get_reg_size(args[1], mnemonic)
			op2, size2, extra2 = get_reg_size(args[2], mnemonic)
		if op1 != REG_TYPE_DIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Second operand must be a direct register")
		if op2 == REG_TYPE_IMMEDIATE:
			if size2 > dsize:
				old_value = extra2
				max_val = 1 << dsize
				extra2 %= max_val
				if extra2 >= max_val // 2:
					extra2 -= max_val
				if extra2 < 0:
					size2 = max(8, 1 << (extra2 + 1).bit_length().bit_length())
				else:
					size2 = max(8, 1 << extra2.bit_length().bit_length())
				if debug:
					print(f"Value {old_value} truncated to {extra2} ({dsize})")
				emit_warning(f'Immediate value truncated')
			if size2 <= 8:
				imm_size = 1
				opcode = [0x6B]
			else:
				imm_size = 4
				opcode = [0x69]
			opcode += [0xC0 | (dextra << 3) | extra1]
			opcode += list(extra2.to_bytes(imm_size, byteorder='little', signed=True))
			return opcode

		if op2 != REG_TYPE_INDIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Third operand must be an immediate or indirect value")
		if dextra != extra1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")

		extra_ind = extra2
		has_reg1 = extra_ind[0] is not None
		has_regM = extra_ind[1] is not None
		has_regI = extra_ind[2] is not None
		reg1 = extra_ind[0]['value'] if has_reg1 else None
		regM = (extra_ind[1]['valueR'], extra_ind[1]['valueM']) if has_regM else None
		regI = (extra_ind[2]['value'], extra_ind[2]['size']) if has_regI else (0, 0)
		if not has_regI or not has_reg1 or has_regM:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Not implemented")
		return [0x0F, 0xAF, 0x80 | (dextra << 3) | reg1] + list(regI[0].to_bytes(4, byteorder='little', signed=True))

	# FPU
	if mnemonic == 'FNSTSW':
		if args[0] != 'AX':
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected AX")
		return [0xDF, 0xE0]
	if mnemonic in fpu_only_st:
		args = list(filter(lambda d: d != 'ST', args))
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
		i = parse_st_arg(args[0], mnemonic)
		l = list(fpu_only_st[mnemonic])
		l[-1] |= i
		return l
	if mnemonic in fpu_opcodes:
		if len(args) == 0:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 or 2 operands")
		opcodes = fpu_opcodes[mnemonic]
		if len(args) == 2 and args[0].startswith('ST') and args[1] == 'ST' and not mnemonic.startswith('FST'):
			args[1] = 'ST0'
		args = list(filter(lambda d: d != 'ST', args))
		if len(args) == 2 or args[0].startswith('ST'):
			if len(args) == 1:
				st1 = 0
				st2 = parse_st_arg(args[0], mnemonic)
			else:
				st1 = parse_st_arg(args[0], mnemonic)
				st2 = parse_st_arg(args[1], mnemonic)
			if st1 != 0 and st2 != 0:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): One operand must be st(0)")
			if mnemonic.startswith('FST'):
				return [opcodes[1], 0xC0 | opcodes[0] | st2]
			elif mnemonic.startswith('FLD'):
				return [opcodes[2], 0xC0 | opcodes[0] | st2]
			elif st1 == 0:
				return [opcodes[2], 0xC0 | opcodes[0] | st2]
			elif mnemonic == 'FSUB':
				return [opcodes[1], 0xC8 | opcodes[0] | st1]
			else:
				return [opcodes[1], 0xC0 | opcodes[0] | st1]
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 or 2 operands")
		op, size, extra_ind = get_reg_size(args[0], mnemonic)
		if mnemonic in ['FNSTCW', 'FLDCW'] and size == 0:
			size = 16
		if op != REG_TYPE_INDIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
		has_reg1 = extra_ind[0] is not None
		has_regM = extra_ind[1] is not None
		has_regI = extra_ind[2] is not None
		reg1 = extra_ind[0]['value'] if has_reg1 else None
		regM = (extra_ind[1]['valueR'], extra_ind[1]['valueM']) if has_regM else None
		regI = (extra_ind[2]['value'], extra_ind[2]['size']) if has_regI else (0, 0)
		imm_size = 1
		if size == 64:
			opcode = [opcodes[1]]
		else:
			opcode = [opcodes[2]]
		flag_pos = len(opcode)
		opcode.append(opcodes[0])

		if reg1 == 5:
			has_regI = True
		elif reg1 == 4 and not (has_reg1 and has_regM and not has_regI):
			opcode.append(0x24)
		if has_reg1 and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(reg1 | (regM[0] << 3) | (regM[1] << 6))
		elif has_regM:
			has_regI = True
		elif has_reg1:
			opcode[flag_pos] |= reg1
		if has_regI and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(0x5 | (regM[0] << 3) | (regM[1] << 6))
			opcode += list(regI[0].to_bytes(4, byteorder='little', signed=True))
		elif has_regI:
			if not has_reg1:
				imm_size = 4
				opcode[flag_pos] |= 0x5
			elif regI[1] <= 8:
				opcode[flag_pos] |= 0x40
			else:
				opcode[flag_pos] |= 0x80
				imm_size = 4
			opcode += list(regI[0].to_bytes(imm_size, byteorder='little', signed=True))
		return opcode

	if mnemonic in no_operands:
		if len(args) != 0:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected no operand")
		return no_operands[mnemonic]

	if mnemonic == 'NEG':
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
		op, size, extra = get_reg_size(args[0], mnemonic)
		if op != REG_TYPE_DIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Not implemented")
		if size == 8:
			return [0xF6, 0xD8 | extra]
		if size == 16:
			return [0x66, 0xF7, 0xD8 | extra]
		return [0xF7, 0xD8 | extra]

	if mnemonic == 'SETE':
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
		op, size, extra = get_reg_size(args[0], mnemonic)
		if op != REG_TYPE_DIRECT or size != 8:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
		return [0x0F, 0x94, 0xC0 | extra]

	if mnemonic in ['MOVZX', 'MOVSX']:
		if len(args) != 2:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 2 operands")
		op1, size1, extra1 = get_reg_size(args[0], mnemonic)
		op2, size2, extra2 = get_reg_size(args[1], mnemonic)
		if op1 != REG_TYPE_DIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): First operand must be a register")
		if op2 == REG_TYPE_IMMEDIATE:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Values can't be immediate")
		if size1 <= size2:
			print(f"Bit size {size1} ({args[0]}) <= {size2} ({args[1]})")
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operands size")
		opcode = [0x0F]
		if mnemonic == 'MOVZX':
			if size2 == 8:
				opcode.append(0xB6)
			if size2 == 16:
				opcode.append(0xB7)
		else:
			if size2 == 8:
				opcode.append(0xBE)
			if size2 == 16:
				opcode.append(0xBF)
		if size1 == 16:
			opcode = [0x66] + opcode
		if op2 == REG_TYPE_DIRECT:
			return opcode + [0xC0 | extra2 | (extra1 << 3)]

		extra_reg = extra1
		extra_ind = extra2

		has_reg1 = extra_ind[0] is not None
		has_regM = extra_ind[1] is not None
		has_regI = extra_ind[2] is not None
		reg1 = extra_ind[0]['value'] if has_reg1 else None
		regM = (extra_ind[1]['valueR'], extra_ind[1]['valueM']) if has_regM else None
		regI = (extra_ind[2]['value'], extra_ind[2]['size']) if has_regI else (0, 0)
		imm_size = 1
		flag_pos = len(opcode)
		opcode.append(extra_reg << 3)

		if reg1 == 5:
			has_regI = True
		elif reg1 == 4:
			opcode.append(0x24)
		if has_reg1 and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(reg1 | (regM['valueR'] << 3) | (regM["valueM"] << 6))
		elif has_regM:
			has_regI = True
		elif has_reg1:
			opcode[flag_pos] |= reg1
		if has_regI and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(0x5 | (regM['valueR'] << 3) | (regM["valueM"] << 6))
			opcode += list(regI[0].to_bytes(4, byteorder='little', signed=True))
		elif has_regI:
			if regI[1] <= 8:
				opcode[flag_pos] |= 0x40
			else:
				opcode[flag_pos] |= 0x80
				imm_size = 4
			opcode += list(regI[0].to_bytes(imm_size, byteorder='little', signed=True))
		return opcode

	arr = ['INC', 'DEC', 'PUSH', 'POP']
	if mnemonic in arr:
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
		op, size, extra = get_reg_size(args[0], mnemonic)
		if op == REG_TYPE_DIRECT:
			if size == 32:
				opcode = 0x40 + arr.index(mnemonic) * 8
				return [opcode + extra]
			if size == 16:
				opcode = 0x40 + arr.index(mnemonic) * 8
				return [0x66, opcode + extra]
			if arr.index(mnemonic) >= 2:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			return [0xFE, 0xC0 | extra | (arr.index(mnemonic) << 3)]
		if op == REG_TYPE_IMMEDIATE:
			if mnemonic != 'PUSH':
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			if size <= 8:
				return [0x6A] + list(extra.to_bytes(1, byteorder='little', signed=True))
			elif size > 32:
				old_value = extra
				max_val = 1 << 32
				extra %= max_val
				if extra >= max_val // 2:
					extra -= max_val
				if debug:
					print(f"Value {old_value} truncated to {extra} (32)")
				emit_warning(f'Immediate value truncated')
			return [0x68] + list(extra.to_bytes(4, byteorder='little', signed=True))

		has_reg1 = extra[0] is not None
		has_regM = extra[1] is not None
		has_regI = extra[2] is not None
		reg1 = extra[0]['value'] if has_reg1 else None
		regM = (extra[1]['valueR'], extra[1]['valueM']) if has_regM else None
		regI = (extra[2]['value'], extra[2]['size']) if has_regI else (0, 0)

		if mnemonic == 'PUSH':
			if size != 32:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			return [0xFF, 0xB0 | reg1] + list(regI[0].to_bytes(4, byteorder='little', signed=True))

		if arr.index(mnemonic) >= 2:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
		if size == 8:
			return [0xFE, 0x80 | reg1 | (arr.index(mnemonic) << 3)] + list(regI[0].to_bytes(4, byteorder='little', signed=True))
		elif size == 16:
			return [0x66, 0xFF, 0x80 | reg1 | (arr.index(mnemonic) << 3)] + list(regI[0].to_bytes(4, byteorder='little', signed=True))
		return [0xFF, 0x80 | reg1 | (arr.index(mnemonic) << 3)] + list(regI[0].to_bytes(4, byteorder='little', signed=True))

	# Branch
	if mnemonic in jumps:
		opcodes = jumps[mnemonic]
		if len(args) != 1:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
		if mnemonic == 'JMP' and args == ['DWORD PTR [EAX*0x4+0x5C6B6C]']:
			return [0xFF, 0x24, 0x85, 0x6C, 0x6B, 0x5C, 0x00]
		if mnemonic == 'JMP' and args == ['DWORD PTR [EAX*4+0x5C6B6C]']:
			return [0xFF, 0x24, 0x85, 0x6C, 0x6B, 0x5C, 0x00]
		args = args[0].split(" ")
		if args[0] == "SHORT":
			if len(args[0]) is None:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
			if opcodes[1] is None:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 1 operand")
			args.pop(0)
			# labels.append([args[1], pos + len(opcodes[1]), 1])
			# return list(opcodes[1]) + [0xFF]
		if args[0].startswith('['):
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Indirect JMPs are not implemented")
		if args[0] in reg_32bits:
			if opcodes[2] is None:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): {mnemonic} doesn't support indirect mode")
			l = list(opcodes[2])
			l[-1] |= reg_32bits.index(args[0])
			return l
		labels.append([args[0], pos + len(opcodes[0]), 4])
		return list(opcodes[0]) + [0xDE, 0xAD, 0xBE, 0xEF]

	if mnemonic == 'LEA':
		if len(args) != 2:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 2 operands")

		op1, size1, extra1 = get_reg_size(args[0], mnemonic)
		op2, size2, extra2 = get_reg_size(args[1], mnemonic)
		if op1 != REG_TYPE_DIRECT or op2 != REG_TYPE_INDIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operands")
		if not size1:
			size1 = size2
		if not size2:
			size2 = size1
		if size1 != size2:
			if debug:
				print(f"Bit size {size1} ({args[0]}) != {size2} ({args[1]})")
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Bit size mismatch")
		if size1 != 32:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Register must be 32bits")

		opcode = [0x8D]

		extra_reg = extra1
		extra_ind = extra2

		has_reg1 = extra_ind[0] is not None
		has_regM = extra_ind[1] is not None
		has_regI = extra_ind[2] is not None and extra_ind[2]['value'] != 0
		reg1 = extra_ind[0]['value'] if has_reg1 else None
		regM = (extra_ind[1]['valueR'], extra_ind[1]['valueM']) if has_regM else None
		regI = (extra_ind[2]['value'], extra_ind[2]['size']) if has_regI else (0, 0)
		imm_size = 1
		flag_pos = len(opcode)
		opcode.append(extra_reg << 3)

		if reg1 == 5:
			has_regI = True
		elif reg1 == 4 and not has_regM:
			opcode.append(0x24)
		if has_reg1 and has_regM:
			pass
		elif has_regM:
			has_regI = True
		elif has_reg1:
			opcode[flag_pos] |= reg1
		if has_reg1 and has_regI and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(reg1 | (regM[0] << 3) | (regM[1] << 6))
			if regI[1] <= 8:
				opcode[flag_pos] |= 0x40
			else:
				opcode[flag_pos] |= 0x80
				imm_size = 4
			opcode += list(regI[0].to_bytes(imm_size, byteorder='little', signed=True))
		elif has_regI and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(0x5 | (regM[0] << 3) | (regM[1] << 6))
			opcode += list(regI[0].to_bytes(4, byteorder='little', signed=True))
		elif has_regI:
			if regI[1] <= 8:
				opcode[flag_pos] |= 0x40
			else:
				opcode[flag_pos] |= 0x80
				imm_size = 4
			opcode += list(regI[0].to_bytes(imm_size, byteorder='little', signed=True))
		return opcode

	# Bin 2 operands with immediate, direct and indirect values
	if mnemonic in r_operands_bin:
		if len(args) != 2:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Expected 2 operands")
		opcodes = r_operands_bin[mnemonic]
		op1, size1, extra1 = get_reg_size(args[0], mnemonic)
		op2, size2, extra2 = get_reg_size(args[1], mnemonic)
		if not size1:
			size1 = size2
		if not size2:
			size2 = size1
		if op2 == REG_TYPE_IMMEDIATE and size2 > size1:
			old_value = extra2
			max_val = 1 << size1
			extra2 %= max_val
			if extra2 >= max_val // 2:
				extra2 -= max_val
			if extra2 < 0:
				size2 = max(8, 1 << (extra2 + 1).bit_length().bit_length())
			else:
				size2 = max(8, 1 << extra2.bit_length().bit_length())
			if debug:
				print(f"Value {old_value} truncated to {extra2} ({size1})")
			emit_warning(f'Immediate value truncated')
		if size1 != size2 and op2 != REG_TYPE_IMMEDIATE or size2 > size1:
			if debug:
				print(f"Bit size {size1} ({args[0]}) != {size2} ({args[1]})")
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Bit size mismatch")
		if op1 == REG_TYPE_IMMEDIATE:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): First value can't be immediate")
		if op1 == op2 == REG_TYPE_INDIRECT:
			raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): {mnemonic} between 2 indirect values")
		if op1 == op2 == REG_TYPE_DIRECT:
			if not opcodes['DD']:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			if size1 == 8:
				return [opcodes['DD'][0], 0xC0 | extra1 | (extra2 << 3)]
			if size1 == 16:
				return [0x66, opcodes['DD'][1], 0xC0 | extra1 | (extra2 << 3)]
			return [opcodes['DD'][1], 0xC0 | extra1 | (extra2 << 3)]
		if op1 == REG_TYPE_DIRECT and op2 == REG_TYPE_IMMEDIATE:
			if not opcodes['DIm']:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			if size1 <= 8 or mnemonic == 'SHL':
				opcode = list(opcodes['DIm'][0])
				size = 1
			elif size1 <= 16:
				opcode = [0x66] + list(opcodes['DIm'][1])
				size = 2
			elif size1 <= 32:
				opcode = list(opcodes['DIm'][1])
				size = 4
			else:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			opcode[-1] |= extra1
			return opcode + list(extra2.to_bytes(size, byteorder='little', signed=True))

		append_extra = []
		if op1 == REG_TYPE_DIRECT and op2 == REG_TYPE_INDIRECT:
			if debug:
				print("DIRECT INDIRECT", extra2)
			if size1 == 8:
				if not opcodes['DI'][0]:
					raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
				opcode = [opcodes['DI'][0]]
			elif size1 == 16:
				if not opcodes['DI'][0]:
					raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
				opcode = [0x66, opcodes['DI'][1]]
			elif size1 == 32:
				opcode = [opcodes['DI'][1]]
			extra_reg = extra1
			extra_ind = extra2
		elif op1 == REG_TYPE_INDIRECT and op2 == REG_TYPE_DIRECT:
			if not opcodes['DD']:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			if debug:
				print("INDIRECT DIRECT", extra1)
			if size1 == 8:
				opcode = [opcodes['DD'][0]]
			elif size1 == 16:
				opcode = [0x66, opcodes['DD'][1]]
			elif size1 == 32:
				opcode = [opcodes['DD'][1]]
			extra_reg = extra2
			extra_ind = extra1
		else:
			if not opcodes['IIm']:
				raise AssemblyError(f"Invalid operands to {mnemonic} ({",".join(args)}): Invalid operand")
			if debug:
				print("INDIRECT IMMEDIATE", extra1)
			if size1 == 8:
				opcode = [opcodes['IIm'][0]]
			elif size1 == 16:
				opcode = [0x66, opcodes['IIm'][1]]
			elif size1 == 32:
				opcode = [opcodes['IIm'][1]]
			append_extra = list(extra2.to_bytes(size1 // 8, byteorder='little', signed=True))
			extra_reg = opcodes['r']
			extra_ind = extra1

		has_reg1 = extra_ind[0] is not None
		has_regM = extra_ind[1] is not None
		has_regI = extra_ind[2] is not None
		reg1 = extra_ind[0]['value'] if has_reg1 else None
		regM = (extra_ind[1]['valueR'], extra_ind[1]['valueM']) if has_regM else None
		regI = (extra_ind[2]['value'], extra_ind[2]['size']) if has_regI else (0, 0)
		imm_size = 1
		flag_pos = len(opcode)
		opcode.append(extra_reg << 3)

		if reg1 == 5:
			has_regI = True
		elif reg1 == 4:
			opcode.append(0x24)
		if has_reg1 and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(reg1 | (regM[0] << 3) | (regM[1] << 6))
		elif has_regM:
			has_regI = True
		elif has_reg1:
			opcode[flag_pos] |= reg1
		if has_regI and has_regM:
			opcode[flag_pos] |= 0x4
			opcode.append(0x5 | (regM[0] << 3) | (regM[1] << 6))
			opcode += list(regI[0].to_bytes(4, byteorder='little', signed=True))
		elif has_regI:
			if regI[1] <= 8:
				opcode[flag_pos] |= 0x40
			else:
				opcode[flag_pos] |= 0x80
				imm_size = 4
			opcode += list(regI[0].to_bytes(imm_size, byteorder='little', signed=True))
		return opcode + append_extra
	raise AssemblyError(f"Unknown mnemonic '{mnemonic}'")


def resolve_label(labels, label, offset, size, bytecode):
	try:
		int(label, 0)
		raise UnresolvableError(f'{label} is an absolute address')
	except ValueError:
		pass
	if label not in labels:
		raise AssemblyError(f"Label {label} was never declared")
	code_offset = labels[label][0]
	relative = code_offset - (offset + size)
	try:
		b = relative.to_bytes(size, byteorder='little', signed=True)
	except OverflowError:
		raise AssemblyError(f"Label {label} is too far away ({label} is declared at {code_offset}, trying to jump from {offset + size})")
	for i in range(size):
		bytecode[offset + i] = b[i]


def assemble_block(asm_block, debug_gen, debug_asm):
	bytecode = []
	comments = []
	labels = {}
	unlinked = []
	labels_to_resolve = []
	old_label_size = 0
	global current_line

	for pos, asm in asm_block:
		asm = re.sub('\\s+', ' ', asm).upper().replace('0X', '0x')
		current_line = pos
		gg = None
		try:
			if asm.endswith(':'):
				label = asm[:-1]
				if label in labels:
					raise AssemblyError(f"Label {label} was already declared on line{labels[label][1]}")
				labels[label] = (len(bytecode), pos)
			else:
				generated = assemble(asm, len(bytecode), debug_asm, labels_to_resolve)
				if old_label_size != len(labels_to_resolve):
					labels_to_resolve[-1].append(pos)
					labels_to_resolve[-1].append(asm)
				decoder = iced_x86.Decoder(32, bytes(generated))
				assert decoder.can_decode
				instr = decoder.decode()
				assert instr.len == len(generated)
				try:
					gg = assemble(f'{instr:iXrsU}', len(bytecode), debug_asm, [])
					assert generated == gg
				except AssemblyError as e:
					print(e)
					assert False
				old_label_size = len(labels_to_resolve)
				if debug_gen:
					print(f"{asm} -> {" ".join(map(lambda d: f'{d:02X}', generated))}")
				bytecode += generated
				comments.append({ 'caption': f"{asm} ; line {pos}", 'offset': len(bytecode) })
		except AssemblyError as e:
			emit_error(f'{str(e)}')
		except AssertionError as e:
			if gg is not None:
				emit_error(f"Invalid bytecode generated for {asm} ({" ".join(map(lambda d: f'{d:02X}', generated))}) but the generated instruction is {instr:iXrsU} ({" ".join(map(lambda d: f'{d:02X}', gg))})")
			else:
				emit_error(f"Invalid bytecode generated for {asm} ({" ".join(map(lambda d: f'{d:02X}', generated))}) but the generated instruction is {instr:iXrsU}: {e}")


	for label, offset, size, line, asm in labels_to_resolve:
		current_line = line
		try:
			resolve_label(labels, label, offset, size, bytecode)
		except AssemblyError as e:
			emit_error(f'{str(e)}')
		except UnresolvableError:
			if size != 4:
				raise AssemblyError(f"Cannot link label {label} which cannot be found and is a short jump")
			unlinked.append({'offset': offset, 'size': size, 'target': int(label, 0)})

	current_line = None
	return { 'bytecode': bytecode, 'comments': comments, 'unlinked': unlinked }


def colored(text, color):
	if not has_colors or os.name == 'win':
		return text
	return termcolor.colored(text, color)


def parse_function(lines, index):
	block = []
	while lines[index][1] != 'end':
		block.append(lines[index])
		index += 1
	return block, index + 1


def get_instruction_size(data, offset):
	decoder = iced_x86.Decoder(32, data[offset:], ip=offset + 0x400000)
	instr = decoder.decode()
	total = instr.len
	while total < 5:
		decoder.decode_out(instr)
		total += instr.len
	return total


def get_function_bounds(data, vtable, v_offset):
	vtable += v_offset * 4
	vtable -= 0x400000
	addr = int.from_bytes(data[vtable:vtable + 4], 'little', signed=False)
	if addr == 0:
		raise
	decoder = iced_x86.Decoder(32, data[addr - 0x400000:], ip=addr)
	instr = decoder.decode()
	while f'{instr}' != 'int 3':
		decoder.decode_out(instr)
	return addr, addr + decoder.position - 1


def main():
	global has_error
	global current_file
	global current_fct
	global args
	global current_line

	parser = argparse.ArgumentParser(prog=sys.argv[0], description='Convert Soku2 ASM files to CharacterEngine characters patches', epilog='')
	parser.add_argument('input', help="Input assembly file")
	parser.add_argument('output_path', help="Output directory")
	parser.add_argument('--major', help="Version major.", required=True)
	parser.add_argument('--minor', help="Version minor.", required=True)
	parser.add_argument('-s', '--soku_exe', help="Path to th123.exe.", required=True)
	parser.add_argument('-b', '--base', help="Base character class.", required=True)
	parser.add_argument('-v', '--csv', help="Soku2 character CSV.", required=True)
	parser.add_argument('-c', '--class_name', help="Name of class emited.", required=True)
	parser.add_argument('-e', '--emit_unused', help="Parse and emit unused assembly blocks in the patch file. By default, they are not parsed.", action='store_true')
	parser.add_argument('-d', '--debug', action='store_true')
	parser.add_argument('-2', '--debug2', action='store_true')
	args = parser.parse_args()

	has_error = False
	current_fct = None
	current_line = None
	current_file = args.input
	with open(args.input, "r") as fd:
		text = fd.read()
	with open(args.soku_exe, "rb") as fd:
		soku_data = fd.read()
	with open(args.csv, "r") as fd:
		csv_line = [l for l in fd.read().split('\n') if args.class_name.lower() in l]
		if not csv_line:
			emit_error(f'{args.base.lower()} not found in character CSV')
		else:
			csv = csv_line[0].split(';')
	base = args.base.lower()
	if base not in game_vtables:
		emit_error(f'{base} is not a valid soku character')
	if hashlib.md5(soku_data).hexdigest() != 'df35d1fbc7b583317adabe8cd9f53b2e':
		emit_error(f'{args.soku_exe} isn\'t Touhou 12.3 1.10a executable')


	lines = [line.split(';')[0].strip() for line in text.split('\n')]
	lines = [(i + 1, l) for i, l in enumerate(lines) if l]
	function_blocks = []
	locations = []

	line_index = 0
	while line_index < len(lines):
		pos, line = lines[line_index]
		if line.startswith('func '):
			start = pos
			block, line_index = parse_function(lines, line_index + 1)
			name = line.split(' ')[-1]
			try:
				elem = next(f for f in function_blocks if f['name'] == name)
				print(f"{colored('error', 'red')}: line {pos} on declaration of {name}: {name} has already been declared at line {elem['pos']}.")
				has_error = True
			except StopIteration as e:
				pass
			function_blocks.append({'name': name, 'asm': block, 'pos': start})
		else:
			operands = [f.strip() for f in line.split('=')]
			arr = operands[1].split(':')
			addr = int(operands[0], 16)
			locations.append({'address': addr, 'name': arr[0], 'pos': pos, 'size': int(arr[1]) if len(arr) >= 2 else get_instruction_size(soku_data, addr - 0x400000)})
			line_index += 1

	used = set(f['name'] for f in locations)
	unused = [f['name'] for f in function_blocks if f['name'] not in used]

	if args.debug:
		print("Function blocks found:\n - " + "\n - ".join(map(lambda d: f"'{d['name']}' declared line {d['pos']}:\n{"\n".join(map(lambda g: f'\t{g[1]} ; line {g[0]}' if ':' not in g[1] else g[1], d['asm']))}", function_blocks)))
		print("Locations found:\n - " + "\n - ".join(map(lambda d: f"'{d['name']}' at 0x{d['address']:X} declared line {d['pos']}", locations)))
		print("Used functions", used)
		print("Unused functions", unused)

	if args.emit_unused:
		for warning in map(lambda d: f"{d} is never used.", unused):
			emit_warning(warning)
	else:
		for warning in map(lambda d: f"{d} is never used and will not be emited.", unused):
			emit_warning(warning)
		function_blocks = [f for f in function_blocks if f['name'] in used]

	assembled_blocks = []
	for block in function_blocks:
		data = assemble_block(block['asm'], args.debug, args.debug2)
		data['name'] = block['name']
		data['pos'] = block['pos']
		assembled_blocks.append(data)
	if has_error:
		exit(1)

	with open("class_template.hpp", "r") as fd:
		header_template = fd.read()
	with open("class_template.cpp", "r") as fd:
		source_template = fd.read()
	with open("entry_point_template.cpp", "r") as fd:
		entry_point_template = fd.read()
	with open("CMakeLists_template.txt", "r") as fd:
		cmake_template = fd.read()
	with open("version_template.rc", "r") as fd:
		version_template = fd.read()
	with open("character_template.json", "r") as fd:
		character_template = fd.read()
	with open("code_patches_template.hpp", "r") as fd:
		patches_header_template = fd.read()
	with open("code_patches_template.cpp", "r") as fd:
		patches_source_template = fd.read()

	CLASS_NAME = args.class_name.upper()
	ClassName = args.class_name.capitalize()
	class_name = args.class_name.lower()
	BaseName = args.base.capitalize()
	MAJOR = args.major
	MINOR = args.minor

	PATCH_BYTECODE = ""
	PATCH_SKELETONS = ""
	PATCH_SKELETONS_INDICES = ""

	for block in assembled_blocks:
		comments = block['comments']
		current_comment = 0
		code_max = 0
		last_offset = 0
		for comment in comments:
			current = comment['offset'] - last_offset
			code_max = max(code_max, current)
			last_offset = comment['offset']
		last_offset = -1
		first = True
		#'bytecode': bytecode, 'comments': comments, 'unlinked': unlinked
		PATCH_BYTECODE += f'// {block['name']} declared in {current_file.split('/')[-1]} line {block['pos']}\n'
		if len(block['bytecode']) != 0:
			PATCH_BYTECODE += f'static unsigned char {block['name']}_byteCode[] = {{\n'
			for index, byte in enumerate(block['bytecode']):
				if first:
					first = False
					PATCH_BYTECODE += '\t'
				PATCH_BYTECODE += f"0x{byte:02X}, "
				if current_comment < len(comments) and comments[current_comment]['offset'] == index + 1:
					PATCH_BYTECODE += f"{" " * ((code_max - (index - last_offset)) * 6)}// {comments[current_comment]['caption']}\n"
					first = True
					current_comment += 1
					last_offset = index
			if not first:
				PATCH_BYTECODE += "\n"
			PATCH_BYTECODE += '};\n'

		if block['unlinked']:
			PATCH_BYTECODE += f'static JumpTarget {block['name']}_jumpData[] = {{\n'
			for jmp in block['unlinked']:
				PATCH_BYTECODE += f'\t{{ 0x{jmp['offset']:03X}, 0x{jmp['target']:06X} }},\n'
			PATCH_BYTECODE += '};\n'

		PATCH_BYTECODE += f'static PatchByteCode {block['name']}_patchByteCode = {{ '
		if block['bytecode']:
			PATCH_BYTECODE += f'{block['name']}_byteCode, {len(block['bytecode'])}, '
		else:
			PATCH_BYTECODE += 'nullptr, 0, '
		if block['unlinked']:
			PATCH_BYTECODE += f'{block['name']}_jumpData, {len(block['unlinked'])} '
		else:
			PATCH_BYTECODE += 'nullptr, 0 '
		PATCH_BYTECODE += f'}};\n'

	max_size = max(len(f) for f in used)
	PATCH_SKELETONS += f'// {current_file.split('/')[-1]}\n'
	PATCH_SKELETONS += f'std::array<PatchSkeleton, {len(locations)}> patchList = {{\n'
	for location in locations:
		offset = location['address'] - 0x400000
		decoder = iced_x86.Decoder(32, soku_data[offset:offset + location['size']], ip=location['address'])
		instr = decoder.decode()
		if location['name'] not in used:
			PATCH_BYTECODE += f"// Created from use at line {location['pos']}\n"
			PATCH_BYTECODE += f"static PatchByteCode {location['name']}_patchByteCode = {{ nullptr, 0, nullptr, 0 }}\n"
			used.add(location['name'])
			current_line = location['pos']
			emit_warning(f'{location['name']} was never declared')
		PATCH_SKELETONS += '\tPatchSkeleton{ '
		PATCH_SKELETONS += f'{location['name']}_patchByteCode, {" " * (max_size - len(location['name']))}'
		PATCH_SKELETONS += f'(void *)0x{location['address']:06X}, '
		PATCH_SKELETONS += f'{location['size']:2}'
		PATCH_SKELETONS += f'}}, // Declared line {location['pos']}. Patch on {instr:iXrsU}\n'
	PATCH_SKELETONS += '};\n'

	vtable = game_vtables[base]
	invalid = [
		get_function_bounds(soku_data, vtable[0], 0),
		get_function_bounds(soku_data, vtable[1], 0)
	]
	addresses = {
		'skeletonsForUpdate':                 get_function_bounds(soku_data, vtable[0], 10),
		'skeletonsForInitializeAction':       get_function_bounds(soku_data, vtable[0], 15),
		'skeletonsForInitialize':             get_function_bounds(soku_data, vtable[0], 17),
		'skeletonsForHandleInputs':           get_function_bounds(soku_data, vtable[0], 20),
		'skeletonsForVUnknown58':             get_function_bounds(soku_data, vtable[0], 22),
		'skeletonsForVUnknown5C':             get_function_bounds(soku_data, vtable[0], 23),
		'skeletonsForVUnknown60':             get_function_bounds(soku_data, vtable[0], 24),
		'skeletonsForObjectUpdate':           get_function_bounds(soku_data, vtable[1], 10),
		'skeletonsForObjectInitializeAction': get_function_bounds(soku_data, vtable[1], 15),
	}
	arrays = {
		'skeletonsForUpdate':                 [],
		'skeletonsForInitializeAction':       [],
		'skeletonsForInitialize':             [],
		'skeletonsForHandleInputs':           [],
		'skeletonsForVUnknown58':             [],
		'skeletonsForVUnknown5C':             [],
		'skeletonsForVUnknown60':             [],
		'skeletonsForObjectUpdate':           [],
		'skeletonsForObjectInitializeAction': [],
	}
	for index, location in enumerate(locations):
		current_line = location['pos']
		found = False
		for low, high in invalid:
			if low < location['address'] < high:
				found = False
				emit_error('Hooking destructors is not supported')
		if found:
			continue
		for key, (low, high) in addresses.items():
			if low < location['address'] < high:
				arrays[key].append(index)
				found = True
		if found:
			continue
		if args.debug:
			print(f'{hex(location['address'])}')
			for key, (low, high) in addresses.items():
				print(f'{key}: ({hex(low)}, {hex(high)})')
		emit_warning('Address is not in any character function, placing it in update hook pool')
		arrays['skeletonsForUpdate'].append(index)

	if has_error:
		exit(1)

	for key, indices in arrays.items():
		if not indices:
			PATCH_SKELETONS_INDICES += f"static std::vector<unsigned> {key};\n"
		elif len(indices) < 16:
			PATCH_SKELETONS_INDICES += f"static std::array<unsigned, {len(indices)}> {key} = {{ {", ".join(map(str, indices))} }};\n"
		else:
			PATCH_SKELETONS_INDICES += f"static std::array<unsigned, {len(indices)}> {key} = {{\n"
			for i in range(0, len(indices), 16):
				PATCH_SKELETONS_INDICES += f"\t{",".join(map(lambda d: f'{d: 4d}', indices[i:i + 16]))},\n"
			PATCH_SKELETONS_INDICES += "};\n"

	patches_header_template = (patches_header_template
			   .replace('{{CLASS_NAME}}', CLASS_NAME)
			   .replace('{{ClassName}}', ClassName)
			   .replace('{{BaseName}}', BaseName))
	patches_source_template = (patches_source_template
			   .replace('{{CLASS_NAME}}', CLASS_NAME)
			   .replace('{{ClassName}}', ClassName)
			   .replace('{{BaseName}}', BaseName)
			   .replace('{{PATCH_BYTECODE}}', PATCH_BYTECODE)
			   .replace('{{PATCH_SKELETONS}}', PATCH_SKELETONS)
			   .replace('{{PATCH_SKELETONS_INDICES}}', PATCH_SKELETONS_INDICES))
	header_template = (header_template
			   .replace('{{CLASS_NAME}}', CLASS_NAME)
			   .replace('{{ClassName}}', ClassName)
			   .replace('{{BaseName}}', BaseName))
	source_template = (source_template
			   .replace('{{CLASS_NAME}}', CLASS_NAME)
			   .replace('{{ClassName}}', ClassName)
			   .replace('{{BaseName}}', BaseName))
	entry_point_template = (entry_point_template
				.replace('{{MAJOR}}', MAJOR)
				.replace('{{MINOR}}', MINOR)
				.replace('{{ClassName}}', ClassName)
				.replace('{{BaseName}}', BaseName))
	cmake_template = (cmake_template
			  .replace('{{ClassName}}', ClassName))
	version_template = (version_template
			    .replace('{{MAJOR}}', MAJOR)
			    .replace('{{MINOR}}', MINOR)
			    .replace('{{ClassName}}', ClassName)
			    .replace('{{BaseName}}', BaseName))
	character_template = (character_template
			      .replace('{{class_name}}', class_name)
			      .replace('{{ClassName}}', ClassName)
			      .replace('{{ID}}', csv[0])
			      .replace('{{SHORT_NAME}}', csv[2])
			      .replace('{{FULL_NAME}}', csv[3])
			      .replace('{{SKILLS}}', ", ".join(csv[4].split(','))))

	if args.debug2 or args.output_path == '-':
		print(f" ---------- {ClassName}.hpp ---------- ")
		print(header_template, end="")
		print(f" ---------- {ClassName}.cpp ---------- ")
		print(source_template, end="")
		print(f" ---------- code_patches.hpp ---------- ")
		print(patches_header_template, end="")
		print(f" ---------- code_patches.cpp ---------- ")
		print(patches_source_template, end="")
		print(" ---------- entry_point.cpp ---------- ")
		print(entry_point_template, end="")
		print(" ---------- CMakeLists.txt ---------- ")
		print(cmake_template, end="")
		print(" ---------- version.rc ---------- ")
		print(version_template, end="")
		print(" ---------- character.json ---------- ")
		print(character_template, end="")

	if args.output_path != '-':
		with open(f'{args.output_path}/{ClassName}.hpp', "w") as fd:
			fd.write(header_template)
		with open(f'{args.output_path}/{ClassName}.cpp', "w") as fd:
			fd.write(source_template)
		with open(f'{args.output_path}/code_patches.hpp', "w") as fd:
			fd.write(patches_header_template)
		with open(f'{args.output_path}/code_patches.cpp', "w") as fd:
			fd.write(patches_source_template)
		with open(f'{args.output_path}/entry_point.cpp', "w") as fd:
			fd.write(entry_point_template)
		with open(f'{args.output_path}/CMakeLists.txt', "w") as fd:
			fd.write(cmake_template)
		with open(f'{args.output_path}/version.rc', "w") as fd:
			fd.write(version_template)
		with open(f'{args.output_path}/character.json', "w") as fd:
			fd.write(character_template)


if __name__ == '__main__':
	main()
