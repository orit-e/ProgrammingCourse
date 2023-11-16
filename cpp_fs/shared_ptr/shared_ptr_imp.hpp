/*******************************************************************************
 * 																			   *
 * 					File name:			shared_ptr_imp.hpp			    	   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#include "shared_ptr.hpp"

namespace ilrd
{
template<typename T>
SharedPtr<T>::SharedPtr(T *resource_)
: m_shareCounter(InitShareCounter())
, m_resource(resource_)
{
    ; // Empty on purpose.
}

template<typename T>
SharedPtr<T>::~SharedPtr() noexcept
{
    DecreaseOrDestroyCounter();
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other_) noexcept
: m_shareCounter(other_.m_shareCounter)
, m_resource(other_.m_resource)
{
    IncreaseCounter();
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other_) noexcept
{
    SharedPtr<T> otherCopyTmp(*this);

    DecreaseOrDestroyCounter();

    m_resource = other_.m_resource;
    m_shareCounter = other_.m_shareCounter;

    return *this;
}

template<typename T>
T& SharedPtr<T>::operator*() const
{
    return *m_resource;
}

template<typename T>
T *SharedPtr<T>::operator->() const
{
    return m_resource;
}

template<typename T>
T *SharedPtr<T>::get() const noexcept
{
    return m_resource;
}

template<typename T>
size_t *SharedPtr<T>::InitShareCounter()
{
    size_t *tmp = new size_t(1);

    return tmp;
}
template<typename T>
void SharedPtr<T>::DecreaseOrDestroyCounter() noexcept
{
    if(1 != *m_shareCounter)
    {
        *m_shareCounter -= 1;
    }
    else
    {
        delete m_shareCounter;
        m_shareCounter = 0;
        m_resource = ReturnBadPtr();
    }
}

template<typename T>
T *SharedPtr<T>::ReturnBadPtr()
{
    return (T *)0xDEADBEAFDEADBEAF;
}

template<typename T>
void SharedPtr<T>::IncreaseCounter() noexcept
{
    *m_shareCounter += 1;
}

template<typename T>
template<typename S>
SharedPtr<T>::SharedPtr(const SharedPtr<S>& other_)
: m_shareCounter(other_.m_shareCounter)
, m_resource(other_.m_resource)
{
    IncreaseCounter();
}
}