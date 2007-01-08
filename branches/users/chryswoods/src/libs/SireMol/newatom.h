#ifndef SIREMOL_NEWATOM_H
#define SIREMOL_NEWATOM_H

#include <QVariant>
#include <QSharedDataPointer>

#include "idtypes.h"
#include "cgatomid.h"

#include "SireMaths/vector.h"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class NewAtom;
}

QDataStream& operator<<(QDataStream&, const SireMol::NewAtom&);
QDataStream& operator>>(QDataStream&, SireMol::NewAtom&);

uint qHash(const SireMol::NewAtom&);

namespace SireMaths
{
class Quaternion;
class Matrix;
}

namespace SireMol
{

class Molecule;
class MoleculeData;
class Residue;
class Element;
class AtomInfo;

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

/** This is a replacement for the Atom class that provides an
    Atom that is in context with its containing molecule.

    The main difference to the old Atom class is that this
    one contains a pointer to the MoleculeData of the
    Molecule that contains this Atom, and that therefore,
    the editing functions available are more limited than
    Atom (which can be heavily edited due to its
    use of multiple inheritance)

    This is a view on a MoleculeData in the same way that
    Residue and Molecule are views of MoleculeData

    @author Christopher Woods
*/
class SIREMOL_EXPORT NewAtom
{

friend class Molecule;
friend class Residue;

friend QDataStream& ::operator<<(QDataStream&, const NewAtom&);
friend QDataStream& ::operator>>(QDataStream&, NewAtom&);

public:
    NewAtom();
    NewAtom(const CGAtomID &cgatomid, const Molecule &molecule);
    NewAtom(const IDMolAtom &idmolatom, const Molecule &molecule);

    NewAtom(const QString &name, const Residue &residue);
    NewAtom(AtomID i, const Residue &residue);

    NewAtom(const NewAtom &other);

    ~NewAtom();

    NewAtom& operator=(const NewAtom &other);

    NewAtom& operator=(const Vector &newcoords);

    NewAtom& operator+=(const Vector &delta);
    NewAtom& operator-=(const Vector &delta);

    bool operator==(const NewAtom &other) const;
    bool operator!=(const NewAtom &other) const;

    QString name() const;

    const AtomInfo& info() const;
    Element element() const;
    Vector coordinates() const;

    const CGAtomID& cgAtomID() const;

    QVariant property(const QString &name) const;

    template<class T>
    T property(const QString &name) const;

    QString toString() const;

    Molecule molecule() const;
    Residue residue() const;

    QSet<NewAtom> bondedAtoms() const;
    QSet<Residue> bondedResidues() const;

    operator Vector() const;
    operator Element() const;
    operator const AtomInfo&() const;

    void setCoordinates(double x, double y, double z);
    void setCoordinates(const Vector &newcoords);

    void translate(const Vector &delta);
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    bool withinBondRadii(const NewAtom &other, double err=0) const;

private:
    /** Pointer to the MoleculeData containing the data for this atom */
    QSharedDataPointer<MoleculeData> d;

    /** The index of this atom in the molecule */
    CGAtomID cgatomid;
};

template<class T>
T NewAtom::property(const QString &name) const
{
    //get the property as a variant
    QVariant v = this->property(name);

    //can we cast this variant?
    if (not v.canConvert<T>())
        throw SireError::invalid_cast( QObject::tr(
                "Cannot cast a property of type \"%1\" to a property of type \"%2\"")
                    .arg(v.typeName()).arg(MetaType<T>::typeName()),
                        CODELOC );
}

}

Q_DECLARE_METATYPE(SireMol::NewAtom);

SIRE_END_HEADER

#endif
