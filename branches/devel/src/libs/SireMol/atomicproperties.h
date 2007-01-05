#ifndef SIREMOL_ATOMICPROPERTIES_H
#define SIREMOL_ATOMICPROPERTIES_H

#include "property.h"

SIRE_BEGIN_HEADER

class QVariant;

namespace SireMol
{

class CutGroupID;
class CGAtomID;

/** This class provides the interface which must be supplied
    by all properties that claim to be AtomicProperties
    (i.e. have one value for each atom in the molecule)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomicProperties : public PropertyBase
{
public:
    AtomicProperties();
    AtomicProperties(const AtomicProperties &other);

    ~AtomicProperties();

    static const char* typeName()
    {
        return "SireMol::AtomicProperties";
    }

    virtual QVariant value(const CGAtomID &cgatomid) const=0;

protected:
    void throwMissingCutGroup(CutGroupID cgid, int ncg) const;
    void throwMissingAtom(const CGAtomID &cgatomid, int nats) const;
};

}

SIRE_END_HEADER

#endif
