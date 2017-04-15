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
	int path[1][1000];//每一条路的路径存储数组
	int path_length[1];//每一条路的长度
	int path_w[1];//每一条路的所用带宽
	int path_num = 0;//路的条数
	int server[1];//每一条路对应的服务器
	int point_cost[1000];//从起始点到其它点的花销
};
//建立服务节点服务信息
struct server_inf
{
	int server[500];
	int server_num = 0;
};
//每个点消费节点入度统计
extern int*interpoint_num;

//寻找主干路
void Find_mainroute(User_inf userinf[], int*interpoint_num, int num_point, int num_user, int server_cost, int **matrix_w, int**matrix_p, int*user_w, int*user);

//寻找服务节点
void deploy_server(int num_point, int num_user, User_inf userinf[], int*interpoint_num, int*user_w, int*user, server_inf &server);

//得到结果
bool Get_result(User_inf userinf[], int num_point, int num_user, int**real_w, char*filename);

#endif
