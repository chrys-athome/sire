#ifndef SIREMM_CHARGEPARAMETER_H
#define SIREMM_CHARGEPARAMETER_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ChargeParameter;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::ChargeParameter&);
QDataStream& operator>>(QDataStream&, SireMM::ChargeParameter&);

namespace SireMM
{

/**
A ChargeParameter holds the partial charge parameter

@author Christopher Woods
*/
class SIREMM_EXPORT ChargeParameter
{
public:
    ChargeParameter();
    ChargeParameter(double charge);
    
    ChargeParameter(const ChargeParameter &other);
    
    ~ChargeParameter();
    
    ChargeParameter& operator=(const ChargeParameter &other);
    
    bool isDummy() const;
    bool zeroCharge() const;
    
    double charge() const;
    
    QString toString() const;
    
    bool operator==(const ChargeParameter &other) const;
    bool operator!=(const ChargeParameter &other) const;
    
    static ChargeParameter dummy();

private:
    /** The actual chage, in |e| */
    double chg;
};

/** Return whether or not this is a dummy charge (zero charge) */
inline bool ChargeParameter::isDummy() const
{
    return chg == 0;
}

/** Return whether or not this is a zero charge */
inline bool ChargeParameter::zeroCharge() const
{
    return chg == 0;
}

/** Return the actual charge */
inline double ChargeParameter::charge() const
{
    return chg;
}

}

Q_DECLARE_METATYPE(SireMM::ChargeParameter)
Q_DECLARE_TYPEINFO(SireMM::ChargeParameter, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
