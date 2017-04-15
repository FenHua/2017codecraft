// cdn.cpp : �������̨Ӧ�ó������ڵ㡣
#include"sa.h"
#include "lib_io.h"
#include"iostream"
#include "deploy.h"
#include"mincost.h"
using namespace std;
//ȫ�ֱ���
int num_point = 0;
int num_edge = 0;
int num_user = 0;
int server_cost = 0;
int**matrix_w;
int**matrix_p;
int *user_w;
int*user;
server_inf server;
int*interpoint_num;
User_inf *userinf;//���ѽڵ���Ϣ�ṹ������
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
	matrix_p = new int*[num_point];
	for (int i = 0; i < num_point; i++)
	{
		matrix_w[i] = new int[num_point];
		matrix_p[i] = new int[num_point];
	}
	//��ʼ�����������ڵ�û������ʱ�������ͼ۸񶨼�Ϊ101
	for (int i = 0; i < num_point; i++)
	{
		for (int j = 0; j < num_point; j++)
		{
			matrix_p[i][j] = 101;
			matrix_w[i][j] = 101;
		}
	}
	user_w = new int[num_user];
	user = new int[num_user];
	GetData(line_num, num_user, num_point, num_edge, topo,matrix_w,matrix_p, user_w, user);//��������
	//��¼����ڵ������
	interpoint_num = new int[num_point];
	for (int i = 0; i < num_point; i++)
	{
		interpoint_num[i] = 0;
	}
	
	release_buff(topo, line_num);
}

void Initial_solution(char*filename)
{
	userinf = new User_inf[num_user];
	//��ʼ�� userinf���飬��������¼ÿ�����ѽڵ������·���ʹ���
	for (int i = 0; i < num_user; i++)//��ʼ�����ѽڵ���Ϣ
	{
		memset(&userinf[i].path, 0, sizeof(userinf[i].path));
		memset(&userinf[i].path_length, 0, sizeof(userinf[i].path_length));
		memset(&userinf[i].path_w, 0, sizeof(userinf[i].path_w));
		memset(&userinf[i].point_cost, 0, sizeof(userinf[i].point_cost));
		memset(&userinf[i].server, 0, sizeof(userinf[i].server));
	}
	Find_mainroute(userinf, interpoint_num, num_point, num_user, server_cost, matrix_w, matrix_p, user_w, user);
	//��ʼ��server���飬��¼������λ��
	deploy_server(num_point, num_user, userinf, interpoint_num, user_w, user, server);
	Get_result(userinf, num_point, num_user,matrix_w,filename);
}


int main(int argc, char *argv[])
{
	print_time("Begin");
	char *topo_file = argv[1];
	//char*topo_file = "C:/Users/YAN/Desktop/case/case8.txt";
	char*filename = argv[2];
	//char*filename = "C:/Users/YAN/Desktop/case/result.txt";
	Initial_data(topo_file);
	Initial_solution(filename);//�õ����ںϲ��������õ��ĳ�ʼ��
	SA(server_cost, num_user, num_point, user, user_w,server, path, filename);
	print_time("End");
	return 0;
}
