#include <stdio.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/fcntl.h>

int RGB(int r, int g, int b)
{
	return (r<<16) | (g<<8) | (b<<4);
}

int main(void) 
{
	int fb = open("/dev/fb0", O_RDWR);
	struct fb_var_screeninfo info;
	ioctl(fb, FBIOGET_VSCREENINFO, &info);
	size_t len = 4 * info.xres * info.yres;

	int *buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

	int cols = info.xres;
	int rows = info.yres;
	int maxval = 255;

	double pr, pi;
	double newRe;
	double newIm;
	double oldRe;
	double oldIm;

	double zoomx = 0.3;
	double zoomy = 0.3;
	double moveX = -1.57458795122839f;
	double moveY = 0;
int it = 0;
while(it<100)
{
it++;
zoomx*=2;
zoomy*=2;
	double divx = 1/((zoomx*cols)/2);
	double divy = 1/((zoomy*rows)/2);

	int itcnt = 256;

	for(int x=0;x<cols;x++)
	{
		for(int y=0;y<rows;y++)
		{
			pr = (x-cols/2)*divx;
			pr = pr + pr/2;
			pr = pr + moveX;

			pi = (y-rows/2)*divy;
			pi = pi + moveY;

			newRe = 0;
			newIm = 0;
			oldRe = 0;
			oldIm = 0;
			int i;
			for(i=0;i<itcnt;i++)
			{
				oldRe = newRe;
				oldIm = newIm;

				newRe = oldRe*oldRe - oldIm*oldIm+pr;
				newIm = 2*oldRe*oldIm+pi;
				if((newRe*newRe + newIm*newIm)>4)
					break;
			}
			if(i<itcnt)
				buf[y*info.xres+x] = RGB(maxval-(int)newIm*2, maxval-(int)newRe*2,i%128);
			else
				buf[y*info.xres+x] = RGB((int)oldRe*2, (int)oldIm*2,255-(i%128));
		}
	}
}
	return 0;
}
