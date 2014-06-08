/*给你一个字符串，里面只包含 () [] 四种符号，请问你需要至少多少个括号才能使这些括号匹配起来。如：
	[] 是匹配的
	([])[]是匹配的
	((]是不匹配的
	([)]是不匹配的

输入：
第一行输入一个正整数N,表示测试数据组数(N<=10)
每组数据都只有一行，是一个字符串S,S中字包含以上四种字符，S的长度不超过100

输出：
对于魅族测试数据都输出一个正整数，表示最少需要添加的括号数量。每组测试输出栈一行
样例输入：
4
[]
([])[]
((]
([)] 
样例输出：
0
0
3
2 */
#include <iostream>
#include <cstring>
using namespace std;
/* 动态规划： DP[i][j] 表示从i到j所需要的最少括号数。
 * i == j时，只有一个字符，故需要一个括号
 * i < j时，s[i]与s[j]匹配, 则DP[i][j] = min{DP[i][j],DP[i+1][j-1]},其中，假设i <= k <= j状态转移方程为DP[i][j] = min{DP[i][j], DP[i][k] + DP[k+1][j]}
 */
#define MAXSIZ 101
bool cmp(int m, int n) {
	if ((m == '(' && n == ')') || (m == '[' && n == ']'))
		return true;
	return false;
}
int min(int a, int b){
	return a>b?b:a;
}
void acm() {
	string ss;
	int len;
	int num = 0;
	int DP[MAXSIZ][MAXSIZ] = {0};
	cin>>ss;
	len = ss.length();
	for (int i = 0; i < len; ++i) {
		DP[i][i] = 1;
	}
	for (int m = 1; m < len; ++m) {
		for (int i = 0; i < len - m; ++i) {
			int j = i + m;
			DP[i][j] = MAXSIZ; // 设置当前未设置的值为最大值
			if (cmp(ss[i],ss[j])) {
				DP[i][j] = min(DP[i][j],DP[i+1][j-1]);
			}
			for (int k = i; k < j; ++k)
				DP[i][j] = min(DP[i][j], DP[i][k] + DP[k+1][j]);
		}
	}
	cout<<DP[0][len - 1]<<endl;
}
int main(int argc, char const *argv[]) {
	int n;
	cin>>n;
	for (int i = 0; i < n; ++i) {
		acm();	
	}	
	return 0;
}