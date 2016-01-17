#include <forward_list>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

double c0 = 3e1;
double e0 = 8.854e-12;
double u0 = 1.2566e-6;



double dx=0.01;

double dt=.1;

double cons=.7;

double* un = NULL;
double* up = NULL;
double* u = NULL;
bool* bd = NULL;
uint32_t* pixels = NULL;

int width, height;

#define I(i,j) ((i) + width*(j))

struct hit {
	int i, j;
	int time;
};

double dmin, dmax;

std::forward_list<hit> hits;

extern "C" {

	void resize(int newheight, int newwidth)
	{
		height = newheight;
		width = newwidth;
		delete[] un;
		delete[] up;
		delete[] bd;

		un = new double[height*width];
		u = new double[height*width];
		up = new double[height*width];
		bd = new bool[height*width];
		pixels = new uint32_t[height*width];

		for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			bd[I(i,j)] = false;
			un[I(i,j)] = 0; 
			up[I(i,j)] = 0; 
			u[I(i,j)] = 0; 
		}


		for (int i = 0; i < width; i++)
		{
			bd[I(i,0)] = true;
			bd[I(i, height-1)] = true;
		}
		for (int j = 0; j < height; j++)
		{
			bd[I(0,j)] = true;
			bd[I(width-1,j)] = true;
		}

		hits.clear();
	}

	void setbd(int i, int j)
	{
		bd[I(i,j)] = true;
	}

	void addhit(int i, int j)
	{
		hit h = {};
		h.i = i;
		h.j = j;
		hits.push_front(h);
	}
	int status = 0;
	int return1()
	{
		return status;
	}
	double* getu()
	{
		return u;
	}
	double getMin()
	{
		return dmin;
	}
	double getMax()
	{
		return dmax;
	}

	uint32_t* step()
	{
		dmin = dmax = u[0];
		for(int j = 1; j < height-1; j++)
		{
			for (int i = 1; i < width-1; i++)
			{
				//final rows, cols are in db no no out-of-bounds access
				/*if (i == 100 && j == 100)
					printf("%f %f +.49(%f %f - %f %f %f)\n", un[I(i,j)], up[I(i,j)], u[I(i+1,j)], u[I(i-1,j)], u[I(i,j)], u[I(i,j+1)], u[I(i,j-1)] ); */
				if (!bd[I(i,j)])
					un[I(i,j)] = 2*u[I(i,j)]-up[I(i,j)]+cons*cons*(u[I(i+1,j)]+u[I(i-1,j)]-4*u[I(i,j)]+u[I(i,j+1)]+u[I(i,j-1)] );
				else
					un[I(i,j)] = 0;
				dmax = fmax(dmax, un[I(i,j)]);
				dmin = fmin(dmax, un[I(i,j)]);
			}
		}
		printf("Prior to applying touch, min=%f, max=%f\n", dmin, dmax);

		for(int j = 0; j < height; j++)
			for (int i = 0; i < width; i++)
			{
				pixels[I(i,j)] = (255 << 24) | ((int)(un[I(i,j)]/dmax)*255)<<8;
			}
		pixels[0] = 0xDEADBEEF;

		double* newun = up;
		up = u;
		u = un;
		un = newun;

		status = 2;

		while(hits.begin() != hits.end() && hits.begin()->time >= 10)
			hits.pop_front();
		for(std::forward_list<hit>::iterator it = hits.begin(); it != hits.end(); ++it)
		{
			int r = 10;
			for(int j = -r; j <= r; j++)
			for(int i = -r; i <= r; i++)
			{
				int sqnorm = i*i+j*j;
				int oi = it->i + i;
				int oj = it->j + j;
				if (oi >= 0 && oi < width && oj >= 0 && oj < height && !bd[I(oi, oj)] &&
						sqnorm <= r*r)
				{
					u[I(oi, oj)] += (10-sqnorm/(4.0*r))*cos(0.31/2*it->time);
				}
			}
			it->time++;
		}

		return pixels;
	}


}
/*
int main()
{
	resize(200,200);
	addhit(40,40);
	for (int i = 0; i < 40; i++)
	{
		char filenamebuf[128];
		sprintf(filenamebuf, "out/%d", i);
		std::filebuf fb;
		fb.open (filenamebuf,std::ios::out);
		std::ostream os(&fb);

		for (int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				if (j > 0) os << ",";
				os << u[I(i,j)];
			}
			os << std::endl;
		}
		fb.close();
		step();

	}
}*/
