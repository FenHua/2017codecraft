#include"mincost.h"
#include"lib_io.h"
#include"stdio.h"
#include"iostream"
#include"cstring"
#define MAX_Band 2000
using namespace std;

int T;
int S;
MCMF mincost;

int GetData(int line_num, int num_user, int num_point, int num_edge, char*topo[],int **matrix_w,int**matrix_p, int*user_w, int*user)
{
	T = num_point + num_user;
	S = num_point + num_user + 1;
	mincost.init(num_point + num_user + 2,S,T);
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
		mincost.addEdge(x, y, w, p);
		mincost.addEdge(y, x, w, p);
		matrix_w[x][y] = w;
		matrix_w[y][x] = w;//无向图，全双工通信
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
		mincost.addEdge(y, x + num_point, MAX_Band, 0);
		mincost.addEdge(x + num_point,y,MAX_Band, 0);
		mincost.addEdge(x + num_point, T, w, 0);
	}
	return 0;
}

MCMF target(server_inf server, int *user, int *user_w, int num_user,int num_point,int*cost)
{
	MCMF temp;
	temp = mincost;
	int totalflow = 0;
	int min_cost = 0;
	//添加超源，其与服务器节点相连
	//可能更改的数据集建立副本
	for (int i = 0; i < server.server_num; i++)
	{
		temp.addEdge(S, server.server[i], MAX_Band, 0);
	}
	for (int j = 0; j < num_user; j++)
	{
		totalflow = totalflow+ user_w[j];
	}
	min_cost = temp.MinCost(S,T,totalflow);
	cost[0] = min_cost;
	cost[1] = totalflow;
	return temp;
}