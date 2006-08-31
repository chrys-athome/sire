#ifndef SIREMOL_MOLECULEDATA_H
#define SIREMOL_MOLECULEDATA_H

#include <QSharedDataPointer>

#include <boost/shared_ptr.hpp>

#include "atomindex.h"
#include "residueidset.h"

#include "moleculebonds.h"
#include "moleculeversion.h"

#include "moleculecginfo.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
class Matrix;
class Quaternion;
class Line;
class Triangle;
class Torsion;
class Angle;
}

namespace SireVol
{
class CoordGroup;
}

namespace SireMol
{
class MoleculeData;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::MoleculeData&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeData&);

namespace SireMol
{

class MoleculeDataPvt;
typedef boost::shared_ptr< QSharedDataPointer<MoleculeDataPvt> > MoleculeDataPtr;

class Molecule;
class Residue;
class ResidueBonds;

class Bond;
class Angle;
class Dihedral;
class Improper;

class AtomIDGroup;
class WeightFunction;

class MoleculeSignature;

using SireVol::CoordGroup;

/**
This class holds the shared molecule data for the Molecule and Residue classes (which are both just views on this MolculeData class). This is very similar to the EditMolData class, on which EditMol and EditRes are merely views.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeData&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeData&);

public:
    MoleculeData();
    
    MoleculeData(const QString &molname, 
                 const ResidueIDSet &resids = ResidueIDSet(),
                 const QVector<CutGroup> &cutgroups = QVector<CutGroup>(),
                 const MoleculeBonds &molbonds = MoleculeBonds());
    
    MoleculeData(const MoleculeData &other);
    
    ~MoleculeData();

   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();
     MoleculeData clone() const;
     
     const MoleculeVersion& version() const;
     void incrementMajorVersion();
     void incrementMinorVersion();
   /////////////////////////////////////////////////////////
    

   ////// Operators ////////////////////////////////////////
    MoleculeData& operator=(const MoleculeData &other); 
    
    bool operator==(const MoleculeData &other) const;
    bool operator!=(const MoleculeData &other) const;
   /////////////////////////////////////////////////////////
   
   
   ///// Memory management /////////////////////////////////
    void detach();
    
    MoleculeData deepCopy() const;
    MoleculeData shallowCopy() const;
   /////////////////////////////////////////////////////////


   ///// Interfacing with Molecule and Residue /////////////
    Molecule molecule();
    Residue residue(ResNum renum);
    Residue residue(const QString &resname);
    
    QHash<ResNum,Residue> residues();
    
    Residue at(int i);
    const Atom& at(ResNum resnum, int i) const;
   /////////////////////////////////////////////////////////
   
   
   ///// Querying the molecule /////////////////////////////
    MoleculeBonds connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;
                  
    const MoleculeCGInfo& info() const;
    
    MoleculeSignature signature() const;
    
    AtomSet atoms() const;
    AtomSet atoms(ResNum resnum) const;
    
    AtomVector atomVector() const;
    AtomVector atomVector(ResNum resnum) const;
    
    CutGroupSet cutGroups() const;
    CutGroupSet cutGroups(ResNum resnum) const;
    
    const CutGroup& cutGroup(CutGroupID id) const;
    const Atom& atom(const CGAtomID &id) const;
    
    const QString& name() const;
    QString residueName(ResNum resnum) const;
    
    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    
    ResNumList residueNumbers() const;
    QStringList residueNames() const;
    
    ResNumList residueNumbers(const QString &resnam) const;
    
    ResidueSet residuesBondedTo(ResNum resnum);
    
    const Atom& atom(const AtomIndex &atm) const;
    
    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(const Bond &bond) const;
    
    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(CutGroupID id) const;
    
    int nResidues() const;
    
    int nCutGroups() const;
    
    QStringList atomNames(ResNum resnum) const;
    
    void checkResidue(ResNum) const;
    
    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;
    
    double measure(const Bond &bnd) const;
    SireMaths::Angle measure(const SireMol::Angle &ang) const;
    SireMaths::Angle measure(const Dihedral &dih) const;
    SireMaths::Angle measure(const Improper &improper) const;
   
    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1, 
                     const WeightFunction &weightfunc) const;
   /////////////////////////////////////////////////////////


   //////// Moving the molecule ////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);
    void translate(const AtomIndexSet &atoms, const Vector &delta);
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);
    
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
   /////////////////////////////////////////////////

   
   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta, 
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
    
    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
    
    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
                          
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
   /////////////////////////////////////////////////

private:
    
    const MoleculeDataPvt& d() const;
    MoleculeDataPvt& d();
    
    /** Shared pointer to the private implementation 
        of this class */
    MoleculeDataPtr ptr;
};

}

SIRE_END_HEADER

#endif
