#ifndef SIRECAS_INTEGRATIONCONSTANT_H
#define SIRECAS_INTEGRATIONCONSTANT_H

#include "symbol.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class IntegrationConstant;
}

QDataStream& operator<<(QDataStream&, const SireCAS::IntegrationConstant&);
QDataStream& operator>>(QDataStream&, SireCAS::IntegrationConstant&);

namespace SireCAS
{

/**
This class represents a constant of integration. This is not a number or function in the normal sense, but rather a placeholder that is created during indefinite integration.

@author Christopher Woods
*/
class SIRECAS_EXPORT IntegrationConstant : public Symbol
{

friend QDataStream& ::operator<<(QDataStream&, const IntegrationConstant&);
friend QDataStream& ::operator>>(QDataStream&, IntegrationConstant&);

public:
    IntegrationConstant();
    
    IntegrationConstant(const IntegrationConstant &other);
    
    ~IntegrationConstant();

    bool operator==(const ExBase &other) const;
    
    uint hash() const;

    const char* what() const
    {
        return "SireCAS::IntegrationConstant";
    }

    Expression integrate(const Symbol &symbol) const;

protected:
    ExBase* clone() const
    {
        return new IntegrationConstant(*this);
    }

};

}

Q_DECLARE_METATYPE(SireCAS::IntegrationConstant)

SIRE_END_HEADER

#endif
