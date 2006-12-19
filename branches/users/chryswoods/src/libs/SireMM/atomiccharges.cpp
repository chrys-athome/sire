
#include "atomiccharges.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/cutgroupid.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomicCharges> r_atomchgs("SireMM::AtomicCharges");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AtomicCharges &atomchgs)
{
    writeHeader(ds, r_atomchgs, 1)
            << static_cast<const PropertyBase&>(atomchgs)
            << static_cast<const QVector< QVector<ChargeParameter> >&>(atomchgs);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AtomicCharges &atomchgs)
{
    VersionID v = readHeader(ds, r_atomchgs);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(atomchgs)
           >> static_cast<QVector< QVector<ChargeParameter> >&>(atomchgs);
    }
    else
        throw version_error(v, "1", r_atomchgs, CODELOC);

    return ds;
}

/** Null constructor */
AtomicCharges::AtomicCharges()
              : PropertyBase(), QVector< QVector<ChargeParameter> >()
{}

/** Construct charges that are copied from 'charges' */
AtomicCharges::AtomicCharges(const QVector< QVector<ChargeParameter> > &charges)
              : PropertyBase(), QVector< QVector<ChargeParameter> >(charges)
{}

/** Construct charges that are copied from 'charges' (single CutGroup) */
AtomicCharges::AtomicCharges(const QVector<ChargeParameter> &charges)
              : PropertyBase(), QVector< QVector<ChargeParameter> >(1, charges)
{}

/** Copy constructor */
AtomicCharges::AtomicCharges(const AtomicCharges &other)
              : PropertyBase(other), QVector< QVector<ChargeParameter> >(other)
{}

/** Destructor */
AtomicCharges::~AtomicCharges()
{}

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
