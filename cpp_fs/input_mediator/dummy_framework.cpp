#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "../factory/factory.hpp"
#include "../thread_pool/thread_pool.hpp"
#include "input_mediator.hpp"

#include "dummy_framework.hpp"

namespace ilrd
{

FrameWork::FrameWork()
: m_factory()
, m_tP(new ThreadPool(1ul))
, m_iM(boost::bind(&FrameWork::CreatorFunc, this, _1), m_tP)
{
    m_tP->Run();
    m_iM.ReactorStart();
}

FrameWork::~FrameWork()
{
    m_iM.ReactorStop();
    delete m_tP;
    m_tP = NULL;
}

InputMediator::cmd_t FrameWork::CreatorFunc(boost::shared_ptr<Data> data_)
{
    return m_factory.Create(data_->s_str, data_->s_data);
}

}