#ifndef SIREMOL_MOLECULE_H
#define SIREMOL_MOLECULE_H
/**
  * @file
  *
  * C++ Interface: Molecule
  *
  * Description:
  * Interface for Molecule
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "moleculedata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

QDataStream& operator<<(QDataStream&, const SireMol::Molecule&);
QDataStream& operator>>(QDataStream&, SireMol::Molecule&);

namespace SireMol
{

/**
A Molecule represents a complete molecule. This class is merely a view on the underlying
MoleculeData class.

@author Christopher Woods
*/
class SIREMOL_EXPORT Molecule
{

friend class MoleculeData; //so it can call the MoleculeData constructor
friend QDataStream& ::operator<<(QDataStream&, const Molecule&);
friend QDataStream& ::operator>>(QDataStream&, Molecule&);

public:
    Molecule();

    Molecule(const EditMol &editmol,
             const ConvertFunction &converter = ResidueCutting() );

    Molecule(const Molecule &other);

    ~Molecule();

   ////// Dealing with the ID number ///////////////////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;
   /////////////////////////////////////////////////////////


   ///// Operators /////////////////////////////////////////
    bool operator==(const Molecule &other) const;
    bool operator!=(const Molecule &other) const;

    Molecule& operator=(const Molecule &other);
    Molecule& operator=(const Residue &other);

    Residue operator[](int i) const;
   /////////////////////////////////////////////////////////


   ///// Memory mangement and interface with residue ///////
    Residue residue(ResNum resnum) const;
    Residue at(int i) const;

    QHash<ResNum,Residue> residues();
    QHash<ResNum,Residue> residues() const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    QString toString() const;

    QString name() const;

    const MoleculeInfo& info() const;

    bool isNull() const;
    bool isEmpty() const;

    CutGroup cutGroup(CutGroupID cgid) const;
    QVector<CutGroup> cutGroups() const;

    CoordGroup coordinates(CutGroupID cgid) const;
    QVector<CoordGroup> coordinates() const;

    MoleculeBonds connectivity() const;

    QString residueName(ResNum resnum) const;

    QList<ResNum> residueNumbers() const;
    QList<ResNum> residueNumbers(const QString &resnam) const;

    QStringList residueNames() const;

    QHash<ResNum,QString> residueIDs() const;

    Atom atom( const AtomIndex &atm ) const;

    QList<Atom> atoms() const;
    QList<Atom> atoms(ResNum resnum) const;

    QVector<Atom> atomVector() const;
    QVector<Atom> atomVector(ResNum resnum) const;

    QList<Bond> bonds() const;
    QList<Bond> bonds(ResNum resnum) const;

    int nAtoms() const;
    int nAtoms(CutGroupID id) const;
    int nAtoms(ResNum resnum) const;

    int nBonds() const;
    int nResidues() const;
    int nCutGroups() const;

    int count() const;

    SireMaths::Line bond( const Bond &bnd ) const;
    SireMaths::Triangle angle( const Angle &ang ) const;
    SireMaths::Torsion dihedral( const Dihedral &dih ) const;
    SireMaths::Torsion improper( const Improper &imp ) const;

    double measure( const Bond &bond ) const;
    SireMaths::Angle measure( const Angle &ang ) const;
    SireMaths::Angle measure( const Dihedral &dih ) const;
    SireMaths::Angle measure( const Improper &imp ) const;
   /////////////////////////////////////////////////////////


   ///// Moving the molecule ///////////////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);
    void translate(const AtomIndexSet &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);

    void setCoordinates(CutGroupID cgid, const CoordGroup &coords);
    void setCoordinates(const QHash<CutGroupID,CoordGroup> coords);
   /////////////////////////////////////////////////////////


   ///// Internal geometry moves ///////////////////////////
    void change(const Bond &bond, double delta,
                const AtomIndexSet &anchors = AtomIndexSet());
    void change(const Bond &bond, double delta, const WeightFunction &func,
                const AtomIndexSet &anchors = AtomIndexSet());

    void change(const Angle &angle, const SireMaths::Angle &delta,
                const AtomIndexSet &anchors = AtomIndexSet());
    void change(const Angle &angle, const SireMaths::Angle &delta,
                const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const AtomIndexSet &anchors = AtomIndexSet());
    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void change(const Bond &bond, const SireMaths::Angle &delta,
                const AtomIndexSet &anchors = AtomIndexSet());
    void change(const Bond &bond, const SireMaths::Angle &delta,
                const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIndexSet &anchors = AtomIndexSet());
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void set(const Bond &bond, double lgth, const AtomIndexSet &anchors = AtomIndexSet());
    void set(const Bond &bond, double lgth, const WeightFunction &func,
             const AtomIndexSet &anchors = AtomIndexSet());

    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const AtomIndexSet &anchors = AtomIndexSet());
    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const AtomIndexSet &anchors = AtomIndexSet());
    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const AtomIndexSet &anchors = AtomIndexSet());
    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());

    void set(const Improper &improper, const SireMaths::Angle &ang,
             const AtomIndexSet &anchors = AtomIndexSet());
    void set(const Improper &improper, const SireMaths::Angle &ang,
             const WeightFunction &func, const AtomIndexSet &anchors = AtomIndexSet());
   /////////////////////////////////////////////////////////

private:
    Molecule(const MoleculeData &moldata);

    /** The MoleculeData object that contains all of the data
        and functionality of this molecule */
    MoleculeData moldata;
};

}

Q_DECLARE_METATYPE(SireMol::Molecule)

SIRE_END_HEADER

#endif
