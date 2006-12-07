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
class SIREFF_EXPORT ForceField
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const ForceField &other);
    ForceField(const FFBase &ffbase);

    ~ForceField();

    ForceField& operator=(const ForceField &other);
    ForceField& operator=(const FFBase &ffbase);

    const char* what() const;

    const QString& name() const;

    double energy();
    double energy(const Function &component);

    Values energies();

    static int TOTAL()
    {
        return FFBase::TOTAL();
    }

    const Function& total() const;
    const Function& component(int componentid) const;
    QList<Function> components() const;

    const Molecule& molecule(MoleculeID molid) const;
    //virtual const Residue& residue(const MolResNumID &molresid) const=0;

    bool move(const Molecule &mol);
    bool move(const Residue &res);

    bool isDirty() const;
    bool isClean() const;

    void assertContains(const Function &component) const;

    void assertContains(const Molecule &molecule) const;
    void assertContains(const Residue &residue) const;

    void assertSameMajorVersion(const Molecule &molecule) const;
    void assertSameMajorVersion(const Residue &residue) const;

    void assertSameVersion(const Molecule &molecule) const;
    void assertSameVersion(const Residue &residue) const;

private:
    /** Shared pointer to the actual forcefield implementation */
    SireBase::SharedPolyPointer<FFBase> d;
};

/** Return a string containing the type of the forcefield */
inline const char* ForceField::what() const
{
    return d->what();
}

/** Return the name of the forcefield */
inline const QString& ForceField::name() const
{
    return d->name();
}

/** Return the total energy of the forcefield */
inline double ForceField::energy()
{
    return d->energy();
}

/** Return the energy of the component represented by the
    function 'component'

    \throw SireFF::missing_component
*/
inline double ForceField::energy(const Function &component)
{
    return d->energy(component);
}

/** Return the energies of all of the components */
inline Values ForceField::energies()
{
    return d->energies();
}

/** Return the function representing the total energy of this forcefield */
inline const Function& ForceField::total() const
{
    return d->total();
}

/** Return the function representing the component with ID == componentid

    \throw SireFF::missing_component
*/
inline const Function& ForceField::component(int componentid) const
{
    return d->component(componentid);
}

/** Return a list of the functions representing all of the components
    in this forcefield */
inline QList<Function> ForceField::components() const
{
    return d->components();
}

/** Return the molecule in this forcefield with ID == molid

    \throw SireMol::missing_molecule
*/
inline const Molecule& ForceField::molecule(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Move the molecule 'mol' */
inline bool ForceField::move(const Molecule &mol)
{
    return d->move(mol);
}

/** Move the residue 'res' */
inline bool ForceField::move(const Residue &res)
{
    return d->move(res);
}

/** Return whether the forcefield is dirty (requires an energy recalcualtion) */
inline bool ForceField::isDirty() const
{
    return d->isDirty();
}

/** Return whether or not the forcefield is clean (does not require
    an energy recalcualtion) */
inline bool ForceField::isClean() const
{
    return d->isClean();
}

/** Assert that this forcefield contains a component represented by
    'component'

    \throw SireFF::missing_component
*/
inline void ForceField::assertContains(const Function &component) const
{
    d->assertContains(component);
}

/** Assert that this forcefield contains the molecule 'molecule'

    \throw SireMol::missing_molecule
*/
inline void ForceField::assertContains(const Molecule &molecule) const
{
    d->assertContains(molecule);
}

/** Assert that this forcefield contains the residue 'residue'

    \throw SireMol::missing_residue
*/
inline void ForceField::assertContains(const Residue &residue) const
{
    d->assertContains(residue);
}

/** Assert that the molecule 'molecule' has the same major version as
    the copy in this forcefield

    \throw SireMol::missing_molecule
    \throw SireError::version_error
*/
inline void ForceField::assertSameMajorVersion(const Molecule &molecule) const
{
    d->assertSameMajorVersion(molecule);
}

/** Assert that the residue 'residue' has the same major version as
    the copy in this forcefield

    \throw SireMol::missing_residue
    \throw SireError::version_error
*/
inline void ForceField::assertSameMajorVersion(const Residue &residue) const
{
    d->assertSameMajorVersion(residue);
}

/** Assert that the molecule 'molecule' has the same version as the
    one in this forcefield

    \throw SireMol::missing_molecule
    \throw SireError::version_error
*/
inline void ForceField::assertSameVersion(const Molecule &molecule) const
{
    d->assertSameVersion(molecule);
}

/** Assert that the residue 'residue' has the same version as the one
    in this forcefield

    \throw SireMol::missing_residue
    \throw SireError::version_error
*/
inline void ForceField::assertSameVersion(const Residue &residue) const
{
    d->assertSameVersion(residue);
}

}

SIRE_END_HEADER

#endif
