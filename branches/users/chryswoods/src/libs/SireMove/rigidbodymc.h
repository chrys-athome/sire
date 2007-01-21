#ifndef SIREMOVE_RIGIDBODYMC_H
#define SIREMOVE_RIGIDBODYMC_H

#include <QHash>

#include "montecarlo.h"
#include "sampler.h"

#include "SireMaths/angle.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RigidBodyMC;
}

QDataStream& operator<<(QDataStream&, const SireMove::RigidBodyMC&);
QDataStream& operator>>(QDataStream&, SireMove::RigidBodyMC&);

namespace SireSystem
{
class SimSystem;
}

namespace SireMol
{
class MoleculeID;
class MoleculeGroup;
}

namespace SireMove
{

class Sampler;

using SireSystem::SimSystem;

using SireMol::MoleculeGroup;
using SireMol::MoleculeID;

/** This class implements a rigid body Monte Carlo move that
    may be applied to a random molecule within a MoleculeGroup
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RigidBodyMC : public MonteCarlo
{

friend QDataStream& ::operator<<(QDataStream&, const RigidBodyMC&);
friend QDataStream& ::operator>>(QDataStream&, RigidBodyMC&);

public:
    RigidBodyMC();

    RigidBodyMC(const MoleculeGroup &group,
                const Sampler &sampler);

    RigidBodyMC(const MoleculeGroup &group,
                const RanGenerator &generator,
                const Sampler &sampler);

    RigidBodyMC(const RigidBodyMC &other);

    ~RigidBodyMC();

    /** Small class used to hold the maximum amounts by
        which the molecule can be translated and rotated */
    class Delta
    {
    public:
        Delta(double adel=0, SireMaths::Angle rdel=0);
        ~Delta();

        double maximumTranslation() const
        {
            return adel;
        }

        SireMaths::Angle maximumRotation() const
        {
            return rdel;
        }

    private:
        /** Maximum translation (in A) */
        double adel;
        SireMaths::Angle rdel;
    };

    /** Small class used to hold a record of the number
        of moves on a molecule, and whether or not they
        were successful */
    class MoveRecord
    {
    public:
        MoveRecord();
        ~MoveRecord();

        void incrementAccept();
        void incrementReject();

        quint32 nAttempted();

        quint32 nAccepted();
        quint32 nRejected();

    private:
        /** The total number of accepted moves */
        quint32 naccept;
        /** The total number of rejected moves */
        quint32 nreject;
    };

    /** Small class that holds everything necessary to checkpoint
        the MC move */
    class CheckPoint : public MonteCarlo::CheckPoint
    {
    public:
        CheckPoint();

        CheckPoint(const SimSystem &system,
                   const Sampler &sampler);

        CheckPoint(const CheckPoint &other);

        ~CheckPoint();

    private:
        /** We need to checkpoint our sampler */
        Sampler smplr;
    };

    static const char* typeName()
    {
        return "SireMove::RigidBodyMC";
    }

    const char* what() const
    {
        return RigidBodyMC::typeName();
    }

    RigidBodyMC* clone() const
    {
        return new RigidBodyMC(*this);
    }

    void initialise(const SimSystem &system);

    void move(SimSystem &system);

    quint32 nAttempted() const;
    quint32 nAccepted() const;
    quint32 nRejected() const;

    double acceptanceRatio() const;

    quint32 nAttempted(const Molecule &mol) const;
    quint32 nAccepted(const Molecule &mol) const;
    quint32 nRejected(const Molecule &mol) const;

    double acceptanceRatio(const Molecule &mol) const;

protected:
    RigidBodyMC::Delta maximumDelta(const Molecule &mol) const;

    void incrementAccept(const Molecule &mol);
    void incrementReject(const Molecule &mol);

private:
    /** Hash containing the maximum amount by which
        each molecule can be translated or rotated */
    QHash<MoleculeID, RigidBodyMC::Delta> maxdelta;

    /** Hash mapping the number of attempted and accepted moves
        for each molecule */
    QHash<MoleculeID, RigidBodyMC::MoveRecord> moverecord;
    
    /** The sampler used to select random molecules from the 
        MoleculeGroup */
    Sampler smplr;
};

}

Q_DECLARE_METATYPE(SireMove::RigidBodyMC);

SIRE_END_HEADER

#endif
