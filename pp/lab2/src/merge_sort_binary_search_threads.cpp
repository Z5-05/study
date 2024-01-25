#include <iostream>
#include <pthread.h>
#include <time.h>
#include "utils.cpp"
#include <unistd.h>
#include <thread>
#pragma GCC diagnostic ignored "-Wreturn-type"

using namespace std;

const size_t MAX_THREADS = 8;

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
	TASK_merge* task = (TASK_merge*)arg;

	int n1 = task->high1 - task->low1 + 1;
	int n2 = task->high2 - task->low2 + 1;

	if (n1 < n2)
	{
		swap(task->low1, task->low2);
		swap(task->high1, task->high2);
		swap(n1, n2);
	}

	if (n1 == 0)
		return (void*)NULL;
	else
	{
		q1 = (task->low1 + task->high1) / 2;
		q2 = binary_search(task->T[q1], task->T, task->low2, task->high2);
		q3 = task->low_base + (q1 - task->low1) + (q2 - task->low2);
		task->array[q3] = task->T[q1];

		TASK_merge* task1 = new TASK_merge(task->T, task->low1, q1 - 1, task->low2, q2 - 1, 
										   task->array, task->low_base);
		bs_threads_merge(task1);
		TASK_merge* task2 = new TASK_merge(task->T, q1 + 1, task->high1, q2, task->high2, 
		                                   task->array, q3+1);
		bs_threads_merge(task2);
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
		bs_threads_sort(TASK1);
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

	// cout << "Базовый массив:\n";
	// for (int i = 0; i < array_size; i++)
	// 	cout << A[i] << " ";
	// cout << endl;

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * MAX_THREADS);
	TASK_sort* tasklist = (TASK_sort*)malloc(sizeof(TASK_sort) * MAX_THREADS);

	int len = array_size / MAX_THREADS;

	TASK_sort* task;
	int low = 0;
	clock_t time = clock();

	for (int i = 0; i < MAX_THREADS; i++, low += len)
	{
		task = &tasklist[i];
		task->low = low;
		task->high = low + len - 1;
		task->low_base = low;
		if (i == (MAX_THREADS - 1))
			task->high = array_size - 1;
	}

	for (int i = 0; i < MAX_THREADS; i++)
	{
		task = &tasklist[i];
		task->A = A;
		task->B = B;
		pthread_create(&threads[i], 0, bs_threads_sort, task);
	}
    
	for (int i = 0; i < MAX_THREADS; i++)
		pthread_join(threads[i], NULL);

	TASK_sort* taskm = &tasklist[0];
	for (int i = 1; i < MAX_THREADS; i++)
	{
		TASK_sort* task = &tasklist[i];
		merge(taskm->B, taskm->low, task->low - 1, task->high);
	}

	diff_time = (clock() - time) / 1000.0L;


	int last = 0;
	for (int i = 0; i < array_size; i++)
	{
		if (B[i] < last)
		{
			throw std::exception();
		}
		last = B[i];
	}

	free(B);
	free(tasklist);
	free(threads);
	cout << array_size << ",merge_sort_binary_search_threads," << diff_time << endl;
	return 0;
}