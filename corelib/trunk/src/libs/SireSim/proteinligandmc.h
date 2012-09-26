/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#ifndef SIRESIM_PROTEINLIGANDMC_H
#define SIRESIM_PROTEINLIGANDMC_H

#include "simparams.h"

#include "SireMove/ensemble.h"
#include "SireMove/moves.h"

#include "SireCAS/symbol.h"

#include "SireSystem/system.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/temperature.h"

namespace SireSim
{
class ProteinLigandMC;
class ProteinLigandMCParams;
}

QDataStream& operator<<(QDataStream&, const SireSim::ProteinLigandMC&);
QDataStream& operator>>(QDataStream&, SireSim::ProteinLigandMC&);

QDataStream& operator<<(QDataStream&, const SireSim::ProteinLigandMCParams&);
QDataStream& operator>>(QDataStream&, SireSim::ProteinLigandMCParams&);

namespace SireSim
{

using SireBase::PropertyPtr;

using SireSystem::System;

using SireMove::Moves;
using SireMove::MovesPtr;
using SireMove::Ensemble;

using SireCAS::Symbol;

using SireUnits::Dimension::Length;
using SireUnits::Dimension::Area;
using SireUnits::Dimension::Angle;
using SireUnits::Dimension::Temperature;
using SireUnits::Dimension::Pressure;

/** This is a simple simulation that runs Monte Carlo on a protein
    ligand system that is loaded from the files produced via
    auto-setup
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT ProteinLigandMC
        : public SireBase::ConcreteProperty<ProteinLigandMC,Sim>
{

friend QDataStream& ::operator<<(QDataStream&, const ProteinLigandMC&);
friend QDataStream& ::operator>>(QDataStream&, ProteinLigandMC&);

public:
    ProteinLigandMC();
    ProteinLigandMC(const ProteinLigandMCParams &params);
    ProteinLigandMC(const QString &s3file);
    
    ProteinLigandMC(const ProteinLigandMC &other);
    
    ~ProteinLigandMC();
    
    static const char* typeName();
    
    ProteinLigandMC& operator=(const ProteinLigandMC &other);
    
    bool operator==(const ProteinLigandMC &other) const;
    bool operator!=(const ProteinLigandMC &other) const;
    
    void run();
    
    /*System system() const;
    MovesPtr moves() const;
    
    void setSystem(const System &system);
    void setMoves(const Moves &moves);*/
};

/** This class provides all of the user-supplied parameters that
    are needed to run a Monte Carlo simulation on a protein-ligand
    system using the ProteinLigandMC class 
    
    @author Christopher Woods
*/
class SIRESIM_EXPORT ProteinLigandMCParams
        : public SireBase::ConcreteProperty<ProteinLigandMCParams,SimParams>
{

friend QDataStream& ::operator<<(QDataStream&, const ProteinLigandMCParams&);
friend QDataStream& ::operator>>(QDataStream&, ProteinLigandMCParams&);

public:
    ProteinLigandMCParams();
    ProteinLigandMCParams(const QString &filename);
    
    ProteinLigandMCParams(const ProteinLigandMCParams &other);
    
    ~ProteinLigandMCParams();
    
    static const char* typeName();
    
    ProteinLigandMCParams& operator=(const ProteinLigandMCParams &other);
    
    bool operator==(const ProteinLigandMCParams &other) const;
    bool operator!=(const ProteinLigandMCParams &other) const;
};

}

Q_DECLARE_METATYPE( SireSim::ProteinLigandMC )
Q_DECLARE_METATYPE( SireSim::ProteinLigandMCParams )

SIRE_EXPOSE_CLASS( SireSim::ProteinLigandMC )
SIRE_EXPOSE_CLASS( SireSim::ProteinLigandMCParams )

#endif
