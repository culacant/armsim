#include <stdio.h>

int alu_op_cnt = 9;
char alu_op[][128] = 
	{
		"add",
		"adc",
		"sub",
		"sbc",
		"rsb",
		"rsc",
		"and",
		"bic",
		"orr",
	};
int mov_op_cnt = 2;
char mov_op[][128] =
	{
		"mov",
		"mvn",
	};
int cmp_op_cnt = 4;
char cmp_op[][128] =
	{
		"cmp",
		"cmn",
		"tst",
		"teq",
	};
int cmpmov_arg_cnt = 4;
char cmpmov_arg[][128] = 
	{
		"r11, #20",
		"r12, r13",
		"r11, r12, lsl #2",
		"r11, r12, lsl r2",
	};
int alu_arg_cnt = 4;
char alu_arg[][128] = 
	{
		"r10, r11, #20",
		"r11, r12, r13",
		"r10, r11, r12, lsl #2",
		"r10, r11, r12, lsl r2",
	};
int ldrstr_op_cnt = 2;
char ldrstr_op[][128] = 
	{
		"ldr",
		"str",
	};
int ldrstr_arg_cnt = 9;
char ldrstr_arg[][128] = 
	{
		"r10, [r11, #20]",
		"r10, [r11, r12]",
		"r10, [r11, r12, lsl #2]",
		"r10, [r11, #20]!",
		"r10, [r11, r12]!",
		"r10, [r11, r12, lsl #2]!",
		"r10, [r11], #2",
		"r10, [r11], r12",
		"r10, [r11], r12, lsl #2",
	};

int main()
{
	for(int i=0;i<alu_op_cnt;i++)
	{
		for(int j=0;j<alu_arg_cnt;j++)
		{
			printf("%s %s\n", alu_op[i], alu_arg[j]);
		}
	}
	for(int i=0;i<cmp_op_cnt;i++)
	{
		for(int j=0;j<cmpmov_arg_cnt;j++)
		{
			printf("%s %s\n", cmp_op[i], cmpmov_arg[j]);
		}
	}
	for(int i=0;i<mov_op_cnt;i++)
	{
		for(int j=0;j<cmpmov_arg_cnt;j++)
		{
			printf("%s %s\n", mov_op[i], cmpmov_arg[j]);
		}
	}
	for(int i=0;i<ldrstr_op_cnt;i++)
	{
		for(int j=0;j<ldrstr_arg_cnt;j++)
		{
			printf("%s %s\n", ldrstr_op[i], ldrstr_arg[j]);
		}
	}

	return 0;
}

