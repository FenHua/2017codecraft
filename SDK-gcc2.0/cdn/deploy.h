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
//建立服务节点服务信息
struct server_inf
{
	int server[500];
	int server_num = 0;
};
//每个点消费节点入度统计
extern int*rate_flow;

//得到每个点的流量
extern void Get_RFlow(int*rate_flow, int**matrix_w, int num_point);
//得到初始解
//暴力选取初始server
extern void initial_server(int num_user, int num_point, int server_cost, int*user, int*user_w, int**matrix_w, int**matrix_p, server_inf&server);
//得到初始解
extern void initial_S(int num_user, int num_point, int*user, int*user_w, server_inf&server, pathinf&path, char*filename);

#endif
