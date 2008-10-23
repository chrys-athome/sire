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

#ifndef SQUIRE_QMPROGRAM_H
#define SQUIRE_QMPROGRAM_H

#include "qmpotential.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class QMProgram;
class NullQM;
}

QDataStream& operator<<(QDataStream&, const Squire::QMProgram&);
QDataStream& operator>>(QDataStream&, Squire::QMProgram&);

QDataStream& operator<<(QDataStream&, const Squire::NullQM&);
QDataStream& operator>>(QDataStream&, Squire::NullQM&);

namespace Squire
{

class LatticeCharges;

class QMMMElecEmbedPotential;

/** This is the base class of all QM programs. These are wrappers that
    provide the functionality to calculate QM energies and forces
    by calling separate QM programs
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT QMProgram : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const QMProgram&);
friend QDataStream& ::operator>>(QDataStream&, QMProgram&);

friend class QMPotential;            //so it can call the force and energy functions
friend class QMMMElecEmbedPotential; //so it can call the force and energy functions

public:
    QMProgram();

    QMProgram(const QMProgram &other);
    
    virtual ~QMProgram();
    
    static const char* typeName()
    {
        return "Squire::QMProgram";
    }
    
    virtual QMProgram* clone() const=0;
    
    /** Return whether or not this QM program supports the use
        of point lattice charges (which can polarise the QM wavefunction) */
    virtual bool supportsLatticeCharges() const
    {
        return false;
    }
    
    /** Return whether or not this QM program supports the use
        of gaussian lattice charges (which can polarise the QM wavefunction) */
    virtual bool supportsGaussianCharges() const
    {
        return false;
    }
    
    static const NullQM& null();
    
protected:
    /** Calculate and return the QM energy of all of the molecules
        in 'molecules' */
    virtual double calculateEnergy(const QMPotential::Molecules &molecules) const=0;

    virtual double calculateEnergy(const QMPotential::Molecules &molecules,
                                   const LatticeCharges &lattice_charges) const;
    
    /** Return the contents of the command file that would be used
        to run the QM program to calculate energies */
    virtual QString energyCommandFile(const QMPotential::Molecules &molecules) const=0;

    virtual QString energyCommandFile(const QMPotential::Molecules &molecules,
                                      const LatticeCharges &lattice_charges) const;
    
    /** Return the contents of the command file that would be used
        to run the QM program to calculate forces */
    virtual QString forceCommandFile(const QMPotential::Molecules &molecules) const=0;

    virtual QString forceCommandFile(const QMPotential::Molecules &molecules,
                                     const LatticeCharges &lattice_charges) const;
};

/** This is the null QM program that returns zero energy and force */
class SQUIRE_EXPORT NullQM 
        : public SireBase::ConcreteProperty<NullQM,QMProgram>
{

friend QDataStream& ::operator<<(QDataStream&, const NullQM&);
friend QDataStream& ::operator>>(QDataStream&, NullQM&);

public:
    NullQM();
    
    NullQM(const NullQM &other);
    
    ~NullQM();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullQM>() );
    }
    
    NullQM* clone() const
    {
        return new NullQM(*this);
    }
    
    NullQM& operator=(const NullQM &other);
    
    bool operator==(const NullQM &other) const;
    bool operator!=(const NullQM &other) const;
    
protected:
    double calculateEnergy(const QMPotential::Molecules &molecules) const;
    
    QString energyCommandFile(const QMPotential::Molecules &molecules) const;
    QString forceCommandFile(const QMPotential::Molecules &molecules) const;
};

typedef SireBase::PropPtr<QMProgram> QMProgPtr;

}

Q_DECLARE_METATYPE( Squire::NullQM )

SIRE_EXPOSE_CLASS( Squire::QMProgram )
SIRE_EXPOSE_CLASS( Squire::NullQM )

SIRE_EXPOSE_PROPERTY( Squire::QMProgPtr, Squire::QMProgram )

SIRE_END_HEADER

#endif