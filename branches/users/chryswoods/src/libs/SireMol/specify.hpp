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

#ifndef SIREMOL_SPECIFY_HPP
#define SIREMOL_SPECIFY_HPP

#include "moleculeinfodata.h"

#include "SireID/index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class is used to help form specified ID matches, 
    e.g. the third residue called alanine ( ResName("ALA")[2] )
    or the last three atoms called "CA" ( AtomName("CA")(-3,-1) )
    
    @author Christopher Woods
*/
template<class ID>
class Specify : public ID
{
private:
    static QString typname;

public:
    Specify(const ID &id, qint32 i);
    Specify(const ID &id, qint32 i, qint32 j);
    
    Specify(const Specify<ID> &other);
    
    ~Specify();
    
    static const char* typeName()
    {
        return qPrintable(typname);
    }
    
    const char* what() const
    {
        return Specify<ID>::typeName();
    }
    
    Specify<ID>* clone() const
    {
        return new Specify<ID>(*this);
    }

    Specify<ID>& operator=(const Specify<ID> &other);
    
    bool operator==(const Specify<ID> &other) const;
    bool operator==(const SireID::ID &other) const;

    bool operator!=(const Specify<ID> &other) const
    {
        return not this->operator==(other);
    }
    
    bool operator!=(const SireID::ID &other) const
    {
        return not this->operator==(other);
    }
    
    bool isNull() const
    {
        return false;
    }
    
    QString toString() const;
    
    QList<typename ID::Index> map(const MoleculeInfoData &molinfo) const;

private:
    /** Hide the index operators */
    int operator[](int) const
    {
        return 0;
    }
    
    int operator()(int) const
    {
        return 0;
    }
    
    int operator()(int,int) const
    {
        return 0;
    }

    SireID::Index strt, end;
};

template<class ID>
QString Specify<ID>::typname = QString("SireMol::Specify<%1>").arg(ID::typeName());

/** Construct, using the passed ID and index */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
Specify<ID>::Specify(const ID &id, qint32 i)
            : ID(id), strt(i), end(i)
{}

/** Construct using the passed ID and range */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
Specify<ID>::Specify(const ID &id, qint32 i, qint32 j)
            : ID(id), strt(i), end(j)
{}
  
/** Copy constructor */  
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
Specify<ID>::Specify(const Specify<ID> &other)
            : ID(other), strt(other.strt), end(other.end)
{}
  
/** Destructor */  
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
Specify<ID>::~Specify()
{}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
Specify<ID>& Specify<ID>::operator=(const Specify<ID> &other)
{
    if (&other != this)
    {
        ID::operator=(other);
        strt = other.strt;
        end = other.end;
    }
    
    return *this;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool Specify<ID>::operator==(const Specify<ID> &other) const
{
    return strt == other.strt and end == other.end and 
           ID::operator==(other);
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool Specify<ID>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< Specify<ID> >(*this, other);
}

/** Return a string representation of this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QString Specify<ID>::toString() const
{
    if (strt == end)
        return QString("(%1)[%2]").arg(ID::toString()).arg(strt);
    else
        return QString("(%1)[%2:%3]").arg(ID::toString())
                                     .arg(strt).arg(end);
}

/** Map this ID to the indicies of the matching parts of the molecule */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QList<typename ID::Index> Specify<ID>::map(const MoleculeInfoData &molinfo) const
{
    //first get all of the matches
    QList<typename ID::Index> idxs = ID::map(molinfo);
    
    //now get the specified matches
    int nmatches = idxs.count();

    int sane_strt = strt.map(nmatches);
    int sane_end = end.map(nmatches);
    
    if (sane_strt > sane_end)
        qSwap(sane_strt,sane_end);
    
    if (sane_end - sane_strt == nmatches)
        return idxs;
    else
    {
        QList<typename ID::Index> specified_idxs;
    
        for (int i=strt; i<=end; ++i)
        {
            specified_idxs.append(idxs[i]);
        }
        
        return specified_idxs;
    }
}

}

SIRE_END_HEADER

#endif
