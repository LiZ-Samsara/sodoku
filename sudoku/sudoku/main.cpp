// Sudoku.cpp: 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "getopt.h"
#include "create_sudoku.h"
#include "solve_sudoku.h"

inline int to_integer(char* arg)
{
	int ret = 0;
	while (*arg)
	{
		if (!isdigit(*arg)) return -1;
		ret = ret * 10 + *arg - '0';
		arg++;
	}
	return ret;
}

int main(int argc, char* argv[])
{
	opterr = 0;
	int opt = getopt(argc, argv, "c:s:");
	if (opt == 'c')
	{//生成数独
		int n = to_integer(optarg);
		if (n >= 1 && n <= 1000000)
		{//输入格式正确
			double start_time = clock();
			create_sudoku(n);
			printf("%d sudoku final status are created successfully. Execution time : %.03lf s\n", n, (clock() - start_time) / 1000);
		}
		else printf("ERROR! n should be an integer ranging from 1 to 1000000!\n");
	}
	else if (opt == 's')
	{//求解数独
		double start_time = clock();
		int n = solve_sudoku(fopen(optarg, "r"));
		printf("%d sudoku puzzles have been solved. Execution time : %.03lf s\n", n, (clock() - start_time) / 1000);
	}
	else
	{//输入错误
		printf("ERROR! Please input right command line as following:\n");
		printf("    -c n : create n different sudoku final status.\n");
		printf("    -s absolute_path : solve several(1~1000) sudoku puzzles.\n");
	}
    return 0;
}
