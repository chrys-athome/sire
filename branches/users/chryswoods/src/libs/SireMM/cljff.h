#ifndef SIREMM_CLJFF_H
#define SIREMM_CLJFF_H

#include "SireFF/ffworker.h"
#include "SireMaths/maths.h"

#include "SireVol/simvolume.h"
#include "SireUnits/units.h"

#include "cljpair.h"

namespace SireMM
{

using SireFF::ForceField;
using SireVol::DistMatrix;

namespace detail
{

class CLJWorkspace
{
public:
    CLJWorkspace();

    CLJWorkspace(const CLJWorkspace &other)
           : distmatrix(other.distmatrix),
             cljmatrix(other.cljmatrix),
             cnrg(other.cnrg), ljnrg(other.ljnrg)
    {}

    ~CLJWorkspace();

    /** The distance matrix used to hold the distances
        between all atoms of two CoordGroups */
    DistMatrix distmatrix;

    /** The pair matrix used to store all of the CLJ
        parameters for a pair of atoms */
    CLJPairMatrix cljmatrix;

    /** The accumulated value of the coulomb energy */
    double cnrg;

    /** The accumulated value of the LJ energy */
    double ljnrg;
};

}

/**
A CLJFF is the base-class for all forcefields that implement a charge/Lennard Jones (CLJ) forcefield. This provides basic functions for calculating the CLJ energy of pairs of CutGroups.

@author Christopher Woods
*/
class CLJFF : public FFWorker
{

public:
    CLJFF();
    CLJFF(const CLJFF &other);

    ~CLJFF();

    const Space& space() const;

protected:
    static void calculateEnergy(const CoordGroup &group0,
                                const QVector<CLJParameter> &params0,
                                const CoordGroup &group1,
                                const QVector<CLJParameter> &params1,
                                const Space &space,
                                const CombiningRules &combrules,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const CoordGroup &group,
                                const QVector<CLJParameter> &params,
                                const Space &space,
                                const CombiningRules &combrules,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const detail::MolCLJInfo &mol0,
                                const detail::MolCLJInfo &mol1,
                                const Space &space,
                                const CombiningRules &combrules,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const detail::MolCLJInfo &mol,
                                const Space &space,
                                const CombiningRules &combrules,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    void setSpace(const Space &space);

private:
    static void calculatePairEnergy(detail::CLJWorkspace &workspace);
    static void calculateSelfEnergy(detail::CLJWorkspace &workspace);

    /** The space in which the calculation will be performed */
    Space spce;
};

}

#endif
