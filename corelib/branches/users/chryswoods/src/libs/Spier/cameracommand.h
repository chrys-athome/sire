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

#ifndef SPIER_CAMERACOMMAND_H
#define SPIER_CAMERACOMMAND_H

#include "renderviewcommand.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class CameraCommand;
class RotateCamera;
class TranslateCamera;
}

QDataStream& operator<<(QDataStream&, const Spier::CameraCommand&);
QDataStream& operator>>(QDataStream&, Spier::CameraCommand&);

QDataStream& operator<<(QDataStream&, const Spier::RotateCamera&);
QDataStream& operator>>(QDataStream&, Spier::RotateCamera&);

QDataStream& operator<<(QDataStream&, const Spier::TranslateCamera&);
QDataStream& operator>>(QDataStream&, Spier::TranslateCamera&);

namespace Spier
{

class Camera;

/** This is the base class of all commands that are used
    to manipulate the camera
    
    @author Christopher Woods
*/
class SPIER_EXPORT CameraCommand : public RenderViewCommand
{

friend QDataStream& ::operator<<(QDataStream&, const CameraCommand&);
friend QDataStream& ::operator>>(QDataStream&, CameraCommand&);

public:
    CameraCommand(const QString &description);
    
    CameraCommand(const CameraCommand &other);
    
    ~CameraCommand();
    
    static const char* typeName()
    {
        return "Spier::CameraCommand";
    }

protected:
    CameraCommand& operator=(const CameraCommand &other);
    
    bool operator==(const CameraCommand &other) const;
    bool operator!=(const CameraCommand &other) const;

    const Camera& getCamera() const;

    void setCamera(const Camera &camera) const;
};

/** This command is used to rotate the camera */
class SPIER_EXPORT RotateCamera
         : public SireBase::ConcreteProperty<RotateCamera,CameraCommand>
{

friend QDataStream& ::operator<<(QDataStream&, const RotateCamera&);
friend QDataStream& ::operator>>(QDataStream&, RotateCamera&);

public:
    RotateCamera();
    
    RotateCamera(const SireUnits::Dimension::Angle &rotate_x,
                 const SireUnits::Dimension::Angle &rotate_y,
                 const SireUnits::Dimension::Angle &rotate_z);

    RotateCamera(const RotateCamera &other);
    
    ~RotateCamera();
    
    RotateCamera& operator=(const RotateCamera &other);
    
    bool operator==(const RotateCamera &other) const;
    bool operator!=(const RotateCamera &other) const;
    
    static const char* typeName();

    void undo() const;
    void redo() const;

    CommandPtr mergeWith(const Command &other) const;

protected:
    void execute();

private:
    /** The amount by which to rotate the camera */
    SireUnits::Dimension::Angle delta_rotate[3];
};

/** This command is used to translate the camera */
class SPIER_EXPORT TranslateCamera
         : public SireBase::ConcreteProperty<TranslateCamera,CameraCommand>
{

friend QDataStream& ::operator<<(QDataStream&, const TranslateCamera&);
friend QDataStream& ::operator>>(QDataStream&, TranslateCamera&);

public:
    TranslateCamera();
    
    TranslateCamera(const SireUnits::Dimension::Length &translate_x,
                    const SireUnits::Dimension::Length &translate_y,
                    const SireUnits::Dimension::Length &translate_z);

    TranslateCamera(const TranslateCamera &other);
    
    ~TranslateCamera();
    
    TranslateCamera& operator=(const TranslateCamera &other);
    
    bool operator==(const TranslateCamera &other) const;
    bool operator!=(const TranslateCamera &other) const;
    
    static const char* typeName();

    void undo() const;
    void redo() const;

    CommandPtr mergeWith(const Command &other) const;

protected:
    void execute();

private:
    /** The amounts by which to translate the camera */
    SireUnits::Dimension::Length delta_translate[3];
};

}

Q_DECLARE_METATYPE( Spier::RotateCamera )
Q_DECLARE_METATYPE( Spier::TranslateCamera )

SIRE_EXPOSE_CLASS( Spier::CameraCommand )
SIRE_EXPOSE_CLASS( Spier::RotateCamera )
SIRE_EXPOSE_CLASS( Spier::TranslateCamera )

SIRE_END_HEADER

#endif
