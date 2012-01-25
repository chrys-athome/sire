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

#include "proteinligandmc.h"

#include "SireIO/amber.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSim;
using namespace SireIO;
using namespace SireBase;
using namespace SireMol;
using namespace SireStream;

////////////
//////////// Implementation of ProteinLigandMCParams
////////////

static const RegisterMetaType<ProteinLigandMCParams> r_params;

QDataStream SIRESIM_EXPORT &operator<<(QDataStream &ds,
                                       const ProteinLigandMCParams &params)
{
    writeHeader(ds, r_params, 0);
    
    ds << static_cast<const SimParams&>(params);
    
    return ds;
}

QDataStream SIRESIM_EXPORT &operator>>(QDataStream &ds,
                                       ProteinLigandMCParams &params)
{
    VersionID v = readHeader(ds, r_params);
    
    if (v == 0)
    {
        ds >> static_cast<SimParams&>(params);
    }
    else
        throw version_error(v, "0", r_params, CODELOC);
        
    return ds;
}

/** Constructor */
ProteinLigandMCParams::ProteinLigandMCParams()
                      : ConcreteProperty<ProteinLigandMCParams,SimParams>()
{}

/** Construct from the passed file. This can either be a text file, or
    it can be a file written by Sire.Stream.save that contains a 
    ProteinLigandMCParams object */
ProteinLigandMCParams::ProteinLigandMCParams(const QString &filename)
                      : ConcreteProperty<ProteinLigandMCParams,SimParams>()
{}

/** Copy constructor */
ProteinLigandMCParams::ProteinLigandMCParams(const ProteinLigandMCParams &other)
                      : ConcreteProperty<ProteinLigandMCParams,SimParams>(other)                
{}

/** Destructor */
ProteinLigandMCParams::~ProteinLigandMCParams()
{}

const char* ProteinLigandMCParams::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ProteinLigandMCParams>() );
}

/** Copy assignment operator */
ProteinLigandMCParams& ProteinLigandMCParams::operator=(
                                        const ProteinLigandMCParams &other)
{
    if (this != &other)
    {
        SimParams::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool ProteinLigandMCParams::operator==(const ProteinLigandMCParams &other) const
{
    return SimParams::operator==(other);
}

/** Comparison operator */
bool ProteinLigandMCParams::operator!=(const ProteinLigandMCParams &other) const
{
    return not ProteinLigandMCParams::operator==(other);
}

////////////
//////////// Implementation of ProteinLigandMC
////////////

static const RegisterMetaType<ProteinLigandMC> r_sim;

QDataStream SIRESIM_EXPORT &operator<<(QDataStream &ds,
                                       const ProteinLigandMC &sim)
{
    writeHeader(ds, r_sim, 0);
    
    ds << static_cast<const Sim&>(sim);
    
    return ds;
}

QDataStream SIRESIM_EXPORT &operator>>(QDataStream &ds, ProteinLigandMC &sim)
{
    VersionID v = readHeader(ds, r_sim);
    
    if (v == 0)
    {
        ds >> static_cast<Sim&>(sim);
    }
    else
        throw version_error(v, "0", r_sim, CODELOC);
        
    return ds;
}

/** Constructor */
ProteinLigandMC::ProteinLigandMC() 
                : ConcreteProperty<ProteinLigandMC,Sim>()
{}

/** Construct, passing in the parameters in 'params' */
ProteinLigandMC::ProteinLigandMC(const ProteinLigandMCParams &params)
                : ConcreteProperty<ProteinLigandMC,Sim>()
{}

/** Construct from a s3 file output by Sire.Stream.save */
ProteinLigandMC::ProteinLigandMC(const QString &s3file)
                : ConcreteProperty<ProteinLigandMC,Sim>()
{}

/** Copy constructor */
ProteinLigandMC::ProteinLigandMC(const ProteinLigandMC &other)
                : ConcreteProperty<ProteinLigandMC,Sim>(other)
{}

/** Destructor */
ProteinLigandMC::~ProteinLigandMC()
{}

const char* ProteinLigandMC::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ProteinLigandMC>() );
}

/** Copy assignment operator */
ProteinLigandMC& ProteinLigandMC::operator=(const ProteinLigandMC &other)
{
    if (this != &other)
    {
        Sim::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool ProteinLigandMC::operator==(const ProteinLigandMC &other) const
{
    return Sim::operator==(other);
}

/** Comparison operator */
bool ProteinLigandMC::operator!=(const ProteinLigandMC &other) const
{
    return not ProteinLigandMC::operator==(other);
}

/** Run one iteration of this simulation */
//void ProteinLigandMC::run()
//{
//}

void ProteinLigandMC::run()
{
   qDebug() << "HELLO";
}
