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

#include "cameracommand.h"
#include "renderview.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"

using namespace Spier;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

///////////
/////////// Implementation of CameraCommand
///////////

static const RegisterMetaType<CameraCommand> r_camcommand( MAGIC_ONLY,
                                                           CameraCommand::typeName() );
                                                           
/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const CameraCommand &camcommand)
{
    writeHeader(ds, r_camcommand, 1);
    
    ds << static_cast<const Command&>(camcommand);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, CameraCommand &camcommand)
{
    VersionID v = readHeader(ds, r_camcommand);
    
    if (v == 1)
    {
        ds >> static_cast<Command&>(camcommand);
    }
    else
        throw version_error( v, "1", r_camcommand, CODELOC );
        
    return ds;
}

/** Constructor */
CameraCommand::CameraCommand(const QString &description)
              : Command(description)
{}

/** Copy constructor */
CameraCommand::CameraCommand(const CameraCommand &other)
              : Command(other)
{}

/** Destructor */
CameraCommand::~CameraCommand()
{}

/** Copy assignment operator */
CameraCommand& CameraCommand::operator=(const CameraCommand &other)
{
    Command::operator=(other);
    return *this;
}

/** Comparison operator */
bool CameraCommand::operator==(const CameraCommand &other) const
{
    return Command::operator==(other);
}

/** Comparison operator */
bool CameraCommand::operator!=(const CameraCommand &other) const
{
    return Command::operator!=(other);
}

///////////
/////////// Implementation of RotateCamera
///////////

static const RegisterMetaType<RotateCamera> r_rotatecam;

/** Serialise to a binary datastream */
QDataStream SPIER_EXPORT &operator<<(QDataStream &ds, const RotateCamera &rotatecam)
{
    writeHeader(ds, r_rotatecam, 1);
    
    for (int i=0; i<3; ++i)
    {
        double ang = rotatecam.delta_rotate[i].to(radians);
        ds << ang;
    }
    
    ds << static_cast<const CameraCommand&>(rotatecam);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, RotateCamera &rotatecam)
{
    VersionID v = readHeader(ds, r_rotatecam);
    
    if (v == 1)
    {
        for (int i=0; i<3; ++i)
        {
            double ang;
            ds >> ang;
            
            rotatecam.delta_rotate[i] = ang * radians;
            
            ds >> static_cast<CameraCommand&>(rotatecam);
        }
    }
    else
        throw version_error( v, "1", r_rotatecam, CODELOC );
        
    return ds;
}

///////////
/////////// Implementation of TranslateCamera
///////////
