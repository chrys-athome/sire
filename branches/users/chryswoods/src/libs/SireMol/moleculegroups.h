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

#include <QVector>
#include <QHash>

#include "moleculegroup.h"
#include "molnum.h"
#include "mgnum.h"

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

    virtual ~MoleculeGroups();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MoleculeGroups>() );
    }

    virtual const char* what() const
    {
        return MoleculeGroups::typeName();
    }
    
    virtual MoleculeGroups* clone() const
    {
        return new MoleculeGroups(*this);
    }

    virtual MoleculeGroups& operator=(const MoleculeGroup &other);
    virtual MoleculeGroups& operator=(const MoleculeGroups &other);

    bool operator==(const MoleculeGroups &other) const;
    bool operator!=(const MoleculeGroups &other) const;

    const MoleculeGroup& operator[](MGIdx mgidx) const;
    const MoleculeGroup& operator[](MGNum mgnum) const;
    const MoleculeGroup& operator[](const MGID &mgid) const;
    
    const ViewsOfMol& operator[](const MolID &molid) const;
    
    const MoleculeGroup& at(MGIdx mgidx) const;
    const MoleculeGroup& at(MGNum mgnum) const;
    const MoleculeGroup& at(const MGID &mgid) const;
    
    const ViewsOfMol& at(const MolID &molid) const;

    const_iterator begin() const;
    const_iterator constBegin() const;
    
    const MoleculeGroup& first() const;
    const MoleculeGroup& find(const MGID &mgid) const;
    const MoleculeGroup& last() const;
    
    const_iterator end() const;
    const_iterator constEnd() const;

    void add(const MoleculeGroup &group);
    void add(const MoleculeGroups &groups);

    void add(const MoleculeView &molview, const MGID &mgid);
                       
    void add(const ViewsOfMol &molviews, const MGID &mgid);
                       
    void add(const Molecules &molecules, const MGID &mgid);

    void update(const MoleculeGroup &group);
    void update(const MoleculeGroups &groups);
    
    void update(const MoleculeView &molview);
    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    
    void remove(const MoleculeGroup &group);
    void remove(const MoleculeGroups &groups);

    void remove(const MGID &mgid);

    void remove(const MoleculeView &molview);
    void remove(const MoleculeView &molview, const MGID &mgid);

    void remove(const ViewsOfMol &molviews);
    void remove(const ViewsOfMol &molviews, const MGID &mgid);

    void remove(const Molecules &molecules);
    void remove(const Molecules &molecules, const MGID &mgid);

    void remove(const MolID &molid);
    void remove(const MolID &molid, const MGID &mgid);

    int nGroups() const;
    
    int nMolecules() const;
    int nMolecules(const MGID &mgid) const;
    
    int nViews() const;
    int nViews(const MGID &mgid) const;

    const MoleculeGroup& group(MGNum mgnum) const;
    const MoleculeGroup& group(const MGID &mgid) const;
    MoleculeGroups groups(const MGID &mgid) const;

    Molecules molecules() const;
    Molecules molecules(const MGID &mgid) const;

    ViewsOfMol molecule(const MolID &molid) const;

private:
    // Editing functions - these are virtual so that they can be
    // overridden by the System class (which will inherit from 
    // MoleculeGroups). In System, the ._pvt_add(...) functions (etc.)
    // will do what is necessary in the forcefields, and will then
    // copy the resulting MoleculeGroups from the forcefields back
    // into this set. It will be able to do that as it will
    // then be responsible for maintaining the constraint that
    // all molecules must be at the same version

    virtual void _pvt_addToIndex(int idx);
    
    virtual void _pvt_removeFromIndex(MGNum mgnum);
    virtual void _pvt_removeFromIndex(MGNum mgnum, MolNum molnum);
    
    virtual void _pvt_update(const Molecules &mols, QSet<MGNum> &already_updated);
    virtual void _pvt_update(const MoleculeGroup &molgroup);
    virtual void _pvt_update(const MoleculeGroups &molgroups);
    virtual void _pvt_update(const MoleculeData &moldata);
    
    virtual void _pvt_add(const MoleculeGroup &molgroup);
    virtual void _pvt_add(const MoleculeView &molview, MGNum mgnum);
    virtual void _pvt_add(const ViewsOfMol &molviews, MGNum mgnum);
    virtual void _pvt_add(const Molecules &molecules, MGNum mgnum);
    
    virtual void _pvt_remove(MGNum mgnum);
    virtual void _pvt_remove(MolNum molnum, const QSet<MGNum> &mgnums);
    virtual void _pvt_remove(const MoleculeView &molview, const QSet<MGNum> &mgnums);
    virtual void _pvt_remove(const ViewsOfMol &molviews, const QSet<MGNum> &mgnums);
    
    virtual void _pvt_reindex();

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
