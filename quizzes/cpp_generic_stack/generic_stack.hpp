#ifndef ILRD_RD9315_GENERIC_STACK_HPP
#define ILRD_RD9315_GENERIC_STACK_HPP

#include <iosfwd> // size_t

#include "utils.hpp"

namespace ilrd
{

class Stack
{
public:
    explicit Stack(size_t capacity_ = 8); // Throw bad_alloc.
    ~Stack() noexcept;
    Stack(const Stack& other_); // Throw bad_alloc.
    Stack& operator=(const Stack& other_); // Throw bad_alloc.

    void Push(void *data); // Throw out_of_range.
    void *Pop(); // Throw out_of_range.
    void *Peek(); // Throw out_of_range.

    size_t GetSize() const noexcept;
    size_t GetCapacity() const noexcept;
private:
    size_t m_capasity;
    size_t m_size;
    void **m_stack;

    void **InitStack(size_t capacity_);
    void **CopyStack(const Stack& other_);
    void DestroyStack();
};


} // namespace ilrd
#endif // ILRD_RD9315_RC_STRING_HPP