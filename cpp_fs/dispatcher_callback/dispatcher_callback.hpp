/*******************************************************************************
 * 																			   *
 * 					File name:			dispatcher_callback.hpp		    	   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Dvir								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_DISPATCHER_CALLBACK_HPP
#define ILRD_RD9315_DISPATCHER_CALLBACK_HPP

#include <iosfwd>
#include <list>
#include <boost/core/noncopyable.hpp>
#include <boost/function.hpp>

#include "../utils/utils.hpp"

namespace ilrd
{
template <typename DATA, typename SUBJECT>
class CallbackBase : private boost::noncopyable
{
public:
    typedef boost::function<void(DATA&)> updateFunc_t;
    typedef boost::function<void()> updateDeathFunc_t;

    CallbackBase(SUBJECT *d_);
    virtual ~CallbackBase() noexcept;

    virtual void Update(DATA& data_) = 0;
    void UpdateDispatcherDeath();
protected:
    void UnSubscribe(CallbackBase *s_);
private:
    SUBJECT *m_dispatcher;

    virtual void UpdateDispatcherDeathAction() = 0;
};


template <typename DATA, typename SUBJECT>
class SimpleCallback : public CallbackBase<DATA, SUBJECT>
{
public:
    SimpleCallback(SUBJECT *d_, typename CallbackBase<DATA, 
                    SUBJECT>::updateFunc_t update_, 
                    typename CallbackBase<DATA, 
                    SUBJECT>::updateDeathFunc_t updateDeath_);
    virtual ~SimpleCallback() noexcept;
    virtual void Update(DATA& data_);
private:
    SUBJECT *m_dSimple;
    typename CallbackBase<DATA, SUBJECT>::updateFunc_t m_update;
    typename CallbackBase<DATA, SUBJECT>::updateDeathFunc_t m_updateDeath;

    virtual void UpdateDispatcherDeathAction();
};


template <typename DATA, typename SUBJECT>
class Dispatcher : private boost::noncopyable
{
public:
    typedef CallbackBase<DATA, SUBJECT> CB_t;
    //compiler generated:
    //Dispatcher();
    ~Dispatcher();

    void Broadcast(DATA& data_);
    void Subscribe(CB_t *s_);
    void UnSubscribe(CB_t *s_);

private:
    std::list<CB_t *> m_subscribers;
    
    void DeathNotification();
};

} // namespace ilrd

#include "dispatcher_callback_imp.hpp"

#endif // ILRD_RD9315_DISPATCHER_CALLBACK_HPP