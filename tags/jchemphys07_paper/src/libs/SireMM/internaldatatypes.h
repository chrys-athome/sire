/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_INTERNALDATATYPES_H
#define SIREMM_INTERNALDATATYPES_H

#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"

#include "SireMol/idtypes.h"

#include "SireMM/errors.h"

#include <QSet>

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class T>
class internal_type_data;

class BondResID;
class BondAtomID;

class AngleResID;
class AngleAtomID;

class DihedralResID;
class DihedralAtomID;

}

class QDataStream;

QDataStream& operator<<(QDataStream&, const SireMM::BondResID&);
QDataStream& operator>>(QDataStream&, SireMM::BondResID&);

QDataStream& operator<<(QDataStream&, const SireMM::BondAtomID&);
QDataStream& operator>>(QDataStream&, SireMM::BondAtomID&);

QDataStream& operator<<(QDataStream&, const SireMM::AngleResID&);
QDataStream& operator>>(QDataStream&, SireMM::AngleResID&);

QDataStream& operator<<(QDataStream&, const SireMM::AngleAtomID&);
QDataStream& operator>>(QDataStream&, SireMM::AngleAtomID&);

QDataStream& operator<<(QDataStream&, const SireMM::DihedralResID&);
QDataStream& operator>>(QDataStream&, SireMM::DihedralResID&);

QDataStream& operator<<(QDataStream&, const SireMM::DihedralAtomID&);
QDataStream& operator>>(QDataStream&, SireMM::DihedralAtomID&);

uint qHash(const SireMM::BondResID&);
uint qHash(const SireMM::BondAtomID&);

uint qHash(const SireMM::AngleResID&);
uint qHash(const SireMM::AngleAtomID&);

uint qHash(const SireMM::DihedralResID&);
uint qHash(const SireMM::DihedralAtomID&);

namespace SireMM
{

using SireMol::AtomIndex;
using SireMol::Bond;
using SireMol::Angle;
using SireMol::Dihedral;
using SireMol::ResNum;


/** Internal class used by MolBondInfo and ResBondInfo
    to identify the residues in a bond.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT BondResID
{

friend QDataStream& ::operator>>(QDataStream&, BondResID&);

public:
    BondResID()
    {
        id[0] = ResNum(0);
        id[1] = ResNum(0);
    }

    BondResID(ResNum resnum)
    {
        id[0] = resnum;
        id[1] = resnum;
    }

    BondResID(ResNum resnum0, ResNum resnum1)
    {
        //ensure that the residues are sorted correctly
        //(resnum0 is the smaller of the two)
        if (resnum0 <= resnum1)
        {
            id[0] = resnum0;
            id[1] = resnum1;
        }
        else
        {
            id[0] = resnum1;
            id[1] = resnum0;
        }
    }
    
    BondResID(const Bond &bond)
    {
        id[0] = bond.resNum0();
        id[1] = bond.resNum1();
    }
    
    BondResID(const BondResID &other)
    {
        id[0] = other.id[0];
        id[1] = other.id[1];
    }
    
    ~BondResID()
    {}
        
    ResNum resNum0() const
    {
        return id[0];
    }
    
    ResNum resNum1() const
    {
        return id[1];
    }

    int size() const
    {
        return 2;
    }

    ResNum operator[](int i) const
    {
        return id[ i%2 ];
    }

    bool isInterResidue() const
    {
        return id[0] != id[1];
    }
    
    bool isIntraResidue() const
    {
        return id[0] == id[1];
    }

    bool operator==(const BondResID &other) const
    {
        return id[0] == other.id[0] and id[1] == other.id[1];
    }
    
    bool operator!=(const BondResID &other) const
    {
        return id[0] != other.id[0] or id[1] != other.id[1];
    }

    QSet<ResNum> residueNumbers() const
    {
        QSet<ResNum> resnums;
        resnums.insert( id[0] );
        resnums.insert( id[1] );
        return resnums;
    }

private:
    /** The residue numbers of the two atoms in the bond */
    ResNum id[2];
};

/** Internal class used by MolBondInfo and ResBondInfo
    to identify the atoms in a bond.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT BondAtomID
{

friend QDataStream& ::operator>>(QDataStream&, BondAtomID&);

public:
    BondAtomID()
    {}

    BondAtomID(const QString &atom0, const QString &atom1)
    {
        atms[0] = atom0;
        atms[1] = atom1;
    }
               
    BondAtomID(const Bond &bond)
    {
        atms[0] = bond.atom0().name();
        atms[1] = bond.atom1().name();
    }
               
    BondAtomID(const BondAtomID &other)
    {
        atms[0] = other.atms[0];
        atms[1] = other.atms[1];
    }
               
    ~BondAtomID()
    {}

    int size() const
    {
        return 2;
    }

    const QString& operator[](int i) const
    {
        return atms[ i%2 ];
    }
    
    const QString& atom0() const
    {
        return atms[0];
    }
    
    const QString& atom1() const
    {
        return atms[1];
    }

    bool operator==(const BondAtomID &other) const
    {
        return atms[0] == other.atms[0] and atms[1] == other.atms[1];
    }
    
    bool operator!=(const BondAtomID &other) const
    {
        return atms[0] != other.atms[0] or atms[1] != other.atms[1];
    }

private:
    /** The names of the two atoms in the bond */
    QString atms[2];
};

/** Internal class used by MolAngleInfo and ResAngleInfo
    to identify the residues in an angle.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT AngleResID
{

friend QDataStream& ::operator>>(QDataStream&, AngleResID&);

public:
    AngleResID()
    {
        id[0] = ResNum(0);
        id[1] = ResNum(0);
        id[2] = ResNum(0);
    }

    AngleResID(ResNum resnum)
    {
        id[0] = resnum;
        id[1] = resnum;
        id[2] = resnum;
    }

    AngleResID(ResNum resnum0, ResNum resnum1, ResNum resnum2)
    {
        //ensure that the residues are sorted correctly
        //(resnum0 <= resnum2)
        if (resnum0 <= resnum2)
        {
            id[0] = resnum0;
            id[2] = resnum2;
        }
        else
        {
            id[0] = resnum2;
            id[2] = resnum0;
        }
        
        id[1] = resnum1;
        
    }
    
    AngleResID(const Angle &angle)
    {
        id[0] = angle.resNum0();
        id[1] = angle.resNum1();
        id[2] = angle.resNum2();
    }
    
    AngleResID(const AngleResID &other)
    {
        id[0] = other.id[0];
        id[1] = other.id[1];
        id[2] = other.id[2];
    }
    
    ~AngleResID()
    {}
        
    int size() const
    {
        return 3;
    }

    ResNum operator[](int i) const
    {
        return id[ i%3 ];
    }
    
    ResNum resNum0() const
    {
        return id[0];
    }
    
    ResNum resNum1() const
    {
        return id[1];
    }
    
    ResNum resNum2() const
    {
        return id[2];
    }

    bool isIntraResidue() const
    {
        return id[0] == id[1] and id[0] == id[2];
    }
    
    bool isInterResidue() const
    {
        return not isIntraResidue();
    }

    bool operator==(const AngleResID &other) const
    {
        return id[0] == other.id[0] and id[1] == other.id[1] and
               id[2] == other.id[2];
    }
    
    bool operator!=(const AngleResID &other) const
    {
        return id[0] != other.id[0] or id[1] != other.id[1] or
               id[2] != other.id[2];
    }

    QSet<ResNum> residueNumbers() const
    {
        QSet<ResNum> resnums;
        resnums.insert( id[0] );
        resnums.insert( id[1] );
        resnums.insert( id[2] );
        return resnums;
    }

private:
    /** The residue numbers of the three atoms in the angle */
    ResNum id[3];
};

/** Internal class used by MolAngleInfo and ResAngleInfo
    to identify the atoms in an angle.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT AngleAtomID
{

friend QDataStream& ::operator>>(QDataStream&, AngleAtomID&);

public:
    AngleAtomID()
    {}

    AngleAtomID(const QString &atom0, const QString &atom1,
                const QString &atom2)
    {
        atms[0] = atom0;
        atms[1] = atom1;
        atms[2] = atom2;
    }
               
    AngleAtomID(const Angle &angle)
    {
        atms[0] = angle.atom0().name();
        atms[1] = angle.atom1().name();
        atms[2] = angle.atom2().name();
    }
               
    AngleAtomID(const AngleAtomID &other)
    {
        atms[0] = other.atms[0];
        atms[1] = other.atms[1];
        atms[2] = other.atms[2];
    }
               
    ~AngleAtomID()
    {}

    int size() const
    {
        return 3;
    }

    const QString& operator[](int i) const
    {
        return atms[ i%3 ];
    }
    
    const QString& atom0() const
    {
        return atms[0];
    }
    
    const QString& atom1() const
    {
        return atms[1];
    }
    
    const QString& atom2() const
    {
        return atms[2];
    }

    bool operator==(const AngleAtomID &other) const
    {
        return atms[0] == other.atms[0] and atms[1] == other.atms[1] and
               atms[2] == other.atms[2];
    }
    
    bool operator!=(const AngleAtomID &other) const
    {
        return atms[0] != other.atms[0] or atms[1] != other.atms[1] or
               atms[2] != other.atms[2];
    }

private:
    /** The names of the three atoms in the angle */
    QString atms[3];
};

/** Internal class used by MolDihedralInfo and ResDihedralInfo
    to identify the residues in an dihedral.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT DihedralResID
{

friend QDataStream& ::operator>>(QDataStream&, DihedralResID&);

public:
    DihedralResID()
    {
        id[0] = ResNum(0);
        id[1] = ResNum(0);
        id[2] = ResNum(0);
        id[3] = ResNum(0);
    }

    DihedralResID(ResNum resnum)
    {
        id[0] = resnum;
        id[1] = resnum;
        id[2] = resnum;
        id[3] = resnum;
    }

    DihedralResID(ResNum resnum0, ResNum resnum1, 
                  ResNum resnum2, ResNum resnum3)
    {
        //ensure that the residues are sorted
        //(resnum0 < resnum3, or resnum0 == resnum3 and 
        // resnum1 <= resnum2)
        if (resnum0 == resnum3)
        {
            if (resnum1 <= resnum2)
            {
                id[0] = resnum0;
                id[1] = resnum1;
                id[2] = resnum2;
                id[3] = resnum3;
            }
            else
            {
                id[0] = resnum3;
                id[1] = resnum2;
                id[2] = resnum1;
                id[3] = resnum0;
            }
        }
        else if (resnum0 < resnum3)
        {
            id[0] = resnum0;
            id[1] = resnum1;
            id[2] = resnum2;
            id[3] = resnum3;
        }
        else
        {
            id[0] = resnum3;
            id[1] = resnum2;
            id[2] = resnum1;
            id[3] = resnum0;
        }
    }
    
    DihedralResID(const Dihedral &dihedral)
    {
        id[0] = dihedral.resNum0();
        id[1] = dihedral.resNum1();
        id[2] = dihedral.resNum2();
        id[3] = dihedral.resNum3();
    }
    
    DihedralResID(const DihedralResID &other)
    {
        id[0] = other.id[0];
        id[1] = other.id[1];
        id[2] = other.id[2];
        id[3] = other.id[3];
    }
    
    ~DihedralResID()
    {}

    int size() const
    {
        return 4;
    }

    ResNum operator[](int i) const
    {
        return id[ i%4 ];
    }
        
    ResNum resNum0() const
    {
        return id[0];
    }
    
    ResNum resNum1() const
    {
        return id[1];
    }
    
    ResNum resNum2() const
    {
        return id[2];
    }

    ResNum resNum3() const
    {
        return id[3];
    }

    bool isIntraResidue() const
    {
        return id[0] == id[1] and id[0] == id[2] and id[0] == id[3];
    }
    
    bool isInterResidue() const
    {
        return not isIntraResidue();
    }

    bool operator==(const DihedralResID &other) const
    {
        return id[0] == other.id[0] and id[1] == other.id[1] and
               id[2] == other.id[2] and id[3] == other.id[3];
    }
    
    bool operator!=(const DihedralResID &other) const
    {
        return id[0] != other.id[0] or id[1] != other.id[1] or
               id[2] != other.id[2] or id[3] != other.id[3];
    }

    QSet<ResNum> residueNumbers() const
    {
        QSet<ResNum> resnums;
        resnums.insert( id[0] );
        resnums.insert( id[1] );
        resnums.insert( id[2] );
        resnums.insert( id[3] );
        return resnums;
    }

private:
    /** The residue numbers of the four atoms in the dihedral */
    ResNum id[4];
};

/** Internal class used by MolAngleInfo and ResAngleInfo
    to identify the atoms in an angle.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT DihedralAtomID
{

friend QDataStream& ::operator>>(QDataStream&, DihedralAtomID&);

public:
    DihedralAtomID()
    {}

    DihedralAtomID(const QString &atom0, const QString &atom1,
                   const QString &atom2, const QString &atom3)
    {
        atms[0] = atom0;
        atms[1] = atom1;
        atms[2] = atom2;
        atms[3] = atom3;
    }
               
    DihedralAtomID(const Dihedral &dihedral)
    {
        atms[0] = dihedral.atom0().name();
        atms[1] = dihedral.atom1().name();
        atms[2] = dihedral.atom2().name();
        atms[3] = dihedral.atom3().name();
    }
               
    DihedralAtomID(const DihedralAtomID &other)
    {
        atms[0] = other.atms[0];
        atms[1] = other.atms[1];
        atms[2] = other.atms[2];
        atms[3] = other.atms[3];
    }
               
    ~DihedralAtomID()
    {}

    int size() const
    {
        return 4;
    }

    const QString& operator[](int i) const
    {
        return atms[ i%4 ];
    }
    
    const QString& atom0() const
    {
        return atms[0];
    }
    
    const QString& atom1() const
    {
        return atms[1];
    }
    
    const QString& atom2() const
    {
        return atms[2];
    }

    const QString& atom3() const
    {
        return atms[3];
    }

    bool operator==(const DihedralAtomID &other) const
    {
        return atms[0] == other.atms[0] and atms[1] == other.atms[1] and
               atms[2] == other.atms[2] and atms[3] == other.atms[3];
    }
    
    bool operator!=(const DihedralAtomID &other) const
    {
        return atms[0] != other.atms[0] or atms[1] != other.atms[1] or
               atms[2] != other.atms[2] or atms[3] != other.atms[3];
    }

private:
    /** The names of the four atoms in the dihedral */
    QString atms[4];
};

/** Provide some metainfo that is used by detail::InternalInfo 
    to index the bonds */
template<>
class internal_type_data<Bond>
{
public:
    typedef SireMM::missing_bond missing_error_type;
    typedef BondResID residue_id_type;
    typedef BondAtomID atom_id_type;
    
    static Bond convertToInternal( const BondResID &resid, const BondAtomID &atmid )
    {
        return Bond( AtomIndex(atmid.atom0(), resid.resNum0()),
                     AtomIndex(atmid.atom1(), resid.resNum1()) );
    }
};

/** Provide some metainfo that is used by detail::InternalInfo
    to index the angles */
template<>
class internal_type_data<Angle>
{
public:
    typedef SireMM::missing_angle missing_error_type;
    typedef AngleResID residue_id_type;
    typedef AngleAtomID atom_id_type;
    
    static Angle convertToInternal( const AngleResID &resid, const AngleAtomID &atmid )
    {
        return Angle( AtomIndex(atmid.atom0(), resid.resNum0()),
                      AtomIndex(atmid.atom1(), resid.resNum1()),
                      AtomIndex(atmid.atom2(), resid.resNum2()) );
    }
};

/** Provide some metainfo that is used by detail::InternalInfo 
    to index the dihedrals */
template<>
class internal_type_data<Dihedral>
{
public:
    typedef SireMM::missing_dihedral missing_error_type;
    typedef DihedralResID residue_id_type;
    typedef DihedralAtomID atom_id_type;
    
    static Dihedral convertToInternal( const DihedralResID &resid,
                                       const DihedralAtomID &atmid )
    {
        return Dihedral( AtomIndex(atmid.atom0(), resid.resNum0()),
                         AtomIndex(atmid.atom1(), resid.resNum1()),
                         AtomIndex(atmid.atom2(), resid.resNum2()),
                         AtomIndex(atmid.atom3(), resid.resNum3()) );
    }
};

}

SIRE_END_HEADER

#endif
