#ifndef ILRD_RD9315_DERIVED_CLASSES_HPP
#define ILRD_RD9315_DERIVED_CLASSES_HPP

#include <vector>
#include <boost/foreach.hpp>

#include "dispatcher_callback.hpp"
#include "../utils/utils.hpp"

namespace ilrd
{

class DispatcherEnvelop
{
public:
    void Broadcast(int& data_);
    void Subscribe(CallbackBase<int, DispatcherEnvelop> *s_);
    void UnSubscribe(CallbackBase<int, DispatcherEnvelop> *s_);
private:
    Dispatcher<int, DispatcherEnvelop> m_dispacher;
};


template<typename DATA, typename OBSERVER_PTR>
class CallbackRemove : public CallbackBase<DATA, DispatcherEnvelop>
{
public:
    CallbackRemove(DispatcherEnvelop *d_, 
                    OBSERVER_PTR& other_);
    virtual void Update(DATA& data_);
private:
    DispatcherEnvelop *m_dRemove;
    OBSERVER_PTR& m_other;

    virtual void UpdateDispatcherDeathAction();
};

template<typename DATA, typename OBSERVER_PTR>
CallbackRemove<DATA, OBSERVER_PTR>::CallbackRemove(DispatcherEnvelop *d_, 
                                                    OBSERVER_PTR& other_)
: CallbackBase<DATA, DispatcherEnvelop>(d_)
, m_dRemove(d_)
, m_other(other_)
{
    // Empty on purpose.
}

template<typename DATA, typename OBSERVER_PTR>
void CallbackRemove<DATA, OBSERVER_PTR>::Update(DATA& data_)
{
    data_ += 1;
    m_dRemove->UnSubscribe(m_other);
    m_other->UpdateDispatcherDeath();
    m_other = NULL;
}

template<typename DATA, typename OBSERVER_PTR>
void CallbackRemove<DATA, OBSERVER_PTR>::UpdateDispatcherDeathAction()
{

}

//******************************************************************************
template<typename DATA>
class CallbackRemoveAll : public CallbackBase<DATA, DispatcherEnvelop>
{
public:
    typedef CallbackBase<DATA, DispatcherEnvelop> RMCB_t;
    CallbackRemoveAll(DispatcherEnvelop *d_,
                std::vector<RMCB_t**> allOfThem_,
                        int& checkRemove_);
    virtual ~CallbackRemoveAll() noexcept;
    virtual void Update(DATA& data_);
private:
    DispatcherEnvelop *m_dRemoveAll;
    std::vector<RMCB_t**> m_allOfThem;
    int& m_checkRemove;
    virtual void UpdateDispatcherDeathAction();
};

template<typename DATA>
CallbackRemoveAll<DATA>::CallbackRemoveAll(DispatcherEnvelop *d_,
                std::vector<RMCB_t**> allOfThem_,
                                int& checkRemove_)
: CallbackBase<DATA, DispatcherEnvelop>(d_)
, m_dRemoveAll(d_)
, m_allOfThem(allOfThem_)
, m_checkRemove(checkRemove_)
{
    // Empty on purpose.
}

template<typename DATA>
CallbackRemoveAll<DATA>::~CallbackRemoveAll() noexcept
{
    // Empty on purpose.
}

template<typename DATA>
void CallbackRemoveAll<DATA>::Update(DATA& data_)
{
    data_ += 1;
    BOOST_FOREACH(RMCB_t **iter, m_allOfThem)
    {
        m_dRemoveAll->UnSubscribe(*iter);
    }
    m_dRemoveAll->UnSubscribe(this);
}

template<typename DATA>
void CallbackRemoveAll<DATA>::UpdateDispatcherDeathAction()
{
    m_checkRemove += 1;
}

//******************************************************************************
template<typename DATA>
class CallBackAdd : public CallbackBase<DATA, DispatcherEnvelop>
{
public:
    CallBackAdd(DispatcherEnvelop *d_, 
                SimpleCallback<DATA, DispatcherEnvelop> **cPtr_,
        typename CallbackBase<DATA, DispatcherEnvelop>::updateFunc_t update_,
        typename CallbackBase<DATA, DispatcherEnvelop>::updateDeathFunc_t 
                                                                updateDeath_);
    virtual ~CallBackAdd() noexcept;
    virtual void Update(DATA& data_);
private:
    DispatcherEnvelop *m_dAdd;
    SimpleCallback<DATA, DispatcherEnvelop> **m_cPtr;
    typename CallbackBase<DATA, DispatcherEnvelop>::updateFunc_t m_update;
    typename CallbackBase<DATA, DispatcherEnvelop>::updateDeathFunc_t 
                                                                m_updateDeath;

    virtual void UpdateDispatcherDeathAction();
};

template<typename DATA>
CallBackAdd<DATA>::CallBackAdd(DispatcherEnvelop *d_, 
                            SimpleCallback<DATA, DispatcherEnvelop> **cPtr_,
        typename CallbackBase<DATA, DispatcherEnvelop>::updateFunc_t update_,
        typename CallbackBase<DATA, DispatcherEnvelop>::updateDeathFunc_t 
                                                                updateDeath_)
: CallbackBase<DATA, DispatcherEnvelop>(d_)
, m_dAdd(d_)
, m_cPtr(cPtr_)
, m_update(update_)
, m_updateDeath(updateDeath_)
{
    // Empty on purpose.
}

template<typename DATA>
CallBackAdd<DATA>::~CallBackAdd() noexcept
{
    // Empty on purpose.
}

template<typename DATA>
void CallBackAdd<DATA>::Update(DATA& data_)
{
    *m_cPtr = new SimpleCallback<DATA, DispatcherEnvelop>(m_dAdd, m_update, 
                                                            m_updateDeath);
    data_ += 1;
}

template<typename DATA>
void CallBackAdd<DATA>::UpdateDispatcherDeathAction()
{
    // Empty on purpose.
}
} // namespace ilrd

#endif // ILRD_RD9315_DERIVED_CLASSES_HPP