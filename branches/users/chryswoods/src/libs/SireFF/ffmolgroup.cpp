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

#include "ffmolgroup.h"

#include "SireBase/propertymap.h"

#include "SireMol/mgnum.h"
#include "SireMol/mgname.h"
#include "SireMol/molnum.h"
#include "SireMol/molecule.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireMol;
using namespace SireStream;

/////////
///////// Implementation of FFMolGroupPvt
/////////

static const RegisterMetaType<FFMolGroupPvt> r_ffmolgrouppvt( MAGIC_ONLY,
                                                 FFMolGroupPvt::typeName() );
                                                 
/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const FFMolGroupPvt &ffmolgrouppvt)
{
    writeHeader(ds, r_ffmolgrouppvt, 1);
    
    ds << ffmolgrouppvt.mgidx << static_cast<const MolGroup&>(ffmolgrouppvt);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream& operator>>(QDataStream &ds, FFMolGroupPvt &ffmolgrouppvt)
{
    VersionID v = readHeader(ds, r_ffmolgrouppvt);
    
    if (v == 1)
    {
        ds >> ffmolgrouppvt.mgidx >> static_cast<MolGroup&>(ffmolgrouppvt);
        
        //we have to rely on our parent to set the ff pointer...
        ffmolgrouppvt.ffield = 0;
    }
    else
        throw version_error(v, "1", r_ffmolgrouppvt, CODELOC);
        
    return ds;
}

/** Null constructor */
FFMolGroupPvt::FFMolGroupPvt() : MolGroup(), ffield(0)
{}

/** Construct a named group that is a child of the forcefield 'ff' */
FFMolGroupPvt::FFMolGroupPvt(const QString &name, quint32 i, FF *ff)
              : MolGroup(name), mgidx(i), ffield(ff)
{}

/** Copy constructor (our parent forcefield must update our pointer) */
FFMolGroupPvt::FFMolGroupPvt(const FFMolGroupPvt &other)
              : MolGroup(other), mgidx(other.mgidx), ffield(0)
{}

/** Destructor */
FFMolGroupPvt::~FFMolGroupPvt()
{}

/** Copy assignment operator */
FFMolGroupPvt& FFMolGroupPvt::operator=(const FFMolGroupPvt &other)
{
    MolGroup::operator=(other);
    
    //don't change the parent or index - this is because our parent
    //sets this pointer at construction
    
    return *this;
}

/** Set the parent forcefield of this group (this is the forcefield
    that contains this group) */
void FFMolGroupPvt::setParent(FF *new_parent)
{
    ffield = new_parent;
}

/** Set the index of this group in the parent forcefield */
void FFMolGroupPvt::setIndex(quint32 new_idx)
{
    mgidx = MGIdx(new_idx);
}

/** Assert that this is not null (which would be a program bug!)

    \throw SireError::program_bug
*/
void FFMolGroupPvt::assertNotNull() const
{
    if (ffield == 0)
        throw SireError::program_bug( QObject::tr(
            "There is a problem - the parent forcefield of %1 (%2) "
            "is null!").arg(this->name()).arg(this->number()),
                CODELOC );
}

/////////
///////// Implementation of FFMolGroup
/////////

static const RegisterMetaType<FFMolGroup> r_ffmolgroup;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FFMolGroup &ffmolgroup)
{
    writeHeader(ds, r_ffmolgroup, 1);
    
    SharedDataStream sds(ds);
    
    sds << ffmolgroup.mgidx << ffmolgroup.ffield 
        << static_cast<const MolGroup&>(ffmolgroup);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FFMolGroup &ffmolgroup)
{
    VersionID v = readHeader(ds, r_ffmolgroup);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> ffmolgroup.mgidx >> ffmolgroup.ffield 
            >> static_cast<MolGroup&>(ffmolgroup);
    }
    else
        throw version_error(v, "1", r_ffmolgroup, CODELOC);
        
    return ds;
}

/** Null constructor - this creates a useless FFMolGroup. You can
    only construct a valid FFMolGroup by getting a reference to 
    a valid FFMolGroupPvt from a forcefield */
FFMolGroup::FFMolGroup() : ConcreteProperty<FFMolGroup,MolGroup>()
{}

/** Construct from an FFMolGroupPvt - this grabs a copy of the
    forcefield that contains the FFMolGroupPvt */
FFMolGroup::FFMolGroup(const detail::FFMolGroupPvt &ffmolgroup)
           : ConcreteProperty<FFMolGroup,MolGroup>(ffmolgroup), 
             mgidx(ffmolgroup.index()), ffield( ffmolgroup.forceField() )
{}

/** Construct from a MolGroup - you can only construct from a FFMolGroup
    or from an FFMolGroupPvt 
    
    \throw SireError::invalid_arg
*/
FFMolGroup::FFMolGroup(const MolGroup &other)
           : ConcreteProperty<FFMolGroup,MolGroup>(other)
{
    this->operator=(other);
}

/** Copy constructor */
FFMolGroup::FFMolGroup(const FFMolGroup &other)
           : ConcreteProperty<FFMolGroup,MolGroup>(other), 
             mgidx(other.mgidx), ffield(other.ffield)
{}

/** Destructor */
FFMolGroup::~FFMolGroup()
{}

/** Copy assignment operator */
FFMolGroup& FFMolGroup::operator=(const FFMolGroup &other)
{
    MolGroup::operator=(other);
    ffield = other.ffield;
    mgidx = other.mgidx;
    
    return *this;
}

/** Copy assignment from another MolGroup - you can only assign from
    another FFMolGroup, or from an SireFF::detail::FFMolGroupPvt 
    
    \throw SireError::invalid_arg
*/
FFMolGroup& FFMolGroup::operator=(const MolGroup &other)
{
    if (other.isA<FFMolGroupPvt>())
    {
        const FFMolGroupPvt &ff_other = other.asA<FFMolGroupPvt>();
        MolGroup::operator=(other);
        ffield = ff_other.forceField();
        mgidx = ff_other.index();
    }
    else if (other.isA<FFMolGroup>())
    {
        return this->operator=(other.asA<FFMolGroup>());
    }
    else
        throw SireError::invalid_arg( QObject::tr(
            "You can only assign an FFMolGroup from another FFMolGroup "
            "or from an FFMolGroupPvt. You cannot assign from a %1.")
                .arg(other.what()), CODELOC );

    return *this;
}

/** Assert that this is not a null FFMolGroup

    \throw SireError::nullptr_error
*/
void FFMolGroup::assertNotNull() const
{
    if (ffield.constData() == 0)
        throw SireError::nullptr_error( QObject::tr(
            "You cannot use a null FFMolGroup. Please construct a valid "
            "FFMolGroup by copying one from a valid forcefield."), CODELOC );
}

/** Return the forcefield that contains this molecule group */
const FF& FFMolGroup::forceField() const
{
    assertNotNull();
    return *ffield;
}

/** Return the index of this group in the parent forcefield */
MGIdx FFMolGroup::index() const
{
    return mgidx;
}

/** Update this group so that it is current */
void FFMolGroup::updateGroup()
{
    MolGroup::operator=( ffield->group(this->number()) );
}

/** Set the name of this molecule group */
void FFMolGroup::setName(const QString &name)
{
    assertNotNull();
    ffield.edit().group_setName(mgidx, name);
    updateGroup();
}

void FFMolGroup::add(const MoleculeView &molview)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molview, PropertyMap());
    updateGroup();
}

void FFMolGroup::add(const ViewsOfMol &molviews)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molviews, PropertyMap());
    updateGroup();
}

void FFMolGroup::add(const Molecules &molecules)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molecules, PropertyMap());
    updateGroup();
}

void FFMolGroup::add(const MolGroup &molgroup)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molgroup, PropertyMap());
    updateGroup();
}

void FFMolGroup::add(const MoleculeView &molview, const PropertyMap &map)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molview, map);
    updateGroup();
}

void FFMolGroup::add(const ViewsOfMol &molviews, const PropertyMap &map)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molviews, map);
    updateGroup();
}

void FFMolGroup::add(const Molecules &molecules, const PropertyMap &map)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molecules, map);
    updateGroup();
}

void FFMolGroup::add(const MolGroup &molgroup, const PropertyMap &map)
{
    assertNotNull();
    ffield.edit().group_add(mgidx, molgroup, map);
    updateGroup();
}

bool FFMolGroup::addIfUnique(const MoleculeView &molview)
{
    assertNotNull();
    bool ret = ffield.edit().group_addIfUnique(mgidx, molview, PropertyMap());
    updateGroup();
    
    return ret;
}

ViewsOfMol FFMolGroup::addIfUnique(const ViewsOfMol &molviews)
{
    assertNotNull();
    ViewsOfMol ret = ffield.edit().group_addIfUnique(mgidx, molviews, PropertyMap());
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::addIfUnique(const Molecules &molecules)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_addIfUnique(mgidx, molecules, 
                                                            PropertyMap());
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::addIfUnique(const MolGroup &molgroup)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_addIfUnique(mgidx, molgroup, 
                                                            PropertyMap());
    updateGroup();
    
    return ret;
}

bool FFMolGroup::addIfUnique(const MoleculeView &molview, 
                             const PropertyMap &map)
{
    assertNotNull();
    bool ret = ffield.edit().group_addIfUnique(mgidx, molview, map);
    updateGroup();
    
    return ret;
}

ViewsOfMol FFMolGroup::addIfUnique(const ViewsOfMol &molviews, 
                                   const PropertyMap &map)
{
    assertNotNull();
    ViewsOfMol ret = ffield.edit().group_addIfUnique(mgidx, molviews, map);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::addIfUnique(const Molecules &molecules, 
                                          const PropertyMap &map)
{

    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_addIfUnique(mgidx, molecules, map);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::addIfUnique(const MolGroup &molgroup, 
                                          const PropertyMap &map)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_addIfUnique(mgidx, molgroup, map);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::remove(const MoleculeView &molview)
{
    assertNotNull();
    bool ret = ffield.edit().group_remove(mgidx, molview);
    updateGroup();
    
    return ret;
}

ViewsOfMol FFMolGroup::remove(const ViewsOfMol &molviews)
{
    assertNotNull();
    ViewsOfMol ret = ffield.edit().group_remove(mgidx, molviews);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::remove(const Molecules &molecules)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_remove(mgidx, molecules);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::remove(const MolGroup &molgroup)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_remove(mgidx, molgroup);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::removeAll(const MoleculeView &molview)
{
    assertNotNull();
    bool ret = ffield.edit().group_removeAll(mgidx, molview);
    updateGroup();
    
    return ret;
}

ViewsOfMol FFMolGroup::removeAll(const ViewsOfMol &molviews)
{
    assertNotNull();
    ViewsOfMol ret = ffield.edit().group_removeAll(mgidx, molviews);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::removeAll(const Molecules &molecules)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_removeAll(mgidx, molecules);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::removeAll(const MolGroup &molgroup)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_removeAll(mgidx, molgroup);
    updateGroup();
    
    return ret;
}

ViewsOfMol FFMolGroup::remove(MolNum molnum)
{
    assertNotNull();
    ViewsOfMol ret = ffield.edit().group_remove(mgidx, molnum);
    updateGroup();
    
    return ret;
}

QList<ViewsOfMol> FFMolGroup::remove(const QSet<MolNum> &molnums)
{
    assertNotNull();
    QList<ViewsOfMol> ret = ffield.edit().group_remove(mgidx, molnums);
    updateGroup();
    
    return ret;
}

void FFMolGroup::removeAll()
{
    assertNotNull();
    ffield.edit().group_removeAll(mgidx);
    updateGroup();
}

bool FFMolGroup::update(const MoleculeData &moldata)
{
    assertNotNull();
    bool ret = ffield.edit().group_update(mgidx, moldata);
    updateGroup();
    
    return ret;
}

QList<Molecule> FFMolGroup::update(const Molecules &molecules)
{
    assertNotNull();
    QList<Molecule> ret = ffield.edit().group_update(mgidx, molecules);
    updateGroup();
    
    return ret;
}

QList<Molecule> FFMolGroup::update(const MolGroup &molgroup)
{
    assertNotNull();
    QList<Molecule> ret = ffield.edit().group_update(mgidx, molgroup);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const MoleculeView &molview)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molview, PropertyMap());
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const ViewsOfMol &molviews)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molviews, PropertyMap());
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const Molecules &molecules)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molecules, PropertyMap());
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const MolGroup &molgroup)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molgroup, PropertyMap());
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const MoleculeView &molview, const PropertyMap &map)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molview, map);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const ViewsOfMol &molviews, const PropertyMap &map)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molviews, map);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const Molecules &molecules, const PropertyMap &map)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molecules, map);
    updateGroup();
    
    return ret;
}

bool FFMolGroup::setContents(const MolGroup &molgroup, const PropertyMap &map)
{
    assertNotNull();
    bool ret = ffield.edit().group_setContents(mgidx, molgroup, map);
    updateGroup();
    
    return ret;
}
