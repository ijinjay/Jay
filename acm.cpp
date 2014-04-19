#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// 求子串
char *substr(char *strdest, char *strsrc, int nindex, int ncount)
{
	for(int i=0;i<ncount;i++)
	{
		*(strdest+i)=*(strsrc+nindex+i);
	}
	strdest[ncount]='\0';
	return strdest;
}
int main(int argc, char const *argv[]) {
	int m[500] = {0}; // 存放随机数据
	int mark[500] = {0}; // 存放随机后的序列化数据
	int len = 0; // 文件长度
	char data[55000]; // 文件中的数据
	int index_l[4000] = {0}; // < 在data的位置索引
	int index_r[4000] = {0}; // > 在data的位置索引
	char first[500][200]; // 存放第一个字段
	char second[500][200]; // 第二个字段
	char third[500][200]; // 第三个字段
	FILE * f;  // 文件指针
	srand((int)time(0)); 
	for (int i = 0; i < 500; ++i) {
		m[i] = rand()%500;
	}
	for (int i = 0; i < 500; ++i) {
		if (mark[m[i]] == 0) {
			mark[m[i]] = i;
		}
		else {
			int j = mark[m[i]];
			j = j+1;
			if (j >= 500) 
				j = 0;
			while(mark[j] != 0) {
				j ++;
				if (j >= 500)
					j = 0;
			}
			mark[j] = i;
		}
	}
	// 打开文件，初始化len，data
	if ((f = fopen("danci.txt","r")) != NULL) {
		fseek(f,0L,SEEK_END);
		len = ftell(f);
		printf("Lenght of file is %d\n", len);
		fseek(f,0L,SEEK_SET);
		fread(data,sizeof(char),len,f);
		fclose(f);
	}
	// number_l: < 的个数
	int number_l = 0;
	int number_r = 0;
	// 赋值index_l, index_r
	for (int i = 0; i < strlen(data); ++i) {
		if (data[i] == '<') {
			index_l[number_l] = i;
			number_l ++;
		}
		if (data[i] == '>') {
			index_r[number_r] = i;
			number_r ++;
		}
	}
	// numbers: 总行数
	int numbers = number_r/8;
	// 存放字段值
	for (int i = 0; i < 20; ++i) {
		printf("%d,%d\n", index_l[i], index_r[i]);
	}
	for (int i = 0; i < numbers; ++i) {
		substr(first[i], data, index_r[8*i+1]+1, index_l[8*i+2]-index_r[8*i+1]-1);	
		substr(second[i], data, index_r[8*i+3]+1, index_l[8*i+4]-index_r[8*i+3]-1);
		substr(third[i], data, index_r[8*i+5]+1, index_l[8*i+6]-index_r[8*i+5]-1);
	}
	if ((f = fopen("cnew.txt","w+")) != NULL ) {
		fprintf(f, "%s\n%s\n%s\n", first[0],second[0],third[0]);
		fclose(f);
	}
	getchar();
	getchar();
	return 0;
}