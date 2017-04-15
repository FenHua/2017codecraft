#include"sa.h"
#include"mincost.h"
#include<stack>
#include<time.h>
#include<iostream>
//���º깩ģ���˻�ʹ��
#define Term     1000    //��ʼ�¶�  
#define EPS   1    //��ֹ�¶�  
#define DELTA 0.2    //�¶�˥����  
#define LIMIT 10   //����ѡ������  
#define OLOOP 10    //��ѭ������  
using namespace std;

inline bool findroutes(int start, int end, int num_point, int totalcost, int**w, pathinf&path)
{
	path.path_num = 0;//���¼�����
	//�������
	stack<int>q;//����һ�����д洢���нڵ�
	bool*visited = new bool[num_point];// ����һ�����飬�洢���ʹ��ĵ�
	bool**v_edge = new bool*[num_point];	// ����һ�����㣬�Ƿ��߹����֧�ı�־λ
	for (int i = 0; i <num_point; i++)
		v_edge[i] = new bool[num_point];
	for (int i = 0; i < num_point; i++)
	{
		visited[i] = false;
		for (int j = 0; j < num_point; j++)
			v_edge[i][j] = false;
	}
	q.push(start);// �����ѽڵ�point��ջ
	visited[start] = true;
	int path_index = 0;//��¼���ѽڵ�point��·����
	int min_w = 2001;//��¼��ǰ·������С����
	while (!q.empty())
	{
		//����
		int i = q.top();
		int j = 0;
		while (j <num_point)
		{
			if ((w[i][j]>0) && (2001>w[i][j]) && (!visited[j]) && (!v_edge[i][j]))
			{
				visited[j] = true;
				v_edge[i][j] = true;// ���ñ�־λΪ:�ѷ���
				q.push(j);// ��jѹ��ջ
				break;
			}
			else
			{
				j++;
			}
		}
		// ���û��·���ߵ�ʱ�򣬻����ߵ��������ʱ��Ҫ��ջ
		if (j == num_point || q.top() == end)
		{
			if (q.top() ==end)
			{
				//�����յ�Ҫ��
				int num = q.size();
				for (int z = num - 1; z >= 0; z--)
				{
					path.path[path.path_num][z] = q.top();
					if (z > 1)
					{
						int toppoint = q.top();
						q.pop();// ������ǰԪ�ز�����־λ��λ
						visited[toppoint] = false;
						for (int k = 0; k <num_point; k++)
						{
							v_edge[k][toppoint] = false;
						}
					}
					else
					{
						q.pop();
					}
				}
				for (int z = 0; z <3; z++)
				{
					q.push(path.path[path.path_num][z]);
				}
				for (int z = 0; z+1<num; z++)
				{
					if (w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] < min_w)
						min_w = w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]];
				}
				if (min_w >= totalcost)
				{
					for (int z = 0; z+1<num; z++)
					{
						w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] = w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] - totalcost;
					}
					path.path_length[path.path_num] = num;
					path.path_w[path.path_num] = totalcost;
					path.path_num++;
					return true;
				}
				else
				{
					for (int z = 0; z+1<num; z++)
					{
						w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] = w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] - min_w;
					}
					path.path_w[path.path_num] = min_w;
					path.path_length[path.path_num] = num;
					path.path_num++;
					totalcost = totalcost - min_w;
					min_w = 101;//��¼��ǰ·������С����
				}
			}
			int toppoint = q.top();
			q.pop();// ������ǰԪ�ز�����־λ��λ
			visited[toppoint] = false;
			for (int k = 0; k <num_point; k++)
			{
				v_edge[toppoint][k] = false;
			}
		}
	}
	return false;
}

bool findroute_s(int num_user, int num_point, int N_s_cost, pathinf&path,MCMF&temp)
{
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
	for (int i = 0; i < temp.edges.size(); i++)
	{
		if (temp.edges[i].flow>0)
		{
			w[temp.edges[i].from][temp.edges[i].to] = temp.edges[i].flow;
		}
		
	}
	bool flag = findroutes(temp.s, temp.t,(num_point + num_user + 2), N_s_cost, w, path);
	return flag;
}


bool WResult(int num_point,pathinf&path, char*filename)
{
	int net_routes = path.path_num;
	string  topo_file;
	char temp[10];
	sprintf(temp, "%d", net_routes);//���ڴ���
	topo_file.append(temp);
	topo_file.append("\n");
	for (int t = 0; t < path.path_num; t++)
	{
		topo_file.append("\n");
		for (int f = 1; f < path.path_length[t] - 2; f++)
		{
			sprintf(temp, "%d ", path.path[t][f]);
			topo_file.append(temp);
		}
		sprintf(temp, "%d ", path.path[t][(path.path_length[t] - 2)]-num_point);
		topo_file.append(temp);//������ѽڵ�
		sprintf(temp, "%d", path.path_w[t]);
		topo_file.append(temp);
	}
	const char *result = topo_file.c_str();
	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	write_result(result, filename);
	return true;
}


void SA(int server_cost, int num_user, int num_point, int*user, int*user_w,server_inf &server,pathinf&path, char*filename)
{
	MCMF Mflow;
	int *cost=new int[2];
	srand(time(NULL));
	double t = Term;
	Mflow=target(server, user, user_w, num_user,num_point,cost);
	int curCost = cost[0]+ (server.server_num)*server_cost;
	int newCost = 0;
	int out_loop = 0;//��ѭ������
	int Probability_limit = 0;
	int count = 0;
	while (count<10)
	{
		int index = rand() % (server.server_num);//������ȥindex�㴦������
		for (int f = index; f < server.server_num; f++)
		{
			server.server[f] = server.server[f + 1];
		}
		server.server_num--;//�ܵķ���������һ
		Mflow =target(server, user, user_w, num_user, num_point, cost);
		newCost =cost[0] + (server.server_num)*server_cost;
		double delta_E = newCost - curCost;//���۲�
		if (delta_E < 0)   //����ҵ�����ֵ��ֱ�Ӹ���  
		{
			curCost = newCost;
			bool flag=findroute_s(num_user, num_point,cost[1],path,Mflow);
			if (flag)
			{
				WResult(num_point, path, filename);
			}
			out_loop = 0;//��ѭ������Ϊ��
			Probability_limit = 0;
		}
		else
		{
			break;
		}
		count++;
		/*
		else
		{
			double rand_num = rand() / (RAND_MAX + 1.0);
			if ((exp(delta_E / t) > rand_num) && (exp(delta_E / t) < 1))
			{
				//����ҵ��ȵ�ǰ����Ľ⣬��һ�����ʽ��ܸý⣬����������ʻ�Խ��ԽС 
				curCost = newCost;
				WResult(num_point,path, filename);
			}
			Probability_limit++;
			if (Probability_limit>LIMIT)
			{
				//���ڸ���ѡ�����޾�ֹͣ
				out_loop++;
				break;
			}
		}
		if ((out_loop > OLOOP) || (t < EPS))
		{
			break;
		}
		t *= DELTA;
		*/
	}
}