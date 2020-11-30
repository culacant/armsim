#define SWI_PRINTREG 255
#define SWI_PRINTMEM 256
mnem_data mnem_swi_tbl[] = {
// INPUT: /
// OUTPUT: /
		{SWI_PRINTREG, "swi_printreg"},
// INPUT: r0: adr, r1: cnt
// OUTPUT: /
		{SWI_PRINTMEM, "swi_printmem"},
		{0xFFFFFFFF, "DONE"},
	};

void swi_printreg()
{
printf("%s\n", __func__);
	printf("\nSWI PRINTREG\n");
	for(unsigned int i=0;i<16;i++)
	{
		printf("r%i: %i\n", i, r[i]);
	}
}
void swi_printmem()
{
printf("%s\n", __func__);
	int adr = r[0];
	int cnt = r[1];
	printf("\nSWI PRINTMEM\n");
	for(unsigned int i=0;i<cnt;i++)
	{
		printf("%x: %.8x\n", adr+i, MEM[adr+i]);
	}
}


