#include "stdafx.h"
#include "solve_sudoku.h"
#include "reader.h"
#include "writer.h"
#include "solver.h"

int solve_sudoku(FILE* in_file)
{
	if (!in_file)
	{
		printf("ERROR: unable to open input file!\n");
		exit(1);
	}
	FILE* out_file = fopen("sudoku.txt", "w");
	if (!out_file)
	{
		fprintf(stderr, "ERROR: unable to open output file!\n");
		exit(1);
	}

	Reader reader(in_file);
	Writer writer(out_file);

	Solver::init();
	Solver solver(&reader, &writer);
	solver.start();

	HANDLE ht = CreateWaitableTimerW(NULL, FALSE, NULL);
	LARGE_INTEGER due;
	due.QuadPart = 0LL;
	SetWaitableTimer(ht, &due, 1000, NULL, NULL, FALSE);

	HANDLE waitv[2] = { ht, solver.get_synchronize_object() };
	for (;;)
	{
		int id = WaitForMultipleObjects(2, waitv, FALSE, INFINITE);
		if (id == 0)
		{
			printf("%d sudoku puzzles solved.\r", solver.get_solved_cnt());
		}
		else break;
	}

	CloseHandle(ht);
	return solver.get_solved_cnt();
}