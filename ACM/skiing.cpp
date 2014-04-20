/*
	Michael喜欢滑雪百这并不奇怪，因为滑雪的确很刺激。可是 为了获得速度，滑的区域必须向下倾斜，而且当你滑到坡底，你不得不再次走上坡或者等待升降机来载你。Michael想知道载一个区域中最长底滑坡。区域由 一个二维数组给出。数组的每个数字代表点的高度。下面是一个例子
1  2  3  4 5
16 17 18 19 6
15 24 25 20 7
14 23 22 21 8
13 12 11 10 9
一个人可以从某个点滑向上下左右相邻四个点之一，当且仅当高度减小。在上面的例子中，一条可滑行的滑坡为24-17-16-1。当然25-24-23-...-3-2-1更长。事实上，这是最长的一条。
Input
输入的第一行表示区域的行数R和列数C(1 <= R,C <= 100)。下面是R行，每行有C个整数，代表高度h，0<=h<=10000。
*/
#include <iostream>
using namespace std;

int area[100][100] = {0};
int height[100][100] = {0};
int row = 0;
int column = 0;

int DP(int i, int j){
	int max = 0;
	if (height[i][j])
		return height[i][j];
	// 上
	if (j > 0 && area[i][j] > area[i][j-1] && max < DP(i,j-1))
		max = DP(i,j-1);
	// 下
	if (j < column-1 && area[i][j] > area[i][j+1] && max < DP(i,j+1))
		max = DP(i,j+1);
	// 左
	if (i > 0 && area[i][j] > area[i-1][j] && max < DP(i-1,j))
		max = DP(i-1, j);
	// 右
	if (i < row-1 && area[i][j] > area[i+1][j] && max < DP(i+1,j))
		max = DP(i+1, j);
	height[i][j] = max +1;
	return height[i][j];
}
void acm() {
	int maxHeight = 0;
	cin>>row>>column;
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			cin>>area[i][j];	
			height[i][j] = 0;
		}
	}
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			int temp = DP(i,j);
			if (maxHeight < temp)
				maxHeight = temp; 
		}
	}
	// for (int i = 0; i < row; ++i) {
	// 	for (int j = 0; j < column; ++j)
	// 		cout<<height[i][j]<<"\t";
	// 	cout<<endl;
	// }
				
	cout<<maxHeight<<endl;
}
int main(int argc, char const *argv[]) {
	int n;
	cin>>n;
	for(int i=0;i<n;i++)
		acm();
	return 0;
}
