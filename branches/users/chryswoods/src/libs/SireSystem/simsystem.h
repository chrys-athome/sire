#ifndef SIRESYSTEM_SIMSYSTEM_H
#define SIRESYSTEM_SIMSYSTEM_H

#include <boost/noncopyable.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class Version;
}

namespace SireCAS
{
class Function;
}

namespace SireMol
{
class Molecule;
class Residue;
class NewAtom;
}

namespace SireFF
{
class FFComponent;
class ForceField;
class ForceFieldID;
}

namespace SireSystem
{

class SystemData;
class System;
class SystemID;
class MoleculeGroup;
class MoleculeGroupID;

using SireCAS::Function;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;

using SireFF::ForceField;
using SireFF::ForceFieldID;
using SireFF::FFComponent;

using SireBase::Version;

/** This pure-virtual class provides the interface to a simulation
    system that is being simulated. This allows the
    System to be simulated both in the local process
    and also on a cluster.

    While System is an implicitly-shared class,
    SimSystem is designed to be a non-copyable
    class (as a Simulation is occuring only once).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SimSystem : public boost::noncopyable
{

//friend so that it can see the makeSystem function 
//so that System can make 'makeSystem' a friend!
friend class System;

public:
    SimSystem();

    virtual ~SimSystem();

    virtual double energy(const Function &component)=0;
    virtual double energy(const FFComponent &component)=0;

    virtual System checkpoint()=0;

    virtual void rollback(const System &checkpoint)=0;

    virtual MoleculeGroup group(MoleculeGroupID id)=0;

    virtual QHash<MoleculeGroupID,MoleculeGroup> groups()=0;

    virtual SystemID ID()=0;

    virtual Version version()=0;

    virtual void change(const Molecule &molecule)=0;
    virtual void change(const Residue &residue)=0;
    virtual void change(const NewAtom &atom)=0;

protected:
    static System makeSystem(const SystemData &data,
                             const QHash<ForceFieldID,ForceField> &ffields);
};

}

SIRE_END_HEADER

#endif
