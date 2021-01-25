#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

#define NUMTHREADS 4
/*
A10-9700B:
1: 20, 20
2: 12, 24
3: 11, 25
4: 9, 27

T20:
1: 29, 29
2: 14, 29
*/
#define NUMITS 32

typedef struct mandeldata
{
	int cols;
	int rows;
	int maxval;

	double zoomx;
	double zoomy;
	double moveX;
	double moveY;

	int miny;
	int maxy;

	int *buf;

} mandeldata;
int RGB(int r, int g, int b)
{
	return (r<<16) | (g<<8) | (b<<4);
}
int domandelbrot(void *arg)
{
	mandeldata data = *(mandeldata*)arg;
	double pr, pi;
	double newRe;
	double newIm;
	double oldRe;
	double oldIm;

	double divx = 1/((data.zoomx*data.cols)/2);
	double divy = 1/((data.zoomy*data.rows)/2);

	int itcnt = 256;

	for(int x=0;x<data.cols;x++)
	{
		for(int y=data.miny;y<data.maxy;y++)
		{
			pr = (x-data.cols/2)*divx;
			pr = pr + pr/2;
			pr = pr + data.moveX;

			pi = (y-data.rows/2)*divy;
			pi = pi + data.moveY;

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
				data.buf[(y-data.miny)*data.cols+x] = RGB(data.maxval-(int)newRe, data.maxval-(int)newIm,i%255);
			else
				//data.buf[(y-data.miny)*data.cols+x] = RGB((int)oldRe, (int)oldIm,255-(i%255));
				data.buf[(y-data.miny)*data.cols+x] = RGB(i*oldRe, i*oldIm, i+(oldRe*oldIm));
		}
	}
	return 0;
}
int main(void) 
{
	int fb = open("/dev/fb0", O_RDWR);
	struct fb_var_screeninfo info;
	ioctl(fb, FBIOGET_VSCREENINFO, &info);
	size_t len = 4 * info.xres * info.yres;
	mandeldata data;

	int *buf = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

	data.cols = info.xres;
	data.rows = info.yres;
	data.maxval = 255;

	data.miny = 0;
	data.maxy = 255;

	data.zoomx = 2;
	data.zoomy = 2;
	data.moveX = -1.5745879;
	data.moveY = -0.000248123;
	int it = 0;
	mandeldata *threaddata = calloc(sizeof(mandeldata),NUMTHREADS);
	const int STACK_SIZE = 65536;
	char** stack = malloc(sizeof(char*)*NUMTHREADS);
	int pid[NUMTHREADS];
	for(int i=0;i<NUMTHREADS;i++)
	{
		stack[i] = malloc(STACK_SIZE);
		threaddata[i].buf = malloc(len/NUMTHREADS);
		threaddata[i].cols = data.cols;
		threaddata[i].rows = data.rows;
		threaddata[i].maxval = data.maxval;
		threaddata[i].moveX = data.moveX;
		threaddata[i].moveY = data.moveY;
	}
	while(it<NUMITS)
	{
		it++;
		data.zoomx*=2;
		data.zoomy*=2;

		for(int i=0;i<NUMTHREADS;i++)
		{



			threaddata[i].zoomx = data.zoomx;
			threaddata[i].zoomy = data.zoomy;
	
			threaddata[i].miny = (data.rows/NUMTHREADS)*i;
			threaddata[i].maxy = threaddata[i].miny + (data.rows/NUMTHREADS);

			pid[i] = clone(domandelbrot, stack[i] + STACK_SIZE, CLONE_VM | SIGCHLD, &threaddata[i]);
		}
		for(int i=0;i<NUMTHREADS;i++)
		{
			int status;
			waitpid(pid[i], &status, 0);
			memcpy(buf+(len/NUMTHREADS*i)/4, threaddata[i].buf, len/NUMTHREADS);
		}
	}
	for(int i=0;i<NUMTHREADS;i++)
	{
		free(stack[i]);
		free(threaddata[i].buf);
	}
	return 0;
}
