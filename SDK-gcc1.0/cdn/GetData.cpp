#include"lib_io.h"
#include"stdio.h"
#include"iostream"
#include"cstring"
using namespace std;
int GetData(int line_num, int num_point, int num_edge, char*topo[], int **origin_w,int **matrix_w, int**matrix_p, int*user_w, int*user)
{
	int i;
	int x, y;
	int w, p;
	for (i = 4; i < 4 + num_edge; i++)
	{
		char*Inf = strtok(topo[i], " ");
		x = atoi(Inf);
		Inf = strtok(NULL, " ");
		y = atoi(Inf);
		Inf = strtok(NULL, " ");
		w = atoi(Inf);
		Inf = strtok(NULL, " ");
		p = atoi(Inf);
		matrix_w[x][y] = w;
		origin_w[x][y] = w;//副本
		matrix_w[y][x] = w;//无向图，全双工通信
		origin_w[y][x] = w;//副本
		matrix_p[x][y] = p;
		matrix_p[y][x] = p;//无向图
	}
	for (i = 5 + num_edge; i < line_num; i++)
	{
		char*Inf = strtok(topo[i], " ");
		x = atoi(Inf);
		Inf = strtok(NULL, " ");
		y = atoi(Inf);
		Inf = strtok(NULL, " ");
		w = atoi(Inf);
		user_w[x] = w;
		user[x] = y;
	}
	return 0;
}

void sort_p(int num_point, int**matrix_p, int**sort_p)
{
	int**FB_p = new int*[num_point];
	for (int i = 0; i < num_point; i++)
	{
		FB_p[i] = new int[num_point];
	}
	for (int i = 0; i < num_point; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			FB_p[i][j] = matrix_p[i][j];
		}
	}
	for (int i = 0; i < num_point; i++)
	{
		for (int f = 0; f < num_point; f++)
		{
			int min = 101;
			int index = 0;
			for (int j = 0; j < num_point; j++)
			{
				if ((FB_p[i][j]!=-1)&&(FB_p[i][j] <=min))
				{
					min =FB_p[i][j];
					index = j;
				}
			}
			sort_p[i][f] = index;
			FB_p[i][index] =-1;
		}
	}
}