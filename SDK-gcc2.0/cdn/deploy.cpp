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
inline void C_S_D(int num_user, int num_point, int*user, int*user_w, server_inf&server)
{
	int *cost = new int[2];
	int old_cost;
	int *temp_cost = new int[num_user];
	//case0:暴力
	for (int i = 0; i < num_user; i++)
	{
		server.server[i] = user[i];
	}
	server.server_num = num_user;
	old_cost = 0.5*INF;
	server_inf temp_server;
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
		temp_cost[i] = old_cost - cost[0];
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
		int min =INF;
		sort_index[i] = i;
		for (int j = 0; j <num_user; j++)
		{
			if (min >= overlap[j])
			{
				min= overlap[j];
				sort_index[i] = j;
			}
		}
		overlap[sort_index[i]] =INF;
	}
	//删点
	for (int i = 0; i < num_user; i++)
	{
		if (temp_cost[sort_index[i]] < 0)
			break;
		temp = server.server[sort_index[i]];
		server.server[sort_index[i]] = INF;
		temp_server.server_num = 0;
		for (int i = 0; i <server.server_num; i++)
		{
			if (server.server[i] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[i];
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
	server = temp_server;
}
//广度优先换点
inline void C_A_B(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w, int**matrix_p, server_inf&server)
{
	int *cost = new int[2];
	//step1:服务器流量排序
	int*sort_index = new int[server.server_num];
	int *overlap = new int[server.server_num];
	for (int i = 0; i <server.server_num; i++)
	{
		overlap[i] = rate_flow[server.server[i]];
	}
	for (int i = 0; i <server.server_num; i++)
	{
		int min = INF;
		sort_index[i] = i;
		for (int j = 0; j <server.server_num; j++)
		{
			if (overlap[j]<min)
			{
				min = overlap[j];
				sort_index[i] = j;
			}
		}
		overlap[sort_index[i]] = INF;
	}
	//step:2广度遍历节点
	target(server, user, user_w, num_user, num_point, cost);
	int old_cost = cost[0];
	int route_cost;
	for (int i = 0; i <server.server_num; i++)
	{
		route_cost = 0;
		int x = server.server[i];
		while (1)
		{
			int best_point = x;
			int temp_index;
			for (int j = 0; j < num_point; j++)
			{
				if ((matrix_w[x][j] != 101) && (route_cost<server_cost) && (rate_flow[j]>(0.5*rate_flow[x])))
				{
					temp_index = server.server[i];
					server.server[i] = j;
					target(server, user, user_w, num_user, num_point, cost);
					if (old_cost>cost[0])
					{
						best_point = j;
						if (old_cost - cost[0] >= server_cost)
						{
							break;
						}
						else
						{
							old_cost = cost[0];
						}
					}
					else
					{
						server.server[i] = temp_index;
					}
				}
			}
			if ((best_point == x) || (old_cost - cost[0] >= server_cost))
			{
				if (old_cost - cost[0] >= server_cost)
				{
					old_cost = cost[0];
				}
				break;
			}
			else
			{
				route_cost = route_cost + matrix_w[x][best_point] * matrix_p[x][best_point];
				if (route_cost > server_cost)
				{
					server.server[i] = x;
					break;
				}
				else
				{
					x = best_point;
					server.server[i] = x;
				}
				x = best_point;
			}
		}
	}
}
//暴力选取初始server
void initial_server(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w, int**matrix_p, server_inf&server)
{
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
		C_S_D(num_user, num_point, user, user_w, server);
		C_A_B(num_user, num_point, server_cost, user, user_w, matrix_w, matrix_p, server);
	}
}
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
	server = temp_server;
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
