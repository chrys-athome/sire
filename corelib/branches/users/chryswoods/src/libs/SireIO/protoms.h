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

#ifndef SIREIO_PROTOMS_H
#define SIREIO_PROTOMS_H

#include <QString>
#include <QStringList>

#include "SireBase/properties.h"
#include "SireBase/propertymap.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
class ProtoMS;
}

QDataStream& operator<<(QDataStream&, const SireIO::ProtoMS&);
QDataStream& operator>>(QDataStream&, SireIO::ProtoMS&);

class QTextStream;

namespace SireMM
{
class TwoAtomFunctions;
class ThreeAtomFunctions;
class FourAtomFunctions;
}

namespace SireMol
{
class Molecule;
class Molecules;
class MolEditor;
}

namespace SireMove
{
class ZMatrix;
}

namespace SireBase
{
class TempDir;
}

namespace SireIO
{

using SireBase::PropertyName;
using SireBase::PropertyMap;

using SireMol::Molecule;
using SireMol::Molecules;
using SireMol::MolEditor;

using SireMove::ZMatrix;

/** This class holds all of the source and default values of the 
    properties used by the ProtoMS parameter reader
    
    @author Christopher Woods
*/
class SIREIO_EXPORT ProtoMSParameters
{
public:
    ProtoMSParameters();
    ~ProtoMSParameters();
    
    //// Locations of the properties into which to place data

    /** Return the name of the property that will contain the 
        partial atomic charges
        
        default == "charge"
    */
    const PropertyName& charge() const
    {
        return charge_property;
    }

    /** Return the name of the property that will contain the 
        atomic Lennard Jones parameters
        
        default == "LJ"
    */
    const PropertyName& lj() const
    {
        return lj_property;
    }

    /** Return the name of the property that will contain the 
        bond parameters
        
        default == "bond"
    */
    const PropertyName& bond() const
    {
        return bond_property;
    }

    /** Return the name of the property that will contain the 
        angle parameters
        
        default == "angle"
    */
    const PropertyName& angle() const
    {
        return angle_property;
    }

    /** Return the name of the property that will contain the 
        dihedral parameters
        
        default == "dihedral"
    */
    const PropertyName& dihedral() const
    {
        return dihedral_property;
    }

    /** Return the name of the property that will contain the 
        Urey-Bradley parameters
        
        default == "Urey-Bradley"
    */
    const PropertyName& ureyBradley() const
    {
        return ub_property;
    }

    /** Return the name of the property that will contain the 
        z-matrix
        
        default == "zmatrix"
    */
    const PropertyName& zmatrix() const
    {
        return zmatrix_property;
    }

private:
    ///////
    /////// Properties that hold the data of the molecule
    ///////
    
    /** The default name of the partial charge property */
    static PropertyName charge_property;

    /** The default name of the LJ property */
    static PropertyName lj_property;

    /** The default name of the bond property */
    static PropertyName bond_property;

    /** The default name of the angle property */
    static PropertyName angle_property;

    /** The default name of the dihedral property */
    static PropertyName dihedral_property;

    /** The default name of the Urey-Bradley property */
    static PropertyName ub_property;

    /** The default name of the zmatrix property */
    static PropertyName zmatrix_property;
};

/** This class is used to read in ProtoMS parameter files and
    parameterise passed molecules.
 
    @author Christopher Woods
*/
class SIREIO_EXPORT ProtoMS
{

friend QDataStream& ::operator<<(QDataStream&, const SireIO::ProtoMS&);
friend QDataStream& ::operator>>(QDataStream&, SireIO::ProtoMS&);

public:
    enum { PROTEIN = 1,     // a ProtoMS protein molecule
           SOLUTE  = 2,     // a ProtoMS solute molecule
           SOLVENT = 3  };  // a ProtoMS solvent molecule

    ProtoMS();
    ProtoMS(const QString &protoms);
    
    ~ProtoMS();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ProtoMS>() );
    }

    const char* what() const
    {
        return ProtoMS::typeName();
    }
    
    ProtoMS* clone() const
    {
        return new ProtoMS(*this);
    }

    static const ProtoMSParameters& parameters()
    {
        return protoms_parameters;
    }

    void setExecutable(const QString &protoms);

    void addParameterFile(const QString &paramfile);
    
    QStringList parameterFiles() const;
    
    Molecule parameterise(const Molecule &molecule, int type,
                          const PropertyMap &map = PropertyMap());

    Molecules parameterise(const Molecules &molecules, int type,
                           const PropertyMap &map = PropertyMap());

private:
    QString writeShellFile(const SireBase::TempDir &tempdir,
                           const QString &cmdfile) const;
    QString writeCommandFile(const SireBase::TempDir &tempdir, 
                             const Molecule &molecule, int type) const;
    
    void processZMatrixLine(const QStringList &words, 
                            const Molecule &mol, int type,
                            ZMatrix &zmatrix) const;
                            
    void processAtomLine(const QStringList &words,
                         MolEditor &editmol, int type,
                         const QString &charge_property,
                         const QString &lj_property) const;
    
    void processBondLine(const QStringList &words,
                         const Molecule &molecule, int type,
                         SireMM::TwoAtomFunctions &bondfuncs) const;
    
    void processAngleLine(const QStringList &words,
                          const Molecule &molecule, int type,
                          SireMM::ThreeAtomFunctions &anglefuncs) const;
    
    QString processDihedralLine(QTextStream &ts,
                                const QStringList &words,
                                const Molecule &molecule, int type,
                                SireMM::FourAtomFunctions &dihedralfuncs) const;

    void processBondDeltaLine(const QStringList &words,
                              const Molecule &molecule, int type,
                              ZMatrix &zmatrix) const;

    void processAngleDeltaLine(const QStringList &words,
                               const Molecule &molecule, int type,
                               ZMatrix &zmatrix) const;

    void processDihedralDeltaLine(const QStringList &words,
                                  const Molecule &molecule, int type,
                                  ZMatrix &zmatrix) const;
    
    void processUBLine(const QStringList &words,
                       const Molecule &molecule, int type,
                       SireMM::TwoAtomFunctions &ubfuncs) const;
    
    Molecule runProtoMS(const Molecule &molecule, int type,
                        const PropertyMap &map) const;

    /** The default properties used to store the parameters */
    static ProtoMSParameters protoms_parameters;

    /** The list of parameter files that will be used to 
        parameterise the molecules */
    QStringList paramfiles;
    
    /** The full path to the ProtoMS executable that will
        be used to perform the parameterisation */
    QString protoms_exe;
};

}

Q_DECLARE_METATYPE( SireIO::ProtoMS )

SIRE_EXPOSE_CLASS( SireIO::ProtoMS )

SIRE_END_HEADER

#endif
