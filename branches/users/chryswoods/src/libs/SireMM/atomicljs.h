#ifndef SIREMM_ATOMICLJS_H
#define SIREMM_ATOMICLJS_H

#include <QVector>

#include "SireMol/property.h"

#include "ljparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class AtomicLJs;
}

QDataStream& operator<<(QDataStream&, const SireMM::AtomicLJs&);
QDataStream& operator>>(QDataStream&, SireMM::AtomicLJs&);

namespace SireMM
{

using SireMol::Molecule;

/** This class is used to hold all of the atomic charges for the
    atoms in a molecule. All of the atomic charges are held in
    groups that correspond to the CutGroups in the molecule.

    This is really just a thin-wrapper around
    QVector< QVector<LJParameter> >

    @author Christopher Woods
*/
class SIREMM_EXPORT AtomicLJs : public SireMol::PropertyBase,
                                public QVector< QVector<LJParameter> >
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicLJs&);
friend QDataStream& ::operator>>(QDataStream&, AtomicLJs&);

public:
    AtomicLJs();

    AtomicLJs(const QVector< QVector<LJParameter> > &charges);

    AtomicLJs(const QVector<LJParameter> &charges);

    AtomicLJs(const AtomicLJs &other);

    ~AtomicLJs();

    static const char* typeName()
    {
        return "SireMM::AtomicLJs";
    }

    const char* what() const
    {
        return AtomicLJs::typeName();
    }

    AtomicLJs* clone() const
    {
        return new AtomicLJs(*this);
    }

    bool isCompatibleWith(const Molecule &molecule) const;
};

}

Q_DECLARE_METATYPE(SireMM::AtomicLJs);

SIRE_END_HEADER

#endif
