enum ARG_TYPE
{
	ARG_REG			= 0,
	ARG_IMM			= 1,
	ARG_SHIFT		= 2,
	ARG_LBL			= 3,
	ARG_LIT			= 4,
	ARG_ADRMODE		= 5,
};

typedef struct label
{
	char name[32];
	unsigned int adr;
} label;
typedef struct mnem_data
{
	unsigned int opcode;
	char str[32];
} mnem_data;

typedef struct op_instr
{
	char line[128];
	int linenr;
	int opcode;
	int cond;
	int flag;
	int argcnt;
	int argtype[20];
	char arg[20][10];
} op_instr;

enum DATA_TYPE
{
	TYPE_DATA	= 0,
	TYPE_INSTR	= 1,
};

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
enum ERR_ARG_TYPE
{
	ERR_ARG_CND			= 0,
	ERR_ARG_OPCODE		= 1,
	ERR_ARG_PUBWL		= 2,
	ERR_ARG_S			= 3,
	ERR_ARG_REG			= 4,
	ERR_ARG_ROT			= 5,
	ERR_ARG_IMM8		= 6,
	ERR_ARG_IMM12		= 7,
	ERR_ARG_SHIFT		= 8,
	ERR_ARG_REGMAP		= 9,
	ERR_ARG_SWI			= 10,
	ERR_ARG_BTA			= 11,
	ERR_ARG_ARGS		= 12,
};
int check_err(op_instr instr, char *str, unsigned int val, int type);
void print_intermediate();

mnem_data parseline_op(char *string);
op_instr parseline_cnd(mnem_data mnem, op_instr out, char *string);

op_instr parse_reg(op_instr in, char *string);
op_instr parse_imm(op_instr in, char *string);
op_instr parse_lbl(op_instr in, char *string);
op_instr parse_shft(op_instr in, char *string);
op_instr parse_alu(op_instr in, char *string);
op_instr parse_mul(op_instr in, char *string);
op_instr parse_ldrstr(op_instr in, char *string);
op_instr parse_b(op_instr in, char *string);
op_instr parse_dcd(op_instr in, char *string);
op_instr parse_dcb(op_instr in, char *string);
op_instr parse_fill(op_instr in, char *string);
op_instr parse_ldmstm(op_instr in, char *string);
op_instr parse_swi(op_instr in, char *string);
op_instr parse_adr(op_instr in, char *string);

int parse_file(char *filename);

unsigned int BLDOP_ALU_I(int cond, int op, int s, int rn, int rd, int rot, int imm);
unsigned int BLDOP_ALU_RSR(int cond, int op, int s, int rn, int rd, int rs, int shift, int rm);
unsigned int BLDOP_ALU_RSI(int cond, int op, int s, int rn, int rd, int imm, int shift, int rm);
unsigned int BLDOP_MUL(int cond, int op, int s, int rn, int rd, int rs, int rm);
unsigned int BLDOP_LDRSTR_I(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm);
unsigned int BLDOP_LDRSTR_RSR(int cond, int p, int u, int b, int w, int l, int rn, int rd, int rs, int shift, int rm);
unsigned int BLDOP_LDRSTR_RSI(int cond, int p, int u, int b, int w, int l, int rn, int rd, int imm , int shift, int rm);
unsigned int BLDOP_LDMSTM(int cond, int p, int u, int b, int w, int l, int rn, int regmap);
unsigned int BLDOP_B(int cond, int l, int bta);
unsigned int BLDOP_SWI(int cond, int imm);

int str2shift(char *string);
int lbl2adr(char *string);

unsigned int parse_inter_mov(op_instr in);
unsigned int parse_inter_cmp(op_instr in);
unsigned int parse_inter_mul(op_instr in);
unsigned int parse_inter_alu(op_instr in);
unsigned int parse_inter_ldrstr(op_instr in, unsigned int adr);
unsigned int parse_inter_ldmstm(op_instr in);
unsigned int parse_inter_b(op_instr in, unsigned int adr);
unsigned int parse_inter_dcd(op_instr in);
unsigned int parse_inter_fill(op_instr in);
unsigned int parse_inter_swi(op_instr in);

unsigned int parse_intermediate();
