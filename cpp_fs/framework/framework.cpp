/*******************************************************************************
 * 																			   *
 * 					File name:			framework.cpp		         	       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Tomer								   *
 * 																			   *
 * ****************************************************************************/
#include <boost/bind.hpp>

#include <stdlib.h>

#include "framework.hpp"
#include "../input_mediator/input_mediator.hpp"
#include "../plug_in_manager/plug_in_manager.hpp"
#include "../logger/logger.hpp"
#include "../factory/factory.hpp"
#include "../singleton/singleton.hpp"

namespace ilrd
{

const char *Framework::FW_PLUG_IN_DIR_ENV = "FW_PLUG_IN_DIR_ENV";

Framework::Framework()
: m_threadPool(1)
, m_logger(Singleton<Logger>::GetInstance())
, m_factory(Singleton<CommandFactory_t>::GetInstance())
, m_plugInManager(GetPlugInPath())
, m_inputMediator(boost::bind(&Framework::CreatorFunc, this, _1), &m_threadPool)
{
    m_threadPool.Run();
    m_inputMediator.ReactorStart();
}

void Framework::InputMediatorRegister(boost::shared_ptr<Proxy> proxy_, 
                                        Fd_t readFd_)
{
    m_inputMediator.Subscribe(proxy_, readFd_);
}

void Framework::FactoryRegister(FKey_t key_, CommandCreator_t creator_)
{
    m_factory.Add(key_, creator_);
}

Framework::FCommand_t Framework::CreatorFunc(boost::shared_ptr<Data> data_)
{
    return m_factory.Create(data_->s_str, data_->s_data);
}

const std::string Framework::GetPlugInPath()
{
    char *dirName = getenv(FW_PLUG_IN_DIR_ENV);
    if(NULL == dirName)
    {
        return "./plug_in_dir";
    }

    return dirName;
}

} // namespace ilrd