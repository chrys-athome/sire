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

#include "cgto.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CGTO> r_cgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const CGTO &cgto)
{
	writeHeader(ds, r_cgto, 1);

	SharedDataStream sds(ds);
    
    sds << cgto.prims << static_cast<const OrbitalShell&>(cgto);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, CGTO &cgto)
{
	VersionID v = readHeader(ds, r_cgto);
    
    if (v == 1)
    {
    	SharedDataStream sds(ds);
        
        sds >> cgto.prims >> static_cast<OrbitalShell&>(cgto);
    }
    else
    	throw version_error(v, "1", r_cgto, CODELOC);
        
    return ds;
}

/** Constructor */
CGTO::CGTO() : ConcreteProperty<CGTO,OrbitalShell>()
{}

void CGTO::assertSane()
{
	//remove all primitives with a zero coefficient
    {
		int idx = 0;
        
        while (idx < prims.count())
        {
        	if (prims.at(idx).isNull())
            {
                prims.remove(idx);
            }
            else
            	++idx;
        }
    
    	if (prims.isEmpty())
        {
        	prims = QVector<GTOPtr>();
            return;
        }
    }
    
    //now check that all of the primitives have the same angular
    //momentum
    {
    	int l = prims.at(0).read().angularMomentum();
    
	    for (QVector<GTOPtr>::const_iterator it = prims.constBegin();
    	     it != prims.constEnd();
        	 ++it)
		{
    		if (it->read().angularMomentum() != l)
            {
            	throw SireError::incompatible_error( QObject::tr(
                		"Cannot construct a contracted Gaussian type "
                        "orbital shell from the passed primitives as they "
                        "don't all have the same angular momentum! %1")
                        	.arg( Sire::toString(prims) ), CODELOC );
            }
    	}
	}

    //collapse down any repeated primitives
    {
    	int i = 0;
        
        while (i < prims.count())
        {
        	const GTO &start = prims[i].read();
            
            int j = i + 1;
            
            while( j < prims.count() )
            {
            	const GTO &next = prims[j].read();
            
            	if (start.alpha() == next.alpha())
                {
                	prims[i] = start.multiply( 1 + (next.scale()/start.scale()) );

                    prims.remove(j);
                }
                else
                	++j;
            }
            
            ++i;
        }
    }
    
    prims.squeeze();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const boost::tuple<double,GTOPtr> &primitive)
     : ConcreteProperty<CGTO,OrbitalShell>()
{
    prims.append( primitive.get<1>().read().multiply(primitive.get<0>()) );
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const boost::tuple<GTOPtr,double> &primitive)
     : ConcreteProperty<CGTO,OrbitalShell>()
{
    prims.append( primitive.get<0>().read().multiply(primitive.get<1>()) );
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const GTO &primitive, double coefficient)
	 : ConcreteProperty<CGTO,OrbitalShell>()
{
    prims.append(primitive.multiply(coefficient));
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(double coefficient, const GTO &primitive)
     : ConcreteProperty<CGTO,OrbitalShell>()
{
    prims.append(primitive.multiply(coefficient));
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const QVector< boost::tuple<double,GTOPtr> > &primitives)
	 : ConcreteProperty<CGTO,OrbitalShell>()
{
	for (QVector< boost::tuple<double,GTOPtr> >::const_iterator 
    												it = primitives.constBegin();
		 it != primitives.constEnd();
         ++it)
	{
    	prims.append( it->get<1>().read().multiply(it->get<0>()) );
    }
    
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const QVector< boost::tuple<GTOPtr,double> > &primitives)
     : ConcreteProperty<CGTO,OrbitalShell>()
{
	for (QVector< boost::tuple<GTOPtr,double> >::const_iterator
    												it = primitives.constBegin();
         it != primitives.constEnd();
         ++it)
    {
        prims.append( it->get<0>().read().multiply(it->get<1>()) );
    }
    
    this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const QVector<GTOPtr> &primitives, const QVector<double> &coefficients)
     : ConcreteProperty<CGTO,OrbitalShell>(),
       prims(primitives)
{
	if (coefficients.count() != primitives.count())
    	throw SireError::incompatible_error( QObject::tr(
        	"Cannot construct a contracted Gaussian type orbital shell "
            "as the number of coefficients (%1) is different to the number "
            "of primitives (%2).")
            	.arg(coefficients.count()).arg(primitives.count()), CODELOC );

	for (int i=0; i<coefficients.count(); ++i)
    {
    	prims[i] = prims[i].read().multiply( coefficients[i] );
    }

	this->assertSane();
}

/** Construct from the passed set of primitives and coefficients.
    All of the primitives must have the same angular momentum 

	\throw SireError::incompatible_error
*/
CGTO::CGTO(const QVector<double> &coefficients, const QVector<GTOPtr> &primitives)
     : ConcreteProperty<CGTO,OrbitalShell>(),
       prims(primitives)

{
	if (coefficients.count() != primitives.count())
    	throw SireError::incompatible_error( QObject::tr(
        	"Cannot construct a contracted Gaussian type orbital shell "
            "as the number of coefficients (%1) is different to the number "
            "of primitives (%2).")
            	.arg(coefficients.count()).arg(primitives.count()), CODELOC );

	for (int i=0; i<coefficients.count(); ++i)
    {
    	prims[i] = prims[i].read().multiply( coefficients[i] );
    }

	this->assertSane();
}

/** Copy constructor */
CGTO::CGTO(const CGTO &other)
     : ConcreteProperty<CGTO,OrbitalShell>(other),
       prims(other.prims)
{}

/** Destructor */
CGTO::~CGTO()
{}

const char* CGTO::typeName()
{
	return QMetaType::typeName( qMetaTypeId<CGTO>() );
}

/** Copy assignment operator */
CGTO& CGTO::operator=(const CGTO &other)
{
	if (this != &other)
    {
    	prims = other.prims;
        OrbitalShell::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool CGTO::operator==(const CGTO &other) const
{
	return this == &other or
           (prims == other.prims and
            OrbitalShell::operator==(other));
}

/** Comparison operator */
bool CGTO::operator!=(const CGTO &other) const
{
	return not CGTO::operator==(other);
}

/** Return the array of primitives that are used to construct
    this orbital shell */
const QVector<GTOPtr>& CGTO::primitives() const
{
	return prims;
}

/** Return the number of primitives that are used to 
    construct this orbital shell */
int CGTO::nPrimitives() const
{
	return prims.count();
}

/** Return whether or not this is null (has no primitives) */
bool CGTO::isNull() const
{
	return prims.isEmpty();
}

/** Return a string representation of this orbital */
QString CGTO::toString() const
{
	if (prims.isEmpty())
    	return QObject::tr("GTO::null");
        
	QStringList primitives;
    
    for (int i=0; i<prims.count(); ++i)
    {
    	primitives.append(prims[i].read().toString());
    }
    
    return primitives.join(" + ");
}

/** Return the angular momentum of this shell */
int CGTO::angularMomentum() const
{
	if (not prims.isEmpty())
    	return prims.at(0).read().angularMomentum();
	else
    	return -1;
}

/** Return the number of orbitals that make up this shell */
int CGTO::nOrbitals() const
{
	if (not prims.isEmpty())
    	return prims.at(0).read().nOrbitals();
    else
    	return 0;
}
