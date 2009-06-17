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

#include "point.h"

#include "SireMol/evaluator.h"
#include "SireMol/molecule.h"

#include "SireVol/aabox.h"

#include <boost/tuple/tuple.hpp>

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireMaths;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

using boost::tuples::tuple;

//////////////
////////////// Implementation of Point
//////////////

static const RegisterMetaType<Point> r_point( MAGIC_ONLY, Point::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const Point &point)
{
    writeHeader(ds, r_point, 1);
    
    ds << point.p << static_cast<const Property&>(point);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, Point &point)
{
    VersionID v = readHeader(ds, r_point);
    
    if (v == 1)
    {
        ds >> point.p >> static_cast<Property&>(point);
    }
    else
        throw version_error(v, "1", r_point, CODELOC);
        
    return ds;
}

/** Constructor */
Point::Point() : Property()
{}

/** Construct with the passed initial point */
Point::Point(const Vector &point) : Property(), p(point)
{}

/** Copy constructor */
Point::Point(const Point &other) : Property(other), p(other.p)
{}

/** Destructor */
Point::~Point()
{}

/** Copy assignment operator */
Point& Point::operator=(const Point &other)
{
    p = other.p;
    Property::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool Point::operator==(const Point &other) const
{
    return p == other.p and Property::operator==(other);
}

/** Comparison operator */
bool Point::operator!=(const Point &other) const
{
    return p != other.p or Property::operator!=(other);
}

/** Return the point in 3D space */
const Vector& Point::operator()() const
{
    return p;
}

/** Update the point to 'point' */
void Point::updatePoint(const Vector &point)
{
    p = point;
}

/** Return the point in 3D space */
const Vector& Point::point() const
{
    return Point::operator()();
}

//////////////
////////////// Implementation of AtomPoint
//////////////

static const RegisterMetaType<AtomPoint> r_atompoint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const AtomPoint &atompoint)
{
    writeHeader(ds, r_atompoint, 1);
    
    SharedDataStream sds(ds);
    
    sds << atompoint.atm << static_cast<const Point&>(atompoint);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, AtomPoint &atompoint)
{
    VersionID v = readHeader(ds, r_atompoint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> atompoint.atm >> static_cast<Point&>(atompoint);
    }
    else
        throw version_error(v, "1", r_atompoint, CODELOC);
        
    return ds;
}

/** Constructor */
AtomPoint::AtomPoint() : ConcreteProperty<AtomPoint,Point>()
{}

/** Construct for the passed atom */
AtomPoint::AtomPoint(const Atom &atom, const PropertyMap &map)
          : ConcreteProperty<AtomPoint,Point>(), atm(atom)
{
    AtomPoint::update(atom.data(), map);
}

/** Copy constructor */
AtomPoint::AtomPoint(const AtomPoint &other)
          : ConcreteProperty<AtomPoint,Point>(other), atm(other.atm)
{}

/** Destructor */
AtomPoint::~AtomPoint()
{}

/** Copy assignment operator */
AtomPoint& AtomPoint::operator=(const AtomPoint &other)
{
    Point::operator=(other);
    atm = other.atm;
    
    return *this;
}

/** Comparison operator */
bool AtomPoint::operator==(const AtomPoint &other) const
{
    return atm == other.atm and Point::operator==(other);
}

/** Comparison operator */
bool AtomPoint::operator!=(const AtomPoint &other) const
{
    return atm != other.atm or Point::operator!=(other);
}

const char* AtomPoint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomPoint>() );
}

/** Update this point 

    \throw SireBase::missing_property
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
void AtomPoint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (atm.data().number() == moldata.number())
    {
        atm.update(moldata);
        this->updatePoint(atm.property<Vector>(map["coordinates"]));
    }
}
                    
/** Update this point */
void AtomPoint::update(const Molecules &molecules, const PropertyMap &map)
{
    if (molecules.contains(atm.data().number()))
    {
        this->update( molecules[atm.data().number()].data(), map );
    }
}

/** Return the molecules needed to get this point */
Molecules AtomPoint::molecules() const
{
    Molecules mols;

    if (not atm.isEmpty())
        mols += atm;
    
    return mols;
}

/** Does this point require information from the molecule with number 'molnum' */
bool AtomPoint::contains(MolNum molnum) const
{
    return atm.data().number() == molnum;
}

/** Does this point require information from the molecule with ID 'molid' */
bool AtomPoint::contains(const MolID &molid) const
{
    try
    {
        return not molid.map( this->molecules() ).isEmpty();
    }
    catch(...)
    {
        return false;
    }
}

/** Return the actual atom */
const Atom& AtomPoint::atom() const
{
    return atm;
}

//////////////
////////////// Implementation of VectorPoint
//////////////

static const RegisterMetaType<VectorPoint> r_vectorpoint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const VectorPoint &vectorpoint)
{
    writeHeader(ds, r_vectorpoint, 1);
    
    ds << static_cast<const Point&>(vectorpoint);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, VectorPoint &vectorpoint)
{
    VersionID v = readHeader(ds, r_vectorpoint);
    
    if (v == 1)
    {
        ds >> static_cast<Point&>(vectorpoint);
    }
    else
        throw version_error( v, "1", r_vectorpoint, CODELOC );
        
    return ds;
}

/** Constructor */
VectorPoint::VectorPoint() : ConcreteProperty<VectorPoint,Point>()
{}

/** Constructor for the specified point */
VectorPoint::VectorPoint(const Vector &point)
            : ConcreteProperty<VectorPoint,Point>(point)
{}

/** Copy constructor */
VectorPoint::VectorPoint(const VectorPoint &other)
            : ConcreteProperty<VectorPoint,Point>(other)
{}

/** Destructor */
VectorPoint::~VectorPoint()
{}

/** Copy assignment operator */
VectorPoint& VectorPoint::operator=(const VectorPoint &other)
{
    Point::operator=(other);
    return *this;
}

/** Comparison operator */
bool VectorPoint::operator==(const VectorPoint &other) const
{
    return Point::operator==(other);
}

/** Comparison operator */
bool VectorPoint::operator!=(const VectorPoint &other) const
{
    return Point::operator!=(other);
}

const char* VectorPoint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VectorPoint>() );
}

/** A VectorPoint is not updatable */
void VectorPoint::update(const MoleculeData&, const PropertyMap&)
{}
                    
/** A VectorPoint is not updatable */
void VectorPoint::update(const Molecules&, const PropertyMap&)
{}

/** No molecules are needed to create this point */
Molecules VectorPoint::molecules() const
{
    return Molecules();
}

/** No molecules are needed to create this point */
bool VectorPoint::contains(MolNum) const
{
    return false;
}

/** No molecules are needed to create this point */
bool VectorPoint::contains(const MolID&) const
{
    return false;
}

//////////////
////////////// Implementation of Center
//////////////

static const RegisterMetaType<Center> r_center;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const Center &center)
{
    writeHeader(ds, r_center, 1);
    
    SharedDataStream sds(ds);
    
    sds << center.mols << static_cast<const Point&>(center);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, Center &center)
{
    VersionID v = readHeader(ds, r_center);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> center.mols >> static_cast<Point&>(center);
    }
    else
        throw version_error( v, "1", r_center, CODELOC );
        
    return ds;
}

/** Constructor */
Center::Center() : ConcreteProperty<Center,Point>()
{}

/** Construct to get the center of the molecule view 'molview' using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Center::Center(const MoleculeView &molview, const PropertyMap &map)
       : ConcreteProperty<Center,Point>(), mols(molview)
{
    Center::update(molview.data(), map);
}

/** Construct to get the center of the molecules in 'molecules', using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Center::Center(const Molecules &molecules, const PropertyMap &map)
       : ConcreteProperty<Center,Point>(), mols(molecules)
{
    Center::update(molecules, map);
}

/** Copy constructor */
Center::Center(const Center &other)
       : ConcreteProperty<Center,Point>(other), mols(other.mols)
{}

/** Destructor */
Center::~Center()
{}

/** Copy assignment operator */
Center& Center::operator=(const Center &other)
{
    mols = other.mols;
    Point::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool Center::operator==(const Center &other) const
{
    return mols == other.mols and Point::operator==(other);
}

/** Comparison operator */
bool Center::operator!=(const Center &other) const
{
    return mols != other.mols or Point::operator!=(other);
}

const char* Center::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Center>() );
}

/** Internal function used to recalculate the center */
void Center::recalculatePoint(const PropertyMap &map)
{
    if (mols.isEmpty())
        return;

    Molecules::const_iterator it = mols.constBegin();

    AABox aabox = it->evaluate().center(map);
    
    for (++it;
         it != mols.constEnd();
         ++it)
    {
        aabox += it->evaluate().center(map);
    }
    
    Point::updatePoint( aabox.center() );
}

/** Update the molecules used to create this point */
void Center::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (mols.contains(moldata.number()))
    {
        if (mols.update(moldata))
            this->recalculatePoint(map);
    }
}
                    
/** Update the molecules used to create this point */
void Center::update(const Molecules &molecules, const PropertyMap &map)
{
    if (not mols.update(molecules).isEmpty())
    {
        this->recalculatePoint(map);
    }
}

/** Return all of the molecules used to generate this point */
Molecules Center::molecules() const
{
    return mols;
}

/** Return whether or not the molecule with number 'molnum' is
    needed to generate this point */
bool Center::contains(MolNum molnum) const
{
    return mols.contains(molnum);
}

/** Return whether or not this molecule with ID 'molid' is 
    needed to generate this point */
bool Center::contains(const MolID &molid) const
{
    try
    {
        return not molid.map(mols).isEmpty();
    }
    catch(...)
    {
        return false;
    }
}

//////////////
////////////// Implementation of CenterOfGeometry
//////////////

static const RegisterMetaType<CenterOfGeometry> r_cog;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CenterOfGeometry &cog)
{
    writeHeader(ds, r_cog, 1);
    
    SharedDataStream sds(ds);
    
    sds << cog.mols << static_cast<const Point&>(cog);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CenterOfGeometry &cog)
{
    VersionID v = readHeader(ds, r_cog);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cog.mols >> static_cast<Point&>(cog);
    }
    else
        throw version_error( v, "1", r_cog, CODELOC );
        
    return ds;
}

/** Constructor */
CenterOfGeometry::CenterOfGeometry() : ConcreteProperty<CenterOfGeometry,Point>()
{}

/** Construct to get the center of the molecule view 'molview' using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfGeometry::CenterOfGeometry(const MoleculeView &molview, const PropertyMap &map)
                 : ConcreteProperty<CenterOfGeometry,Point>(), mols(molview)
{
    CenterOfGeometry::update(molview.data(), map);
}

/** Construct to get the center of the molecules in 'molecules', using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfGeometry::CenterOfGeometry(const Molecules &molecules, const PropertyMap &map)
                 : ConcreteProperty<CenterOfGeometry,Point>(), mols(molecules)
{
    CenterOfGeometry::update(molecules, map);
}

/** Copy constructor */
CenterOfGeometry::CenterOfGeometry(const CenterOfGeometry &other)
                 : ConcreteProperty<CenterOfGeometry,Point>(other), mols(other.mols)
{}

/** Destructor */
CenterOfGeometry::~CenterOfGeometry()
{}

/** Copy assignment operator */
CenterOfGeometry& CenterOfGeometry::operator=(const CenterOfGeometry &other)
{
    mols = other.mols;
    Point::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool CenterOfGeometry::operator==(const CenterOfGeometry &other) const
{
    return mols == other.mols and Point::operator==(other);
}

/** Comparison operator */
bool CenterOfGeometry::operator!=(const CenterOfGeometry &other) const
{
    return mols != other.mols or Point::operator!=(other);
}

const char* CenterOfGeometry::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CenterOfGeometry>() );
}

static Vector averagePoints(const QList< tuple<Vector,double> > &points)
{
    double sum_of_weights = 0;
    
    for (QList< tuple<Vector,double> >::const_iterator it = points.constBegin();
         it != points.constEnd();
         ++it)
    {
        sum_of_weights += it->get<1>();
    }
    
    sum_of_weights = 1.0 / sum_of_weights;
    
    Vector point;
    
    for (QList< tuple<Vector,double> >::const_iterator it = points.constBegin();
         it != points.constEnd();
         ++it)
    {
        point += (sum_of_weights * it->get<1>()) * it->get<0>();
    }
    
    return point;
}

/** Internal function used to recalculate the center */
void CenterOfGeometry::recalculatePoint(const PropertyMap &map)
{
    if (mols.isEmpty())
        return;

    else if (mols.count() == 1)
    {
        Point::updatePoint( mols.constBegin()->evaluate().centerOfGeometry(map) );
        return;
    }

    QList< tuple<Vector,double> > points;

    for (Molecules::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        points.append( tuple<Vector,double>( it->evaluate().centerOfGeometry(map),
                                             it->selection().nAtoms() ) );
    }
    
    Point::updatePoint( ::averagePoints(points) );
}

/** Update the molecules used to create this point */
void CenterOfGeometry::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (mols.contains(moldata.number()))
    {
        if (mols.update(moldata))
            this->recalculatePoint(map);
    }
}
                    
/** Update the molecules used to create this point */
void CenterOfGeometry::update(const Molecules &molecules, const PropertyMap &map)
{
    if (not mols.update(molecules).isEmpty())
    {
        this->recalculatePoint(map);
    }
}

/** Return all of the molecules used to generate this point */
Molecules CenterOfGeometry::molecules() const
{
    return mols;
}

/** Return whether or not the molecule with number 'molnum' is
    needed to generate this point */
bool CenterOfGeometry::contains(MolNum molnum) const
{
    return mols.contains(molnum);
}

/** Return whether or not this molecule with ID 'molid' is 
    needed to generate this point */
bool CenterOfGeometry::contains(const MolID &molid) const
{
    try
    {
        return not molid.map(mols).isEmpty();
    }
    catch(...)
    {
        return false;
    }
}

//////////////
////////////// Implementation of CenterOfMass
//////////////

static const RegisterMetaType<CenterOfMass> r_com;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CenterOfMass &com)
{
    writeHeader(ds, r_com, 1);
    
    SharedDataStream sds(ds);
    
    sds << com.mols << static_cast<const Point&>(com);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CenterOfMass &com)
{
    VersionID v = readHeader(ds, r_com);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> com.mols >> static_cast<Point&>(com);
    }
    else
        throw version_error( v, "1", r_com, CODELOC );
        
    return ds;
}

/** Constructor */
CenterOfMass::CenterOfMass() : ConcreteProperty<CenterOfMass,Point>()
{}

/** Construct to get the center of the molecule view 'molview' using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfMass::CenterOfMass(const MoleculeView &molview, const PropertyMap &map)
                 : ConcreteProperty<CenterOfMass,Point>(), mols(molview)
{
    CenterOfMass::update(molview.data(), map);
}

/** Construct to get the center of the molecules in 'molecules', using the
    passed property map to find the required properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfMass::CenterOfMass(const Molecules &molecules, const PropertyMap &map)
                 : ConcreteProperty<CenterOfMass,Point>(), mols(molecules)
{
    CenterOfMass::update(molecules, map);
}

/** Copy constructor */
CenterOfMass::CenterOfMass(const CenterOfMass &other)
                 : ConcreteProperty<CenterOfMass,Point>(other), mols(other.mols)
{}

/** Destructor */
CenterOfMass::~CenterOfMass()
{}

/** Copy assignment operator */
CenterOfMass& CenterOfMass::operator=(const CenterOfMass &other)
{
    mols = other.mols;
    Point::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool CenterOfMass::operator==(const CenterOfMass &other) const
{
    return mols == other.mols and Point::operator==(other);
}

/** Comparison operator */
bool CenterOfMass::operator!=(const CenterOfMass &other) const
{
    return mols != other.mols or Point::operator!=(other);
}

const char* CenterOfMass::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CenterOfMass>() );
}

/** Internal function used to recalculate the center */
void CenterOfMass::recalculatePoint(const PropertyMap &map)
{
    if (mols.isEmpty())
        return;

    else if (mols.count() == 1)
    {
        Point::updatePoint( mols.constBegin()->evaluate().centerOfMass(map) );
        return;
    }

    QList< tuple<Vector,double> > points;

    for (Molecules::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        Evaluator evaluator(*it);
    
        points.append( tuple<Vector,double>( evaluator.centerOfMass(map),
                                             evaluator.mass() ) );
    }
    
    Point::updatePoint( ::averagePoints(points) );
}

/** Update the molecules used to create this point */
void CenterOfMass::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (mols.contains(moldata.number()))
    {
        if (mols.update(moldata))
            this->recalculatePoint(map);
    }
}
                    
/** Update the molecules used to create this point */
void CenterOfMass::update(const Molecules &molecules, const PropertyMap &map)
{
    if (not mols.update(molecules).isEmpty())
    {
        this->recalculatePoint(map);
    }
}

/** Return all of the molecules used to generate this point */
Molecules CenterOfMass::molecules() const
{
    return mols;
}

/** Return whether or not the molecule with number 'molnum' is
    needed to generate this point */
bool CenterOfMass::contains(MolNum molnum) const
{
    return mols.contains(molnum);
}

/** Return whether or not this molecule with ID 'molid' is 
    needed to generate this point */
bool CenterOfMass::contains(const MolID &molid) const
{
    try
    {
        return not molid.map(mols).isEmpty();
    }
    catch(...)
    {
        return false;
    }
}

//////////////
////////////// Implementation of PointRef
//////////////

/** Construct from the passed atom */
PointRef::PointRef(const Atom &atom) : ptr( AtomPoint(atom) )
{}

/** Construct from the passed point */
PointRef::PointRef(const Vector &point) : ptr( VectorPoint(point) )
{}

/** Construct from the passed point */
PointRef::PointRef(const Point &point) : ptr(point)
{}

/** Destructor */
PointRef::~PointRef()
{}

/** Return the point in 3D space */
const Vector& PointRef::operator()() const
{
    return ptr.read().operator()();
}

/** Return the point in 3D space */
const Vector& PointRef::point() const
{
    return ptr.read().point();
}

/** Allow automatic casting to a Point */
PointRef::operator const Point&() const
{
    return ptr.read();
}
