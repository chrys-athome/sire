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

#include "camera.h"
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
    
    ds << static_cast<const RenderViewCommand&>(camcommand);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SPIER_EXPORT &operator>>(QDataStream &ds, CameraCommand &camcommand)
{
    VersionID v = readHeader(ds, r_camcommand);
    
    if (v == 1)
    {
        ds >> static_cast<RenderViewCommand&>(camcommand);
    }
    else
        throw version_error( v, "1", r_camcommand, CODELOC );
        
    return ds;
}

/** Constructor */
CameraCommand::CameraCommand(const QString &description)
              : RenderViewCommand(description)
{}

/** Copy constructor */
CameraCommand::CameraCommand(const CameraCommand &other)
              : RenderViewCommand(other)
{}

/** Destructor */
CameraCommand::~CameraCommand()
{}

/** Copy assignment operator */
CameraCommand& CameraCommand::operator=(const CameraCommand &other)
{
    RenderViewCommand::operator=(other);
    return *this;
}

/** Comparison operator */
bool CameraCommand::operator==(const CameraCommand &other) const
{
    return RenderViewCommand::operator==(other);
}

/** Comparison operator */
bool CameraCommand::operator!=(const CameraCommand &other) const
{
    return RenderViewCommand::operator!=(other);
}

/** Internal function used by the Camera commands to get access to
    the camera in a render view */
const Camera& CameraCommand::getCamera() const
{
    return renderView().getCamera();
}
    
/** Internal function used by the Camera commands to set the camera 
    in a render view */
void CameraCommand::setCamera(const Camera &camera) const
{
    const_cast<CameraCommand*>(this)->renderView().setCamera(camera);
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

/** Empty constructor */
RotateCamera::RotateCamera()
             : ConcreteProperty<RotateCamera,CameraCommand>(
                    QObject::tr("Rotate camera (0, 0, 0)") )
{
    for (int i=0; i<3; ++i)
    {
        delta_rotate[i] = SireUnits::Dimension::Angle(0);
    }
}

static QString degreeString(const SireUnits::Dimension::Angle &angle)
{
    return QString("%1%2").arg( angle.to(degrees) )
                          .arg( QLatin1Char(176) );  //degree is ascii 176
}

/** Construct to rotate the camera by 'rotate_x' around the x-axis,
    'rotate_y' around the y-axis and 'rotate_z' around the z-axis */
RotateCamera::RotateCamera(const SireUnits::Dimension::Angle &rotate_x,
                           const SireUnits::Dimension::Angle &rotate_y,
                           const SireUnits::Dimension::Angle &rotate_z)
             : ConcreteProperty<RotateCamera,CameraCommand>(
                    QObject::tr("Rotate camera (%1, %2, %3)")
                            .arg( ::degreeString(rotate_x),
                                  ::degreeString(rotate_y),
                                  ::degreeString(rotate_z) ) )
{
    delta_rotate[0] = rotate_x;
    delta_rotate[1] = rotate_y;
    delta_rotate[2] = rotate_z;
}

/** Copy constructor */
RotateCamera::RotateCamera(const RotateCamera &other)
             : ConcreteProperty<RotateCamera,CameraCommand>(other)
{
    for (int i=0; i<3; ++i)
    {
        delta_rotate[i] = other.delta_rotate[i];
    }
}

/** Destructor */
RotateCamera::~RotateCamera()
{}

/** Copy assignment operator */
RotateCamera& RotateCamera::operator=(const RotateCamera &other)
{
    if (this != &other)
    {
        RenderViewCommand::operator=(other);
    
        for (int i=0; i<3; ++i)
        {
            delta_rotate[i] = other.delta_rotate[i];
        }
    }
    
    return *this;
}

/** Comparison operator */
bool RotateCamera::operator==(const RotateCamera &other) const
{
    return this == &other or
           ( delta_rotate[0] == other.delta_rotate[0] and
             delta_rotate[1] == other.delta_rotate[1] and
             delta_rotate[2] == other.delta_rotate[2] and
             RenderViewCommand::operator==(other) );
}

/** Comparison operator */
bool RotateCamera::operator!=(const RotateCamera &other) const
{
    return not RotateCamera::operator==(other);
}

const char* RotateCamera::typeName()
{
    return QMetaType::typeName( qMetaTypeId<RotateCamera>() );
}

static void rotateCamera(CameraPtr &camera,
                         const SireUnits::Dimension::Angle *delta)
{
    if ( not (SireMaths::isZero(delta[0]) and SireMaths::isZero(delta[1])) )
    {
        camera.edit().rotate(delta[0], 5*delta[1]);
    }
    
    if ( not SireMaths::isZero(delta[2]) )
    {
        camera.edit().spin(delta[2]);
    }
}

/** Execute this command */
void RotateCamera::execute()
{
    CameraPtr camera = this->getCamera();
    
    ::rotateCamera(camera, delta_rotate);

    this->setCamera(camera);
}

/** Undo this camera command */
void RotateCamera::undo() const
{
    //undo this command
    CameraPtr camera = this->getCamera();

    SireUnits::Dimension::Angle undo_delta[3];
    
    for (int i=0; i<3; ++i)
    {
        undo_delta[i] = -(delta_rotate[i]);
    }
    
    ::rotateCamera(camera, undo_delta);
    
    this->setCamera(camera);
} 

/** Redo this camera command */
void RotateCamera::redo() const
{
    CameraPtr camera = this->getCamera();
    
    ::rotateCamera(camera, delta_rotate);

    this->setCamera(camera);
}

/** Will eventually merge rotations together... */
CommandPtr RotateCamera::mergeWith(const Command &other) const
{
    return CommandPtr();
}

///////////
/////////// Implementation of TranslateCamera
///////////
