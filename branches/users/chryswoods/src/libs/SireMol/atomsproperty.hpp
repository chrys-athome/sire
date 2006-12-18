#ifndef SIREMOL_ATOMSPROPERTY_HPP
#define SIREMOL_ATOMSPROPERTY_HPP

#include "property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class T>
class AtomsPropertyData;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMol::AtomsPropertyData<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMol::AtomsPropertyData<T>&);

namespace SireMol
{

/** This simple class provides some decoration for QVector< QVector<T> > 
    which allows it to act as a store for the properties of the 
    atoms in a molecule.
    
    @author Christopher Woods
*/
template<class T>
class AtomsPropertyData : public PropertyData, public QVector< QVector<T> >
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomsPropertyData<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomsPropertyData<T>&);

public:
    AtomsPropertyData();
    
    AtomsPropertyData(const QVector< QVector<T> > &values);
    
    AtomsPropertyData(const AtomsPropertyData<T> &other);
    
    void assertCompatibleWith(const Molecule &molecule) const;
    
    static const char* typeName()
    {
        return "SireMol::AtomsPropertyData<T>";
    }
    
    const char* what() const
    {
        return AtomsPropertyData<T>::typeName();
    }
    
    AtomsPropertyData<T>* clone() const
    {
        return new AtomsPropertyData<T>(*this);
    }
};

/** Null constructor */
template<class T>
AtomsPropertyData<T>::AtomsPropertyData()
                     : PropertyData(), QVector< QVector<T> >()
{}

/** Construct from the passed values */
template<class T>
AtomsPropertyData<T>::AtomsPropertyData(const QVector< QVector<T> > &values)
                     : PropertyData(), QVector< QVector<T> >(values)
{}

/** Copy constructor */
template<class T>
AtomsPropertyData<T>::AtomsPropertyData(const AtomsPropertyData<T> &other)
                     : PropertyData(other), QVector< QVector<T> >(other)
{}

/** Assert that this is compatible with the molecule 'molecule' 

    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void AtomsPropertyData<T>::assertCompatibleWith(const Molecule &molecule) const
{
    int ng = this->count();
    
    throw "need to write this!";
}

/** This simple class is used to provide a holder for AtomsPropertyData,
    and to allow automatic conversion from QVector< QVector<T> > into
    AtomsPropertyData<T>. This allows functions to be written as;
    
    void setAtomsProperty( const QString &property, const AtomsProperty &atomprops );
    
    but will accept QVector< QVector<T> > objects being passed.
    
    @author Christopher Woods
*/
class AtomsProperty : public Property
{
public:
    /** Null constructor */
    AtomsProperty() : Property()
    {}
    
    /** Construct from passed AtomsPropertyData */
    template<class T>
    AtomsProperty(const AtomsPropertyData<T> &atomdata)
                     : Property(atomdata)
    {}
    
    /** Construct from a pointer to the passed AtomsPropertyData - 
        this pointer must not be null, and this object will take
        over ownership of the pointer
        
        \throw SireError::nullptr_error
    */
    template<class T>
    AtomsProperty(AtomsPropertyData<T> *ptr)
                     : Property( ptr )
    {}
    
    /** Construct from a shared pointer to the passed AtomsPropertyData
        - this must not be null!
        
        \throw SireError::nullptr_error
    */
    template<class T>
    AtomsProperty(const SireBase::SharedPolyPointer<T> &ptr)
                     : Property( ptr )
    {}
    
    /** Construct from the passed vector of Atom properties */
    template<class T>
    AtomsProperty(const QVector< QVector<T> > &values)
                     : Property( new AtomsPropertyData<T>(values) )
    {}
    
    /** Copy constructor */
    AtomsProperty(const AtomsProperty &other) : Property(other)
    {}
    
    /** Destructor */
    ~AtomsProperty()
    {}
};

}

SIRE_END_HEADER

#endif
