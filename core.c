#include "core.h"

unsigned long (*exec_tbl[16])(unsigned int, unsigned int) = {
	exec_and, 
	exec_eor,
	exec_sub,
	exec_rsb,
	exec_add,
	exec_adc,
	exec_sbc,
	exec_rsc,
	exec_tst,
	exec_teq,
	exec_cmp,
	exec_cmn,
	exec_orr,
	exec_mov,
	exec_bic,
	exec_mvn,
	};

unsigned long exec_mov(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in2;
}
unsigned long exec_mvn(unsigned int in1, unsigned int in2)
{
	return (unsigned long)~in1;
}

unsigned long exec_add(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1+in2;
}
unsigned long exec_adc(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1+in2+((cpsr&C)>>29);
}
unsigned long exec_sub(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1-in2;
}
unsigned long exec_sbc(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1-in2+((cpsr&C)>>29);
}
unsigned long exec_rsb(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in2-in1;
}
unsigned long exec_rsc(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in2-in1+((cpsr&C)>>29);
}
unsigned long exec_and(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1&in2;
}
unsigned long exec_eor(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1^in2;
}
unsigned long exec_bic(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1&(!in2);
}
unsigned long exec_orr(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1|in2;
}

unsigned long exec_cmp(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1-in2;
}
unsigned long exec_cmn(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1+in2;
}
unsigned long exec_tst(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1&in2;
}
unsigned long exec_teq(unsigned int in1, unsigned int in2)
{
	return (unsigned long)in1^in2;
}

// BRANCH
void exec_b(unsigned int in1)
{
	pc = in1;
}

// EXEC
int exec_cond(unsigned int opcode)
{
	switch(opcode>>CONDSHIFT)
	{
		case CND_EQ:
            if(cpsr & Z)
                return 1;
            break;
        case CND_NE:
            if(!(cpsr & Z))
                return 1;
            break;
        case CND_CS:
            if(cpsr & C)
                return 1;
            break;
        case CND_CC:
            if(!(cpsr & C))
                return 1;
            break;
        case CND_MI:
            if(cpsr & N)
                return 1;
            break;
        case CND_PL:
            if(!(cpsr & N))
                return 1;
            break;
        case CND_VS:
            if(cpsr & V)
                return 1;
            break;
        case CND_VC:
            if(!(cpsr & V))
                return 1;
            break;
        case CND_HI:
            if((cpsr & C) && !(cpsr & Z))
                return 1;
            break;
        case CND_LS:
            if(!(cpsr & C) && (cpsr & Z))
                return 1;
            break;
        case CND_GE:
            if((cpsr & N) == (cpsr & V))
                return 1;
            break;
        case CND_LT:
            if((cpsr & N) != (cpsr & V))
                return 1;
            break;
        case CND_GT:
            if(!(cpsr & Z) && (cpsr & N) == (cpsr & V))
                return 1;
            break;
        case CND_LE:
            if((cpsr & Z) && (cpsr & N) != (cpsr & V))
                return 1;
            break;
        case CND_AL:
			return 1;
			break;
	}
	return 0;
}
int exec_shifter(unsigned int in, unsigned int shift, unsigned int shiftval)
{
    switch(shift)
    {
        case SHFT_LSL:
            return in<<shiftval;
        case SHFT_LSR:
            return in>>shiftval;
            break;
        case SHFT_ASR:
        {
            int logical = (((unsigned int)-1) >> 1) > 0;
            unsigned int fixu = -(logical & (in< 0));
            unsigned int fix = *(unsigned int *)&fixu;
            return (in>> shiftval) | (fix ^ (fix >> shiftval));
        }
        case SHFT_ROR:
            // TODO: ror
            break;
    }
    return in;
}
void exec_flags(unsigned int opcode, unsigned long in)
{
    if(!(opcode&(0x1<<20)))
		return;

	cpsr = 0;
    if((signed long)in < 0)
        cpsr |= N;
    if(in == 0)
        cpsr |= Z;
    if(in > (0x1 << 31))
        cpsr |= C;
    if(in > (0x1 << 30))
        cpsr |= V;
}

void exec_alu(unsigned int opcode)
{
	int i = (opcode&OPMASK)>>OPSHIFT;
	if(i<0 || i>0xF)
	{
		printf("wrong opcode!\n");
		return;
	}
	unsigned int in1 = -1;
	unsigned int in2 = -1;
	int rn = (opcode&RNMASK)>>RNSHIFT;
	int rd = (opcode&RDMASK)>>RDSHIFT;

	in1 = r[rn];
	if((opcode&IMASK)>>ISHIFT == 1)
	{
		int imm = (opcode&IMMMASK)>>IMMSHIFT;
// TODO: implement rot
//		int rot = (opcode&ROTMASK)>>ROTSHIFT;
		in2 = imm;
	}
	else
	{
		int shift = (opcode&SHIFTMASK)>>SHIFTSHIFT;
		int rm = (opcode&RMMASK)>>RMSHIFT;
		if((opcode&SHIFTREGMASK)>>SHIFTREGSHIFT== 1)
		{
			int rs = (opcode&RSMASK)>>RSSHIFT;
			in2 = exec_shifter(r[rm], shift, r[rs]);
		}
		else
		{
			int shiftval = (opcode&SHIFTVALMASK)>>SHIFTVALSHIFT;
			in2 = exec_shifter(r[rm], shift, shiftval);
		}
	}
	unsigned long out = exec_tbl[i](in1, in2);
// tst, teq, cmp, cmn:
	if((opcode&OPMASK)>>OPSHIFT != OP_TST &&
	   (opcode&OPMASK)>>OPSHIFT != OP_TEQ &&
	   (opcode&OPMASK)>>OPSHIFT != OP_CMP &&
	   (opcode&OPMASK)>>OPSHIFT != OP_CMN)
	{
		r[rd] = (unsigned int)out;
	}
	exec_flags(opcode, out);

}
void exec_mul(unsigned int opcode)
{
	int rn = (opcode&RNMASK)>>RNSHIFT;
	int rd = (opcode&RDMASK)>>RDSHIFT;
	int rs = (opcode&RSMASK)>>RSSHIFT;
	int rm = (opcode&RMMASK)>>RMSHIFT;
	int out;

	if((opcode&OPMASK)>>OPSHIFT == (OP_MLA&0xF))
		out = r[rm]*r[rs]+r[rd];
	else if((opcode&OPMASK)>>OPSHIFT == (OP_MUL&0xF))
		out = r[rm]*r[rs];
	r[rn] = out;
	exec_flags(opcode, out);
}
void exec_ldrstr(unsigned int opcode)
{
	unsigned int in1 = -1;
	unsigned int in2 = -1;
	int rn = (opcode&RNMASK)>>RNSHIFT;
	int rd = (opcode&RDMASK)>>RDSHIFT;

	in1 = r[rn];
	if((opcode&IMASK)>>ISHIFT == 0)
	{
		int imm = (opcode&IMMLDRSTRMASK)>>IMMSHIFT;
		in2 = imm;
	}
	else
	{
		int shift = (opcode&SHIFTMASK)>>SHIFTSHIFT;
		int rm = (opcode&RMMASK)>>RMSHIFT;
		if((opcode&SHIFTREGMASK)>>SHIFTREGSHIFT== 0)
		{
			int shiftval = (opcode&SHIFTVALMASK)>>SHIFTVALSHIFT;
			in2 = exec_shifter(r[rm], shift, shiftval);
		}
	}

	if((opcode&UMASK)>>USHIFT == 0)
		in2 = -in2;

// TODO: b flag

	if((opcode&PMASK)>>PSHIFT == 1)
		in1+=in2;


	if((opcode&LOADMASK)>>LOADSHIFT == 1)
	{
		if((opcode&BMASK)>>BSHIFT == 1)
			r[rd] = MEM[in1];
		else
			memcpy(&r[rd], &MEM[in1], sizeof(unsigned int));
	}
	else
	{
		if((opcode&BMASK)>>BSHIFT == 1)
			MEM[in1] = (char)r[rd];
			// bswap
		else
		{
			memcpy(&MEM[in1], &r[rd], sizeof(unsigned int));
		}
	}

	if(((opcode&PMASK)>>PSHIFT == 0) ||
	   (((opcode&PMASK)>>PSHIFT == 1) && ((opcode&WMASK)>>WSHIFT == 1)))
		r[rn] += in2;
}
void exec_ldmstm(unsigned int opcode)
{
	int rn = (opcode&RNMASK)>>RNSHIFT;
// PU WL
	int adr = r[rn];
	int p = (opcode&PMASK)>>PSHIFT?0:1;
	int u = (opcode&UMASK)>>USHIFT;
	int w = (opcode&WMASK)>>WSHIFT;
	int l = (opcode&LOADMASK)>>LOADSHIFT;

	int istart = u?0:15;
	for(int i=istart;u?i<15:i>0;u?i++:i--)
	{
		if(opcode&(1<<i))
		{
			if(!p)
			{
				if(u)
					adr+=4;
				else
					adr-=4;
			}
			if(l)
				memcpy(&r[i], &MEM[adr], sizeof(unsigned int));
			else
				memcpy(&MEM[adr], &r[i], sizeof(unsigned int));
			if(p)
			{
				if(u)
					adr+=4;
				else
					adr-=4;
			}
		}
	}
	if(w)
		r[rn] = adr;
}
void exec_branch(unsigned int opcode)
{
	if((opcode&LINKMASK)>>LINKSHIFT == 1)
		lr = pc;

	int bta = (opcode&BTAMASK);
	bta |= (bta&800000)?0xFF000000:0;
	bta = bta<<BTASHIFT;
	pc+=bta;
}

void exec_swi(unsigned int opcode)
{
	int swinr = opcode&SWIMASK;
	if(swinr >= SWICNT)
	{
		printf("SWI %i NIMP\n", swinr);
		return;
	}
	else
		swi_tbl[swinr]();
}

void exec(unsigned int opcode)
{
	pc += PIPELINE_OFS;
	if(!exec_cond(opcode))
		return;
	switch((opcode&FUNMASK)>>FUNSHIFT)
	{
		case FUN_ALU:
			if((opcode&0xF0) == 0x90)
				exec_mul(opcode);
			else
				exec_alu(opcode);
			break;
		case FUN_LDRSTR:
			exec_ldrstr(opcode);
			break;
		case FUN_B:
			if((opcode&(0x7<<(FUNSHIFT-1))) == FUN_LDMSTM<<(FUNSHIFT-2))
				exec_ldmstm(opcode);
			else
				exec_branch(opcode);
			break;
		case FUN_SWI:
			exec_swi(opcode);
			break;
		default:
			printf("NOFUN!\n");
			break;
	}
}
