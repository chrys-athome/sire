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

#ifndef SIREMOVE_FLEXIBILITY_H
#define SIREMOVE_FLEXIBILITY_H

#include "SireBase/propertymap.h"
#include "SireMol/molviewproperty.h"
#include "SireUnits/units.h"
#include "movermove.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
  class Flexibility;
}

QDataStream& operator<<(QDataStream &ds, const SireMove::Flexibility&);
QDataStream& operator>>(QDataStream &ds, SireMove::Flexibility&);

namespace SireMol
{
  class BondID;
  class AngleID;
  class DihedralID;
  class Molecule;
  class MoleculeInfoData;
}


namespace SireMove
{
  using SireBase::PropertyMap;
  using SireUnits::Dimension::Length;
  using SireUnits::Dimension::Angle;
  using SireMol::BondID;
  using SireMol::AngleID;
  using SireMol::DihedralID;
  using SireMol::Molecule;
  using SireMol::MoleculeInfoData;

 /** This class holds a the list of bonds, angles and dihedrals of a molecule that 
     can be moved by a MoverMove object, as well as the maximum rotations and 
     translations that are applied to a molecule by a rigid body move object
    
    @author Julien Michel
 */
class SIREMOVE_EXPORT Flexibility
  : public SireBase::ConcreteProperty<Flexibility,SireMol::MoleculeProperty>
    {
      friend QDataStream& ::operator<<(QDataStream&, const Flexibility&);
      friend QDataStream& ::operator>>(QDataStream&, Flexibility&);
      
    public:
      Flexibility();
      Flexibility(const Molecule &molecule);
      Flexibility(const Flexibility &other);

      ~Flexibility();
      
      Flexibility& operator=(const Flexibility &other);
      
      bool operator==(const Flexibility &other) const;
      bool operator!=(const Flexibility &other) const;
      
      const SireMol::MoleculeInfoData& info() const;

      QString toString() const;

      bool isCompatibleWith(const SireMol::MoleculeInfoData &molinfo) const;

      void setRotation(const Angle &rotation);
      void setTranslation(const Length &translation);
      void setMaximumVar(const int &maxvar);

      Length translation() const;
      Angle rotation() const;
      int maximumVar() const;

      void add(const BondID &bond, const Length &delta);
      void add(const AngleID &angle, const Angle &delta);
      void add(const DihedralID &dihedral, const Angle &delta);
      
      void remove(const BondID &bond);
      void remove(const AngleID &angle);
      void remove(const DihedralID &dihedral);
      
      bool contains(const BondID &bond) const;
      bool contains(const AngleID &angle) const;
      bool contains(const DihedralID &dihedral) const;

      void setDelta(const BondID &bond, const Length &delta);
      void setDelta(const AngleID &angle, const Angle &delta);
      void setDelta(const DihedralID &dihedral, const Angle &delta);

      Length delta(const BondID &bond) const;
      Angle delta(const AngleID &angle) const;
      Angle delta(const DihedralID &dihedral) const;
    
      static const char* typeName();

      friend class MoverMove;// to allow direct enumeration of bonds, angles, dihedrals 

    private:
      /** The molecule that this flexibility operates on */
      SireBase::SharedDataPointer<SireMol::MoleculeInfoData> molinfo;
      /** The maximum translation for that molecule */
      Length maxtranslation;
      /** The maximum rotation for that molecule */
      Angle maxrotation;
      /** The maximum number of dofs to sample in one move */
      int maxvar;
      /** The list of bonds that are flexible */
      QList<BondID> bonds;
      /** The list of angles that are flexible */
      QList<AngleID> angles;
      /** The list of dihedrals that are flexible */
      QList<DihedralID> dihedrals;
      /** The list of delta values for bonds*/
      QHash<DofID,SireUnits::Dimension::Length> bond_deltas;
      /** The list of delta values for angle/dihedrals*/
      QHash<DofID,SireUnits::Dimension::Angle> angle_deltas;
    };
}

Q_DECLARE_METATYPE( SireMove::Flexibility )

SIRE_EXPOSE_CLASS( SireMove::Flexibility )

SIRE_END_HEADER

#endif
