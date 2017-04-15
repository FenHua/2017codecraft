#ifndef __LIB_SA_H_
#define __LIB_SA_H_
#include"deploy.h"
#include"mincost.h"

extern MCMF target(server_inf &server, int *user, int *user_w, int num_user, int num_point, int*cost);

//���¹滮·��
bool findroute_s(int num_user, int num_point, int N_s_cost, pathinf&path, MCMF&temp);

//������
void BruteForce(clock_t starttime, int server_cost, int num_user, int num_point, int*user, int*user_w, server_inf &server, pathinf&path, char*filename);

//ģ���˻�
void SA(int server_cost, int num_user, int num_point, int*user, int*user_w, server_inf &server, pathinf&path, char*filename);

//д���
bool WResult(int num_point, pathinf&path, char*filename);

#endif