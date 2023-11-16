#include "dispatcher_callback.hpp"
#include "derived_classes.hpp"

namespace ilrd
{

void DispatcherEnvelop::Broadcast(int& data_)
{
    m_dispacher.Broadcast(data_);
}

void DispatcherEnvelop::Subscribe(CallbackBase<int, DispatcherEnvelop> *s_)
{
    m_dispacher.Subscribe(s_);
}

void DispatcherEnvelop::UnSubscribe(CallbackBase<int, DispatcherEnvelop> *s_)
{
    m_dispacher.UnSubscribe(s_);
}

} // namespace ilrd