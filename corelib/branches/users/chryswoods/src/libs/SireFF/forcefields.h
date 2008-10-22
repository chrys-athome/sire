/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREFF_FORCEFIELDS_H
#define SIREFF_FORCEFIELDS_H

#include "forcefield.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceFields;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceFields&);
QDataStream& operator>>(QDataStream&, SireFF::ForceFields&);

namespace SireFF
{

class ForceTable;

using SireMol::MoleculeGroup;
using SireMol::MGNum;
using SireMol::MGID;
using SireMol::Molecules;
using SireMol::ViewsOfMol;
using SireMol::MoleculeView;

using SireBase::PropertyPtr;

using SireCAS::Symbols;

namespace detail
{
class FFSymbol;
typedef boost::shared_ptr<FFSymbol> FFSymbolPtr;
}

/** A ForceFields object contains a collection of forcefields,
    together with energy functions that allow energies/forces
    of a combination of forcefield components to be evaluated.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFields 
        : public SireBase::ConcreteProperty<ForceFields,SireMol::MolGroupsBase>
{

friend QDataStream& ::operator<<(QDataStream&, const ForceFields&);
friend QDataStream& ::operator>>(QDataStream&, ForceFields&);

public:
    ForceFields();
    ForceFields(const FF& forcefield);
    ForceFields(const QList<FFPtr> &forcefields);
    ForceFields(const QVector<FFPtr> &forcefields);
    
    ForceFields(const ForceFields &other);
    
    ~ForceFields();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ForceFields>() );
    }

    ForceFields& operator=(const ForceFields &other);
    
    bool operator==(const ForceFields &other) const;
    bool operator!=(const ForceFields &other) const;
    
    using SireMol::MolGroupsBase::operator[];
    using SireMol::MolGroupsBase::at;
    
    const FF& operator[](const FFName &ffname) const;
    const FF& operator[](const FFIdx &ffidx) const;
    const FF& operator[](const FFID &ffid) const;
    
    const FF& at(const FFName &ffname) const;
    const FF& at(const FFIdx &ffidx) const;
    const FF& at(const FFID &ffid) const;
    
    const FF& forceField(const FFName &ffname) const;
    const FF& forceField(const FFIdx &ffidx) const;
    const FF& forceField(const FFID &ffid) const;
    const FF& forceField(const MGNum &mgnum) const;
    
    int nForceFields() const;
    
    FFIdx ffIdx(const FFName &ffname) const;
    FFIdx ffIdx(const FFIdx &ffidx) const;
    FFIdx ffIdx(const FFID &ffid) const;
    
    const FFName& ffName(const FFName &ffname) const;
    const FFName& ffName(const FFIdx &ffidx) const;
    const FFName& ffName(const FFID &ffid) const;
    
    QString toString() const;
    
    static const Symbol& totalComponent();
    
    SireUnits::Dimension::Energy energy();
    SireUnits::Dimension::Energy energy(const Symbol &component);
    
    Symbols components() const;
    
    bool hasComponent(const Symbol &symbol) const;
    
    Values energies(const QSet<Symbol> &components);
    Values energies();
    
    void force(ForceTable &forcetable, double scale_force=1);
    void force(ForceTable &forcetable, const Symbol &component,
               double scale_force=1);
    
    void setProperty(const QString &name, const Property &value);
    void setProperty(const FFID &ffid, const QString &name, const Property &value);
    
    void setComponent(const Symbol &symbol, double value);
    void setComponent(const Symbol &symbol, const SireCAS::Expression &expression);
    
    SireCAS::Expression getComponent(const Symbol &symbol) const;
    
    const Property& property(const QString &name) const;

    const Property& property(const FFID &ffid, const QString &name) const;
 
    QStringList propertyKeys() const;
    QStringList propertyKeys(const FFID &ffid) const;

    bool containsProperty(const QString &name) const;
    bool containsProperty(const FFID &ffid, const QString &name) const;
    
    Properties properties() const;
    Properties properties(const FFID &ffid) const;
    
    QVector<FFPtr> forceFieldsWithProperty(const QString &name) const;
    QVector<FFPtr> forceFieldsWithProperty(const FFID &ffid, const QString &name) const;
    
    const QVector<FFPtr>& forceFields() const;
    QList<FFName> ffNames() const;
    
    QVector<FFPtr> forceFields(const FFID &ffid) const;
    QList<FFName> ffNames(const FFID &ffid) const;
    
    const QVector<FFPtr>& list() const;
    QList<FFName> names() const;
    
    void mustNowRecalculateFromScratch();
    
    bool isDirty() const;
    bool isClean() const;
    
    using SireMol::MolGroupsBase::add;
    using SireMol::MolGroupsBase::remove;
    using SireMol::MolGroupsBase::update;
    
    void add(const FF &forcefield);
    
    void remove(const FFIdx &ffidx);
    void remove(const FFName &ffname);
    void remove(const FFID &ffid);

    void removeAllForceFields();

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

protected:
    const MoleculeGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MoleculeGroup*,10> &groups) const;

    QHash<MGNum,const MoleculeGroup*> getGroups() const;

private:
    void rebuildIndex();

    const FF& _pvt_forceField(int idx) const;
    FF& _pvt_forceField(int idx);

    const FF& _pvt_forceField(const FFName &ffname) const;
    FF& _pvt_forceField(const FFName &ffname);

    const FF& _pvt_forceField(const MGNum &mgnum) const;
    FF& _pvt_forceField(const MGNum &mgnum);

    void _pvt_remove(int i);

    /** The global symbol used to refer to the total energy of a collection
        of forcefields */
    static Symbol total_component;

    /** All of the forcefields arranged by FFIdx */
    QVector<FFPtr> ffields_by_idx;
    
    /** Map from forcefield name to its index */
    QHash<FFName,int> ffields_by_name;
    
    /** Map from molecule group number to the name of the 
        forcefield that contains that group */
    QHash<MGNum, FFName> mgroups_by_num;
    
    /** All of the energy components, expressions and constants
        available in this collection of forcefields */
    QHash<Symbol, detail::FFSymbolPtr> ffsymbols;
};

}

Q_DECLARE_METATYPE( SireFF::ForceFields )

SIRE_EXPOSE_CLASS( SireFF::ForceFields )

SIRE_END_HEADER

#endif
