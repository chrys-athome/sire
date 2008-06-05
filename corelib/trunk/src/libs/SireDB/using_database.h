#ifndef SIREDB_USING_DATABASE_H
#define SIREDB_USING_DATABASE_H

#include "assigninstruction.h"

#include <QStringList>

SIRE_BEGIN_HEADER

namespace SireDB
{
class using_database;
}

QDataStream& operator<<(QDataStream&, const SireDB::using_database&);
QDataStream& operator>>(QDataStream&, SireDB::using_database&);

namespace SireDB
{

/** This is the virtual base class of using_relationships and using_parameters.
    This class cannot be used on its own, but rather abstracts the 
    utilities of the two derived classes, and provides an interface for 
    the other assign classes.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT using_database : public AssignInstruction, public QStringList
{
public:
    /** Constructor */
    using_database() : AssignInstruction(), QStringList()
    {}
    
    /** Construct from a list of typenames */
    using_database(const QStringList &typenames) 
                    : AssignInstruction(), QStringList(typenames)
    {}
    
    /** Copy constructor */
    using_database(const using_database &other) 
                    : AssignInstruction(other), QStringList(other)
    {}
    
    /** Destructor */
    virtual ~using_database()
    {}

private:    
    /** Add a type onto the list */
    void addType(const char *type_name)
    {
        if (type_name)
            this->append(type_name);
    }
};

namespace detail
{
struct unspecified
{
    static const char* typeName()
    {
        return 0;
    }
};
}

}

SIRE_END_HEADER

#endif
