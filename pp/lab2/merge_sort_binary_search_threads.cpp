#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "utils.cpp"
#include <unistd.h>

#pragma GCC diagnostic ignored "-Wreturn-type"

using namespace std;

const int MAX_THREADS = 4;
int threads_count = 1;

struct TASK_merge
{
	int* T;
	int low1, low2, high1, high2, low_base;
	int* array;
	TASK_merge(int *T, int low1, int high1, int low2, int high2, int* array, int low_base) :
	T(T), low1(low1), high1(high1), low2(low2), high2(high2), array(array), low_base(low_base) {}
};

struct TASK_sort
{
	int* B;
	int low, high, low_base;
	int* A;
	TASK_sort(int *A, int low, int high, int* B, int low_base) :
	A(A), low(low), high(high), B(B), low_base(low_base) {}
};

void swap(int& a, int& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

int binary_search(int val, int* array, int p, int r)
{
	int high = p > (r + 1) ? p : (r + 1);
	while (p < high)
	{
		int mid = (p + high) / 2;
		if (val <= array[mid])
			high = mid;
		else
			p = mid + 1;
	}
	return high;
}

void* bs_threads_merge(void* arg)
{	
	int q1, q2, q3;
	TASK_merge* TASK = (TASK_merge*)arg;

	int n1 = TASK->high1 - TASK->low1 + 1;
	int n2 = TASK->high2 - TASK->low2 + 1;

	if (n1 < n2)
	{
		swap(TASK->low1, TASK->low2);
		swap(TASK->high1, TASK->high2);
		swap(n1, n2);
	}

	if (n1 == 0)
		return (void*)NULL;
	else
	{
		q1 = (TASK->low1 + TASK->high1) / 2;
		q2 = binary_search(TASK->T[q1], TASK->T, TASK->low2, TASK->high2);
		q3 = TASK->low_base + (q1 - TASK->low1) + (q2 - TASK->low2);
		TASK->array[q3] = TASK->T[q1];

		TASK_merge* TASK1 = new TASK_merge(TASK->T, TASK->low1, q1 - 1, TASK->low2, q2 - 1, TASK->array, TASK->low_base);
		if (threads_count >= MAX_THREADS)
		{
			bs_threads_merge(TASK1);
		}
		else 
		{
			++threads_count;
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, bs_threads_merge, TASK1);
			pthread_join(new_thread, NULL);
			--threads_count;
		}
		TASK_merge* TASK2 = new TASK_merge(TASK->T, q1 + 1, TASK->high1, q2, TASK->high2, TASK->array, TASK->low_base+1);
		bs_threads_merge(TASK2);
	}
}

void* bs_threads_sort(void* arg)
{	
	TASK_sort* TASK = (TASK_sort*)arg;
	int n = TASK->high - TASK->low + 1;

	if (n == 1)
		TASK->B[TASK->low_base] = TASK->A[TASK->low];
	else
	{
		int *T = new int[n+1];
		int q1 = (TASK->low + TASK->high) / 2;
		int q2 = q1 - TASK->low + 1;

		TASK_sort* TASK1 = new TASK_sort(TASK->A, TASK->low, q1, T, 1);
		if (threads_count >= MAX_THREADS)
		{
			bs_threads_sort(TASK1);
		}
		else 
		{
			++threads_count;
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, bs_threads_sort, TASK1);
			pthread_join(new_thread, NULL);
			--threads_count;
		}
		TASK_sort* TASK2 = new TASK_sort(TASK->A, q1 + 1, TASK->high, T, q2 + 1);
		bs_threads_sort(TASK2);

		TASK_merge* TASK_mg = new TASK_merge(T, 1, q2, q2 + 1, n, TASK->B, TASK->low_base);
		bs_threads_merge(TASK_mg);

		delete[] T;
	}
}
 
int main(int argc, char* argv[])
{	
	int array_size = atoi(argv[1]);
	float diff_time;
	int* A = (int*)malloc(sizeof(int) * array_size);
	int* B = (int*)malloc(sizeof(int) * array_size);
    
	srand(clock());
	for (int i = 0; i < array_size; i++)
		A[i] = rand() % 10;

	clock_t time = clock();
	TASK_sort* TASK = new TASK_sort(A, 0, array_size - 1, B, 0);
	bs_threads_sort(TASK);
	diff_time = (clock() - time) / 1000.0L;

	int last = 0;
	for (int i = 0; i < array_size; i++)
	{
		if (B[i] < last)
		{
			cout << "Here is problem.\n";
			throw std::exception();
		}
		last = B[i];
	}
	free(B);
	cout << array_size << ",merge_sort_binary_search_threads," << diff_time << endl;
	return 0;
}