
#include "atomicljs.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/cutgroupid.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomicLJs> r_atomljs("SireMM::AtomicLJs");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AtomicLJs &atomljs)
{
    writeHeader(ds, r_atomljs, 1)
            << static_cast<const PropertyBase&>(atomljs)
            << static_cast<const QVector< QVector<LJParameter> >&>(atomljs);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AtomicLJs &atomljs)
{
    VersionID v = readHeader(ds, r_atomljs);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(atomljs)
           >> static_cast<QVector< QVector<LJParameter> >&>(atomljs);
    }
    else
        throw version_error(v, "1", r_atomljs, CODELOC);

    return ds;
}

/** Null constructor */
AtomicLJs::AtomicLJs()
          : PropertyBase(), QVector< QVector<LJParameter> >()
{}

/** Construct LJ parameters that are copied from 'ljs' */
AtomicLJs::AtomicLJs(const QVector< QVector<LJParameter> > &ljs)
          : PropertyBase(), QVector< QVector<LJParameter> >(ljs)
{}

/** Construct LJ parameters that are copied from 'ljs' (single CutGroup) */
AtomicLJs::AtomicLJs(const QVector<LJParameter> &ljs)
          : PropertyBase(), QVector< QVector<LJParameter> >(1, ljs)
{}

/** Copy constructor */
AtomicLJs::AtomicLJs(const AtomicLJs &other)
          : PropertyBase(other), QVector< QVector<LJParameter> >(other)
{}

/** Destructor */
AtomicLJs::~AtomicLJs()
{}

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
