#include "raylib.h"
#include <string.h>
typedef struct drawdata
{
	int spritenr;
	int x;
	int y;
} drawdata;

#define SPRITECNT 64
Texture2D SPRITES[SPRITECNT];

#define DRAWDATA_FG_CNT 32
drawdata DRAWDATA_FG[DRAWDATA_FG_CNT];
#define DRAWDATA_BG_CNT 32
drawdata DRAWDATA_BG[DRAWDATA_BG_CNT];

void raylib_init_sprites()
{
	memset(DRAWDATA_FG, 0, DRAWDATA_FG_CNT*sizeof(drawdata));
	memset(DRAWDATA_BG, 0, DRAWDATA_BG_CNT*sizeof(drawdata));
	memset(SPRITES, 0, SPRITECNT*sizeof(Texture2D));
	SPRITES[1] = LoadTexture("prog/sprite/player.png");
	SPRITES[2] = LoadTexture("prog/sprite/stone.png");
	SPRITES[3] = LoadTexture("prog/sprite/grass.png");
	SPRITES[4] = LoadTexture("prog/sprite/dirt.png");
	SPRITES[5] = LoadTexture("prog/sprite/door.png");
}
