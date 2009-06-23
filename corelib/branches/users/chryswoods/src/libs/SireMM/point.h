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

#ifndef SIREMM_POINT_H
#define SIREMM_POINT_H

#include "SireBase/property.h"

#include "SireMol/atom.h"
#include "SireMol/molecules.h"
#include "SireMol/selector.hpp"
#include "SireMol/mover.hpp"

#include "SireVol/space.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

class Point;
class PointBase;

class AtomPoint;
class VectorPoint;
class Center;
class CenterOfMass;
class CenterOfGeometry;

}

QDataStream& operator<<(QDataStream&, const SireMM::Point&);
QDataStream& operator>>(QDataStream&, SireMM::Point&);

QDataStream& operator<<(QDataStream&, const SireMM::AtomPoint&);
QDataStream& operator>>(QDataStream&, SireMM::AtomPoint&);

QDataStream& operator<<(QDataStream&, const SireMM::VectorPoint&);
QDataStream& operator>>(QDataStream&, SireMM::VectorPoint&);

QDataStream& operator<<(QDataStream&, const SireMM::Center&);
QDataStream& operator>>(QDataStream&, SireMM::Center&);

QDataStream& operator<<(QDataStream&, const SireMM::CenterOfGeometry&);
QDataStream& operator>>(QDataStream&, SireMM::CenterOfGeometry&);

QDataStream& operator<<(QDataStream&, const SireMM::CenterOfMass&);
QDataStream& operator>>(QDataStream&, SireMM::CenterOfMass&);

namespace SireFF
{
class MolForceTable;
class ForceTable;
}

namespace SireMM
{

using SireMol::Atom;
using SireMol::MoleculeData;
using SireMol::MoleculeView;
using SireMol::Molecules;
using SireMol::MolNum;
using SireMol::MolID;

using SireFF::MolForceTable;
using SireFF::ForceTable;

using SireBase::PropertyMap;

using SireVol::Space;

using SireMaths::Vector;

/** This is the base class of all Points. A Point is a class that 
    allows a view of a molecule (or molecules) to be turned
    into a 3D point. This 3D point can then be used within a restraint
    class (thereby seperating out the code that implements the 
    restraint from the code that selects the parts of the molecule(s)
    being restrained)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT Point : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Point&);
friend QDataStream& ::operator>>(QDataStream&, Point&);

public:
    Point();
    
    virtual ~Point();

    virtual Point* clone() const=0;
    
    const Vector& operator()() const;
    
    static const char* typeName()
    {
        return "SireMM::Point";
    }
    
    const Vector& point() const;
    
    const Space& space() const;
    
    virtual void setSpace(const Space &space);
    
    virtual void update(const MoleculeData &moldata)=0;
    virtual void update(const Molecules &molecules)=0;
    
    virtual Molecules molecules() const=0;
    
    virtual int nMolecules() const=0;
    
    virtual bool contains(MolNum molnum) const=0;
    virtual bool contains(const MolID &molid) const=0;
    
    virtual bool usesMoleculesIn(const ForceTable &forcetable) const=0;
    virtual bool usesMoleculesIn(const Molecules &molecules) const=0;
    
    virtual bool addForce(MolForceTable &molforces, const Vector &force) const=0;
    virtual bool addForce(ForceTable &forces, const Vector &force) const=0;
    
    static const VectorPoint& null();

    static bool intraMoleculePoints(const Point &point0, const Point &point1);
    
protected:
    Point(const Vector &point);
    Point(const Point &other);
    
    Point& operator=(const Point &other);
    
    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;

    void updatePoint(const Vector &point);

private:
    /** The actual point in space */
    Vector p;
    
    /** The 3D space in which this point is calculated and exists */
    SireVol::SpacePtr spce;
};

typedef SireBase::PropPtr<Point> PointPtr;

/** This is a small class used to help convert different molecule
    views into a Point class in function signatures, e.g.
    
    DistanceRestraint(const PointRef &point0, const PointRef &point1);
    
    would allow distance restraints to be created between pairs
    of atoms, or an atom and a vector etc.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT PointRef
{
public:
    PointRef(const Atom &atom);
    PointRef(const Vector &point);
    PointRef(const Point &point);
    PointRef(const PointPtr &point);
    
    ~PointRef();
    
    const Vector& operator()() const;
    
    const Vector& point() const;
    
    operator const Point&() const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;
    
private:
    /** Pointer to the implementation of this point */
    PointPtr ptr;
};

/** This point returns the location of an atom */
class SIREMM_EXPORT AtomPoint : public SireBase::ConcreteProperty<AtomPoint,Point>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomPoint&);
friend QDataStream& ::operator>>(QDataStream&, AtomPoint&);

public:
    AtomPoint();
    AtomPoint(const Atom &atom, const PropertyMap &map=PropertyMap());
    
    AtomPoint(const AtomPoint &other);
    
    ~AtomPoint();
    
    AtomPoint& operator=(const AtomPoint &other);
    
    bool operator==(const AtomPoint &other) const;
    bool operator!=(const AtomPoint &other) const;
    
    static const char* typeName();
    
    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    
    Molecules molecules() const;
    
    int nMolecules() const;
    
    bool contains(MolNum molnum) const;
    bool contains(const MolID &molid) const;

    bool usesMoleculesIn(const ForceTable &forcetable) const;
    bool usesMoleculesIn(const Molecules &molecules) const;

    const Atom& atom() const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;

private:
    /** The actual atom! */
    Atom atm;
    
    /** The coordinates property */
    SireBase::PropertyName coords_property;
};

/** This is a simple wrapper for a point in space */
class SIREMM_EXPORT VectorPoint : public SireBase::ConcreteProperty<VectorPoint,Point>
{

friend QDataStream& ::operator<<(QDataStream&, const VectorPoint&);
friend QDataStream& ::operator>>(QDataStream&, VectorPoint&);

public:
    VectorPoint();
    VectorPoint(const Vector &point);
    
    VectorPoint(const VectorPoint &other);
    
    ~VectorPoint();
    
    VectorPoint& operator=(const VectorPoint &other);
    
    bool operator==(const VectorPoint &other) const;
    bool operator!=(const VectorPoint &other) const;
    
    static const char* typeName();
    
    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    
    Molecules molecules() const;
    
    int nMolecules() const;
    
    bool contains(MolNum molnum) const;
    bool contains(const MolID &molid) const;

    bool usesMoleculesIn(const ForceTable &forcetable) const;
    bool usesMoleculesIn(const Molecules &molecules) const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;
};

/** This point returns the center of a view of a molecule, or group
    of molecules */
class SIREMM_EXPORT Center : public SireBase::ConcreteProperty<Center,Point>
{

friend QDataStream& ::operator<<(QDataStream&, const Center&);
friend QDataStream& ::operator>>(QDataStream&, Center&);

public:
    Center();
    Center(const MoleculeView &molview, const PropertyMap &map = PropertyMap());
    Center(const Molecules &molecules, const PropertyMap &map = PropertyMap());
    
    Center(const Center &other);
    
    ~Center();
    
    Center& operator=(const Center &other);
    
    bool operator==(const Center &other) const;
    bool operator!=(const Center &other) const;
    
    static const char* typeName();

    void setSpace(const Space &space);
    
    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    
    Molecules molecules() const;
    
    int nMolecules() const;
    
    bool contains(MolNum molnum) const;
    bool contains(const MolID &molid) const;

    bool usesMoleculesIn(const ForceTable &forcetable) const;
    bool usesMoleculesIn(const Molecules &molecules) const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;

private:
    void recalculatePoint();

    /** The molecules whose center is to be determined */
    Molecules mols;

    /** The map used to find the properties */
    PropertyMap property_map;
};

/** This point returns the center of geometry of a view of a molecule,
    or group of molecules */
class SIREMM_EXPORT CenterOfGeometry 
            : public SireBase::ConcreteProperty<CenterOfGeometry,Point>
{

friend QDataStream& ::operator<<(QDataStream&, const CenterOfGeometry&);
friend QDataStream& ::operator>>(QDataStream&, CenterOfGeometry&);

public:
    CenterOfGeometry();
    CenterOfGeometry(const MoleculeView &molview,
                     const PropertyMap &map = PropertyMap());
    CenterOfGeometry(const Molecules &molecules,
                     const PropertyMap &map = PropertyMap());
                     
    CenterOfGeometry(const CenterOfGeometry &other);
    
    ~CenterOfGeometry();
    
    CenterOfGeometry& operator=(const CenterOfGeometry &other);

    bool operator==(const CenterOfGeometry &other) const;
    bool operator!=(const CenterOfGeometry &other) const;

    static const char* typeName();

    void setSpace(const Space &space);
    
    void update(const MoleculeData &moldata);
    void update(const Molecules &molecules);
    
    Molecules molecules() const;
    
    int nMolecules() const;
    
    bool contains(MolNum molnum) const;
    bool contains(const MolID &molid) const;

    bool usesMoleculesIn(const ForceTable &forcetable) const;
    bool usesMoleculesIn(const Molecules &molecules) const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;

private:
    void recalculatePoint();

    /** The molecules whose center of geometry is to be determined */
    Molecules mols;

    /** The map used to find the properties */
    PropertyMap property_map;
};

/** This point returns the center of mass of a view of a molecule,
    or group of molecules */
class SIREMM_EXPORT CenterOfMass 
            : public SireBase::ConcreteProperty<CenterOfMass,Point>
{

friend QDataStream& ::operator<<(QDataStream&, const CenterOfMass&);
friend QDataStream& ::operator>>(QDataStream&, CenterOfMass&);

public:
    CenterOfMass();
    CenterOfMass(const MoleculeView &molview,
                 const PropertyMap &map = PropertyMap());
    CenterOfMass(const Molecules &molecules,
                 const PropertyMap &map = PropertyMap());
                     
    CenterOfMass(const CenterOfMass &other);
    
    ~CenterOfMass();
    
    CenterOfMass& operator=(const CenterOfMass &other);

    bool operator==(const CenterOfMass &other) const;
    bool operator!=(const CenterOfMass &other) const;

    static const char* typeName();

    void setSpace(const Space &space);
    
    bool wouldChangeProperties(const PropertyMap &map) const;
    
    void update(const MoleculeData &moldata);
    void update(const MoleculeData &moldata, const PropertyMap &map);
                        
    void update(const Molecules &molecules);
    void update(const Molecules &molecules, const PropertyMap &map);
    
    Molecules molecules() const;
    
    int nMolecules() const;
    
    bool contains(MolNum molnum) const;
    bool contains(const MolID &molid) const;

    bool usesMoleculesIn(const ForceTable &forcetable) const;
    bool usesMoleculesIn(const Molecules &molecules) const;
    
    bool addForce(MolForceTable &molforces, const Vector &force) const;
    bool addForce(ForceTable &forces, const Vector &force) const;

private:
    void recalculatePoint();

    /** The molecules whose center of geometry is to be determined */
    Molecules mols;

    /** The map used to find the properties */
    PropertyMap property_map;
};

}

Q_DECLARE_METATYPE( SireMM::AtomPoint )
Q_DECLARE_METATYPE( SireMM::VectorPoint )
Q_DECLARE_METATYPE( SireMM::Center )
Q_DECLARE_METATYPE( SireMM::CenterOfGeometry )
Q_DECLARE_METATYPE( SireMM::CenterOfMass )

SIRE_EXPOSE_CLASS( SireMM::Point )
SIRE_EXPOSE_CLASS( SireMM::PointRef )
SIRE_EXPOSE_CLASS( SireMM::AtomPoint )
SIRE_EXPOSE_CLASS( SireMM::VectorPoint )
SIRE_EXPOSE_CLASS( SireMM::Center )
SIRE_EXPOSE_CLASS( SireMM::CenterOfGeometry )
SIRE_EXPOSE_CLASS( SireMM::CenterOfMass )

SIRE_EXPOSE_PROPERTY( SireMM::PointPtr, SireMM::Point )

SIRE_END_HEADER

#endif
