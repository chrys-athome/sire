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

#include "SireMol/qhash_siremol.h"

#include "moleculemover.h"
#include "moleculeview_inlines.h"

#include "cgatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "atomidgroup.h"

#include "bond.h"
#include "angle.h"
#include "dihedral.h"

#include "splitmolecule.h"
#include "weightfunction.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireVol/coordgroup.h"
#include "SireVol/space.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireVol;
using namespace SireStream;

#warning MoleculeMover current moves the entire molecule! It needs work \
         to ensure that it will only move the selected atoms!!!

static const RegisterMetaType<MoleculeMover> r_molmover;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeMover &molmover)
{
    writeHeader(ds, r_molmover, 1)
          << static_cast<const MolDataView&>(molmover);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeMover &molmover)
{
    VersionID v = readHeader(ds, r_molmover);

    if (v == 1)
    {
        ds >> static_cast<MolDataView&>(molmover);
    }
    else
        throw version_error(v, "1", r_molmover, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeMover::MoleculeMover() : MolDataView()
{}

/** Construct the mover to move atoms from 'molecule' */
MoleculeMover::MoleculeMover(const MoleculeView &molecule)
             : MolDataView(molecule)
{}

/** Construct the mover to move atoms from the 'selection'
    within the molecule 'molecule'

    \throw SireError::incompatible_error
*/
MoleculeMover::MoleculeMover(const MoleculeView &molecule,
                             const SelectionFromMol &selection)
              : MolDataView(molecule)
{
    _pvt_selection() = selectedAtoms().intersect(selection);
}

/** Copy constructor */
MoleculeMover::MoleculeMover(const MoleculeMover &other)
             : MolDataView(other)
{}

/** Copy assignment operator */
MoleculeMover& MoleculeMover::operator=(const MoleculeMover &other)
{
    MolDataView::operator=(other);
    return *this;
}

/** Destructor */
MoleculeMover::~MoleculeMover()
{}

/** Map the whole of this molecule into the Space 'space'. This move
    is unusual in that it will move the entire molecule, regardless of
    how much is currently selected */
MoleculeMover MoleculeMover::mapInto(const Space &space) const
{
    //get the CoordGroups containing the current coordinates
    QVector<CoordGroup> coords = data().coordGroups();

    //map them all into the space
    QVector<CoordGroup> mapped_coords = space.moveToCenterBox(coords);

    //has anything changed?
    if (coords.constData() == mapped_coords.constData())
        //no - the arrays are the same
        return *this;
    else
    {
        //at least one of the CutGroups may have changed

        //update the molecule
        MoleculeMover newmol = *this;
        newmol.data().setCoordinates(mapped_coords);

        return newmol;
    }
}

/** Translate all of the molecule by 'delta' */
MoleculeMover MoleculeMover::translate(const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(delta);
    return ret;
}

/** Translate the group of atoms 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::translate(const AtomIDGroup &group,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(group, delta);
    return ret;
}

/** Translate the atom 'atom' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::translate(const AtomIndex &atom,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(atom, delta);
    return ret;
}

/** Translate the set of atoms in 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::translate(const QSet<AtomIndex> &atoms,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(atoms, delta);
    return ret;
}

/** Translate the atoms in the residue with number 'resnum' whose names are
    in 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::translate(ResNum resnum, const QStringList &atoms,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resnum, atoms, delta);
    return ret;
}

/** Translate the residue with number 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::translate(ResNum resnum, const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resnum, delta);
    return ret;
}

/** Translate the set of residues whose residue numbers are in 'resnums'
    by 'delta'

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::translate(const QSet<ResNum> &resnums,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resnums, delta);
    return ret;
}

/** Translate the atoms in the residue at index 'resid' whose names are in
    'atoms' by 'delta'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::translate(ResID resid, const QStringList &atoms,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resid, atoms, delta);
    return ret;
}

/** Translate the residue at index 'resid' by 'delta'

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::translate(ResID resid, const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resid, delta);
    return ret;
}

/** Translate the set of residues whose indicies are in 'resids' by 'delta'

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::translate(const QSet<ResID> &resids,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(resids, delta);
    return ret;
}

/** Translate all of the atoms in the CutGroup with ID == cgid by 'delta'

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::translate(CutGroupID cgid, const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(cgid, delta);
    return ret;
}

/** Translate all of the atoms in the CutGroups with IDs in 'cgids' by 'delta'

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::translate(const QSet<CutGroupID> &cgids,
                                       const Vector &delta) const
{
    MoleculeMover ret = *this;
    ret.data().translate(cgids, delta);
    return ret;
}

/** Rotate the whole molecule using the quaternion 'quat' about the point 'point' */
MoleculeMover MoleculeMover::rotate(const Quaternion &quat, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(quat, point);
    return ret;
}

/** Rotate the atoms in the group 'group' using the quaternion 'quat' about the
    point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const AtomIDGroup &group, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(group, quat, point);
    return ret;
}

/** Rotate the atom 'atom' using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const AtomIndex &atom, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(atom, quat, point);
    return ret;
}

/** Rotate the atoms in 'atoms' using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(atoms, quat, point);
    return ret;
}

/** Rotate the atoms in the residue with number 'resnum' whose names are in 'atoms'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(ResNum resnum, const QStringList &atoms,
                                    const Quaternion &quat, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnum, atoms, quat, point);
    return ret;
}

/** Rotate the atoms in the residue with number 'resnum' using the quaternion
    'quat' about the point 'point'.

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::rotate(ResNum resnum, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnum, quat, point);
    return ret;
}

/** Rotate the atoms in the residues whose numbers are in 'resnums'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::rotate(const QSet<ResNum> &resnums, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnums, quat, point);
    return ret;
}

/** Rotate the atoms in the residue at index 'resid' whose names are in 'atoms'
    using the quaternion 'quat' about the point 'point'.

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(ResID resid, const QStringList &atoms,
                                    const Quaternion &quat, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resid, atoms, quat, point);
    return ret;
}

/** Rotate the atoms in the residue at index 'resid' using the quaternion 'quat'
    about the point 'point'.

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::rotate(ResID resid, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resid, quat, point);
    return ret;
}

/** Rotate the atoms in the residues whose indicies are in 'resids'
    using the quaternion 'quat' about the point 'point'.

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::rotate(const QSet<ResID> &resids, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resids, quat, point);
    return ret;
}

/** Rotate the atoms in the CutGroup with ID == cgid using the quaternion 'quat'
    about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::rotate(CutGroupID cgid, const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(cgid, quat, point);
    return ret;
}

/** Rotate the atoms in the CutGroups whose IDs are in 'cgids'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::rotate(const QSet<CutGroupID> &cgids,
                                    const Quaternion &quat,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(cgids, quat, point);
    return ret;
}

/** Rotate the whole molecule using the matrix 'matrix' about the point 'point' */
MoleculeMover MoleculeMover::rotate(const Matrix &matrix, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(matrix, point);
    return ret;
}

/** Rotate the atoms in the group 'group' using the matrix 'matrix' about the
    point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const AtomIDGroup &group, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(group, matrix, point);
    return ret;
}

/** Rotate the atom 'atom' using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const AtomIndex &atom, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(atom, matrix, point);
    return ret;
}

/** Rotate the atoms in 'atoms' using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(atoms, matrix, point);
    return ret;
}

/** Rotate the atoms in the residue with number 'resnum' whose names are in 'atoms'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(ResNum resnum, const QStringList &atoms,
                                    const Matrix &matrix, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnum, atoms, matrix, point);
    return ret;
}

/** Rotate the atoms in the residue with number 'resnum' using the matrixernion
    'matrix' about the point 'point'.

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::rotate(ResNum resnum, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnum, matrix, point);
    return ret;
}

/** Rotate the atoms in the residues whose numbers are in 'resnums'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resnums, matrix, point);
    return ret;
}

/** Rotate the atoms in the residue at index 'resid' whose names are in 'atoms'
    using the matrix 'matrix' about the point 'point'.

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::rotate(ResID resid, const QStringList &atoms,
                                    const Matrix &matrix, const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resid, atoms, matrix, point);
    return ret;
}

/** Rotate the atoms in the residue at index 'resid' using the matrix 'matrix'
    about the point 'point'.

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::rotate(ResID resid, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resid, matrix, point);
    return ret;
}

/** Rotate the atoms in the residues whose indicies are in 'resids'
    using the matrix 'matrix' about the point 'point'.

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::rotate(const QSet<ResID> &resids, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(resids, matrix, point);
    return ret;
}

/** Rotate the atoms in the CutGroup with ID == cgid using the matrix 'matrix'
    about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::rotate(CutGroupID cgid, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(cgid, matrix, point);
    return ret;
}

/** Rotate the atoms in the CutGroups whose IDs are in 'cgids'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix,
                                    const Vector &point) const
{
    MoleculeMover ret = *this;
    ret.data().rotate(cgids, matrix, point);
    return ret;
}

/** Simultaneously translate this molecule by 'delta', and rotate it about the
    point 'point'. The molecule is translated *after* it has been rotated! */
MoleculeMover MoleculeMover::translateAndRotate(const Vector &delta,
                                                const Quaternion &quat,
                                                const Vector &point) const
{
    MoleculeMover ret = *this;

    ret.data().rotate(quat,point);
    ret.data().translate(delta);

    return ret;
}

/** Set the coordinates of the atoms in the CutGroup with ID == cgid to
    'newcoords'.

    \warning 'newcoords' must have the same number of points as there
             are atoms in the CutGroup or else an exception will be thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::setCoordinates(CutGroupID cgid,
                                            const CoordGroup &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(cgid, newcoords);
    return ret;
}

/** Set the coordinates of lots of CutGroups at once, with the new
    coordinates in 'newcoords', indexed by the ID of the CutGroup
    for which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::setCoordinates(
                        const QHash<CutGroupID,CoordGroup> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates from the array of CoordGroups - these must be
    in the same order as they are in the molecule, and must have the
    same number of points in each CoordGroup as there are atoms in each
    CutGroup

    \throw SireError::incompatible_error
*/
MoleculeMover MoleculeMover::setCoordinates(const QVector<CoordGroup> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of all of the atoms in the whole molecule to the
    the coordinates stored in the array 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the molecule or else an exception will
             be thrown.

    \throw SireError::incompatible_error
*/
MoleculeMover MoleculeMover::setCoordinates(const QVector<Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the CutGroup with ID == cgid to the coordinates
    stored in the array 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the CutGroup or else an exception will
             be thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::setCoordinates(CutGroupID cgid,
                                            const QVector<Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(cgid, newcoords);
    return ret;
}

/** Set the coordinates for many CutGroups, using the arrays of coordinates
    stored in 'newcoords', indexed by the ID numbers of the CutGroups for
    which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
MoleculeMover MoleculeMover::setCoordinates(
                const QHash< CutGroupID,QVector<Vector> > &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the residue 'resnum' using the array of coordinates
    in 'newcoords'. The coordinates should be in the same order as the atoms
    are in the residue.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the residue or else an exception will be
             thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::setCoordinates(ResNum resnum,
                                            const QVector<Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(resnum, newcoords);
    return ret;
}

/** Set the coordinates of a large number of residues, using the arrays
    of coordinates in 'newcoords' indexed by the residue number of the
    residue for which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
MoleculeMover MoleculeMover::setCoordinates(
                      const QHash< ResNum,QVector<Vector> > &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the residue at index 'resid' using the array of
    coordinates 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the residue or else an exception will be
             thrown.

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(ResID resid,
                                            const QVector<Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(resid, newcoords);
    return ret;
}

/** Set the coordinates of many residues, with the new coordinates stored
    as arrays indexed by the index (ResID) of the residue for which they
    are updating.

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(
                      const QHash< ResID,QVector<Vector> > &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the individual atom 'atom' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::setCoordinates(const AtomIndex &atom,
                                            const Vector &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(atom, newcoords);
    return ret;
}

/** Set the coordinates of many atoms to 'newcoords', with the new coordinates
    indexed by the AtomIndex of the atoms that they are updating.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
MoleculeMover MoleculeMover::setCoordinates(
                      const QHash<AtomIndex,Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the atom at index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(const CGAtomID &cgatomid,
                                            const Vector &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(cgatomid, newcoords);
    return ret;
}

/** Set the coordinates of many atoms, with the new coordinates indexed
    by the CGAtomID of the atoms that they are updating.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(
                            const QHash<CGAtomID,Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the atom with index 'resatomid' to 'newcoords'.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(const ResNumAtomID &resatomid,
                                            const Vector &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(resatomid, newcoords);
    return ret;
}

/** Set the coordinates of many atoms, with the new coordinates indexed
    by the ResNumAtomID of the atoms that they are updating.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(
                          const QHash<ResNumAtomID,Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(const ResIDAtomID &resatomid,
                                            const Vector &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(resatomid, newcoords);
    return ret;
}

/** Set the coordinates for many atoms, with the new coordinates indexed
    by the ResIDAtomID of the atoms that they are updating.

    \throw SireError::invalid_index
*/
MoleculeMover MoleculeMover::setCoordinates(
                              const QHash<ResIDAtomID,Vector> &newcoords) const
{
    MoleculeMover ret = *this;
    ret.data().setCoordinates(newcoords);
    return ret;
}

/** Change the length of the bond 'bnd' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used
    to control how the two parts of the residue that are moved (the two sides of
    the moving bond) are weighted when they are moved.

    This will throw an exception if either the atoms in the bond are not present
    in this molecule, if both sides of the bond are anchored, or if the bond
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
MoleculeMover MoleculeMover::change(const Bond &bnd, double delta,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    //Split the molecule into two groups based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd,
                                                        data().connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    MoleculeMover ret = *this;

    ret.data().change(bnd, delta, group0, group1, weightfunc, anchors);

    return ret;
}

/** Overloaded function that changes the bond length using the RelFromMass weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::change(const Bond &bond, double delta,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(bond, delta, RelFromMass(), anchors);
}

/** Change the size of the angle 'ang' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    angle) are weighted when they are moved.

    This will throw an exception if either the atoms in the angle are not present
    in this molecule, if both sides of the angle are anchored, or if the angle
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
MoleculeMover MoleculeMover::change(const SireMol::Angle &ang,
                                    const SireMaths::Angle &delta,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    //split the molecule into two parts based on the angle
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(ang,
                                                      data().connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    MoleculeMover ret = *this;

    ret.data().change(ang, delta, group0, group1, weightfunc, anchors);

    return ret;
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::change(const SireMol::Angle &ang,
                                    const SireMaths::Angle &delta,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(ang, delta, RelFromMass(), anchors);
}

/** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    dihedral) are weighted when they are moved.

    This will throw an exception if either the atoms in the dihedral are not present
    in this molecule, if both sides of the angle are anchored, or if the dihedral
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
MoleculeMover MoleculeMover::change(const Dihedral &dih, const SireMaths::Angle &delta,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    //split the molecule into two parts based on the dihedral
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(dih, data().connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    MoleculeMover ret = *this;

    ret.data().change(Bond(dih.atom1(),dih.atom2()),
                      delta, group0, group1, weightfunc, anchors);

    return ret;
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::change(const Dihedral &dih, const SireMaths::Angle &delta,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(dih,delta,RelFromMass(),anchors);
}

/** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    dihedral) are weighted when they are moved. This moves all atoms that are either side
    of the dihedral.

    This will throw an exception if either the atoms in the dihedral are not present
    in this molecule, if both sides of the angle are anchored, or if the dihedral
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::change(const Bond &bnd, const SireMaths::Angle &delta,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    //split the molecule into two parts based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, data().connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    MoleculeMover ret = *this;

    ret.data().change(bnd, delta, group0, group1, weightfunc, anchors);

    return ret;
}

/** Overload of the function that uses a default WeightFunction */
MoleculeMover MoleculeMover::change(const Bond &dih, const SireMaths::Angle &delta,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(dih, delta, RelFromMass(), anchors);
}

/** Change the size of the improper angle 'improper' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    improper angle) are weighted when they are moved.

    This will throw an exception if either the atoms in the improper are not present
    in this molecule, if both sides of the angle are anchored, or if the improper
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
MoleculeMover MoleculeMover::change(const Improper &improper,
                                    const SireMaths::Angle &delta,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    //split the molecule into two parts based on the improper
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(improper, data().connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    MoleculeMover ret = *this;

    ret.data().change(improper, delta, group0, group1, weightfunc, anchors);

    return ret;
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::change(const Improper &improper,
                                    const SireMaths::Angle &delta,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(improper, delta, RelFromMass(), anchors);
}

/** This function is similar to MoleculeMover::changeBond, except that it sets the specified
    bond to a particular length

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Bond &bnd, double val,
                                 const WeightFunction &weightfunc,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(bnd, val-data().measure(bnd), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Bond &bnd, double val,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(bnd, val-data().measure(bnd), anchors);
}

/** This function is similar to MoleculeMover::changeAngle, except that it set the specified angle
    to a particular size. Note that this will probably change the size of other angles, so
    if you wish to set a whole series of angles, then you will need to think carefully about
    the order in which you set them.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const SireMol::Angle &ang,
                                 const SireMaths::Angle &val,
                                 const WeightFunction &weightfunc,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(ang, val-data().measure(ang), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const SireMol::Angle &ang,
                                 const SireMaths::Angle &val,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(ang, val-data().measure(ang), anchors);
}

/** This function is similar to MoleculeMover::changeDihedral, except that it sets the specified
    dihedral to a particular size. Note that this will probably change the size of other
    dihedrals, so think about the order in which you apply multiple calls of this function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Dihedral &dih,
                                 const SireMaths::Angle &val,
                                 const WeightFunction &weightfunc,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(dih, val-data().measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Dihedral &dih,
                                 const SireMaths::Angle &val,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(dih, val-data().measure(dih), anchors);
}

/** This function is similar to MoleculeMover::change(const Bond&), except that it sets the specified
    dihedral to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::setAll(const Dihedral &dih,
                                    const SireMaths::Angle &val,
                                    const WeightFunction &weightfunc,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(dih, val-data().measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::setAll(const Dihedral &dih,
                                    const SireMaths::Angle &val,
                                    const QSet<AtomIndex> &anchors) const
{
    return change(dih, val-data().measure(dih), anchors);
}

/** This function is similar to MoleculeMover::change(const Improper&), except that it
    sets the specified improper angle to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Improper &improper,
                                 const SireMaths::Angle &size,
                                 const WeightFunction &weightfunc,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(improper, size-data().measure(improper), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
MoleculeMover MoleculeMover::set(const Improper &improper,
                                 const SireMaths::Angle &size,
                                 const QSet<AtomIndex> &anchors) const
{
    return change(improper, size-data().measure(improper), anchors);
}
