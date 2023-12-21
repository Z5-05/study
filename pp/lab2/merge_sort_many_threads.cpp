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

void* merge_sort_many_threads(void* arg)
{		
    TASK* task = (TASK*)arg;
    int low, high;

    low = task->low;
    high = task->high;
    
    int mid = low + (high - low) / 2;
	
    if (low < high) {

		TASK* task1 = (TASK*)malloc(sizeof(TASK));
		task1->low = low;
		task1->high = mid;
		task1->a = task->a;

		TASK* task2 = (TASK*)malloc(sizeof(TASK));
		task2->low = mid+1;
		task2->high = high;
		task2->a = task->a;

		if (threads_count >= MAX_THREADS)
		{
			merge_sort_many_threads(task1);
		}
		else 
		{
			++threads_count;
			pthread_t new_thread;
			pthread_create(&new_thread, NULL, merge_sort_many_threads, task1);
			pthread_join(new_thread, NULL);
			--threads_count;
		}
		merge_sort_many_threads(task2);

        merge(task->a, low, mid, high);
    }
}
 
int main(int argc, char* argv[])
{	
	int array_size = atoi(argv[1]);
	float diff_time;
	int* array = (int*)malloc(sizeof(int) * array_size);
    
	srand(clock());
	for (int i = 0; i < array_size; i++)
		array[i] = rand() % 10;

	TASK* task = (TASK*)malloc(sizeof(TASK));
	int low = 0;
	int high = array_size-1;
	

	task->low = low;
	task->high = high;
	task->a = array; 

	
	pthread_t thread;
	clock_t time = clock();
	pthread_create(&thread, NULL, merge_sort_many_threads, task);
    pthread_join(thread, NULL);
	diff_time = (clock() - time) / 1000.0L;

	int last = 0;
	for (int i = 0; i < array_size; i++)
	{
		if (array[i] < last)
		{
			cout << "Here is problem.\n";
			throw std::exception();
		}
		last = array[i];
	}
	free(array);
	cout << array_size << ",merge_sort_many_threads," << diff_time << endl;
	return 0;
}