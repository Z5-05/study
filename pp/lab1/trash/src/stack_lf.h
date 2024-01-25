#ifndef STACK_H
#define STACK_H

#include "node.h"
#include "hazard_pointers.h"
#include <atomic>
#include <optional>

template <typename T>
class Stack_lf {
    public:
        std::atomic<Node<T>*> head;
        
        /* methods */
        void push(const T& value);
        void pop();
        bool isEmpty();
        std::optional<T> top();

        Stack_lf();
        ~Stack_lf();
};

#endif // STACK_H