#ifndef CORE_H
#define CORE_H

#include <stdio.h>

#define PIPELINE_OFS 4

#define MEM_SIZE 1024
char MEM[MEM_SIZE*4];

unsigned int r[16];
unsigned int cpsr;

#define sl r[10]
#define fp r[11]
#define ip r[12]
#define sp r[13]
#define lr r[14]
#define pc r[15]

#define V 1<<28
#define C 1<<29
#define Z 1<<30
#define N 1<<31

#define CONDSHIFT 28
#define CONDMASK (0xF<<CONDSHIFT)

#define FUNSHIFT 26
#define FUNMASK (0x3<<FUNSHIFT)

#define OPSHIFT 21
#define OPMASK (0xF<<OPSHIFT)

#define ISHIFT 25
#define IMASK (1<<ISHIFT)

#define PSHIFT 24
#define PMASK (1<<PSHIFT)

#define USHIFT 23
#define UMASK (1<<USHIFT)

#define BSHIFT 22
#define BMASK (1<<BSHIFT)

#define WSHIFT 21
#define WMASK (1<<WSHIFT)

#define LOADSHIFT 20
#define LOADMASK (1<<LOADSHIFT)

#define LINKSHIFT 24
#define LINKMASK (1<<LINKSHIFT)

#define RNSHIFT 16
#define RNMASK (0xF<<RNSHIFT)
#define RDSHIFT 12
#define RDMASK (0xF<<RDSHIFT)
#define RSSHIFT 8
#define RSMASK (0xF<<RSSHIFT)

#define RMSHIFT 0
#define RMMASK (0xF<<RMSHIFT)

#define ROTSHIFT 8
#define ROTMASK (0xF<<ROTSHIFT)

#define IMMSHIFT 0
#define IMMMASK (0xFF<<IMMSHIFT)
#define IMMLDRSTRMASK (0xFFF<<IMMSHIFT)

#define SHIFTSHIFT 5
#define SHIFTMASK (0x3<<SHIFTSHIFT)

#define SHIFTREGSHIFT 4
#define SHIFTREGMASK (0x1<<SHIFTREGSHIFT)

#define SHIFTVALSHIFT 7
#define SHIFTVALMASK (0x1F<<SHIFTVALSHIFT)

#define BTASHIFT 2
#define BTAMASK (0xFFFFFF)

#define SWISHIFT 0
#define SWIMASK (0x00FFFFFF)

enum COND
{
	CND_EQ	= 0x0,
	CND_NE	= 0x1,
	CND_CS	= 0x2,
	CND_CC	= 0x3,
	CND_MI	= 0x4,
	CND_PL	= 0x5,
	CND_VS	= 0x6,
	CND_VC	= 0x7,
	CND_HI	= 0x8,
	CND_LS	= 0x9,
	CND_GE	= 0xA,
	CND_LT	= 0xB,
	CND_GT	= 0xC,
	CND_LE	= 0xD,
	CND_AL	= 0xE,
	CND_CNT = 0xF,
};
enum SHIFT
{
	SHFT_LSL = 0x0,
	SHFT_LSR = 0x1,
	SHFT_ASR = 0x2,
	SHFT_ROR = 0x3,
//	RRX = 0x??,
};
enum FUN
{
	FUN_ALU    = 0,
	FUN_LDRSTR = 1,
	FUN_LDMSTM = 8,
	FUN_B      = 2,
	FUN_SWI    = 3,
};
enum OPCODE
{
// ALU
	OP_MOV = 0xD,
	OP_MVN = 0xF,

	OP_ADD = 0x4,
	OP_ADC = 0x5,
	OP_SUB = 0x2,
	OP_SBC = 0x6,
	OP_RSB = 0x3,
	OP_RSC = 0x7,
	OP_AND = 0x0,
	OP_EOR = 0x1,
	OP_BIC = 0xE,
	OP_ORR = 0xC,

	OP_CMP = 0xA,
	OP_CMN = 0xB,
	OP_TST = 0x8,
	OP_TEQ = 0x9,

// MUL: prefixed with F
	OP_MUL = 0xF0,
	OP_MLA = 0xF1,

// LDRSTR
	OP_LDR = 0x10,
	OP_STR = 0x11,

// LDMSTRM
	OP_LDM = 0x12,
	OP_STM = 0x13,

// BRANCH
	OP_B   = 0x14,
// SWI
	OP_SWI = 0xFFF,

// DATA
	OP_DCD = 0xFFFF,
	OP_FIL = 0xFFFFF,

// VAR
	OP_END = 0xFFFFFFFF,
};
// ALU
// in2 unused, needed for jump table
unsigned long exec_mov(unsigned int in1, unsigned int in2);
unsigned long exec_mvn(unsigned int in1, unsigned int in2);

unsigned long exec_add(unsigned int in1, unsigned int in2);
unsigned long exec_adc(unsigned int in1, unsigned int in2);
unsigned long exec_sub(unsigned int in1, unsigned int in2);
unsigned long exec_sbc(unsigned int in1, unsigned int in2);
unsigned long exec_rsb(unsigned int in1, unsigned int in2);
unsigned long exec_rsc(unsigned int in1, unsigned int in2);
unsigned long exec_and(unsigned int in1, unsigned int in2);
unsigned long exec_eor(unsigned int in1, unsigned int in2);
unsigned long exec_bic(unsigned int in1, unsigned int in2);
unsigned long exec_orr(unsigned int in1, unsigned int in2);

unsigned long exec_cmp(unsigned int in1, unsigned int in2);
unsigned long exec_cmn(unsigned int in1, unsigned int in2);
unsigned long exec_tst(unsigned int in1, unsigned int in2);
unsigned long exec_teq(unsigned int in1, unsigned int in2);

// BRANCH
void exec_b(unsigned int in1);

int exec_cond(unsigned int opcode);
int exec_shifter(unsigned int in, unsigned int shiftval, unsigned int shift);
void exec_flags(unsigned int opcode, unsigned long in);
void exec_alu(unsigned int opcode);
void exec_mul(unsigned int opcode);
void exec_ldrstr(unsigned int opcode);
void exec_ldmstm(unsigned int opcode);
void exec_branch(unsigned int opcode);
void exec_swi(unsigned int opcode);

void exec(unsigned int opcode);

#endif //CORE_H
