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

    bool change(const Molecule &mol);
    bool change(const Residue &res);

    bool add(const Molecule &molecule,
             const ParameterMap &map = ParameterMap());
    bool add(const Residue &residue,
             const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);

    bool replace(const Molecule &oldmol,
                 const Molecule &newmol,
                 const ParameterMap &map = ParameterMap());

    bool contains(const Molecule &molecule) const;
    bool contains(const Residue &residue) const;

    Molecule molecule(MoleculeID molid) const;
    Residue residue(MoleculeID molid, ResNum resnum) const;

    Molecule molecule(const Molecule &mol) const;
    Residue residue(const Residue &res) const;

    bool isDirty() const;
    bool isClean() const;

    quint32 ID() const;
    const Version& version() const;

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

/** Add the molecule 'molecule' to this forcefield using
    the optional parameter map to find any necessary parameters
    from properties of the molecule. This will replace any
    existing copy of the molecule that already exists in
    this forcefield. This returns whether or not the
    forcefield has been changed by this addition, and therefore
    whether its energy needs recalculating.

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
inline bool ForceField::add(const Residue &residue,
                            const ParameterMap &map)
{
    return d().add(residue,map);
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

/** Return whether this forcefield contains a copy of the molecule
    'molecule' */
inline bool ForceField::contains(const Molecule &molecule) const
{
    return d().contains(molecule);
}

/** Return whether this forcefield contains a copy of the
    residue 'residue' */
inline bool ForceField::contains(const Residue &residue) const
{
    return d().contains(residue);
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
inline quint32 ForceField::ID() const
{
    return d().ID();
}

/** Return the version number of the forcefield */
inline const Version& ForceField::version() const
{
    return d().version();
}

}

Q_DECLARE_METATYPE(SireFF::ForceField);

SIRE_END_HEADER

#endif
