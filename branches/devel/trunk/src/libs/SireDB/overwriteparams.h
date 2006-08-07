#ifndef SIREDB_OVERWRITEPARAMS_H
#define SIREDB_OVERWRITEPARAMS_H

#include <QString>
#include <QObject>

#include "assigninstruction.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This small class is used to set whether or not parameters should 
    be overwritten or not.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT OverWriteParams : public AssignInstruction
{
public:
    /** Default constructor - parameters will be overwritten */
    OverWriteParams(bool flag=true) : AssignInstruction(), _overwrite(flag)
    {}
    
    /** Copy constructor */
    OverWriteParams(const OverWriteParams &other)
              : AssignInstruction(other), _overwrite(other._overwrite)
    {}
    
    /** Destructor */
    ~OverWriteParams()
    {}
    
    /** Return a string representation of this flag */
    QString toString() const
    {
        if (_overwrite)
            return QObject::tr("overwrite == True");
        else
            return QObject::tr("overwrite == False");
    }
    
    /** Equality operator used to set the overwrite status, e.g.
        overwrite_parameters == false
        returns an OverWriteParams set to false. */
    OverWriteParams operator==(bool flag) const
    {
        return OverWriteParams(flag);
    }
    
    OverWriteParams* clone() const
    {
        return new OverWriteParams(*this);
    }
    
    const char* what() const
    {
        return "SireDB::OverWriteParams";
    }
    
    /** Evaluates to true if the parameters should be overwritten */
    operator bool() const
    {
        return _overwrite;
    }
    
    /** Return the overwrite flag */
    bool value() const
    {
        return _overwrite;
    }
    
private:
    /** Whether or not the parameters are overwritten */
    bool _overwrite;
};

/** Static instance of 'OverWriteParams' that allows for some nice 
    syntax :-) */
const OverWriteParams overwrite_parameters(true);

/** Static instance of 'OverWriteParams' that allows for some nice 
    syntax :-) */
const OverWriteParams overwrite(true);

}

SIRE_END_HEADER

#endif
