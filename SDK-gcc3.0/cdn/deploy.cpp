// deploy.cpp : 定义控制台应用程序的入口点。
//

#include "deploy.h"
#include <stdio.h>
#include"brute.h"
#include"iostream"
using namespace std;

void Get_RFlow(int*rate_flow, int**matrix_w, int num_point)
{
	int temp;
	for (int i = 0; i < num_point; i++)
	{
		temp = 0;
		for (int j = 0; j < num_point; j++)
		{
			if ((101>matrix_w[i][j]) && (matrix_w[i][j]>0))
			{
				temp = temp + matrix_w[i][j];
			}
			rate_flow[i] = temp;
		}
	}
}
//消费直连，再删
inline void C_S_D(int num_user, int num_point, int server_cost,int*user, int*user_w, server_inf&server)
{
	/*
	for (int i = 0; i < num_user; i++)
	{
		server.server[i] = user[i];
	}
	server.server_num = num_user;
	//*/
	int *cost = new int[2];
	int *temp_cost = new int[num_user];
	server_inf temp_server;
	memset(temp_server.server, 0, sizeof(temp_server.server));
	int temp;
	//step2:删除直连点
	//先按贡献排序
	int num = server.server_num;
	for (int i = 0; i < num; i++)
	{
		temp_server.server_num = 0;
		temp = server.server[i];
		server.server[i] = INF;
		for (int j = 0; j < num; j++)
		{
			if (server.server[j] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[j];
				temp_server.server_num++;
			}
		}
		target(temp_server, user, user_w, num_user, num_point, cost);
		temp_cost[i] =cost[0];
		server.server[i] = temp;
	}
	//排序，再删点
	int*sort_index = new int[num_user];
	int *overlap = new int[num_user];
	for (int i = 0; i <num_user; i++)
	{
		overlap[i] = temp_cost[i];
	}
	for (int i = 0; i <num_user; i++)
	{
		int min=1000001;
		sort_index[i] = i;
		for (int j = 0; j <num_user; j++)
		{
			if (min>overlap[j])
			{
				min = overlap[j];
				sort_index[i] = j;
			}
		}
		overlap[sort_index[i]] = 1000001;
	}
	//删点
	for (int i = 0; i < num_user; i++)
	{
		if (temp_cost[sort_index[i]]>server_cost)
			break;
		temp = server.server[sort_index[i]];
		server.server[sort_index[i]] = INF;
		temp_server.server_num = 0;
		for (int j = 0; j <server.server_num; j++)
		{
			if (server.server[j] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[j];
				temp_server.server_num++;
			}
		}
		target(temp_server, user, user_w, num_user, num_point, cost);
		if (cost[0] == INF)
		{
			server.server[sort_index[i]] = temp;
		}
	}
	//整理服务器
	temp_server.server_num = 0;
	for (int i = 0; i <server.server_num; i++)
	{
		if (server.server[i] != INF)
		{
			temp_server.server[temp_server.server_num] = server.server[i];
			temp_server.server_num++;
		}
	}
	//server = temp_server;
	server.server_num = temp_server.server_num;
	memcpy(server.server, temp_server.server, sizeof(server.server));
}
//迭代换点
inline void EX(int num_user, int num_point, int server_cost, int*user, int*user_w,server_inf &server)
{
	MCMF Mflow;
	int*cost = new int[2];
	bool *state_server = new bool[num_point];
	int num = server.server_num;
	int**w = new int*[num_point + num_user + 2];
	for (int i = 0; i < num_point + num_user + 2; i++)
		w[i] = new int[num_point + num_user + 2];
	for (int i = 0; i <num_point + num_user + 2; i++)
	{
		for (int j = 0; j < num_point + num_user + 2; j++)
		{
			w[i][j] = 0;
		}
	}//流矩阵
	for (int j = 0; j < num_point; j++)
	{
		state_server[j] =false;
	}
	for (int j = 0; j <num; j++)
	{
		state_server[server.server[j]] =true;
	}
	Mflow = target(server, user, user_w, num_user, num_point, cost);
	int old_cost = cost[0];

	//换点
	int minflow = INF;//最小流量
	int index = 0;//最小流量点的下坐标
	int temp_server;
	for (int i = 0; i < num_point; i++)
	{
		if (state_server[i] == false)
		{
			server.server[server.server_num] = i;
			server.server_num++;
			Mflow = target(server, user, user_w, num_user, num_point, cost);
			FLOW(num_user, num_point, Mflow, w);
			for (int f = 0; f < num; f++)
			{
				if (minflow>w[S][server.server[f]])
				{
					minflow = w[S][server.server[f]];
					index = f;
				}
			}
			temp_server = server.server[index];
			server.server[index] = server.server[(server.server_num - 1)];
			server.server_num--;
			Mflow = target(server, user, user_w, num_user, num_point, cost);
			if (cost[0]>old_cost)
			{
				server.server[index] = temp_server;
			}
			else
			{
				old_cost = cost[0];
			}
			minflow = INF;
		}
	}//*/
}
//暴力选取初始server
void initial_server(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w,server_inf&server)
{
	C_S_D(num_user, num_point, server_cost, user, user_w, server);
	EX(num_user, num_point, server_cost, user, user_w, server);
	/*
	if (num_point > 600)
	{
		for (int i = 0; i < num_user; i++)
		{
			server.server[i] = user[i];
		}
		server.server_num = num_user;
	}
	else
	{
		C_S_D(num_user, num_point,server_cost,user, user_w, server);
		EX(num_user, num_point, server_cost, user, user_w, server);
	}
	*/
}

inline void test(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w, server_inf &server)
{
	MCMF Mflow;
	int *cost = new int[2];
	int *temp_cost = new int[num_user];
	server_inf temp_server;
	memset(&temp_server, 0, sizeof(server_inf));
	int temp;
	//step2:删除直连点
	//先按贡献排序
	int num = server.server_num;
	for (int i = 0; i < num; i++)
	{
		temp_server.server_num = 0;
		temp = server.server[i];
		server.server[i] = INF;
		for (int j = 0; j < num; j++)
		{
			if (server.server[j] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[j];
				temp_server.server_num++;
			}
		}
		target(temp_server, user, user_w, num_user, num_point, cost);
		temp_cost[i] = cost[0];
		server.server[i] = temp;
	}
	//排序，再删点
	int *sort_index = new int[num_user];
	int *overlap = new int[num_user];
	for (int i = 0; i <num_user; i++)
	{
		overlap[i] = temp_cost[i];
	}
	for (int i = 0; i <num_user; i++)
	{
		int min = 1000001;
		sort_index[i] = i;
		for (int j = 0; j <num_user; j++)
		{
			if (min>overlap[j])
			{
				min = overlap[j];
				sort_index[i] = j;
			}
		}
		overlap[sort_index[i]] = 1000001;
	}
	//删点
	for (int i = 0; i < num_user; i++)
	{
		if (temp_cost[sort_index[i]]>server_cost)
			break;
		temp = server.server[sort_index[i]];
		server.server[sort_index[i]] = INF;
		temp_server.server_num = 0;
		for (int j = 0; j <server.server_num; j++)
		{
			if (server.server[i] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[j];
				temp_server.server_num++;
			}
		}
		target(temp_server, user, user_w, num_user, num_point, cost);
		
		if (cost[0] == INF)
		{
			server.server[sort_index[i]] = temp;
		}
	}
	//整理服务器
	temp_server.server_num = 0;
	for (int i = 0; i <server.server_num; i++)
	{
		if (server.server[i] != INF)
		{
			temp_server.server[temp_server.server_num] = server.server[i];
			temp_server.server_num++;
		}
	}
	//server = temp_server;
	server.server_num = temp_server.server_num;
	memcpy(server.server, temp_server.server, sizeof(server.server));

	bool *state_server = new bool[num_point];
    num = server.server_num;
	int**w = new int*[num_point + num_user + 2];
	for (int i = 0; i < num_point + num_user + 2; i++)
		w[i] = new int[num_point + num_user + 2];
	for (int i = 0; i <num_point + num_user + 2; i++)
	{
		for (int j = 0; j < num_point + num_user + 2; j++)
		{
			w[i][j] = 0;
		}
	}
	//流矩阵
	for (int j = 0; j < num_point; j++)
	{
		state_server[j] = false;
	}
	for (int j = 0; j <num; j++)
	{
		state_server[server.server[j]] = true;
	}
	Mflow = target(server, user, user_w, num_user, num_point, cost);
	int old_cost = cost[0];
	//换点
	int minflow = INF;//最小流量
	int index;//最小流量点的下坐标
	int temp_s;
	for (int i = 0; i < num_point; i++)
	{
		if (state_server[i] == false)
		{
			server.server[server.server_num] = i;
			server.server_num++;
			Mflow = target(server, user, user_w, num_user, num_point, cost);
			FLOW(num_user, num_point, Mflow, w);
			for (int f = 0; f < num; f++)
			{
				if (minflow>w[S][server.server[f]])
				{
					minflow = w[S][server.server[f]];
					index = f;
				}
			}
			temp_s = server.server[index];
			server.server[index] = server.server[(server.server_num - 1)];
			server.server_num--;
			Mflow = target(server, user, user_w, num_user, num_point, cost);
			if (cost[0]>old_cost)
			{
				server.server[index] = temp_s;
			}
			else
			{
				old_cost = cost[0];
			}
			minflow = INF;
		}
	}
}
//test
void Iteration(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w,pathinf&path, char*filename)
{
	MCMF M;
	server_inf server;
	memset(server.server, 0, sizeof(server.server));
	int*cost = new int[2];
	int oldcost = 1;
	int newcost = 2;
	for (int i = 0; i < num_user; i++)
	{
		server.server[i] = user[i];
	}
	server.server_num = num_user;
	int count = 1;
	//test(num_user, num_point, server_cost, user, user_w, matrix_w, server);
	while (oldcost != newcost)
	{
		M = target(server, user, user_w, num_user, num_point, cost);
		oldcost = cost[0] + server.server_num*server_cost;
		initial_server(num_user, num_point, server_cost, user, user_w, matrix_w, server);
		M = target(server, user, user_w, num_user, num_point, cost);
		newcost = cost[0] + server.server_num*server_cost;
		cout << "cost: " << oldcost << endl;
		cout << "server:" << endl;
		for (int i = 0; i < server.server_num; i++)
		{
			cout << server.server[i] << " ";
		}
		cout << endl;
		count++;
	}
	cout << "The number of iteration is:" << count << endl;
}

//test





//得到初始解
void initial_S(int num_user, int num_point, int*user, int*user_w, server_inf&server, pathinf&path, char*filename)
{
	MCMF Mflow;
	int *cost = new int[2];
	//整理server
	server_inf temp_server;
	int num = server.server_num;
	for (int i =1; i < num; i++)
	{
		for (int j = i-1; j >= 0; j--)
		{
			if (server.server[i] == server.server[j])
			{
				server.server[i] = INF;
			}
		}
	}
	temp_server.server_num = 0;
	for (int i = 0; i < num; i++)
	{
		if (server.server[i] != INF)
		{
			temp_server.server[temp_server.server_num] = server.server[i];
			temp_server.server_num++;
		}
	}

	//server = temp_server;
	server.server_num = temp_server.server_num;
	memcpy(server.server, temp_server.server, sizeof(server.server));


	Mflow = target(server, user, user_w, num_user, num_point, cost);
	if (cost[0] < INF)
	{
		bool flag = findroute_s(num_user, num_point, cost[1], path, Mflow);
		if (flag)
		{
			WResult(num_point, path, filename);
		}
		return;
	}
}
