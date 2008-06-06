/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_MOLGROUPS_H
#define SIREMOL_MOLGROUPS_H

#include <QVarLengthArray>
#include <QHash>
#include <QList>

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolGroupsBase;
class MolGroups;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolGroupsBase&);
QDataStream& operator>>(QDataStream&, SireMol::MolGroupsBase&);

QDataStream& operator<<(QDataStream&, const SireMol::MolGroups&);
QDataStream& operator>>(QDataStream&, SireMol::MolGroups&);

namespace SireMol
{

using SireBase::PropertyBase;
using SireBase::ConcreteProperty;

template<class T>
class Selector;

class MolGroup;
class MoleculeGroup;

class Molecules;
class ViewsOfMol;
class MoleculeView;
class MoleculeData;
class Segment;
class Chain;
class Residue;
class CutGroup;
class Atom;

class MGNum;
class MGName;
class MGIdx;
class MGID;

class MolNum;
class MolName;
class MolIdx;
class MolID;

class SegID;
class ChainID;
class ResID;
class CGID;
class AtomID;

/** This is the base class of all MoleculeGroups objects.
    These are containers for MoleculeGroup objects, thereby
    allowing lots of MoleculeGroup objects to be collected
    together and indexed (e.g. so that you can find all
    CA atoms in the "proteins" group). This is the virtual
    base class of the hierarchy - see MolGroups for a simple
    concrete instantiation.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolGroupsBase : public PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolGroupsBase&);
friend QDataStream& ::operator>>(QDataStream&, MolGroupsBase&);

public:    
    virtual ~MolGroupsBase();
    
    const MolGroup& operator[](MGNum mgnum) const;
    const MolGroup& operator[](const MGName &mgname) const;
    const MolGroup& operator[](MGIdx mgidx) const;
    const MolGroup& operator[](const MGID &mgid) const;
    
    ViewsOfMol operator[](MolNum molnum) const;
    ViewsOfMol operator[](const MolID &molid) const;
    
    Segment operator[](const SegID &segid) const;
    Chain operator[](const ChainID &chainid) const;
    Residue operator[](const ResID &resid) const;
    CutGroup operator[](const CGID &cgid) const;
    Atom operator[](const AtomID &atomid) const;
    
    virtual MolGroupsBase* clone() const=0;
    
    static const char* typeName()
    {
        return "SireMol::MolGroupsBase";
    }
    
    MGNum getGroupNumber(MGNum mgnum) const;
    MGNum getGroupNumber(MGIdx mgidx) const;
    MGNum getGroupNumber(const MGName &mgname) const;
    MGNum getGroupNumber(const MGID &mgid) const;
    
    MGIdx mgIdx(MGNum mgnum) const;
    
    QList<MGNum> map(MGNum mgnum) const;
    QList<MGNum> map(MGIdx mgidx) const;
    QList<MGNum> map(const MGName &mgname) const;
    QList<MGNum> map(const MGID &mgid) const;
    
    MolNum getMoleculeNumber(MolNum molnum) const;
    MolNum getMoleculeNumber(MolIdx molidx) const;
    MolNum getMoleculeNumber(const MolName &molname) const;
    MolNum getMoleculeNumber(const MolID &molid) const;
    
    QList<MolNum> molNums() const;
    QList<MolNum> getMoleculeNumbers() const;
    
    QList<MolNum> map(MolNum molnum) const;
    QList<MolNum> map(MolIdx molidx) const;
    QList<MolNum> map(const MolName &molname) const;
    QList<MolNum> map(const MolID &molid) const;
    
    virtual const MolGroup& at(MGNum mgnum) const=0;
    
    const MolGroup& at(MGIdx mgidx) const;
    const MolGroup& at(const MGName &mgname) const;
    const MolGroup& at(const MGID &mgid) const;
    
    ViewsOfMol at(MolNum molnum) const;
    ViewsOfMol at(const MolID &molid) const;

    Segment at(const SegID &segid) const;
    Chain at(const ChainID &chainid) const;
    Residue at(const ResID &resid) const;
    CutGroup at(const CGID &cgid) const;
    Atom at(const AtomID &atomid) const;

    MoleculeGroup select(const MGID &mgid) const;
    ViewsOfMol select(const MolID &molid) const;
    Segment select(const SegID &segid) const;
    Chain select(const ChainID &chainid) const;
    Residue select(const ResID &resid) const;
    CutGroup select(const CGID &cgid) const;
    Atom select(const AtomID &atomid) const;

    QList<MoleculeGroup> selectAll() const;

    QList<MoleculeGroup> selectAll(MGNum mgnum) const;
    QList<MoleculeGroup> selectAll(MGIdx mgidx) const;
    QList<MoleculeGroup> selectAll(const MGName &mgname) const;
    QList<MoleculeGroup> selectAll(const MGID &mgid) const;
    
    QList<ViewsOfMol> selectAll(const MolID &molid) const;
    
    QHash< MolNum,Selector<Segment> > selectAll(const SegID &segid) const;
    QHash< MolNum,Selector<Chain> > selectAll(const ChainID &chainid) const;
    QHash< MolNum,Selector<Residue> > selectAll(const ResID &resid) const;
    QHash< MolNum,Selector<CutGroup> > selectAll(const CGID &cgid) const;
    QHash< MolNum,Selector<Atom> > selectAll(const AtomID &atomid) const;
    
    const MolGroup& group(MGNum mgnum) const;
    const MolGroup& group(const MGName &mgname) const;
    const MolGroup& group(MGIdx mgidx) const;
    const MolGroup& group(const MGID &mgid) const;
    
    QList<MGNum> groupNums() const;
    
    QList<MoleculeGroup> groups() const;
    
    QList<MoleculeGroup> groups(MGNum mgnum) const;
    QList<MoleculeGroup> groups(MGIdx mgidx) const;
    QList<MoleculeGroup> groups(const MGName &mgname) const;
    QList<MoleculeGroup> groups(const MGID &mgid) const;
    
    ViewsOfMol molecule(MolNum molnum) const;
    ViewsOfMol molecule(const MolID &molid) const;
    
    QList<ViewsOfMol> molecules(MolNum molnum) const;
    QList<ViewsOfMol> molecules(const MolID &molid) const;
    
    Segment segment(const SegID &segid) const;
    Chain chain(const ChainID &chainid) const;
    Residue residue(const ResID &resid) const;
    CutGroup cutGroup(const CGID &cgid) const;
    Atom atom(const AtomID &atomid) const;
    
    QHash< MolNum,Selector<Segment> > segments(const SegID &segid) const;
    QHash< MolNum,Selector<Chain> > chains(const ChainID &chainid) const;
    QHash< MolNum,Selector<Residue> > residues(const ResID &resid) const;
    QHash< MolNum,Selector<CutGroup> > cutGroups(const CGID &cgid) const;
    QHash< MolNum,Selector<Atom> > atoms(const AtomID &atomid) const;

    bool contains(MGNum mgnum) const;
    bool contains(MolNum molnum) const;
    bool contains(const MoleculeView &molview) const;
    bool contains(const ViewsOfMol &molviews) const;
    bool contains(const Molecules &molecules) const;
    
    bool intersects(const MoleculeView &molview) const;
    bool intersects(const Molecules &other) const;

    const QList<MGNum>& groupsContaining(MolNum molnum) const;
    
    int nGroups() const;
    int count() const;
    
    int nMolecules() const;
    
    int nViews() const;
    int nViews(MolNum molnum) const;

    bool isEmpty() const;

    Molecules molecules() const;
    
    QList<MGNum> mgNums() const;
    
    void assertContains(MolNum molnum) const;
    void assertContains(const MolID &molid) const;
    
    void assertContains(MGNum mgnum) const;
    void assertContains(const MGID &mgid) const;
    
    virtual void add(const MoleculeView &molview, const MGID &mgid)=0;
    virtual void add(const ViewsOfMol &molviews, const MGID &mgid)=0;
    virtual void add(const Molecules &molecules, const MGID &mgid)=0;
    virtual void add(const MolGroup &molgroup, const MGID &mgid)=0;
    
    virtual void addIfUnique(const MoleculeView &molview,
                             const MGID &mgid)=0;
    virtual void addIfUnique(const ViewsOfMol &molviews,
                             const MGID &mgid)=0;
    virtual void addIfUnique(const Molecules &molecules,
                             const MGID &mgid)=0;
    virtual void addIfUnique(const MolGroup &molgroup,
                             const MGID &mgid)=0;
    
    void unite(const MoleculeView &molview, const MGID &mgid);
    void unite(const ViewsOfMol &molviews, const MGID &mgid);
    void unite(const Molecules &molecules, const MGID &mgid);
    void unite(const MolGroup &molgroup, const MGID &mgid);
    
    virtual void remove(const MoleculeView &molview);
    virtual void remove(const ViewsOfMol &molviews);
    virtual void remove(const Molecules &molecules);
    virtual void remove(const MolGroup &molgroup);
    
    virtual void removeAll(const MoleculeView &molview);
    virtual void removeAll(const ViewsOfMol &molviews);
    virtual void removeAll(const Molecules &molecules);
    virtual void removeAll(const MolGroup &molgroup);

    virtual void remove(MolNum molnum);
    virtual void remove(const QSet<MolNum> &molnums);

    virtual void removeAll(const MGID &mgid)=0;
    virtual void removeAll();
    
    virtual void remove(const MoleculeView &molview, const MGID &mgid)=0;
    virtual void remove(const ViewsOfMol &molviews, const MGID &mgid)=0;
    virtual void remove(const Molecules &molecules, const MGID &mgid)=0;
    virtual void remove(const MolGroup &molgroup, const MGID &mgid)=0;
    
    virtual void removeAll(const MoleculeView &molview, const MGID &mgid)=0;
    virtual void removeAll(const ViewsOfMol &molviews, const MGID &mgid)=0;
    virtual void removeAll(const Molecules &molecules, const MGID &mgid)=0;
    virtual void removeAll(const MolGroup &molgroup, const MGID &mgid)=0;

    virtual void remove(MolNum molnum, const MGID &mgid)=0;
    virtual void remove(const QSet<MolNum> &molnums, const MGID &mgid)=0;

    virtual void update(const MoleculeData &moldata)=0;
    void update(const MoleculeView &molview);
    
    virtual void update(const Molecules &molecules)=0;
    virtual void update(const MolGroup &molgroup)=0;
    
    virtual void setContents(const MGID &mgid, const MoleculeView &molview)=0;
    virtual void setContents(const MGID &mgid, const ViewsOfMol &molviews)=0;
    virtual void setContents(const MGID &mgid, const Molecules &molecules)=0;
    virtual void setContents(const MGID &mgid, const MolGroup &molgroup)=0;
    
protected:
    MolGroupsBase();
    
    MolGroupsBase(const MolGroupsBase &other);

    MolGroupsBase& operator=(const MolGroupsBase &other);

    virtual const MolGroup& getGroup(MGNum mgnum) const=0;
    
    virtual void getGroups(const QList<MGNum> &mgnums,
                           QVarLengthArray<const MolGroup*,10> &groups) const=0;

    virtual QHash<MGNum,const MolGroup*> getGroups() const=0;

    bool needToUpdate(const MoleculeData &moldata) const;

    const MoleculeData& matchToExistingVersion(const MoleculeData &moldata) const;

    Molecules matchToExistingVersion(const Molecules &molecules) const;

    void addToIndex(const MolGroup &molgroup);
    void addToIndex(MGNum mgnum, MolNum molnum);
    void addToIndex(MGNum mgnum, const QSet<MolNum> &molnums);
    
    void removeFromIndex(MGNum mgnum);
    void removeFromIndex(MolNum molnum);
    
    void removeFromIndex(MGNum mgnum, MolNum molnum);
    void removeFromIndex(MGNum mgnum, const QSet<MolNum> &molnums);

    void changeNameIndex(MGNum mgnum, const MGName &old_name, 
                         const MGName &new_name);

    void clearIndex(MGNum mgnum);

private:
    /** This index keeps an order of MolGroup objects */
    QList<MGNum> mgidx_to_num;
    
    /** This index maps the names of the MolGroup objects */
    QHash< MGName, QList<MGNum> > mgname_to_mgnum;
    
    /** This is an index of which groups contain which molecules */
    QHash< MolNum, QList<MGNum> > molnum_to_mgnum;
};

/** This class holds a collection of MoleculeGroup objects. This
    allows multiple groups to be themselves grouped together.
    This is a virtual class, which can hold the virtual
    MoleculeGroup class objects. This can be used usefully in
    several situations, e.g.;
    
    System is derived from MolGroupsBase, and uses the MolGroups
    code to manage the indexing and version management of all
    of the molecules in the system.
    
    The forcefields are also derived from MolGroupsBase, allowing
    the MolGroups code to do the indexing and version management
    of molecules in a forcefield. Also, this allows easy 
    management of multiple groups in a forcefield, e.g.
    QM molecules and MM molecules, or group A and group B.
    
    While System and the forcefields are derived from MolGroupsBase,
    this class, MolGroups, provides a concrete class that allows
    the user to easily group together different MoleculeGroups.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolGroups : public ConcreteProperty<MolGroups,MolGroupsBase>
{

friend QDataStream& ::operator<<(QDataStream&, const MolGroups&);
friend QDataStream& ::operator>>(QDataStream&, MolGroups&);

public:
    MolGroups();
    
    MolGroups(const MolGroup &molgroup);
    
    MolGroups(const QList<MoleculeGroup> &molgroups);
    
    MolGroups(const MolGroups &other);
    
    ~MolGroups();

    MolGroups* clone() const
    {
        return new MolGroups(*this);
    }
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolGroups>() );
    }
    
    MolGroups& operator=(const MolGroups &other);
    
    bool operator==(const MolGroups &other) const;
    bool operator!=(const MolGroups &other) const;
    
    MolGroups& operator+=(const MolGroup &molgroup);
    MolGroups& operator-=(const MolGroup &molgroup);
    
    MolGroups& operator-=(const MGID &mgid);
    
    MolGroups& operator-=(const Molecules &molecules);
    MolGroups& operator-=(const MolID &molid);

    void add(const MolGroup &molgroup);
    void remove(MGNum mgnum);

    void remove(const MolGroup &molgroup);
    void remove(const MGID &mgid);
    void remove(const MolID &molid);
    
    ///////////////////////////////////////////////
    /// Pure virtual functions of MolGroupsBase ///
    ///////////////////////////////////////////////

    const MolGroup& at(MGNum mgnum) const;

    void add(const MoleculeView &molview, const MGID &mgid);
    void add(const ViewsOfMol &molviews, const MGID &mgid);
    void add(const Molecules &molecules, const MGID &mgid);
    void add(const MolGroup &molgroup, const MGID &mgid);
    
    void addIfUnique(const MoleculeView &molview, const MGID &mgid);
    void addIfUnique(const ViewsOfMol &molviews, const MGID &mgid);
    void addIfUnique(const Molecules &molecules, const MGID &mgid);
    void addIfUnique(const MolGroup &molgroup, const MGID &mgid);

    void remove(const MoleculeView &molview, const MGID &mgid);
    void remove(const ViewsOfMol &molviews, const MGID &mgid);
    void remove(const Molecules &molecules, const MGID &mgid);
    void remove(const MolGroup &molgroup, const MGID &mgid);
    
    void removeAll(const MoleculeView &molview, const MGID &mgid);
    void removeAll(const ViewsOfMol &molviews, const MGID &mgid);
    void removeAll(const Molecules &molecules, const MGID &mgid);
    void removeAll(const MolGroup &molgroup, const MGID &mgid);

    void removeAll(const MGID &mgid);

    void remove(MolNum molnum, const MGID &mgid);
    void remove(const QSet<MolNum> &molnums, const MGID &mgid);

    void update(const MoleculeData &moldata);
    
    void update(const Molecules &molecules);
    void update(const MolGroup &molgroup);
    
    void setContents(const MGID &mgid, const MoleculeView &molview);
    void setContents(const MGID &mgid, const ViewsOfMol &molviews);
    void setContents(const MGID &mgid, const Molecules &molecules);
    void setContents(const MGID &mgid, const MolGroup &molgroup);
 
protected:
    const MolGroup& getGroup(MGNum mgnum) const;

    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MolGroup*,10> &groups) const;

    QHash<MGNum,const MolGroup*> getGroups() const;

private:
    /** All of the MoleculeGroup objects in this collection, 
        indexed by molecule group number */
    QHash<MGNum,MoleculeGroup> mgroups;
};

}

Q_DECLARE_METATYPE(SireMol::MolGroups);

SIRE_EXPOSE_CLASS( SireMol::MolGroupsBase )
SIRE_EXPOSE_CLASS( SireMol::MolGroups )

SIRE_END_HEADER

#endif
