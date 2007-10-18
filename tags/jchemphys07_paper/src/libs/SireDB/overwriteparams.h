/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
