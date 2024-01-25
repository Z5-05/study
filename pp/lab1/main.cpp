#include "src/stack_lf.cpp"
#include "atomic_threadpool/at_thpool.h"
#include <cstdlib>
#include <iostream>
#include <stack>
#include <limits.h>
#include <string>

#pragma GCC diagnostic ignored "-Wreturn-type"
using namespace std;

const int max_threads = 8;

Stack_lf<int> stack1;
stack<int> stack2;


void push1(void *arg) {
    stack1.push(*((int*)arg));
}
void pop1(void *arg) {
    stack1.pop();
}
void push2(void *arg) {
    stack2.push(*((int*)arg));
}
void pop2(void *arg) {
    stack2.pop();
}

int process(int size, int nthreads) {
    int curr;
    at_thpool_t *thpool = at_thpool_create(nthreads); 
    
    clock_t tm = clock();
    for (int i = 0; i < size; ++i) { 
        
        curr = rand() % 100;
        at_thpool_newtask(thpool, push1, (void*)&curr);
        if (curr % 4 == 0) {
            at_thpool_newtask(thpool, pop1, (void*)&curr);
        }
    }
    float diff = (clock() - tm) / 1000.0L;
    
    cout << "Количество потоков: " << nthreads << " Время: " << diff << endl;
}



int main(int argc, char* argv[])
{	
	int size = atoi(argv[1]);
    process(size, max_threads);
    process(size, 1);
}


