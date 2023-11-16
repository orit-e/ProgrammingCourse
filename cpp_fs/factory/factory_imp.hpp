/*******************************************************************************
 * 																			   *
 * 					File name:			factory_imp.hpp		       		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_FACTORY_IMP_HPP
#define ILRD_RD9315_FACTORY_IMP_HPP

// standard includes:
#include <map>  // std::map< >

#include "factory.hpp"


namespace ilrd
{
template <typename BASE, typename KEY, typename CALLABLE, typename DATA>
void Factory<BASE, KEY, CALLABLE, DATA>::Add(const KEY& key_, 
                                                const CALLABLE& func_)
{
    std::pair<KEY, CALLABLE> tmp(key_, func_);
    std::pair<typename std::map<KEY, CALLABLE>::iterator, bool> status = 
                    m_container.insert(tmp);
    if(false == status.second)
    {
        m_container.erase(key_);
        m_container.insert(tmp);
    }
}

template <typename BASE, typename KEY, typename CALLABLE, typename DATA>
BASE Factory<BASE, KEY, CALLABLE, DATA>::Create(const KEY& key_, DATA param_)
{
    typename std::map<KEY, CALLABLE>::iterator iter = m_container.find(key_);
    if(m_container.end() == iter)
    {
        throw(BadKeyException());
    }

    return iter->second(param_);
}

template <typename BASE, typename KEY, typename CALLABLE, typename DATA>
const char *
    Factory<BASE, KEY, CALLABLE, DATA>::BadKeyException::what() const noexcept
{
    return "Create(): Bad Key Inserted.";
}


} // namespace ilrd


#endif // ILRD_RD9315_FACTORY_IMP_HPP