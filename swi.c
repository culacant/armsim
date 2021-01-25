#include "raylib.h"
void swi_printreg()
{
	printf("\nSWI PRINTREG\n");
	for(unsigned int i=0;i<16;i++)
	{
		printf("r%i: %i\n", i, r[i]);
	}
	printf("cpsr: ");
	if(cpsr&V)
		printf("V");
	if(cpsr&C)
		printf("C");
	if(cpsr&Z)
		printf("Z");
	if(cpsr&N)
		printf("N");
	printf("\n");
}
void swi_printmem()
{
	int adr = r[0];
	int cnt = r[1];
	printf("\nSWI PRINTMEM\n");
	for(unsigned int i=0;i<cnt;i++)
	{
		int opcode;
		memcpy(&opcode, &adr+4*i, sizeof(unsigned int));
		printf("%x: %.8x\n", adr+4*i, opcode);
	}
}
void swi_initraylib()
{
	const int width = 640;
	const int height = 640;
	InitWindow(width, height, "armsim");
	SetTargetFPS(15);
	raylib_init_sprites();
}

void swi_runraylib()
{
	BeginDrawing();
		ClearBackground(RAYWHITE);
// bg
		int i = 0;
		i = 0;
		while(DRAWDATA_BG[i].spritenr != 0)
		{
			DrawTexture(SPRITES[DRAWDATA_BG[i].spritenr], DRAWDATA_BG[i].x, DRAWDATA_BG[i].y, WHITE);
			i++;
		}
// fg
		i = 0;
		while(DRAWDATA_FG[i].spritenr != 0)
		{
			char *rb = (char*)&DRAWDATA_FG[i].spritenr;
			Rectangle spriterec = { (float)(64.f*rb[1]),(float)(64.f*rb[2]), 64.f, 64.f};
			DrawTextureRec(SPRITES[(int)rb[0]], spriterec, (Vector2){DRAWDATA_FG[i].x, DRAWDATA_FG[i].y}, WHITE);
			i++;
		}
		DrawFPS(10,10);
	EndDrawing();
	memset(DRAWDATA_FG, 0, DRAWDATA_FG_CNT*sizeof(drawdata));
	DRAWDATA_FG_I = 0;
    memset(DRAWDATA_BG, 0, DRAWDATA_BG_CNT*sizeof(drawdata));
	DRAWDATA_BG_I = 0;
// set inputs
	r[0] = 0;
	if(IsKeyDown(KEY_W))
		r[0] |= 1<<0;
	if(IsKeyDown(KEY_S))
		r[0] |= 1<<1;
	if(IsKeyDown(KEY_A))
		r[0] |= 1<<2;
	if(IsKeyDown(KEY_D))
		r[0] |= 1<<3;
	if(IsKeyDown(KEY_Q))
		r[0] |= 1<<4;
	if(IsKeyDown(KEY_SPACE))
		r[0] |= 1<<5;
}
void swi_exitraylib()
{
	CloseWindow();
}
void swi_drawsprite_fg()
{
/*
char *rb = (char*)&r[0];
printf("SPRITE #: %i OFS X: %i Y: %i R: %i\n", rb[0], rb[1], rb[2], rb[3]);
*/
	drawdata draw = {0};
	draw.spritenr 	= r[0];
	draw.x 			= r[1];
	draw.y 			= r[2];
	DRAWDATA_FG[DRAWDATA_FG_I] = draw;
	DRAWDATA_FG_I++;
}
void swi_drawsprite_bg()
{
	drawdata draw = {0};
	draw.spritenr 	= r[0];
	draw.x 			= r[1];
	draw.y 			= r[2];
	DRAWDATA_BG[DRAWDATA_BG_I] = draw;
	DRAWDATA_BG_I++;
}
mnem_data mnem_swi_tbl[] = {
// INPUT: /
// OUTPUT: /
		{0, "swi_printreg"},
// INPUT: r0: adr, r1: cnt
// OUTPUT: /
		{1, "swi_printmem"},
		{2, "swi_initraylib"},
// OUTPUT: r0: inputs
		{3, "swi_runraylib"},
		{4, "swi_stopraylib"},
// INPUT: r0: spritenr, r1: xpos, r2: ypos: r3: drawnr
// OUTPUT: /
		{5, "swi_drawsprite_fg"},
		{6, "swi_drawsprite_bg"},
		{0xFFFFFFFF, "DONE"},
	};
void (*swi_tbl[])(void) = {
		swi_printreg,		// 0
		swi_printmem,		// 1
		swi_initraylib,		// 2
		swi_runraylib,		// 3
		swi_exitraylib,		// 4
		swi_drawsprite_fg,	// 5
		swi_drawsprite_bg,	// 6
	};
#define SWICNT 7


