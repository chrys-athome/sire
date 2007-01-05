
#include <QVariant>

#include "atomiccharges.h"

#include "SireBase/sharedpolypointer_cast.hpp"

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/cgatomid.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<AtomicCharges> r_atomchgs;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AtomicCharges &atomchgs)
{
    writeHeader(ds, r_atomchgs, 1)
            << static_cast<const AtomicProperties&>(atomchgs)
            << static_cast<const QVector< QVector<ChargeParameter> >&>(atomchgs);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AtomicCharges &atomchgs)
{
    VersionID v = readHeader(ds, r_atomchgs);

    if (v == 1)
    {
        ds >> static_cast<AtomicProperties&>(atomchgs)
           >> static_cast<QVector< QVector<ChargeParameter> >&>(atomchgs);
    }
    else
        throw version_error(v, "1", r_atomchgs, CODELOC);

    return ds;
}

/** Null constructor */
AtomicCharges::AtomicCharges()
              : AtomicProperties(), QVector< QVector<ChargeParameter> >()
{}

/** Construct charges that are copied from 'charges' */
AtomicCharges::AtomicCharges(const QVector< QVector<ChargeParameter> > &charges)
              : AtomicProperties(), QVector< QVector<ChargeParameter> >(charges)
{}

/** Construct charges that are copied from 'charges' (single CutGroup) */
AtomicCharges::AtomicCharges(const QVector<ChargeParameter> &charges)
              : AtomicProperties(), QVector< QVector<ChargeParameter> >(1, charges)
{}

/** Construct from a Property

    \throw SireError::invalid_cast
*/
AtomicCharges::AtomicCharges(const Property &property)
              : AtomicProperties(), QVector< QVector<ChargeParameter> >()
{
    *this = property;
}

/** Copy constructor */
AtomicCharges::AtomicCharges(const AtomicCharges &other)
              : AtomicProperties(other), QVector< QVector<ChargeParameter> >(other)
{}

/** Destructor */
AtomicCharges::~AtomicCharges()
{}

/** Assignment operator */
AtomicCharges& AtomicCharges::operator=(
                    const QVector< QVector<ChargeParameter> > &charges)
{
    QVector< QVector<ChargeParameter> >::operator=(charges);
    return *this;
}

/** Assignment operator */
AtomicCharges& AtomicCharges::operator=(const QVector<ChargeParameter> &charges)
{
    QVector< QVector<ChargeParameter> >::operator=(
                        QVector< QVector<ChargeParameter> >(1, charges) );

    return *this;
}

/** Assignment operator */
AtomicCharges& AtomicCharges::operator=(const AtomicCharges &other)
{
    QVector< QVector<ChargeParameter> >::operator=(other);
    PropertyBase::operator=(other);

    return *this;
}

/** Assignment operator

    \throw SireError::invalid_cast
*/
AtomicCharges& AtomicCharges::operator=(const Property &property)
{
    return AtomicCharges::operator=(sharedpolypointer_cast<AtomicCharges>(property));
}

/** Return whether or not this is compatible with 'molecule' */
bool AtomicCharges::isCompatibleWith(const Molecule &molecule) const
{
    const MoleculeInfo &molinfo = molecule.info();

    int ncg = molinfo.nCutGroups();

    if (this->count() != ncg)
        return false;
    else
    {
        const QVector<ChargeParameter> *params = this->constData();

        for (int i=0; i<ncg; ++i)
        {
            if ( params[i].count() != molinfo.nAtoms(CutGroupID(i)) )
                return false;
        }
    }

    return true;
}

/** Return the charge as a QVariant for the atom at index 'CGAtomID'

    \throw SireError::invalid_index
*/
QVariant AtomicCharges::value(const CGAtomID &cgatomid) const
{
    if (cgatomid.cutGroupID() >= this->count())
        throwMissingCutGroup(cgatomid.cutGroupID(), this->count());

    const QVector<ChargeParameter> &charges =
                            this->constData()[cgatomid.cutGroupID()];

    if (cgatomid.atomID() >= charges.count())
        throwMissingAtom(cgatomid, charges.count());

    return QVariant::fromValue( charges.constData()[cgatomid.atomID()] );
}
