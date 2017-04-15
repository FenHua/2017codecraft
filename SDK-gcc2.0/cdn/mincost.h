#ifndef __MINCOST__
#define __MINCOST__

#include<vector>
#include<queue>
#include"deploy.h"
#include<string.h>
const int pointmax = 5000; //�ܽ������ֵ
#define INF 1000000
using namespace std;

extern int T;
extern int S;

//�ߵ����ݽṹ
struct edge
{
	int from, to, cap, flow, cost;
};
//��С��������������ݽṹ
struct MCMF
{
	int n;//n��ʾ�ڵ����
	int m;//�ߵĸ���
	int s;//��Դ
	int t;//����
	vector<edge> edges; //���бߵĴ洢
	vector<int> G[pointmax];//�ڽӱ�G[i][j]��ʾ���i�ĵ�j������edge�����е����
	int inqueue[pointmax]; //��־λ�����ж����Ƿ��ڶ�����
	int dist[pointmax]; //Bellman-Ford�㷨��Դ�㵽����������С����
	int pre[pointmax]; //��һ����
	int a[pointmax]; //���ԸĽ�����
	//��ʼ������
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
	//������ݺ���
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

		m = edges.size();//����
		G[from].push_back(m - 2);
		G[to].push_back(m - 1);
	}
	//��С�������㷨BF
	bool bellManFord(int s, int t, int &flow, int &cost)
	{
		for (int i = 0; i < n; i++)
		{
			dist[i] = INF;//��ʼ��Դ���������ľ���
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
			//s->t ����ͨ
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
//�����������黯
extern int GetData(int line_num, int num_user, int num_point, int num_edge, char*topo[], int **matrix_w,int**matrix_p,int*user_w, int*user);

#endif