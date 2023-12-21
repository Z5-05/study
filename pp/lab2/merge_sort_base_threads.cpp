#include <iostream>
#include <pthread.h>
#include <time.h>
#include "utils.cpp"
#include <unistd.h>
#pragma GCC diagnostic ignored "-Wreturn-type"

using namespace std;

void* merge_sort_thread(void* arg)
{
    TASK* task = (TASK*)arg;
    int low, high;

    low = task->low;
    high = task->high;
    
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(task->a, low, mid);
        merge_sort(task->a, mid + 1, high);
        merge(task->a, low, mid, high);
    }
}
 
int main(int argc, char* argv[])
{	
	int threads_num = atoi(argv[2]);
	int array_size = atoi(argv[1]);

	float diff_time;
	int* array = (int*)malloc(sizeof(int) * array_size);
    
	srand(clock());
	for (int i = 0; i < array_size; i++)
		array[i] = rand();

	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * threads_num);
	TASK* tasklist = (TASK*)malloc(sizeof(TASK) * threads_num);

	int len = array_size / threads_num;

	TASK* task;
	int low = 0;

	clock_t time = clock();

	for (int i = 0; i < threads_num; i++, low += len)
	{
		task = &tasklist[i];
		task->low = low;
		task->high = low + len - 1;
		if (i == (threads_num - 1))
			task->high = array_size - 1;
	}

	for (int i = 0; i < threads_num; i++)
	{
		task = &tasklist[i];
		task->a = array;
		pthread_create(&threads[i], 0, merge_sort_thread, task);
	}
    
	for (int i = 0; i < threads_num; i++)
		pthread_join(threads[i], NULL);

	TASK* taskm = &tasklist[0];
	for (int i = 1; i < threads_num; i++)
	{
		TASK* task = &tasklist[i];
		merge(taskm->a, taskm->low, task->low - 1, task->high);
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
	cout << array_size << ",merge_sort_base_threads," << diff_time << "," << threads_num << endl;
	return 0;
}