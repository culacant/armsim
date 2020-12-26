#include <string.h>
#include <stdlib.h>

int check_err(op_instr instr, char *str, unsigned int val, int type)
{
	int err = 0;
	if(instr.line[strlen(instr.line)-1] == '\n')
		instr.line[strlen(instr.line)-1] = '\0';
	switch(type)
	{
		    case ERR_ARG_CND:
				if(val>=0xF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid conditional: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_OPCODE:
				if(val>0xF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid opcode: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_PUBWL:
				if(val!=1 && val!=0)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid PUBWL value: %i\n", val);
					err = 1;
				}
				break;
			case ERR_ARG_S:
				if(val!=1 && val!=0)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid flags value: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_REG:
				if(val>0xF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid register: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_ROT:
				if(val>0xF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid rotation: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_IMM8:
				if(val>0xFF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Immediate more than 8 bits: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_IMM12:
				if(val>0xFF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Immediate more than 12 bits: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_SHIFT:
				if(val>0x1FF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid shift: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_REGMAP:
				if(val>0xFFFF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid register map: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_SWI:
				if(val>0xFFFFFF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid SWI #: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_BTA:
				if(val>0xFFFFFF)
				{
					printf("Line %i: %s\n", instr.linenr, instr.line);
					printf("Invalid branch offset: %s, %i\n", str, val);
					err = 1;
				}
				break;
			case ERR_ARG_ARGS:
				printf("Line %i: %s\n", instr.linenr, instr.line);
				printf("Invalid arguments\n");
				break;
	}
	return err;
}
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
		printf("\nLINE: %i: %s", INTERMEDIATE[i].linenr, INTERMEDIATE[i].line);
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
op_instr parseline_cnd(mnem_data mnem, op_instr out, char *string)
{
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
	op_instr out = in;
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
	int linenr = 0;
	while(fgets(line, 128, f) != NULL)
	{
		linenr++;
		char lineo[256];
		sprintf(lineo,"%s", line);
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
			op_instr instr = {0};
			strcpy(instr.line, lineo);
			instr.linenr = linenr;
			if(pch[0] == ';')
				break;
			mnem_data opmnem = parseline_op(pch);
			if(strcmp(opmnem.str, "ERR") !=0)
			{
				instr = parseline_cnd(opmnem, instr, pch);
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
		int rd = -1;
		int imm = -1;
		sscanf(in.arg[0], "%i", &rd);
		sscanf(in.arg[1], "%i", &imm);

		check_err(in, in.arg[0], rd, ERR_ARG_REG);
		check_err(in, in.arg[1], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, 0, rd, 0, imm);
	}
// mov rX, =lbl
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_LBL && in.argcnt == 2)
	{
		int rd = -1;
		int imm = -1;
		sscanf(in.arg[0], "%i", &rd);
		imm = lbl2adr(in.arg[1]);

		check_err(in, in.arg[0], rd, ERR_ARG_REG);
		check_err(in, in.arg[1], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, 0, rd, 0, imm);
	}
// mov rX, rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argcnt == 2)
	{	
		int rd = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rd);
		sscanf(in.arg[1], "%i", &rm);

		check_err(in, in.arg[0], rd, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);

		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, 0, rd, 0, 0, rm);
	}
// mov rX, rX, LSL #IMM
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_IMM && in.argcnt == 4)
	{
		int rd = -1;
		int imm = -1;
		int shift = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rd);
		sscanf(in.arg[1], "%i", &rm);
		shift = str2shift(in.arg[2]);
		sscanf(in.arg[3], "%i", &imm);

		check_err(in, in.arg[0], rd, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);
		check_err(in, in.arg[2], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[3], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, 0, rd, imm, shift, rm);
	}
// mov rX, rX, LSL rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_REG && in.argcnt == 4)
	{
		int rd = -1;
		int rs = -1;
		int shift = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rd);
		sscanf(in.arg[1], "%i", &rm);
		shift = str2shift(in.arg[2]);
		sscanf(in.arg[3], "%i", &rs);

		check_err(in, in.arg[0], rd, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);
		check_err(in, in.arg[2], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[3], rs, ERR_ARG_REG);

		return BLDOP_ALU_RSR(in.cond, in.opcode, in.flag, 0, rd, rs, shift, rm);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);

	return 0;
}
unsigned int parse_inter_cmp(op_instr in)
{
// cmp rX, #IMM
	if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_IMM && in.argcnt == 2)
	{
		int rn = -1;
		int imm = -1;
		sscanf(in.arg[0], "%i", &rn);
		sscanf(in.arg[1], "%i", &imm);

		check_err(in, in.arg[0], rn, ERR_ARG_REG);
		check_err(in, in.arg[1], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_I(in.cond, in.opcode, 1, rn, 0, 0, imm);
	}
// cmp rX, rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argcnt == 2)
	{	
		int rn = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rn);
		sscanf(in.arg[1], "%i", &rm);

		check_err(in, in.arg[0], rn, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);

		return BLDOP_ALU_RSI(in.cond, in.opcode, 1, rn, 0, 0, 0, rm);
	}
// cmp rX, rX, LSL #IMM
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_IMM && in.argcnt == 4)
	{
		int rn = -1;
		int imm = -1;
		int shift = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rn);
		sscanf(in.arg[1], "%i", &rm);
		shift = str2shift(in.arg[2]);
		sscanf(in.arg[3], "%i", &imm);


		check_err(in, in.arg[0], rn, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);
		check_err(in, in.arg[2], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[3], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_RSI(in.cond, in.opcode, 1, rn, 0, imm, shift, rm);
	}
// cmp rX, rX, LSL rX
	else if(in.argtype[0] == ARG_REG && in.argtype[1] == ARG_REG && in.argtype[2] == ARG_SHIFT && in.argtype[3] == ARG_REG && in.argcnt == 4)
	{
		int rn = -1;
		int rs = -1;
		int shift = -1;
		int rm = -1;
		sscanf(in.arg[0], "%i", &rn);
		sscanf(in.arg[1], "%i", &rm);
		shift = str2shift(in.arg[2]);
		sscanf(in.arg[3], "%i", &rs);

		check_err(in, in.arg[0], rn, ERR_ARG_REG);
		check_err(in, in.arg[1], rm, ERR_ARG_REG);
		check_err(in, in.arg[2], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[3], rs, ERR_ARG_REG);

		return BLDOP_ALU_RSR(in.cond, in.opcode, 1, rn, 0, rs, shift, rm);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);

	return 0;
}
unsigned int parse_inter_mul(op_instr in)
{
	int rd = -1;
	int rs = -1;
	int rm = -1;
	int rn = 0;

	sscanf(in.arg[0], "%i", &rd);
	sscanf(in.arg[1], "%i", &rm);
	sscanf(in.arg[2], "%i", &rs);

	check_err(in, in.arg[0], rn, ERR_ARG_REG);
	check_err(in, in.arg[1], rm, ERR_ARG_REG);
	check_err(in, in.arg[2], rs, ERR_ARG_REG);

	if(in.opcode == OP_MLA)
	{
		rn = -1;
		sscanf(in.arg[3], "%i", &rn);
		check_err(in, in.arg[3], rn, ERR_ARG_REG);
	}
	in.opcode = in.opcode&0xF;
	return BLDOP_MUL(in.cond, in.opcode, in.flag, rn, rd, rs, rm);
}
unsigned int parse_inter_alu(op_instr in)
{
// add rX, rX, #IMM
	int rd = -1;
	sscanf(in.arg[0], "%i", &rd);

	check_err(in, in.arg[0], rd, ERR_ARG_REG);

	if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_IMM && in.argcnt == 3)
	{
		int rn = -1;
		int imm = -1;
		sscanf(in.arg[1], "%i", &rn);
		sscanf(in.arg[2], "%i", &imm);

		check_err(in, in.arg[1], rn, ERR_ARG_REG);
		check_err(in, in.arg[2], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_I(in.cond, in.opcode, in.flag, rn, rd, 0, imm);
	}
// add rX, rX, rX
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argcnt == 3)
	{	
		int rn = -1;
		int rm = -1;
		sscanf(in.arg[1], "%i", &rn);
		sscanf(in.arg[2], "%i", &rm);

		check_err(in, in.arg[1], rn, ERR_ARG_REG);
		check_err(in, in.arg[2], rm, ERR_ARG_REG);

		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, rn, rd, 0, 0, rm);
	}
// add rX, rX, rX LSL #IMM
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_SHIFT && in.argtype[4]==ARG_IMM && in.argcnt == 5)
	{
		int rn = -1;
		int rm = -1;
		int shift = -1;
		int imm = -1;
		sscanf(in.arg[1], "%i", &rn);
		sscanf(in.arg[2], "%i", &rm);
		shift = str2shift(in.arg[3]);
		sscanf(in.arg[4], "%i", &imm);


		check_err(in, in.arg[1], rn, ERR_ARG_REG);
		check_err(in, in.arg[2], rm, ERR_ARG_REG);
		check_err(in, in.arg[3], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[4], imm, ERR_ARG_IMM8);

		return BLDOP_ALU_RSI(in.cond, in.opcode, in.flag, rn, rd, imm, shift, rm);
	}
// add rX, rX, rX LSL rX
	else if(in.argtype[1] == ARG_REG && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_SHIFT && in.argtype[4]==ARG_REG&& in.argcnt == 5)
	{
		int rn = -1;
		int rm = -1;
		int shift = -1;
		int rs = -1;
		sscanf(in.arg[1], "%i", &rn);
		sscanf(in.arg[2], "%i", &rm);
		shift = str2shift(in.arg[3]);
		sscanf(in.arg[4], "%i", &rs);


		check_err(in, in.arg[1], rn, ERR_ARG_REG);
		check_err(in, in.arg[2], rm, ERR_ARG_REG);
		check_err(in, in.arg[3], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[4], rs, ERR_ARG_REG);

		return BLDOP_ALU_RSR(in.cond, in.opcode, in.flag, rn, rd, rs, shift, rm);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);

	return 0;
}
unsigned int parse_inter_ldrstr(op_instr in, unsigned int adr)
{
	int l = (in.opcode==OP_LDR||in.opcode==OP_LDRB)?1:0;
	int b = (in.opcode==OP_LDRB||in.opcode==OP_STRB)?1:0;
	int rd = -1;
	sscanf(in.arg[0], "%i", &rd);

	check_err(in, in.arg[0], rd, ERR_ARG_REG);

// ldr rX, =LBL		<< translates to rX, pc, #OFS
	if(in.argtype[1] == ARG_LBL && in.argcnt == 2)
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = 15;	// PC
		int imm = lbl2adr(in.arg[1]);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		if(in.argcnt == 6)
			w = 1;

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, "NOPE", rn, ERR_ARG_REG);
		check_err(in, in.arg[1], imm, ERR_ARG_IMM12);

		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}
// ldr rX, [ rX] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && ((in.argcnt == 4) || (in.argcnt == 5 && in.argtype[4] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = -1;
		sscanf(in.arg[2], "%i", &rn);
		if(in.argcnt == 5)
			w = 1;
		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);

		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, 0);
	}

// ldr rX, [ rX, #IMM] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_IMM && in.argtype[4] == ARG_ADRMODE && ((in.argcnt == 5) || (in.argcnt == 6 && in.argtype[5] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = -1;
		int imm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[3], "%i", &imm);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}
		if(in.argcnt == 6)
			w = 1;

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[3], imm, ERR_ARG_IMM12);

		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}
// ldr rX, [ rX, rX] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_REG && in.argtype[4] == ARG_ADRMODE && ((in.argcnt == 5) || (in.argcnt == 6 && in.argtype[5] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = -1;
		int rm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[3], "%i", &rm);
		if(in.argcnt == 6)
			w = 1;

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[3], rm, ERR_ARG_REG);

		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, 0, 0, rm);
	}
// ldr rX, [ rX, rX lsl #IMM] {!}
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_REG && in.argtype[4] == ARG_SHIFT && in.argtype[5] == ARG_IMM && in.argtype[6] == ARG_ADRMODE && ((in.argcnt == 7) || (in.argcnt == 8 && in.argtype[7] == ARG_ADRMODE)))
	{
		int p = 1;
		int u = 1;
		int w = 0;
		int rn = -1;
		int rm = -1;
		int shift = -1;
		int imm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[3], "%i", &rm);
		shift = str2shift(in.arg[4]);
		sscanf(in.arg[5], "%i", &imm);
		if(in.argcnt == 8)
			w = 1;
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[3], rm, ERR_ARG_REG);
		check_err(in, in.arg[4], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[5], imm, ERR_ARG_IMM12);

		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, imm, shift, rm);
	}
// ldr rX, [ rX], #IMM
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_IMM && in.argcnt == 5)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn = -1;
		int imm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[4], "%i", &imm);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[4], imm, ERR_ARG_IMM12);

		return BLDOP_LDRSTR_I(in.cond, p, u, b, w, l, rn, rd, imm);
	}
// ldr rX, [ rX], rX
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_REG && in.argcnt == 5)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn = -1;
		int rm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[4], "%i", &rm);

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[4], rm, ERR_ARG_REG);

		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, 0, 0, rm);
	}
// ldr rX, [ rX], rX
	else if(in.argtype[1] == ARG_ADRMODE && in.argtype[2] == ARG_REG && in.argtype[3] == ARG_ADRMODE && in.argtype[4] == ARG_REG && in.argtype[5] == ARG_SHIFT && in.argtype[6] == ARG_IMM && in.argcnt == 7)
	{
		int p = 0;
		int u = 1;
		int w = 0;
		int rn = -1;
		int rm = -1;
		int shift = -1;
		int imm = -1;
		sscanf(in.arg[2], "%i", &rn);
		sscanf(in.arg[4], "%i", &rm);
		shift = str2shift(in.arg[5]);
		sscanf(in.arg[6], "%i", &imm);
		if(imm<0)
		{
			u = 0;
			imm = -imm;
		}

		check_err(in, "NOPE", p, ERR_ARG_PUBWL);
		check_err(in, "NOPE", u, ERR_ARG_PUBWL);
		check_err(in, "NOPE", b, ERR_ARG_PUBWL);
		check_err(in, "NOPE", w, ERR_ARG_PUBWL);
		check_err(in, "NOPE", l, ERR_ARG_PUBWL);
		check_err(in, in.arg[2], rn, ERR_ARG_REG);
		check_err(in, in.arg[4], rm, ERR_ARG_REG);
		check_err(in, in.arg[5], shift, ERR_ARG_SHIFT);
		check_err(in, in.arg[6], imm, ERR_ARG_IMM12);

		return BLDOP_LDRSTR_RSI(in.cond, p, u, b, w, l, rn, rd, imm, shift, rm);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);

	return 0;
}
unsigned int parse_inter_ldmstm(op_instr in)
{
	int p = 0;
	int u = 0;
	int b = 0;
	int w = 0;
	int l = (in.opcode==OP_LDM)?1:0;
	int rn = -1;
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

	sscanf(in.arg[0], "%i", &rn);

	check_err(in, in.arg[0], rn, ERR_ARG_REG);

	int regstart = 1;
	if(in.argtype[1] == ARG_ADRMODE)
	{
		w = 1;
		regstart++;
	}
	for(int i=regstart;i<in.argcnt;i++)
	{
		int reg;
		sscanf(in.arg[i], "%i", &reg);
		regmap |= (1<<reg);

		check_err(in, in.arg[i], reg, ERR_ARG_REG);
	}
	return BLDOP_LDMSTM(in.cond, p, u, b, w, l, rn, regmap);
}
unsigned int parse_inter_b(op_instr in, unsigned int adr)
{
	int bta = lbl2adr(in.arg[0]);
	bta = ((bta-adr-PIPELINE_OFS)>>2)&0xFFFFFF;

	check_err(in, in.arg[0], bta, ERR_ARG_BTA);

	return BLDOP_B(in.cond, in.flag, bta);
}
unsigned int parse_inter_dcd(op_instr in)
{
	int out;
	if(in.argtype[0] == ARG_LIT || in.argtype[0] == ARG_IMM)
	{
		if(sscanf(in.arg[0], "%i", &out) != 1)
			check_err(in, "NOPE", 0, ERR_ARG_ARGS);
	}
	else if(in.argtype[0] == ARG_LBL)
	{
		out = lbl2adr(in.arg[0]);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);
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
		int imm = -1;
		sscanf(in.arg[0], "%i", &imm);

		check_err(in, in.arg[0], imm, ERR_ARG_SWI);

		return BLDOP_SWI(in.cond, imm);
	}
	else
		check_err(in, "NOPE", 0, ERR_ARG_ARGS);

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
			case OP_STM:
				out = parse_inter_ldmstm(INTERMEDIATE[i]);
				break;
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
