#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "ffbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

class FFBase;

/**
This class is the "user" interface to all forcefield classes. This holds an implicitly
shared copy of the forcefield class.

@author Christopher Woods
*/
class SIREFF_EXPORT ForceField : public SireBase::SharedPolyPointer<FFBase>
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const ForceField &other);
    ForceField(const FFBase &ffbase);
    ForceField(const SireBase::SharedPolyPointer<FFBase> &ffptr);

    ~ForceField();

    ForceField& operator=(const ForceField &other);
    ForceField& operator=(const FFBase &ffbase);

    const char* what() const;

    const QString& name() const;
    void setName(const QString& name);

    const FFBase::Components& components() const;
    const FFBase::Parameters& parameters() const;

    double energy();
    double energy(const FFComponent &component);

    Values energies();
    Values energies(const QSet<FFComponent> &components);

    bool change(const Molecule &mol);
    bool change(const Residue &res);
    bool change(const NewAtom &atom);

    bool add(const Molecule &molecule,
             const ParameterMap &map = ParameterMap());
    bool add(const Residue &residue,
             const ParameterMap &map = ParameterMap());
    bool add(const NewAtom &atom,
             const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);

    bool replace(const Molecule &oldmol,
                 const Molecule &newmol,
                 const ParameterMap &map = ParameterMap());

    bool contains(const Molecule &molecule) const;
    bool contains(const Residue &residue) const;
    bool contains(const NewAtom &atom) const;

    bool refersTo(const Molecule &molecule) const;
    
    QSet<MoleculeID> moleculeIDs() const;

    Molecule molecule(MoleculeID molid) const;
    Residue residue(MoleculeID molid, ResNum resnum) const;
    NewAtom atom(MoleculeID molid, const IDMolAtom &atomid) const;

    Molecule molecule(const Molecule &mol) const;
    Residue residue(const Residue &res) const;
    NewAtom atom(const NewAtom &atom) const;

    bool isDirty() const;
    bool isClean() const;

    ForceFieldID ID() const;
    const Version& version() const;

    void assertContains(const FFComponent &component) const;

protected:
    const FFBase& d() const;
    FFBase& d();
};

/** Return a reference to the FFBase base of this forcefield */
inline const FFBase& ForceField::d() const
{
    return *(constData());
}

/** Return a reference to the FFBase base of this forcefield */
inline FFBase& ForceField::d()
{
    return *(data());
}

/** Return a string containing the type of the forcefield */
inline const char* ForceField::what() const
{
    return d().what();
}

/** Return the components in this forcefield */
inline const FFBase::Components& ForceField::components() const
{
    return d().components();
}

/** Return the parameters necessary for this forcefield */
inline const FFBase::Parameters& ForceField::parameters() const
{
    return d().parameters();
}

/** Return the name of the forcefield */
inline const QString& ForceField::name() const
{
    return d().name();
}

/** Set the name of the forcefield */
inline void ForceField::setName(const QString &name)
{
    d().setName(name);
}

/** Return the total energy of the forcefield */
inline double ForceField::energy()
{
    return d().energy();
}

/** Return the energy of the component represented by the
    function 'component'

    \throw SireFF::missing_component
*/
inline double ForceField::energy(const FFComponent &component)
{
    return d().energy(component);
}

/** Return the energies of all of the specified components

    \throw SireFF::missing_component
*/
inline Values ForceField::energies(const QSet<FFComponent> &components)
{
    return d().energies(components);
}

/** Return the energies of all of the components */
inline Values ForceField::energies()
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
inline bool ForceField::change(const Molecule &mol)
{
    return d().change(mol);
}

/** Change the residue 'res' (e.g. move it, or change its
    parameters). This does nothing if the residue is not
    in this forcefield. Returns whether or not the forcefield
    has been changed by this change, and thus whether the
    energy needs to be recalculated.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
inline bool ForceField::change(const Residue &res)
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
inline bool ForceField::change(const NewAtom &atom)
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
inline bool ForceField::add(const Molecule &molecule,
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
inline bool ForceField::add(const Residue &residue,
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
inline bool ForceField::add(const NewAtom &atom,
                            const ParameterMap &map)
{
    return d().add(atom,map);
}

/** Remove the molecule 'molecule' from this forcefield - this
    does nothing if the molecule is not in this forcefield. This
    returns whether this has changed the forcefield (therefore
    necessitating a recalculation of the energy) */
inline bool ForceField::remove(const Molecule &molecule)
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
inline bool ForceField::remove(const Residue &residue)
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
inline bool ForceField::remove(const NewAtom &atom)
{
    return d().remove(atom);
}

/** Replace the molecule 'oldmol' with 'newmol' (using
    the passed parameter map to find any required parameters
    in the properties of the molecule). This is equivalent
    to 'remove(oldmol)' followed by 'add(newmol,map)', except
    that 'newmol' will only be added if 'oldmol' is contained
    in this forcefield.

    This returns whether this changes the forcefield.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
inline bool ForceField::replace(const Molecule &oldmol,
                                const Molecule &newmol,
                                const ParameterMap &map)
{
    return d().replace(oldmol, newmol, map);
}

/** Return whether or not this forcefield contains *any part*
    of any version of the molecule 'molecule' */
inline bool ForceField::refersTo(const Molecule &molecule) const
{
    return d().refersTo(molecule);
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
     in this forcefield) */
inline QSet<MoleculeID> ForceField::moleculeIDs() const
{
    return d().moleculeIDs();
}

/** Return whether this forcefield contains a complete copy of 
    any version of the molecule 'molecule' */
inline bool ForceField::contains(const Molecule &molecule) const
{
    return d().contains(molecule);
}

/** Return whether this forcefield contains a complete copy 
    of any version of the residue 'residue' */
inline bool ForceField::contains(const Residue &residue) const
{
    return d().contains(residue);
}

/** Return whether or not this forcefield contains
    any version of the atom 'atom' */
inline bool ForceField::contains(const NewAtom &atom) const
{
    return d().contains(atom);
}

/** Return whether the forcefield is dirty (requires an energy recalcualtion) */
inline bool ForceField::isDirty() const
{
    return d().isDirty();
}

/** Return whether or not the forcefield is clean (does not require
    an energy recalcualtion) */
inline bool ForceField::isClean() const
{
    return d().isClean();
}

/** Return the ID number of the forcefield */
inline ForceFieldID ForceField::ID() const
{
    return d().ID();
}

/** Return the version number of the forcefield */
inline const Version& ForceField::version() const
{
    return d().version();
}

/** Assert that this forcefield contains the component 'component'

    \throw SireFF::missing_component
*/
inline void ForceField::assertContains(const FFComponent &component) const
{
    d().assertContains(component);
}

}

Q_DECLARE_METATYPE(SireFF::ForceField);

SIRE_END_HEADER

#endif
