#ifndef __DEPLOY__
#define __DEPLOY__

#include "lib_io.h"
#include<string>

struct pathinf
{
	int path[10000][1000];
	int path_num = 0;
	int path_w[10000];
	int path_length[10000];
};
struct User_inf
{
	int path[1][1000];//ÿһ��·��·���洢����
	int path_length[1];//ÿһ��·�ĳ���
	int path_w[1];//ÿһ��·�����ô���
	int path_num = 0;//·������
	int server[1];//ÿһ��·��Ӧ�ķ�����
	int point_cost[1000];//����ʼ�㵽������Ļ���
};
//��������ڵ������Ϣ
struct server_inf
{
	int server[500];
	int server_num = 0;
};
//ÿ�������ѽڵ����ͳ��
extern int*interpoint_num;

//Ѱ������·
void Find_mainroute(User_inf userinf[], int*interpoint_num, int num_point, int num_user, int server_cost, int **matrix_w, int**matrix_p, int*user_w, int*user);

//Ѱ�ҷ���ڵ�
void deploy_server(int num_point, int num_user, User_inf userinf[], int*interpoint_num, int*user_w, int*user, server_inf &server);

//�õ����
bool Get_result(User_inf userinf[], int num_point, int num_user, int**real_w, char*filename);

#endif
