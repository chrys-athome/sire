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
    MoleculeGroups();
    MoleculeGroups(const MoleculeGroup &group);

    MoleculeGroups(const MoleculeGroups &other);

    ~MoleculeGroups();

    MoleculeGroups& operator=(const MoleculeGroups &other);

    bool operator==(const MoleculeGroups &other) const;
    bool operator!=(const MoleculeGroups &other) const;

    void clear();

    bool add(const MoleculeGroup &group);
    bool change(const MoleculeGroup &group);
    bool remove(const MoleculeGroup &group);

    bool add(const MoleculeGroups &groups);
    bool change(const MoleculeGroups &groups);
    bool remove(const MoleculeGroups &groups);

    bool remove(MoleculeGroupID groupid);
    bool remove(const QString &groupname);

    bool add(const PartialMolecule &molecule, MoleculeGroupID groupid);
    bool add(const QList<PartialMolecule> &molecules, MoleculeGroupID groupid);

    bool remove(const PartialMolecule &molecule, MoleculeGroupID groupid);
    bool remove(const QList<PartialMolecule> &molecules, MoleculeGroupID groupid);

    bool change(const PartialMolecule &molecule);
    bool change(const QList<PartialMolecule> &molecules);
    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool remove(const PartialMolecule &molecule);
    bool remove(const QList<PartialMolecule> &molecules);

    PartialMolecule molecule(MoleculeID molid) const;
    PartialMolecule molecule(MoleculeID molid, MoleculeGroupID groupid) const;
    PartialMolecule molecule(MoleculeID molid, 
                             const QSet<MoleculeGroupID> &groupids) const;

    QHash<MoleculeID,PartialMolecule> molecules() const;

    QSet<MoleculeID> moleculeIDs() const;

    QHash<MoleculeGroupID,MoleculeGroup> groups() const;
    QHash<MoleculeGroupID,MoleculeGroup> 
                      groups(const QSet<MoleculeGroupID> &groupids) const;
    
    QHash<MoleculeGroupID,MoleculeGroup> groups(MoleculeID molid) const;
    QHash<MoleculeGroupID,MoleculeGroup> groups(const PartialMolecule &molecule) const;

    const MoleculeGroup& group(MoleculeGroupID groupid) const;
    const MoleculeGroup& group(const QString &name) const;

    QSet<MoleculeGroupID> groupsContaining(const PartialMolecule &molecule) const;
    bool contains(const PartialMolecule &molecule) const;

    QSet<MoleculeGroupID> groupsReferringTo(MoleculeID molid) const;
    bool refersTo(MoleculeID molid) const;

    int count() const;

    int nMolecules() const;

    void assertContains(MoleculeGroupID groupid) const;

private:
    void reindex();
    void _pvt_index(const MoleculeGroup &group);

    /** The collection of groups in this object, indexed
        by their MoleculeGroupID */
    QHash<MoleculeGroupID, MoleculeGroup> molgroups;

    /** Index of where each molecule resides */
    QHash< MoleculeID, QSet<MoleculeGroupID> > index;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroups);

SIRE_END_HEADER

#endif
