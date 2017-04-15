#ifndef __LIB_IO_H__
#define __LIB_IO_H__

#define MAX_EDGE_NUM    (2000 * 20)

//��ȡ�ļ������������buff��
//buffΪһ��ָ�����飬ÿһ��Ԫ����һ���ַ�ָ�룬��Ӧ�ļ���һ�е����ݡ�
//specΪ������������������
extern int read_file(char ** const buff, const unsigned int spec, const char * const filename);

//��result�������е�����д���ļ���д�뷽ʽΪ����д��
extern void write_result(const char* buff, const char * const filename);

//�ͷŶ��ļ��Ļ�����
extern void release_buff(char ** const buff, const int valid_item_num);

//��ӡʱ�䡣���Ϊ��ӡ��Ϣͷ
extern void print_time(const char *head);

//�����������黯
extern int GetData(int line_num, int num_point, int num_edge, char*topo[], int **matrix_w, int**matrix_p, int*user_w, int*user);
#endif