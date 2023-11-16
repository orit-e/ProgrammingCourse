/*******************************************************************************
 * 																			   *
 * 					File name:			factory.hpp		           		       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_FACTORY_HPP
#define ILRD_RD9315_FACTORY_HPP

// standard includes:
#include <map>  // std::map< >

// 3rd party includes:
#include <boost/core/noncopyable.hpp>   // boost::noncopyable

// local includes:
#include "../utils/utils.hpp"

namespace ilrd
{

// CALLABLE should receive DATA and return BASE
template <typename BASE, typename KEY, typename CALLABLE, typename DATA>
class Factory : private boost::noncopyable
{
public:
    class BadKeyException : public std::exception
    {
    public:
        virtual const char *what() const noexcept;
    };

    // compiler generated:
    // Factory();
    // ~Factory() noexcept;

    void Add(const KEY& key_, const CALLABLE& func_);
    BASE Create(const KEY& key_, DATA param_); // may throw BadKeyException 

private:
    std::map<KEY, CALLABLE> m_container;
};

} // namespace ilrd

#include "factory_imp.hpp"

#endif // ILRD_RD9315_FACTORY_HPP
