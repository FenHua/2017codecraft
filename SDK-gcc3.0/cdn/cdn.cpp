// cdn.cpp : �������̨Ӧ�ó������ڵ㡣
#include"brute.h"
#include "deploy.h"
#include"mincost.h"
#include"lib_io.h"
#include<time.h>
#include"iostream"
using namespace std;
//ȫ�ֱ���
int num_point = 0;
int num_edge = 0;
int num_user = 0;
int server_cost = 0;
int**matrix_w;
int *user_w;
int*user;
server_inf server;
int*rate_flow;
pathinf path;

void Initial_data(char*top_file)
{
	user_w = new int[num_user];
	user = new int[num_user];
	int line_num;
	char *topo[MAX_EDGE_NUM];
	line_num = read_file(topo, MAX_EDGE_NUM, top_file);
	char*Inf = strtok(topo[0], " ");
	num_point = atoi(Inf);
	Inf = strtok(NULL, " ");
	num_edge = atoi(Inf);
	Inf = strtok(NULL, " ");
	num_user = atoi(Inf);
	server_cost = atoi(topo[2]);
	//��ʼ������
	matrix_w = new int*[num_point];
	for (int i = 0; i < num_point; i++)
	{
		matrix_w[i] = new int[num_point];
	}
	//��ʼ�����������ڵ�û������ʱ�������ͼ۸񶨼�Ϊ101
	for (int i = 0; i < num_point; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			matrix_w[i][j] = 101;
		}
	}
	user_w = new int[num_user];
	user = new int[num_user];
	GetData(line_num, num_user, num_point, num_edge, topo, matrix_w,user_w, user);//��������
	//��¼����ڵ������
	rate_flow= new int[num_point];
	for (int i = 0; i < num_point; i++)
	{
		rate_flow[i] = 0;
	}

	release_buff(topo, line_num);
}

void Initial_solution(char*filename)
{
	Get_RFlow(rate_flow,matrix_w,num_point);
	initial_server(num_user,num_point,server_cost,user,user_w,matrix_w,server);
	initial_S(num_user,num_point,user,user_w,server,path,filename);
}

int main(int argc, char *argv[])
{
	clock_t starttime, endtime;
	starttime = clock();
	print_time("Begin");
	//char *topo_file = argv[1];
	char*topo_file = "C:/Users/YAN/Desktop/case/case.txt";
	//char*filename = argv[2];
	char*filename = "C:/Users/YAN/Desktop/case/result.txt";
	Initial_data(topo_file);
	Iteration(num_user, num_point, server_cost, user, user_w, matrix_w,path, filename);
	//Initial_solution(filename);//�õ����ںϲ��������õ��ĳ�ʼ��
	BruteForce(starttime,server_cost, num_user, num_point, user, user_w, server, path, filename);
	print_time("End");

	endtime = clock();
	cout << "The program has costed: " << (double)(endtime - starttime) / CLOCKS_PER_SEC << " seconds" << endl;
	return 0;
}
