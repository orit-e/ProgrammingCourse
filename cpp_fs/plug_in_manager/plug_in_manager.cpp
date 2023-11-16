#include <string>

#include "plug_in_manager.hpp"
#include "../dir_monitor/dir_monitor.hpp"

namespace ilrd
{

PlugInManager::PlugInManager(std::string dirName_)
: m_dirMonitor(dirName_)
, m_dllLoader(m_dirMonitor)
{
    // Empty on purpose.
}

} // namespace ilrd

