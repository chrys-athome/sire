/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "gto.h"

#include "SireMaths/maths.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireMaths;
using namespace SireBase;
using namespace SireID;
using namespace SireStream;

//////////
////////// Implementation of GTO
//////////

static const RegisterMetaType<GTO> r_gto( MAGIC_ONLY, GTO::typeName() );

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const GTO &gto)
{
    writeHeader(ds, r_gto, 1);
    
    ds << gto.alfa << gto.scl << static_cast<const OrbitalShell&>(gto);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, GTO &gto)
{
    VersionID v = readHeader(ds, r_gto);
    
    if (v == 1)
    {
        ds >> gto.alfa >> gto.scl >> static_cast<OrbitalShell&>(gto);
    }
    else
        throw version_error(v, "1", r_gto, CODELOC);
        
    return ds;
}

/** Constructor */
GTO::GTO() : OrbitalShell(), alfa(0), scl(0)
{}

/** Construct with the specified value of the alpha (exponent)
    and scale (unnormalised) */
GTO::GTO(double alpha, double scale) 
    : OrbitalShell(), alfa(alpha), scl(scale)
{}

/** Copy constructor */
GTO::GTO(const GTO &other) 
    : OrbitalShell(other), alfa(other.alfa), scl(other.scl)
{}

/** Destructor */
GTO::~GTO()
{}

/** Copy assignment operator */
GTO& GTO::operator=(const GTO &other)
{
    if (this != &other)
    {
        alfa = other.alfa;
        scl = other.scl;
        OrbitalShell::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool GTO::operator==(const GTO &other) const
{
    return alfa == other.alfa and scl == other.scl and
           OrbitalShell::operator==(other);
}

/** Comparison operator */
bool GTO::operator!=(const GTO &other) const
{
    return not this->operator==(other);
}

/** Return the value of alpha (the exponent) for this gaussian */
double GTO::alpha() const
{
    return alfa;
}

/** Convenient synonym for 'alpha()' - so you can write
    a.alpha() * b.beta() and have it mean what you expect */
double GTO::beta() const
{
    return GTO::alpha();
}

/** Return the unnormalised scaling factor for this gaussian */
double GTO::scale() const
{
    return scl;
}

const char* GTO::typeName()
{
    return "Squire::GTO";
}

//////////
////////// Implementation of CGTO
//////////

static const RegisterMetaType<CGTO> r_cgto( MAGIC_ONLY, CGTO::typeName() );

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const CGTO &cgto)
{
    writeHeader(ds, r_cgto, 1);
    
    SharedDataStream sds(ds);
    
    sds << cgto.alfas << cgto.scls << static_cast<const OrbitalShell&>(cgto);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, CGTO &cgto)
{
    VersionID v = readHeader(ds, r_cgto);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cgto.alfas >> cgto.scls >> static_cast<OrbitalShell&>(cgto);
    }
    else
        throw version_error(v, "1", r_cgto, CODELOC);
        
    return ds;
}

/** Constructor */
CGTO::CGTO() : OrbitalShell()
{}

/** Construct using the passed alphas (exponents) and (unnormalised) 
    scaling factors 
    
    \throw SireError::incompatible_error
*/
CGTO::CGTO(const NVector &alphas, const NVector &scales)
     : OrbitalShell(), alfas(alphas), scls(scales)
{
    if (alfas.count() != scls.count())
        throw SireError::incompatible_error( QObject::tr(
                "You cannot construct a contracted GTO using a different "
                "number of alpha values (%1) to scale factor values (%2).")
                    .arg(alphas.count()).arg(scales.count()), CODELOC );
}

/** Destructor */
CGTO::~CGTO()
{}

/** Copy assignment operator */
CGTO& CGTO::operator=(const CGTO &other)
{
    if (this != &other)
    {
        alfas = other.alfas;
        scls = other.scls;
        OrbitalShell::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool CGTO::operator==(const CGTO &other) const
{
    return this == &other or
           (alfas == other.alfas and scls == other.scls and 
            OrbitalShell::operator==(other ) );
}

/** Comparison operator */
bool CGTO::operator!=(const CGTO &other) const
{
    return not this->operator==(other);
}

/** Return the number of contractions (primitive gaussians) that
    are used to construct this orbital */
int CGTO::nContractions() const
{
    return alfas.count();
}

/** Return the vector of alpha values */
const NVector& CGTO::alpha() const
{
    return alfas;
}

/** Convenient synonym for 'alpha()' - so you can write
    a.alpha() * b.beta() and have it mean what you expect */
const NVector& CGTO::beta() const
{
    return CGTO::alpha();
}

/** Return the vector of unnormalised scaling factors */
const NVector& CGTO::scale() const
{
    return scls;
}

const char* CGTO::typeName()
{
    return "Squire::CGTO";
}
