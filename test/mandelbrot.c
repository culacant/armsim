#include <stdio.h>
#include <ppm.h>

int main(void) 
{
	const int cols = 640*10;
	const int rows = 640*10;
	const pixval maxval = 255;

	pixel **out =  ppm_allocarray(cols,rows);
	double pr, pi;
	double newRe;
	double newIm;
	double oldRe;
	double oldIm;

	double zoomx = 3;
	double zoomy = 3;
	double moveX = -1.f;
	double moveY = 0;

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
				PPM_ASSIGN(out[y][x],maxval-(int)newRe, maxval-(int)newIm,i%255);
			else
				PPM_ASSIGN(out[y][x],(int)oldRe, (int)oldIm,255-(i%255));
		}
	}
	FILE *fd = fopen("mandelbrot.ppm", "wb");
	ppm_writeppm(fd, out, cols, rows, (pixval)255, 0);
	fclose(fd);
	return 0;
}
