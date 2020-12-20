#include <string.h>
#include <stdlib.h>

op_instr INTERMEDIATE[1024];
unsigned int INTERMEDIATE_CNT = 0;
label LABEL[1024];
unsigned int LABEL_CNT = 0;

mnem_data mnem_op_tbl[] = {
// ALU
		{OP_MOV, "mov"},
		{OP_MVN, "mvn"},
		{OP_ADD, "add"},
		{OP_ADC, "adc"},
		{OP_SUB, "sub"},
		{OP_SBC, "sbc"},
		{OP_RSB, "rsb"},
		{OP_RSC, "rsc"},
		{OP_AND, "and"},
		{OP_EOR, "eor"},
		{OP_BIC, "bic"},
		{OP_ORR, "orr"},
		{OP_CMP, "cmp"},
		{OP_CMN, "cmn"},
		{OP_TST, "tst"},
		{OP_TEQ, "teq"},
// MUL
		{OP_MUL, "mul"},
		{OP_MLA, "mla"},
// LDRSTR
		{OP_LDRB, "ldrb"},
		{OP_STRB, "strb"},
		{OP_LDR, "ldr"},
		{OP_STR, "str"},
// LDMSTM
		{OP_LDM, "ldm"},
		{OP_STM, "stm"},
// BRANCH
		{OP_B, 	 "b"},
// data
		{OP_DCD,  "dcd"},
		{OP_DCB,  "dcb"},
		{OP_FIL,  "fill"},
		{OP_SWI,  "swi"},
// pseudo
		{OP_ADR,  "adr"},
// VAR
		{OP_END,  "END"},
		{0,  	  "DONE"},
	};
mnem_data mnem_cnd_tbl[] = {
		{CND_EQ,  "eq"},
		{CND_NE,  "ne"},
		{CND_CS,  "cs"},
		{CND_CC,  "cc"},
		{CND_MI,  "mi"},
		{CND_PL,  "pl"},
		{CND_VS,  "vs"},
		{CND_VC,  "vc"},
		{CND_HI,  "hi"},
		{CND_LS,  "ls"},
		{CND_GE,  "ge"},
		{CND_LT,  "lt"},
		{CND_GT,  "gt"},
		{CND_LE,  "le"},
		{CND_AL,  "al"},
		{0,  	  "DONE"},
	};
mnem_data mnem_shft_tbl[] = {
		{SHFT_LSL,  "lsl"},
		{SHFT_LSR,  "lsr"},
		{SHFT_ASR,  "asr"},
		{SHFT_ROR,  "ror"},
		{0xFF,  	"DONE"},
	};
mnem_data mnem_ldmstm_tbl[] = {
		{0,  "ia"},
		{1,  "ib"},
		{2,  "da"},
		{3,  "db"},
		{0,  "DONE"},
	};

void print_intermediate()
{
	printf("LABELS:\n");
	for(int i=0;i<LABEL_CNT;i++)
	{
		printf("\tNAME: %s\tadr: %i\n", LABEL[i].name, LABEL[i].adr);
	}
	printf("INTERMEDIATES:\n");
	for(int i=0;i<INTERMEDIATE_CNT;i++)
	{
	printf("%i: ", i*4);
		printf("OP: 0x%.2X COND: 0x%X", INTERMEDIATE[i].opcode, INTERMEDIATE[i].cond);
		for(int j=0;j<INTERMEDIATE[i].argcnt;j++)
		{
			switch( INTERMEDIATE[i].argtype[j])
			{
				case ARG_REG:
					printf(" REG:");
					break;
				case ARG_IMM:
					printf(" IMM:");
					break;
				case ARG_LIT:
					printf(" LIT:");
					break;
				case ARG_LBL:
					printf(" LBL:");
					break;
				case ARG_SHIFT:
					printf(" SHIFT:");
					break;
				case ARG_ADRMODE:
					printf(" ADRMODE:");
					break;
				default:
					printf(" ERR:");
					break;
			}
			printf(" %s", INTERMEDIATE[i].arg[j]);
		}
		printf("\n");

	}
}

mnem_data parseline_op(char *string)
{
	int i = 0;
	while(strncmp(mnem_op_tbl[i].str, "DONE",3) != 0)
	{
		if(strncmp(mnem_op_tbl[i].str, string, strlen(mnem_op_tbl[i].str)) == 0)
		{
			return mnem_op_tbl[i];
		}
		i++;
	}
	return (mnem_data){0x0, "ERR"};
}
op_instr parseline_cnd(mnem_data mnem, char *string)
{
	op_instr out = {0};
	out.opcode = mnem.opcode;
	out.cond = CND_AL;
	out.flag = 0;
	out.argcnt = 0;

	char *cnd = string+strlen(mnem.str);
	if(cnd[0] == 's')
	{
		cnd++;
		out.flag = 1;
	}
	int i = 0;
	while(strncmp(mnem_ldmstm_tbl[i].str, "DONE",3) != 0)
	{
		if(strncmp(mnem_ldmstm_tbl[i].str, cnd, 2) == 0)
		{
			out.flag = mnem_ldmstm_tbl[i].opcode;
		}
		i++;
	}
	if(strlen(cnd) == 0)
		return out;
	i = 0;
	while(strncmp(mnem_cnd_tbl[i].str, "DONE",3) != 0)
	{
		if(strncmp(mnem_cnd_tbl[i].str, cnd, 2) == 0)
		{
			out.cond = mnem_cnd_tbl[i].opcode;
		}
		i++;
	}
	if(out.opcode == OP_B)
	{
		if(out.cond==CND_LS || out.cond==CND_LE || out.cond==CND_LT)
		{
			if(cnd[0] == 'l' && cnd[1] == 'l')
				out.flag = 1;
		}
		else
			if(cnd[0] == 'l')
				out.flag = 1;
	}
	return out;
}
op_instr parse_reg(op_instr in, char *string)
{
	if(string[strlen(string)-1] == ']')
		string[strlen(string)-1] = '\0';
	if(strncmp(string, "sl", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "10");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	else if(strncmp(string, "fp", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "11");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	else if(strncmp(string, "ip", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "12");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	else if(strncmp(string, "sp", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "13");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	else if(strncmp(string, "lr", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "14");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	else if(strncmp(string, "pc", 2) == 0)
	{
		strcpy(in.arg[in.argcnt], "15");
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}

	else if(string[0] == 'r')
	{
		strcpy(in.arg[in.argcnt], string+1);
		in.argtype[in.argcnt] = ARG_REG;
		in.argcnt++;
	}
	return in;
}
op_instr parse_imm(op_instr in, char *string)
{
	if(string[0] == '#')
	{
		strcpy(in.arg[in.argcnt], string+1);
		in.argtype[in.argcnt] = ARG_IMM;
		in.argcnt++;
	}
	return in;
}
op_instr parse_lbl(op_instr in, char *string)
{
	if(string[0] == '=')
	{
		strcpy(in.arg[in.argcnt], string+1);
		in.argtype[in.argcnt] = ARG_LBL;
		in.argcnt++;
	}
	return in;
}
op_instr parse_shft(op_instr in, char *string)
{
	int i = 0;
	while(strncmp(mnem_shft_tbl[i].str, "DONE",3) != 0)
	{
		if(strncmp(mnem_shft_tbl[i].str, string, 3) == 0)
		{
			strcpy(in.arg[in.argcnt], string);
			in.argtype[in.argcnt] = ARG_SHIFT;
			in.argcnt++;
		}
		i++;
	}
	return in;
}
op_instr parse_alu(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		in = parse_reg(in, pch);
		in = parse_imm(in, pch);
		in = parse_shft(in, pch);
		in = parse_lbl(in, pch);
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_mul(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		in = parse_reg(in, pch);
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_ldrstr(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		int foundbracket = 0;
		int foundexclam = 0;
		if(pch[0] == '[')
		{
			
			strcpy(in.arg[in.argcnt], "[");
			in.argtype[in.argcnt] = ARG_ADRMODE;
			in.argcnt++;
			pch++;
		}
		if(pch[strlen(pch)-1] == '!')
		{
			foundexclam = 1;
			pch[strlen(pch)-1] = '\0';
		}
		if(pch[strlen(pch)-1] == ']' || pch[strlen(pch)-2] == ']')
		{
			pch[strlen(pch)-(1)] = '\0';
			foundbracket = 1;
		}

		in = parse_reg(in, pch);
		in = parse_imm(in, pch);
		in = parse_lbl(in, pch);
		in = parse_shft(in, pch);

		if(foundbracket)
		{
			strcpy(in.arg[in.argcnt], "]");
			in.argtype[in.argcnt] = ARG_ADRMODE;
			in.argcnt++;
		}
		if(foundexclam)
		{
			strcpy(in.arg[in.argcnt], "!");
			in.argtype[in.argcnt] = ARG_ADRMODE;
			in.argcnt++;
		}
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_b(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		strcpy(in.arg[in.argcnt], string);
		in.argtype[in.argcnt] = ARG_LBL;
		in.argcnt++;
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_dcd(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		if(pch[strlen(pch)-1] == ',')
			pch[strlen(pch)-1] = '\0';

		strcpy(in.arg[0], pch);
		in.argtype[0] = ARG_LIT;
		in.argcnt = 1;

		INTERMEDIATE[INTERMEDIATE_CNT] = in;
		INTERMEDIATE_CNT++;
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_dcb(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	int cnt = 0;
	int out = 0;
	int word = 0;
	in.opcode = OP_DCD;
	in.cond = CND_AL;
	in.flag = 0;
	while(pch != NULL)
	{
		if(pch[strlen(pch)-1] == ',')
			pch[strlen(pch)-1] = '\0';

		if(sscanf(pch, "%i", &out) != 1)
			printf("ERR: can't decode dcdb\n");
		else
		{
			word |= (out<<8*(cnt)); // endianness
			cnt++;
			if(cnt == 4)
			{
				sprintf(in.arg[0], "%i", word);
				in.argtype[0] = ARG_LIT;
				in.argcnt = 1;

				INTERMEDIATE[INTERMEDIATE_CNT] = in;
				INTERMEDIATE_CNT++;

				cnt = 0;
				word = 0;
			}
		}
		pch = strtok(NULL," \t\n");
	}
// might not be multiple of 4
	if(cnt !=0)
	{
		sprintf(in.arg[0], "%i", word);
		in.argtype[0] = ARG_LIT;
		in.argcnt = 1;

		INTERMEDIATE[INTERMEDIATE_CNT] = in;
		INTERMEDIATE_CNT++;
	}
	return in;
}
op_instr parse_fill(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	int cnt = atoi(pch);
	printf("COUNT: %i\n", cnt);
	for(int i=0;i<cnt;i++)
	{
		strcpy(in.arg[0], pch);
		in.argtype[0] = ARG_LIT;
		in.argcnt = 1;

		INTERMEDIATE[INTERMEDIATE_CNT] = in;
		INTERMEDIATE_CNT++;
	}
	return in;
}
op_instr parse_ldmstm(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	while(pch != NULL)
	{
		if(pch[0] == '{')
            pch++;
        if(pch[strlen(pch)-1] == '}')
            pch[strlen(pch)-1] = '\0';

        int reg1;
        int reg2;
        if(sscanf(pch, " r%i-r%i", &reg1, &reg2) == 2)
        {
            for(int i=reg1;i<=reg2;i++)
			{
				sprintf(in.arg[in.argcnt], "%i", i);
				in.argtype[in.argcnt] = ARG_REG;
				in.argcnt++;
			}
        }
        else 
		{
			in = parse_reg(in, pch);
		}
        if(pch[strlen(pch)-2] == '!')
		{
			in.argtype[in.argcnt] = ARG_ADRMODE;
			in.argcnt++;
		}
		pch = strtok(NULL," \t\n");
	}
	return in;
}
op_instr parse_swi(op_instr in, char *string)
{
	char *pch = strtok(string," \t\n");
	int i=0;
	while(strncmp(mnem_swi_tbl[i].str, "DONE",4) != 0)
	{
		if(strcmp(mnem_swi_tbl[i].str, pch) == 0)
		{
			sprintf(in.arg[in.argcnt], "%i", mnem_swi_tbl[i].opcode);
			in.argtype[in.argcnt] = ARG_IMM;
			in.argcnt++;
		}
		i++;
	}
	if(in.argcnt == 0)
		printf("SWI not found!: %s\n", pch);
	INTERMEDIATE[INTERMEDIATE_CNT] = in;
	INTERMEDIATE_CNT++;
	return in;
}
op_instr parse_adr(op_instr in, char *string)
{
// ldr rX, [pc, 8]
// add pc, #4
// dcd IMM

	char *pch = strtok(string," \t\n");
	int i=0;
	while(pch != NULL)
	{
		in = parse_reg(in, pch);
		in = parse_imm(in, pch);
		in = parse_lbl(in, pch);
		i++;
		pch = strtok(NULL," \t\n");
	}
	op_instr out = {0};
// ldr rX, [pc, #8]
	out.opcode = OP_LDR;
	out.cond = in.cond;
	out.flag = in.flag;
	out.argcnt = 5;
	out.argtype[0] = ARG_REG;
	strcpy(out.arg[0], in.arg[0]);
	out.argtype[1] = ARG_ADRMODE;
	strcpy(out.arg[1], "[");
	out.argtype[2] = ARG_REG;
	strcpy(out.arg[2], "15");
	out.argtype[3] = ARG_IMM;
	strcpy(out.arg[3], "4");
	out.argtype[4] = ARG_ADRMODE;
	strcpy(out.arg[4], "]");
	INTERMEDIATE[INTERMEDIATE_CNT] = out;
	INTERMEDIATE_CNT++;
// add pc, #4
	out.opcode = OP_ADD;
	out.argcnt = 3;
	out.argtype[0] = ARG_REG;
	strcpy(out.arg[0], "15");
	out.argtype[1] = ARG_REG;
	strcpy(out.arg[1], "15");
	out.argtype[2] = ARG_IMM;
	strcpy(out.arg[2], "4");
	INTERMEDIATE[INTERMEDIATE_CNT] = out;
	INTERMEDIATE_CNT++;
// dcd IMM
	out.opcode = OP_DCD;
	out.cond = CND_AL;
	out.flag = 0;
	out.argcnt = 1;
	out.argtype[0] = in.argtype[1];
	strcpy(out.arg[0], in.arg[1]);
	INTERMEDIATE[INTERMEDIATE_CNT] = out;
	INTERMEDIATE_CNT++;

	return in;
}
int parse_file(char *filename)
{
	FILE* f = fopen(filename, "r");
	char line[128];
	while(fgets(line, 128, f) != NULL)
	{
		char lineo[128];
		strcpy(lineo, line);
		char *comment = strchr (line, ';');
		if(comment)
		{
			for(int i=0;i<strlen(comment);i++)
			{
				comment[i] = '\0';
			}
		}
		char *pch = strtok(line," \t\n");
		while(pch != NULL)
		{
			if(pch[0] == ';')
				break;
			mnem_data opmnem = parseline_op(pch);
			if(strcmp(opmnem.str, "ERR") !=0)
			{
				op_instr instr = parseline_cnd(opmnem, pch);
				switch(instr.opcode)
				{
					case OP_MOV:
					case OP_MVN:
					case OP_ADD:
					case OP_ADC:
					case OP_SUB:
					case OP_SBC:
					case OP_RSB:
					case OP_RSC:
					case OP_AND:
					case OP_EOR:
					case OP_BIC:
					case OP_ORR:
					case OP_CMP:
					case OP_CMN:
					case OP_TST:
					case OP_TEQ:
						instr = parse_alu(instr, pch+strlen(pch)+1);
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;
					case OP_MUL:
					case OP_MLA:
						instr = parse_mul(instr, pch+strlen(pch)+1);
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;
					case OP_LDR:
					case OP_STR:
					case OP_STRB:
					case OP_LDRB:
						instr = parse_ldrstr(instr, pch+strlen(pch)+1);
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;
					case OP_LDM:
					case OP_STM:
						instr = parse_ldmstm(instr, pch+strlen(pch)+1);
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;
					case OP_B:
						instr = parse_b(instr, pch+strlen(pch)+1);
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;

					// memory
					// DCD
					// DCW
					// DCB
					case OP_DCD:
						instr = parse_dcd(instr, pch+strlen(pch)+1);
						break;
					case OP_DCB:
						instr = parse_dcb(instr, pch+strlen(pch)+1);
						break;
					case OP_FIL:
						instr = parse_fill(instr, pch+strlen(pch)+1);
						break;
					case OP_SWI:
						instr = parse_swi(instr, pch+strlen(pch)+1);
						break;
					case OP_ADR:
						instr = parse_adr(instr, pch+strlen(pch)+1);
						break;
					case OP_END:
						INTERMEDIATE[INTERMEDIATE_CNT] = instr;
						INTERMEDIATE_CNT++;
						break;
				}
				
			}
			else
			{
				LABEL[LABEL_CNT].adr = INTERMEDIATE_CNT;
				strcpy(LABEL[LABEL_CNT].name, pch);
				LABEL_CNT++;
			}
			pch = strtok(NULL," \t\n");
		}
	}
	fclose(f);
	return 0;
}
unsigned int BLDOP_ALU_I(int cond, int op, int s, int rn, int rd, int rot, int imm)
{
	return cond<<28 | 1<<25 | op<<21 | s<<20 | rn<<16 | rd<<12 | rot<<8 | imm;
}
unsigned int BLDOP_ALU_RSR(int cond, int op, int s, int rn, int rd, int rs, int shift, int rm)
{
	return cond<<28 | 0<<25 | op<<21 | s<<20 | rn<<16 | rd<<12 | rm<<0 | shift<<5 | 1<<4 | rs<<8;
}
unsigned int BLDOP_ALU_RSI(int cond, int op, int s, int rn, int rd, int imm, int shift, int rm)
{
	return cond<<28 | 0<<25 | op<<21 | s<<20 | rn<<16 | rd<<12 | rm<<0 | shift<<5 | 0<<4 | imm<<7;
}
unsigned int BLDOP_MUL(int cond, int op, int s, int rn, int rd, int rs, int rm)
{
	return cond<<28 | 0<<25 | op<<21 | s<<20 | rn<<12 | rd<<16 | rs<<8 | 9<<4 | rm<<0;
}
unsigned int BLDOP_LDRSTR_I(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm)
{
	return cond<<28 | 2<<25 | p<<24 | u<<23 | b<<22 | w<<21 | l<<20 | rn<<16 | rd<<12 | imm;
}
unsigned int BLDOP_LDRSTR_RSR(int cond, int p, int u, int b, int w, int l, int rn, int rd, int rs, int shift, int rm)
{
	return cond<<28 | 3<<25 | p<<24 | u<<23 | b<<22 | w<<21 | l<<20 | rn<<16 | rd<<12 | rm<<0 | shift<<5 | 1<<4 | rs<<8;
}
unsigned int BLDOP_LDRSTR_RSI(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm , int shift, int rm)
{
	return cond<<28 | 3<<25 | p<<24 | u<<23 | b<<22 | w<<21 | l<<20 | rn<<16 | rd<<12 | rm<<0 | shift <<5 | 0<<4 | imm<<7;
}
unsigned int BLDOP_LDMSTM(int cond, int p, int u, int b, int w, int l, int rn, int regmap)
{
	return cond<<28 | 4<<25 | p<<24 | u<<23 | b<<22 | w<<21 | l<<20 | rn<<16 | regmap;
}
unsigned int BLDOP_B(int cond, int l, int bta)
{
	return cond<<28 | 5<<25 | l<<24 | bta;
}
unsigned int BLDOP_SWI(int cond, int imm)
{
	return cond<<28 | 0xF<<24 | imm;
}
int str2shift(char *string)
{
	int i=0;
	while(strncmp(mnem_shft_tbl[i].str, "DONE",3) != 0)
	{
		if(strncmp(mnem_shft_tbl[i].str, string, 3) == 0)
		{
			return mnem_shft_tbl[i].opcode;
		}
		i++;
	}
	return -1;
}
int lbl2adr(char *string)
{
	for(int i=0;i<LABEL_CNT;i++)
	{
		if(strcmp(LABEL[i].name, string) == 0)
			return LABEL[i].adr*4;
	}
	return -1;
}
unsigned int parse_inter_mov(op_instr in)
{
// mov rX, #IMM
	if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_IMM && in.argcnt == 2)
	{
		int rd;
		int imm;
		if(sscanf(in.arg[0], "%i", &rd) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, 0, rd, 0, imm);
	}
// mov rX, =lbl
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_LBL && in.argcnt == 2)
	{
		int rd;
		int imm;
		if(sscanf(in.arg[0], "%i", &rd) != 1)
			printf("ERR: %s\n", __func__);
		imm = lbl2adr(in.arg[1]);
		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, 0, rd, 0, imm);
	}
// mov rX, rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argcnt == 2)
	{	
		int rd;
		int rm;
		if(sscanf(in.arg[0], "%i", &rd) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, 0, rd, 0, 0, rm);
	}
// mov rX, rX, LSL #IMM
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_IMM && in.argcnt == 4)
	{
		int rd;
		int imm;
		int shift;
		int rm;
		if(sscanf(in.arg[0], "%i", &rd) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[2]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[3], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, 0, rd, imm, shift, rm);
	}
// mov rX, rX, LSL rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_REG && in.argcnt == 4)
	{
		int rd;
		int rs;
		int shift;
		int rm;
		if(sscanf(in.arg[0], "%i", &rd) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[2]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[3], "%i", &rs) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSR(in.cond, in.opcode, in.flag, 0, rd, rs, shift, rm);
	}

	return 0;
}
unsigned int parse_inter_cmp(op_instr in)
{
// cmp rX, #IMM
	if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_IMM && in.argcnt == 2)
	{
		int rn;
		int imm;
		if(sscanf(in.arg[0], "%i", &rn) != 1)
			printf("ERR1 0: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &imm) != 1)
			printf("ERR1 1: %s\n", __func__);
		return BLDOP_ALU_I(in.cond, in.opcode, 1, rn, 0, 0, imm);
	}
// cmp rX, rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argcnt == 2)
	{	
		int rn;
		int rm;
		if(sscanf(in.arg[0], "%i", &rn) != 1)
			printf("ERR2 0: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR2 1: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, 1, rn, 0, 0, 0, rm);
	}
// cmp rX, rX, LSL #IMM
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_IMM && in.argcnt == 4)
	{
		int rn;
		int imm;
		int shift;
		int rm;
		if(sscanf(in.arg[0], "%i", &rn) != 1)
			printf("ERR3 0: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR3 1: %s\n", __func__);
		shift = str2shift(in.arg[2]);
		if(shift<0)
			printf("ERR3 2: %s \"%s\"\n", __func__, in.arg[2]);
		if(sscanf(in.arg[3], "%i", &imm) != 1)
			printf("ERR3 3: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, 1, rn, 0, imm, shift, rm);
	}
// cmp rX, rX, LSL rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_REG && in.argcnt == 4)
	{
		int rn;
		int rs;
		int shift;
		int rm;
		if(sscanf(in.arg[0], "%i", &rn) != 1)
			printf("ERR4 0: %s\n", __func__);
		if(sscanf(in.arg[1], "%i", &rm) != 1)
			printf("ERR4 1: %s\n", __func__);
		shift = str2shift(in.arg[2]);
		if(shift<0)
			printf("ERR4 2: %s \"%s\"\n", __func__, in.arg[2]);
		if(sscanf(in.arg[3], "%i", &rs) != 1)
			printf("ERR4 3: %s\n", __func__);
		return BLDOP_ALU_RSR(in.cond, in.opcode, 1, rn, 0, rs, shift, rm);
	}
	return 0;
}
unsigned int parse_inter_mul(op_instr in)
{
	int rd;
	int rs;
	int rm;
	int rn = 0;

	if(sscanf(in.arg[0], "%i", &rd) != 1)
		printf("ERR: %s\n", __func__);
	if(sscanf(in.arg[1], "%i", &rm) != 1)
		printf("ERR: %s\n", __func__);
	if(sscanf(in.arg[2], "%i", &rs) != 1)
		printf("ERR: %s\n", __func__);
	if(in.opcode == OP_MLA)
	{
		if(sscanf(in.arg[3], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
	}
	in.opcode = in.opcode&0xF;
	return BLDOP_MUL(in.cond, in.opcode, in.flag, rn, rd, rs, rm);
}
unsigned int parse_inter_alu(op_instr in)
{
// add rX, rX, #IMM
	int rd;
	if(sscanf(in.arg[0], "%i", &rd) != 1)
		printf("ERR: %s\n", __func__);
	if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_IMM && in.argcnt == 3)
	{
		int rn;
		int imm;
		if(sscanf(in.arg[1], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[2], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, rn, rd, 0, imm);
	}
// add rX, rX, rX
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argcnt == 3)
	{	
		int rn;
		int rm;
		if(sscanf(in.arg[1], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[2], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, rn, rd, 0, 0, rm);
	}
// add rX, rX, rX LSL #IMM
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_SHIFT && in.argtype[4]==ARG_IMM && in.argcnt == 5)
	{
		int rn;
		int rm;
		int shift;
		int imm;
		if(sscanf(in.arg[1], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[2], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[3]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[4], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, rn, rd, imm, shift, rm);
	}
// add rX, rX, rX LSL rX
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_SHIFT && in.argtype[4]==ARG_REG&& in.argcnt == 5)
	{
		int rn;
		int rm;
		int shift;
		int rs;
		if(sscanf(in.arg[1], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[2], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[3]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[4], "%i", &rs) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_ALU_RSR(in.cond, in.opcode, in.flag, rn, rd, rs, shift, rm);
	}
	return 0;
}
unsigned int parse_inter_ldrstr(op_instr in, unsigned int adr)
{
/*
BLDOP_LDRSTR_I(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm)
BLDOP_LDRSTR_RSR(int cond, int p, int u, int b, int w, int l, int rn, int rd, int rs, int shift, int rm)
BLDOP_LDRSTR_RSI(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm , int shift, int rm)
*/
	int l = (in.opcode==OP_LDR||in.opcode==OP_LDRB)?1:0;
	int b = (in.opcode==OP_LDRB||in.opcode==OP_STRB)?1:0;
	int rd;
	if(sscanf(in.arg[0], "%i", &rd) != 1)
		printf("ERR: %s\n", __func__);
// ldr rX, =LBL		<< translates to rX, pc, #OFS
	if(in.argtype[1] == ARG_LBL && in.argcnt == 2)
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = 15;	// PC
		int imm = lbl2adr(in.arg[1]);
// FIXME: fix ldrstr imm
//		imm = (imm-adr-PIPELINE_OFS);
		printf("LDRSTR IMM: %i\n", imm);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		if(in.argcnt == 6)
			w = 1;
		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}


// ldr rX, [ rX] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && ((in.argcnt == 4) || (in.argcnt == 5 && in.argtype[4] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(in.argcnt == 5)
			w = 1;
		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, 0);
	}

// ldr rX, [ rX, #IMM] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_IMM && in.argtype[4] == ARG_ADRMODE && ((in.argcnt == 5) || (in.argcnt == 6 && in.argtype[5] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn;
		int imm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[3], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		if(in.argcnt == 6)
			w = 1;
		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}
// ldr rX, [ rX, rX] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_REG && in.argtype[4] == ARG_ADRMODE && ((in.argcnt == 5) || (in.argcnt == 6 && in.argtype[5] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn;
		int rm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[3], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		if(in.argcnt == 6)
			w = 1;
		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, 0, 0, rm);
	}
// ldr rX, [ rX, rX lsl #IMM] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_REG && in.argtype[4] == ARG_SHIFT && in.argtype[5] == ARG_IMM && in.argtype[6] == ARG_ADRMODE && ((in.argcnt == 7) || (in.argcnt == 8 && in.argtype[7] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn;
		int rm;
		int shift;
		int imm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[3], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[4]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[5], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		if(in.argcnt == 8)
			w = 1;
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, imm, shift, rm);
	}
// ldr rX, [ rX], #IMM
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_IMM && in.argcnt == 5)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn;
		int imm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[4], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}
// ldr rX, [ rX], rX
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_REG && in.argcnt == 5)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn;
		int rm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[4], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, 0, 0, rm);
	}
// ldr rX, [ rX], rX
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_REG && in.argtype[5] == ARG_SHIFT && in.argtype[6] == ARG_IMM && in.argcnt == 7)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn;
		int rm;
		int shift;
		int imm;
		if(sscanf(in.arg[2], "%i", &rn) != 1)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[4], "%i", &rm) != 1)
			printf("ERR: %s\n", __func__);
		shift = str2shift(in.arg[5]);
		if(shift<0)
			printf("ERR: %s\n", __func__);
		if(sscanf(in.arg[6], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, imm, shift, rm);
	}
	return 0;
}
unsigned int parse_inter_ldmstm(op_instr in)
{
	int p = 0;
	int u = 0;
	int b = 0;
	int w = 0;
	int l = (in.opcode==OP_LDM)?1:0;
	int rn = 0;
	int regmap = 0;
	switch(in.flag)
	{
		case 0:	// ia
			u = 1;
			p = 0;
			break;
		case 1: // ib
			u = 1;
			p = 1;
			break;
		case 2: // da
			u = 0;
			p = 0;
			break;
		case 3: // db
			u = 0;
			p = 1;
			break;
	}

	if(sscanf(in.arg[0], "%i", &rn) != 1)
		printf("ERR: %s\n", __func__);
	int regstart = 1;
	if(in.argtype[1] == ARG_ADRMODE)
	{
		w = 1;
		regstart++;
	}
	for(int i=regstart;i<in.argcnt;i++)
	{
		int reg;
		if(sscanf(in.arg[i], "%i", &reg) != 1)
			printf("ERR: %s\n", __func__);
		regmap |= (1<<reg);
	}
	return BLDOP_LDMSTM(in.cond, p, u, b, w, l, rn, regmap);
}
unsigned int parse_inter_b(op_instr in, unsigned int adr)
{
	int bta = lbl2adr(in.arg[0]);
	bta = ((bta-adr-PIPELINE_OFS)>>2)&0xFFFFFF;
	return BLDOP_B(in.cond, in.flag, bta);
}
unsigned int parse_inter_dcd(op_instr in)
{
	int out;
	if(in.argtype[0] == ARG_LIT || in.argtype[0] == ARG_IMM)
	{
		if(sscanf(in.arg[0], "%i", &out) != 1)
			printf("ERR: %s, %s\n", __func__, in.arg[0]);
	}
	else if(in.argtype[0] == ARG_LBL)
		out = lbl2adr(in.arg[0]);
	return out;
}
unsigned int parse_inter_fill(op_instr in)
{
	return 0;
}
unsigned int parse_inter_swi(op_instr in)
{
	if(in.argtype[0] == ARG_IMM && in.argcnt == 1)
	{
		int imm;
		if(sscanf(in.arg[0], "%i", &imm) != 1)
			printf("ERR: %s\n", __func__);
		return BLDOP_SWI(in.cond, imm);
	}
	return 0;
}

unsigned int parse_intermediate()
{
	for(int i=0;i<INTERMEDIATE_CNT;i++)
	{
		int out = 0xDEADBEEF;
		switch(INTERMEDIATE[i].opcode)
		{
			case OP_MOV:
			case OP_MVN:
				out = parse_inter_mov(INTERMEDIATE[i]);
				break;

			case OP_ADD:
			case OP_ADC:
			case OP_SUB:
			case OP_SBC:
			case OP_RSB:
			case OP_RSC:
			case OP_AND:
			case OP_EOR:
			case OP_BIC:
			case OP_ORR:
				out = parse_inter_alu(INTERMEDIATE[i]);
				break;

			case OP_CMP:
			case OP_CMN:
			case OP_TST:
			case OP_TEQ:
				out = parse_inter_cmp(INTERMEDIATE[i]);
				break;

			case OP_MUL:
			case OP_MLA:
				out = parse_inter_mul(INTERMEDIATE[i]);
				break;

			case OP_LDM:
			case OP_LDR:
			case OP_STR:
			case OP_LDRB:
			case OP_STRB:
				out = parse_inter_ldrstr(INTERMEDIATE[i], i*4);
				break;

			case OP_B:
				out = parse_inter_b(INTERMEDIATE[i],i*4);
				break;

			case OP_DCD:
				out = parse_inter_dcd(INTERMEDIATE[i]);
				break;
			case OP_FIL:
				out = parse_inter_fill(INTERMEDIATE[i]);
				break;
			case OP_SWI:
				out = parse_inter_swi(INTERMEDIATE[i]);
				break;
			case OP_END:
				out = INTERMEDIATE[i].opcode;
				break;
		}
		memcpy(&MEM[i*4], &out, sizeof(unsigned int));
	}
	return lbl2adr("_start");
}
