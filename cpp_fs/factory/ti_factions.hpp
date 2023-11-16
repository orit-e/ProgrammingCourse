#ifndef ILRD_RD9315_TI_FACTIONS_HPP
#define ILRD_RD9315_TI_FACTIONS_HPP

#include <string>   // std::string

#include "../utils/utils.hpp"

namespace ti
{

struct FactionStats
{
    int warSun;
    int dreadnought;
    int carriers;
    int cruisers;
    int destroyers;
    int fighters;
    int groundFources;
    int spaceDock;
    int pds;
};

struct Flagship
{
    std::string name;
    int cost;
    int combatDemage;
    int combatDice;
    int move;
    int capacity;
};

class Faction
{
public:
    Faction(const FactionStats& stats_, const std::string& name_);
    virtual ~Faction();

    const FactionStats& GetStats() const noexcept;
    const std::string& GetName() const noexcept;
    virtual Flagship SummonFlagship() = 0;

private:
    FactionStats m_stats;
    const std::string m_name;
};

class Lazax : public Faction
{
public:
    Lazax(const FactionStats&, const std::string& name_ = "Lazax");
    virtual ~Lazax();

    virtual Flagship SummonFlagship();
};

class L1z1x : public Lazax 
{
public:
    L1z1x(const FactionStats&, const std::string& name_ = "L1Z1X Mindnet");
    virtual ~L1z1x();

    virtual Flagship SummonFlagship();
};

class Hacan : public Faction
{
public:
    Hacan(const FactionStats&, const std::string& name_ = "Emirates of Hacan");
    virtual ~Hacan();

    virtual Flagship SummonFlagship();
};

class Naalu : public Faction
{
public:
    Naalu(const FactionStats&, const std::string& name_ = "Naalu Collective");
    virtual ~Naalu();

    virtual Flagship SummonFlagship();
};

class JolNar : public Faction
{
public:
    JolNar(const FactionStats&, const std::string& name_ = "Universities of Jol-Nar");
    virtual ~JolNar();

    virtual Flagship SummonFlagship();
};

class Xxcha : public Faction
{
public:
    Xxcha(const FactionStats&, const std::string& name_ = "Xxcha Kingdom");
    virtual ~Xxcha();

    virtual Flagship SummonFlagship();
};

class Muaat : public Faction
{
public:
    Muaat(const FactionStats&, const std::string& name_ = "Embers of Muaat");
    virtual ~Muaat();

    virtual Flagship SummonFlagship();
};

} // namespace ti

#endif // ILRD_RD9315_TI_FACTIONS_HPP
