#ifndef SIRESYSTEM_SIMSYSTEM_H
#define SIRESYSTEM_SIMSYSTEM_H

#include "sireglobal.h"

#include <boost/noncopyable.hpp>

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

/** This class holds a system that is being actively
    simulated.

    While System is an implicitly-shared class,
    SimSystem is designed to be a non-copyable
    class (as a Simulation is occuring only once).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SimSystem : public boost::noncopyable
{

public:
    SimSystem(System &system);
    SimSystem(SystemData &sysdata, ForceFieldsBase &ffields);

    virtual ~SimSystem();

    const SystemData& info() const;
    const ForceFieldsBase& forceFields() const;

    double energy(const Function &component);
    double energy(const FFComponent &component);

    System checkpoint() const;
    void setSystem(System &newsystem);

    const MoleculeGroup& group(MoleculeGroupID id) const;

    const MoleculeGroup& group(const MoleculeGroup &group) const;

    const QHash<MoleculeGroupID,MoleculeGroup>& groups() const;

    SystemID ID() const;

    Version version() const;

    void change(const Molecule &molecule);
    void change(const Residue &residue);
    void change(const NewAtom &atom);

    void remove(const Molecule &molecule);

private:
    /** Reference to the data of the System being simulated */
    SystemData &sysdata;
    
    /** Reference to the forcefields that are used to 
        calculate the energy / forces */
    ForceFieldsBase &ffields;
};

}

SIRE_END_HEADER

#endif
