#ifndef SIREMM_ATOMICCHARGES_H
#define SIREMM_ATOMICCHARGES_H

#include <QVector>

#include "SireMol/property.h"

#include "chargeparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class AtomicCharges;
}

QDataStream& operator<<(QDataStream&, const SireMM::AtomicCharges&);
QDataStream& operator>>(QDataStream&, SireMM::AtomicCharges&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::Property;

/** This class is used to hold all of the atomic charges for the
    atoms in a molecule. All of the atomic charges are held in
    groups that correspond to the CutGroups in the molecule.

    This is really just a thin-wrapper around
    QVector< QVector<ChargeParameter> >

    @author Christopher Woods
*/
class SIREMM_EXPORT AtomicCharges : public SireMol::PropertyBase,
                                    public QVector< QVector<ChargeParameter> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicCharges&);
friend QDataStream& ::operator>>(QDataStream&, AtomicCharges&);

public:
    AtomicCharges();

    AtomicCharges(const QVector< QVector<ChargeParameter> > &charges);

    AtomicCharges(const QVector<ChargeParameter> &charges);

    AtomicCharges(const Property &property);

    AtomicCharges(const AtomicCharges &other);

    ~AtomicCharges();

    AtomicCharges& operator=(const QVector< QVector<ChargeParameter> > &charges);
    AtomicCharges& operator=(const QVector<ChargeParameter> &charges);
    AtomicCharges& operator=(const Property &property);

    AtomicCharges& operator=(const AtomicCharges &other);

    static const char* typeName()
    {
        return "SireMM::AtomicCharges";
    }

    const char* what() const
    {
        return AtomicCharges::typeName();
    }

    AtomicCharges* clone() const
    {
        return new AtomicCharges(*this);
    }

    bool isCompatibleWith(const Molecule &molecule) const;
};

}

Q_DECLARE_METATYPE(SireMM::AtomicCharges);

SIRE_END_HEADER

#endif
