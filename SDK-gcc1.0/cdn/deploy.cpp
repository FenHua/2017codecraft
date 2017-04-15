// deploy.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "deploy.h"
#include <stdio.h>
#include"iostream"
#include<string>
using namespace std;
//Ѱ������·
void Find_mainroute(User_inf*userinf, int*interpoint_num, int num_point, int num_user, int sever_cost, int **real_w, int**matrix_p, int*user_w, int*user)
{
	int**FB_w = new int*[num_point];
	for (int i = 0; i < num_point; i++)
	{
		FB_w[i] = new int[num_point];
	}
	//��ʼ�����������ڵ�û������ʱ�������ͼ۸񶨼�Ϊ101
	for (int i = 0; i < num_point; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			FB_w[i][j] = real_w[i][j];
		}
	}
	for (int i = 0; i < num_user; i++)
	{
		//��Ҫ�ĳ�ʼ��
		int x = user[i];
		int w = user_w[i];
		int *visit = new int[num_point];
		for (int t = 0; t < num_point; t++)
		{
			visit[t] = 0;//ȫ��û�з��ʹ�
		}
		int path_num = 1;
		int path_length = 0;
		userinf[i].path[path_num - 1][path_length] = x;
		userinf[i].path_length[path_num - 1] = path_length + 1;
		userinf[i].path_w[path_num - 1] = w;
		userinf[i].path_num = path_num;
		for (int f = 0; f < num_point; f++)
		{
			userinf[i].point_cost[f] = 0;
		}
		for (int j = 0; j <num_point; j++)
		{
			userinf[i].point_cost[j] = 0;
		}
		visit[x] = 1;
		interpoint_num[x] = 1;

		//��ʼ����
		while (1)
		{
			int best_path = x;
			int temp_p = 101;
			for (int j = 0; j < num_point; j++)
			{
				if ((FB_w[x][j] != 101)&(FB_w[x][j] >= w)&(visit[j] == 0))
				{
					if (matrix_p[x][j] < temp_p)
					{
						temp_p = matrix_p[x][j];
						best_path = j;
					}
				}
			}
			if (best_path == x)
			{
				break;
			}
			else
			{
				interpoint_num[best_path] = interpoint_num[best_path] + 1;
				FB_w[x][best_path] = FB_w[x][best_path] - w;
				path_length = path_length + 1;
				userinf[i].path_length[path_num - 1] = path_length;
				userinf[i].path[path_num - 1][path_length] = best_path;
				userinf[i].point_cost[best_path] = userinf[i].point_cost[x]+matrix_p[x][best_path] * w;
				x = best_path;
				visit[best_path] = 1;
			}
			if (userinf[i].point_cost[best_path] >= sever_cost)
			{
				break;
			}
		}
	}
}
//Ѱ�Ҵ��Է���ڵ�
void deploy_server(int num_point, int num_user, User_inf*userinf, int*interpoint_num, int*user_w, int*user, server_inf &server)
{
	int*sort_index = new int[num_point];
	int *overlap = new int[num_point];
	for (int i = 0; i < num_point; i++)
	{
		overlap[i] = interpoint_num[i];
	}
	//first getting  index of sorting interpoints
	for (int i = 0; i < num_point; i++)
	{
		int max = overlap[i];
		sort_index[i] = i;
		for (int j = 0; j < num_point; j++)
		{
			if (max < overlap[j])
			{
				max = overlap[j];
				sort_index[i] = j;
			}
		}
		overlap[sort_index[i]] = -1;
	}
	// iterate and select sever point
	//��Ҫ�ĳ�ʼ������
	bool *user_visited = new bool[num_user];
	for (int j = 0; j < num_user; j++)
	{
		user_visited[j] = 0;
	}
	int unvisited_size = num_user;
	//�ж��Ƿ�Ϊ����ڵ㣬���Ҽ�¼
	int**array_cost = new int*[num_user];
	for (int i = 0; i < num_user; i++)
	{
		array_cost[i] = new int[num_point];
	}
	for (int i = 0; i < num_user; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			array_cost[i][j] = 0;
		}
	}
	for (int i = 0; i < num_point; i++)
	{
		int temp_index = sort_index[i];
		for (int j = 0; j < num_user; j++)
		{
			if ((userinf[j].point_cost[temp_index] != 0)&(user_visited[j] == 0))
			{
				array_cost[j][temp_index] = userinf[j].point_cost[temp_index];
				user_visited[j] = 1;
				unvisited_size--;
			}
			else if ((userinf[j].point_cost[temp_index] != 0)&(user_visited[j] == 1)&(array_cost[j][temp_index]<userinf[j].point_cost[temp_index]))
			{
				array_cost[j][temp_index] = userinf[j].point_cost[temp_index];
			}
		}
		if (unvisited_size == 0)
		{
			break;
		}
	}
	// �����©���ѽڵ�
	for (int j = 0; j < num_user; j++)
	{
		if (user_visited[j] == 0)
		{
			array_cost[j][user[j]] = user_w[j];//��ʱ�趨����Ϊ����Ϊ0
		}
	}
	//ѡ�������·
	for (int i = 0; i < num_user; i++)
	{
		int j;
		int index = 0;
		int overvisit = 0;
		int min_cost = 100000000;
		for (j = 0; j < num_point; j++)
		{
			if ((array_cost[i][j] != 0)&(array_cost[i][j] < min_cost)&(interpoint_num[j] >= overvisit + 1))
			{
				min_cost = array_cost[i][j];
				overvisit = interpoint_num[j];
				index = j;
			}
		}
		userinf[i].server[userinf[i].path_num - 1] = index;
		if (server.server_num == 0)
		{
			server.server[server.server_num] = index;
			server.server_num++;
		}
		else
		{
			bool s = false;
			for (int f = 0; f < server.server_num; f++)
			{
				if (index == server.server[f])
				{
					s = true;
				}
			}
			if (!s)
			{
				server.server[server.server_num] = index;
				server.server_num++;
			}
		}
	}
}
//�õ�����������
bool Get_result(User_inf*userinf, int num_point, int num_user, int**real_w, char*filename)
{
	int**FB_w = new int*[num_point];
	for (int i = 0; i < num_point; i++)
	{
		FB_w[i] = new int[num_point];
	}
	//��ʼ�����������ڵ�û������ʱ�������ͼ۸񶨼�Ϊ101
	for (int i = 0; i < num_point; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			FB_w[i][j] = real_w[i][j];
		}
	}
	int net_routes = 0;
	for (int i = 0; i<num_user; i++)
	{
		net_routes = net_routes + userinf[i].path_num;
	}
	string  topo_file;
	char temp[10];
	sprintf(temp, "%d", net_routes);//���ڴ���
	topo_file.append(temp);
	topo_file.append("\n");
	for (int t = 0; t < num_user; t++)
	{
		for (int j = 0; j < userinf[t].path_num; j++)
		{
			int X = userinf[t].server[j];
			int num = 0;
			while ((userinf[t].path[j][num] != X)&(num < num_point))
			{
				num++;
			}
			if (num != num_point)
			{
				topo_file.append("\n");
				for (int f = num; f >= 0; f--)
				{
					sprintf(temp, "%d ", userinf[t].path[j][f]);
					topo_file.append(temp);
				}
				sprintf(temp, "%d %d", t, userinf[t].path_w[j]);
				topo_file.append(temp);
			}
			else
			{
				return false;
			}
		}
	}
	const char *result = topo_file.c_str();
	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	write_result(result, filename);
	return true;
}

