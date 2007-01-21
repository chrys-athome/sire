#ifndef SIREMOVE_MONTECARLO_H
#define SIREMOVE_MONTECARLO_H

#include "SireSystem/move.h"

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MonteCarlo;
}

QDataStream& operator<<(QDataStream&, const SireMove::MonteCarlo&);
QDataStream& operator>>(QDataStream&, SireMove::MonteCarlo&);

namespace SireSim
{
class Simulation;
}

namespace SireMove
{

using SireSim::Simulation;

using SireSystem::MoveBase;

using SireMaths::RanGenerator;

/** This is the base class of all Monte Carlo moves

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MonteCarlo : public MoveBase
{

friend QDataStream& ::operator<<(QDataStream&, const MonteCarlo&);
friend QDataStream& ::operator>>(QDataStream&, MonteCarlo&);

public:
    MonteCarlo();
    MonteCarlo(const RanGenerator &generator);

    MonteCarlo(const MonteCarlo &other);

    ~MonteCarlo();

    MonteCarlo& operator=(const MonteCarlo &other);

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    class CheckPoint
    {
    public:
        CheckPoint();

        CheckPoint(const Simulation &simulation);

        CheckPoint(const CheckPoint &other);

        ~CheckPoint();
    };

protected:
    RanGenerator& generator();

private:
    /** The random number generator used to generate
        and test this Monte Carlo moves */
    RanGenerator rangen;
};

/** Internal function used to return the random number 
    generator used by this move */
inline RanGenerator& MonteCarlo::generator()
{
    return rangen;
}

}

SIRE_END_HEADER

#endif
