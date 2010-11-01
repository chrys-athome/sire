/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREIO_FLEXIBILITYMAKER_H
#define SIREIO_FLEXIBILITYMAKER_H

#include "iobase.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
  class FlexibilityMaker;
}

QDataStream& operator<<(QDataStream&, const SireIO::FlexibilityMaker&);
QDataStream& operator>>(QDataStream&, SireIO::FlexibilityMaker&);

namespace SireMol
{
  class Molecule;
}

namespace SireIO
{  
  /** 
      Internal class used to store the data describing a single flexibility template
      @author Julien Michel
   */
  class FlexibilityTemplate
  {
  public:
    FlexibilityTemplate( const QString &name= " ");

    const QString getName();
    void setRotation(double rotation);
    void setTranslation(double translation);
    void setMaximumVar(int maxvar);
    const double getRotation();
    const double getTranslation();
    const int getMaximumVar();
    void addBond( QStringList words );
    void addAngle(QStringList words );
    void addDihedral(QStringList words );
    const QList< QStringList > getBonds();
    const QList< QStringList > getAngles();
    const QList< QStringList > getDihedrals();

  private:
    QString name;
    double translation;
    double rotation;
    int maxvar;
    QList< QStringList > bonds;
    QList< QStringList > angles;
    QList< QStringList > dihedrals;
  };

  /** This class is used to read templates describing how a molecule will be moved by a MoverMove
      and to create the property "flexibility" for a molecule whose template is available
   @author Julien Michel
  */

  class SIREIO_EXPORT FlexibilityMaker
  {
    friend QDataStream& ::operator<<(QDataStream&, const SireIO::FlexibilityMaker&);
    friend QDataStream& ::operator>>(QDataStream&, SireIO::FlexibilityMaker&);

  public:
    FlexibilityMaker();
    ~FlexibilityMaker();
    
    /** Read the contents of an input file to create a set of ZmatrixResidues */
    void loadTemplates( const QString &templatefile);
    /** Add the property 'z-matrix' to molecule */
    Molecule applyTemplates( Molecule &molecule);
    
    static const char* typeName();
    
    const char* what() const
    {
      return FlexibilityMaker::typeName();
    }
  private:
    /** The flexibility templates, indexed by molecule name*/
    QHash< QString, FlexibilityTemplate > templates; 
  };
}

Q_DECLARE_METATYPE( SireIO::FlexibilityMaker )

SIRE_EXPOSE_CLASS( SireIO::FlexibilityMaker )

SIRE_END_HEADER

#endif
