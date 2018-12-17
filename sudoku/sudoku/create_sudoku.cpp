#include "stdafx.h"
#include "create_sudoku.h"

void create_sudoku(int n)
{
	FILE* out_file = fopen("sudoku.txt", "w");
	if (!out_file)
	{
		fprintf(stderr, "ERROR! Can't open output file!\n");
		exit(1);
	}
	int first[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
	for (int i = 0; i < 6 && n; i++)
	{
		if (i)
		{
			std::next_permutation(first + 3, first + 6);
			first[6] = 2, first[7] = 5, first[8] = 8;
		}
		for (int j = 0; j < 6 && n; j++)
		{
			if (j) std::next_permutation(first + 6, first + 9);
			char row[10] = "912345678";
			for (int k = 0; k < 40320 && n; k++)
			{
				if (k) std::next_permutation(row + 1, row + 9);
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
						if (c) fputc(' ', out_file);
						fputc(row[(c + first[r]) % 9], out_file);
					}
					fputc('\n', out_file);
				}
				if (--n) fputc('\n', out_file);
				fflush(out_file);
			}
		}
	}
	fclose(out_file);
}
