/*******************************************************************************
 * 																			   *
 * 					File name:			framework.יpp		         	       *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Tomer								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_FRAMEWORK_HPP
#define ILRD_RD9315_FRAMEWORK_HPP

#include <iosfwd>

#include <boost/core/noncopyable.hpp>   // boost::noncopyable

#include "../input_mediator/input_mediator.hpp"
#include "../plug_in_manager/plug_in_manager.hpp"
#include "../logger/logger.hpp"
#include "../factory/factory.hpp"

namespace ilrd
{

class Framework : private boost::noncopyable
{
public:
    typedef int                                                Fd_t;
    // default dir is: "./plug_in_dir".
    // logger searches for text file path at "LOGGER_FILE_PATH".
    static const char *FW_PLUG_IN_DIR_ENV;

    typedef std::string                                        FKey_t;
    typedef boost::shared_ptr<Command>                         FCommand_t;
    typedef boost::shared_ptr<void>                            FData_t;
    typedef boost::function<FCommand_t(FData_t)>               CommandCreator_t;

    Framework();

    void InputMediatorRegister(boost::shared_ptr<Proxy> proxy_, Fd_t readFd_);
    void FactoryRegister(FKey_t key_, CommandCreator_t creator_);

private:
    typedef Factory<FCommand_t, FKey_t, CommandCreator_t, 
                    FData_t>                                   CommandFactory_t;

    ThreadPool m_threadPool;
    Logger& m_logger;
    CommandFactory_t& m_factory;
    PlugInManager m_plugInManager;
    InputMediator m_inputMediator;

    FCommand_t CreatorFunc(boost::shared_ptr<Data> data_);

    const std::string GetPlugInPath();
};

} // namespace ilrd

#endif // ILRD_RD9315_FRAMEWORK_HPP
