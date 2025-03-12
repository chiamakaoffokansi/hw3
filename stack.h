#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

template <typename T>
class Stack : private std::vector<T> {
public:
    Stack() = default;  // default constructor
    ~Stack() = default; // default destructor

    bool empty() const { return std::vector<T>::empty(); } //ret true if empty (good for test cases)
    size_t size() const { return std::vector<T>::size(); }
    
    void push(const T& item) { this->push_back(item); } //creating space within for new item

    void pop() {
        if (this->empty()) {
            throw std::underflow_error("Stack underflow: Cannot pop from an empty stack");
        }
        this->pop_back(); //rids of element if list is not empty
    }

    const T& top() const { //access recently added element
        if (this->empty()) { //validity checkh here
            throw std::underflow_error("Stack underflow: Cannot access top of an empty stack");
        }
        return this->back(); //points to bottom element
    }
};

#endif