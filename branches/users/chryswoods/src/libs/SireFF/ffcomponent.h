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

/** This is a specialisation of a SireCAS::Function that is used
    to represent a component of a forcefield. This controls the 
    name of the function, allowing the encoding and decoding
    of the forcefield name, component name and forcefield ID
    number into the function's name.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFComponent : public SireCAS::Function
{

friend QDataStream& ::operator<<(QDataStream&, const FFComponent&);
friend QDataStream& ::operator>>(QDataStream&, FFComponent&);

public:
    FFComponent();
    
    FFComponent(const FFBase &ffield);
    
    FFComponent(const FFBase &ffield,
                const QString &component_name);

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
