#ifndef __MINCOST__
#define __MINCOST__

#include<vector>
#include<queue>
#include"deploy.h"
#include<string.h>
const int pointmax = 5000; //总结点的最大值
#define INF 1000000
using namespace std;

extern int T;
extern int S;

//边的数据结构
struct edge
{
	int from, to, cap, flow, cost;
};
//最小费用最大流的数据结构
struct MCMF
{
	int n;//n表示节点个数
	int m;//边的个数
	int s;//超源
	int t;//超汇
	vector<edge> edges; //所有边的存储
	vector<int> G[pointmax];//邻接表，G[i][j]表示结点i的第j条边在edge数组中的序号
	int inqueue[pointmax]; //标志位用于判断你是否在队列中
	int dist[pointmax]; //Bellman-Ford算法，源点到各个汇点的最小费用
	int pre[pointmax]; //上一条弧
	int a[pointmax]; //可以改进的量
	//初始化函数
	void init(int n, int S, int T)
	{
		this->n = n;
		this->s = S;
		this->t = T;
		for (int i = 0; i < n; i++)
		{
			G[i].clear();
		}
		edges.clear();
	}
	//添加数据函数
	void addEdge(int from, int to, int cap, int cost)
	{
		edge ed;
		ed.from = from;
		ed.to = to;
		ed.cap = cap;
		ed.cost = cost;
		ed.flow = 0;
		edges.push_back(ed);
		ed.from = to;
		ed.to = from;
		ed.cap = 0;
		ed.cost = -cost;
		ed.flow = 0;
		edges.push_back(ed);

		m = edges.size();//边数
		G[from].push_back(m - 2);
		G[to].push_back(m - 1);
	}
	//最小费用流算法BF
	bool bellManFord(int s, int t, int &flow, int &cost)
	{
		for (int i = 0; i < n; i++)
		{
			dist[i] = INF;//初始化源到其它结点的距离
		}
		memset(inqueue, 0, sizeof(inqueue));
		dist[s] = 0;
		inqueue[s] = 1;
		pre[s] = 0;
		a[s] = INF;
		queue<int> q;
		q.push(s);
		while (!q.empty())
		{
			int u = q.front();
			q.pop();
			inqueue[u] = 0;
			for (int i = 0; i <(G[u].size()); i++)
			{
				edge& e = edges[G[u][i]];
				if ((e.cap > e.flow) && (dist[e.to] > dist[u] + e.cost))
				{
					dist[e.to] = dist[u] + e.cost;
					pre[e.to] = G[u][i];
					a[e.to] = min(a[u], e.cap - e.flow);
					if (!inqueue[e.to])
					{
						q.push(e.to);
						inqueue[e.to] = 1;
					}
				}
			}
		}
		if (dist[t] == INF)
			//s->t 不连通
			return false;
		flow += a[t];
		cost += dist[t] * a[t];
		int u = t;
		while (u != s)
		{
			edges[pre[u]].flow += a[t];
			edges[pre[u] ^ 1].flow -= a[t];
			u = edges[pre[u]].from;
		}
		return true;
	}
	int MinCost(int s, int t, int totalflow)
	{
		int flow = 0, cost = 0;
		while (bellManFord(s, t, flow, cost));
		if (flow >= totalflow)
		{
			totalflow = flow;
			return cost;
		}
		else
		{
			return INF;
		}
	}
};

extern MCMF mincost;
//将数据流数组化
extern int GetData(int line_num, int num_user, int num_point, int num_edge, char*topo[], int **matrix_w,int**matrix_p,int*user_w, int*user);

#endif