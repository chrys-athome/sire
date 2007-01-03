#ifndef SIREFF_FFCOMPONENT_H
#define SIREFF_FFCOMPONENT_H

#include "SireCAS/function.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFComponent;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFComponent&);
QDataStream& operator>>(QDataStream&, SireFF::FFComponent&);

namespace SireFF
{

class FFBase;

using SireCAS::Function;
using SireCAS::Symbol;
using SireCAS::Symbols;

/** This is a specialisation of a SireCAS::Function that is used
    to represent a component of a forcefield. This controls the
    name of the function, allowing the encoding and decoding
    of the forcefield name, component name and forcefield ID
    number into the function's name.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFComponent : public Function
{

friend QDataStream& ::operator<<(QDataStream&, const FFComponent&);
friend QDataStream& ::operator>>(QDataStream&, FFComponent&);

public:
    FFComponent();

    FFComponent(const FFBase &ffield);

    FFComponent(const FFBase &ffield, const Symbols &symbols);

    FFComponent(const FFBase &ffield,
                const Symbol &sym0);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8);
    FFComponent(const FFBase &ffield,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                const Symbol &sym9);

    FFComponent(const FFBase &ffield,
                const QString &component_name);

    FFComponent(const FFBase &ffield,
                const QString &component_name, const Symbols &symbols);

    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8);
    FFComponent(const FFBase &ffield, const QString &component_name,
                const Symbol &sym0, const Symbol &sym1, const Symbol &sym2,
                const Symbol &sym3, const Symbol &sym4, const Symbol &sym5,
                const Symbol &sym6, const Symbol &sym7, const Symbol &sym8,
                const Symbol &sym9);

    FFComponent(const SireCAS::Function &function);

    FFComponent(const FFComponent &other);

    ~FFComponent();

    FFComponent& operator=(const FFComponent &other);
    FFComponent& operator=(const SireCAS::Function &function);

    QString forcefieldName() const;
    QString forcefieldComponent() const;

    quint32 forcefieldID() const;

private:
    /** The ID number of the forcefield */
    quint32 ffid;
};

}

Q_DECLARE_METATYPE(SireFF::FFComponent);

SIRE_END_HEADER

#endif
