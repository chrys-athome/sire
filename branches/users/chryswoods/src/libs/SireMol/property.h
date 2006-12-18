#ifndef SIREMOL_PROPERTY_H
#define SIREMOL_PROPERTY_H

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class PropertyData;
}

QDataStream& operator<<(QDataStream&, const SireMol::PropertyData&);
QDataStream& operator>>(QDataStream&, SireMol::PropertyData&);

namespace SireMol
{

class Property;
class Molecule;

/** This is the base class of all properties that may be attached to a 
    molecule. Properties are used to assign extra information to a molecule,
    which may then be carried by the molecule throughout its passage
    through the simulation. Examples of properties may include the file
    from which the molecule was read, the charge parameters on the atoms,
    the PDB code etc.
    
    Properties form a polymorphic hierarchy which are implicitly shared
    via SireBase::SharedPolyPointer smart pointers.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT PropertyData : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const PropertyData&);
friend QDataStream& ::operator>>(QDataStream&, PropertyData&);

public:
    PropertyData();
    
    PropertyData(const PropertyData &other);
    
    virtual ~PropertyData();
    
    virtual PropertyData* clone() const=0;
    
    virtual const char* what() const=0;
    
    static Property null_property();
    
    virtual void assertCompatibleWith(const Molecule &molecule) const=0;
};

/** This is the visible holder class for PropertyBase. This is just 
    a thin wrapper around SireBase::SharedPolyPointer<PropertyBase>
    that has extra tests that ensure that this is never null. 
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT Property : public SireBase::SharedPolyPointer<PropertyData>
{
public:
    Property();
    
    Property(const SireBase::SharedPolyPointer<PropertyData> &ptr);
    
    Property(const PropertyData &property);
    Property(PropertyData *property);
    
    Property(const Property &other);
    
    ~Property();
};

}

Q_DECLARE_METATYPE(SireMol::Property);

SIRE_END_HEADER

#endif
