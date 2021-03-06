#include "stdafx.h"
#include "writer.h"

inline void dec(int x, int& r, int& c, int& v)
{
	x--;
	v = x % 9, x /= 9;
	c = x % 9, x /= 9;
	r = x;
}

inline void trans(const std::vector<int>& pos, char (&board)[10][10])
{
	for (auto i : pos)
	{
		int r, c, v;
		dec(i, r, c, v);
		board[r][c] = '1' + v;
	}
}

void WINAPI output_main(void* args)
{
	Writer* pWriter = (Writer*)args;
	for (;;)
	{
		WaitForSingleObject(pWriter->semaphore, INFINITE);
		EnterCriticalSection(&pWriter->lock);

		if (pWriter->idx.empty() && pWriter->kill)
		{
			LeaveCriticalSection(&pWriter->lock);
			break;
		}

		int n = pWriter->idx.front();
		pWriter->idx.pop();
		char board[10][10];
		trans(pWriter->boards.front(), board);
		pWriter->boards.pop();

		LeaveCriticalSection(&pWriter->lock);

		if (n) fputc('\n', pWriter->index);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (j) fputc(' ', pWriter->index);
				fputc(board[i][j], pWriter->index);
			}
			fputc('\n', pWriter->index);
		}
		fflush(pWriter->index);
	}
}

Writer::Writer(FILE* index) :
	index(index), kill(false), idx(), boards()
{
	InitializeCriticalSection(&lock);
	semaphore = CreateSemaphoreW(NULL, 0, LONG_MAX, NULL);
	hThread = (HANDLE)_beginthread(output_main, 0, this);
}

Writer::~Writer()
{
	fclose(index);
	CloseHandle(hThread);
	CloseHandle(semaphore);
}

void Writer::pass(int n, std::vector<int>& pos)
{
	EnterCriticalSection(&lock);

	idx.push(n);
	boards.push(pos);
	ReleaseSemaphore(semaphore, 1, NULL);

	LeaveCriticalSection(&lock);
}

void Writer::join()
{
	WaitForSingleObject(hThread, INFINITE);
}

void Writer::set_kill()
{
	EnterCriticalSection(&lock);

	kill = true;
	ReleaseSemaphore(semaphore, 1, NULL);

	LeaveCriticalSection(&lock);
}