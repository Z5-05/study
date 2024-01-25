#include <iostream>
#include <pthread.h>
#include <time.h>
#include "utils.cpp"
#include <unistd.h>
#include <thread>
#pragma GCC diagnostic ignored "-Wreturn-type"

using namespace std;

const size_t MAX_THREADS = 8;

struct TASK
{
	size_t low;
	size_t high;
	int* arr;
	TASK(size_t low, size_t high, int* arr) :
	low(low), high(high), arr(arr) {}
};

void* merge_sort_thread(void* arg)
{
    TASK* task = (TASK*)arg;
    int low, high;

    low = task->low;
    high = task->high;
    
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(task->arr, low, mid);
        merge_sort(task->arr, mid + 1, high);
        merge(task->arr, low, mid, high);
    }
}
 
int main(int argc, char* argv[])
{	
	int array_size = atoi(argv[1]);

	float diff_time;
	int* array = (int*)malloc(sizeof(int) * array_size);
    
	srand(clock());
	for (int i = 0; i < array_size; i++)
		array[i] = rand();

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * MAX_THREADS);
	TASK* tasklist = (TASK*)malloc(sizeof(TASK) * MAX_THREADS);

	int len = array_size / MAX_THREADS;

	TASK* task;
	int low = 0;

	clock_t time = clock();

	for (int i = 0; i < MAX_THREADS; i++, low += len)
	{
		task = &tasklist[i];
		task->low = low;
		task->high = low + len - 1;
		if (i == (MAX_THREADS - 1))
			task->high = array_size - 1;
	}

	for (int i = 0; i < MAX_THREADS; i++)
	{
		task = &tasklist[i];
		task->arr = array;
		pthread_create(&threads[i], 0, merge_sort_thread, task);
	}
    
	for (int i = 0; i < MAX_THREADS; i++)
		pthread_join(threads[i], NULL);

	TASK* taskm = &tasklist[0];
	for (int i = 1; i < MAX_THREADS; i++)
	{
		TASK* task = &tasklist[i];
		merge(taskm->arr, taskm->low, task->low - 1, task->high);
	}

	diff_time = (clock() - time) / 1000.0L;

	int last = 0;
	for (int i = 0; i < array_size; i++)
	{
		if (array[i] < last)
		{
			throw std::exception();
		}
		last = array[i];
	}

	free(array);
	free(tasklist);
	free(threads);
	cout << array_size << ",merge_sort_base_threads," << diff_time << endl;
	return 0;
}