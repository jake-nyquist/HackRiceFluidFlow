#include <forward_list>
#include <cmath>
#include <cstdio>
#include <cstdlib>

double c0 = 3e1;
double e0 = 8.854e-12;
double u0 = 1.2566e-6;



double dx=0.01;

double dt=dx/(c0);

double cons=.1;

double* un = NULL;
double* up = NULL;
double* u = NULL;
bool* bd = NULL;

int width, height;

#define I(i,j) ((i) + width*(j))

struct hit {
	int i, j;
	int time;
};

std::forward_list<hit> hits;

extern "C" {

	void resize(int newheight, int newwidth)
	{
		height = newheight;
		width = newwidth;
		delete[] un;
		delete[] up;
		delete[] u;
		delete[] bd;

		un = new double[height*width];
		up = new double[height*width];
		u = new double[height*width];
		bd = new bool[height*width];

		for (int i = 0; i < width; i++)
		for (int j = 0; i < height; i++)
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
		if (bd[I(i,j)])
			return;
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

	double* step()
	{
		for (int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				//final rows, cols are in db no no out-of-bounds access
				if (!bd[I(i,j)])
					un[I(i,j)] = 2*un[I(i,j)]-up[I(i,j)]+cons*cons*(u[I(i+1,j)]+u[I(i-1,j)]-4*u[I(i,j)]+u[I(i,j+1)]+u[I(i,j-1)] );
				else
					un[I(i,j)] = 0;
			}
		}

		up = u;
		u = un;
		status = 2;

		while(hits.begin() != hits.end() && hits.begin()->time >= 10)
			hits.pop_front();
		for(std::forward_list<hit>::iterator it = hits.begin(); it != hits.end(); ++it)
		{
			int r = 20;
			for(int i = -r; i <= r; i++)
			for(int j = -r; j <= r; j++)
			{
				int sqnorm = i*i+j*j;
				if (sqnorm <= r*r+r)
				{
					u[I(it->i + i, it->j+j)] += (10-sqnorm/(3*r))*sin(0.32*it->time);
				}
			}
			it->time++;
		}

		return u;
	}


}

/*
int main()
{
	resize(1000);
	printf("%x\n", step());
	printf("%x\n", step());
	addhit(20,20);
	for (int i = 0; i < 40; i++)
		printf("%x\n", step());
}
*/
