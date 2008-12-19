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

#ifndef SIRESYSTEM_SYSTEM_H
#define SIRESYSTEM_SYSTEM_H

#include <QUuid>

#include "sysname.h"
#include "systemmonitors.h"
#include "constraints.h"

#include "SireVol/space.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"
#include "SireMol/mgnum.h"

#include "SireFF/forcefields.h"

#include "SireBase/majorminorversion.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class System;
}

QDataStream& operator<<(QDataStream&, const SireSystem::System&);
QDataStream& operator>>(QDataStream&, SireSystem::System&);

namespace SireSystem
{

using SireFF::FFPtr;
using SireFF::ForceFields;
using SireFF::FF;
using SireFF::FFID;
using SireFF::FFIdx;
using SireFF::FFName;
using SireFF::ForceTable;

using SireMol::MolGroupsBase;
using SireMol::MoleculeGroup;
using SireMol::MolGroupsPtr;
using SireMol::MoleculeGroups;
using SireMol::MGNum;
using SireMol::MGIdx;
using SireMol::MGName;
using SireMol::MGID;
using SireMol::MolID;
using SireMol::MolNum;
using SireMol::MoleculeData;
using SireMol::MoleculeView;
using SireMol::ViewsOfMol;
using SireMol::Molecules;

using SireVol::Space;
using SireVol::Space;

using SireCAS::Symbol;
using SireCAS::Symbols;
using SireCAS::Expression;
using SireCAS::Values;

using SireBase::Property;
using SireBase::PropertyPtr;
using SireBase::Properties;
using SireBase::PropertyMap;
using SireBase::PropertyName;
using SireBase::MajorMinorVersion;
using SireBase::Version;

/** This is a simulation system. If contains molecules, forcefields that
    provide energy functions of those molecules, and monitors that
    can monitor the changing state of the system
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT System 
            : public SireBase::ConcreteProperty<System,MolGroupsBase>
{

friend QDataStream& ::operator<<(QDataStream&, const System&);
friend QDataStream& ::operator>>(QDataStream&, System&);

public:
    System();
    System(const QString &name);
    
    System(const System &other);

    ~System();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<System>() );
    }
    
    System& operator=(const System &other);
    
    bool operator==(const System &other) const;
    bool operator!=(const System &other) const;

    using SireMol::MolGroupsBase::operator[];

    const FF& operator[](const FFID &ffid) const;
    const SystemMonitor& operator[](const MonitorID &monid) const;
    const MoleculeGroup& operator[](const MGID &mgid) const;
    
    System& operator+=(const FF &forcefield);
    System& operator+=(const MoleculeGroup &molgroup);
    System& operator+=(const Constraint &constraint);
    System& operator+=(const Constraints &constraints);
    
    System& operator-=(const FF &forcefield);
    System& operator-=(const MoleculeGroup &molgroup);
    System& operator-=(const FFID &ffid);
    System& operator-=(const MGID &mgid);
    System& operator-=(const MolID &molid);
    System& operator-=(const Constraint &constraint);
    System& operator-=(const Constraints &constraints);
    
    const QUuid& UID() const;
    const SysName& name() const;
    const Version& version() const;

    void setName(const QString &newname);

    void collectStats();

    void applyConstraints();

    bool constraintsSatisfied();

    using SireMol::MolGroupsBase::at;
    
    const FF& at(const FFID &ffid) const;
    const SystemMonitor& at(const MonitorID &monid) const;
    
    const FF& forceField(const FFID &ffid) const;
    const FF& forceField(const MGID &mgid) const;
    
    const SystemMonitor& monitor(const MonitorID &monid) const;

    QList<SysMonPtr> monitors(const MonitorID &monid) const;

    int nForceFields() const;
    int nMonitors() const;
    int nConstraints() const;
    
    FFIdx ffIdx(const FFID &ffid) const;
    
    const FFName& ffName(const FFID &ffid) const;

    MonitorName monitorName(const MonitorID &monid) const;
    
    QString toString() const;
    
    const Symbol& totalComponent() const;
    
    SireUnits::Dimension::MolarEnergy energy();
    SireUnits::Dimension::MolarEnergy energy(const Symbol &component);
    
    Values energies(const QSet<Symbol> &components);
    Values energies();

    double componentValue(const Symbol &component);
    Values componentValues(const QSet<Symbol> &components);
    Values componentValues();
    
    void force(ForceTable &forcetable, double scale_force=1);
    void force(ForceTable &forcetable, const Symbol &component,
               double scale_force=1);
    
    void setProperty(const QString &name, const Property &value);
    void setProperty(const FFID &ffid, const QString &name, const Property &value);
    
    void setComponent(const Symbol &symbol, double value);
    void setComponent(const Symbol &symbol, const SireCAS::Expression &expression);
    
    Symbols components() const;
    
    SireCAS::Expression getComponent(const Symbol &symbol) const;
    
    bool hasComponent(const Symbol &symbol) const;
    
    const Property& property(const PropertyName &name) const;

    const Property& property(const FFID &ffid, const PropertyName &name) const;

    QStringList propertyKeys() const;
    QStringList propertyKeys(const FFID &ffid) const;

    bool containsProperty(const QString &name) const;
    bool containsProperty(const FFID &ffid, const QString &name) const;
    
    Properties properties() const;
    Properties properties(const FFID &ffid) const;
    
    const SystemMonitors& monitors() const;
    const ForceFields& forceFields() const;
    const MoleculeGroups& extraGroups() const;
    const Constraints& constraints() const;
    
    void clearStatistics();
    void mustNowRecalculateFromScratch();
    
    bool isDirty() const;
    bool isClean() const;
    
    using SireMol::MolGroupsBase::add;
    using SireMol::MolGroupsBase::remove;
    using SireMol::MolGroupsBase::update;
    
    void add(const QString &name, const SystemMonitor &monitor,
             int frequency = 1);
    
    void add(const SystemMonitors &monitors);
    void add(const SystemMonitors &monitors, int frequency);
    
    void setMonitors(const SystemMonitors &monitors);
    void setMonitors(const SystemMonitors &monitors, int frequency);
    
    void add(const FF &forcefield);
    void add(const MoleculeGroup &molgroup);
    
    void add(const Constraint &constraint);
    void add(const Constraints &constraints);

    void setConstraints(const Constraints &constraints);
    
    void remove(const MonitorID &monid);
    
    void remove(const FFID &ffid);
    void remove(const FF &ff);

    void remove(const MGID &mgid);
    void remove(const MoleculeGroup &molgroup);

    void remove(const MolID &molid);

    void remove(const Constraint &constraint);
    void remove(const Constraints &constraints);

    void removeAllMolecules();
    void removeAllMoleculeGroups();
    void removeAllForceFields();
    void removeAllMonitors();
    void removeAllConstraints();

    //overloading MolGroupsBase virtual functions
    const MoleculeGroup& at(MGNum mgnum) const;

    void add(const MoleculeView &molview, const MGID &mgid,
             const PropertyMap &map);
    void add(const ViewsOfMol &molviews, const MGID &mgid,
             const PropertyMap &map);
    void add(const Molecules &molecules, const MGID &mgid,
             const PropertyMap &map);
    void add(const MoleculeGroup &molgroup, const MGID &mgid,
             const PropertyMap &map);
    
    void addIfUnique(const MoleculeView &molview, const MGID &mgid,
                     const PropertyMap &map);
    void addIfUnique(const ViewsOfMol &molviews, const MGID &mgid,
                     const PropertyMap &map);
    void addIfUnique(const Molecules &molecules, const MGID &mgid,
                     const PropertyMap &map);
    void addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid,
                     const PropertyMap &map);
    
    void add(const MoleculeView &molview, const MGID &mgid);
    void add(const ViewsOfMol &molviews, const MGID &mgid);
    void add(const Molecules &molecules, const MGID &mgid);
    void add(const MoleculeGroup &molgroup, const MGID &mgid);
    
    void addIfUnique(const MoleculeView &molview, const MGID &mgid);
    void addIfUnique(const ViewsOfMol &molviews, const MGID &mgid);
    void addIfUnique(const Molecules &molecules, const MGID &mgid);
    void addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid);

    using MolGroupsBase::removeAll;

    void removeAll(const MGID &mgid);
    void remove(const MoleculeView &molview, const MGID &mgid);
    void remove(const ViewsOfMol &molviews, const MGID &mgid);
    void remove(const Molecules &molecules, const MGID &mgid);
    void remove(const MoleculeGroup &molgroup, const MGID &mgid);
    
    void removeAll(const MoleculeView &molview, const MGID &mgid);
    void removeAll(const ViewsOfMol &molviews, const MGID &mgid);
    void removeAll(const Molecules &molecules, const MGID &mgid);
    void removeAll(const MoleculeGroup &molgroup, const MGID &mgid);

    void remove(MolNum molnum, const MGID &mgid);
    void remove(const QSet<MolNum> &molnums, const MGID &mgid);

    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    void update(const MoleculeGroup &molgroup);
    
    void setContents(const MGID &mgid, const MoleculeView &molview,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const ViewsOfMol &molviews,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const Molecules &molecules,
                     const PropertyMap &map);
    void setContents(const MGID &mgid, const MoleculeGroup &molgroup,
                     const PropertyMap &map);
    
    void setContents(const MGID &mgid, const MoleculeView &molview);
    void setContents(const MGID &mgid, const ViewsOfMol &molviews);
    void setContents(const MGID &mgid, const Molecules &molecules);
    void setContents(const MGID &mgid, const MoleculeGroup &molgroup);    

    static const System& null();

protected:
    const MoleculeGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MoleculeGroup*,10> &groups) const;

    QHash<MGNum,const MoleculeGroup*> getGroups() const;

    void reindex();

private:
    void rebuildIndex();

    ForceFields& _pvt_forceFields();

    const ForceFields& _pvt_constForceFields() const;
    const ForceFields& _pvt_forceFields() const;

    MoleculeGroups& _pvt_moleculeGroups();
    
    const MoleculeGroups& _pvt_constMoleculeGroups() const;
    const MoleculeGroups& _pvt_moleculeGroups() const;

    MolGroupsBase& _pvt_moleculeGroups(MGNum mgnum);
    
    const MolGroupsBase& _pvt_moleculeGroups(MGNum mgnum) const;
    const MolGroupsBase& _pvt_constMoleculeGroups(MGNum mgnum) const;
    
    const MoleculeGroup& _pvt_moleculeGroup(MGNum mgnum) const;

    void _pvt_throwMissingGroup(MGNum mgnum) const;

    /** The unique ID for this system */
    QUuid uid;
    
    /** The name of this system */
    SysName sysname;
    
    /** The version number of this system */
    MajorMinorVersion sysversion;

    /** The molecule groups in this system. These are divided
        into two - the first set are actually the forcefields,
        while the second are the non-forcefield groups */
    MolGroupsPtr molgroups[2];

    /** All of the monitors that monitor this system */
    SystemMonitors sysmonitors;

    /** All of the constraints that are applied to this system */
    Constraints cons;
    
    /** The index of which of the two set of MoleculeGroups each
        individual molecule group in this set is in */
    QHash<MGNum,int> mgroups_by_num;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return the unique ID of this system */
inline const QUuid& System::UID() const
{
    return uid;
}

/** Return the name of this system */
inline const SysName& System::name() const
{
    return sysname;
}

/** Return a reference to the version of this system */
inline const Version& System::version() const
{
    return sysversion.version();
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireSystem::System )

SIRE_EXPOSE_CLASS( SireSystem::System )

SIRE_END_HEADER

#endif
