#ifndef SIREMOL_RESIDUEINFO_H
#define SIREMOL_RESIDUEINFO_H

#include <QHash>

#include "atomindexset.h"
#include "residueidset.h"
#include "idtypes.h"

#include "SireMol/errors.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueInfoBase;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::ResidueInfoBase&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueInfoBase&);

namespace SireMol
{

/** 
  * This class provides the template-independent parts of ResidueInfo.
  * This should improve compilation time and reduce code-bloat.
  *
  * @author Christopher Woods
  *
  */
class SIREMOL_EXPORT ResidueInfoBase
{

friend QDataStream& ::operator<<(QDataStream&, const ResidueInfoBase&);
friend QDataStream& ::operator>>(QDataStream&, ResidueInfoBase&);

public:
    ResidueInfoBase();
    ResidueInfoBase(const QString &resnam, ResNum resnum);
    
    ResidueInfoBase(const ResidueInfoBase &other);
    
    ~ResidueInfoBase();
    
    ResidueInfoBase& operator=(const ResidueInfoBase &other);
    
    bool operator==(const ResidueInfoBase &other) const;
    bool operator!=(const ResidueInfoBase &other) const;
    
    QString toString() const;
    
    int size() const;
    int nAtoms() const;
    int count() const;
    
    QString name() const;
    QString resName() const;
    
    ResNum number() const;
    ResNum resNum() const;
    
    const ResidueID& ID() const;
    
    AtomIndex atom(AtomID i) const;
    
    const QStringList& atomNames() const;
    
    void setName(const QString &name);
    void setNumber(ResNum resnum);
    
    void clear();
    void squeeze();
    
    void reserve(int n);
    
    bool contains(const QString &atmname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(AtomID atm) const;
    
protected:

    void addAtom(const QString &atm, quint32 i);

    /** The name and number of this residue */
    ResidueID resid;
    
    /** Hash mapping atom names to indexes */
    QHash<QString,qint32> name2idx;

    /** List of the names of the atoms, in the order that 
        they were added to the residue */
    QStringList atmnames;
};

/** Return the name of this residue */
inline QString ResidueInfoBase::name() const
{
    return resid.name();
}

/** Return the name of this residue */
inline QString ResidueInfoBase::resName() const
{
    return resid.name();
}
    
/** Return the residue's number */
inline ResNum ResidueInfoBase::number() const
{
    return resid.number();
}

/** Return the residue's number */
inline ResNum ResidueInfoBase::resNum() const
{
    return resid.number();
}
    
/** Return the residue's ID (combined name and number) */
inline const ResidueID& ResidueInfoBase::ID() const
{
    return resid;
}
    
/** Return the names of the atoms in this residue (in the order in which
    they were added to the residue) */
inline const QStringList& ResidueInfoBase::atomNames() const
{
    return atmnames;
}

/** Return whether or not this residue contains the atom with name 'atmname' */
inline bool ResidueInfoBase::contains(const QString &atmname) const
{
    return name2idx.contains(atmname);
}

/** Return whether or not this residue contains the atom with index 'atm' */
inline bool ResidueInfoBase::contains(const AtomIndex &atm) const
{
    return atm.resNum() == resNum() and contains(atm.name());
}

/** Return whether or not this residue contains the atom with index 'atm' */
inline bool ResidueInfoBase::contains(AtomID atm) const
{
    return atm < nAtoms();
}

}

SIRE_END_HEADER

#endif
