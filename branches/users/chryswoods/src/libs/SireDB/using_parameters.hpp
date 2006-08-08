#ifndef SIREDB_USINGDBS_HPP
#define SIREDB_USINGDBS_HPP

#include <QStringList>

#include "using_database.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class using_parameters_base;
}

QDataStream& operator<<(QDataStream&, const SireDB::using_parameters_base&);
QDataStream& operator>>(QDataStream&, SireDB::using_parameters_base&);

namespace SireDB
{

/** This is the base class of 'using_parameters' which is a utility
    class that allows for the easy specification of the database
    classes to use for parameter assignment.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT using_parameters_base : public using_database
{
public:
    /** Constructor */
    using_parameters_base() : using_database()
    {}
    
    /** Construct from a list of typenames */
    using_parameters_base(const QStringList &typenames) 
                    : using_database(typenames)
    {}
    
    /** Copy constructor */
    using_parameters_base(const using_parameters_base &other) 
                    : using_database(other)
    {}
    
    /** Destructor */
    ~using_parameters_base()
    {}
    
    using_parameters_base* clone() const
    {
        return new using_parameters_base(*this);
    }
    
    const char* what() const
    {
        return "SireDB::using_parameters_base";
    }
};

/** This class is used to specify which classes to use to 
    obtain paramters.
    
    You use this class by adding the databases to use onto
    the list of templates.
    
    @author Christopher Woods
*/
template<class T0, class T1=detail::unspecified, class T2=detail::unspecified, 
         class T3=detail::unspecified, class T4=detail::unspecified, 
         class T5=detail::unspecified, class T6=detail::unspecified,
         class T7=detail::unspecified, class T8=detail::unspecified, 
         class T9=detail::unspecified>
class SIREDB_EXPORT using_parameters : public using_parameters_base
{
public:
    /** Constructor */
    using_parameters() : using_parameters_base()
    {
        //add the typename of each class in turn...
        this->addType( T0::typeName() );
        this->addType( T1::typeName() );
        this->addType( T2::typeName() );
        this->addType( T3::typeName() );
        this->addType( T4::typeName() );
        this->addType( T5::typeName() );
        this->addType( T6::typeName() );
        this->addType( T7::typeName() );
        this->addType( T8::typeName() );
        this->addType( T9::typeName() );
    }

    /** Copy constructor */
    using_parameters(const using_parameters &other) : using_parameters_base(other)
    {}
    
    /** Destructor */
    ~using_parameters()
    {}
    
private:
    /** Add the name of the type to the list */
    void addType(const char *type_name)
    {
        if (type_name)
            this->append( type_name );
    }
};

}

Q_DECLARE_METATYPE(SireDB::using_parameters_base)

SIRE_END_HEADER

#endif
