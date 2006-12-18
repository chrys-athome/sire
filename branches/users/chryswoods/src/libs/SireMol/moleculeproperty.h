#ifndef SIREMOL_MOLECULEPROPERTY_H
#define SIREMOL_MOLECULEPROPERTY_H

#include <QVariant>

#include "property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculePropertyData;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculePropertyData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculePropertyData&);

namespace SireMol
{

/** This is a molecule property - this is just a QVariant, so that 
    any object can be associated with the molecule.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculePropertyData : public PropertyData, public QVariant
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculePropertyData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculePropertyData&);

public:
    MoleculePropertyData();
    
    MoleculePropertyData(const QVariant &value);
    
    MoleculePropertyData(const MoleculePropertyData &other);
    
    ~MoleculePropertyData();
    
    MoleculePropertyData* clone() const
    {
        return new MoleculePropertyData(*this);
    }
    
    static const char* typeName()
    {
        return "SireMol::MoleculePropertyData";
    }
    
    const char* what() const
    {
        return MoleculePropertyData::typeName();
    }
    
    void assertCompatibleWith(const Molecule &molecule) const;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculePropertyData);

SIRE_END_HEADER

#endif
