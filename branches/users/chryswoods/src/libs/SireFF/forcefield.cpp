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

#include "SireCAS/qhash_sirecas.h"

#include "forcefield.h"
#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireFF;

static const RegisterMetaType<ForceField> r_forcefield;

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceField&)
{
    writeHeader(ds, r_forcefield, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceField&)
{
    VersionID v = readHeader(ds, r_forcefield);

    if (v != 0)
        throw version_error(v, "1", r_forcefield, CODELOC);

    return ds;
}

namespace SireFF
{
namespace detail
{
class NullFF;
}
}

QDataStream& operator<<(QDataStream&, const SireFF::detail::NullFF&);
QDataStream& operator>>(QDataStream&, SireFF::detail::NullFF&);

namespace SireFF
{
namespace detail
{

/** This is a null forcefield, which is used to provide a null object
    to prevent segfaults when using the null ForceField class.

    @author Christopher Woods
*/
class NullFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const NullFF&);
friend QDataStream& ::operator>>(QDataStream&, NullFF&);

public:
    NullFF() : FFBase()
    {}

    NullFF(const NullFF &other) : FFBase(other)
    {}

    ~NullFF()
    {}

    class SIREFF_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters()
        {}

        ~Parameters()
        {}

        static Parameters default_parameter;
    };

    const Parameters& parameters() const
    {
        return Parameters::default_parameter;
    }

    static const char* typeName()
    {
        return "SireFF::detail::NullFF";
    }

    const char* what() const
    {
        return NullFF::typeName();
    }

    NullFF* clone() const
    {
        return new NullFF(*this);
    }

protected:
    void recalculateEnergy()
    {}

    /** Null molecule returned by the null forcefield */
    static Molecule null_molecule;
};

NullFF::Parameters NullFF::Parameters::default_parameter;

} // detail
} // SireFF

////////////
//////////// Implementation of NullFF
////////////

Q_DECLARE_METATYPE(SireFF::detail::NullFF)

using namespace SireFF::detail;

static const RegisterMetaType<NullFF> r_nullff;

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const NullFF &nullff)
{
    writeHeader(ds, r_nullff, 1)
                  << static_cast<const FFBase&>(nullff);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, NullFF &nullff)
{
    VersionID v = readHeader(ds, r_nullff);

    if (v == 1)
    {
        ds >> static_cast<FFBase&>(nullff);
    }
    else
        throw version_error(v, "1", r_nullff, CODELOC);

    return ds;
}

Molecule NullFF::null_molecule;

////////////
//////////// Implementation of ForceField
////////////

static const SharedPolyPointer<FFBase> shared_null( new NullFF() );

/** Constructor */
ForceField::ForceField() : SharedPolyPointer<FFBase>(shared_null)
{}

/** Construct from the passed FFBase forcefield */
ForceField::ForceField(const FFBase &ffield)
           : SharedPolyPointer<FFBase>(ffield.clone())
{}

/** Construct from a shared pointer to a forcefield */
ForceField::ForceField(const SharedPolyPointer<FFBase> &ffptr)
           : SharedPolyPointer<FFBase>(ffptr)
{
    if (not constData())
        SharedPolyPointer<FFBase>::operator=(shared_null);
}

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : SharedPolyPointer<FFBase>(other)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Assignment operator */
ForceField& ForceField::operator=(const ForceField &other)
{
    SharedPolyPointer<FFBase>::operator=(other);
    return *this;
}

/** Assignment operator */
ForceField& ForceField::operator=(const FFBase &other)
{
    SharedPolyPointer<FFBase>::operator=(other.clone());
    return *this;
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
Molecule ForceField::molecule(MoleculeID molid) const
{
    return d().molecule(molid);
}

/** Return the copy of the residue in this forcefield that
    is in the molecule with ID == molid and with residue number
    'resnum'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_residue
*/
Residue ForceField::residue(MoleculeID molid, ResNum resnum) const
{
    return d().residue(molid, resnum);
}

/** Return a copy of the atom in this forcefield that is
    in the molecule with ID == molid and with atom index 'atomid'

    \throw SireMol::missing_molecule
    \throw SireMol::missing_atom
*/
NewAtom ForceField::atom(MoleculeID molid, const IDMolAtom &atomid) const
{
    return d().atom(molid, atomid);
}

/** Return the copy of the molecule 'mol' that is in this forcefield

    \throw SireMol::missing_molecule
*/
Molecule ForceField::molecule(const Molecule &mol) const
{
    return d().molecule(mol);
}

/** Return the copy of the residue 'res' that is in this forcefield

    \throw SireMol::missing_residue
*/
Residue ForceField::residue(const Residue &res) const
{
    return d().residue(res);
}

/** Return a copy of the atom 'atom' that is in this forcefield

    \throw SireMol::missing_atom
*/
NewAtom ForceField::atom(const NewAtom &atom) const
{
    return d().atom(atom);
}

/** Return a reference to the FFBase base of this forcefield */
const FFBase& ForceField::d() const
{
    return *(constData());
}

/** Return a reference to the FFBase base of this forcefield */
FFBase& ForceField::d()
{
    return *(data());
}

/** Return a string containing the type of the forcefield */
const char* ForceField::what() const
{
    return d().what();
}

/** Return the components in this forcefield */
const FFBase::Components& ForceField::components() const
{
    return d().components();
}

/** Return the parameters necessary for this forcefield */
const FFBase::Parameters& ForceField::parameters() const
{
    return d().parameters();
}

/** Return the name of the forcefield */
const QString& ForceField::name() const
{
    return d().name();
}

/** Set the name of the forcefield */
void ForceField::setName(const QString &name)
{
    d().setName(name);
}

/** Return the total energy of the forcefield */
double ForceField::energy()
{
    return d().energy();
}

/** Return the energy of the component represented by the
    function 'component'

    \throw SireFF::missing_component
*/
double ForceField::energy(const FFComponent &component)
{
    return d().energy(component);
}

/** Return the energies of all of the specified components

    \throw SireFF::missing_component
*/
Values ForceField::energies(const QSet<FFComponent> &components)
{
    return d().energies(components);
}

/** Return the energies of all of the components */
Values ForceField::energies()
{
    return d().energies();
}

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
*/
bool ForceField::change(const Molecule &mol)
{
    return d().change(mol);
}

/** Change the molecules 'molecules' (e.g. move them, or change
    their parameters). This does nothing if the molecules are not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated. The same parameter map
    that was used when these molecules were added will be used
    to extract any necessary parameters from the molecule's
    properties

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::change(const QHash<MoleculeID,Molecule> &molecules)
{
    return d().change(molecules);
}

/** Change the residue 'res' (e.g. move it, or change its
    parameters). This does nothing if the residue is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::change(const Residue &res)
{
    return d().change(res);
}

/** Change the atom 'atom' (e.g. move it, or change its
    parameters). This does nothing if the atom is not
    in this forcefield. Returns whether or not the
    forcefield has been changed by this change, and thus
    whether the energy needs to be recalculated.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::change(const NewAtom &atom)
{
    return d().change(atom);
}

/** Add the molecule 'molecule' to this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule.

    This will add the molecule if either the
    whole molecule does not already exist in the
    forcefield, or if the existing copy has a lower
    version number.

    This returns whether or not the forcefield was
    changed by this addition.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::add(const Molecule &molecule,
                            const ParameterMap &map)
{
    return d().add(molecule,map);
}

/** Add the residue 'residue' to this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the residue.

    This will add the residue if either the
    whole residue does not already exist in the
    forcefield, or if the existing copy has a lower
    version number.

    This will throw an exception if this forcefield does not
    support partial molecules.

    This returns whether or not the forcefield was
    changed by this addition.

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::add(const Residue &residue,
                            const ParameterMap &map)
{
    return d().add(residue,map);
}

/** Add the atom 'atom' to this forcefield using
    the optional parameter map to find any necessary
    parameters from properties of the atom.

    This will add the atom if either it doesn't
    exist in the forcefield, or if the existing
    copy has a lower version number.

    This will throw an exception if this forcefield does
    not support partial molecules.

    This returns whether or not the forcefield was
    changed by this addition.

    \throw SireError::invalid_operation
    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool ForceField::add(const NewAtom &atom,
                            const ParameterMap &map)
{
    return d().add(atom,map);
}

/** Add lots of molecules to the forcefield */
bool ForceField::add(const QList<Molecule> &molecules,
                            const ParameterMap &map)
{
    return d().add(molecules, map);
}

/** Add lots of residues to the forcefield */
bool ForceField::add(const QList<Residue> &residues,
                            const ParameterMap &map)
{
    return d().add(residues, map);
}

/** Add lots of atoms to the forcefield */
bool ForceField::add(const QList<NewAtom> &atoms,
                            const ParameterMap &map)
{
    return d().add(atoms, map);
}

/** Add the selected atoms 'selected_atoms' to this forcefield using the
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
bool ForceField::add(const Molecule &molecule,
                            const AtomSelection &selected_atoms,
                            const ParameterMap &map)
{
    return d().add(molecule, selected_atoms, map);
}

/** Add the selected atoms 'selected_atoms' to this forcefield using the
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
bool ForceField::addTo(const FFBase::Group &group, const Molecule &molecule,
                              const AtomSelection &selected_atoms, const ParameterMap &map)
{
    return d().addTo(group, molecule, selected_atoms, map);
}

/** Add the molecule 'molecule' to the group 'group' in this forcefield using
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
bool ForceField::addTo(const FFBase::Group &group,
                              const Molecule &molecule, const ParameterMap &map)
{
    return d().addTo(group, molecule, map);
}

/** Add the residue 'residue' to the group 'group'
    in this forcefield using
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
bool ForceField::addTo(const FFBase::Group &group,
                              const Residue &residue, const ParameterMap &map)
{
    return d().addTo(group, residue, map);
}

/** Add the atom 'atom' to the group 'group'
    in this forcefield using the
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
bool ForceField::addTo(const FFBase::Group &group,
                              const NewAtom &atom, const ParameterMap &map)
{
    return d().addTo(group, atom, map);
}

/** Add lots of molecules to group 'group' in this forcefield */
bool ForceField::addTo(const FFBase::Group &group,
                              const QList<Molecule> &molecules,
                              const ParameterMap &map)
{
    return d().addTo(group, molecules, map);
}

/** Add lots of residues to group 'group' in this forcefield */
bool ForceField::addTo(const FFBase::Group &group,
                              const QList<Residue> &molecules,
                              const ParameterMap &map)
{
    return d().addTo(group, residues, map);
}

/** Add lots of atoms to group 'group' in this forcefield */
bool ForceField::addTo(const FFBase::Group &group,
                              const QList<NewAtom> &atoms,
                              const ParameterMap &map)
{
    return d().addTo(group, atoms, map);
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy) */
bool ForceField::remove(const Molecule &molecule)
{
    return d().remove(molecule);
}

/** Remove the residue 'residue' from this forcefield - this
    does nothing if the residue is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy)

    This will throw an exception if this forcefield does not
    support partial molecules.

    \throw SireError::invalid_operation
*/
bool ForceField::remove(const Residue &residue)
{
    return d().remove(residue);
}

/** Remove the atom 'atom' from this forcefield - this
    does nothing if the atom is not in this forcefield.
    This returns whether this has changed the forcefield
    (therefore necessitating a recalculation of the
    energy)

    This will throw an exception if this forcefield does
    not support partial molecules

    \throw SireError::invalid_operation
*/
bool ForceField::remove(const NewAtom &atom)
{
    return d().remove(atom);
}

/** Remove the selected atoms in 'molecule' from this
    forcefield - this does nothing if the atom is not
    in this forcefield. This returns whether this has
    changed the forcefield (therefore necessitating
    a recalculation of the energy)

    This will throw an exception if this forcefield does
    not support partial molecules

    \throw SireError::invalid_operation
*/
bool ForceField::remove(const Molecule &molecule,
                               const AtomSelection &selected_atoms)
{
    return d().remove(molecule, selected_atoms);
}

/** Remove lots of molecules from this forcefield */
bool ForceField::remove(const QList<Molecule> &molecules)
{
    return d().remove(molecules);
}

/** Remove lots of residues from this forcefield */
bool ForceField::remove(const QList<Residue> &residues)
{
    return d().remove(residues);
}

/** Remove lots of atoms from this forcefield */
bool ForceField::remove(const QList<NewAtom> &atoms)
{
    return d().remove(atoms);
}

/** Return whether or not this forcefield contains *any part*
    of any version of the molecule 'molecule' */
bool ForceField::refersTo(const Molecule &molecule) const
{
    return d().refersTo(molecule);
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
     in this forcefield) */
QSet<MoleculeID> ForceField::moleculeIDs() const
{
    return d().moleculeIDs();
}

/** Return whether this forcefield contains a complete copy of
    any version of the molecule 'molecule' */
bool ForceField::contains(const Molecule &molecule) const
{
    return d().contains(molecule);
}

/** Return whether this forcefield contains a complete copy
    of any version of the residue 'residue' */
bool ForceField::contains(const Residue &residue) const
{
    return d().contains(residue);
}

/** Return whether or not this forcefield contains
    any version of the atom 'atom' */
bool ForceField::contains(const NewAtom &atom) const
{
    return d().contains(atom);
}

/** Return whether the forcefield is dirty (requires an energy recalcualtion) */
bool ForceField::isDirty() const
{
    return d().isDirty();
}

/** Return whether or not the forcefield is clean (does not require
    an energy recalcualtion) */
bool ForceField::isClean() const
{
    return d().isClean();
}

/** Return the ID number of the forcefield */
ForceFieldID ForceField::ID() const
{
    return d().ID();
}

/** Return the version number of the forcefield */
const Version& ForceField::version() const
{
    return d().version();
}

/** Assert that this forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
void ForceField::assertContains(const FFComponent &component) const
{
    d().assertContains(component);
}
