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
    double energy(const Function &component);

    Values energies();

    bool move(const Molecule &mol);
    bool move(const Residue &res);

    bool isDirty() const;
    bool isClean() const;

    template<class T>
    bool isA() const;

    template<class T>
    SireBase::SharedPolyPointer<T> asA() const;

private:
    /** Shared pointer to the actual forcefield implementation */
    SireBase::SharedPolyPointer<FFBase> d;
};

/** Return a string containing the type of the forcefield */
inline const char* ForceField::what() const
{
    return d->what();
}

/** Return the components in this forcefield */
inline const FFBase::Components& ForceField::components() const
{
    return d->components();
}

/** Return the parameters necessary for this forcefield */
inline const FFBase::Parameters& ForceField::parameters() const
{
    return d->parameters();
}

/** Return the name of the forcefield */
inline const QString& ForceField::name() const
{
    return d->name();
}

/** Set the name of the forcefield */
inline void ForceField::setName(const QString &name)
{
    d->setName(name);
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

/** Return whether or not this forcefield is of type 'T' */
template<class T>
SIRE_INLINE_TEMPLATE
bool ForceField::isA() const
{
    return d.isA<T>();
}

/** Return a copy of the forcefield cast as type T. This
    will throw an exception if this forcefield is not derivable
    from type T.

    \throw SireError::invalid_cast
*/
template<class T>
SIRE_INLINE_TEMPLATE
SireBase::SharedPolyPointer<T> ForceField::asA() const
{
    return SireBase::SharedPolyPointer<T>( d );
}

}

SIRE_END_HEADER

#endif
