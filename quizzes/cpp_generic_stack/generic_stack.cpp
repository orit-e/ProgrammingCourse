#include <iostream>	//	size_t, new[], delete[]
#include <cstring>	//	memcpy()

#include "generic_stack.hpp"

namespace ilrd
{
Stack::Stack(size_t capacity_)
: m_capasity(capacity_)
, m_size(0)
,m_stack(InitStack(capacity_))
{
    ; // Empty on purpose.
}

Stack::~Stack() noexcept
{
    DestroyStack();
    m_stack = 0;
}

Stack::Stack(const Stack& other_)
: m_capasity(other_.m_capasity)
, m_size(other_.m_size)
,m_stack(CopyStack(other_))
{
    ; // Empty on purpose.
}

Stack& Stack::operator=(const Stack& other_)
{
    m_capasity = other_.m_capasity;
    m_size = other_.m_size;

    void **tmp = CopyStack(other_);
    DestroyStack();
    m_stack = tmp;

    return *this;
}

void Stack::Push(void *data)
{
    m_stack[m_size] = data;
    ++m_size;
}

void *Stack::Pop()
{
    return m_stack[--m_size];
}

void *Stack::Peek()
{
    return m_stack[m_size - 1];
}

size_t Stack::GetSize() const noexcept
{
    return m_size;
}

size_t Stack::GetCapacity() const noexcept
{
    return m_capasity;
}

void **Stack::InitStack(size_t capacity_)
{
    return (new void*[capacity_]);
}

void **Stack::CopyStack(const Stack& other_)
{
    void **tmp = InitStack(other_.m_capasity);

    memcpy(tmp, other_.m_stack, other_.m_size * sizeof(void*));

    return tmp;
}

void Stack::DestroyStack()
{
    delete[] m_stack;
}

} // namespace ilrd