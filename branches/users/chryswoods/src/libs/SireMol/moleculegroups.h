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

#ifndef SIREMOL_MOLECULEGROUPS_H
#define SIREMOL_MOLECULEGROUPS_H

#include "moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeGroups;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroups&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroups&);

namespace SireMol
{

/** This class holds a collection of MoleculeGroups, together
    with information about which molecules are contained in this
    group, and which groups they are contained within.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroups
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroups&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroups&);

public:
    typedef QVector<MoleculeGroup>::const_iterator iterator;
    typedef QVector<MoleculeGroup>::const_iterator const_iterator;

    MoleculeGroups();
    MoleculeGroups(const MoleculeGroup &group);

    MoleculeGroups(const MoleculeGroups &other);

    ~MoleculeGroups();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MoleculeGroups>() );
    }

    const char* what() const
    {
        return MoleculeGroups::typeName();
    }
    
    MoleculeGroups* clone() const
    {
        return new MoleculeGroups(*this);
    }

    MoleculeGroups& operator=(const MoleculeGroup &other);
    MoleculeGroups& operator=(const MoleculeGroups &other);

    bool operator==(const MoleculeGroups &other) const;
    bool operator!=(const MoleculeGroups &other) const;

    const_iterator begin() const;
    const_iterator constBegin() const;
    
    const_iterator end() const;
    const_iterator constEnd() const;

    MoleculeGroups add(const MoleculeGroup &group) const;
    MoleculeGroups add(const MoleculeGroups &groups) const;

    MoleculeGroups add(const MoleculeView &molview,
                       const MGID &mgid) const;
                       
    MoleculeGroups add(const ViewsOfMol &molviews,
                       const MGID &mgid) const;
                       
    MoleculeGroups add(const Molecules &molecules,
                       const MGID &mgid) const;

    MoleculeGroups update(const MoleculeGroup &group) const;
    MoleculeGroups update(const MoleculeGroups &groups) const;
    
    MoleculeGroups update(const MoleculeView &molview) const;
    MoleculeGroups update(const MoleculeData &moldata) const;
    MoleculeGroups update(const Molecules &molecules) const;
    
    MoleculeGroups remove(const MoleculeGroup &group) const;
    MoleculeGroups remove(const MoleculeGroups &groups) const;

    MoleculeGroups remove(const MGID &mgid) const;

    MoleculeGroups remove(const MoleculeView &molview) const;
    MoleculeGroups remove(const MoleculeView &molview, 
                          const MGID &mgid) const;

    MoleculeGroups remove(const ViewsOfMol &molviews) const;
    MoleculeGroups remove(const ViewsOfMol &molviews,
                          const MGID &mgid) const;

    MoleculeGroups remove(const Molecules &molecules) const;
    MoleculeGroups remove(const Molecules &molecules,
                          const MGID &mgid) const;

    MoleculeGroups remove(const MolID &molid) const;
    MoleculeGroups remove(const MolID &molid, const MGID &mgid) const;

    int nGroups() const;
    
    int nMolecules() const;
    int nMolecules(const MGID &mgid) const;
    
    int nViews() const;
    int nViews(const MGID &mgid) const;

    const MoleculeGroup& group(const MGID &mgid) const;
    MoleculeGroups groups(const MGID &mgid) const;

    Molecules molecules() const;
    Molecules molecules(const MGID &mgid) const;

    const ViewsOfMol& molecule(const MolID &molid) const;
    

private:
    /** The list of all groups in this set */
    QVector<MoleculeGroup> groups_by_idx;
    
    /** The index of the groups by number */
    QHash<MGNum,int> groups_by_num;
    
    /** Index of which groups contain which molecule */
    QHash< MolNum, QSet<MGNum> > groups_by_mol;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroups);

SIRE_END_HEADER

#endif
