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

#ifndef SireFF_FFBASE_H
#define SireFF_FFBASE_H

#include "SireMol/molgroups.h"
#include "SireBase/properties.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFBase;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFBase&);
QDataStream& operator>>(QDataStream&, SireFF::FFBase&);

namespace SireFF
{

/** This class is the base class of all of the forcefield classes. 
    A forcefield is a collection of molecule groups (SireMol::MolGroups) 
    that had additional code to allow the energies, forces (and
    further derivatives) to be calculated for the molecules in those
    groups. The forcefield can, optionally, break this energy
    down into a set of components, and the energy, force (and/or
    further derivatives) of the components for the group(s) of
    molecules can be queried individually.

    FF derived objects are derived from MolGroupsBase, and hold
    molecule group objects that are derived from MolGroup, e.g.
    
    FFGroupPvt : public MolGroup  (used internally)
    
    FFGroup : public MolGroup (used externally - needed so can
                               hold a copy of the FF) - FFGroupPvt
                               auto-converts to FFGroup when copied

    This class is derived into a series of types, e.g.
    
    G1FF - forcefield that only contains a single group of molecules
    G2FF - forcefield that contains two groups of molecules
    
    MultiFF - forcefield that is an amalgamation of other forcefields
    
    There are also additional pure virtual interfaces, e.g.
    
    FF2D - interface for all 2-dimensional forcefields
           (can be used to calculate forces in 2D space)
    
    FF3D - interface for all 3-dimensional forcefields
           (can be used to calculate forces in 3D space)
    
    template<class Pot2B>
    Inter2BFF : public G1FF, protected Pot2B
    
    template<class Pot2B3D>
    Inter2B3DFF : public Inter2BFF<Pot2B3D>, public FF3D
    
    InterCLJFF : public Inter2B3DFF< Potential2B3D<CLJTerms> >
    
    template<class Pot2B>
    Intra2BFF : public G1FF, protected Pot2B
    
    template<class Pot2B3D>
    Intra2B3DFF : public Intra2BFF<Pot2B3D>, public FF3D
    
    IntraCLJFF : public Intra2BodyFF< TwoBodyPotential<CLJTerms> >
    
    template<class Pot2B>
    Inter2G2BFF : public G2FF, protected Pot2B
    
    template<class Pot2B3D>
    Inter2G2B3DFF : public Inter2G2BFF<Pot2B3D>, public FF3D
    
    InterGroupCLJFF : Inter2G2B3DFF< Potential2B3D<CLJTerms> >
    
    BondFF : public G1FF, public FF3D
    AngleFF : public G1FF, public FF3D
    DihedralFF : public G1FF, public FF3D
    
    Bond2DFF : public G1FF, public FF2D
    Angle2DFF : public G1FF, public FF2D
    Dihedral2DFF : public G1FF, public FF2D
    
    MultiFF : public FF
      - this is a FF that can hold multiple groups - it also
      - does its best to shared underlying MoleculeGroups...
    
    AmberFF : public MultiFF
      - contains InterCLJFF, IntraCLJFF, BondFF, 
                 AngleFF, DihedralFF in an array?
                  - has array of pointers for MultiFF?

    Amber2DFF : public MultiFF
      - contains InterCLJ2DFF, IntraCLJ2DFF, Bond2DFF,
                 Angle2DFF, Dihedral2DFF

    @author Christopher Woods
*/
class SIREFF_EXPORT FF : public SireMol::MolGroupsBase
{

friend QDataStream& ::operator<<(QDataStream&, const FFBase&);
friend QDataStream& ::operator>>(QDataStream&, FFBase&);

public:
    virtual ~FF();

    static const char* typeName()
    {
        return "SireFF::FF";
    }

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;

    /** Return a clone of this forcefield */
    virtual FF* clone() const=0;

    virtual QString toString() const;

    const FFName& name() const;
    void setName(const QString &name);

    FFNum number() const;
    
    quint64 majorVersion() const;
    quint64 minorVersion() const;

    SireUnits::Dimension::Energy energy();
    SireUnits::Dimension::Energy energy(const Symbol &component);

    Values energies(const QSet<Symbol> &components);
    Values energies();

    bool setProperty(const QString &name, const Property &value);

    Property getProperty(const QString &name) const;

    bool containsProperty(const QString &name) const;

    const Properties& properties() const;

    /** Tell the forcefield that it has to recalculate everything from
        scratch */
    virtual void mustNowRecalculateFromScratch()=0;

    bool isDirty() const;
    bool isClean() const;

    ForceFieldID ID() const;
    const Version& version() const;

    ///////
    /////// Overloading MolGroups functions
    ///////
        
    void add(const MoleculeView &molview, const MGID &mgid);
    void add(const ViewsOfMol &molviews, const MGID &mgid);
    void add(const Molecules &molecules, const MGID &mgid);
    void add(const MolGroup &molgroup, const MGID &mgid);
    
    void addIfUnique(const MoleculeView &molview, const MGID &mgid);
    void addIfUnique(const ViewsOfMol &molviews, const MGID &mgid);
    void addIfUnique(const Molecules &molecules, const MGID &mgid);
    void addIfUnique(const MolGroup &molgroup, const MGID &mgid);
    
    void remove(const MoleculeView &molview);
    void remove(const ViewsOfMol &molviews);
    void remove(const Molecules &molecules);
    void remove(const MolGroup &molgroup);
    
    void removeAll(const MoleculeView &molview);
    void removeAll(const ViewsOfMol &molviews);
    void removeAll(const Molecules &molecules);
    void removeAll(const MolGroup &molgroup);

    void remove(MolNum molnum);
    void remove(const QSet<MolNum> &molnums);

    void removeAll(const MGID &mgid);
    void removeAll();
    
    void remove(const MoleculeView &molview, const MGID &mgid);
    void remove(const ViewsOfMol &molviews, const MGID &mgid);
    void remove(const Molecules &molecules, const MGID &mgid);
    void remove(const MolGroup &molgroup, const MGID &mgid);
    
    void removeAll(const MoleculeView &molview, const MGID &mgid);
    void removeAll(const ViewsOfMol &molviews, const MGID &mgid);
    void removeAll(const Molecules &molecules, const MGID &mgid);
    void removeAll(const MolGroup &molgroup, const MGID &mgid);

    void remove(MolNum molnum, const MGID &mgid);
    void remove(const QSet<MolNum> &molnums, const MGID &mgid);

    void update(const MoleculeData &moldata);
    void update(const MoleculeView &molview);
    
    void update(const Molecules &molecules);
    void update(const MolGroup &molgroup);
    
    void setContents(const MGID &mgid, const MoleculeView &molview);
    void setContents(const MGID &mgid, const ViewsOfMol &molviews);
    void setContents(const MGID &mgid, const Molecules &molecules);
    void setContents(const MGID &mgid, const MolGroup &molgroup);

protected:
    FFBase();
    FFBase(const QString &name);

    FFBase(const FFBase &other);

    FFBase& operator=(const FFBase &other);
    FFBase& operator=(const ForceField &ffield);

    void setComponent(const Symbol &component, double nrg);
    void changeComponent(const Symbol &component, double delta);

    void setDirty();
    void setClean();

    const Values& currentEnergies() const;

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

    ////
    //// Overloading MolGroups virtual functions
    ////
    
    MolGroup& getGroup(MGNum mgnum); 
    const MolGroup& getGroup(MGNum mgnum) const;
    
    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<MolGroup*,10> &groups);

    void getGroups(const QList<MGNum> &mgnums,
                   QVarLengthArray<const MolGroup*,10> &groups) const;

    QHash<MGNum,MolGroup*> getGroups();
    QHash<MGNum,const MolGroup*> getGroups() const;

private:
    /** The name of this forcefield */
    FFName ffname;

    /** The number of this forcefield - this is a unique number
        used to provide a unique ID to all of the symbols in 
        this forcefield */
    FFNum ffnum;

    /** The values of all of the properties of this forcefield */
    Properties props;

    /** All of the cached energy components in this forcefield, indexed
        by their symbol ID number (includes the total energy) */
    Values nrg_components;

    /** Whether or not this forcefield is dirty (requires an update) */
    bool isdirty;
};

/** Set the energy value of the component 'comp' */
inline void FFBase::setComponent(const Symbol &component, double nrg)
{
    nrg_components.set(component,nrg);
}

/** Change the existing value of the component 'comp' by delta */
inline void FFBase::changeComponent(const Symbol &component, double delta)
{
    nrg_components.set( component, delta + nrg_components.value(component) );
}

/** Return whether or not the forcefield is dirty (the energy
    needs to be recalculated) */
inline bool FFBase::isDirty() const
{
    return isdirty;
}

/** Return whether or not the forcefield is clean (the energy
    does not need to be recalculated) */
inline bool FFBase::isClean() const
{
    return not isDirty();
}

}

SIRE_END_HEADER

#endif
