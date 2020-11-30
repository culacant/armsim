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
