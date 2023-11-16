// standard includes:
#include <iostream>

// 3rd party includes:
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

// local includes:
#include "factory.hpp"
#include "ti_factions.hpp"

namespace ti
{

enum TIFactions
{
    LAZAX = 0,
    L1Z1X,
    HACAN,
    NAALU,
    JOLNAR,
    XXCHA,
    MUAAT,
    NUM_OF_FACTIONS
};

FactionStats lazaxStats   = {0, 1, 0, 3, 3, 0, 6, 1, 2};
FactionStats l1z1xStats   = {0, 1, 1, 0, 0, 3, 5, 1, 1};
FactionStats hacanStats   = {0, 0, 2, 1, 0, 2, 4, 1, 0};
FactionStats naaluStats   = {0, 0, 1, 1, 1, 4, 4, 1, 1};
FactionStats jolnarStats  = {0, 1, 2, 0, 0, 1, 2, 1, 2};
FactionStats xxchaStats   = {0, 0, 1, 3, 0, 3, 2, 1, 1};
FactionStats muaatStats   = {1, 0, 0, 0, 0, 2, 4, 1, 0};

Faction *CreateLazax(const FactionStats& stats_)
{
    return new Lazax(stats_);
}

Faction *CreateL1Z1X(const FactionStats& stats_)
{
    return new L1z1x(stats_);
}

Faction *CreateHacan(const FactionStats& stats_)
{
    return new Hacan(stats_);
}

Faction *CreateNaalu(const FactionStats& stats_)
{
    return new Naalu(stats_);
}

Faction *CreateJolNar(const FactionStats& stats_)
{
    return new JolNar(stats_);
}

Faction *CreateXxcha(const FactionStats& stats_)
{
    return new Xxcha(stats_);
}

Faction *CreateMuaat(const FactionStats& stats_)
{
    return new Muaat(stats_);
}

typedef boost::function<ti::Faction *(const ti::FactionStats&)> TIFunc_t;

TIFunc_t funcArr[NUM_OF_FACTIONS] = {CreateLazax, CreateL1Z1X, CreateHacan, CreateNaalu, CreateJolNar, CreateXxcha, CreateMuaat};

FactionStats statsArr[NUM_OF_FACTIONS] = {lazaxStats, l1z1xStats, hacanStats, naaluStats, jolnarStats, xxchaStats, muaatStats};

} // namespace ti

int main()
{
    ilrd::Factory<ti::Faction *, ti::TIFactions, ti::TIFunc_t, ti::FactionStats> tiFact;
    
    for (size_t i = 0; ti::NUM_OF_FACTIONS > i; ++i)
    {
        tiFact.Add(static_cast<ti::TIFactions>(i), ti::funcArr[i]);
    }

    std::vector<boost::shared_ptr<ti::Faction> > factionsVector;

    for (size_t i = 0; ti::NUM_OF_FACTIONS > i; ++i)
    {
        factionsVector.push_back(boost::shared_ptr<ti::Faction>(
        tiFact.Create(static_cast<ti::TIFactions>(i), ti::statsArr[i])));
    }

    ti::Flagship flagshipsArr[ti::NUM_OF_FACTIONS];

    for (size_t i = 0; ti::NUM_OF_FACTIONS > i; ++i)
    {
        flagshipsArr[i] = factionsVector[i]->SummonFlagship();
    }

    for (size_t i = 0; ti::NUM_OF_FACTIONS > i; ++i)
    {
        std::cout << "The " << factionsVector[i]->GetName() << " have a flagship called: ";
        std::cout << flagshipsArr[i].name <<std::endl;
    }

    return 0;
}
