#include "raylib.h"
void swi_printreg()
{
	printf("\nSWI PRINTREG\n");
	for(unsigned int i=0;i<16;i++)
	{
		printf("r%i: %i\n", i, r[i]);
	}
}
void swi_printmem()
{
	int adr = r[0];
	int cnt = r[1];
	printf("\nSWI PRINTMEM\n");
	for(unsigned int i=0;i<cnt;i++)
	{
		printf("%x: %.8x\n", adr+i, MEM[adr+i]);
	}
}
void swi_initraylib()
{
	const int width = 640;
	const int height = 480;
	InitWindow(width, height, "armsim");
	SetTargetFPS(10);
	raylib_init_sprites();
}

void swi_runraylib()
{
	BeginDrawing();
		ClearBackground(RAYWHITE);
		int i = 0;
		while(DRAWDATA_BG[i].spritenr != 0)
		{
			i++;
		}
		i = 0;
		while(DRAWDATA_FG[i].spritenr != 0)
		{
			DrawTexture(SPRITES[DRAWDATA_FG[i].spritenr], DRAWDATA_FG[i].x, DRAWDATA_FG[i].y, WHITE);
			i++;
		}
	EndDrawing();
	memset(DRAWDATA_FG, 0, DRAWDATA_FG_CNT*sizeof(drawdata));
    memset(DRAWDATA_BG, 0, DRAWDATA_BG_CNT*sizeof(drawdata));
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
}
void swi_exitraylib()
{
	CloseWindow();
}
void swi_drawsprite_fg()
{
	drawdata draw = {0};
	draw.spritenr 	= r[0];
	draw.x 			= r[1];
	draw.y 			= r[2];
	DRAWDATA_FG[r[3]] = draw;
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
		{0xFFFFFFFF, "DONE"},
	};
void (*swi_tbl[])(void) = {
		swi_printreg,		// 0
		swi_printmem,		// 1
		swi_initraylib,		// 2
		swi_runraylib,		// 3
		swi_exitraylib,		// 4
		swi_drawsprite_fg,	// 5
	};
#define SWICNT 6


