#ifndef SIREMOL_MOLECULEBONDS_H
#define SIREMOL_MOLECULEBONDS_H

#include "residuebonds.h"

#include <QHash>

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeBonds;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::MoleculeBonds&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeBonds&);

namespace SireMol
{

/**
This class contains all of the bonding information about a molecule. It enables very fast lookup of which atoms are bonded together, which residues are bonded together etc. This class is used by both EditMol and Molecule to hold the bonding information, and is the foundation on which the intramolecular move classes are constructed (as this class is used to split a molecule into parts based on the bonding).
 
This class is implicitly shared, so copying is fast.
 
@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeBonds
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeBonds&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeBonds&);

public:
    MoleculeBonds();
    MoleculeBonds(const MoleculeBonds &other);
    
    ~MoleculeBonds();

   ////////// Operators ////////////////////////////////
    bool operator==(const MoleculeBonds &other) const;
    bool operator!=(const MoleculeBonds &other) const;
   /////////////////////////////////////////////////////
   

   ////////// Editing functions ////////////////////////
    void add(const AtomIndex &atom0, const AtomIndex &atom1);
    void add(ResNum resnum, const QString &atom0, const QString &atom1);
    void add(const Bond &bond);
    
    void remove(const AtomIndex &atom0, const AtomIndex &atom1);
    void remove(ResNum resnum, const QString &atom0, const QString &atom1);
    void remove(const Bond &bond);
    
    void removeAll(const AtomIndex &atom);
    void removeAll(ResNum resnum);
    
    void clear();
    void finalise();
   ////////////////////////////////////////////////////

    
   ////////// Querying functions //////////////////////
    bool isEmpty() const;
    
    ResidueBonds residue(ResNum resnum) const;
    
    QString toString() const;
    
    QList<ResidueBonds> bondedResidues(ResNum resnum) const;
    
    QList<Bond> bonds() const;
    QList<Bond> bonds(const AtomIndex &atom) const;
    QList<Bond> bonds(ResNum resnum) const;
    
    int nBonds() const;
    int nResidues() const;
    
    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atom) const;
    bool contains(const Bond &bond) const;
    
    bool bonded(ResNum resnum0, ResNum resnum1) const;
    bool bonded(const AtomIndex &atom0, const AtomIndex &atom1) const;
   
    ResNumList resNumsBondedTo(ResNum resnum) const;
   
    ResNumList resNums() const;
    AtomIndexSet atoms() const;
   
   ////////////////////////////////////////////////////

private:

    ResidueBonds& getResidue(ResNum resnum);

    /** Hash of all of the ResidueBonds objects that describe the 
        bonding in each residue of this molecule - indexed by residue number */
    QHash<ResNum, ResidueBonds> resbnds;

};

}

Q_DECLARE_METATYPE(SireMol::MoleculeBonds)

SIRE_END_HEADER

#endif
