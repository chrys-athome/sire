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

#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "ff.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class NullFF;
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::NullFF&);
QDataStream& operator>>(QDataStream&, SireFF::NullFF&);

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

using SireBase::PropertyBase;

/** This is a completely null forcefield. It has zero energy!

    @author Christopher Woods
*/
class SIREFF_EXPORT NullFF : public SireBase::ConcreteProperty<NullFF,FF>
{

friend QDataStream& ::operator<<(QDataStream&, const NullFF&);
friend QDataStream& ::operator>>(QDataStream&, NullFF&);

public:
    NullFF();
    NullFF(const NullFF &other);
    
    ~NullFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullFF>() );
    }

    const FFComponent& components() const;
    
    NullFF& operator=(const NullFF &other);
    
    bool operator==(const NullFF &other) const;
    bool operator!=(const NullFF &other) const;
    
    const MolGroup& at(MGNum mgnum) const;
    
    QString toString() const;

    bool setProperty(const QString &name, const Property &value);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;
    void mustNowRecalculateFromScratch();

protected:
    void recalculateEnergy();
    void group_add(quint32 i, const MoleculeView &molview,
                   const PropertyMap &map);
    void group_add(quint32 i, const ViewsOfMol &molviews, 
                   const PropertyMap &map);
    void group_add(quint32 i, const Molecules &molecules, 
                   const PropertyMap &map);
    void group_add(quint32 i, const MolGroup &molgroup, 
                   const PropertyMap &map);
    bool group_addIfUnique(quint32 i, const MoleculeView &molview, 
                           const PropertyMap &map);
    ViewsOfMol group_addIfUnique(quint32 i, const ViewsOfMol &molviews, 
                                 const PropertyMap &map);
    QList<ViewsOfMol> group_addIfUnique(quint32 i, const Molecules &molecules, 
                                        const PropertyMap &map);
    QList<ViewsOfMol> group_addIfUnique(quint32 i, const MolGroup &molgroup, 
                                        const PropertyMap &map);

    bool group_remove(quint32 i, const MoleculeView &molview);
    ViewsOfMol group_remove(quint32 i, const ViewsOfMol &molviews);
    QList<ViewsOfMol> group_remove(quint32 i, const Molecules &molecules);
    QList<ViewsOfMol> group_remove(quint32 i, const MolGroup &molgroup);
    
    bool group_removeAll(quint32 i, const MoleculeView &molview);
    ViewsOfMol group_removeAll(quint32 i, const ViewsOfMol &molviews);
    QList<ViewsOfMol> group_removeAll(quint32 i, const Molecules &molecules);
    QList<ViewsOfMol> group_removeAll(quint32 i, const MolGroup &molgroup);

    ViewsOfMol group_remove(quint32 i, MolNum molnum);
    QList<ViewsOfMol> group_remove(quint32 i, const QSet<MolNum> &molnums);

    void group_removeAll(quint32 i);

    bool group_update(quint32 i, const MoleculeData &moldata);

    QList<Molecule> group_update(quint32 i, const Molecules &molecules);
    QList<Molecule> group_update(quint32 i, const MolGroup &molgroup);
    
    bool group_setContents(quint32 i, const MoleculeView &molview, 
                           const PropertyMap &map);
    bool group_setContents(quint32 i, const ViewsOfMol &molviews, 
                           const PropertyMap &map);
    bool group_setContents(quint32 i, const Molecules &molecules, 
                           const PropertyMap &map);
    bool group_setContents(quint32 i, const MolGroup &molgroup, 
                           const PropertyMap &map);

    void _pvt_updateName();
    void _pvt_restore(const ForceField &ffield);

    const MolGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MolGroup*,10> &groups) const;

    QHash<MGNum,const MolGroup*> getGroups() const;
    
    void group_setName(quint32 i, const QString &new_name);

private:
    NullFF(bool);
    
    static NullFF& getSharedNullFF();

    Properties props;
};

/** This is the polymorphic pointer holder for the 
    ForceField (FF derived) classes.
    
    ForceField are ForceField that contain functions
    that allow the intermolecular potential energy of the
    group(s) of molecules to be evaluated. As such, they 
    are central to the concept of a molecular simulation
    program
    
    @author Christopher Woods
*/
class SIREFF_EXPORT ForceField : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const PropertyBase &property);
    ForceField(const FF &molgroup);

    ForceField(const ForceField &other);
    
    ~ForceField();
    
    virtual ForceField& operator=(const PropertyBase &property);
    virtual ForceField& operator=(const FF &other);

    const FF* operator->() const;
    const FF& operator*() const;
    
    const FF& read() const;
    FF& edit();
    
    const FF* data() const;
    const FF* constData() const;
    
    FF* data();
    
    operator const FF&() const;

    static const ForceField& shared_null();
};

}

Q_DECLARE_METATYPE(SireFF::NullFF);
Q_DECLARE_METATYPE(SireFF::ForceField);

SIRE_EXPOSE_CLASS( SireFF::NullFF )

SIRE_EXPOSE_PROPERTY( SireFF::ForceField, SireFF::FF )

SIRE_END_HEADER

#endif
