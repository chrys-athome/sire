
/**
  * This file contains the boost::python wrapping of a Molecule
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/atomidgroup.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"
#include "SireMol/splitmolecule.h"
#include "SireMol/weightfunction.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireMol
{

void export_Molecule()
{

    class_<Molecule>( "Molecule", init<>() )
    
        .def( init<const Residue&>() )
        .def( init<const Molecule&>() )
        
        .def( "ID", (MoleculeID (Molecule::*)() const)
                        &Molecule::ID )
        .def( "setNewID", (void (Molecule::*)())
                        &Molecule::setNewID )

        .def( "version", (const MoleculeVersion& (Molecule::*)() const)
                        &Molecule::version,
                        return_value_policy<copy_const_reference>() )
    
        .def( self == self )
        .def( self != self )

        .def( "__getitem__", &__getitem__Molecule<CutGroupID,CutGroup> )
        .def( "__getitem__", &__getitem__Molecule<ResID,Residue> )
        .def( "__getitem__", &__getitem__Molecule<ResNum,Residue> )
        .def( "__getitem__", &__getitem__Molecule<AtomID,Atom> )
        .def( "__getitem__", &__getitem__Molecule<const CGAtomID&,Atom> )
        .def( "__getitem__", &__getitem__Molecule<const CGNumAtomID&,Atom> )
        .def( "__getitem__", &__getitem__Molecule<const ResNumAtomID&,Atom> )
        .def( "__getitem__", &__getitem__Molecule<const ResIDAtomID&,Atom> )
        .def( "__getitem__", &__getitem__Molecule<const AtomIndex&,Atom> )
    
        .def( "residue", (Residue (Molecule::*)(ResNum) const)
                        &Molecule::residue )
        .def( "residue", (Residue (Molecule::*)(ResID) const)
                        &Molecule::residue )
        .def( "residue", (Residue (Molecule::*)(const QString&) const)
                        &Molecule::residue )

        .def( "residues", (QHash<ResNum,Residue> (Molecule::*)() const)
                        &Molecule::residues )
        .def( "residues", (QHash<ResNum,Residue> (Molecule::*)(const QSet<ResNum>&) const)
                        &Molecule::residues )

        .def( "at", (Residue (Molecule::*)(ResNum) const)
                        &Molecule::at )
        .def( "at", (Residue (Molecule::*)(ResID) const)
                        &Molecule::at )
    
        .def( "edit", (EditMol (Molecule::*)() const)
                        &Molecule::edit )
    
        .def( "at", (CutGroup (Molecule::*)(CutGroupID) const)
                        &Molecule::at )

        .def( "at", (Atom (Molecule::*)(AtomID) const)
                        &Molecule::at )
        .def( "at", (Atom (Molecule::*)(const ResNumAtomID&) const)
                        &Molecule::at )
        .def( "at", (Atom (Molecule::*)(const ResIDAtomID&) const)
                        &Molecule::at )
        .def( "at", (Atom (Molecule::*)(const CGAtomID&) const)
                        &Molecule::at )
        .def( "at", (Atom (Molecule::*)(const CGNumAtomID&) const)
                        &Molecule::at )
        .def( "at", (Atom (Molecule::*)(const AtomIndex&) const)
                        &Molecule::at )

        .def( "connectivity", (MoleculeBonds (Molecule::*)() const)
                        &Molecule::connectivity )

        .def( "connectivity", (ResidueBonds (Molecule::*)(ResNum) const)
                        &Molecule::connectivity )
        .def( "connectivity", (ResidueBonds (Molecule::*)(ResID) const)
                        &Molecule::connectivity )

        .def( "info", (const MoleculeInfo& (Molecule::*)() const)
                        &Molecule::info,
                        return_value_policy<copy_const_reference>() )

        .def( "cutGroups", (QVector<CutGroup> (Molecule::*)() const)
                        &Molecule::cutGroups )
        .def( "cutGroups", (QHash<CutGroupID,CutGroup> (Molecule::*)(
                                                          ResNum) const)
                        &Molecule::cutGroups )
        .def( "cutGroups", (QHash<CutGroupID,CutGroup> (Molecule::*)(
                                                          ResID) const)
                        &Molecule::cutGroups )

        .def( "cutGroup", (CutGroup (Molecule::*)(CutGroupID) const)
                        &Molecule::cutGroup )
        .def( "cutGroup", (CutGroup (Molecule::*)(CutGroupNum) const)
                        &Molecule::cutGroup )

        .def( "coordGroups", (QVector<CoordGroup> (Molecule::*)() const)
                        &Molecule::coordGroups )
        .def( "coordGroups", (QHash<CutGroupID,CoordGroup> (Molecule::*)(
                                                          ResNum) const)
                        &Molecule::coordGroups )
        .def( "coordGroups", (QHash<CutGroupID,CoordGroup> (Molecule::*)(
                                                          ResID) const)
                        &Molecule::coordGroups )

        .def( "coordGroups", (QHash<CutGroupID,CoordGroup> (Molecule::*)(
                                              const QSet<CutGroupID>&) const)
                        &Molecule::coordGroups )
        .def( "coordGroups", (QHash<CutGroupID,CoordGroup> (Molecule::*)(
                                              const QSet<ResNum>&) const)
                        &Molecule::coordGroups )
        .def( "coordGroups", (QHash<CutGroupID,CoordGroup> (Molecule::*)(
                                              const QSet<ResID>&) const)
                        &Molecule::coordGroups )

        .def( "coordGroup", (CoordGroup (Molecule::*)(CutGroupID) const)
                        &Molecule::coordGroup )
        .def( "coordGroup", (CoordGroup (Molecule::*)(CutGroupNum) const)
                        &Molecule::coordGroup )

        .def( "atom", (Atom (Molecule::*)(AtomID) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(CutGroupID, AtomID) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(const CGAtomID&) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(ResNum, AtomID) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(const ResNumAtomID&) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(ResID, AtomID) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(const ResIDAtomID&) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(const AtomIndex&) const)
                        &Molecule::atom )
        .def( "atom", (Atom (Molecule::*)(ResNum, const QString&) const)
                        &Molecule::atom )

        .def( "coordinates", (Vector (Molecule::*)(AtomID) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(CutGroupID, AtomID) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(const CGAtomID&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(ResNum, AtomID) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(const ResNumAtomID&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(ResID, AtomID) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(const ResIDAtomID&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(const AtomIndex&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (Vector (Molecule::*)(ResNum, const QString&) const)
                        &Molecule::coordinates )

        .def( "atoms", (QVector<Atom> (Molecule::*)() const)
                        &Molecule::atoms )
        .def( "coordinates", (QVector<Vector> (Molecule::*)() const)
                        &Molecule::coordinates )

        .def( "atoms", (QHash<AtomID,Atom> (Molecule::*)(const QSet<AtomID>&) const)
                        &Molecule::atoms )
        .def( "atoms", (QHash<CGAtomID,Atom> (Molecule::*)(const QSet<CGAtomID>&) const)
                        &Molecule::atoms )
        .def( "atoms", (QHash<ResNumAtomID,Atom> (Molecule::*)(
                                              const QSet<ResNumAtomID>&) const)
                        &Molecule::atoms )
        .def( "atoms", (QHash<ResIDAtomID,Atom> (Molecule::*)(
                                              const QSet<ResIDAtomID>&) const)
                        &Molecule::atoms )
        .def( "atoms", (QHash<AtomIndex,Atom> (Molecule::*)(
                                              const QSet<AtomIndex>&) const)
                        &Molecule::atoms )

        .def( "coordinates", (QHash<AtomID,Vector> (Molecule::*)(
                                              const QSet<AtomID>&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (QHash<CGAtomID,Vector> (Molecule::*)(
                                              const QSet<CGAtomID>&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (QHash<ResNumAtomID,Vector> (Molecule::*)(
                                              const QSet<ResNumAtomID>&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (QHash<ResIDAtomID,Vector> (Molecule::*)(
                                              const QSet<ResIDAtomID>&) const)
                        &Molecule::coordinates )
        .def( "coordinates", (QHash<AtomIndex,Vector> (Molecule::*)(
                                              const QSet<AtomIndex>&) const)
                        &Molecule::coordinates )

        .def( "atoms", (QVector<Atom> (Molecule::*)(CutGroupID) const)
                        &Molecule::atoms )
        .def( "CutGroupID,QVector<Atom>", (QHash< (Molecule::*)(> atoms(const QSet<CutGroupID> &cgids) const))
                        &Molecule::CutGroupID,QVector<Atom> )

        .def( "atoms", (QVector<Atom> (Molecule::*)(ResNum resnum) const))
                        &Molecule::atoms )
        .def( "ResNum,QVector<Atom>", (QHash< (Molecule::*)(> atoms(const QSet<ResNum> &resnums) const))
                        &Molecule::ResNum,QVector<Atom> )

        .def( "atoms", (QVector<Atom> (Molecule::*)(ResID resid) const))
                        &Molecule::atoms )
        .def( "ResID,QVector<Atom>", (QHash< (Molecule::*)(> atoms(const QSet<ResID> &resids) const))
                        &Molecule::ResID,QVector<Atom> )

        .def( "coordinates", (QVector<Vector> (Molecule::*)(CutGroupID cgid)))
                        &Molecule::coordinates )
        .def( "CutGroupID,QVector<Vector>", (QHash< (Molecule::*)())
                        &Molecule::CutGroupID,QVector<Vector> )
        .def( "QSet<CutGroupID>", (coordinates(const (Molecule::*)(&cgids) const))
                        &Molecule::QSet<CutGroupID> )

        .def( "coordinates", (QVector<Vector> (Molecule::*)(ResNum resnum)))
                        &Molecule::coordinates )
        .def( "ResNum,QVector<Vector>", (QHash< (Molecule::*)())
                        &Molecule::ResNum,QVector<Vector> )
        .def( "QSet<ResNum>", (coordinates(const (Molecule::*)(&resnums) const))
                        &Molecule::QSet<ResNum> )

        .def( "coordinates", (QVector<Vector> (Molecule::*)(ResID resid)))
                        &Molecule::coordinates )
        .def( "ResID,QVector<Vector>", (QHash< (Molecule::*)())
                        &Molecule::ResID,QVector<Vector> )
        .def( "QSet<ResID>", (coordinates(const (Molecule::*)(&resids) const))
                        &Molecule::QSet<ResID> )

        .def( "name", (QString (Molecule::*)() const))
                        &Molecule::name )

        .def( "residueName", (QString (Molecule::*)(ResNum resnum) const))
                        &Molecule::residueName )
        .def( "residueName", (QString (Molecule::*)(ResID resid) const))
                        &Molecule::residueName )

        .def( "residueNumber", (ResNum (Molecule::*)(ResID resid) const))
                        &Molecule::residueNumber )
        .def( "residueNumber", (ResNum (Molecule::*)(const QString &resname) const))
                        &Molecule::residueNumber )

        .def( "isEmpty", (bool (Molecule::*)() const))
                        &Molecule::isEmpty )
        .def( "isEmpty", (bool (Molecule::*)(ResNum resnum) const))
                        &Molecule::isEmpty )
        .def( "isEmpty", (bool (Molecule::*)(ResID resid) const))
                        &Molecule::isEmpty )
        .def( "isEmpty", (bool (Molecule::*)(CutGroupID cgid) const))
                        &Molecule::isEmpty )

        .def( "residueNumbers", (QVector<ResNum> (Molecule::*)() const))
                        &Molecule::residueNumbers )
        .def( "residueNames", (QStringList (Molecule::*)() const))
                        &Molecule::residueNames )

        .def( "residueNumbers", (QVector<ResNum> (Molecule::*)(const QString &resnam) const))
                        &Molecule::residueNumbers )
        .def( "residueNumbers", (QVector<ResNum> (Molecule::*)(CutGroupID cgid) const))
                        &Molecule::residueNumbers )

        .def( "residuesBondedTo", (QHash<ResNum,Residue> (Molecule::*)(ResNum resnum) const))
                        &Molecule::residuesBondedTo )
        .def( "residuesBondedTo", (QHash<ResNum,Residue> (Molecule::*)(ResID resid) const))
                        &Molecule::residuesBondedTo )

        .def( "contains", (bool (Molecule::*)(CutGroupID cgid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(ResNum resnum) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(ResID resid) const))
                        &Molecule::contains )

        .def( "contains", (bool (Molecule::*)(ResNum resnum, const QString &atomname) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(const AtomIndex &atm) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(CutGroupID cgid, AtomID atomid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(const CGAtomID &cgatomid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(ResNum resnum, AtomID atomid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(const ResNumAtomID &resatomid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(ResID resid, AtomID atomid) const))
                        &Molecule::contains )
        .def( "contains", (bool (Molecule::*)(const ResIDAtomID &resatomid) const))
                        &Molecule::contains )

        .def( "contains", (bool (Molecule::*)(const Bond &bond) const))
                        &Molecule::contains )

        .def( "nAtoms", (int (Molecule::*)() const))
                        &Molecule::nAtoms )
        .def( "nAtoms", (int (Molecule::*)(ResNum resnum) const))
                        &Molecule::nAtoms )
        .def( "nAtoms", (int (Molecule::*)(ResID resid) const))
                        &Molecule::nAtoms )
        .def( "nAtoms", (int (Molecule::*)(CutGroupID id) const))
                        &Molecule::nAtoms )

        .def( "nResidues", (int (Molecule::*)() const))
                        &Molecule::nResidues )

        .def( "nCutGroups", (int (Molecule::*)() const))
                        &Molecule::nCutGroups )

        .def( "nBonds", (int (Molecule::*)() const))
                        &Molecule::nBonds )
        .def( "nBonds", (int (Molecule::*)(ResNum resnum) const))
                        &Molecule::nBonds )
        .def( "nBonds", (int (Molecule::*)(ResID resid) const))
                        &Molecule::nBonds )

        .def( "nInterBonds", (int (Molecule::*)() const))
                        &Molecule::nInterBonds )
        .def( "nInterBonds", (int (Molecule::*)(ResNum resnum) const))
                        &Molecule::nInterBonds )
        .def( "nInterBonds", (int (Molecule::*)(ResID resid) const))
                        &Molecule::nInterBonds )

        .def( "nIntraBonds", (int (Molecule::*)() const))
                        &Molecule::nIntraBonds )
        .def( "nIntraBonds", (int (Molecule::*)(ResNum resnum) const))
                        &Molecule::nIntraBonds )
        .def( "nIntraBonds", (int (Molecule::*)(ResID resid) const))
                        &Molecule::nIntraBonds )

        .def( "atomNames", (QStringList (Molecule::*)(ResNum resnum) const))
                        &Molecule::atomNames )
        .def( "atomNames", (QStringList (Molecule::*)(ResID resid) const))
                        &Molecule::atomNames )

        .def( "bond", (SireMaths::Line (Molecule::*)(const Bond &bnd) const))
                        &Molecule::bond )
        .def( "angle", (SireMaths::Triangle (Molecule::*)(angle(const SireMol::Angle &ang) const))
                        &Molecule::angle )
        .def( "dihedral", (SireMaths::Torsion (Molecule::*)(const Dihedral &dih) const))
                        &Molecule::dihedral )
        .def( "improper", (SireMaths::Torsion (Molecule::*)(const Improper &improper) const))
                        &Molecule::improper )

        .def( "measure", (double (Molecule::*)(const Bond &bnd) const))
                        &Molecule::measure )
        .def( "measure", (SireMaths::Angle (Molecule::*)(const SireMol::Angle &ang) const))
                        &Molecule::measure )
        .def( "measure", (SireMaths::Angle (Molecule::*)(const Dihedral &dih) const))
                        &Molecule::measure )
        .def( "measure", (SireMaths::Angle (Molecule::*)(const Improper &improper) const))
                        &Molecule::measure )

        .def( "getWeight", (double (Molecule::*)(const AtomIDGroup &group0, const AtomIDGroup &group1))
                        &Molecule::getWeight )
        .def( "weightfunc)", (const WeightFunction& (Molecule::*)(const))
                        &Molecule::weightfunc),
                        return_value_policy<copy_const_reference>() )
    
        .def( "translate", (void (Molecule::*)(const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const AtomIDGroup &group, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const AtomIndex &atom, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const QSet<AtomIndex> &atoms, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(ResNum resnum, const QStringList &atoms, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(ResNum resnum, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const QSet<ResNum> &resnums, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(ResID resid, const QStringList &atoms, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(ResID resid, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const QSet<ResID> &resids, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(CutGroupID cgid, const Vector &delta)))
                        &Molecule::translate )
        .def( "translate", (void (Molecule::*)(const QSet<CutGroupID> &cgids, const Vector &delta)))
                        &Molecule::translate )

        .def( "rotate", (void (Molecule::*)(const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const AtomIndex &atom, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResNum resnum, const QStringList &atoms, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResNum resnum, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<ResNum> &resnums, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResID resid, const QStringList &atoms, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResID resid, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<ResID> &resids, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(CutGroupID cgid, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<CutGroupID> &cgids, const Quaternion &quat, const Vector &point)))
                        &Molecule::rotate )

        .def( "rotate", (void (Molecule::*)(const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const AtomIDGroup &group, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const AtomIndex &atom, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResNum resnum, const QStringList &atoms, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResNum resnum, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<ResNum> &resnums, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResID resid, const QStringList &atoms, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(ResID resid, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<ResID> &resids, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(CutGroupID cgid, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )
        .def( "rotate", (void (Molecule::*)(const QSet<CutGroupID> &cgids, const Matrix &matrix, const Vector &point)))
                        &Molecule::rotate )

        .def( "setCoordinates", (void (Molecule::*)(CutGroupID cgid, const CoordGroup &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash<CutGroupID,CoordGroup> &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(const QVector<Vector> &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(CutGroupID cgid, const QVector<Vector> &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash< CutGroupID,QVector<Vector> > &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(ResNum resnum, const QVector<Vector> &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash< ResNum,QVector<Vector> > &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(ResID resid, const QVector<Vector> &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash< ResID,QVector<Vector> > &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(const AtomIndex &atom, const Vector &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash<AtomIndex,Vector> &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(const CGAtomID &cgatomid, const Vector &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash<CGAtomID,Vector> &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(const ResNumAtomID &resatomid, const Vector &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash<ResNumAtomID,Vector> &newcoords)))
                        &Molecule::setCoordinates )

        .def( "setCoordinates", (void (Molecule::*)(const ResIDAtomID &resatomid, const Vector &newcoords)))
                        &Molecule::setCoordinates )
        .def( "setCoordinates", (void (Molecule::*)(const QHash<ResIDAtomID,Vector> &newcoords)))
                        &Molecule::setCoordinates )
    
        .def( "change", (void (Molecule::*)(const Bond &bond, double delta, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )
        .def( "change", (void (Molecule::*)(const Bond &bond, double delta, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )

        .def( "change", (void (Molecule::*)(const Angle &angle, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )
        .def( "change", (void (Molecule::*)(const Angle &angle, const SireMaths::Angle &delta, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )

        .def( "change", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )
        .def( "change", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &delta, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )

        .def( "change", (void (Molecule::*)(const Bond &bond, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )
        .def( "change", (void (Molecule::*)(const Bond &bond, const SireMaths::Angle &delta, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )

        .def( "change", (void (Molecule::*)(const Improper &improper, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )
        .def( "change", (void (Molecule::*)(const Improper &improper, const SireMaths::Angle &delta, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::change )

        .def( "set", (void (Molecule::*)(const Bond &bond, double lgth, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )
        .def( "set", (void (Molecule::*)(const Bond &bond, double lgth, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )

        .def( "set", (void (Molecule::*)(const SireMol::Angle &angle, const SireMaths::Angle &ang, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )
        .def( "set", (void (Molecule::*)(const SireMol::Angle &angle, const SireMaths::Angle &ang, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )

        .def( "set", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &ang, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )
        .def( "set", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &ang, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )

        .def( "setAll", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &ang, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::setAll )
        .def( "setAll", (void (Molecule::*)(const Dihedral &dihedral, const SireMaths::Angle &ang, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::setAll )

        .def( "set", (void (Molecule::*)(const Improper &improper, const SireMaths::Angle &ang, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )
        .def( "set", (void (Molecule::*)(const Improper &improper, const SireMaths::Angle &ang, const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>())))
                        &Molecule::set )

    ;
}

}
