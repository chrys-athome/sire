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

#include "twoatomfunctions.h"
#include "threeatomfunctions.h"
#include "fouratomfunctions.h"

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
      -Torsion                         distances (1-2), (2-3), (3-4), (1-4)
                                       r12, r23, r34, r14 and angles
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
    const QVector<TwoAtomFunction>& bondForces() const;
    
    const QVector<ThreeAtomFunction>& angleParameters() const;
    const QVector<ThreeAtomFunction>& angleForces() const;
    
    const QVector<FourAtomFunction>& dihedralParameters() const;
    const QVector<FourAtomFunction>& dihedralForces() const;
    
    const QVector<FourAtomFunction>& improperParameters() const;
    const QVector<FourAtomFunction>& improper_Theta_Forces() const;
    const QVector<FourAtomFunction>& improper_Phi_Forces() const;
    
    const QVector<TwoAtomFunction>& ureyBradleyParameters() const;
    const QVector<TwoAtomFunction>& ureyBradleyForces() const;
    
    const QVector<ThreeAtomFunction>& stretchStretchParameters() const;
    const QVector<ThreeAtomFunction>& stretchStretch_R01_Forces() const;
    const QVector<ThreeAtomFunction>& stretchStretch_R21_Forces() const;
    
    const QVector<ThreeAtomFunction>& stretchBendParameters() const;
    const QVector<ThreeAtomFunction>& stretchBend_Theta_Forces() const;
    const QVector<ThreeAtomFunction>& stretchBend_R01_Forces() const;
    const QVector<ThreeAtomFunction>& stretchBend_R21_Forces() const;
    
    const QVector<FourAtomFunction>& bendBendParameters() const;
    const QVector<FourAtomFunction>& bendBend_Theta123_Forces() const;
    const QVector<FourAtomFunction>& bendBend_Theta324_Forces() const;
    const QVector<FourAtomFunction>& bendBend_Theta421_Forces() const;
    
    const QVector<FourAtomFunction>& stretchBendTorsionParameters() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_Phi_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_R01_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_R12_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_R32_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_R03_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_Theta012_Forces() const;
    const QVector<FourAtomFunction>& stretchBendTorsion_Theta321_Forces() const;

    void setBondParameters(const QVector<TwoAtomFunction> &potential,
                           const QVector<TwoAtomFunction> &forces);
                           
    void setAngleParameters(const QVector<ThreeAtomFunction> &potential,
                            const QVector<ThreeAtomFunction> &forces);
    
    void setDihedralParameters(const QVector<FourAtomFunction> &potential,
                               const QVector<FourAtomFunction> &forces);
    
    void setImproperParameters(const QVector<FourAtomFunction> &potential,
                               const QVector<FourAtomFunction> &theta_forces,
                               const QVector<FourAtomFunction> &phi_forces);
                               
    void setUreyBradleyParameters(const QVector<TwoAtomFunction> &potential,
                                  const QVector<TwoAtomFunction> &forces);
    
    void setStretchStretchParameters(const QVector<ThreeAtomFunction> &ssparams);
    void setStretchBendParameters(const QVector<ThreeAtomFunction> &sbparams);
    void setBendBendParameters(const QVector

    GroupInternalParameters includeOnly(const AtomSelection &selected_atoms) const;

private:
    class GroupInternalCrossParameters : public QSharedData
    {
    public:
        GroupInternalCrossParameters();
        GroupInternalCrossParameters(const GroupInternalCrossParameters &other);
        
        ~GroupInternalCrossParameters();
        
        GroupInternalCrossParameters& operator=(
                                const GroupInternalCrossParameters &other);
    
        /** The array of stretch-stretch parameters */
        QVector<ThreeAtomFunction> stretch_stretch_params;
    
        /** The array of stretch-bend parameters */
        QVector<ThreeAtomFunction> stretch_bend_params;
    
        /** The array of bend-bend parameters */
        QVector<FourAtomFunction> bend_bend_params;
    
        /** The array of stretch-bend-torsion parameters */
        QVector<FourAtomFunction> stretch_bend_torsion_params;
    };

    class GroupInternalParametersData : public QSharedData
    {
    public:
        GroupInternalParametersData();
        GroupInternalParametersData(const GroupInternalParametersData &other);
        
        ~GroupInternalParametersData();
        
        GroupInternalParametersData& operator=(
                                const GroupInternalParametersData &other);

        bool hasCrossTerms() const;

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

        /** The array of bond parameters and forces */
        QVector<TwoAtomFunction> bond_params, bond_forces;
    
        /** The array of angle parameters and forces */
        QVector<ThreeAtomFunction> angle_params, angle_forces;
    
        /** The array of dihedral parameters and forces */
        QVector<FourAtomFunction> dihedral_params, dihedral_forces;
    
        /** The array of improper parameters */
        QVector<FourAtomFunction> improper_params;
    
        /** The improper forces, acting on theta and phi */
        QVector<FourAtomFunction> improper_theta_forces, improper_phi_forces;
    
        /** The array of Urey-Bradley parameters and forces */
        QVector<TwoAtomFunction> ub_params, ub_forces;
        
        /** Shared pointer to the cross-term internal parameters
            (double-pointer so as to save memory, as cross-terms
            tend not to be used with very large molecules) */
        QSharedDataPointer<GroupInternalCrossParameters> cross_terms;
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
    quint32 getIndex(quint32 cgidx0) const;
    quint32 getIndex(quint32 cgidx0, quint32 cgidx1) const;
    quint32 getIndex(quint32 cgidx0, quint32 cgidx1,
                     quint32 cgidx2) const;
    quint32 getIndex(quint32 cgidx0, quint32 cgidx1,
                     quint32 cgidx2, quint32 cgidx3) const;
    
    const GroupInternalParameters& getGroup(quint32 cgidx0) const;
    const GroupInternalParameters& getGroup(quint32 cgidx0, quint32 cgidx1) const;
    const GroupInternalParameters& getGroup(quint32 cgidx0, quint32 cgidx1,
                                            quint32 cgidx2) const;
    const GroupInternalParameters& getGroup(quint32 cgidx0, quint32 cgidx1,
                                            quint32 cgidx2, quint32 cgidx3) const;
    
    GroupInternalParameters& addGroup(quint32 cgidx0);
    GroupInternalParameters& addGroup(quint32 cgidx0, quint32 cgidx1);
    GroupInternalParameters& addGroup(quint32 cgidx0, quint32 cgidx1,
                                      quint32 cgidx2);
    GroupInternalParameters& addGroup(quint32 cgidx0, quint32 cgidx1,
                                      quint32 cgidx2, quint32 cgidx3);
    
    /** All of the groups of internal parameters */
    QVector<GroupInternalParameters> group_params;
    
    /** The indicies of the groups that contains the parameters
        that involve a particular CutGroup, indexed by the CGIdx
        of that CutGroup */
    QSet< CGIdx, QSet<quint32> > groups_by_cgidx;
    
    /** The index of each CutGroup combination */
    QHash<detail::IDQuad,quint32> groups_by_combination;
};

}

#endif
