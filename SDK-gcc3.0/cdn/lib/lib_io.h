#ifndef __LIB_IO_H__
#define __LIB_IO_H__

#define MAX_EDGE_NUM    (2000 * 20)

//读取文件并按行输出到buff。
//buff为一个指针数组，每一个元素是一个字符指针，对应文件中一行的内容。
//spec为允许解析的最大行数。
extern int read_file(char ** const buff, const unsigned int spec, const char * const filename);

//将result缓冲区中的内容写入文件，写入方式为覆盖写入
extern void write_result(const char* buff, const char * const filename);

//释放读文件的缓冲区
extern void release_buff(char ** const buff, const int valid_item_num);

//打印时间。入参为打印信息头
extern void print_time(const char *head);

//将数据流数组化
extern int GetData(int line_num, int num_point, int num_edge, char*topo[], int **matrix_w, int**matrix_p, int*user_w, int*user);
#endif