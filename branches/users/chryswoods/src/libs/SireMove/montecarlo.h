#ifndef SIREMOVE_MONTECARLO_H
#define SIREMOVE_MONTECARLO_H

#include "move.h"

#include "SireSystem/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireSim
{
class Simulation;
}

namespace SireMove
{

using SireSim::Simulation;
using SireSystem::MoleculeGroup;

/** This is the base class of all Monte Carlo moves

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MonteCarlo : public MoveBase
{
public:
    MonteCarlo();

    MonteCarlo(const MonteCarlo &other);

    ~MonteCarlo();

    void setSeed(int seed);

    class CheckPoint
    {
    public:
        CheckPoint();

        CheckPoint(const Simulation &simulation);

        CheckPoint(const CheckPoint &other);

        ~CheckPoint();
    };

protected:
    Vector randomVectorOnASphere(double radius);

    void rand();

private:

    /** The random number generator used to generate
        random numbers between 0 and 1 (inclusive) */
    UniformRand uni_rand_0_1;
};

}

SIRE_END_HEADER

#endif
