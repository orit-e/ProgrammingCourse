/*******************************************************************************
 * 																			   *
 * 					File name:			future_task_imp.hpp		       		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy A								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_FUTURE_TASK_IMP_HPP
#define ILRD_RD9315_FUTURE_TASK_IMP_HPP

#include "thread_pool.hpp"

namespace ilrd
{

template<typename OUTPUT>
ThreadPool::FutureTask<OUTPUT>::FutureTask(FTFunc_t func_)
: m_func(func_)
, m_returnVal()
, m_sem(0)
{
    // Empty on purpose.
}

template<typename OUTPUT>
ThreadPool::FutureTask<OUTPUT>::~FutureTask()
{
    // Empty on purpose.
}

template<typename OUTPUT>
void ThreadPool::FutureTask<OUTPUT>::Run()
{
    m_returnVal = m_func();
    m_sem.post();
}

template<typename OUTPUT>
OUTPUT ThreadPool::FutureTask<OUTPUT>::GetReturnVal()
{
    m_sem.wait();
    return m_returnVal;
}



} // namespace ilrd

#endif // ILRD_RD9315_FUTURE_TASK_IMP_HPP