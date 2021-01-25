#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <math.h>

int RGB(int r, int g, int b)
{
	return (r<<16) | (g<<8) | (b<<4);
}

int width;
int height;
int *buf;
void popcorn(int x0, int y0, int h, int it)
{
	static unsigned int col = 0;
	float x = (float)x0;
	float y = (float)y0;

	for(int i=0;i<it;i++)
	{
		float xt = x;
		x = x-h*sinf(y+tan(3.f*y));
		y = y-h*sinf(xt+tan(3.f*xt));

		if(x<0)
			x = width-x;
		if(x>width)
			x -= width;
		if(y<0)
			y = height-y;
		if(y>height)
			y -= height; 
		int dx = (int)x+0;
		int dy = (int)y+4;
		if(dy<height && dx<width &&
			dy>0 && dx>0)
		{
			//buf[dy*width+dx] = col;
			buf[dy*width+dx]++;
//			usleep(1);
			col++;
//			printf("%.8x\n", col);
			if(col%0x10000 == 0)
				col += 0x11100000;
		}
	}
}
int main(void) 
{
	int fb = open("/dev/fb0", O_RDWR);
	struct fb_var_screeninfo info;
	ioctl(fb, FBIOGET_VSCREENINFO, &info);
	size_t len = 4 * info.xres * info.yres;

	buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

	width = info.xres;
	height = info.yres;
	int numit = 100;
	float h = 3.f;
	int dx = width/2;
	int dy = height/2;
	popcorn(dx, dy, h, 1000000000);
	return 0;
}
