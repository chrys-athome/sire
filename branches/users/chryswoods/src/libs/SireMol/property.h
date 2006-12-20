#ifndef SIREMOL_PROPERTY_H
#define SIREMOL_PROPERTY_H

#include "SireBase/sharedpolypointer.hpp"

#include <QVariant>

SIRE_BEGIN_HEADER

namespace SireMol
{
class PropertyBase;
class Property;
}

QDataStream& operator<<(QDataStream&, const SireMol::PropertyBase&);
QDataStream& operator>>(QDataStream&, SireMol::PropertyBase&);

QDataStream& operator<<(QDataStream&, const SireMol::Property&);
QDataStream& operator>>(QDataStream&, SireMol::Property&);

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
class SIREMOL_EXPORT PropertyBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const PropertyBase&);
friend QDataStream& ::operator>>(QDataStream&, PropertyBase&);

public:
    PropertyBase();

    PropertyBase(const PropertyBase &other);

    virtual ~PropertyBase();

    virtual PropertyBase* clone() const=0;

    virtual const char* what() const=0;

    static Property null_property();

    virtual bool isCompatibleWith(const Molecule &molecule) const=0;

    void assertCompatibleWith(const Molecule &molecule) const;
};

/** This is a simple property that holds any value as a QVariant. This
    is designed to be used for metadata that doesn't need any tight
    checking (e.g. the author of the molecule file, the source of
    the coordinates, the 'header' lines etc.)

    @author Christopher Woods
*/
class SIREMOL_EXPORT VariantProperty : public PropertyBase, public QVariant
{
public:
    VariantProperty();

    VariantProperty(const QVariant &value);

    VariantProperty(const VariantProperty &other);

    ~VariantProperty();

    static const char* typeName()
    {
        return "SireMol::VariantProperty";
    }

    const char* what() const
    {
        return VariantProperty::typeName();
    }

    VariantProperty* clone() const
    {
        return new VariantProperty(*this);
    }

    /** A variant property is compatible with everything! */
    bool isCompatibleWith(const Molecule&) const
    {
        return true;
    }
};

/** This is the visible holder class for PropertyBase. This is just
    a thin wrapper around SireBase::SharedPolyPointer<PropertyBase>
    that has extra tests that ensure that this is never null.

    @author Christopher Woods
*/
class SIREMOL_EXPORT Property : public SireBase::SharedPolyPointer<PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const Property&);
friend QDataStream& ::operator>>(QDataStream&, Property&);

public:
    Property();

    Property(const SireBase::SharedPolyPointer<PropertyBase> &ptr);

    Property(const PropertyBase &property);
    Property(PropertyBase *property);

    Property(const Property &other);

    ~Property();

    void assertCompatibleWith(const Molecule &molecule) const
    {
        constData()->assertCompatibleWith(molecule);
    }
};

}

Q_DECLARE_METATYPE(SireMol::Property);
Q_DECLARE_METATYPE(SireMol::VariantProperty);

SIRE_END_HEADER

#endif
