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

#include <boost/scoped_ptr.hpp>

#include <QSet>

#include "SireBase/sharedpolypointer.hpp"
#include "SireBase/idmajminversion.h"

#include "SireCAS/function.h"
#include "SireCAS/values.h"

#include "SireMol/moleculeid.h"
#include "SireMol/idmolatom.h"

#include "parametermap.h"
#include "ffcomponent.h"

#include "forcefieldid.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFBase;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFBase&);
QDataStream& operator>>(QDataStream&, SireFF::FFBase&);

namespace SireMol
{
class PartialMolecule;
class Molecule;
class Residue;
class NewAtom;

class MoleculeID;
class ResNum;
class ResID;
}

namespace SireFF
{

class ForceField;
class MovedMols;

using SireBase::Version;
using SireBase::IDMajMinVersion;

using SireCAS::Function;
using SireCAS::Values;
using SireCAS::Symbol;
using SireCAS::Symbols;

using SireMol::MoleculeID;
using SireMol::Molecule;
using SireMol::Residue;
using SireMol::ResNum;
using SireMol::ResID;
using SireMol::NewAtom;
using SireMol::IDMolAtom;
using SireMol::PartialMolecule;

/**
This class is the base class of all of the forcefield classes. The forcefields all form
a polymorphic class hierarchy derived from this class. They are then held via
the SharedPolyPointer in ForceField, which provides the common user-interface to the
forcefields.

@author Christopher Woods
*/
class SIREFF_EXPORT FFBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const FFBase&);
friend QDataStream& ::operator>>(QDataStream&, FFBase&);

public:
    FFBase();
    FFBase(const QString &name);

    FFBase(const FFBase &other);

    virtual ~FFBase();

    virtual FFBase& operator=(const FFBase &other)=0;
    FFBase& operator=(const ForceField &ffield);

    static const char* typeName()
    {
        return "SireFF::FFBase";
    }

    /** Return the class name of the forcefield */
    virtual const char* what() const=0;

    /** Return a clone of this forcefield */
    virtual FFBase* clone() const=0;

    virtual QString toString() const;

    /** This encapsulated class must be derived by
        each inheriting forcefield to provide information
        about all of the components of the forcefield */
    class SIREFF_EXPORT Components
    {
    friend class FFBase;

    public:
        Components();
        Components(const FFBase &ffield, const Symbols &symbols = Symbols());
        Components(const Components &other);

        virtual ~Components();

        virtual Components* clone() const
        {
            return new Components(*this);
        }

        Components& operator=(const Components &other);

        const FFComponent& total() const
        {
            return e_total;
        }

        static QString describe_total();

        static Symbol x()
        {
            return _x;
        }

        static Symbol y()
        {
            return _y;
        }

        static Symbol z()
        {
            return _z;
        }

        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4, const Symbol &sym5);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                            const Symbol &sym6);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                            const Symbol &sym6, const Symbol &sym7);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                            const Symbol &sym6, const Symbol &sym7, const Symbol &sym8);
        static Symbols addToSymbols(Symbols symbols,
                            const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                            const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                            const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                            const Symbol &sym9);

        bool contains(const FFComponent &component) const
        {
            return symbolids.contains(component.ID());
        }

        void assertContains(const FFComponent &component) const;

    protected:
        virtual void setForceField(const FFBase &ffbase);

        void registerComponent(const FFComponent &component);

    private:
        static Symbol _x;
        static Symbol _y;
        static Symbol _z;

        /** The FFComponent representing the total energy
            of the forcefield */
        FFComponent e_total;

        /** The SymbolIDs of all of the functions that
            represent the components */
        QSet<SireCAS::SymbolID> symbolids;
    };

    /** Return the object describing the components of this
        forcefield */
    const FFBase::Components& components() const
    {
        return *components_ptr;
    }

    /** This encapsulated class must be derived by all
        inheriting classes to provide the object
        that contains information about the parameters
        used by the forcefield, and the default source
        properties from which to obtain those parameters */
    class SIREFF_EXPORT Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);
        virtual ~Parameters();
    };

    virtual const FFBase::Parameters& parameters() const=0;

    /** This encapsulated class provides an ID for a group
        within this forcefield */
    class SIREFF_EXPORT Group
    {
    public:
        explicit Group(quint32 val=0) : _val(val)
        {}

        Group(const Group &other) : _val(other._val)
        {}

        ~Group()
        {}

        operator quint32() const
        {
            return _val;
        }

    private:
        quint32 _val;
    };

    /** This encapsulated class must be derived by all
        inheriting classes to provide the object that
        contains information about the available groups
        in the forcefield */
    class SIREFF_EXPORT Groups
    {
    friend class FFBase;

    public:
        Groups();
        Groups(const Groups &other);

        virtual ~Groups();

        /** Return the name of the main group
            in the forcefield (the default group) */
        FFBase::Group main() const
        {
            return mainid;
        }

        /** Return the number of groups in this forcefield */
        quint32 count() const
        {
            return n;
        }

    protected:
        static Groups default_group;

        FFBase::Group getUniqueID();

    private:
        /** The ID of the main (default) group
            of this forcefield */
        FFBase::Group mainid;

        /** The number of groups in this forcefield */
        quint32 n;
    };

    virtual const FFBase::Groups& groups() const
    {
        return FFBase::Groups::default_group;
    }

    const QString& name() const;
    void setName(const QString &name);

    double energy();
    double energy(const Function &component);

    Values energies(const QSet<FFComponent> &components);
    Values energies();

    // Pure virtual functions used to provide the interface to
    // all of the forcefields

    /** Change the molecule 'mol' (e.g. move it, or change its
        parameters). This does nothing if the molecule is not
        in this forcefield. Returns whether or not the forcefield
        has been changed by this change, and thus whether the
        energy needs to be recalculated. The same parameter map
        that was used when this molecule was added will be used
        to extract any necessary parameters from the molecule's
        properties

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool change(const Molecule &mol)=0;

    /** Change the residue 'res' (e.g. move it, or change its
        parameters). This does nothing if the residue is not
        in this forcefield. Returns whether or not the forcefield
        has been changed by this change, and thus whether the
        energy needs to be recalculated.

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool change(const Residue &res)=0;

    /** Change the atom 'atom'  (e.g. move it, or change
        its parameters). This does nothing if this atom
        is not in this forcefield. Returns whether or not
        the forcefield has been changed by this change, and
        thus whether the energy needs to be recalculated.

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool change(const NewAtom &atom)=0;

    /** Change a whole load of molecules

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool change(const QHash<MoleculeID,Molecule> &mols)=0;

    /** Change a whole load of partial molecules

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool change(const QHash<MoleculeID,PartialMolecule> &mols)=0;

    /** Add the molecule 'molecule' to this forcefield using
        the optional parameter map to find any necessary parameters
        from properties of the molecule. This will replace any
        existing copy of the molecule that already exists in
        this forcefield. This returns whether or not the
        forcefield has been changed by this addition, and therefore
        whether its energy needs recalculating.

        \throw SireMol::missing_property
        \throw SireError::invalid_cast
        \throw SireError::invalid_operation
    */
    virtual bool add(const Molecule &molecule,
                     const ParameterMap &map = ParameterMap())=0;

    /** Add the residue 'residue' to this forcefield using
        the optional parameter map to find any necessary parameters
        from properties of the residue. This will replace any
        existing copy of the residue that already exists in
        this forcefield. This returns whether or not the
        forcefield has been changed by this addition, and therefore
        whether its energy needs recalculating.

        This will throw an exception if this forcefield does not
        support partial molecules.

        \throw SireError::invalid_operation
        \throw SireMol::missing_property
        \throw SireError::invalid_cast
    */
    virtual bool add(const Residue &residue,
                     const ParameterMap &map = ParameterMap());

    /** Add the atom 'atom' to this forcefield using the
        optional parameter map to find any necessay parameters
        from properties of the atom. This will replace any
        existing copy of the atom that already exists in
        this forcefield. This returns whether or not the
        forcefield has been changed by this addition, and therefore
        whether its energy needs recalculating.

        This will throw an exception if this forcefield doens't
        support partial molecules.

        \throw SireError::invalid_operation
        \throw SireMol::missing_property
        \throw SireError::invalid_cast
    */
    virtual bool add(const NewAtom &atom,
                     const ParameterMap &map = ParameterMap());

    /** Add the partial molecule 'molecule' to this forcefield using the
        optional parameter map to find any necessay parameters
        from properties of the atom. This will replace any
        existing copy of the atom that already exists in
        this forcefield. This returns whether or not the
        forcefield has been changed by this addition, and therefore
        whether its energy needs recalculating.

        This will throw an exception if this forcefield doens't
        support partial molecules.

        \throw SireError::invalid_operation
        \throw SireMol::missing_property
        \throw SireError::invalid_cast
    */
    virtual bool add(const PartialMolecule &molecule,
                     const ParameterMap &map = ParameterMap())=0;

    virtual bool addTo(const FFBase::Group &group,
                       const Molecule &molecule,
                       const ParameterMap &map = ParameterMap());

    virtual bool addTo(const FFBase::Group &group,
                       const Residue &residue,
                       const ParameterMap &map = ParameterMap());

    virtual bool addTo(const FFBase::Group &group,
                       const NewAtom &atom,
                       const ParameterMap &map = ParameterMap());

    virtual bool addTo(const FFBase::Group &group,
                       const PartialMolecule &molecule,
                       const ParameterMap &map = ParameterMap());

    virtual bool add(const QList<Molecule> &molecules,
                     const ParameterMap &map = ParameterMap());
    virtual bool add(const QList<Residue> &residues,
                     const ParameterMap &map = ParameterMap());
    virtual bool add(const QList<NewAtom> &atoms,
                     const ParameterMap &map = ParameterMap());
    virtual bool add(const QList<PartialMolecule> &molecules,
                     const ParameterMap &map = ParameterMap());

    virtual bool addTo(const FFBase::Group &group,
                       const QList<Molecule> &molecules,
                       const ParameterMap &map = ParameterMap());
    virtual bool addTo(const FFBase::Group &group,
                       const QList<Residue> &residues,
                       const ParameterMap &map = ParameterMap());
    virtual bool addTo(const FFBase::Group &group,
                       const QList<NewAtom> &atoms,
                       const ParameterMap &map = ParameterMap());
    virtual bool addTo(const FFBase::Group &group,
                       const QList<PartialMolecule> &molecules,
                       const ParameterMap &map = ParameterMap());

    /** Remove the molecule 'molecule' from this forcefield - this
        does nothing if the molecule is not in this forcefield. This
        returns whether this has changed the forcefield (therefore
        necessitating a recalculation of the energy)

        \throw SireError::invalid_operation
    */
    virtual bool remove(const Molecule &molecule)=0;

    /** Remove the residue 'residue' from this forcefield - this
        does nothing if the residue is not in this forcefield. This
        returns whether this has changed the forcefield (therefore
        necessitating a recalculation of the energy)

        This will throw an exception if this forcefield does not
        support partial molecules.

        \throw SireError::invalid_operation
    */
    virtual bool remove(const Residue &residue)=0;

    /** Remove the atom 'atom' from this forcefield - this does
        nothing if the atom is not in this forcefield. This
        returns whether this has changed the forcefield (therefore
        necessitating a recalculation of the energy)

        This will throw an exception if this forcefield does not
        support partial molecules.

        \throw SireError::invalid_operation
    */
    virtual bool remove(const NewAtom &atom)=0;

    /** Remove the partial molecule 'molecule' from
        the molecule 'molecule' from this forcefield - this does
        nothing if the atoms are not in this forcefield. This
        returns whether this has changed the forcefield (therefore
        necessitating a recalculation of the energy)

        This will throw an exception if this forcefield does not
        support partial molecules.

        \throw SireError::invalid_operation
    */
    virtual bool remove(const PartialMolecule &molecule)=0;

    virtual bool remove(const QList<Molecule> &molecules);
    virtual bool remove(const QList<Residue> &residues);
    virtual bool remove(const QList<NewAtom> &atoms);
    virtual bool remove(const QList<PartialMolecule> &molecules);

    /** Return whether this forcefield contains a complete copy of
        any version of the Molecule 'molecule' */
    virtual bool contains(const Molecule &molecule) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the Residue 'residue' */
    virtual bool contains(const Residue &residue) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the Atom 'atom' */
    virtual bool contains(const NewAtom &atom) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the partial molecule 'molecule' */
    virtual bool contains(const PartialMolecule &molecule) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the Molecule 'molecule' in the group 'group' */
    virtual bool contains(const Molecule &molecule,
                          const FFBase::Group &group) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the Residue 'residue' in the group 'group' */
    virtual bool contains(const Residue &residue,
                          const FFBase::Group &group) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the Atom 'atom' in the group 'group' */
    virtual bool contains(const NewAtom &atom,
                          const FFBase::Group &group) const=0;

    /** Return whether this forcefield contains a complete copy of
        any version of the partial molecule 'molecule' in the group 'group' */
    virtual bool contains(const PartialMolecule &molecule,
                          const FFBase::Group &group) const=0;

    /** Return whether or not this forcefield contains *any part* of
        any version of the molecule with ID 'molid' */
    virtual bool refersTo(MoleculeID molid) const=0;

    /** Return whether or not the group 'group' in this forcefield
        contains *any part* of any version of the molecule with ID
        'molid' */
    virtual bool refersTo(MoleculeID molid, const FFBase::Group &group) const=0;

    /** Return the set of all of the ID numbers of all of the
        molecules that are referred to by this forcefield
        (i.e. all molecules that have at least some part
        in this forcefield) */
    virtual QSet<MoleculeID> moleculeIDs() const=0;

    /** Return the set of all of the ID numbers of all of the
        molecules that are referred to by group 'group' in
        this forcefield (i.e. all molecules that have at least
        some part in this group in this forcefield) */
    virtual QSet<MoleculeID> moleculeIDs(const FFBase::Group &group) const=0;

    /** Return the copy of the molecule in this forcefield that
        has the ID == molid

        \throw SireMol::missing_molecule
    */
    virtual Molecule molecule(MoleculeID molid) const=0;

    virtual Residue residue(MoleculeID molid, ResNum resnum) const;
    virtual Residue residue(MoleculeID molid, ResID resid) const;
    virtual Residue residue(MoleculeID molid, const QString &resname) const;

    virtual NewAtom atom(MoleculeID molid, const IDMolAtom &atomid) const;

    virtual Molecule molecule(const Molecule &mol) const;
    virtual Residue residue(const Residue &res) const;
    virtual NewAtom atom(const NewAtom &atom) const;

    virtual PartialMolecule contents(MoleculeID molid) const=0;
    virtual QHash<MoleculeID,PartialMolecule> contents(
                                        const FFGroup::Group group) const=0;
    virtual QHash<MoleculeID,PartialMolecule> contents() const=0;

    bool isDirty() const;
    bool isClean() const;

    ForceFieldID ID() const;
    const Version& version() const;

    void assertContains(const FFComponent &component) const;

protected:
    void registerComponents(FFBase::Components *components);

    FFBase& copy(const FFBase &other);

    void incrementMajorVersion();
    void incrementMinorVersion();

    void setComponent(const FFComponent &comp, double nrg);
    void changeComponent(const FFComponent &comp, double delta);

    void setDirty();
    void setClean();

    Values currentEnergies() const;

    static QHash<MoleculeID,int> index(const QVector<Molecule> &molecules);

    /** Virtual function used to trigger a recalculation of the total energy
        and of all of the component energies */
    virtual void recalculateEnergy()=0;

    /** Virtual function used to return the current copy of the molecule 'mol'.
        The entire molecule is returned even if the forcefield contains only
        a part of the molecule */
    virtual Molecule getMolecule(MoleculeID molid) const=0;

private:
    template<class T>
    bool _pvt_add(const QList<T> &objs,
                  const ParameterMap &map);

    template<class T>
    bool _pvt_addTo(const FFBase::Group &group, const T &obj,
                    const ParameterMap &map);

    template<class T>
    bool _pvt_addTo(const FFBase::Group &group,
                    const QList<T> &objs,
                    const ParameterMap &map);

    template<class T>
    bool _pvt_remove(const QList<T> &objs);

    /** The name of this forcefield - this may be used to give a unique
        name to all of the component-symbols in this forcefield. */
    QString ffname;

    /** The ID number and version numbers of this forcefield. The
        major version number changes whenever the number of molecules
        in the forcefield changes - the minor number changes whenever
        the molecules themselves change (move or change parameters) */
    IDMajMinVersion id_and_version;

    /** All of the cached energy components in this forcefield, indexed
        by their symbol ID number (includes the total energy) */
    Values nrg_components;

    /** Active pointer to the object containing the information about all
        of the components of this forcefield */
    boost::scoped_ptr<FFBase::Components> components_ptr;

    /** Whether or not this forcefield is dirty (requires an update) */
    bool isdirty;
};

/** Set the energy value of the component 'comp' */
inline void FFBase::setComponent(const FFComponent &comp, double nrg)
{
    nrg_components.set(comp,nrg);
}

/** Change the existing value of the component 'comp' by delta */
inline void FFBase::changeComponent(const FFComponent &comp, double delta)
{
    nrg_components.set( comp, delta + nrg_components.value(comp) );
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

/** Internal function used by derived classes to increment the major
    number of the forcefield */
inline void FFBase::incrementMajorVersion()
{
    id_and_version.incrementMajor();
    setDirty();
}

/** Internal function used by derived classes to increment the minor
    number of the forcefield */
inline void FFBase::incrementMinorVersion()
{
    id_and_version.incrementMinor();
    setDirty();
}

/** Return the ID number of the forcefield */
inline ForceFieldID FFBase::ID() const
{
    return ForceFieldID(id_and_version.ID());
}

/** Return the version number of the forcefield */
inline const Version& FFBase::version() const
{
    return id_and_version.version();
}

/** Assert that this forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
inline void FFBase::assertContains(const FFComponent &component) const
{
    components().assertContains(component);
}

}

SIRE_END_HEADER

#endif
