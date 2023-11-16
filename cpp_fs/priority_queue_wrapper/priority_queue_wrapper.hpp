/*******************************************************************************
 * 																			   *
 * 					File name:			priority_queue_wrapper.hpp		  	   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Guy B								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_PRIORITY_QUEUE_WRAPPER_HPP
#define ILRD_RD9315_PRIORITY_QUEUE_WRAPPER_HPP

#include <vector>       // std::vector
#include <queue>        // std::priority_queue
#include <functional>   // std::less

namespace ilrd
{


template <typename T, typename Q_CONTAINER = std::vector<T>, 
            typename COMPARE = std::less<T> >
class PQWrapper : private std::priority_queue<T, Q_CONTAINER, COMPARE>
{
private:
    typedef std::priority_queue<T, Q_CONTAINER, COMPARE> pq_t;
public:
    typedef typename Q_CONTAINER::value_type value_type;
    typedef typename Q_CONTAINER::reference reference;

    using pq_t::push;
    using pq_t::pop;
    using pq_t::empty;

    // compiler genrated:
    //explicit PQWrapper(const COMPARE& compare = COMPARE(),
    //                   const Q_CONTAINER& cont = Q_CONTAINER());
    // ~PQWrapper();
    // PQWrapper(const PQWrapper& other_);
    // PQWrapper& operator=(const PQWrapper& other_);

    const T& front();
};

template <typename T, typename Q_CONTAINER, typename COMPARE>
const T& PQWrapper<T, Q_CONTAINER, COMPARE>::front()
{
    return pq_t::top();
}

} // namespace ilrd

#endif // ILRD_RD9315_PRIORITY_QUEUE_WRAPPER_HPP
