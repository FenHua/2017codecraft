#ifndef __LIB_SA_H_
#define __LIB_SA_H_
#include"deploy.h"
#include"mincost.h"

extern MCMF target(server_inf server, int *user, int *user_w, int num_user, int num_point, int*cost);

//���¹滮·��
bool findroute_s(int num_user, int num_point, int N_s_cost, pathinf&path, MCMF&temp);

//ģ���˻�
void SA(int sever_cost, int num_user, int num_point, int*user, int*user_w, server_inf &server, pathinf&path, char*filename);

//д���
bool WResult(int num_point,pathinf&path, char*filename);

#endif