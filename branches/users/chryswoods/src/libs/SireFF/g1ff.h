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

#ifndef SIREFF_G1FF_H
#define SIREFF_G1FF_H

#include "ff.h"
#include "ffmolgroup.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class G1FF;
}

QDataStream& operator<<(QDataStream&, const SireFF::G1FF&);
QDataStream& operator>>(QDataStream&, SireFF::G1FF&);

namespace SireFF
{

/** This is the base class of all forcefields that hold just
    a single group of molecules
    
    @author Christopher Woods
*/
class SIREFF_EXPORT G1FF : public FF
{

friend QDataStream& ::operator<<(QDataStream&, const G1FF&);
friend QDataStream& ::operator>>(QDataStream&, G1FF&);

public:
    ~G1FF();

protected:
    G1FF();
    G1FF(const G1FF &other);
    
    G1FF& operator=(const G1FF &other);

    ////
    //// Virtual functions that must be overridden by
    //// derived virtual forcefields
    ////

    const MolGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MolGroup*,10> &groups) const;

    QHash<MGNum,const MolGroup*> getGroups() const;
    
    void group_setName(quint32 i, const QString &new_name);
        
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

private:
    /** The single group of molecules in this forcefield */
    detail::FFMolGroupPvt molgroup;
};

}

SIRE_END_HEADER

#endif
