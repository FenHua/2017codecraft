#ifndef __DEPLOY__
#define __DEPLOY__
#include<string>

struct pathinf
{
	int path[10000][1000];
	int path_num = 0;
	int path_w[10000];
	int path_length[10000];
};
//��������ڵ������Ϣ
struct server_inf
{
	int server[500];
	int server_num = 0;
};
//ÿ�������ѽڵ����ͳ��
extern int*rate_flow;

//�õ�ÿ���������
extern void Get_RFlow(int*rate_flow, int**matrix_w, int num_point);
//�õ���ʼ��
//����ѡȡ��ʼserver
extern void initial_server(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w, int**matrix_p, server_inf&server);
//�õ���ʼ��
extern void initial_S(int num_user, int num_point, int*user, int*user_w, server_inf&server, pathinf&path, char*filename);

#endif
