#ifndef SIREMM_CLJFF_H
#define SIREMM_CLJFF_H

#include <QVector>

#include "cljpair.h"
#include "combiningrules.h"
#include "switchingfunction.h"

#include "SireVol/space.h"
#include "SireFF/ffbase.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFF&);

namespace SireMM
{

using SireFF::FFBase;
using SireFF::ParameterName;

using SireCAS::Function;

using SireVol::Space;
using SireVol::DistMatrix;
using SireVol::CoordGroup;

namespace detail
{

class MolCLJInfo;
class ChangedMolCLJInfo;
class ResCLJInfo;

class CLJWorkspace
{

public:
    CLJWorkspace();

    /** Copy constructor */
    CLJWorkspace(const CLJWorkspace &other)
           : distmatrix(other.distmatrix),
             cnrg(other.cnrg), ljnrg(other.ljnrg)
    {}

    ~CLJWorkspace();

    /** The distance matrix used to hold the distances
        between all atoms of two CoordGroups */
    DistMatrix distmatrix;

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
class SIREMM_EXPORT CLJFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFF&);
friend QDataStream& ::operator>>(QDataStream&, CLJFF&);

public:
    CLJFF();

    CLJFF(const Space &space, const SwitchingFunction &switchingfunction);

    CLJFF(const CLJFF &other);

    ~CLJFF();

    class SIREMM_EXPORT Components : public FFBase::Components
    {
    public:
        Components();
        Components(const Components &other);
        Components(const QString &basename);

        ~Components();

        Components* clone() const
        {
            return new Components(*this);
        }

        const Function& coulomb() const
        {
            return e_coulomb;
        }

        const Function& lj() const
        {
            return e_lj;
        }

        static QString describe_coulomb();
        static QString describe_lj();

    protected:
        void setBaseName(const QString &basename);

    private:
        /** The coulomb component */
        Function e_coulomb;
        /** The LJ component */
        Function e_lj;
    };

    class SIREMM_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters();
        
        ~Parameters();
        
        /** Return the default source of the coulomb parameters */
        const ParameterName& coulomb() const
        {
            return coulomb_params;
        }
        
        /** Return the default source of the LJ parameters */
        const ParameterName& lj() const
        {
            return lj_params;
        }
        
        static Parameters default_sources;
        
    private:
        /** The name and default source of the coulomb parameters */
        ParameterName coulomb_params;
        /** The name and default source of the LJ parameters */
        ParameterName lj_params;
    };

    const Parameters& parameters() const
    {
        return Parameters::default_sources;
    }

    const Space& space() const;

    const SwitchingFunction& switchingFunction() const;

    static int COULOMB()
    {
        return 1;
    }

    static int LJ()
    {
        return 2;
    }

    /** Return the object describing the components of this
        forcefield */
    const CLJFF::Components& p_components() const
    {
        return *components_ptr;
    }

    const Function& coulomb() const;
    const Function& lj() const;

protected:
    static void calculateEnergy(const CoordGroup &group0,
                                const QVector<ChargeParameter> &chg0,
                                const QVector<LJParameter> &lj0,
                                const CoordGroup &group1,
                                const QVector<ChargeParameter> &chg1,
                                const QVector<LJParameter> &lj1,
                                const Space &space,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const CoordGroup &group,
                                const QVector<ChargeParameter> &chgs,
                                const QVector<LJParameter> &ljs,
                                const Space &space,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const detail::MolCLJInfo &mol0,
                                const detail::MolCLJInfo &mol1,
                                const Space &space,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    static void calculateEnergy(const detail::MolCLJInfo &mol,
                                const Space &space,
                                const SwitchingFunction &switchfunc,
                                detail::CLJWorkspace &workspace);

    detail::CLJWorkspace& workspace();
    const detail::CLJWorkspace& workspace() const;

private:
    static void calculatePairEnergy(detail::CLJWorkspace &workspace,
                                    const QVector<ChargeParameter> &chg0,
                                    const QVector<LJParameter> &lj0,
                                    const QVector<ChargeParameter> &chg1,
                                    const QVector<LJParameter> &lj1);

    static void calculateSelfEnergy(detail::CLJWorkspace &workspace,
                                    const QVector<ChargeParameter> &chgs,
                                    const QVector<LJParameter> &ljs);

    void registerComponents();

    /** The workspace in which the calculation will take place */
    detail::CLJWorkspace wkspace;

    /** The space in which the calculation will be performed */
    Space spce;

    /** The switching function used to truncate the CLJ interactions */
    SwitchingFunction switchfunc;

    /** Pointer to the object containing the components of
        this forcefield */
    const CLJFF::Components *components_ptr;
};

/** Return the space in which this calculation is performed (the volume
    of space in which the molecules exist, e.g. PeriodicBox, TruncatedOctahedron
    etc.) */
inline const Space& CLJFF::space() const
{
    return spce;
}

/** Return the switching function used to truncate the CLJ interaction */
inline const SwitchingFunction& CLJFF::switchingFunction() const
{
    return switchfunc;
}

/** Internal function used to return the workspace that is used to perform
    the calculation */
inline detail::CLJWorkspace& CLJFF::workspace()
{
    return wkspace;
}

/** Internal function used to return the workspace that is used to perform
    the calculation */
inline const detail::CLJWorkspace& CLJFF::workspace() const
{
    return wkspace;
}

}

SIRE_END_HEADER

#endif
