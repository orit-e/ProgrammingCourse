#ifndef ILRD_RD9315_PLUG_IN_MANAGER_HPP
#define ILRD_RD9315_PLUG_IN_MANAGER_HPP

#include <iosfwd>

#include "../utils/utils.hpp"
#include "../dir_monitor/dir_monitor.hpp"

namespace ilrd
{

class PlugInManager
{
public:
    explicit PlugInManager(std::string dirName_);
    //~PlugNPlay();
private:
    DirMonitor  m_dirMonitor;
    DLLLoader   m_dllLoader;
};

} // namespace ilrd

#endif // ILRD_RD9315_PLUG_IN_MANAGER_HPP