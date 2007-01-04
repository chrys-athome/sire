
#include "atomicljs.h"

#include "SireBase/sharedpolypointer_cast.hpp"

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/cgatomid.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<AtomicLJs> r_atomljs;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AtomicLJs &atomljs)
{
    writeHeader(ds, r_atomljs, 1)
            << static_cast<const AtomicProperties&>(atomljs)
            << static_cast<const QVector< QVector<LJParameter> >&>(atomljs);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AtomicLJs &atomljs)
{
    VersionID v = readHeader(ds, r_atomljs);

    if (v == 1)
    {
        ds >> static_cast<AtomicProperties&>(atomljs)
           >> static_cast<QVector< QVector<LJParameter> >&>(atomljs);
    }
    else
        throw version_error(v, "1", r_atomljs, CODELOC);

    return ds;
}

/** Null constructor */
AtomicLJs::AtomicLJs()
          : AtomicProperties(), QVector< QVector<LJParameter> >()
{}

/** Construct LJ parameters that are copied from 'ljs' */
AtomicLJs::AtomicLJs(const QVector< QVector<LJParameter> > &ljs)
          : AtomicProperties(), QVector< QVector<LJParameter> >(ljs)
{}

/** Construct LJ parameters that are copied from 'ljs' (single CutGroup) */
AtomicLJs::AtomicLJs(const QVector<LJParameter> &ljs)
          : AtomicProperties(), QVector< QVector<LJParameter> >(1, ljs)
{}

/** Copy from a Property

    \throw SireError::invalid_cast
*/
AtomicLJs::AtomicLJs(const Property &property)
          : AtomicProperties(), QVector< QVector<LJParameter> >()
{
    *this = property;
}

/** Copy constructor */
AtomicLJs::AtomicLJs(const AtomicLJs &other)
          : AtomicProperties(other), QVector< QVector<LJParameter> >(other)
{}

/** Destructor */
AtomicLJs::~AtomicLJs()
{}

/** Assignment operator */
AtomicLJs& AtomicLJs::operator=(const QVector< QVector<LJParameter> > &ljparams)
{
    QVector< QVector<LJParameter> >::operator=(ljparams);

    return *this;
}

/** Assignment operator */
AtomicLJs& AtomicLJs::operator=(const QVector<LJParameter> &ljparams)
{
    QVector< QVector<LJParameter> >::operator=(
                  QVector< QVector<LJParameter> >(1, ljparams) );

    return *this;
}

/** Assignment operator */
AtomicLJs& AtomicLJs::operator=(const AtomicLJs &other)
{
    QVector< QVector<LJParameter> >::operator=(other);
    AtomicProperties::operator=(other);

    return *this;
}

/** Assignment operator

    \throw SireError::invalid_cast
*/
AtomicLJs& AtomicLJs::operator=(const Property &property)
{
    return AtomicLJs::operator=(sharedpolypointer_cast<AtomicLJs>(property));
}

/** Return whether or not this is compatible with 'molecule' */
bool AtomicLJs::isCompatibleWith(const Molecule &molecule) const
{
    const MoleculeInfo &molinfo = molecule.info();

    int ncg = molinfo.nCutGroups();

    if (this->count() != ncg)
        return false;
    else
    {
        const QVector<LJParameter> *params = this->constData();

        for (int i=0; i<ncg; ++i)
        {
            if ( params[i].count() != molinfo.nAtoms(CutGroupID(i)) )
                return false;
        }
    }

    return true;
}

/** Return the LJ parameter as a QVariant for the atom at index 'CGAtomID'

    \throw SireError::invalid_index
*/
QVariant AtomicLJs::value(const CGAtomID &cgatomid) const
{
    if (cgatomid.cutGroupID() >= this->count())
        throwMissingCutGroup(cgatomid.cutGroupID(), this->count());

    const QVector<LJParameter> &ljs =
                            this->constData()[cgatomid.cutGroupID()];

    if (cgatomid.atomID() >= ljs.count())
        throwMissingAtom(cgatomid, ljs.count());

    return QVariant::fromValue( ljs.constData()[cgatomid.atomID()] );
}
