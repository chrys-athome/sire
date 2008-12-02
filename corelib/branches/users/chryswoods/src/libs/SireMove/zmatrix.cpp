/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "zmatrix.h"

#include "SireID/index.h"

#include "SireMol/molecule.h"
#include "SireMol/mover.hpp"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"

#include "SireMove/errors.h"
#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireID;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

//////////
////////// Implementation of ZMatrixLine
//////////

static const RegisterMetaType<ZMatrixLine> r_zmatline;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const ZMatrixLine &zmatline)
{
    writeHeader(ds, r_zmatline, 1);
    
    for (int i=0; i<4; ++i)
        ds << zmatline.atms[i];
        
    ds << zmatline.deltas;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, ZMatrixLine &zmatline)
{
    VersionID v = readHeader(ds, r_zmatline);
    
    if (v == 1)
    {
        for (int i=0; i<4; ++i)
            ds >> zmatline.atms[i];
            
        ds >> zmatline.deltas;
    }
    else
        throw version_error( v, "1", r_zmatline, CODELOC );
        
    return ds;
}

/** Null constructor */
ZMatrixLine::ZMatrixLine()
{
    for (int i=0; i<4; ++i)
        atms[i] = AtomIdx(0);
}

/** Construct with the specified atom indicies */
ZMatrixLine::ZMatrixLine(AtomIdx atom, AtomIdx bond, 
                         AtomIdx angle, AtomIdx dihedral)
{
    atms[0] = atom;
    atms[1] = bond;
    atms[2] = angle;
    atms[3] = dihedral;
}

/** Copy constructor */
ZMatrixLine::ZMatrixLine(const ZMatrixLine &other)
            : deltas(other.deltas)
{
    for (int i=0; i<4; ++i)
        atms[i] = other.atms[i];
}

/** Destructor */
ZMatrixLine::~ZMatrixLine()
{}

/** Copy assignment operator */
ZMatrixLine& ZMatrixLine::operator=(const ZMatrixLine &other)
{
    if (this != &other)
    {
        deltas = other.deltas;
        
        for (int i=0; i<4; ++i)
            atms[i] = other.atms[i];
    }
    
    return *this;
}

/** Comparison operator */
bool ZMatrixLine::operator==(const ZMatrixLine &other) const
{
    return atms[0] == other.atms[0] and 
           atms[1] == other.atms[1] and
           atms[2] == other.atms[2] and
           atms[3] == other.atms[3] and
           deltas == other.deltas;
}

/** Comparison operator */
bool ZMatrixLine::operator!=(const ZMatrixLine &other) const
{
    return not this->operator==(other);
}

/** Return a string representation */
QString ZMatrixLine::toString() const
{
    return QObject::tr("ZMatrix: %1-%2-%3-%4")
                 .arg(atms[0].value()).arg(atms[1].value())
                 .arg(atms[2].value()).arg(atms[3].value());
}

/** Return the ith atom index (i==0 is atom, i==1 is bond etc.)

    \throw SireError::invalid_index
*/
AtomIdx ZMatrixLine::operator[](int i) const
{
    return atms[ Index(i).map(4) ];
}

/** Return the index of the atom whose coordinates
    are described in this line */
AtomIdx ZMatrixLine::atom() const
{
    return atms[0];
}

/** Return the index of the bonded atom */
AtomIdx ZMatrixLine::bond() const
{
    return atms[1];
}

/** Return the index of the angled atom */
AtomIdx ZMatrixLine::angle() const
{
    return atms[2];
}

/** Return the index of the dihedralled atom */
AtomIdx ZMatrixLine::dihedral() const
{
    return atms[3];
}

/** Return the maximum amount by which the bond should be changed */
Length ZMatrixLine::bondDelta() const
{
    return Length( deltas[0] );
}

/** Return the maximum amount by which the angle should be changed */
Angle ZMatrixLine::angleDelta() const
{
    return Angle( deltas[1] );
}

/** Return the maximum amount by which the dihedral should be changed */
Angle ZMatrixLine::dihedralDelta() const
{
    return Angle( deltas[2] );
}

/** Set the maximum amount by which the bond should be changed */
void ZMatrixLine::setBondDelta(const Length &delta)
{
    deltas.setX( delta.value() );
}

/** Set the maximum amount by which the angle should be changed */
void ZMatrixLine::setAngleDelta(const Angle &delta)
{
    deltas.setY( delta.value() );
}

/** Set the maximum amount by which the dihedral should be changed */
void ZMatrixLine::setDihedralDelta(const Angle &delta)
{
    deltas.setZ( delta.value() );
}

//////////
////////// Implementation of ZMatrixCoordsLine
//////////

static const RegisterMetaType<ZMatrixCoordsLine> r_zmatcoordsline;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const ZMatrixCoordsLine &zmatcoordsline)
{
    writeHeader(ds, r_zmatcoordsline, 1);
    
    ds << zmatcoordsline.coords
       << static_cast<const ZMatrixLine&>(zmatcoordsline);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, 
                                        ZMatrixCoordsLine &zmatcoordsline)
{
    VersionID v = readHeader(ds, r_zmatcoordsline);
    
    if (v == 1)
    {
        ds >> zmatcoordsline.coords
           >> static_cast<ZMatrixLine&>(zmatcoordsline);
    }
    else
        throw version_error( v, "1", r_zmatcoordsline, CODELOC );
        
    return ds;
}

/** Null constructor */
ZMatrixCoordsLine::ZMatrixCoordsLine() : ZMatrixLine()
{}

/** Construct from the passed line (but with zero coordinates) */
ZMatrixCoordsLine::ZMatrixCoordsLine(const ZMatrixLine &line)
                  : ZMatrixLine(line)
{}

/** Construct from the passed line and coordinates */
ZMatrixCoordsLine::ZMatrixCoordsLine(const ZMatrixLine &line,
                                     const Length &bond, const Angle &angle, 
                                     const Angle &dihedral)
                  : ZMatrixLine(line)
{
    coords.setX( bond.value() );
    coords.setY( angle.value() );
    coords.setZ( dihedral.value() );
}

/** Copy constructor */
ZMatrixCoordsLine::ZMatrixCoordsLine(const ZMatrixCoordsLine &other)
                  : ZMatrixLine(other), coords(other.coords)
{}

/** Destructor */
ZMatrixCoordsLine::~ZMatrixCoordsLine()
{}

/** Copy assignment operator */
ZMatrixCoordsLine& ZMatrixCoordsLine::operator=(const ZMatrixCoordsLine &other)
{
    ZMatrixLine::operator=(other);
    coords = other.coords;
    
    return *this;
}

/** Comparison operator */
bool ZMatrixCoordsLine::operator==(const ZMatrixCoordsLine &other) const
{
    return ZMatrixLine::operator==(other) and coords == other.coords;
}

/** Comparison operator */
bool ZMatrixCoordsLine::operator!=(const ZMatrixCoordsLine &other) const
{
    return ZMatrixLine::operator!=(other) or coords != other.coords;
}

/** Return a string representation */
QString ZMatrixCoordsLine::toString() const
{
    return QObject::tr("%1 - %2 A : %3° : %4°")
                 .arg(ZMatrixLine::toString())
                 .arg(coords[0])
                 .arg( SireUnits::convertTo(coords[1],degrees) )
                 .arg( SireUnits::convertTo(coords[2],degrees) );
}

/** Return the length of the bond */
Length ZMatrixCoordsLine::bondLength() const
{
    return Length(coords[0]);
}

/** Return the size of the angle */
Angle ZMatrixCoordsLine::angleSize() const
{
    return Angle(coords[1]);
}

/** Return the size of the dihedral */
Angle ZMatrixCoordsLine::dihedralSize() const
{
    return Angle(coords[2]);
}

/** Set the length of the bond */
void ZMatrixCoordsLine::setBond(const Length &length)
{
    coords.setX( length.value() );
}

/** Set the size of the angle */
void ZMatrixCoordsLine::setAngle(const Angle &size)
{
    coords.setY( size.value() );
}

/** Set the size of the dihedral */
void ZMatrixCoordsLine::setDihedral(const Angle &size)
{
    coords.setZ( size.value() );
}

//////////
////////// Implementation of ZMatrix
//////////

static const RegisterMetaType<ZMatrix> r_zmat;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const ZMatrix &zmat)
{
    writeHeader(ds, r_zmat, 1);
    
    SharedDataStream sds(ds);
    
    sds << zmat.molinfo << zmat.zmat
        << static_cast<const MoleculeProperty&>(zmat);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, ZMatrix &zmat)
{
    VersionID v = readHeader(ds, r_zmat);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> zmat.molinfo >> zmat.zmat
            >> static_cast<MoleculeProperty&>(zmat);
        
        zmat.reindex();
    }
    else
        throw version_error( v, "1", r_zmat, CODELOC );
        
    return ds;
}

/** Null constructor */
ZMatrix::ZMatrix() : ConcreteProperty<ZMatrix,MoleculeProperty>()
{}

/** Construct to hold the z-matrix for the passed molecule */
ZMatrix::ZMatrix(const Molecule &molecule)
        : ConcreteProperty<ZMatrix,MoleculeProperty>(),
          molinfo(molecule.data().info())
{}

/** Copy constructor */
ZMatrix::ZMatrix(const ZMatrix &other)
        : ConcreteProperty<ZMatrix,MoleculeProperty>(),
          molinfo(other.molinfo),
          zmat(other.zmat), atomidx_to_zmat(other.atomidx_to_zmat)
{}

/** Destructor */
ZMatrix::~ZMatrix()
{}

/** Copy assignment operator */
ZMatrix& ZMatrix::operator=(const ZMatrix &other)
{
    if (this != &other)
    {
        MoleculeProperty::operator=(other);
        molinfo = other.molinfo;
        zmat = other.zmat;
        atomidx_to_zmat = other.atomidx_to_zmat;
    }
    
    return *this;
}

/** Comparison operator */
bool ZMatrix::operator==(const ZMatrix &other) const
{
    return molinfo == other.molinfo and zmat == other.zmat;
}

/** Comparison operator */
bool ZMatrix::operator!=(const ZMatrix &other) const
{
    return molinfo != other.molinfo or zmat != other.zmat;
}

/** Reindex the z-matrix lines */
void ZMatrix::reindex()
{
    atomidx_to_zmat.clear();
    
    int nlines = zmat.count();
    const ZMatrixLine *zmat_array = zmat.constData();
    
    atomidx_to_zmat.reserve(nlines);
    
    for (int i=0; i<nlines; ++i)
    {
        atomidx_to_zmat.insert( zmat_array[i].atom(), i );
    }
}

/** Return the layout of the molecule whose z-matrix is contained
    in this object */
const MoleculeInfoData& ZMatrix::info() const
{
    if (molinfo.constData() == 0)
        return MoleculeInfoData::null();
    else
        return *molinfo;
}

/** Return a string representation of this z-matrix */
QString ZMatrix::toString() const
{
    QStringList lines;
    
    lines.append( QObject::tr("ZMatrix nAtoms() == %1").arg(molinfo->nAtoms()) );
    
    for (int i=0; i<zmat.count(); ++i)
    {
        const ZMatrixLine &line = zmat.at(i);
    
        lines.append( QObject::tr("%1-%2-%3-%4")
                        .arg(line.atom().value())
                        .arg(line.bond().value())
                        .arg(line.angle().value())
                        .arg(line.dihedral().value()) );
    }

    return lines.join("\n");
}

/** Return the line for the atom identified by 'atom'. This
    raises an exception if this is not a valid ID, or if
    there is no z-matrix line for this atom
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
const ZMatrixLine& ZMatrix::operator[](const AtomID &atom) const
{
    AtomIdx idx = molinfo->atomIdx(atom);
    
    if (not atomidx_to_zmat.contains(idx))
        throw SireMove::zmatrix_error( QObject::tr(
            "The atom with ID %1 does not appear in the z-matrix.")
                .arg(atom.toString()), CODELOC );

    return zmat.at(idx);
}

/** Return the line for the atom identified by 'atom'. This
    raises an exception if this is not a valid ID, or if
    there is no z-matrix line for this atom
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
const ZMatrixLine& ZMatrix::at(const AtomID &atom) const
{
    return this->operator[](atom);
}

/** Return all of the lines of the z-matrix */
const QVector<ZMatrixLine>& ZMatrix::lines() const
{
    return zmat;
}

/** Return the index of AtomIdx to z-matrix line number. This
    is used to index the output of ZMatrix::lines() */
const QHash<AtomIdx,int>& ZMatrix::index() const
{
    return atomidx_to_zmat;
}

/** Return whether or not this z-matrix contains a line for the 
    atom with ID 'atom' 
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
bool ZMatrix::contains(const AtomID &atom) const
{
    return atomidx_to_zmat.contains( molinfo->atomIdx(atom) );
}

/** Add a line to the z-matrix that gives the coordinates of the 
    atom 'atom' based on the passed bond, angle and dihedal atoms 
    
    An exception is raised if adding this line would lead
    to a circular reference
    
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
void ZMatrix::add(const AtomID &atom, const AtomID &bond, 
                  const AtomID &angle, const AtomID &dihedral)
{
    AtomIdx atm = molinfo.constData()->atomIdx(atom);
    AtomIdx bnd = molinfo.constData()->atomIdx(bond);
    AtomIdx ang = molinfo.constData()->atomIdx(angle);
    AtomIdx dih = molinfo.constData()->atomIdx(dihedral);
    
    if (atm == bnd or atm == ang or atm == dih or
        bnd == ang or bnd == dih or
        ang == dih)
    {
        throw SireMove::zmatrix_error( QObject::tr(
            "You cannot add a z-matrix line with repeated atoms! "
            "(%1-%2-%3-%4)")
                .arg(atom.toString(), bond.toString(),
                     angle.toString(), dihedral.toString()), CODELOC );
    }

    //work out where this line should be (move it to as late as
    //it can go in the file)
    int earliest = 0;
    int latest = 0;
    
    int nlines = zmat.count();
    const ZMatrixLine *zmat_array = zmat.constData();
    
    for (int i=0; i<nlines; ++i)
    {
        const ZMatrixLine &line = zmat_array[i];
        
        if (line.atom() == atm)
            throw SireMol::duplicate_atom( QObject::tr(
                "The atom %1 is already present in this z-matrix. It cannot "
                "be added twice.").arg(atom.toString()), CODELOC );
    
        if (line.atom() == bnd or line.atom() == ang or line.atom() == dih)
            //we must come after this line
            earliest = i+1;
            
        if (line.bond() == atm or line.angle() == atm or line.dihedral() == atm)
            //we must come before this line
            latest = i;
    }
    
    if (latest < earliest)
        throw SireMove::zmatrix_error( QObject::tr(
            "Cannot add the line %1-%2-%3-%4 as doing so would create "
            "a circular reference.")
                .arg( atom.toString(), bond.toString(),
                      angle.toString(), dihedral.toString() ), CODELOC );
                      
    if (latest + 1 >= nlines)
    {
        zmat.append( ZMatrixLine(atm,bnd,ang,dih) );
        atomidx_to_zmat.insert(atm, nlines);
    }
    else
    {
        zmat.insert( latest+1, ZMatrixLine(atm,bnd,ang,dih) );
        this->reindex();
    }
}

/** Remove the z-matrix line that gives the coordinates of the 
    atom 'atom'. This does nothing if there is no z-matrix
    line for this atom - note this removes the lines for
    *all* matching atoms
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
void ZMatrix::remove(const AtomID &atom)
{
    QList<AtomIdx> atomidxs = atom.map( *(molinfo.constData()) );

    bool need_reindex = false;
    
    foreach (AtomIdx atomidx, atomidxs)
    {
        if (atomidx_to_zmat.contains(atomidx))
        {
            zmat.remove( atomidx_to_zmat.value(atomidx) );
            need_reindex = true;
        }
    }
    
    if (need_reindex)
        this->reindex();
}

/** Remove the z-matrix line involving the specified atom, bond,
    angle and dihedral. This matches only a single line, and
    will do nothing if this line is not in this z-matrix
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
void ZMatrix::remove(const AtomID &atom, const AtomID &bond,
                     const AtomID &angle, const AtomID &dihedral)
{
    AtomIdx atm = molinfo.constData()->atomIdx(atom);
    AtomIdx bnd = molinfo.constData()->atomIdx(bond);
    AtomIdx ang = molinfo.constData()->atomIdx(angle);
    AtomIdx dih = molinfo.constData()->atomIdx(dihedral);
    
    if (atomidx_to_zmat.contains(atm))
    {
        const ZMatrixLine &line = zmat.at( atomidx_to_zmat.value(atm) );
        
        if (line.bond() == bnd and line.angle() == ang and
            line.dihedral() == dih)
        {
            zmat.remove( atomidx_to_zmat.value(atm) );
            this->reindex();
        }
    }
}

/** Add the z-matrix line 'zmatline'.

    An exception is raised if adding this line would lead to 
    a circular reference
    
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
void ZMatrix::add(const ZMatrixLine &zmatline)
{
    this->add( zmatline.atom(), zmatline.bond(),
               zmatline.angle(), zmatline.dihedral() );
               
    int i = atomidx_to_zmat.value( molinfo->atomIdx(zmatline.atom()) );
    
    zmat[i].setBondDelta( zmatline.bondDelta() );
    zmat[i].setAngleDelta( zmatline.angleDelta() );
    zmat[i].setDihedralDelta( zmatline.dihedralDelta() );
}

/** Remove the z-matrix line 'zmatline' from this z-matrix */
void ZMatrix::remove(const ZMatrixLine &zmatline)
{
    this->remove( zmatline.atom(), zmatline.bond(),
                  zmatline.angle(), zmatline.dihedral() );
}
    
    void setBondDelta(const AtomID &atom, const Length &delta);
    void setAngleDelta(const AtomID &atom, const Angle &delta);
    void setDihedralDelta(const AtomID &atom, const Angle &delta);
    
    void setBondDelta(const AtomID &atom, const AtomID &bond
                      const Length &delta);
                      
    void setAngleDelta(const AtomID &atom, const AtomID &bond,
                       const AtomID &angle, const Angle &delta);

    void setDihedralDelta(const AtomID &atom, const AtomID &bond,
                          const AtomID &angle, const AtomID &dihedral,
                          const Angle &delta);

/** Return whether or not this z-matrix is compatible with the 
    the molecule whose info is in 'molinfo' */
bool ZMatrix::isCompatibleWith(const SireMol::MoleculeInfoData &info) const
{
    return molinfo->UID() == info.UID();
}

//////////
////////// Implementation of ZMatrixCoords
//////////

static const RegisterMetaType<ZMatrixCoords> r_zmatcoords;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const ZMatrixCoords &zmatcoords)
{
    writeHeader(ds, r_zmatcoords, 1);
    
    SharedDataStream sds(ds);
    
    sds << zmatcoords.zmat << zmatcoords.internal_coords
        << zmatcoords.cartesian_coords
        << static_cast<const MoleculeProperty&>(zmatcoords);

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, ZMatrixCoords &zmatcoords)
{
    VersionID v = readHeader(ds, r_zmatline);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> zmatcoords.zmat >> zmatcoords.internal_coords
            >> zmatcoords.cartesian_coords
            >> static_cast<MoleculeProperty&>(zmatcoords);
    }
    else
        throw version_error( v, "1", r_zmatcoords, CODELOC );
        
    return ds;
}

/** Null constructor */
ZMatrixCoords::ZMatrixCoords()
              : ConcreteProperty<ZMatrixCoords,MoleculeProperty>(),
                need_rebuild(false)
{}

/** Construct the z-matrix for the molecule 'molecule' using
    the passed property map to find the coordinates property */
ZMatrixCoords::ZMatrixCoords(const Molecule &molecule, const PropertyMap &map)
              : ConcreteProperty<ZMatrixCoords,MoleculeProperty>(),
                need_rebuild(false)
{
    cartesian_coords = molecule.property( map["coordinates"] )
                               .asA<AtomCoords>();
}

/** Internal function used to calculate the internal coordinates
    of the passed z-matrix line */
Vector ZMatrixCoords::getInternalCoords(const ZMatrixLine &line) const
{
    Vector atom = cartesian_coords[info().cgAtomIdx(line.atom())];
    Vector bond = cartesian_coords[info().cgAtomIdx(line.bond())];
    Vector angle = cartesian_coords[info().cgAtomIdx(line.angle())];
    Vector dihedral = cartesian_coords[info().cgAtomIdx(line.dihedral())];
    
    return Vector( Vector::distance(atom,bond),
                   Vector::angle(atom,bond,angle).value(),
                   Vector::dihedral(atom,bond,angle,dihedral).value() );
}

/** Internal function used to rebuild the internal coordinates
    from the cartesian coordinates */
void ZMatrixCoords::rebuildInternals()
{
    if (zmat.lines().isEmpty())
    {
        internal_coords.clear();
        return;
    }
    
    this->rebuildCartesian();
    
    int nlines = zmat.lines().count();
    
    const ZMatrixLine *lines_array = zmat.lines().constData();
    
    internal_coords = QVector<Vector>(nlines);
    internal_coords.squeeze();
    
    Vector *internal_coords_array = internal_coords.data();
    
    for (int i=0; i<nlines; ++i)
    {
        internal_coords_array[i] = this->getInternalCoords(lines_array[i]);
    }
}

/** Construct the z-matrix for the passed coordinates and z-matrix */              
ZMatrixCoords::ZMatrixCoords(const ZMatrix &zmatrix, const AtomCoords &coords)
              : ConcreteProperty<ZMatrixCoords,MoleculeProperty>(),
                zmat(zmatrix), cartesian_coords(coords), need_rebuild(false)
{
    coords.assertCompatibleWith( zmat.info() );

    //calculate the internal coordinates
    this->rebuildInternals();
}
              
/** Construct the z-matrix for the molecule 'molecule' using the 
    z-matrix in 'zmatrix' and using the passed property map to
    find the coordinates */
ZMatrixCoords::ZMatrixCoords(const ZMatrix &zmatrix, const Molecule &molecule,
                             const PropertyMap &map)
              : ConcreteProperty<ZMatrixCoords,MoleculeProperty>()
{
    this->operator=( ZMatrixCoords(zmatrix, molecule.property( map["coordinates"] )
                                                    .asA<AtomCoords>()) );
}
              
/** Copy constructor */
ZMatrixCoords::ZMatrixCoords(const ZMatrixCoords &other)
              : ConcreteProperty<ZMatrixCoords,MoleculeProperty>(other),
                zmat(other.zmat), internal_coords(other.internal_coords),
                cartesian_coords(other.cartesian_coords), 
                need_rebuild(other.need_rebuild)
{}

/** Destructor */
ZMatrixCoords::~ZMatrixCoords()
{}

/** Copy assignment operator */
ZMatrixCoords& ZMatrixCoords::operator=(const ZMatrixCoords &other)
{
    if (this != &other)
    {
        MoleculeProperty::operator=(other);
        zmat = other.zmat;
        internal_coords = other.internal_coords;
        cartesian_coords = other.cartesian_coords;
        need_rebuild = other.need_rebuild;
    }
    
    return *this;
}

/** Comparison operator */
bool ZMatrixCoords::operator==(const ZMatrixCoords &other) const
{
    return this == &other or
           zmat == other.zmat and internal_coords == other.internal_coords and
           cartesian_coords == other.cartesian_coords and
           need_rebuild == other.need_rebuild;
}

/** Comparison operator */
bool ZMatrixCoords::operator!=(const ZMatrixCoords &other) const
{
    return not this->operator==(other);
}

Q_GLOBAL_STATIC( QMutex, zmatrixMutex );

/** Internal function called to rebuild the cartesian coordinates */
void ZMatrixCoords::_pvt_rebuildCartesian()
{
    int nlines = zmat.lines().count();
    
    const ZMatrixLine *lines_array = zmat.lines().constData();
    const Vector *internal_coords_array = internal_coords.constData();

    AtomCoords new_coords = cartesian_coords;

    for (int i=0; i<nlines; ++i)
    {
        const ZMatrixLine &line = lines_array[i];
        const Vector &internal = internal_coords_array[i];
    
        //get the coordinates of the bond, angle and dihedral atoms
        Vector bond = new_coords[ info().cgAtomIdx(line.bond()) ];
        Vector angle = new_coords[ info().cgAtomIdx(line.angle()) ];
        Vector dihedral = new_coords[ info().cgAtomIdx(line.dihedral()) ];
        
        //now use these to build the coordinates of the atom
        new_coords.set( info().cgAtomIdx(line.atom()),
                        Vector::generate(internal[0], bond,
                                         Angle(internal[1]), angle,
                                         Angle(internal[2]), dihedral) );
    }
}

/** Internal function called to rebuild the cartesian coordinates
    from the internal coordinates */
void ZMatrixCoords::rebuildCartesian() const
{
    //need a mutex as we have declared this as a const function
    //and this breaks implicit sharing
    QMutexLocker lkr( zmatrixMutex() );
    
    if (not need_rebuild)
        return;
        
    const_cast<ZMatrixCoords*>(this)->_pvt_rebuildCartesian();
}

/** Return the z-matrix line for the atom identified by 'atom'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
ZMatrixCoordsLine ZMatrixCoords::operator[](const AtomID &atom) const
{
    AtomIdx idx = zmat.info().atomIdx(atom);
    
    if (not zmat.index().contains(idx))
        throw SireMove::zmatrix_error( QObject::tr(
            "The atom %1 does not appear in the z-matrix.")
                .arg(atom.toString()), CODELOC );
    
    const Vector &coords = internal_coords.at( zmat.index().value(idx) );
    
    return ZMatrixCoordsLine( zmat[idx], Length(coords[0]), 
                              Angle(coords[1]), Angle(coords[2]) );
}

/** Return the z-matrix line for the atom identified by 'atom'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
ZMatrixCoordsLine ZMatrixCoords::at(const AtomID &atom) const
{
    return this->operator[](atom);
}

/** Return all of the lines of the z-matrix */
QVector<ZMatrixCoordsLine> ZMatrixCoords::lines() const
{
    if (zmat.lines().isEmpty())
        return QVector<ZMatrixCoordsLine>();
        
    QVector<ZMatrixCoordsLine> coords( zmat.lines().count() );
    
    const ZMatrixLine *zmat_array = zmat.lines().constData();
    const Vector *internal_coords_array = internal_coords.constData();
    ZMatrixCoordsLine *coords_array = coords.data();
    
    for (int i=0; i<zmat.lines().count(); ++i)
    {
        const Vector &c = internal_coords_array[i];
        
        coords_array[i] = ZMatrixCoordsLine( zmat_array[i], Length(c[0]), 
                                             Angle(c[1]), Angle(c[2]) );
    }
    
    return coords;
}

/** Return the index of AtomIdx to z-matrix line number. This
    is used to index the output of ZMatrixCoords::lines() */
const QHash<AtomIdx,int>& ZMatrixCoords::index() const
{
    return zmat.index();
}

/** Return the layout of the molecule whose z-matrix is contained
    in this object */
const MoleculeInfoData& ZMatrixCoords::info() const
{
    return zmat.info();
}

/** Return the raw z-matrix, which does not contain any
    coordinate information */
const ZMatrix& ZMatrixCoords::zmatrix() const
{
    return zmat;
}

/** Return a string representation of this z-matrix */
QString ZMatrixCoords::toString() const
{
    QStringList output;
    
    int nats = info().nAtoms();
    
    output.append( QObject::tr("ZMatrix nAtoms() == %1").arg(nats) );

    QVector<ZMatrixCoordsLine> zmatrix = this->lines();
    
    for (AtomIdx i(0); i<nats; ++i)
    {
        if (zmat.index().contains(i))
        {
            const ZMatrixCoordsLine &line = zmatrix.at(zmat.index().value(i));

            output.append( QObject::tr("%1-%2-%3-%4 - %5 A : %6° : %7°")
                            .arg(line.atom().value())
                            .arg(line.bond().value())
                            .arg(line.angle().value())
                            .arg(line.dihedral().value())
                            .arg(line.bondLength())
                            .arg( SireUnits::convertTo(line.angleSize(),degrees) )
                            .arg( SireUnits::convertTo(line.dihedralSize(),degrees) ) );
        }
        else
        {
            output.append( QObject::tr("%1 - %2")
                                .arg(i)
                                .arg(cartesian_coords[info().cgAtomIdx(i)].toString()) );
        }
    }
    
    return output.join("\n");
}

/** Return whether or not the z-matrix contains an atom 
    identified by 'atom'
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
bool ZMatrixCoords::contains(const AtomID &atom) const
{
    return zmat.contains(atom);
}

/** Internal function used to add a single set of internal
    coordinates to the z-matrix */
void ZMatrixCoords::addInternal(const AtomIdx &atom)
{
    if (internal_coords.count() != zmat.index().count())
    {
        //this has added the coordinates
        if (internal_coords.count() != zmat.index().count() - 1)
            //more than one line has been added???
            throw SireError::program_bug( QObject::tr(
                "How can we have %1 lines of zmatrix and %2 lines of coordinates?")
                    .arg(zmat.index().count()).arg(internal_coords.count()),
                        CODELOC );
    
        //add space for the coordinates
        if (not zmat.index().contains(atom))
            throw SireError::program_bug( QObject::tr(
                "What's happened now? The index doesn't contain the atom %1.")
                    .arg(atom.toString()), CODELOC );
                    
        internal_coords.insert( zmat.index().value(atom), 1, Vector() );
    }

    if (not zmat.index().contains(atom))
        throw SireError::program_bug( QObject::tr(
             "What's happened here? The index doesn't contain the atom %1.")
                 .arg(atom.toString()), CODELOC );
    
    //now calculate the coordinates
    int idx = zmat.index().value(atom);
    
    const ZMatrixLine &line = zmat.lines().at(idx);
    
    this->rebuildCartesian();
    
    internal_coords[idx] = this->getInternalCoords(line);
}

/** Add the z-matrix line containing the passed atoms. The internal
    coordinates of this line are set from the current cartesian coordinates

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index

    \throw SireMove::zmatrix_error
*/
void ZMatrixCoords::add(const AtomID &atom, const AtomID &bond, 
                        const AtomID &angle, const AtomID &dihedral)
{
    ZMatrix old_zmat = zmat;
    
    try
    {
        zmat.add(atom, bond, angle, dihedral);
        this->addInternal( info().atomIdx(atom) );
    }
    catch(...)
    {
        zmat = old_zmat;
        throw;
    }
}

/** Add the z-matrix line using the supplied atoms and internal coordinates

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
void ZMatrixCoords::add(const AtomID &atom,
                        const Length &bondlength, const AtomID &bond,
                        const Angle &anglesize, const AtomID &angle,
                        const Angle &dihedralsize, const AtomID &dihedral)
{
    ZMatrix old_zmat = zmat;
    
    try
    {
        zmat.add(atom, bond, angle, dihedral);
        
        AtomIdx atomidx = info().atomIdx(atom);
        
        this->addInternal(atomidx);
        
        BOOST_ASSERT( zmat.index().contains(atomidx) );
        
        int idx = zmat.index().value(atomidx);
        
        internal_coords[idx] = Vector( bondlength.value(), anglesize.value(),
                                       dihedralsize.value() );
                                       
        need_rebuild = true;
    }
    catch(...)
    {
        zmat = old_zmat;
        throw;
    }
}

void ZMatrixCoords::remove(const AtomID &atom);
void ZMatrixCoords::remove(const AtomID &atom, const AtomID &bond,
            const AtomID &angle, const AtomID &dihedral);

/** Add the z-matrix line 'zmatline'

    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
void ZMatrixCoords::add(const ZMatrixLine &zmatline)
{
    ZMatrix old_zmat = zmat;

    try
    {
        zmat.add(zmatline);
        this->addInternal(zmatline.atom());
    }
    catch(...)
    {
        zmat = old_zmat;
        throw;
    }
}

void ZMatrixCoords::remove(const ZMatrixLine &zmatline);

/** Add the z-matrix line 'zmatline' to this z-matrix, adding both
    the atoms and also setting the values of the internal coordinates
    
    \throw SireError::invalid_index
    \throw SireMove::zmatrix_error
*/
void ZMatrixCoords::add(const ZMatrixCoordsLine &zmatline)
{
    ZMatrix old_zmat = zmat;
    
    try
    {
        zmat.add(zmatline);
        
        this->addInternal(zmatline.atom());
        
        BOOST_ASSERT( zmat.index().contains(zmatline.atom()) );
        
        int idx = zmat.index().value(zmatline.atom());
        
        internal_coords[idx] = Vector( zmatline.bondLength().value(), 
                                       zmatline.angleSize().value(),
                                       zmatline.dihedralSize().value() );
                                       
        need_rebuild = true;
    }
    catch(...)
    {
        zmat = old_zmat;
        throw;
    }
}

bool ZMatrixCoords::isCompatibleWith(const SireMol::MoleculeInfoData &molinfo) const;

const AtomCoords& ZMatrixCoords::toCartesian() const;
    
    void moveBond(const AtomID &atom, const Length &delta);
    void moveAngle(const AtomID &atom, const Angle &delta);
    void moveDihedral(const AtomID &atom, const Angle &delta);
    
    void moveBond(const AtomID &atom0, const AtomID &atom1, 
                  Length &delta);
                  
    void moveAngle(const AtomID &atom0, const AtomID &atom1,
                   const AtomID &atom2, const Angle &delta);
   
    void moveDihedral(const AtomID &atom0, const AtomID &atom1,
                      const AtomID &atom2, const AtomID &atom3,
                      const Angle &delta);

    void setBond(const AtomID &atom, const Length &length);
    void setAngle(const AtomID &atom, const Angle &size);
    void setDihedral(const AtomID &atom, const Angle &size);
                      
    void setBond(const AtomID &atom0, const AtomID &atom1, 
                 const Length &length);
                  
    void setAngle(const AtomID &atom0, const AtomID &atom1,
                  const AtomID &atom2, const Angle &size);
   
    void setDihedral(const AtomID &atom0, const AtomID &atom1,
                     const AtomID &atom2, const AtomID &atom3,
                     const Angle &size);
    
    void setBondDelta(const AtomID &atom, const Length &delta);
    void setAngleDelta(const AtomID &atom, const Angle &delta);
    void setDihedralDelta(const AtomID &atom, const Angle &delta);
    
    void setBondDelta(const AtomID &atom, const AtomID &bond
                      const Length &delta);
                      
    void setAngleDelta(const AtomID &atom, const AtomID &bond,
                       const AtomID &angle, const Angle &delta);

    void setDihedralDelta(const AtomID &atom, const AtomID &bond,
                          const AtomID &angle, const AtomID &dihedral,
                          const Angle &delta);
