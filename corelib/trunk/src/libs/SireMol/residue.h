#ifndef SIREMOL_RESIDUE_H
#define SIREMOL_RESIDUE_H

#include "moleculedata.h"
#include "residueset.h"
#include "bondset.h"

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

namespace SireMol
{
class Residue;
}

QDataStream& operator<<(QDataStream&, const SireMol::Residue&);
QDataStream& operator>>(QDataStream&, SireMol::Residue&);

uint qHash(const SireMol::Residue&);

namespace SireMol
{

class Molecule;
class ResidueBonds;
class Bond;
class Angle;
class Dihedral;
class Improper;

class WeightFunction;

/**
This class represents a Residue in a Molecule. 
 
@author Christopher Woods
*/
class SIREMOL_EXPORT Residue
{

friend class MoleculeData; //so it can call the MoleculeData constructor 
friend QDataStream& ::operator<<(QDataStream&, const Residue&);
friend QDataStream& ::operator>>(QDataStream&, Residue&);

public:
    Residue();
    
    Residue(const Residue &other);
    
    ~Residue();

   ///// Operators /////////////////////////////////////////
    bool operator==(const Residue &other) const;
    bool operator!=(const Residue &other) const;
    
    Residue& operator=(const Residue &other);
    
    const Atom& operator[](int i) const;
   /////////////////////////////////////////////////////////


   ///// Memory mangement and interface with molecule ///////
    Residue deepCopy() const;
    Residue shallowCopy();
    
    void detach();
    
    Molecule molecule();
   /////////////////////////////////////////////////////////


   ///// Querying the residue //////////////////////////////
    QString name() const;
    QString toString() const;

    bool isNull() const;
    bool isEmpty() const;

    ResNum number() const;
    ResNum resNum() const;

    CutGroupSet cutGroups() const;

    const Atom& at(int i) const;
    
    int count() const;
    int size() const;
    int nAtoms() const;
    
    QStringList atomNames() const;
    
    int nBonds() const;
    ResidueBonds connectivity() const;
    
    bool contains(const QString &name) const;
    
    const Atom& atom(const QString &name) const;
    const Atom& atom(int i) const;

    SireMaths::Line bond(const QString &atom0, const QString &atom1) const;
    SireMaths::Triangle angle(const QString &atom0, const QString &atom1,
                              const QString &atom2) const;
    SireMaths::Torsion dihedral(const QString &atom0, const QString &atom1,
                                const QString &atom2, const QString &atom3) const;
    SireMaths::Torsion improper(const QString &atom0, const QString &atom1,
                                const QString &atom2, const QString &atom3) const;
        
    double measure(const QString &atom0, const QString &atom1) const;
    SireMaths::Angle measure(const QString &atom0, const QString &atom1,
                             const QString &atom2) const;
    SireMaths::Angle measure(const QString &atom0, const QString &atom1,
                             const QString &atom2, const QString &atom3) const;
    SireMaths::Angle measureImproper(const QString &atom0, const QString &atom1,
                                     const QString &atom2, const QString &atom3) const;
   
    AtomSet atoms() const;
    AtomVector atomVector() const;
    
    BondList bonds() const;
    
    ResidueSet residuesBondedTo();
   /////////////////////////////////////////////////////////


   //// Moving the residue /////////////////////////////////
    void translate(const Vector &delta);
    void translate(const QString &atom, const Vector &delta);
    void translate(const QStringList &atoms, const Vector &delta);
          
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const QString &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point);
   /////////////////////////////////////////////////////////
   
   
   //// Internal geometry moves ////////////////////////////
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
    Residue(const MoleculeData &moldata, ResNum resnum);
    
    /** MoleculeData object containing the actual data of the molecule */
    MoleculeData moldata;
    
    /** The residue number of this residue */
    ResNum rnum;

};

}

/** Function used to index a Residue in a ResidueSet */
template<>
inline SireMol::ResNum set_indexer(const SireMol::Residue &res)
{
    return res.resNum();
}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
