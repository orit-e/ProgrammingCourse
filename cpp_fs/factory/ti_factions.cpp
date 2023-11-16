#include "ti_factions.hpp"

namespace ti
{

// Faction --------------------------------------------------------------------

Faction::Faction(const FactionStats& stats_, const std::string& name_)
: m_stats(stats_)
, m_name(name_)
{
    // empty on purpose
}

Faction::~Faction()
{
    // empty on purpose
}

const FactionStats& Faction::GetStats() const noexcept
{
    return m_stats;
}

const std::string& Faction::GetName() const noexcept
{
    return m_name;
}

// Lazax ----------------------------------------------------------------------

Lazax::Lazax(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

Lazax::~Lazax()
{
    // empty on purpose
}

Flagship Lazax::SummonFlagship()
{
    Flagship lazaxShip = {"Mecatol-Rex", 0, 0, 0, 0, 0};

    return lazaxShip;
}

// L1z1x ----------------------------------------------------------------------

L1z1x::L1z1x(const FactionStats& stats_, const std::string& name_)
: Lazax(stats_, name_)
{
    // empty on purpose
}

L1z1x::~L1z1x()
{
    // empty on purpose
}

Flagship L1z1x::SummonFlagship()
{
    Flagship l1z1xShip = {"[0.0.1]", 10, 5, 2, 1, 2};

    return l1z1xShip;
}

// Hacan ----------------------------------------------------------------------

Hacan::Hacan(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

Hacan::~Hacan()
{
    // empty on purpose
}

Flagship Hacan::SummonFlagship()
{
    Flagship hacanShip = {"Wrath of Kenara", 10, 7 , 3, 2, 4};

    return hacanShip;
}

// Naalu ----------------------------------------------------------------------

Naalu::Naalu(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

Naalu::~Naalu()
{
    // empty on purpose
}

Flagship Naalu::SummonFlagship()
{
    Flagship naaluShip = {"Matriarch", 9, 8, 2, 2, 6};

    return naaluShip;
}

// JolNar ---------------------------------------------------------------------

JolNar::JolNar(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

JolNar::~JolNar()
{
    // empty on purpose
}

Flagship JolNar::SummonFlagship()
{
    Flagship jolnarShip = {"J.N.S Hylarim", 10, 2, 2, 2, 2};

    return jolnarShip;
}

// Xxcha ----------------------------------------------------------------------

Xxcha::Xxcha(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

Xxcha::~Xxcha()
{
    // empty on purpose
}

Flagship Xxcha::SummonFlagship()
{
    Flagship xxchaShip = {"Loncara Ssodu", 9, 6, 2, 1, 4};

    return xxchaShip;
}

// Muaat ----------------------------------------------------------------------

Muaat::Muaat(const FactionStats& stats_, const std::string& name_)
: Faction(stats_, name_)
{
    // empty on purpose
}

Muaat::~Muaat()
{
    // empty on purpose
}

Flagship Muaat::SummonFlagship()
{
    Flagship muaatShip = {"The Inferno", 10, 5, 3, 1, 4};

    return muaatShip;
}

} // namespace ti
