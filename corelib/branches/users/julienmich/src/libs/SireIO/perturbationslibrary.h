/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREIO_PERTURBATIONSLIBRARY_H
#define SIREIO_PERTURBATIONSLIBRARY_H

#include "iobase.h"

#include "SireMM/ljparameter.h"
#include "SireMol/molecule.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
class PerturbationsLibrary;
class PerturbationsTemplate;
}

QDataStream& operator<<(QDataStream&, const SireIO::PerturbationsLibrary&);
QDataStream& operator>>(QDataStream&, SireIO::PerturbationsLibrary&);

QDataStream& operator<<(QDataStream&, const SireIO::PerturbationsTemplate&);
QDataStream& operator>>(QDataStream&, SireIO::PerturbationsTemplate&);

namespace SireMM
{
  class LJParameter;
}

namespace SireIO
{  

  //using SireMol::BondID;
  //using SireMol::AngleID;
  //using SireMol::DihedralID;
  using SireMol::Molecule;

  //using SireMove::Flexibility;
  using SireMM::LJParameter;
  using SireUnits::Dimension::Charge;

/** Internal class used to store the data describing a single perturbations template
    
    @author Julien Michel
*/
class SIREIO_EXPORT PerturbationsTemplate
{

friend QDataStream& ::operator<<(QDataStream&, const PerturbationsTemplate&);
friend QDataStream& ::operator>>(QDataStream&, PerturbationsTemplate&);

public:
    PerturbationsTemplate();
    PerturbationsTemplate(const QString &name);
    
    PerturbationsTemplate(const PerturbationsTemplate &other);
    
    ~PerturbationsTemplate();

    static const char* typeName();

    PerturbationsTemplate& operator=(const PerturbationsTemplate &other);

    bool operator==(const PerturbationsTemplate &other) const;
    bool operator!=(const PerturbationsTemplate &other) const;

    const QString getName();
    
    void setInitCharge(const QString &atomname, const Charge &atomcharge);
    void setFinalCharge(const QString &atomname, const Charge &atomcharge);
    void setInitLJ(const QString &atomname, const LJParameter &atomlj);
    void setFinalLJ(const QString &atomname, const LJParameter &atomlj);
    void setInitType(const QString &atomname, const QString &atype);
    void setFinalType(const QString &atomname, const QString &atype);

    Charge getInitCharge(const QString &atomname) const;
    Charge getFinalCharge(const QString &atomname) const;
    LJParameter getInitLJ(const QString &atomname) const;
    LJParameter getFinalLJ(const QString &atomname) const;
    QString getInitType(const QString &atomname) const;
    QString getFinalType(const QString &atomname) const;

private:
    QString name;
    
    // The atom charges 
    QHash<QString,Charge> initcharges;
    QHash<QString,Charge> finalcharges;
    // The atom LJs
    QHash<QString,LJParameter> initLJs;
    QHash<QString,LJParameter> finalLJs;
    // The atom types
    QHash<QString,QString> initatypes;
    QHash<QString,QString> finalatypes;
    // HAS INFO TO MAKE MANY TWOATOMPOTENTIAL PERTS (BONDS)
    // HAS INFO TO MAKE MANY THREEATOMPOTENTIAL PERTS (ANGLES)
    // HAS INTO TO MAKE MANY FOURATOMPOTENTIAL PERTS (DIHEDRALS, IMPROPERS)
};

/** This class is used to read templates describing how a 
    molecule can be perturbed

    @author Julien Michel
*/

class SIREIO_EXPORT PerturbationsLibrary 
        : public SireBase::ConcreteProperty<PerturbationsLibrary,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const SireIO::PerturbationsLibrary&);
friend QDataStream& ::operator>>(QDataStream&, SireIO::PerturbationsLibrary&);

public:
    PerturbationsLibrary();
    PerturbationsLibrary(const QString &file);
    
    PerturbationsLibrary(const PerturbationsLibrary &other);
    
    ~PerturbationsLibrary();
    
    static const char* typeName();
    
    PerturbationsLibrary& operator=(const PerturbationsLibrary &other);

    bool operator==(const PerturbationsLibrary &other) const;
    bool operator!=(const PerturbationsLibrary &other) const;

    void loadTemplates(const QString &file);

    PerturbationsLibrary& operator+=(const PerturbationsLibrary &other);

    PerturbationsLibrary operator+(const PerturbationsLibrary &other) const;
    
    void add(const PerturbationsLibrary &other);
    
    const PerturbationsTemplate& getTemplate(const QString &key);
    
    void setTemplate(const QString &key, const PerturbationsTemplate &tmplate);
    
    Molecule applyTemplate(const Molecule &molecule) const;

private:
    /** The perturbations templates, indexed by molecule name*/
    QHash<QString,PerturbationsTemplate> templates; 
};

} // end of namespace SireIO

Q_DECLARE_METATYPE( SireIO::PerturbationsLibrary )
Q_DECLARE_METATYPE( SireIO::PerturbationsTemplate )

SIRE_EXPOSE_CLASS( SireIO::PerturbationsLibrary )
SIRE_EXPOSE_CLASS( SireIO::PerturbationsTemplate )

SIRE_END_HEADER

#endif
