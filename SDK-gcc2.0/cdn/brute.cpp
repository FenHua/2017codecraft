#include"brute.h"
#include"lib_io.h"
#include"mincost.h"
#include<stack>
#include<time.h>
#include<math.h>
#include<iostream>
//���º깩ģ���˻�ʹ��
#define Term     2000    //��ʼ�¶�  
#define EPS   1e-8    //��ֹ�¶�  
#define LIMIT 500   //����ѡ������  
#define OLOOP 100    //��ѭ������  
using namespace std;
//DFS
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
			if (q.top() == end)
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
				for (int z = 0; z + 1<num; z++)
				{
					if (w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]] < min_w)
						min_w = w[path.path[path.path_num][z]][path.path[path.path_num][z + 1]];
				}
				if (min_w >= totalcost)
				{
					for (int z = 0; z + 1<num; z++)
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
					for (int z = 0; z + 1<num; z++)
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
//������ȣ���ջʽ��·
bool findroute_s(int num_user, int num_point, int N_s_cost, pathinf&path, MCMF&temp)
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
	bool flag = findroutes(temp.s, temp.t, (num_point + num_user + 2), N_s_cost, w, path);
	return flag;
}
//д���
bool WResult(int num_point, pathinf&path, char*filename)
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
		sprintf(temp, "%d ", path.path[t][(path.path_length[t] - 2)] - num_point);
		topo_file.append(temp);//������ѽڵ�
		sprintf(temp, "%d", path.path_w[t]);
		topo_file.append(temp);
	}
	const char *result = topo_file.c_str();
	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	write_result(result, filename);
	return true;
}
//����ȥ�����������
inline void rand_server(server_inf&server, int*user, int*user_w, int num_user, int num_point, int*cost, double percent)
{
	int*temp_cost = new int[2];
	server_inf new_server;
	for (int i = 0; i < 10; i++)
	{
		new_server = server;
		server_inf temp_server;
		//����
		int*sort_index = new int[server.server_num];
		int *overlap = new int[server.server_num];
		for (int i = 0; i <server.server_num; i++)
		{
			overlap[i] = rate_flow[server.server[i]];
		}
		for (int i = 0; i <server.server_num; i++)
		{
			int max = 0;
			sort_index[i] = i;
			for (int j = 0; j <server.server_num; j++)
			{
				if (max <= overlap[j])
				{
					max = overlap[j];
					sort_index[i] = j;
				}
			}
			overlap[sort_index[i]] = -1;
		}
		//������
		for (int j = 0; j <int(server.server_num*percent); j++)
		{
			temp_server.server[j] = server.server[sort_index[j]];
		}
		temp_server.server_num = server.server_num*percent;
		server = temp_server;
		target(server, user, user_w, num_user, num_point, cost);
		if (cost[0] < INF)
		{
			new_server = server;
			temp_cost[0] = cost[0];
			temp_cost[1] = cost[1];
		}
		else
		{
			server = new_server;
			cost[0] = temp_cost[0];
			cost[1] = temp_cost[1];
			break;
		}
	}
}

void BruteForce(clock_t starttime,int server_cost, int num_user, int num_point, int*user, int*user_w, server_inf &server, pathinf&path, char*filename)
{
	MCMF Mflow;
	int *cost = new int[2];
	int newCost;
	clock_t endtime;
	Mflow = target(server, user, user_w, num_user, num_point, cost);
	int oldCost= cost[0] + (server.server_num)*server_cost;
	cout << "primary cost:" << oldCost << endl;
	int B = server.server_num;
	server_inf temp_server;
	for (int i = 0; i < B; i++)
	{
		temp_server.server_num = 0;
		int temp = server.server[i];
		server.server[i] = INF;
		for (int j = 0; j < B; j++)
		{
			if (server.server[j] != INF)
			{
				temp_server.server[temp_server.server_num] = server.server[j];
				temp_server.server_num++;
			}
		}
		Mflow=target(temp_server, user, user_w, num_user, num_point, cost);
		newCost = cost[0] + (temp_server.server_num)*server_cost;
		if (newCost>oldCost)
		{
			server.server[i] = temp;
		}
		else
		{
			cout << newCost << endl;
			bool flag = findroute_s(num_user, num_point, cost[1], path, Mflow);
			if (flag)
			{
				WResult(num_point, path, filename);
				oldCost= newCost;
			}
			endtime = clock();
			if (((endtime - starttime) / CLOCKS_PER_SEC) >80)
			{
				break;
			}
		}
	}
}

void SA(int server_cost, int num_user, int num_point, int*user, int*user_w, server_inf &server, pathinf&path, char*filename)
{
	MCMF Mflow;
	int *cost = new int[2];
	srand(time(NULL));
	double t = Term;
	double delta;//�¶�˥��ϵ��
	//�жϹ�ģ��ȷ������
	if (num_point >= 800)
		delta = 0.75;
	else if ((800 > num_point) && (num_point >= 500))
		delta = 0.85;
	else if ((500 > num_point) && (num_point> 250))
		delta = 0.95;
	else
		delta = 0.98;
	//��ȥ������ܵķ�������ָ������
	rand_server(server, user, user_w, num_user, num_point, cost, delta);
	//��дһ�ν��
	Mflow = target(server, user, user_w, num_user, num_point, cost);
	bool flag = findroute_s(num_user, num_point, cost[1], path, Mflow);
	if (flag)
	{
		WResult(num_point, path, filename);
	}
	//������һ����С��
	int curCost = cost[0] + (server.server_num)*server_cost;
	int newCost = 0;
	int out_loop = 0;//��ѭ������
	int count = 0;
	int Probability_limit = 0;
	while (t>EPS)
	{
		int index = rand() % (server.server_num);//������ȥindex�㴦������
		int temp_sever = server.server[index];
		for (int f = index; f < server.server_num; f++)
		{
			server.server[f] = server.server[f + 1];
		}
		server.server_num--;//�ܵķ���������һ
		Mflow = target(server, user, user_w, num_user, num_point, cost);
		if (cost[0]>500000)
		{
			server.server_num++;
			server.server[server.server_num-1] = temp_sever;
			count++;
			if (count >0.2*(server.server_num))
			{
				break;
			}
			else
			{
				continue;
			}
		}
		count = 0;
		newCost = cost[0] + (server.server_num)*server_cost;
		double delta_E = newCost - curCost;//���۲�
		if (delta_E < 0)   //����ҵ�����ֵ��ֱ�Ӹ���  
		{
			curCost = newCost;
			cout << curCost << endl;
			bool flag = findroute_s(num_user, num_point, cost[1], path, Mflow);
			if (flag)
			{
				WResult(num_point, path, filename);
			}
			out_loop = 0;//��ѭ������Ϊ��
			Probability_limit = 0;
		}
		else
		{
			double rand_num = rand() / (RAND_MAX + 1.0);
			if ((exp(delta_E / t) > rand_num) && (exp(delta_E / t) < 1))
			{
				//����ҵ��ȵ�ǰ����Ľ⣬��һ�����ʽ��ܸý⣬����������ʻ�Խ��ԽС 
				curCost = newCost;
				WResult(num_point, path, filename);
			}
			Probability_limit++;
			if (Probability_limit>LIMIT)
			{
				//���ڸ���ѡ�����޾�ֹͣ
				cout << "limit limit" << endl;
				out_loop++;
				break;
			}
		}
		if ((out_loop > OLOOP) || (t < EPS))
		{
			if (out_loop > OLOOP)
				cout << "test" << endl;
			cout << "temprature limit" << endl;
			break;
		}
		t *= delta;
	}
}


