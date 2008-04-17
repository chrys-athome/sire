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

#ifndef SIREMM_INTERNALFF_H
#define SIREMM_INTERNALFF_H

#include "g1ff.h"
#include "ff3d.h"

#include "internalcomponent.h"

namespace SireMM
{
class InternalFF;
}

QDataStream &operator<<(QDataStream&, const SireMM::InternalFF&);
QDataStream &operator>>(QDataStream&, SireMM::InternalFF&);

namespace SireMM
{

namespace detail
{

/** This class provides the default name of the 
    property that contains the bond parameters */
class SIREMM_EXPORT BondParameterName
{
public:
    BondParameterName()
    {}
    
    ~BondParameterName()
    {}
    
    const QString& bond() const
    {
        return bond_param;
    }
    
private:
    static QString bond_param;
};

/** This class provides the default name of the 
    property that contains the angle parameters */
class SIREMM_EXPORT AngleParameterName
{
public:
    AngleParameterName()
    {}
    
    ~AngleParameterName()
    {}
    
    const QString& angle() const
    {
        return angle_param;
    }
    
private:
    static QString angle_param;
};

/** This class provides the default name of the 
    property that contains the dihedral parameters */
class SIREMM_EXPORT DihedralParameterName
{
public:
    DihedralParameterName()
    {}
    
    ~DihedralParameterName()
    {}
    
    const QString& dihedral() const
    {
        return dihedral_param;
    }
    
private:
    static QString dihedral_param;
};

/** This class provides the default name of the 
    property that contains the Urey-Bradley parameters */
class SIREMM_EXPORT UreyBradleyParameterName
{
public:
    UreyBradleyParameterName()
    {}
    
    ~UreyBradleyParameterName()
    {}
    
    const QString& ureyBradley() const
    {
        return ub_param;
    }
    
private:
    static QString ub_param;
};

/** This class provides the default name of the properties
    that contain the bond, angle, dihedral and Urey-Bradley parameters */
class SIREMM_EXPORT InternalParameterNames 
            : public BondParameterName, public AngleParameterName,
              public DihedralParameterName, public UreyBradleyParameterName
{
public:
    InternalParameterNames() 
            : BondParameterName(), AngleParameterName(),
              DihedralParameterName(), UreyBradleyParameterName()
    {}
    
    ~InternalParameterNames()
    {}
};

/** This class provides the default name of the properties
    that contain the internal and 3D coordinates properties */
class SIREMM_EXPORT InternalParameterNames3D 
                    : public InternalParameterNames,
                      public SireFF::detail::Coords3DParameterName
{
public:
    InternalParameterNames3D() : InternalParameterNames(),
                                 SireFF::detail::Coords3DParameterName()
    {}
    
    ~InternalParameterNames3D()
    {}
};

/** This potential provides energies and forces caused by 
    molecular mechanics style internal intramolecular 
    terms, e.g. bond, angle, dihedral, urey bradley
    
    @author Christopher Woods
*/
class InternalPotential
{
public:
    

    ~InternalPotential();
    
    void calculateEnergy(const InternalPotential::Molecule &molecule,
                         InternalPotential::Energy &energy,
                         double scale_energy=1) const;
    
    void calculateForce(const InternalPotential::Molecule &molecule,
                        MolForceTable &forces,
                        double scale_force=1) const;

    void calculateBondForce(const InternalPotential::Molecule &molecule,
                            MolForceTable &forces,
                            double scale_force=1) const;
                            
    void calculateAngleForce(const InternalPotential::Molecule &molecule,
                             MolForceTable &forces,
                             double scale_force=1) const;
                             
    void calculateDihedralForce(const InternalPotential::Molecule &molecule,
                                MolForceTable &forces,
                                double scale_force=1) const;
                                
    void calculateUBForce(const InternalPotential::Molecule &molecule,
                          MolForceTable &forces,
                          double scale_force=1) const;
                          
    void calculateForce(const InternalPotential::Molecule &molecule,
                        MolForceTable &forces,
                        const Symbol &symbol,
                        const Components &components,
                        double scale_force=1) const;
    
protected:
    InternalPotential();
    InternalPotential(const InternalPotential &other);
    
    InternalPotential& operator=(const InternalPotential &other);
};

}

/** This is a forcefield that calculates the energies and forces
    caused by molecular mechanics style internal intramolecular
    potentials, e.g. bond, angle, dihedral, urey bradley terms
    
    @author Christopher Woods
*/  
class SIREMM_EXPORT InternalFF 
            : public SireBase::ConcreteProperty<InternalFF,G1FF>,
              public FF3D,
              protected InternalPotential
{

friend QDataStream& ::operator<<(QDataStream&, const InternalFF&);
friend QDataStream& ::operator>>(QDataStream&, InternalFF&);

public:
    typedef SireMM::detail::InternalComponents Components;

    InternalFF();
    InternalFF(const QString &name);
    
    InternalFF(const InternalFF &other);
    
    ~InternalFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<InternalFF>() );
    }
    
    const char *what() const
    {
        return InternalFF::typeName();
    }
    
    InternalFF* clone() const
    {
        return new InternalFF(*this);
    }
    
    InternalFF& operator=(const InternalFF &other);
    
    bool operator==(const InternalFF &other) const;
    bool operator!=(const InternalFF &other) const;
    
    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);
               
protected:
    void _pvt_restore(const ForceField &ffield);

    void _pvt_added(const PartialMolecule &mol, const PropertyMap &map);

    void _pvt_removed(const PartialMolecule &mol);

    void _pvt_changed(const SireMol::Molecule &molecule);
    void _pvt_changed(const QList<SireMol::Molecule> &molecules);
    
    void _pvt_removedAll();
        
    bool _pvt_wouldChangeProperties(MolNum molnum, 
                                    const PropertyMap &map) const;

    void _pvt_updateName();

};

}

#endif
