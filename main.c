#include "core.h"

#include "parse.h"

#include "raylibdata.c"
#include "swi.c"

#include "parse.c"

#include "core.c"

int main()
{
    parse_file("prog/in.s");
    print_intermediate();
    pc = parse_intermediate();
	printf("MEMORY:\n");
    for(int i=0;i<MEM_SIZE;i++)
    {
		int opcode;
        memcpy(&opcode, &MEM[i*4], sizeof(unsigned int));
		if(i%4 == 0)
			printf("%.3i: ", i*4);
        printf("%.8x ", opcode);
		if(i%4 == 3)
			printf("\n");
        if(opcode == OP_END)
            break;
    }
	printf("\n");
	printf("EXECUTING:\n");
    while(1)
    {
		int opcode;
        memcpy(&opcode, &MEM[pc], sizeof(unsigned int));
//        printf("%.3i: %.8x\n", pc, opcode);
        if(opcode == OP_END)
		{
//			printf("\n");
            break;
		}
		exec(opcode);
    }
    return 0;
}

