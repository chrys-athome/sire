/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMM_INTERNALPARAMETERS_H
#define SIREMM_INTERNALPARAMETERS_H

namespace SireMM
{

class InternalParameters;
class GroupInternalParameters;

}

QDataStream& operator<<(QDataStream&, const SireMM::InternalParameters&);
QDataStream& operator>>(QDataStream&, SireMM::InternalParameters&);

QDataStream& operator<<(QDataStream&, const SireMM::GroupInternalParameters&);
QDataStream& operator>>(QDataStream&, SireMM::GroupInternalParameters&);

namespace SireMM
{

/** This class holds a potential function that acts using the 
    coordinate information of just three atoms */
class ThreeAtomFunction : public AtomFunction
{

friend QDataStream& ::operator<<(QDataStream&, const ThreeAtomFunction&);
friend QDataStream& ::operator>>(QDataStream&, ThreeAtomFunction&);

public:
    ThreeAtomFunction();
    ThreeAtomFunction(const CGAtomIdx &atom0, const CGAtomIdx &atom1,
                      const CGAtomIdx &atom2,
                      const SireCAS::Expression &potential);
                  
    ThreeAtomFunction(const ThreeAtomFunction &other);
    
    ~ThreeAtomFunction();

    ThreeAtomFunction& operator=(const ThreeAtomFunction &other);
    
    bool operator==(const ThreeAtomFunction &other) const;
    bool operator!=(const ThreeAtomFunction &other) const;
    
    const CGAtomIdx& atom0() const
    {
        return atm0;
    }
    
    const CGAtomIdx& atom1() const
    {
        return atm1;
    }

    const CGAtomIdx &atom2() const
    {
        return atm2;
    }
    
private:
    /** The indicies of the three atoms */
    CGAtomIdx atm0, atm1, atm2;
};

/** This class holds a potential function that acts using the 
    coordinate information of just four atoms */
class FourAtomFunction : public AtomFunction
{

friend QDataStream& ::operator<<(QDataStream&, const FourAtomFunction&);
friend QDataStream& ::operator>>(QDataStream&, FourAtomFunction&);

public:
    FourAtomFunction();
    FourAtomFunction(const CGAtomIdx &atom0, const CGAtomIdx &atom1,
                     const CGAtomIdx &atom2, const CGAtomIdx &atom3,
                     const SireCAS::Expression &potential);
                  
    FourAtomFunction(const FourAtomFunction &other);
    
    ~FourAtomFunction();

    FourAtomFunction& operator=(const FourAtomFunction &other);
    
    bool operator==(const FourAtomFunction &other) const;
    bool operator!=(const FourAtomFunction &other) const;
    
    const CGAtomIdx& atom0() const
    {
        return atm0;
    }
    
    const CGAtomIdx& atom1() const
    {
        return atm1;
    }

    const CGAtomIdx& atom2() const
    {
        return atm2;
    }
    
    const CGAtomIdx& atom3() const
    {
        return atm3;
    }
    
private:
    /** The indicies of the four atoms */
    CGAtomIdx atm0, atm1, atm2, atm3;
};

/** This class holds all of the internal parameters for one group
    combination within a molecule
    
    There are several types of internal parameters, defined
    by the type of internal used to provide the coordinates,
    and the quantities calculated from those coordinates that
    can be used in the function
    
    Bond            : Input Bond - function uses interatomic distance (1-2), r
    Angle           : Input Angle - function uses angle (1-2-3), theta
    Dihedral        : Input Dihedral - function uses torsion (1-2-3-4), phi
    
    Improper        : Input Improper - function uses either torsion angle
                                       (1-3-4-2), phi, or out of plane angle, theta
                                       
    Urey-Bradley    : Input Angle - function uses distance (1-3), r
    
    Stretch-Stretch : Input Angle - function uses distances (1-2), r12, and 
                                    (3-2), r32
                                    
    Stretch-Bend    : Input Angle - function uses distances angle (1-2-3), theta,
                                    and distances (1-2), r12, and (3-2) r32
                                    
    Bend-Bend       : Input Improper - function uses angles (1-2-3), (3-2-4), (4-2-1),
                                        theta123, theta324, theta421 
    
    Stretch-Bend    : Input Dihedral - function uses torsion (1-2-3-4), phi,
      -Torsion                         distances (1-2), (2-3), (3-4),
                                       r12, r23, r34, and angles
                                       (1-2-3) and (2-3-4), theta123, theta234
    
    @author Christopher Woods
*/
class SIREMM_EXPORT GroupInternalParameters
{

friend QDataStream& ::operator<<(QDataStream&, const GroupInternalParameters&);
friend QDataStream& ::operator>>(QDataStream&, GroupInternalParameters&);

public:
    GroupInternalParameters();
    
    GroupInternalParameters(const GroupInternalParameters &other);
    
    ~GroupInternalParameters();
    
    GroupInternalParameters& operator=(const GroupInternalParameters &other);

    bool isEmpty() const;
    
    bool isSingleCutGroup() const;
    bool isDoubleCutGroup() const;
    bool isTripleCutGroup() const;
    bool isQuadrupleCutGroup() const;

    bool refersTo(CGIdx cgidx) const;
    bool refersTo(const QSet<CGIdx> &cgidxs) const;

    const QVector<TwoAtomFunction>& bondParameters() const;
    
    const QVector<ThreeAtomFunction>& angleParameters() const;
    const QVector<FourAtomFunction>& dihedralParameters() const;
    
    const QVector<FourAtomFunction>& improperParameters() const;
    const QVector<TwoAtomFunction>& ureyBradleyParameters() const;
    
    const QVector<ThreeAtomFunction>& stretchStretchParameters() const;
    const QVector<ThreeAtomFunction>& stretchBendParameters() const;
    const QVector<FourAtomFunction>& bendBendParameters() const;
    const QVector<FourAtomFunction>& stretchBendTorsionParameters() const;

    GroupInternalParameters includeOnly(const AtomSelection &selected_atoms) const;

private:
    class GroupInternalParametersData : public QSharedData
    {
    public:
        GroupInternalParametersData();
        GroupInternalParametersData(const GroupInternalParametersData &other);
        
        ~GroupInternalParametersData();
        
        GroupInternalParametersData& operator=(
                                const GroupInternalParametersData &other);

        bool singleCutGroup() const;
        bool twoCutGroups() const;
        bool threeCutGroups() const;
        bool fourCutGroups() const;

        /** The CGIdx IDs of the (up to) four CutGroups that contain
            the atoms referred to in this group. These are arranged
            in numerical order, and will repeat if there are less than
            four CutGroups used (so if only one CutGroup, then
            cgidx3 == cgidx2 == cgidx1 == cgidx0, while if two
            CutGroups, then cgidx3 == cgidx2 == cgidx1 != cgidx0 */
        CGIdx cgidx0, cgidx1, cgidx2, cgidx3;

        /** The array of bond parameters */
        QVector<TwoAtomFunction> bond_params;
    
        /** The array of angle parameters */
        QVector<ThreeAtomFunction> angle_params;
    
        /** The array of dihedral parameters */
        QVector<FourAtomFunction> dihedral_params;
    
        /** The array of improper parameters */
        QVector<FourAtomFunction> improper_params;
    
        /** The array of Urey-Bradley parameters */
        QVector<TwoAtomFunction> ub_params;
    
        /** The array of stretch-stretch parameters */
        QVector<ThreeAtomFunction> stretch_stretch_params;
    
        /** The array of stretch-bend parameters */
        QVector<ThreeAtomFunction> stretch_bend_params;
    
        /** The array of bend-bend parameters */
        QVector<FourAtomFunction> bend_bend_params;
    
        /** The array of stretch-bend-torsion parameters */
        QVector<FourAtomFunction> stretch_bend_torsion_params;
    };

    /** Implicitly shared pointer to the data of this class */
    QSharedDataPointer<GroupInternalParametersData> d;
};

/** This class holds the internal parameters for a molecule
    (bond, angle, dihedral, improper, Urey-Bradley, 
     stretch-stretch, stretch-bend, bend-bend, stretch-bend-torsion)
     
    The parameters are held in groups, that correspond to the 
    set of CutGroups that contain the atoms, e.g.
    
    group 0 contains all of the parameters that act on
    internals that act only within CutGroup 0
    
    group 0,1 contains all of the parameters that act only
    between atoms in groups 0 and 1
    
    group 0,1,2 contains all of the parameters that act only
    between atoms in groups 0, 1 and 2
     
    @author Christopher Woods
*/
class SIREMM_EXPORT InternalParameters
{

friend QDataStream& ::operator<<(QDataStream&, const InternalParameters&);
friend QDataStream& ::operator>>(QDataStream&, InternalParameters&);

public:
    InternalParameters(const PartialMolecule &molecule,
                       const ParameterMap &map);
    
    InternalParameters(const InternalParameters &other);
    
    ~InternalParameters();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<InternalParameters>() );
    }
    
    const char* what() const
    {
        return InternalParameters::typeName();
    }
    
    InternalParameters& operator=(const InternalParameters &other);
    
    bool operator==(const InternalParameters &other) const;
    bool operator!=(const InternalParameters &other) const;
    
    InternalParameters includeOnly(const AtomSelection &selected_atoms) const;
    
    InternalParameters applyMask(const QSet<CGIdx> &cgidxs) const;

    const QVector<GroupInternalParameters>& groupParameters() const;
    
    QVector<GroupInternalParameters> groupParameters(CGIdx cgidx) const;
    QVector<GroupInternalParameters> groupParameters(const QSet<CGIdx> &cgidxs) const;
    
private:
    /** All of the groups of internal parameters */
    QVector<GroupInternalParameters> group_params;
    
    /** The indicies of the groups that contains the parameters
        that involve a particular CutGroup, indexed by the CGIdx
        of that CutGroup */
    QSet< CGIdx, QSet<quint32> > groups_by_cgidx;
};

}

#endif
