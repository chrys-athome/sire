/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
class MoleculeGroup;
class MoleculeGroups;
class MoleculeGroupID;
}

namespace SireFF
{
class FFComponent;
class ForceField;
class ForceFieldID;
class ForceFieldsBase;
}

namespace SireSystem
{

class SystemData;
class System;
class SystemID;

using SireCAS::Function;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::MoleculeGroup;
using SireMol::MoleculeGroups;
using SireMol::MoleculeGroupID;

using SireFF::ForceField;
using SireFF::ForceFieldsBase;
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

    System checkpoint() const;
    void setSystem(System &newsystem);

    const MoleculeGroup& group(MoleculeGroupID id) const;

    const MoleculeGroup& group(const MoleculeGroup &group) const;

    const MoleculeGroups& groups() const;

    SystemID ID() const;

    const Version& version() const;

    void updateStatistics();

    //interface to ForceFields
    double energy(const Function &component);
    double energy(const FFComponent &component);

    void setProperty(const QString &name, const Property &property)=0;
    void setProperty(ForceFieldID ffid, const QString &name, const Property &property);

    void setProperty(const QSet<ForceFieldID> &ffids,
                     const QString &name, const Property &property);

    QSet<ForceFieldID,Property> getProperty(const QString &name) const;

    Property getProperty(ForceFieldID ffid, const QString &name) const;
    QSet<ForceFieldID,Property> getProperty(const QSet<ForceFieldID> &ffids,
                                            const QString &name) const;

    bool containsProperty(const QString &name) const;
    bool containsProperty(ForceFieldID ffid, const QString &name) const;
    QSet<ForceFieldID> forceFieldsWithProperty(const QString &name) const;

    void change(const PartialMolecule &molecule);
    void change(const QHash<MoleculeID,PartialMolecule> &molecules);

    void add(ForceFieldID ffid, const PartialMolecule &molecule);
    void add(ForceFieldID ffid, const QList<PartialMolecule> &molecules);

    void addTo(ForceFieldID ffid, const FFBase::Group &group,
               const PartialMolecule &molecule);
    void addTo(ForceFieldID ffid, const FFBase::Group &group,
               const QList<PartialMolecule> &molecules);

    void remove(const PartialMolecule &molecule);
    void remove(const QList<PartialMolecule> &molecules);
    void remove(ForceFieldID ffid, const PartialMolecule &molecule);
    void remove(ForceFieldID ffid, const QList<PartialMolecule> &molecules);

    void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                    const PartialMolecule &molecule);
    void removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                    const QList<PartialMolecule> &molecules);

    bool contains(const PartialMolecule &molecule, ForceFieldID ffid) const;
    bool contains(const PartialMolecule &molecule,
                  ForceFieldID ffid, const FFBase::Group &group) const;

    QSet<ForceFieldID> forceFieldsContaining(const PartialMolecule &molecule) const;

    bool refersTo(MoleculeID molid) const;
    bool refersTo(MoleculeID molid, ForceFieldID ffid) const;
    bool refersTo(MoleculeID molid, ForceFieldID ffid,
                  const FFBase::Group &group) const;

    QSet<ForceFieldID> forceFieldsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;
    QSet<MoleculeID> moleculeIDs(ForceFieldID ffid) const;
    QSet<MoleculeID> moleculeIDs(ForceFieldID ffid, const FFBase::Group &group) const;

    PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid) const;

    QHash<MoleculeID,PartialMolecule> contents() const;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid) const;
    QHash<MoleculeID,PartialMolecule> contents(ForceFieldID ffid,
                                               const FFBase::Group &group) const;

private:
    template<class T>
    void _pvt_change(const T &obj);

    /** Reference to the data of the System being simulated */
    SystemData &sysdata;

    /** Reference to the forcefields that are used to
        calculate the energy / forces */
    ForceFieldsBase &ffields;

    /** Reference to the monitors that are used to monitor
        and collect statistics about the simulation */
    //SystemMonitors &monitors;
};

}

SIRE_END_HEADER

#endif
