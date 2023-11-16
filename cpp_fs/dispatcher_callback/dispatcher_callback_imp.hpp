/*******************************************************************************
 * 																			   *
 * 					File name:			dispatcher_callback_imp.hpp		   	   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_DISPATCHER_CALLBACK_IMP_HPP
#define ILRD_RD9315_DISPATCHER_CALLBACK_IMP_HPP

#include <iostream>
#include <list>                 // erase(), list iterator, begin(), end()
#include <iterator>             // list iterator (legacy)
#include <boost/foreach.hpp>    // BOOST_FOREACH()
#include <algorithm>            // find()
#include <functional>           // find()

#include "dispatcher_callback.hpp"

namespace ilrd
{
template <typename DATA, typename SUBJECT>
CallbackBase<DATA, SUBJECT>::CallbackBase(SUBJECT *d_)
: m_dispatcher(d_)
{
    m_dispatcher->Subscribe(this);
}

template <typename DATA, typename SUBJECT>
CallbackBase<DATA, SUBJECT>::~CallbackBase() noexcept
{
    if(NULL != m_dispatcher)
    {
        m_dispatcher->UnSubscribe(this);
    }
}

template <typename DATA, typename SUBJECT>
void CallbackBase<DATA, SUBJECT>::UnSubscribe(CallbackBase *s_)
{
    m_dispatcher->UnSubscribe(s_);
}

template <typename DATA, typename SUBJECT>
void CallbackBase<DATA, SUBJECT>::UpdateDispatcherDeath()
{
    m_dispatcher = NULL;
    UpdateDispatcherDeathAction();
}

//******************************************************************************
template <typename DATA, typename SUBJECT>
SimpleCallback<DATA, SUBJECT>::SimpleCallback(SUBJECT *d_, 
                    typename CallbackBase<DATA, SUBJECT>::updateFunc_t update_,
        typename CallbackBase<DATA, SUBJECT>::updateDeathFunc_t updateDeath_)
: CallbackBase<DATA, SUBJECT>(d_)
, m_dSimple(d_)
, m_update(update_)
, m_updateDeath(updateDeath_)
{
    // Empty on purpose.
}

template <typename DATA, typename SUBJECT>
SimpleCallback<DATA, SUBJECT>::~SimpleCallback() noexcept
{
    // Empty on purpose.
}

template <typename DATA, typename SUBJECT>
void SimpleCallback<DATA, SUBJECT>::Update(DATA& data_)
{
    m_update(data_);
}

template <typename DATA, typename SUBJECT>
void SimpleCallback<DATA, SUBJECT>::UpdateDispatcherDeathAction()
{
    m_updateDeath();
}

//******************************************************************************
template <typename DATA, typename SUBJECT>
Dispatcher<DATA, SUBJECT>::~Dispatcher()
{
    DeathNotification();
}

template <typename DATA, typename SUBJECT>
void Dispatcher<DATA, SUBJECT>::Broadcast(DATA& data_)
{
    BOOST_FOREACH(CB_t *iter, m_subscribers)
    {
        if(NULL != iter)
        {
            iter->Update(data_);
        }
    }
    m_subscribers.remove(NULL);
}

template <typename DATA, typename SUBJECT>
void Dispatcher<DATA, SUBJECT>::Subscribe(CallbackBase<DATA, SUBJECT> *s_)
{
    m_subscribers.push_back(s_);
}

template <typename DATA, typename SUBJECT>
void Dispatcher<DATA, SUBJECT>::UnSubscribe(CallbackBase<DATA, SUBJECT> *s_)
{
    typename std::list<CallbackBase<DATA, SUBJECT> *>::iterator toRemove = 
                    std::find(m_subscribers.begin(), m_subscribers.end(), s_);
    if(m_subscribers.end() != toRemove)
    {
        *toRemove = NULL;
    }
}


template <typename DATA, typename SUBJECT>
void Dispatcher<DATA, SUBJECT>::DeathNotification()
{
    BOOST_FOREACH(CB_t *iter, m_subscribers)
    {
        if(NULL != iter)
        {
            iter->UpdateDispatcherDeath();
        }
    }
}

} // namespace ilrd

#endif // ILRD_RD9315_DISPATCHER_CALLBACK_IMP_HPP