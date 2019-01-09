#include <stdio.h>




int main()
{
	int COLS = 4, ROWS = 4;
	//利用字符串记录当前排序
	unsigned char rank[COLS * ROWS + 1];
	
	int len = COLS * ROWS;
	//协助数组
	int help_arr[COLS * ROWS];
	for(int i = 0; i < COLS * ROWS; i++)
	{
		help_arr[i] = i + 1;
		printf("%d\t", help_arr[i]);
	}
	srandom(3);
	printf("\n");
	for(int i = 0; i < COLS * ROWS; i++)
	{
		int index = random()%len;
		printf("%d\t", index);
		rank[i] = help_arr[index];
		printf("%d\n", rank[i]);
		help_arr[index] = help_arr[len - 1];
		len--;

	}
	for(int i = 0; i < COLS * ROWS; i++)
	{
		printf("%d\t", rank[i]);
	}





}









