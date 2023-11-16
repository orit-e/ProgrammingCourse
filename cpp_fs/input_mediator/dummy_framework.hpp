#ifndef ILRD_RD9513_DUMMY_FRAMEWORK_HPP
#define ILRD_RD9513_DUMMY_FRAMEWORK_HPP

#include <iosfwd>
#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "../factory/factory.hpp"
#include "../thread_pool/thread_pool.hpp"
#include "input_mediator.hpp"

namespace ilrd
{

class FrameWork
{
public:
    explicit FrameWork();
    ~FrameWork();

    InputMediator::cmd_t CreatorFunc(boost::shared_ptr<Data> data_);

    Factory<InputMediator::cmd_t, std::string, 
            boost::function<InputMediator::cmd_t(boost::shared_ptr<void>)>, 
            boost::shared_ptr<void> > m_factory;
    ThreadPool *m_tP;
    InputMediator m_iM;
};

} // namespace ilrd

#endif // ILRD_RD9513_DUMMY_FRAMEWORK_HPP