/*******************************************************************************
 * 																			   *
 * 					File name:			shared_ptr.hpp			    		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_SHARED_PTR_HPP
#define ILRD_RD9315_SHARED_PTR_HPP

#include <iosfwd>
#include "utils.hpp"

namespace ilrd
{

template<typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *resource_); // may throw bad_alloc.
    ~SharedPtr() noexcept;
    SharedPtr(const SharedPtr& other_) noexcept;
    SharedPtr& operator=(const SharedPtr& other_) noexcept;

    template<typename S>
    SharedPtr(const SharedPtr<S>& other_);

    T& operator*() const;
    T* operator->() const;
    T* get() const noexcept;
private:
    size_t *m_shareCounter;
    T *m_resource;

    size_t *InitShareCounter(); // may throw bad_alloc.
    void DecreaseOrDestroyCounter() noexcept;
    void IncreaseCounter() noexcept;
    T *ReturnBadPtr();

    template<typename S>
    friend class SharedPtr;
};

} // namespace ilrd

#include "shared_ptr_imp.hpp"

#endif // ILRD_RD9315_SHARED_PTR_HPP