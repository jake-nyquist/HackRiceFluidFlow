#include <forward_list>
#include <cmath>
#include <cstdio>
#include <cstdlib>

double c0 = 3e1;
double e0 = 8.854e-12;
double u0 = 1.2566e-6;

int size; 


double dx=0.001;

double dt=dx/(c0);

double cons=c0*dt/dx;

double* un = NULL;
double* up = NULL;
double* u = NULL;
bool* bd = NULL;

#define I(i,j) (size*(i) + (j))

struct hit {
	int i, j;
	int time;
};

std::forward_list<hit> hits;

extern "C" {

	void resize(int newSize)
	{
		size = newSize;
		delete[] un;
		delete[] up;
		delete[] u;
		delete[] bd;

		un = new double[size*size];
		up = new double[size*size];
		u = new double[size*size];
		bd = new bool[size*size];


		for (int i = 0; i < size; i++)
		{
			bd[I(0,i)] = true;
			bd[I(i,0)] = true;
			bd[I(size-1,i)] = true;
			bd[I(size-1,0)] = true;
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

	double* step()
	{
		status = 1;
		for (int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
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
		for(std::forward_list<hit>::iterator i = hits.begin(); i != hits.end(); ++i)
		{
			u[I(i->i, i->j)] = sin(0.32*i->time);
			i->time++;
		}
		return u;
	}


}
/*
int main()
{
	resize(100);
	printf("%x\n", step());
}
*/
