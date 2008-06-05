#ifndef SIREMOL_RESIDUEIDSET_H
#define SIREMOL_RESIDUEIDSET_H

#include "SireBase/set.hpp"

#include "atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResidueID&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueID&);

uint qHash(const SireMol::ResidueID&);

namespace SireMol
{

/** This is a very simple class that holds both the residue's name and number 

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResidueID
{

friend QDataStream& ::operator>>(QDataStream&, ResidueID&);

public:
    ResidueID(const QString &resname = QString::null, 
            ResNum resnum = 0) : rname(resname), rnum(resnum)
    {}
    
    ResidueID(const ResidueID &other) : rname(other.rname), rnum(other.rnum)
    {}
    
    ~ResidueID()
    {}
    
    QString name() const
    {
        return rname;
    }
    
    ResNum number() const
    {
        return rnum;
    }
    
    void setName(const QString &resnam)
    {
        rname = resnam;
    }
    
    void setNumber(ResNum resnum)
    {
        rnum = resnum;
    }
    
    bool operator==(const ResidueID &other) const
    {
        return rnum == other.rnum and rname == other.rname;
    }
    
    bool operator!=(const ResidueID &other) const
    {
        return rnum != other.rnum or rname != other.rname;
    }
    
    QString toString() const
    {
        return QString("%1(%2)").arg(rname, rnum.toString());
    }

private:
    QString rname;
    ResNum rnum;
};

/** This is a set of residue names, indexed by residue number */
typedef SireBase::Set<ResNum,ResidueID> ResidueIDSet;

}

Q_DECLARE_METATYPE(SireMol::ResidueID)

SIRE_END_HEADER

#endif
