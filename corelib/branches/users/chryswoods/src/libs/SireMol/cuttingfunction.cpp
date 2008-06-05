/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "cuttingfunction.h"
#include "residuecutting.h"

#include "molecule.h"
#include "mover.hpp"
#include "moleditor.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

//////////////
////////////// Implementation of CuttingFunctionBase
//////////////

/** Constructor */
CuttingFunctionBase::CuttingFunctionBase()
                    : PropertyBase()
{}

/** Copy constructor */
CuttingFunctionBase::CuttingFunctionBase(const CuttingFunctionBase &other)
                    : PropertyBase(other)
{}

/** Destructor */
CuttingFunctionBase::~CuttingFunctionBase()
{}

/** Apply this function to a molecule */
Molecule CuttingFunctionBase::operator()(const Molecule &molecule) const
{
    MolStructureEditor moleditor( molecule );
    
    moleditor = this->operator()(moleditor);
    
    return moleditor.commit();
}

//////////////
////////////// Implementation of CuttingFunction
//////////////

RegisterMetaType<CuttingFunction> r_cutfunc;

/** Serialise a CuttingFunction to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const CuttingFunction &cutfunc)
{
    writeHeader(ds, r_cutfunc, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(cutfunc);
    
    return ds;
}

/** Deserialise a CuttingFunction from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       CuttingFunction &cutfunc)
{
    VersionID v = readHeader(ds, r_cutfunc);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(cutfunc);
    }
    else
        throw version_error(v, "1", r_cutfunc, CODELOC);
        
    return ds;
}

static CuttingFunction *_pvt_shared_null = 0;

const CuttingFunction& CuttingFunction::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new CuttingFunction( ResidueCutting() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, infinite cartesian volume */
CuttingFunction::CuttingFunction() : Property(CuttingFunction::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
CuttingFunction::CuttingFunction(const PropertyBase &property)
      : Property(property.asA<CuttingFunctionBase>())
{}

/** Construct from passed CuttingFunctionBase */
CuttingFunction::CuttingFunction(const CuttingFunctionBase &cutfunc)
      : Property(cutfunc)
{}

/** Copy constructor */
CuttingFunction::CuttingFunction(const CuttingFunction &other)
      : Property(other)
{}

/** Destructor */
CuttingFunction::~CuttingFunction()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
CuttingFunction& CuttingFunction::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<CuttingFunctionBase>());
    return *this;
}

/** Copy assignment operator from another CuttingFunctionBase */
CuttingFunction& CuttingFunction::operator=(const CuttingFunctionBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const CuttingFunctionBase* CuttingFunction::operator->() const
{
    return static_cast<const CuttingFunctionBase*>(&(d()));
}

/** Dereference this pointer */
const CuttingFunctionBase& CuttingFunction::operator*() const
{
    return static_cast<const CuttingFunctionBase&>(d());
}

/** Return a read-only reference to the contained object */
const CuttingFunctionBase& CuttingFunction::read() const
{
    return static_cast<const CuttingFunctionBase&>(d());
}

/** Use this cutting function to divide the passed molecule up into CutGroups */
Molecule CuttingFunction::operator()(const Molecule &molecule) const
{
    return this->read().operator()(molecule);
}

/** Use this cutting function to divide the passed molecule up into CutGroups */
MolStructureEditor CuttingFunction::operator()(MolStructureEditor &moleditor) const
{
    return this->read().operator()(moleditor);
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
CuttingFunctionBase& CuttingFunction::edit()
{
    return static_cast<CuttingFunctionBase&>(d());
}
    
/** Return a raw pointer to the CuttingFunctionBase object */
const CuttingFunctionBase* CuttingFunction::data() const
{
    return static_cast<const CuttingFunctionBase*>(&(d()));
}

/** Return a raw pointer to the CuttingFunctionBase object */
const CuttingFunctionBase* CuttingFunction::constData() const
{
    return static_cast<const CuttingFunctionBase*>(&(d()));
}
    
/** Return a raw pointer to the CuttingFunctionBase object */
CuttingFunctionBase* CuttingFunction::data()
{
    return static_cast<CuttingFunctionBase*>(&(d()));
}

/** Automatic casting operator */
CuttingFunction::operator const CuttingFunctionBase&() const
{
    return static_cast<const CuttingFunctionBase&>(d());
}
