#ifndef SIREMM_CLJPARAMETER_H
#define SIREMM_CLJPARAMETER_H

#include "chargeparameter.h"
#include "ljparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJParameter;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJParameter&);
QDataStream& operator>>(QDataStream&, SireMM::CLJParameter&);

namespace SireMM
{

/**
A CLJParameter holds charge and Lennard Jones (sigma and epsilon) parameters. 
 
@author Christopher Woods
*/
class SIREMM_EXPORT CLJParameter : public ChargeParameter, public LJParameter
{
public:
    CLJParameter();
    CLJParameter(const ChargeParameter &charge, const LJParameter &lj);
    CLJParameter(const ChargeParameter &charge, double sigma, double epsilon);
    
    CLJParameter(const CLJParameter &other);
    
    ~CLJParameter();
    
    bool operator==(const CLJParameter &other) const;
    bool operator!=(const CLJParameter &other) const;
    
    QString toString() const;
    
    bool isDummy() const;
    
    static CLJParameter dummy();
};

/** Is this a dummy parameter? */
inline bool CLJParameter::isDummy() const
{
    return ChargeParameter::isDummy() and LJParameter::isDummy();
}

}

Q_DECLARE_TYPEINFO(SireMM::CLJParameter, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMM::CLJParameter)

SIRE_END_HEADER

#endif
