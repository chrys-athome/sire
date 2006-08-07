#ifndef SIREMOL_CUTGROUP_H
#define SIREMOL_CUTGROUP_H

#include <QSharedDataPointer>

#include "atomset.h"
#include "atomindexset.h"
#include "idtypes.h"

#include "SireMaths/vectorvector.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Quaternion;
}

namespace SireMol
{
class CutGroup;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::CutGroup&);
QDataStream& operator>>(QDataStream&, SireMol::CutGroup&);

namespace SireMol
{

class CutGroupPvt;
class AABox;

using SireMaths::VectorVector;
using SireMaths::Quaternion;

/**
A CutGroup is a logical grouping of Atoms into a single group that is considered for intermolecular non-bonded cutting.
  
A CutGroup is implicitly shared, meaning that it is very fast to copy, with a deep copy only
occuring on writes.

CutGroup is reentrant, but definitely not thread-safe! Copying a CutGroup is thread-safe.
  
@author Christopher Woods
*/
class SIREMOL_EXPORT CutGroup
{

friend QDataStream& ::operator<<(QDataStream&, const CutGroup&);
friend QDataStream& ::operator>>(QDataStream&, CutGroup&);

public:
    
    CutGroup();
    CutGroup(const AtomSet &atoms);
    
    CutGroup(const CutGroup &other);
    
    ~CutGroup();

   //////// Managing the ID number /////////////////////////////////
    const MolCutGroupID& ID() const;
    void setID(const MolCutGroupID &id);
   /////////////////////////////////////////////////////////////////


   //////// Operators //////////////////////////////////////////////
    const Atom& operator[](AtomID i) const;
    
    bool operator==(const CutGroup &other) const;
    bool operator!=(const CutGroup &other) const;
   
    CutGroup& operator=(const CutGroup &other);
   /////////////////////////////////////////////////////////////////


   //////// Query functions ////////////////////////////////////////
    QString toString() const;

    const AtomSet& atoms() const;
    AtomSet atoms(AtomID strt, AtomID end) const;

    const Atom& at(AtomID i) const;
        
    const Atom& atom(AtomID i) const;
    const Atom& atom(const AtomIndex &atmidx) const;

    int nAtoms() const;
    int size() const;
    int count() const;

    const AABox& aaBox() const;

    bool contains(const AtomIndex &atm) const;
   /////////////////////////////////////////////////////////////////


   //////// Getting and setting the coordinates of the atoms ///////
    VectorVector coordinates() const;
    const Vector& coordinates(const AtomIndex &atm) const;
    const Vector& coordinates(AtomID i) const;
    
    void setCoordinates(const VectorVector &coords);
    void setCoordinates(const AtomIndex &atm, const Vector &coords);
    void setCoordinates(AtomID i, const Vector &coords);
   /////////////////////////////////////////////////////////////////

private:
    
    QSharedDataPointer<CutGroupPvt> d;
    
};

}

Q_DECLARE_METATYPE(SireMol::CutGroup)

SIRE_END_HEADER

#endif
